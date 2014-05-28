#pragma once

#define EXPORT_LIBRARY_FUNCTION  extern "C"


#if (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    /* GNU GCC/G++. --------------------------------------------- */
    #ifdef yadoms_shared_EXPORTS
    #define YADOMS_SHARED_EXPORT __attribute__ ((dllexport))
    #else
    #define YADOMS_SHARED_EXPORT __attribute__ ((dllimport))
    #endif
#endif

#if (defined(__clang__))
    //for CLang do not need to export symbols
    #define YADOMS_SHARED_EXPORT
#endif