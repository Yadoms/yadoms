#pragma once
#include "ISynchronousCallback.h"
#include <shared/event/EventHandler.hpp>
#include <shared/Field.hpp>

namespace communication
{
   namespace callback
   {
      //----------------------------------------------
      ///\brief Callback class for sending command results
      //----------------------------------------------
      template <class TCallbackResult>
      class CSynchronousCallback : protected shared::event::CEventHandler, public ISynchronousCallback<TCallbackResult>
      {
      public:
         //----------------------------------------------
         ///\brief Enumeration of events used by this class
         //----------------------------------------------
         enum
         {
            kResult = shared::event::kUserFirstId,
         };


         //----------------------------------------------
         ///\brief The callback result container (nested to be automatically templated)
         //----------------------------------------------
         class CSynchronousResult
         {
         public:
            //----------------------------------------------
            ///\brief The callback result
            //----------------------------------------------
            shared::CField<bool> Success;

            //----------------------------------------------
            ///\brief The error message (in case Success is false)
            //----------------------------------------------
            shared::CField<std::string> ErrorMessage;

            //----------------------------------------------
            ///\brief The result
            //----------------------------------------------
            shared::CField<TCallbackResult> Result;
         };

         //----------------------------------------------
         ///\brief Constructor
         //----------------------------------------------
         CSynchronousCallback()
         {
         }

         //----------------------------------------------
         ///\brief Destructor
         //----------------------------------------------
         virtual ~CSynchronousCallback()
         {
         }

         // ISynchronousCallback implementation
         void sendSuccess(const TCallbackResult& result) override
         {
            m_result.Success = true;
            m_result.ErrorMessage = "";
            m_result.Result = result;
            shared::event::CEventHandler::postEvent<CSynchronousResult>(kResult, m_result);
         }

         void sendError(const std::string& errorMessage) override
         {
            m_result.Success = false;
            m_result.ErrorMessage = errorMessage;
            shared::event::CEventHandler::postEvent<CSynchronousResult>(kResult, m_result);
         }

         // [END] - ISynchronousCallback implementation

         //----------------------------------------------
         ///\brief Wait for the result of a command
         ///\param [in] timeout : The wait time
         ///\return  the command result
         //----------------------------------------------
         int waitForResult(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin)
         {
            return waitForEvents(timeout);
         }

         CSynchronousResult getCallbackResult()
         {
            return getEventData<CSynchronousResult>();
         }


      private:

         //----------------------------------------------
         ///\brief The result
         //----------------------------------------------
         CSynchronousResult m_result;
      };
   }
} // namespace communication::callback


