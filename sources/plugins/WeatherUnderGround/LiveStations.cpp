#include "stdafx.h"
#include "LiveStations.h"
#include <shared/exception/Exception.hpp>
#include <shared/http/HttpMethods.h>

CLiveStations::CLiveStations(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_location = api->getYadomsInformation()->location();
}

std::vector<shared::CDataContainer> CLiveStations::getAllStations(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                  const std::string& apikey)
{
   int nbRetry = 0;
   shared::CDataContainer response;
   std::vector<shared::CDataContainer> stations;

   try
   {
      response = shared::CHttpMethods::SendGetRequest("http://api.wunderground.com/api/" + apikey + "/geolookup/q/" + std::to_string(m_location->latitude()) + "," + std::to_string(m_location->longitude()) + ".json");
      response.printToLog(std::cout);
      stations = response.get<std::vector<shared::CDataContainer>>("location.nearby_weather_stations.airport.station");
   }
   catch (std::exception)
   {}

   return stations;
}

CLiveStations::~CLiveStations()
{}