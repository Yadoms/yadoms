#include "stdafx.h"
#include "LiveStations.h"
#include <shared/exception/Exception.hpp>
#include <shared/http/HttpMethods.h>
#include "Location.h"
#include "../RequestErrorException.hpp"

CLiveStations::CLiveStations(boost::shared_ptr<yApi::IYPluginApi> api)
{
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
      m_response = shared::CHttpMethods::SendGetRequest("http://api.wunderground.com/api/" + apikey + "/geolookup/q/" + std::to_string(m_location->latitude()) + "," + std::to_string(m_location->longitude()) + ".json");
      //m_response.printToLog(std::cout);
      m_stations = m_response.get<std::vector<shared::CDataContainer>>("location.nearby_weather_stations.airport.station");
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