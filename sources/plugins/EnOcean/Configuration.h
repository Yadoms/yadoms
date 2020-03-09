#pragma once

#include "IConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CConfiguration : public IConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CConfiguration();

   // IConfiguration implementation
   void initializeWith(const shared::CDataContainerSharedPtr & data) override;
   std::string getSerialPort() const override;
   CPairingHelper::EPairingMode getPairingMode() const override;
   // [END] IConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_data;
};

