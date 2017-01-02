#pragma once

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //-----------------------------------------------------
         ///\brief Interface for interpreter to stop script
         //-----------------------------------------------------
         class IStopScriptRequest
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IStopScriptRequest()
            {
            }

            //-----------------------------------------------------
            ///\brief Call to say Yadoms that script is stopped
            ///\return The script ID, passed by interpreter at script start
            /// (can be a process ID, or anything else identifying script instance)
            //-----------------------------------------------------
            virtual std::string getScriptProcessId() const = 0;

            //-----------------------------------------------------
            ///\brief Call to say Yadoms that script is stopped
            //-----------------------------------------------------
            virtual void sendSuccess() = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with error
            ///\param [in] errorMessage  The error message
            //-----------------------------------------------------
            virtual void sendError(const std::string& errorMessage) = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


