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
   /// \brief get All forecast stations around the location
   /// \param[in] api       pointer to the API
   /// \param[in] apikey    api key to weatherUnderground
   /// \return all forecast stations if found
   //--------------------------------------------------------------
   std::vector<shared::CDataContainer> getAllStations(boost::shared_ptr<yApi::IYPluginApi> api,
                                                      const std::string& apikey);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLiveStations();

private:

   //--------------------------------------------------------------
   /// \brief	    GPS Location
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::ILocation> m_location;
};

