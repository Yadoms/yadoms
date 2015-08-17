#pragma once

// Common includes (STL, Boost...) required by yScriptApi
#include <shared/script/yScriptApi/commonIncludes.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4310) // MSVC warning 4310 disabled for Boost::archive
#endif
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>