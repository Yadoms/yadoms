#include "stdafx.h"
#include "Location.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "IpApiAutoLocation.h"


namespace automation { namespace script
{

CLocation::CLocation(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager)
   :m_configurationManager(configurationManager)
{
}

CLocation::~CLocation()
{         
}

boost::shared_ptr<IAutoLocation> CLocation::createAutoLocationService() const
{
   // Use ip-api service to try to auto-locate (need an internet connexion)
   boost::shared_ptr<IAutoLocation> service(new CIpApiAutoLocation);
   return service;
}

shared::CDataContainer CLocation::tryAutoLocate() const
{
   boost::shared_ptr<IAutoLocation> autoLocationService = createAutoLocationService();
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
   try
   {
      const shared::CDataContainer location(m_configurationManager->getConfiguration("system", "location")->Value);
      if (location.empty())
         throw shared::exception::CEmptyResult("Location is empty in database");
      return shared::CDataContainer(location);
   }
   catch (shared::exception::CEmptyResult&)
   {
      YADOMS_LOG(information) << "Location was not found in database, try to auto-locate...";
      return tryAutoLocate();
   }
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

