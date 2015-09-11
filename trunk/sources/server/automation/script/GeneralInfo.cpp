#include "stdafx.h"
#include "GeneralInfo.h"
#include "DayLight.h"
#include "Location.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/StringExtension.h>
#include <shared/ServiceLocator.h>

namespace automation { namespace script
{

CGeneralInfo::CGeneralInfo(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager)
   :m_location(new CLocation(configurationManager)),
   m_dayLight(new CDayLight(m_location)),
   m_runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>())
{
}

CGeneralInfo::~CGeneralInfo()
{         
}

std::string CGeneralInfo::get(shared::script::yScriptApi::IYScriptApi::EInfoKeys key) const
{
   switch (key)
   {
   case shared::script::yScriptApi::IYScriptApi::kSunrise: return m_dayLight->sunriseTime();
   case shared::script::yScriptApi::IYScriptApi::kSunset: return m_dayLight->sunsetTime();
   case shared::script::yScriptApi::IYScriptApi::kLatitude: return shared::CStringExtension::cultureInvariantToString(m_location->latitude());
   case shared::script::yScriptApi::IYScriptApi::kLongitude: return shared::CStringExtension::cultureInvariantToString(m_location->longitude());
   case shared::script::yScriptApi::IYScriptApi::kAltitude: return shared::CStringExtension::cultureInvariantToString(m_location->altitude());
   case shared::script::yScriptApi::IYScriptApi::kYadomsServerOS: return m_runningInformation->getOperatingSystemName();
   case shared::script::yScriptApi::IYScriptApi::kYadomsServerVersion: return m_runningInformation->getSoftwareVersion().toString();
   default:
      throw shared::exception::COutOfRange((boost::format("Key %1% doesn't exist") % key).str());
   }
}

} } // namespace automation::script

