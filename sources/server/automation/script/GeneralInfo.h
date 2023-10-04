#pragma once
#include "IGeneralInfo.h"
#include "IDayLightProvider.h"
#include <shared/ILocation.h>
#include "dateTime/ITimeZoneProvider.h"

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script properties
      //-----------------------------------------------------
      class CGeneralInfo final : public IGeneralInfo
      {
      public:
         CGeneralInfo(boost::shared_ptr<shared::ILocation> locationProvider,
                      boost::shared_ptr<dateTime::ITimeZoneProvider> timezoneProvider);
         ~CGeneralInfo() override = default;

      protected:
         // IGeneralInfo Implementation
         std::string get(shared::script::yScriptApi::IYScriptApi::EInfoKeys key) const override;
         // [END] IGeneralInfo Implementation

      private:
         boost::shared_ptr<shared::ILocation> m_locationProvider;
         boost::shared_ptr<IDayLightProvider> m_dayLightProvider;
      };
   }
} // namespace automation::script
