#include "stdafx.h"
#include "GeneralInfo.h"
#include "DayLight.h"
#include "Location.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>

namespace automation { namespace script
{

DECLARE_ENUM_IMPLEMENTATION(EInfo,
   ((Sunrise))
   ((Sunset))

   ((Latitude))
   ((Longitude))
   ((Altitude))

   ((YadomsServerOS))
   ((YadomsServerVersion))
)

CGeneralInfo::CGeneralInfo(
   boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
   boost::shared_ptr<IRunningInformation> runningInformation)
   :m_location(new CLocation(configurationManager)),
   m_dayLight(new CDayLight(m_location)),
   m_runningInformation(runningInformation)
{
}

CGeneralInfo::~CGeneralInfo()
{         
}

std::string CGeneralInfo::get(const std::string& key) const
{
   try
   {
      EInfo info(key);
      switch(info)
      {
      case EInfo::kSunriseValue: return boost::lexical_cast<std::string>(m_dayLight->sunriseTime());
      case EInfo::kSunsetValue: return boost::lexical_cast<std::string>(m_dayLight->sunsetTime());
      case EInfo::kLatitudeValue: return boost::lexical_cast<std::string>(m_location->latitude());
      case EInfo::kLongitudeValue: return boost::lexical_cast<std::string>(m_location->longitude());
      case EInfo::kAltitudeValue: return boost::lexical_cast<std::string>(m_location->altitude());
      case EInfo::kYadomsServerOSValue: return m_runningInformation->getOperatingSystemName();
      case EInfo::kYadomsServerVersionValue: return m_runningInformation->getSoftwareVersion().toString();
      default:
         YADOMS_LOG(error) << "Getting general information : key " << key << " not suported (even defined)";
         return std::string();
      }
   }
   catch (shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(warning) << "Getting general information : key " << key << " not found : " << e.what();
      return std::string();
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(warning) << "Getting general information : key " << key << " is empty : " << e.what();
      return std::string();
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(warning) << "Getting general information : key " << key << " error : " << e.what();
      return std::string();
   }
}

} } // namespace automation::script

