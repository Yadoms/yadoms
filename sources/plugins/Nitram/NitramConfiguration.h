#pragma once

#include "INitramConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CNitramConfiguration : public INitramConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CNitramConfiguration();

   // INitramConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getSerialPort() const override;
   boost::posix_time::time_duration outputShutdownDelay() const override;
   boost::posix_time::time_duration outputRestoreDelay() const override;
   bool outputRestoreDelayActivated() const override;
   // [END] INitramConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_data;
};

