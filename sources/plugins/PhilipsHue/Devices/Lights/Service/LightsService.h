#pragma once

#include "../../Utils/HueLightsResponseType.h"
#include "../../../Entities/HueState.h"
#include "../../../Entities/HueSwUpdate.h"
#include "../../../Entities/HueCapabilities.h"
#include "../../../Entities/HueConfig.h"
#include "../../../UrlManager.h"
#include "../../Interfaces/ILightsService.h"

#include <boost/asio/io_context.hpp>

class CLightsService : public ILightsService
{
public:
   explicit CLightsService(boost::shared_ptr<CUrlManager>& urlManager);
   virtual ~CLightsService() = default;

   std::map<int, CHueLightInformations> getAllLights() override;

   CHueLightInformations getLightAttributesAndState(int id) override;

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

   void setNewLights(const boost::system::error_code& errorCode);
   void startReadingNewLights();
   void closeReadingNewLights();

   boost::shared_ptr<CUrlManager>& m_urlManager;
   std::map<int, CHueLightInformations> m_newLights;

   boost::asio::io_context m_io;

   enum { kSearchNewLightsTimeout = 40 };
};
