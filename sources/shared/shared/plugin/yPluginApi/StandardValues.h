#pragma once
#include <shared/Export.h>


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //--------------------------------------------------------------
         /// \brief		Standard values
         //--------------------------------------------------------------
         class YADOMS_SHARED_EXPORT CStandardValues
         {
         public:
            //--------------------------------------------------------------
            /// \brief		Standard capacities
            //--------------------------------------------------------------
            //        value                              |    Apply on...        |     Description
            static const std::string On; // on           | switch                |
            static const std::string Off; // off         | switch                |
            static const std::string Dim; // dim         | switch                |
            static const std::string Open; // open       | curtain               |
            static const std::string Close; // close     | curtain               |
            static const std::string Stop; // stop       | curtain               |
         };
      }
   }
} // namespace shared::plugin::yPluginApi


