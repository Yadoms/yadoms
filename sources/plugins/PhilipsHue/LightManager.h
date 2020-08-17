#pragma once
#include "UrlManager.h"
#include "ILightManager.h"

class CLightManager : public ILightManager
{
public:
   CLightManager(boost::shared_ptr<CUrlManager>& urlManager);
   virtual ~CLightManager() = default;

   std::map<int, HueLightInformations> getAllLights() override;

   HueLightInformations getLightAttributesAndState(int id) override;

   void setLightId(std::string& lightName, std::map<int, HueLightInformations>& detectedLights) override;

   void lightOn() override;

   void lightOff() override;

private:
   boost::shared_ptr<CUrlManager>& m_urlManager;
   int m_lightId;

   static void setHueLightInformationsState(HueLightInformations& hueLightAttributesAndState,
                                            boost::shared_ptr<shared::CDataContainer>& response);
   static void setHueLightInformationsSwUpdate(HueLightInformations& hueLightAttributesAndState,
                                               boost::shared_ptr<shared::CDataContainer>& response);

   static void setHueLightInformationsCapabilities(HueLightInformations& hueLightAttributesAndState,
                                                   boost::shared_ptr<shared::CDataContainer>& response);

   static void setHueLightInformationsConfig(HueLightInformations& hueLightAttributesAndState,
                                             boost::shared_ptr<shared::CDataContainer>& response);

   static void setLightState(const std::string& lightUrl, shared::CDataContainer& body);
};
