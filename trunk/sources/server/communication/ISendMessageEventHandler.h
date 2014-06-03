#pragma once
#include "command/DeviceCommand.h"


namespace communication {

   //----------------------------------------------
   ///\brief Interface used to send a command
   //----------------------------------------------
   class ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~ISendMessageAsync()
      {
      }

      //----------------------------------------------
      ///\brief Send a command
      ///\param [in] message the message to send
      //----------------------------------------------
      virtual void sendCommandAsync(const command::CDeviceCommand & message) = 0;
   };

} //namespace communication