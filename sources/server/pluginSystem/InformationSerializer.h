#pragma once

#include "IInformationSerializer.h"
#include <shared/plugin/information/IInformation.h>

namespace pluginSystem
{
   class CInformationSerializer :public IInformationSerializer
   {
   public:
      explicit CInformationSerializer(boost::shared_ptr<const shared::plugin::information::IInformation> information);

      virtual ~CInformationSerializer();

      // IInformationSerializer implementation
      void toPb(pbPluginInformation::Information* pb) const override;
      // [END] IInformationSerializer implementation

   private:
      boost::shared_ptr<const shared::plugin::information::IInformation> m_information;
   };

} // namespace pluginSystem
