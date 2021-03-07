#pragma once

////////////////////////////////////////
// Standard Template Library
//
// ! please keep for full list alphabetic ordered !!
////////////////////////////////////////
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <type_traits>


////////////////////////////////////////
// Boost libraries
//
// ! please keep for full list alphabetic ordered !!
////////////////////////////////////////
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/preprocessor.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/regex.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/barrier.hpp>

// Don't include boost/thread.hpp because it increase build files more than 10MB for nothing and can be too heavy for small targets (raspberry)
#include <boost/thread/thread.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <boost/tuple/tuple.hpp>

