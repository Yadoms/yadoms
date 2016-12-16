#include "stdafx.h"
#include "LiveStations.h"
#include <shared/exception/Exception.hpp>
#include <shared/http/HttpMethods.h>
#include "Location.h"

CLiveStations::CLiveStations(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_location = api->getYadomsInformation()->location();
}

CLiveStations::CLiveStations(boost::shared_ptr<const shared::ILocation> location)
{
   m_location = location;
}

void CLiveStations::getAllStations(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const std::string& apikey)
{
   try
   {
      shared::CDataContainer response = shared::CHttpMethods::SendGetRequest("http://api.wunderground.com/api/" + apikey + "/geolookup/q/" + std::to_string(m_location->latitude()) + "," + std::to_string(m_location->longitude()) + ".json");
      response.printToLog(std::cout);
      m_stations = response.get<std::vector<shared::CDataContainer>>("location.nearby_weather_stations.airport.station");
   }
   catch (std::exception& e)
   {
      std::cerr << "exception " << e.what() << std::endl;
      std::cout << "No Stations return by the website" << std::endl;
      // TODO : Gérer la réponse de l'erreur qui est la même que pour les Features
      // TODO : Gestion de l'état du plugin.
   }
}

shared::CDataContainer CLiveStations::bindAvailableStations()
{
   // Create the station list
   std::vector<shared::CDataContainer>::iterator iterStations;
   std::vector<std::string> ICAOList;
   std::vector<std::string>::const_iterator iterICAO;
   int counter = 0;
   shared::CDataContainer value;
   shared::CDataContainer availableStations;

   try {
      for (iterStations = m_stations.begin(); iterStations != m_stations.end(); ++iterStations)
      {
         std::string city = (*iterStations).get<std::string>("city");
         std::string icao = (*iterStations).get<std::string>("icao");

         // Only filled elements are copied
         if (city != "")
         {
            // We add the station only if this one is not already registered
            iterICAO = find(ICAOList.begin(), ICAOList.end(), icao);
            if (iterICAO == ICAOList.end())
            {
               availableStations.set(boost::lexical_cast<std::string>(counter + 1), city + " " + (*iterStations).get<std::string>("country"));

               // we add selectionId, the position into the list proposed to the user, to retrieve it more efficency
               (*iterStations).set("selectionId", boost::lexical_cast<std::string>(counter + 1));

               //we register the icao number of the station. a station is send many times, with a country code, a name different. We register the icao, to keep the station unique
               ICAOList.push_back(icao);
               ++counter;
            }
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
                                                               0);
         }
      }
      catch (std::exception)
      {}
   }

   return location;
}

std::string CLiveStations::getStationName(int selection)
{
   std::vector<shared::CDataContainer>::const_iterator iterStations;
   std::string city;

   for (iterStations = m_stations.begin(); iterStations != m_stations.end(); ++iterStations)
   {
      try {
         if ((*iterStations).get<int>("selectionId") == selection)
            city = (*iterStations).get<std::string>("city");
      }
      catch (std::exception)
      {}
   }

   return city;
}

CLiveStations::~CLiveStations()
{}