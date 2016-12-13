#include "stdafx.h"
#include "LiveStations.h"
#include <shared/exception/Exception.hpp>
#include <shared/http/HttpMethods.h>

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
      response.printToLog(std::cout);
      m_stations = response.get<std::vector<shared::CDataContainer>>("location.nearby_weather_stations.airport.station");
   }
   catch (std::exception)
   {}
}

shared::CDataContainer CLiveStations::bindAvailableStations()
{
   shared::CDataContainer response;

   //TODO : Finish
   return response;
}

CLiveStations::~CLiveStations()
{}