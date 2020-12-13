#pragma once
#include "UrlManager.h"
#include "ILightManager.h"
#include "HueLightsResponseType.h"

class CLightManager : public ILightManager
{
public:
   explicit CLightManager(boost::shared_ptr<CUrlManager>& urlManager);
   virtual ~CLightManager() = default;

   std::map<int, CHueLightInformations> getAllLights() override;

   CHueLightInformations getLightAttributesAndState(int id) override;

   void setLightId(std::string& lightName, std::map<int, CHueLightInformations>& detectedLights) override;

   void lightOn() override;

   void lightOff() override;

   void setLightColorUsingXy(const std::string& hexRgb) override;

   void searchForNewLights() override;

   std::map<int, CHueLightInformations> getNewLights() override;
private:

   static CHueState getHueLightInformationsState(boost::shared_ptr<shared::CDataContainer>& response);
   static CHueSwUpdate getHueLightInformationsSwUpdate(boost::shared_ptr<shared::CDataContainer>& response);
   static CHueCapabilities getHueLightInformationsCapabilities(boost::shared_ptr<shared::CDataContainer>& response);
   static CHueConfig getHueLightInformationsConfig(boost::shared_ptr<shared::CDataContainer>& response);

   static CHueState getHueLightInformationsStateById(int& lightId, boost::shared_ptr<shared::CDataContainer>& response);
   static CHueSwUpdate getHueLightInformationsSwUpdateById(int& lightId,
                                                           boost::shared_ptr<shared::CDataContainer>& response);
   static CHueCapabilities getHueLightInformationsCapabilitiesById(int& lightId,
                                                                   boost::shared_ptr<shared::CDataContainer>& response);
   static CHueConfig getHueLightInformationsConfigById(int& lightId,
                                                       boost::shared_ptr<shared::CDataContainer>& response);

   static void setLightState(const std::string& lightUrl, shared::CDataContainer& body);

   void setNewLights(const boost::system::error_code& errorCode);
   void startReadingNewLights();
   void closeReadingNewLights();

   boost::shared_ptr<CUrlManager>& m_urlManager;
   int m_lightId;
   std::map<int, CHueLightInformations> m_newLights;
   boost::asio::io_service m_ios;

   enum { kSearchNewLightsTimeout = 40 };
};
