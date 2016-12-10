#pragma once
#include <shared/DataContainer.h>

namespace location
{
   //-----------------------------------------------------
   ///\brief The auto-location interface
   //-----------------------------------------------------
   class IAutoLocation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IAutoLocation()
      {
      }

      //-----------------------------------------------------
      ///\brief                  Try to auto locate
      /// \param[in] onFoundFct  Functor called when location is found? Not called if location is not found
      /// \note onFoundFct provide a CDataContainer containing these fields :
      ///     - latitude (as double)
      ///     - longitude (as double)
      ///     - altitude (as double)
      //-----------------------------------------------------
      virtual void tryAutoLocate(boost::function<void(const shared::CDataContainer&)> onFoundFct) const = 0;
   };
} // namespace location


