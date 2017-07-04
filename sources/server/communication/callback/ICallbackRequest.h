#pragma once
#include "ISynchronousCallback.h"

namespace communication
{
   namespace callback
   {
      //--------------------------------------------------------------
      /// \brief	Interface for CallbackRequest
      /// \template  TEventData        the type of data provided
      /// \template  TCallbackResult   the type of expected object in in callback answer
      //--------------------------------------------------------------
      template <class TEventData, class TCallbackResult>
      class ICallbackRequest : public ISynchronousCallback<TCallbackResult>
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~ICallbackRequest()
         {
         }

         // ISynchronousCallback implementation
         void sendSuccess(const TCallbackResult& result) override = 0;
         void sendError(const std::string& errorMessage) override = 0;
         // [END] - ISynchronousCallback implementation

         //--------------------------------------------------------------
         /// \brief	Get the data
         /// \return	The data
         //--------------------------------------------------------------
         virtual const TEventData& getData() const = 0;
      };
   }
} // namespace communication::callback


