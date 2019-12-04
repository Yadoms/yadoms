#pragma once
#include "IAutoLocation.h"

namespace location
{
   //-----------------------------------------------------
   ///\brief The auto-location via ip-api service (http://ip-api.com/)
   //-----------------------------------------------------
   class CIpApiAutoLocation : public IAutoLocation
   {
   public:
      CIpApiAutoLocation() = default;
      virtual ~CIpApiAutoLocation() = default;

   protected:
      // IAutoLocation Implementation
      void tryAutoLocate(boost::function<void(const shared::CDataContainer&)> onFoundFct) const override;
      // [END] IAutoLocation Implementation

      static void doAutoLocate(boost::function<void(const shared::CDataContainer&)> onFoundFct);
   };
} // namespace location
