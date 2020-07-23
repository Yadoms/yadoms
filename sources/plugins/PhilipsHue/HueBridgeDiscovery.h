#pragma once
#include "IHueBridgeDiscovery.h"
#include "UrlManager.h"
#include "Hue.h"

class CHueBridgeDiscovery : public IHueBridgeDiscovery
{
public:
   CHueBridgeDiscovery(boost::shared_ptr<CUrlManager>& urlManager);
   CHueBridgeDiscovery();
   virtual ~CHueBridgeDiscovery() = default;

   std::vector<HueInformations> FindBridges() override;
   HueInformations getHueInformations() override;

private:
   static std::string getIpAddress(const std::string& urlBase);
   boost::shared_ptr<CUrlManager> m_urlManager;
};
