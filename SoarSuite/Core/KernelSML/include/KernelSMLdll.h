#ifndef KERNELSMLDLL_H
#define KERNELSMLDLL_H

#ifndef unused
#define unused(x) (void)(x)
#endif

/* Taken from SWIG output code, because they know what they are doing */
#if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#  ifndef GCC_HASCLASSVISIBILITY
#    define GCC_HASCLASSVISIBILITY
#  endif
#endif

#ifndef EXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define EXPORT
#   else
#     define EXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define EXPORT __attribute__ ((visibility("default")))
#   else
#     define EXPORT
#   endif
# endif
#endif

#endif // KERNELSMLDLL
