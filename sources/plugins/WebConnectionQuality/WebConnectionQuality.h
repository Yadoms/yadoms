#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "WebConnectionQualityConfiguration.h"
#include "shared/process/Process.h"
#include "specificHistorizers/NetworkBandwithHistorizer.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

class CWebConnectionQuality : public plugin_cpp_api::IPlugin
{
public:
   CWebConnectionQuality();
   virtual ~CWebConnectionQuality();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;
   void startMeasure(boost::shared_ptr<yApi::IYPluginApi> api);
   void processResult(boost::shared_ptr<yApi::IYPluginApi> api) const;

private:
   CWebConnectionQualityConfiguration m_configuration;
   boost::shared_ptr<yApi::historization::CDuration> m_pingKw;
   boost::shared_ptr<specificHistorizers::CNetworkBandwithHistorizer> m_uploadKw;
   boost::shared_ptr<specificHistorizers::CNetworkBandwithHistorizer> m_downloadKw;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   boost::shared_ptr<shared::process::CProcess> m_speedTestProcess;
};
