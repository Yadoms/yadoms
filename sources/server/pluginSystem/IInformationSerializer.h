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

      virtual boost::shared_ptr<pbPluginInformation::Information> toPb() const = 0;
   };

} // namespace pluginSystem
