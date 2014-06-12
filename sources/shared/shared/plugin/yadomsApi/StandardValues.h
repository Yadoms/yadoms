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
   };

} } } // namespace shared::plugin::yadomsApi

