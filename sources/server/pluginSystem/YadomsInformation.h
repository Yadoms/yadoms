#pragma once
#include <shared/plugin/information/IYadomsInformation.h>

namespace pluginSystem
{
   class CYadomsInformation : public shared::plugin::information::IYadomsInformation
   {
   public:
      explicit CYadomsInformation(boost::shared_ptr<const shared::ILocation> locationProvider,
                                  const shared::versioning::CSemVer& yadomsVersion);

      virtual ~CYadomsInformation();

      // shared::plugin::information::IYadomsInformation implementation
      bool developperMode() const override;
      shared::versioning::CSemVer version() const override;
      boost::shared_ptr<const shared::ILocation> location() const override;
      // [END] shared::plugin::information::IYadomsInformation implementation

   private:
      const boost::shared_ptr<const shared::ILocation> m_locationProvider;
      const shared::versioning::CSemVer m_yadomsVersion;
   };
} // namespace pluginSystem
