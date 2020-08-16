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

private:
   boost::shared_ptr<CUrlManager>& m_urlManager;

   static void setHueLightInformationsState(HueLightInformations& hueLightAttributesAndState,
                                            boost::shared_ptr<shared::CDataContainer>& response);
   static void setHueLightInformationsSwUpdate(HueLightInformations& hueLightAttributesAndState,
                                               boost::shared_ptr<shared::CDataContainer>& response);

   static void setHueLightInformationsCapabilities(HueLightInformations& hueLightAttributesAndState,
                                                   boost::shared_ptr<shared::CDataContainer>& response);

   static void setHueLightInformationsConfig(HueLightInformations& hueLightAttributesAndState,
                                             boost::shared_ptr<shared::CDataContainer>& response);
};
