#pragma once

#include "IInformation.h"
#include <shared/plugin/information/IInformation.h>

namespace pluginSystem
{
   namespace serializers
   {
      class CInformation :public IInformation
      {
      public:
         explicit CInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information);

         virtual ~CInformation();

         // IInformationSerializer implementation
         void toPb(plugin_IPC::toPlugin::Information* pb) const override;
         // [END] IInformationSerializer implementation

      private:
         boost::shared_ptr<const shared::plugin::information::IInformation> m_information;
      };
   }
} // namespace pluginSystem::serializers
