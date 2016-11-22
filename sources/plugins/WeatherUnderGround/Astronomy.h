#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "WUConfiguration.h"
#include "Keywords/Moon.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Astronomy Module
/// \note   This class consist of the traitment of the astronomy information from the web site
//--------------------------------------------------------------
class CAstronomy
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] api              pointer to the API
   /// \param[in] wuConfiguration  The Configuration of the module
   //--------------------------------------------------------------
   CAstronomy(boost::shared_ptr<yApi::IYPluginApi> api,
              IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] api             pointer to the API
   /// \param[in] wuConfiguration The configuration of the module
   /// \param[in] dataToParse     received data to parse
   //--------------------------------------------------------------
   void parse(boost::shared_ptr<yApi::IYPluginApi> api,
              const IWUConfiguration& wuConfiguration,
              const shared::CDataContainer dataToParse);

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] api                pointer to the API
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void onUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                 IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Return the url
   /// \return    The url string
   //--------------------------------------------------------------
   std::string getUrl() const;

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CAstronomy();

private:

   //--------------------------------------------------------------
   /// \brief	  Initialise Astronomy variables
   /// \param[in] api                pointer to the API
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void initializeVariables(boost::shared_ptr<yApi::IYPluginApi> api,
                            IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_localisation;

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_countryOrState;

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
};

