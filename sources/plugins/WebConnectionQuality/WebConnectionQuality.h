#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "WebConnectionQualityConfiguration.h"
#include "shared/process/Process.h"
#include "specificHistorizers/NetworkBandwidthHistorizer.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

class CWebConnectionQuality final : public plugin_cpp_api::IPlugin
{
public:
   CWebConnectionQuality();
   ~CWebConnectionQuality() = default;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   void declareDevice(const boost::shared_ptr<yApi::IYPluginApi>& api) const;
   void startMeasure(const boost::shared_ptr<yApi::IYPluginApi>& api);
   void processResult(const boost::shared_ptr<yApi::IYPluginApi>& api,
                      const std::string& result) const;

private:
   CWebConnectionQualityConfiguration m_configuration;
   boost::shared_ptr<yApi::historization::CSwitch> m_connectedKw;
   boost::shared_ptr<yApi::historization::CDuration> m_pingKw;
   boost::shared_ptr<specificHistorizers::CNetworkBandwidthHistorizer> m_uploadKw;
   boost::shared_ptr<specificHistorizers::CNetworkBandwidthHistorizer> m_downloadKw;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   boost::shared_ptr<shared::process::CProcess> m_speedTestProcess;
};
