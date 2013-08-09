#ifndef _KISS_COMPAT_HPP_
#define _KISS_COMPAT_HPP_

#ifdef _MSC_VER

#pragma section(".CRT$XCU", read)
#define INITIALIZER(f) \
   static void __cdecl f(void); \
   __declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f; \
   static void __cdecl f(void)

#else

#define INITIALIZER(f) \
	static void f(void) __attribute__((constructor)); \
	static void f(void)

#endif

#endif