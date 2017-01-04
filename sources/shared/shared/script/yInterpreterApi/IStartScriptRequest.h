#pragma once

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //-----------------------------------------------------
         ///\brief Interface for interpreter to start script
         //-----------------------------------------------------
         class IStartScriptRequest
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IStartScriptRequest()
            {
            }

            //-----------------------------------------------------
            ///\brief Get the script IP instance ID
            //-----------------------------------------------------
            virtual int getScriptInstanceId() = 0;

            //-----------------------------------------------------
            ///\brief Get the script path
            //-----------------------------------------------------
            virtual const std::string& getScriptPath() = 0;

            //-----------------------------------------------------
            ///\brief Get the script API instance ID
            //-----------------------------------------------------
            virtual const std::string& getScriptApiId() = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with success, providing the script ID
            ///\param [in] scriptId  Free string, will be passed by Yadoms to interpreter at stop request
            //-----------------------------------------------------
            virtual void sendSuccess(const std::string& scriptProcessId) = 0;//TODO virer les retours ? Ils peuvent être fait par notification de Yadoms par l'interpréteur

            //-----------------------------------------------------
            ///\brief Answer the request with error
            ///\param [in] errorMessage  The error message
            //-----------------------------------------------------
            virtual void sendError(const std::string& errorMessage) = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


