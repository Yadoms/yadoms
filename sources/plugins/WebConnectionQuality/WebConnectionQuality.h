#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "WebConnectionQualityConfiguration.h"

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
   void doMeasure(boost::shared_ptr<yApi::IYPluginApi> api);

private:
   CWebConnectionQualityConfiguration m_configuration;
   boost::shared_ptr<const yApi::historization::CDuration> m_pingKw;
   boost::shared_ptr<const specificHistorizer::CNetworkBandwith> m_uploadKw;
   boost::shared_ptr<const specificHistorizer::CNetworkBandwith> m_downloadKw;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
};
