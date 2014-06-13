#pragma once
#include <shared/Export.h>


namespace shared { namespace plugin { namespace yadomsApi
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
                                                   //        value      |    Apply on...        |     Description
      static const std::string& On;                // on                | switchOnOff           |
      static const std::string& Off;               // off               | switchOnOff           |
      static const std::string& Open;              // open              | curtain               |
      static const std::string& Close;             // close             | curtain               |
      static const std::string& Stop;              // stop              | curtain               |
   };

} } } // namespace shared::plugin::yadomsApi

