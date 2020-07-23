#pragma once
#include "UrlManager.h"
#include "ILightManager.h"

class CLightManager : public ILightManager
{
public:
   CLightManager(boost::shared_ptr<CUrlManager>& urlManager);
   virtual ~CLightManager() = default;

   std::vector<HueLightInformations> getAllLights() override;
private:
   boost::shared_ptr<CUrlManager>& m_urlManager;
};

