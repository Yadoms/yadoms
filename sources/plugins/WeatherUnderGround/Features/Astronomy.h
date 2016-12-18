#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IFeature.h"
#include "../WUConfiguration.h"
#include "Keywords/Moon.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Astronomy Module
/// \note   This class consist of the traitment of the astronomy information from the web site
//--------------------------------------------------------------
class CAstronomy : public features::IFeature
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] api                  pointer to the API
   /// \param[in] wuConfiguration      The Configuration of the module
   /// \param[in] deviceconfiguration  The Configuration of the device
   /// \param[in] location             GPS Position where to find astronomy information
   /// \param[in] deviceName           Name of the device
   //--------------------------------------------------------------
   CAstronomy(boost::shared_ptr<yApi::IYPluginApi> api,
              IWUConfiguration& wuConfiguration,
              boost::shared_ptr<const shared::ILocation> location,
              const std::string& deviceName);

   // IFeature Implementation
   void parse(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer dataToParse, IWUConfiguration& wuConfiguration) override;
   void onPluginUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration) override;
//   void onDeviceUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration, boost::shared_ptr<const shared::ILocation> location) override;
   std::string getUrl() const override;
   std::string getName() const override;
   void setCityName(const std::string& CityName) override;
   // [END] IFeature Implementation

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CAstronomy();

private:

   //--------------------------------------------------------------
   /// \brief	  Initialise Astronomy keywords
   /// \param[in] api                  pointer to the API
   //--------------------------------------------------------------
   void initializeKeywords(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	    The Device Name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	    The url link to access properly the web site
   //--------------------------------------------------------------
   std::stringstream m_url;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   boost::shared_ptr<CMoon> m_moonCharacteristics;

   //--------------------------------------------------------------
   /// \brief  Keywords list
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;

   //--------------------------------------------------------------
   /// \brief  the location used
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::ILocation> m_location;
};

