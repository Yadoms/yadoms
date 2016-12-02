#include "stdafx.h"
#include "Location.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>


namespace location
{
   CLocation::CLocation(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                        boost::shared_ptr<IAutoLocation> autoLocationService)
      : m_configurationManager(configurationManager),
        m_autoLocationService(autoLocationService)
   {
      shared::CDataContainer location;
      try
      {
         location = m_configurationManager->getConfiguration("system", "location")->Value();
      }
      catch (shared::exception::CEmptyResult&)
      {
         // Not found in database ==> let location empty
      }

      if (location.empty())
      {
         YADOMS_LOG(information) << "Location was not found in database, try to auto-locate...";
         tryAutoLocate();
      }
   }

   CLocation::~CLocation()
   {
   }

   void CLocation::tryAutoLocate() const
   {
      try
      {
         m_autoLocationService->tryAutoLocate(
            [&](const shared::CDataContainer& foundLocation)
            {
               // Update location in database if still empty (can be filled by user in the meantime)
               try
               {
                  m_configurationManager->getConfiguration("system", "location");
               }
               catch (shared::exception::CEmptyResult&)
               {
                  // Not found in database ==> write it
                  YADOMS_LOG(information) << "Auto-location was successful";
                  updateLocation(foundLocation);
               }
            });
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(warning) << "Auto-locate failed (" << e.what() << "), location is unknown";
      }
   }

   void CLocation::updateLocation(const shared::CDataContainer& location) const
   {
      database::entities::CConfiguration locationConfiguration;
      locationConfiguration.Section = "system";
      locationConfiguration.Name = "location";
      locationConfiguration.Value = location.serialize();
      m_configurationManager->updateConfiguration(locationConfiguration);
   }

   shared::CDataContainer CLocation::getLocation() const
   {
      return shared::CDataContainer(m_configurationManager->getConfiguration("system", "location")->Value());
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
} // namespace location


