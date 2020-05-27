#pragma once

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This interface manage assembling of cutted messages
      //--------------------------------------------------------------
      class IMessageAssembler
      {
      public:
         virtual ~IMessageAssembler()
         {
         }

         virtual void appendPart(boost::shared_ptr<const unsigned char[]> message,
                                 size_t messageSize) = 0;

         virtual bool isCompleted() const = 0;

         virtual boost::shared_ptr<const unsigned char[]> message() const = 0;
         virtual size_t messageSize() const = 0;
      };
   }
} // namespace shared::communication


