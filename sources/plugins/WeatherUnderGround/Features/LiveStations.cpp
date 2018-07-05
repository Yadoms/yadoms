#include "stdafx.h"
#include "LiveStations.h"
#include <shared/exception/Exception.hpp>
#include <shared/http/HttpMethods.h>
#include <shared/http/StandardSession.h>
#include "Location.h"
#include "../RequestErrorException.hpp"
#include "../webSiteErrorException.hpp"
#include "../ErrorAnswerHandler.h"
#include <shared/Log.h>

CLiveStations::CLiveStations(boost::shared_ptr<yApi::IYPluginApi> api):
   m_location(api->getYadomsInformation()->location()),
   m_lastSearchLocation(boost::make_shared<location::CLocation>(0, 0, 0))
{
}

CLiveStations::CLiveStations(boost::shared_ptr<const shared::ILocation> location):
   m_location(location),
   m_lastSearchLocation(boost::make_shared<location::CLocation>(0, 0, 0))
{
}

void CLiveStations::processLookUp(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const std::string& apikey)
{
   try
   {
      //send a new lookup only if different of the last one
      if (m_location->latitude() != m_lastSearchLocation->latitude() || m_location->longitude() != m_lastSearchLocation->longitude())
      {
         const shared::CDataContainer noParameters;
         const shared::CDataContainer noheaderParameter;
         auto url = "http://api.wunderground.com/api/" + apikey + "/geolookup/q/" + std::to_string(m_location->latitude()) + ","
            + std::to_string(m_location->longitude()) + ".json";
         const auto session = boost::make_shared<shared::CStandardSession>(url);
         shared::CHttpMethods::sendGetRequest(session,
                                              noheaderParameter,
                                              noParameters,
                                              [&](shared::CDataContainer& data)
                                              {
                                                 m_response = data;
                                                 ErrorAnswerHandler errorHandler(api, data);

                                                 if (errorHandler.ContainError())
                                                    throw CWebSiteErrorException(errorHandler.getError());
                                              },
                                              httpRequestBindingTimeout);
      }
      m_lastSearchLocation = m_location;
   }
   catch (CWebSiteErrorException& e)
   {
      throw CWebSiteErrorException(e.what());
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "exception " << e.what();
      YADOMS_LOG(information) << "No Stations return by the website";

      throw CRequestErrorException();
   }
}

boost::shared_ptr<const shared::ILocation> CLiveStations::getStationLocation(int selection)
{
   boost::shared_ptr<const shared::ILocation> location;

   m_stations = m_response.get<std::vector<shared::CDataContainer>>("location.nearby_weather_stations.airport.station");

   for (std::vector<shared::CDataContainer>::const_iterator iterStations = m_stations.begin(); iterStations != m_stations.end(); ++iterStations)
   {
      try
      {
         if ((*iterStations).get<int>("selectionId") == selection)
         {
            location = boost::make_shared<location::CLocation>((*iterStations).get<double>("lon"),
                                                               (*iterStations).get<double>("lat"),
                                                               0);
         }
      }
      catch (std::exception)
      {
      }
   }

   return location;
}

boost::shared_ptr<const shared::ILocation> CLiveStations::getCityLocation() const
{
   return m_location;
}

std::string CLiveStations::getStationName(int selection)
{
   std::string city;

   for (std::vector<shared::CDataContainer>::const_iterator iterStations = m_stations.begin(); iterStations != m_stations.end(); ++iterStations)
   {
      try
      {
         if ((*iterStations).get<int>("selectionId") == selection)
            city = (*iterStations).get<std::string>("city");
      }
      catch (std::exception)
      {
      }
   }

   return city;
}

std::string CLiveStations::getCity() const
{
   return m_response.get<std::string>("location.city");
}

CLiveStations::~CLiveStations()
{
   YADOMS_LOG(information) << "destruction CLiveStation";
}
