#pragma once

// Included only by DecompHarness.cpp, after the Windows/game/log headers.
// Keeping implementation here avoids changing the shared VS project file.
#include <atomic>
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")

namespace bio4
{
    namespace
    {
        struct ShadowRuntime
        {
            void* original = nullptr;
            std::atomic<unsigned long long> compared{0};
            std::atomic<unsigned long long> divergent{0};
            std::atomic<unsigned long long> skipped{0};
        };
        ShadowRuntime ShadowStates[kMaxEntries];
        std::atomic_flag ShadowBusy = ATOMIC_FLAG_INIT;
        bool ShadowKeepOriginal = false;
    }

    void* decomp_shadow_original(unsigned int index) { return ShadowStates[index].original; }
    bool decomp_shadow_keep_original() { return ShadowKeepOriginal; }
    void decomp_shadow_leave() { ShadowBusy.clear(std::memory_order_release); }

    void decomp_shadow_skip(unsigned int index, const char* reason)
    {
        const auto count = ++ShadowStates[index].skipped;
        if (shadow::checkpoint(count))
            spd::log()->info("Decomp shadow: {} skipped={} ({})", kDecompEntries[index].name, count, reason);
    }

    bool decomp_shadow_enter(unsigned int index)
    {
        // Never run overlapping snapshot transactions. Only the two audited
        // read-only leaf originals can be used on this fallback path.
        if (ShadowBusy.test_and_set(std::memory_order_acquire))
        {
            decomp_shadow_skip(index, "nested or concurrent call; original only");
            return false;
        }
        return true;
    }

    void decomp_shadow_finish(unsigned int index, const shadow::Difference& diff)
    {
        auto& state = ShadowStates[index];
        const auto count = ++state.compared;
        if (diff.region && ++state.divergent == 1)
            spd::log()->error("Decomp shadow: {} first divergence compared={} region={} offset={:#x} "
                "original={:02X} decompiled={:02X}", kDecompEntries[index].name,
                count, diff.region, diff.offset, diff.original, diff.decompiled);
        if (shadow::checkpoint(count))
            spd::log()->info("Decomp shadow: {} compared={} divergent={} skipped={} keep={}",
                kDecompEntries[index].name, count, state.divergent.load(), state.skipped.load(),
                ShadowKeepOriginal ? "original" : "decompiled");
    }

    // This digest is only required when shadow is requested. The existing
    // replacement-only installation path remains unchanged.
    bool decomp_shadow_verify_executable(const char* expected)
    {
        wchar_t path[32768];
        const DWORD length = GetModuleFileNameW(nullptr, path, static_cast<DWORD>(sizeof(path) / sizeof(*path)));
        if (!length || length >= sizeof(path) / sizeof(*path))
            return false;
        HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file == INVALID_HANDLE_VALUE)
            return false;
        HCRYPTPROV provider = 0;
        HCRYPTHASH hash = 0;
        bool ok = CryptAcquireContextW(&provider, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT) != FALSE;
        if (ok)
            ok = CryptCreateHash(provider, CALG_SHA_256, 0, 0, &hash) != FALSE;
        unsigned char buffer[16384];
        DWORD read = 0;
        while (ok)
        {
            if (!ReadFile(file, buffer, sizeof(buffer), &read, nullptr)) { ok = false; break; }
            if (!read) break;
            ok = CryptHashData(hash, buffer, read, 0) != FALSE;
        }
        unsigned char digest[32] = {};
        DWORD size = sizeof(digest);
        if (ok)
            ok = CryptGetHashParam(hash, HP_HASHVAL, digest, &size, 0) != FALSE && size == sizeof(digest);
        char hex[65] = {};
        constexpr char digits[] = "0123456789abcdef";
        for (unsigned int i = 0; i < sizeof(digest); ++i)
        {
            hex[2 * i] = digits[digest[i] >> 4];
            hex[2 * i + 1] = digits[digest[i] & 15];
        }
        if (hash) CryptDestroyHash(hash);
        if (provider) CryptReleaseContext(provider, 0);
        CloseHandle(file);
        return ok && std::strcmp(hex, expected) == 0;
    }

    bool decomp_shadow_install(unsigned int index, unsigned char* address)
    {
        const auto& entry = kDecompEntries[index];
        const auto size = entry.shadowStolenBytes;
        // These are audited complete instruction prefixes with no relative
        // branches. Never extend this list by guessing a prologue length.
        if (!entry.shadowReplacement ||
            !((entry.va == 0x006AD4D0 && size == 6) || (entry.va == 0x006AD570 && size == 7)))
            return false;
        static_assert(sizeof(void*) == 4, "shadow trampoline requires Win32 x86");
        auto* trampoline = static_cast<unsigned char*>(VirtualAlloc(nullptr, size + 5,
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
        if (!trampoline) return false;
        // Copy the live prefix, including the ASLR-relocated immediate in
        // pullEmModule; do not copy bytes from the on-disk executable.
        std::memcpy(trampoline, address, size);
        trampoline[size] = 0xE9;
        const auto back = reinterpret_cast<uintptr_t>(address + size) -
            reinterpret_cast<uintptr_t>(trampoline + size + 5);
        std::memcpy(trampoline + size + 1, &back, 4);
        DWORD previous = 0;
        if (!VirtualProtect(trampoline, size + 5, PAGE_EXECUTE_READ, &previous) ||
            !FlushInstructionCache(GetCurrentProcess(), trampoline, size + 5))
        {
            VirtualFree(trampoline, 0, MEM_RELEASE);
            return false;
        }
        if (!VirtualProtect(address, 5, PAGE_EXECUTE_READWRITE, &previous))
        {
            VirtualFree(trampoline, 0, MEM_RELEASE);
            return false;
        }
        // Installation occurs during harness initialization, before gameplay,
        // like the pre-existing MakeJMP replacement path. No hot reloading.
        ShadowStates[index].original = trampoline;
        unsigned char patch[5] = { 0xE9 };
        const auto destination = reinterpret_cast<uintptr_t>(entry.shadowReplacement) -
            reinterpret_cast<uintptr_t>(address + 5);
        std::memcpy(patch + 1, &destination, 4);
        std::memcpy(address, patch, sizeof(patch));
        DWORD ignored = 0;
        // Flush before restoring protection so either failure can still roll
        // back while the entry is writable. Never advertise a partial install.
        if (!FlushInstructionCache(GetCurrentProcess(), address, 5) ||
            !VirtualProtect(address, 5, previous, &ignored))
        {
            std::memcpy(address, trampoline, 5);
            const bool flushed = FlushInstructionCache(GetCurrentProcess(), address, 5) != FALSE;
            const bool protectedAgain = VirtualProtect(address, 5, previous, &ignored) != FALSE;
            // Keep the published trampoline alive: a failed rollback flush
            // must never leave a stale JMP reaching a null/freed original.
            spd::log()->error("Decomp shadow: {} installation rolled back (protect={}, flush={})",
                entry.name, protectedAgain, flushed);
            return false;
        }
        // Keep the trampoline for process lifetime, just like the entry hook.
        return true;
    }
}
