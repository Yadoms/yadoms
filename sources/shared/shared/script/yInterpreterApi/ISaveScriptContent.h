#pragma once

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //-----------------------------------------------------
         ///\brief Interface for a save script content operation
         //-----------------------------------------------------
         class ISaveScriptContent
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~ISaveScriptContent()
            {
            }

            //-----------------------------------------------------
            ///\brief Get the script path
            //-----------------------------------------------------
            virtual const std::string& getScriptPath() = 0;

            //-----------------------------------------------------
            ///\brief Get the script content
            //-----------------------------------------------------
            virtual const std::string& getScriptContent() = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


