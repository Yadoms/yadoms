#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Live Station return by WeatherUnderground
/// \note   This class consist of the traitment of all stations returned by the WeatherUnderground website
//--------------------------------------------------------------
class CLiveStations
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] api              pointer to the API
   //--------------------------------------------------------------
   CLiveStations(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] location       enter directly a location
   //--------------------------------------------------------------
   CLiveStations(boost::shared_ptr<const shared::ILocation> location);

   //--------------------------------------------------------------
   /// \brief get All forecast stations around the location, get the city name
   /// \param[in] api       pointer to the API
   /// \param[in] apikey    api key to weatherUnderground
   //--------------------------------------------------------------
   void processLookUp(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& apikey);

   //--------------------------------------------------------------
   /// \brief	    getStationLocation
   /// \param[in] selection      list selection return by the web site
   /// \return                   the location (longitude, latitude) structure of the selected forecast station
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::ILocation> getStationLocation(int selection);

   //--------------------------------------------------------------
   /// \brief	    getCityLocation
   /// \return                   the location (longitude, latitude) of the city
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::ILocation> getCityLocation();

   //--------------------------------------------------------------
   /// \brief	    getStationName
   /// \param[in] selection      list selection return by the web site
   /// \return                   the forecast station name
   //--------------------------------------------------------------
   std::string getStationName(int selection);

   //--------------------------------------------------------------
   /// \brief	    getCity
   /// \return                   the city corresponding to the coordonates
   //--------------------------------------------------------------
   std::string getCity();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLiveStations();

private:
   //--------------------------------------------------------------
   /// \brief get All forecast stations around the location, get the city name
   /// \param[in] api       data Received
   /// \param[in] apikey    api key to weatherUnderground
   //--------------------------------------------------------------
   void onReceived(shared::CDataContainer& data);

   //--------------------------------------------------------------
   /// \brief	    GPS Location
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::ILocation> m_location;

   //--------------------------------------------------------------
   /// \brief	    last search Location
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::ILocation> m_lastSearchLocation;

   //--------------------------------------------------------------
   /// \brief	    list of all Stations return by WeatherUnderground
   //--------------------------------------------------------------
   std::vector<shared::CDataContainer> m_stations;

   //--------------------------------------------------------------
   /// \brief	    response return by weather underground
   //--------------------------------------------------------------
   shared::CDataContainer m_response;
};

