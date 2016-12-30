#pragma once
#include "ICallbackRequest.h"
#include <shared/Log.h>

namespace communication
{
   namespace callback
   {
      //----------------------------------------------
      ///\brief Callback request (handle data and callback)
      ///\template TEventData       Type of data
      ///\template TCallbackResult  Type of callback result object
      //----------------------------------------------
      template <class TEventData, class TCallbackResult>
      class CCallbackRequest : public ICallbackRequest<TEventData, TCallbackResult>
      {
      public:
         //----------------------------------------------
         ///\brief Constructor
         ///\template data          The data
         ///\template realCallback  The callback
         //----------------------------------------------
         CCallbackRequest(const TEventData& data, ISynchronousCallback<TCallbackResult>& realCallback)
            : m_bResultSent(false), m_realCallback(realCallback), m_data(data)
         {
         }

         //----------------------------------------------
         ///\brief Destructor. If result is not sent, throw exception
         //----------------------------------------------
         virtual ~CCallbackRequest()
         {
            if (!m_bResultSent)
            {
               //notify the waiting objet
               m_realCallback.sendError("The target did not answer the callback");

               //generate a log message in target: -> it must be fixed
               //not throw exception (never throw in destructors)
               YADOMS_LOG(error) << "The callback MUST be called !";
            }
         }

         // shared::communication::callback::ICallbackRequest implementation
         void sendSuccess(const TCallbackResult& result) override
         {
            m_realCallback.sendSuccess(result);
            m_bResultSent = true;
         }

         void sendError(const std::string& errorMessage) override
         {
            m_realCallback.sendError(errorMessage);
            m_bResultSent = true;
         }

         const TEventData& getData() const override
         {
            return m_data;
         }

         // [END] - shared::communication::callback::ICallbackRequest implementation

      private:
         //----------------------------------------------
         ///\brief Current callback used state or not 
         ///       true : the callback has been called
         ///       false : the callback has NOT been called
         //----------------------------------------------
         bool m_bResultSent;

         //----------------------------------------------
         ///\brief The inner callback
         //----------------------------------------------
         ISynchronousCallback<TCallbackResult>& m_realCallback;

         //----------------------------------------------
         ///\brief The data
         //----------------------------------------------
         const TEventData& m_data;
      };
   }
} // namespace communication::callback


