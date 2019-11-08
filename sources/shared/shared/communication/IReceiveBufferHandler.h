#pragma once
#include "Buffer.hpp"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Receive buffer handler
      ///
      /// This interface manages a buffer, which is filled by a communication port.
      /// An implementation of this interface is responsible to notify
      /// an event handler when a message is considered as valid.
      /// As messages can be received in several parts, the implementation
      /// have to reagregate received data and notify event handler
      /// when a full message is received.
      /// As evaluation of a valid message is dependant of the protocol used,
      /// you probably have to write your implementation.
      //--------------------------------------------------------------
      class IReceiveBufferHandler
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~IReceiveBufferHandler()
         {
         }

         //--------------------------------------------------------------
         /// \brief	                     Push received data in the buffer
         /// \param[in] buffer            Data to push
         /// \note When data is pushed in the buffer, the completion
         /// condition is evaluated, et notification is sent if condition
         /// is OK.
         //--------------------------------------------------------------
         virtual void push(const CByteBuffer& buffer) = 0;

         //--------------------------------------------------------------
         /// \brief	                     Clear the receive buffer content
         //--------------------------------------------------------------
         virtual void flush() = 0;
      };
   }
} // namespace shared::communication
