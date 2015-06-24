#pragma once

//For mac, only extern C is needed
#define EXPORT_LIBRARY_FUNCTION  extern "C"

//for CLang and G++ do not need to export symbols
#define YADOMS_SHARED_EXPORT

