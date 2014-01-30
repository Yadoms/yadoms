#pragma once

//For windows
#define EXPORT_LIBRARY_FUNCTION extern "C" __declspec(dllexport)


#ifdef yadoms_shared_EXPORTS
#define YADOMS_SHARED_EXPORT __declspec(dllexport)
#else
#define YADOMS_SHARED_EXPORT __declspec(dllimport)
#endif
