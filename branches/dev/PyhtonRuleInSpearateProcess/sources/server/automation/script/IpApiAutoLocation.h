#pragma once
#include "IAutoLocation.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The auto-location via ip-api service (http://ip-api.com/)
   //-----------------------------------------------------
   class CIpApiAutoLocation : public IAutoLocation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CIpApiAutoLocation();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CIpApiAutoLocation();

   protected:
      // IAutoLocation Implementation
      virtual shared::CDataContainer tryAutoLocate() const;
      // [END] IAutoLocation Implementation
   };

} } // namespace automation::script
	
	