#pragma once
#include "UrlManager.h"
#include "ILightManager.h"

class CLightManager : public ILightManager
{
public:
   explicit CLightManager(boost::shared_ptr<CUrlManager>& urlManager);
   virtual ~CLightManager() = default;

   std::map<int, HueLightInformations> getAllLights() override;

   HueLightInformations getLightAttributesAndState(int id) override;

   void setLightId(std::string& lightName, std::map<int, HueLightInformations>& detectedLights) override;

   void lightOn() override;

   void lightOff() override;

   void setLightColorUsingXy(const std::string& hexRgb) override;

   void searchForNewLights() override;

   std::map<int, HueLightInformations> getNewLights() override;
private:

   static void setHueLightInformationsState(HueLightInformations& hueLightAttributesAndState,
                                            boost::shared_ptr<shared::CDataContainer>& response);
   static void setHueLightInformationsSwUpdate(HueLightInformations& hueLightAttributesAndState,
                                               boost::shared_ptr<shared::CDataContainer>& response);

   static void setHueLightInformationsCapabilities(HueLightInformations& hueLightAttributesAndState,
                                                   boost::shared_ptr<shared::CDataContainer>& response);

   static void setHueLightInformationsConfig(HueLightInformations& hueLightAttributesAndState,
                                             boost::shared_ptr<shared::CDataContainer>& response);

   static void setLightState(const std::string& lightUrl, shared::CDataContainer& body);

   void setNewLights(const boost::system::error_code& errorCode);
   void startReadingNewLights();
   void closeReadingNewLights();

   boost::shared_ptr<CUrlManager>& m_urlManager;
   int m_lightId;
   std::map<int, HueLightInformations> m_newLights;
   boost::asio::io_service m_ios;

   enum { kSearchNewLightsTimeout = 40 };
};
