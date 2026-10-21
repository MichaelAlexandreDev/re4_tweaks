#pragma once

#include <cstddef>
#include <cstring>
#include <type_traits>

// Portable comparison core. Descriptors must cover every effect of both
// callees; snapshots cannot undo allocation, I/O, callbacks or other threads.
namespace bio4::shadow
{
    struct Difference
    {
        const char* region = nullptr; // nullptr means equal; "return" is EAX
        std::size_t offset = 0;
        unsigned char original = 0;
        unsigned char decompiled = 0;
    };

    class Snapshot
    {
        static constexpr std::size_t kMaxRegions = 8;
        static constexpr std::size_t kMaxBytes = 1024;
        struct Region
        {
            const char* name;
            unsigned char* address;
            std::size_t size;
            std::size_t start;
        };
        Region regions_[kMaxRegions] = {};
        unsigned char input_[kMaxBytes] = {};
        unsigned char original_[kMaxBytes] = {};
        std::size_t count_ = 0;
        std::size_t bytes_ = 0;

    public:
        // Fixed storage: no allocation or game-heap calls inside the wrapper.
        bool add(const char* name, void* address, std::size_t size)
        {
            if (!name || !address || !size || count_ == kMaxRegions || size > kMaxBytes - bytes_)
                return false;
            regions_[count_++] = { name, static_cast<unsigned char*>(address), size, bytes_ };
            bytes_ += size;
            return true;
        }

        void save_input()
        {
            for (std::size_t i = 0; i < count_; ++i)
                std::memcpy(input_ + regions_[i].start, regions_[i].address, regions_[i].size);
        }

        void save_original()
        {
            for (std::size_t i = 0; i < count_; ++i)
                std::memcpy(original_ + regions_[i].start, regions_[i].address, regions_[i].size);
        }

        void restore_input() { restore(input_); }
        void restore_original() { restore(original_); }

        Difference difference() const
        {
            for (std::size_t i = 0; i < count_; ++i)
            {
                const auto& r = regions_[i];
                for (std::size_t offset = 0; offset < r.size; ++offset)
                    if (original_[r.start + offset] != r.address[offset])
                        return { r.name, offset, original_[r.start + offset], r.address[offset] };
            }
            return {};
        }

    private:
        void restore(const unsigned char* data)
        {
            for (std::size_t i = 0; i < count_; ++i)
                std::memcpy(regions_[i].address, data + regions_[i].start, regions_[i].size);
        }
    };

    template<class T> struct Result
    {
        T value;
        Difference difference;
    };

    template<class Original, class Decompiled>
    auto compare(Snapshot& state, bool keepOriginal, Original original, Decompiled decompiled)
    {
        using T = decltype(original());
        // Only scalar return ABIs are described in this first version. Do not
        // byte-compare structs with padding, floats with tolerances, or sret.
        static_assert(std::is_integral_v<T> || std::is_pointer_v<T>, "scalar return required");
        static_assert(std::is_same_v<T, decltype(decompiled())>, "return ABI mismatch");
        state.save_input();
        const T a = original();
        state.save_original();
        state.restore_input();
        const T b = decompiled();
        Difference diff;
        const auto* aBytes = reinterpret_cast<const unsigned char*>(&a);
        const auto* bBytes = reinterpret_cast<const unsigned char*>(&b);
        for (std::size_t i = 0; i < sizeof(T); ++i)
            if (aBytes[i] != bBytes[i])
            {
                diff = { "return", i, aBytes[i], bBytes[i] };
                break;
            }
        if (!diff.region)
            diff = state.difference();
        if (keepOriginal)
            state.restore_original();
        return Result<T>{ keepOriginal ? a : b, diff };
    }

    // First comparison and exponentially spaced checkpoints thereafter. The
    // counters always advance, even when no checkpoint line is emitted.
    inline bool checkpoint(unsigned long long count)
    {
        return count == 1 || (count >= 1024 && (count & (count - 1)) == 0);
    }
}
