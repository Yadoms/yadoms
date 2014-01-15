#pragma once

#include <ctime>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/thread.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "../HardwarePluginImplementationHelper.h"

#include <string>
#include <boost/thread/future.hpp>
#include "tools/Xpl/XplMessage.h"
#include "tools/Xpl/XplService.h"

class CFakePlugin : public IHardwarePlugin
{  
public:
   CFakePlugin();

   virtual ~CFakePlugin();

   // IHardwarePlugin implementation
   virtual void doWork(const std::string& configurationValues);
   virtual void updateConfiguration(const std::string& configurationValues);
  // [END] IHardwarePlugin implementation

   // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
   void init(const std::string & pluginAddress, const std::string & serverAddress);

private:
   boost::optional<boost::shared_ptr<const CHardwarePluginConfiguration> > getUpdatedConfiguration();   // TODO à mettre dans une macro DECLARE_CONFIGURATION
   
   // TODO à mettre dans une macro DECLARE_CONFIGURATION
   std::queue<boost::shared_ptr<const CHardwarePluginConfiguration> > m_ConfigurationUpdateQueue;
   boost::mutex m_configurationUpdateMutex;

   void onMessageReceived(CXplMessage & message);
};



