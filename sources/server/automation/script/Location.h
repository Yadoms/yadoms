#pragma once
#include "ILocation.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The location helper
   //-----------------------------------------------------
   class CLocation : public ILocation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CLocation();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CLocation();

   protected:
      // ILocation Implementation
      virtual double latitude() const;
      virtual double longitude() const;
      virtual double altitude() const;
      // [END] ILocation Implementation
   };

} } // namespace automation::script
	
	