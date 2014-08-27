#pragma once
#include <shared/event/EventHandler.hpp>
#include "Result.h"

namespace communication { namespace command {

   //----------------------------------------------
   ///\brief Callback class for sending command results
   //----------------------------------------------
   class CCallback : protected shared::event::CEventHandler
   {
   public:
      //----------------------------------------------
      ///\brief Constructor
      //----------------------------------------------
      CCallback();

      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~CCallback();

      //----------------------------------------------
      ///\brief Send the command result
      ///\param [in] result : The command result to send
      //----------------------------------------------
      void sendResult(CResult & result);

      //----------------------------------------------
      ///\brief Wait for the result of a command
      ///\param [in] timeout : The wait time
      ///\return  the command result
      //----------------------------------------------
      CResult waitForResult(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin);

   private:
      //----------------------------------------------
      ///\brief Enumeration of events used by this class
      //----------------------------------------------
      enum
      {
         kResult = shared::event::kUserFirstId,
      };

   };

} //namespace command
} //namespace communication
