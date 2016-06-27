#pragma once

#include <pluginInit.pb.h>


namespace pluginSystem
{
   namespace serializers
   {
      //--------------------------------------------------------------
      /// \brief		Serializer of IInformation
      //--------------------------------------------------------------
      class IInformation
      {
      public:
         virtual ~IInformation()
         {
         }

         virtual void toPb(toPlugin::Information* pb) const = 0;
      };
   }
} // namespace pluginSystem::serializers
