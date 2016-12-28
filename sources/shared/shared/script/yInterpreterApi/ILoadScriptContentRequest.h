#pragma once

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //-----------------------------------------------------
         ///\brief Interface for a load script content request
         //-----------------------------------------------------
         class ILoadScriptContentRequest
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~ILoadScriptContentRequest()
            {
            }

            //-----------------------------------------------------
            ///\brief Get the script path
            //-----------------------------------------------------
            virtual const std::string& getScriptPath() = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with success, providing the result
            ///\param [in] result  The query result
            //-----------------------------------------------------
            virtual void sendSuccess(const std::string& content) = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with error
            ///\param [in] errorMessage  The error message
            //-----------------------------------------------------
            virtual void sendError(const std::string& errorMessage) = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


