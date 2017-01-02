#include "stdafx.h"
#include "LiveStations.h"
#include <shared/exception/Exception.hpp>
#include <shared/http/HttpMethods.h>
#include "Location.h"
#include "../RequestErrorException.hpp"

CLiveStations::CLiveStations(boost::shared_ptr<yApi::IYPluginApi> api):
   m_lastSearchLocation(boost::make_shared<location::CLocation>(0,0,0))
{
   //TODO : api->getYadomsInformation()->location() renvoi maintenant un pointeur nul si la location
   // est inconnue. Attention, la location peut être nulle au début (le temps de l'init), puis être
   // correcte un peu plus tard. Il convient donc d'appeler cette fonction à chaque fois que nécessaire,
   // et une seule fois à l'init.
   m_location = api->getYadomsInformation()->location();
}

CLiveStations::CLiveStations(boost::shared_ptr<const shared::ILocation> location)
{
   m_location = location;
}

void CLiveStations::processLookUp(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const std::string& apikey)
{
   try
   {
      //send a new lookup only if different of the last one
      if (m_location->latitude() != m_lastSearchLocation->latitude() || m_location->longitude() != m_lastSearchLocation->longitude())
      {
         shared::CDataContainer noParameters;
         shared::CHttpMethods::SendGetRequest("http://api.wunderground.com/api/" + apikey + "/geolookup/q/" + std::to_string(m_location->latitude()) + "," + std::to_string(m_location->longitude()) + ".json",
                                              noParameters,
                                              [&](shared::CDataContainer& data)
                                              {
                                                 m_response = data;
                                                 m_stations = m_response.get<std::vector<shared::CDataContainer>>("location.nearby_weather_stations.airport.station");
                                              },
                                              httpRequestBindingTimeout);
      }
      m_lastSearchLocation = m_location;
   }
   catch (std::exception& e)
   {
      std::cerr << "exception " << e.what() << std::endl;
      std::cout << "No Stations return by the website" << std::endl;
      
      throw CRequestErrorException();
   }
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

boost::shared_ptr<const shared::ILocation> CLiveStations::getCityLocation()
{
   return m_location;
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

std::string CLiveStations::getCity()
{
   return m_response.get<std::string>("location.city");
}

CLiveStations::~CLiveStations()
{
   std::cout << "destruction CLiveStation" << std::endl;
}