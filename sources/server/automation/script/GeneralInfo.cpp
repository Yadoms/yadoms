#include "stdafx.h"
#include "GeneralInfo.h"
#include <shared/ServiceLocator.h>
#include "IRunningInformation.h"
#include "DayLightProvider.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include <shared/StringExtension.h>

namespace automation
{
   namespace script
   {
      CGeneralInfo::CGeneralInfo(boost::shared_ptr<shared::ILocation> locationProvider,
                                 boost::shared_ptr<dateTime::ITimeZoneProvider> timezoneProvider)
         : m_locationProvider(locationProvider),
           m_dayLightProvider(boost::make_shared<CDayLightProvider>(locationProvider, timezoneProvider))
      {
      }

      CGeneralInfo::~CGeneralInfo()
      {
      }

      std::string CGeneralInfo::get(const shared::script::yScriptApi::IYScriptApi::EInfoKeys key) const
      {
         try
         {
            switch (key)
            {
            case shared::script::yScriptApi::IYScriptApi::kSunrise:
               {
                  try
                  {
                     return CDayLightProvider::formatSunEventTime(m_dayLightProvider->sunriseTime());
                  }
                  catch (shared::exception::CEmptyResult&)
                  {
                     YADOMS_LOG(error) <<
                        "General info, get sunrise : daylight is not available (do you set your location ?)";
                     return std::string();
                  }
               }
            case shared::script::yScriptApi::IYScriptApi::kSunset:
               {
                  try
                  {
                     return CDayLightProvider::formatSunEventTime(m_dayLightProvider->sunsetTime());
                  }
                  catch (shared::exception::CEmptyResult&)
                  {
                     YADOMS_LOG(error) <<
                        "General info, get sunset : daylight is not available (do you set your location ?)";
                     return std::string();
                  }
               }
            case shared::script::yScriptApi::IYScriptApi::kLatitude: return shared::CStringExtension::
                  cultureInvariantToString(m_locationProvider->latitude());
            case shared::script::yScriptApi::IYScriptApi::kLongitude: return shared::CStringExtension::
                  cultureInvariantToString(m_locationProvider->longitude());
            case shared::script::yScriptApi::IYScriptApi::kAltitude: return shared::CStringExtension::
                  cultureInvariantToString(m_locationProvider->altitude());
            case shared::script::yScriptApi::IYScriptApi::kYadomsServerOS: return shared::CServiceLocator::instance().
                  get<IRunningInformation>()->getOperatingSystemName();
            case shared::script::yScriptApi::IYScriptApi::kYadomsServerVersion: return shared::CServiceLocator::
                  instance().get<IRunningInformation>()->getSoftwareVersion().serialize();
            default:
               throw shared::exception::COutOfRange((boost::format("Key %1% doesn't exist") % key).str());
            }
         }
         catch (shared::exception::CException& e)
         {
            YADOMS_LOG(error) << "General info, get " << key << " returning error : " << e.what();
            return std::string();
         }
      }
   }
} // namespace automation::script
