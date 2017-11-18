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
         class IStartScript
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IStartScript()
            {
            }

            //-----------------------------------------------------
            ///\brief Get the script instance ID
            //-----------------------------------------------------
            virtual int getScriptInstanceId() const = 0;

            //-----------------------------------------------------
            ///\brief Get the script path
            //-----------------------------------------------------
            virtual const boost::filesystem::path& getScriptPath() const = 0;

            //-----------------------------------------------------
            ///\brief Get the script API instance ID
            //-----------------------------------------------------
            virtual const std::string& getScriptApiId() const = 0;

            //-----------------------------------------------------
            ///\brief Get the script log path
            //-----------------------------------------------------
            virtual const boost::filesystem::path& getScriptLogPath() const = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


