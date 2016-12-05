#include "stdafx.h"
#include "YadomsInformation.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include "Version.h"


namespace pluginSystem
{
   CYadomsInformation::CYadomsInformation(const boost::shared_ptr<const shared::ILocation> locationProvider)
      : m_locationProvider(locationProvider)
   {
   }

   CYadomsInformation::~CYadomsInformation()
   {
   }

   bool CYadomsInformation::developperMode() const
   {
      return shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>()->getDeveloperMode();
   }

   std::string CYadomsInformation::version() const
   {
      return YadomsVersion;
   }

   shared::versioning::EReleaseType CYadomsInformation::releaseType() const
   {
      return YadomsReleaseType;
   }

   boost::shared_ptr<const shared::ILocation> CYadomsInformation::location() const
   {
      return m_locationProvider;
   }
} // namespace pluginSystem


