// All includes needed by plugins to compile with yPluginApi

////////////////////////////////////////
// Standard Template Library
////////////////////////////////////////


////////////////////////////////////////
// Boost libraries
//
// ! please keep for full list alphabetic ordered !!
////////////////////////////////////////
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/preprocessor.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/utility/enable_if.hpp>

//dont't include boost/thread.hpp besause it increase build files more than 10MB for nothing and can be too heavy for small targets (raspberry)
#include <boost/thread/thread.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/thread/recursive_mutex.hpp>

