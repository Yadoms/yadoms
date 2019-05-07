#pragma once
#include "IMegatecUpsConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CMegatecUpsConfiguration : public IMegatecUpsConfiguration
{
public:
   virtual ~CMegatecUpsConfiguration();

   // IMegatecUpsConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getSerialPort() const override;
   bool upsBeepEnable() const override;
   bool autotestEnable() const override;
   double outputShutdownDelay() const override;
   double outputRestoreDelay() const override;
   // [END] IMegatecUpsConfiguration implementation

private:
   yApi::YPluginConfiguration m_data;
};
