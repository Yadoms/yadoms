#pragma once

#include <pluginInformation.pb.h>


namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief		Serializer of IInformation
   //--------------------------------------------------------------
   class IInformationSerializer
   {
   public:
      virtual ~IInformationSerializer() {}

      virtual void toPb(pbPluginInformation::Information* pb) const = 0;
   };

} // namespace pluginSystem
