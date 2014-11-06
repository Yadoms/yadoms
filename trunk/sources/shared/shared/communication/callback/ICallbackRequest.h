#pragma once
#include "ISynchronousCallback.h"

namespace shared { namespace communication { namespace callback
{
   //--------------------------------------------------------------
   /// \brief	Interface for CallbackRequest
   /// \template  TEventData        the type of data provided
   /// \template  TCallbackResult   the type of object excpected in in callback answer
   //--------------------------------------------------------------
   template<class TEventData, class TCallbackResult>
   class ICallbackRequest : public ISynchronousCallback< TCallbackResult >
   {
   public:

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ICallbackRequest()
      {
      }

      // ISynchronousCallback implementation
      virtual void sendSuccess(TCallbackResult & result) = 0;
      virtual void sendError(const std::string & errorMessage) = 0;
      // [END] - ISynchronousCallback implementation

      //--------------------------------------------------------------
      /// \brief	Get the data
      /// \return	The data
      //--------------------------------------------------------------
      virtual const TEventData & getData() const = 0;
   };


} } } // namespace shared::communication::callback
