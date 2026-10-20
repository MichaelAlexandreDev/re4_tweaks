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

#ifdef DECOMP_HARNESS
unsigned int decomp_va(unsigned int va);
#define DECOMP_GLOBAL(type, name, va) \
    static type& name = *reinterpret_cast<type*>(decomp_va(va))
#define DECOMP_GLOBAL_ARRAY(type, name, count, va) \
    static type (&name)[count] = *reinterpret_cast<type(*)[count]>(decomp_va(va))
#define DECOMP_FUNC(ret, name, params, va) \
    static ret (__cdecl* const name) params = reinterpret_cast<ret(__cdecl*) params>(decomp_va(va))
#else
#define DECOMP_GLOBAL(type, name, va) extern type name
#define DECOMP_GLOBAL_ARRAY(type, name, count, va) extern type name[count]
#define DECOMP_FUNC(ret, name, params, va) ret name params
#endif

#endif
