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
            shared::CField<bool> success;

            //----------------------------------------------
            ///\brief The error message (in case success is false)
            //----------------------------------------------
            shared::CField<std::string> errorMessage;

            //----------------------------------------------
            ///\brief The result
            //----------------------------------------------
            shared::CField<TCallbackResult> result;
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
            m_result.success = true;
            m_result.errorMessage = "";
            m_result.result = result;
            CEventHandler::postEvent<CSynchronousResult>(kResult, m_result);
         }

         void sendError(const std::string& errorMessage) override
         {
            m_result.success = false;
            m_result.errorMessage = errorMessage;
            CEventHandler::postEvent<CSynchronousResult>(kResult, m_result);
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


