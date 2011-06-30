#ifndef __OS_H__
#define __OS_H__

#ifdef Q_OS_WIN
#define OS_NAME "win"
#define OS_LIB_EXT "dll"
#elif defined(Q_OS_MAC)
#define OS_NAME "osx"
#define OS_LIB_EXT "dylib"
#else
#define OS_NAME "nix"
#define OS_LIB_EXT "so"
#endif

#endif