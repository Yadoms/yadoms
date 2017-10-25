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
            ///\brief Call to say Yadoms that script is stopped
            ///\return The script instance ID
            //-----------------------------------------------------
            virtual int getScriptInstanceId() const = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


