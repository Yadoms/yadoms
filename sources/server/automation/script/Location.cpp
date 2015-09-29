#include "stdafx.h"
#include "Location.h"
#include <shared/ServiceLocator.h>
#include "IObjectFactory.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "IpApiAutoLocation.h"


namespace automation { namespace script
{

CLocation::CLocation(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager)
   :m_configurationManager(configurationManager)
{
   shared::CDataContainer location;
   try
   {
      location = m_configurationManager->getConfiguration("system", "location")->Value;
   }
   catch (shared::exception::CEmptyResult&)
   {
      // Not found in database ==> let location empty
   }

   if (location.empty())
   {
      YADOMS_LOG(information) << "Location was not found in database, try to auto-locate...";
      tryAutoLocate(shared::CServiceLocator::instance().get<IObjectFactory>());// will throw shared::exception::CEmptyResult if unable to autolocate
   }
}

CLocation::~CLocation()
{         
}

shared::CDataContainer CLocation::tryAutoLocate(boost::shared_ptr<IObjectFactory> factory) const
{
   boost::shared_ptr<IAutoLocation> autoLocationService = factory->getAutoLocationService();
   shared::CDataContainer foundLocation = autoLocationService->tryAutoLocate();
   updateLocation(foundLocation);
   return foundLocation;
}

void CLocation::updateLocation(const shared::CDataContainer& location) const
{
   database::entities::CConfiguration locationConfiguration;
   locationConfiguration.Section = "system";
   locationConfiguration.Name    = "location";
   locationConfiguration.Value   = location.serialize();
   m_configurationManager->updateConfiguration(locationConfiguration);
}

shared::CDataContainer CLocation::getLocation() const
{
   return m_configurationManager->getConfiguration("system", "location")->Value;
}

double CLocation::latitude() const
{
   return getLocation().get<double>("latitude");
}

double CLocation::longitude() const
{
   return getLocation().get<double>("longitude");
}

double CLocation::altitude() const
{
   return getLocation().get<double>("altitude");
}

} } // namespace automation::script

