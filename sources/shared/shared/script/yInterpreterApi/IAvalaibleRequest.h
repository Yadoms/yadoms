#pragma once

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //-----------------------------------------------------
         ///\brief Interface for the interpreter avalaiblity request
         //-----------------------------------------------------
         class IAvalaibleRequest
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IAvalaibleRequest()
            {
            }

            //-----------------------------------------------------
            ///\brief Answer the request with success, providing the interpreter avalaiblity state
            ///\param [in] avalaible  True is interpreter is avalaible and works
            //-----------------------------------------------------
            virtual void sendSuccess(bool avalaible) = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with error
            ///\param [in] errorMessage  The error message
            //-----------------------------------------------------
            virtual void sendError(const std::string& errorMessage) = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


