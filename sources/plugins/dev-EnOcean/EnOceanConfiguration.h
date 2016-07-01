#pragma once

#include "IEnOceanConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CEnOceanConfiguration : public IEnOceanConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CEnOceanConfiguration();

   // IEnOceanConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getSerialPort() const override;
   // [END] IEnOceanConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_data;
};

