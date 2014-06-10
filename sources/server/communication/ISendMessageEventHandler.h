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
      ///\brief                     Send a command
      ///\param [in] deviceId       Target device Id
      ///\param [in] keywordId      Target keyword Id
      ///\param [in] body           The command body to send
      //----------------------------------------------
      virtual void sendCommandAsync(int deviceId, int keywordId, const std::string& body) = 0;
   };

} //namespace communication