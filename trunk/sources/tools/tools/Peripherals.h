//
// Peripherals.h
//
// Platforms peripherals management (serial ports...)
//
#pragma once


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) 
#  include "../windows/tools/Peripherals.h" 
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 
#  include "../linux/tools/Peripherals.h" 
#elif defined(MAC)   // TODO : vérifier la condition, j'ai mis au pif (JMD... ;-) )
#  include "../mac/tools/Peripherals.h" 
#else 
#  error "Platform not supported." //TODO : voir comment ça se passe sous MacOs (si on peut utiliser l'implémentation linux ou pas)
#endif 

