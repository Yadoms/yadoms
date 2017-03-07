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
         class IStopScript
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IStopScript()
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


