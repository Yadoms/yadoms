#pragma once

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //-----------------------------------------------------
         ///\brief Interface for interpreter to purge script logs
         //-----------------------------------------------------
         class IPurgeScriptLog
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IPurgeScriptLog()
            {
            }

            //-----------------------------------------------------
            ///\brief Get the script instance ID (useful when script is running)
            ///\return The script instance ID
            //-----------------------------------------------------
            virtual int getScriptInstanceId() const = 0;

            //-----------------------------------------------------
            ///\brief Get the script log path (useful when script is not running)
            ///\return The script log path
            //-----------------------------------------------------
            virtual const boost::filesystem::path& getScriptLogPath() const = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


