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
         ///\param[in] deviceName                      The device name
         ///\param[in] keywordName                     The keyword name
         //-----------------------------------------------------
         CClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                const std::string& deviceName,
                const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                                      updateFromDevice
         ///\param[in] api                             Yadoms API
         ///\param[in] keywordsToHistorize             list of keywords to historize
         ///\param[in] energyClampValue                The energy clamp value sent by the WES
         //-----------------------------------------------------
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                               std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                               const Poco::Int64& energyClampValue);

         //-----------------------------------------------------
         ///\brief                     get the device name
         ///\return                    the clamp keyword Name
         //-----------------------------------------------------
         std::string name();

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CClamp();

      private:

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