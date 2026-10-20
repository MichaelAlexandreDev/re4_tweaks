#ifndef BIND_H
#define BIND_H

// Binds decomp code to game globals and functions in two builds:
//
// - matching build (decomp/tools/objcmp.py, MSVC 2010): plain extern
//   declarations, so the object references symbols like the original and
//   objcmp checks each relocation against the VA given here;
// - harness build (re4_tweaks, DECOMP_HARNESS): references and function
//   pointers into the running bio4.exe, relocated from its 0x00400000
//   image base because the executable is ASLR-enabled.
//
// Headers under decomp/ must not include system headers: the harness build
// compiles them inside namespace bio4.

#define DECOMP_UNPAREN(...) __VA_ARGS__

#ifdef DECOMP_HARNESS
unsigned int decomp_va(unsigned int va);
#define DECOMP_GLOBAL(type, name, va) \
    static type& name = *reinterpret_cast<type*>(decomp_va(va))
#define DECOMP_GLOBAL_ARRAY(type, name, count, va) \
    static type (&name)[count] = *reinterpret_cast<type(*)[count]>(decomp_va(va))
#define DECOMP_FUNC(ret, name, params, va) \
    static ret (__cdecl* const name) params = reinterpret_cast<ret(__cdecl*) params>(decomp_va(va))
// __thiscall member of a game class: called as __fastcall with an unused EDX,
// which passes `this` in ECX, the arguments on the stack and lets the callee
// clean them, exactly like __thiscall.
#define DECOMP_METHOD(ret, cls, name, params, args, va) \
    ret name params { \
        typedef ret (__fastcall* fn_t)(cls*, int, DECOMP_UNPAREN params); \
        return reinterpret_cast<fn_t>(decomp_va(va))(this, 0, DECOMP_UNPAREN args); \
    }
#define DECOMP_METHOD0(ret, cls, name, va) \
    ret name() { \
        typedef ret (__fastcall* fn_t)(cls*, int); \
        return reinterpret_cast<fn_t>(decomp_va(va))(this, 0); \
    }
#else
#define DECOMP_GLOBAL(type, name, va) extern type name
#define DECOMP_GLOBAL_ARRAY(type, name, count, va) extern type name[count]
#define DECOMP_FUNC(ret, name, params, va) ret name params
#define DECOMP_METHOD(ret, cls, name, params, args, va) ret name params
#define DECOMP_METHOD0(ret, cls, name, va) ret name()
#endif

#endif
