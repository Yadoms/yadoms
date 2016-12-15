#include "stdafx.h"
#include "LiveStations.h"
#include <shared/exception/Exception.hpp>
#include <shared/http/HttpMethods.h>
#include "Location.h"

CLiveStations::CLiveStations(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_location = api->getYadomsInformation()->location();
}

void CLiveStations::getAllStations(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const std::string& apikey)
{
   try
   {
      shared::CDataContainer response = shared::CHttpMethods::SendGetRequest("http://api.wunderground.com/api/" + apikey + "/geolookup/q/" + std::to_string(m_location->latitude()) + "," + std::to_string(m_location->longitude()) + ".json");
      m_stations = response.get<std::vector<shared::CDataContainer>>("location.nearby_weather_stations.airport.station");
   }
   catch (std::exception& e)
   {
      std::cerr << "exception " << e.what() << std::endl;
      std::cout << "No Stations return by the website" << std::endl;
   }
}

shared::CDataContainer CLiveStations::bindAvailableStations()
{
   // Create the station list
   std::vector<shared::CDataContainer>::iterator iterStations;
   int counter = 0;
   shared::CDataContainer value;
   shared::CDataContainer availableStations;

   try {
      for (iterStations = m_stations.begin(); iterStations != m_stations.end(); ++iterStations)
      {
         std::string city = (*iterStations).get<std::string>("city");

         // Only filled elements are copied
         if (city != "")
         {
            availableStations.set(boost::lexical_cast<std::string>(counter + 1), city + " " + (*iterStations).get<std::string>("country"));

            // we add selectionId, the position into the list proposed to the user, to retrieve it more efficency
            (*iterStations).set("selectionId", boost::lexical_cast<std::string>(counter + 1));
            ++counter;
         }

         /* return format :
         -city :
         -state :
         -country :
         -icao :
         -lat :
         -lon :
         */
      }
   }
   catch (std::exception& e)
   {
      std::cerr << "exception " << e.what() << std::endl;
      std::cout << "No Stations return in binding" << std::endl;

      // reset values
      shared::CDataContainer defaultReturn;
      availableStations.initializeWith(defaultReturn);
   }

   availableStations.set(boost::lexical_cast<std::string>(counter + 1), "Essai");

   value.set("required", "true");
   value.set("type", "enum");
   value.set("values", availableStations);
   value.set("defaultValue", "1");

   return value;
}

boost::shared_ptr<const shared::ILocation> CLiveStations::getStationLocation(int selection)
{
   std::vector<shared::CDataContainer>::const_iterator iterStations;
   boost::shared_ptr<const shared::ILocation> location;

   for (iterStations = m_stations.begin(); iterStations != m_stations.end(); ++iterStations)
   {
      try {
         if ((*iterStations).get<int>("selectionId") == selection)
         {
            location = boost::make_shared<location::CLocation>((*iterStations).get<double>("lon"),
                                                               (*iterStations).get<double>("lat"),
                                                               0
                                                               );
         }
      }
      catch (std::exception)
      {}
   }

   return location;
}

CLiveStations::~CLiveStations()
{}