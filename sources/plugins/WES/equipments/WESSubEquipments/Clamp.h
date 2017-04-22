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
      ///\brief WES equipment
      //-----------------------------------------------------
      class CClamp
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in]   api          Yadoms API
         ///\param[in] device         The device name
         //-----------------------------------------------------
         CClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                const bool isInstantCurrentClampRegistered,
                const std::string& deviceName,
                const std::string& keywordName);

         std::string getDeviceName() const;
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                               std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                               const bool isInstantCurrentClampRegistered,
                               const double& instantCurrentValue,
                               const double& energyClampValue);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CClamp();

      private:

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