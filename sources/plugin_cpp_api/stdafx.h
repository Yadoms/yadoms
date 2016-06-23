#pragma once
// All includes needed by plugins to compile with plugin_cpp_api


////////////////////////////////////////
// Standard Template Library
////////////////////////////////////////
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>


////////////////////////////////////////
// Boost libraries
//
// ! please keep for full list alphabetic ordered !!
////////////////////////////////////////
#include <boost/bind.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/preprocessor.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>

//dont't include boost/thread.hpp besause it increase build files more than 10MB for nothing and can be too heavy for small targets (raspberry)
#include <boost/thread/barrier.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/thread/recursive_mutex.hpp>


////////////////////////////////////////
// Poco libraries
//
// ! please keep for full list alphabetic ordered !!
////////////////////////////////////////
#include <Poco/Util/AbstractConfiguration.h>