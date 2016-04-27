#pragma once


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

         virtual void toPb(pbBin::Information* pb) const = 0;
      };
   }
} // namespace pluginSystem::serializers
