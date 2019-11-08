#pragma once

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Part of a SmallHeaderMessageAssembler message
      //--------------------------------------------------------------
      class IMessageAssemblerPart
      {
      public:
         virtual ~IMessageAssemblerPart(boost::shared_ptr<const unsigned char[]> message,
                                        size_t messageSize)
         {
         }

         virtual bool isFirst() const = 0;
         virtual bool isLast() const = 0;
         virtual unsigned int partCount() const = 0;
      };
   }
} // namespace shared::communication


