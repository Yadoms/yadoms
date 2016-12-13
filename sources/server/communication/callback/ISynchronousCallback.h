#pragma once

namespace communication
{
   namespace callback
   {
      //--------------------------------------------------------------
      /// \brief	Interface for SynchronousCallback
      /// \template  TCallbackResult   the type of object excpected in callback answer 
      //--------------------------------------------------------------
      template <class TCallbackResult>
      class ISynchronousCallback
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~ISynchronousCallback()
         {
         };

         //----------------------------------------------
         ///\brief Answer the callback with success
         ///\param [in] result : The callback result
         //----------------------------------------------      
         virtual void sendSuccess(const TCallbackResult& result) = 0;

         //----------------------------------------------
         ///\brief Answer the callback with an error
         ///\param [in] errorMessage : The error message
         //----------------------------------------------      
         virtual void sendError(const std::string& errorMessage) = 0;
      };
   }
} // namespace communication::callback


