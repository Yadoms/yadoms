#include "stdafx.h"
#include "YadomsInformation.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"


namespace pluginSystem
{
   CYadomsInformation::CYadomsInformation(const boost::shared_ptr<const shared::ILocation> locationProvider,
                                          const shared::versioning::CSemVer& yadomsVersion)
      : m_locationProvider(locationProvider),
        m_yadomsVersion(yadomsVersion)
   {
   }

   CYadomsInformation::~CYadomsInformation()
   {
   }

   bool CYadomsInformation::developperMode() const
   {
      return shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>()->getDeveloperMode();
   }

   shared::versioning::CSemVer CYadomsInformation::version() const
   {
      return m_yadomsVersion;
   }

   boost::shared_ptr<const shared::ILocation> CYadomsInformation::location() const
   {
      return m_locationProvider;
   }
} // namespace pluginSystem
