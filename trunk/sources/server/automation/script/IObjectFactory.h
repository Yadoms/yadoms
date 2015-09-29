//
// Object factory
//
#pragma once
#include "IAutoLocation.h"
#include "ILocation.h"
#include "IDayLight.h"

namespace automation { namespace script
{

   //-----------------------------------------------------------------------------
   /// \class              Object factory
   //-----------------------------------------------------------------------------
   class IObjectFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IObjectFactory()
      {
      }

      //--------------------------------------------------------------
      /// \brief           Get the auto-location service
      //--------------------------------------------------------------
      virtual boost::shared_ptr<automation::script::IAutoLocation> getAutoLocationService() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the location
      //--------------------------------------------------------------
      virtual boost::shared_ptr<automation::script::ILocation> getLocation() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the daylight
      //--------------------------------------------------------------
      virtual boost::shared_ptr<automation::script::IDayLight> getDayLight() = 0;
   };

} } // namespace automation::script
