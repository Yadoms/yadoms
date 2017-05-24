#pragma once
#include "ICallbackRequest.h"
#include <shared/Log.h>

namespace communication
{
   namespace callback
   {
      typedef int DummyType;  // Define a dummy type to hide data

      //----------------------------------------------
      ///\brief Callback request (handle data and callback)
      ///\template TCallbackResult  Type of callback result object
      //----------------------------------------------
      template <class TCallbackResult>
      class CNoDataCallbackRequest : public ICallbackRequest<DummyType, TCallbackResult>
      {
      public:
         //----------------------------------------------
         ///\brief Constructor
         ///\template data          The data
         ///\template realCallback  The callback
         //----------------------------------------------
         explicit CNoDataCallbackRequest(ISynchronousCallback<TCallbackResult>& realCallback)
            : m_bResultSent(false), m_realCallback(realCallback)
         {
         }

         //----------------------------------------------
         ///\brief Destructor. If result is not sent, throw exception
         //----------------------------------------------
         virtual ~CNoDataCallbackRequest()
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
         // [END] - shared::communication::callback::ICallbackRequest implementation

         // shared::communication::callback::ICallbackRequest implementation
         const DummyType& getData() const override
         {
            static const auto dummyValue = 0;
            return dummyValue;
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
      };
   }
} // namespace communication::callback


