#pragma once
#include "IMegatecUpsConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CMegatecUpsConfiguration final : public IMegatecUpsConfiguration
{
public:
   ~CMegatecUpsConfiguration() override = default;

   // IMegatecUpsConfiguration implementation
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   std::string getSerialPort() const override;
   bool upsBeepEnable() const override;
   bool autotestEnable() const override;
   boost::posix_time::time_duration autotestStartTime() const override;
   double outputShutdownDelay() const override;
   double outputRestoreDelay() const override;
   // [END] IMegatecUpsConfiguration implementation

private:
   yApi::YPluginConfiguration m_data;
};
