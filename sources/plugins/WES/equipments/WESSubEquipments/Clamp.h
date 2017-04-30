#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/Period.h"
#include "../specificHistorizers/TeleInfoStatus.h"
#include "../masterDeviceConfiguration.h"
#include "../../IWESConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   namespace subdevices
   {
      //-----------------------------------------------------
      ///\brief Clamp equipment to be connected to the WES
      //-----------------------------------------------------
      class CClamp
      {
      public:
         //-----------------------------------------------------
         ///\brief                                      Constructor
         ///\param[in] api                             Yadoms API
         ///\param[in] keywordsToDeclare               list of keywords to declare
         ///\param[in] pluginConfiguration             the plugin configuration
         ///\param[in] isInstantCurrentClampRegistered Boolean value for instant current to historize if check in the interface
         ///\param[in] deviceName                      The device name
         ///\param[in] keywordName                     The keyword name
         //-----------------------------------------------------
         CClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                const bool isInstantCurrentClampRegistered,
                const std::string& deviceName,
                const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                                      updateFromDevice
         ///\param[in] api                             Yadoms API
         ///\param[in] keywordsToHistorize             list of keywords to historize
         ///\param[in] isInstantCurrentClampRegistered Boolean value for instant current to historize if check in the interface
         ///\param[in] instantCurrentValue             The instant current value sent by the WES
         ///\param[in] energyClampValue                The energy clamp value sent by the WES
         //-----------------------------------------------------
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                               std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                               const bool isInstantCurrentClampRegistered,
                               const double& instantCurrentValue,
                               const Poco::Int64& energyClampValue);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CClamp();

      private:

         //-----------------------------------------------------
         ///\brief                                      initializeClamp
         ///\param[in] api                             Yadoms API
         ///\param[in] keywordsToDeclare               list of keywords to declare
         ///\param[in] pluginConfiguration             the plugin configuration
         ///\param[in] isInstantCurrentClampRegistered Boolean value for instant current to historize if check in the interface
         ///\param[in] keywordName                     The keyword name
         //-----------------------------------------------------
         void initializeClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                              std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                              const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                              const bool isInstantCurrentClampRegistered,
                              const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                     The device name
         //-----------------------------------------------------
         std::string m_deviceName;

         //--------------------------------------------------------------
         /// \brief  Keywords list
         //--------------------------------------------------------------
         boost::shared_ptr<yApi::historization::CCurrent> m_CurrentClamp;
         boost::shared_ptr<yApi::historization::CEnergy>  m_CounterClamp;

         //--------------------------------------------------------------
         /// \brief  Keywords Name
         //--------------------------------------------------------------
         std::string m_keywordName;

      };
   }
} // namespace equipments::subdevices