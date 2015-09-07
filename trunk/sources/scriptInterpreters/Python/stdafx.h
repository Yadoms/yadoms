#pragma once

// Common includes (STL, Boost...) required by yScriptApi
#include <shared/script/yScriptApi/commonIncludes.h>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread/barrier.hpp>

#include <Poco/Environment.h>
#include <Poco/PipeStream.h>
#include <Poco/Process.h>
#include <Poco/StreamCopier.h>
#include <fstream>