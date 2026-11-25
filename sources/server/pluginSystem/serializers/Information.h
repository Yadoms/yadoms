#pragma once

#include "IInformation.h"
#include <shared/plugin/information/IInformation.h>

namespace pluginSystem
{
   namespace serializers
   {
      class CInformation final : public IInformation
      {
      public:
         explicit CInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information);

         ~CInformation() override = default;

         // IInformationSerializer implementation
         void toPb(plugin_IPC::toPlugin::Information* pb) const override;
         // [END] IInformationSerializer implementation

      private:
         boost::shared_ptr<const shared::plugin::information::IInformation> m_information;
      };
   }
} // namespace pluginSystem::serializers
