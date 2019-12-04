#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
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
      ///\brief Analog subdevices connected to the WES
      //-----------------------------------------------------
      class CAnalog
      {
      public:
         //-----------------------------------------------------
         ///\brief                                      Constructor
         ///\param[in] api                             Yadoms API
         ///\param[in] keywordsToDeclare               list of keywords to declare
         ///\param[in] pluginConfiguration             the plugin configuration
         ///\param[in] type                            the declaration type
         ///\param[in] deviceName                      The device name
         ///\param[in] keywordName                     The keyword name
         //-----------------------------------------------------
         CAnalog(boost::shared_ptr<yApi::IYPluginApi> api,
                std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                const std::string& type,
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
                               const double& Value);

         //-----------------------------------------------------
         ///\brief                     get the device name
         ///\return                    the clamp keyword Name
         //-----------------------------------------------------
         std::string name();

         //-----------------------------------------------------
         ///\brief                     get the device name
         ///\return                    the clamp keyword Name
         //-----------------------------------------------------
         std::string type();

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CAnalog();

      private:

         //-----------------------------------------------------
         ///\brief                     The device name
         //-----------------------------------------------------
         std::string m_deviceName;

         //--------------------------------------------------------------
         /// \brief  Keywords list
         //--------------------------------------------------------------
         boost::shared_ptr<yApi::historization::CCurrent> m_CurrentHistorizer;
         boost::shared_ptr<yApi::historization::CVoltage> m_VoltageHistorizer;
         boost::shared_ptr<yApi::historization::CEnergy> m_EnergyHistorizer;
         boost::shared_ptr<yApi::historization::CTemperature> m_TemperatureHistorizer;

         //--------------------------------------------------------------
         /// \brief  Keywords Name
         //--------------------------------------------------------------
         std::string m_keywordName;

         //--------------------------------------------------------------
         /// \brief  Type
         //--------------------------------------------------------------
         std::string m_type;
      };
   }
} // namespace equipments::subdevices