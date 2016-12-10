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
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLiveStations();

private:


};

