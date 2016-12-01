#pragma once
#include <shared/plugin/information/IYadomsInformation.h>

namespace pluginSystem
{
   class CYadomsInformation :public shared::plugin::information::IYadomsInformation
   {
   public:
      explicit CYadomsInformation(const boost::shared_ptr<const shared::ILocation> locationProvider);

      virtual ~CYadomsInformation();

      // shared::plugin::information::IYadomsInformation implementation
      bool developperMode() const override;
      std::string version() const override;
      shared::versioning::EReleaseType releaseType() const override;
      boost::shared_ptr<const shared::ILocation> location() const override;
      // [END] shared::plugin::information::IYadomsInformation implementation

   private:
      const boost::shared_ptr<const shared::ILocation> m_locationProvider;
   };
} // namespace pluginSystem


