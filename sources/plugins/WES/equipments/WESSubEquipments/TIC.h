#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/Period.h"
#include "../specificHistorizers/TeleInfoStatus.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   namespace subdevices
   {
      //-----------------------------------------------------
      ///\brief WES equipment
      //-----------------------------------------------------
      class CTIC
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in]   api          Yadoms API
         ///\param[in] device         The device name
         //-----------------------------------------------------
         CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& deviceName);

         std::string getDeviceName() const;
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CTIC();

      private:

         //-----------------------------------------------------
         ///\brief                     The device name
         //-----------------------------------------------------
         std::string m_deviceName;

         //--------------------------------------------------------------
         /// \brief  Keywords list
         //--------------------------------------------------------------
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

         //--------------------------------------------------------------
         /// \brief  Keywords
         //--------------------------------------------------------------
         boost::shared_ptr<yApi::historization::CEnergy> m_baseCounter;
         boost::shared_ptr<yApi::historization::CEnergy> m_lowCostCounter;
         boost::shared_ptr<yApi::historization::CEnergy> m_normalCostCounter;
         boost::shared_ptr<yApi::historization::CEnergy> m_EJPPeakPeriod;
         boost::shared_ptr<yApi::historization::CEnergy> m_EJPNormalPeriod;
         boost::shared_ptr<yApi::historization::CEnergy> m_tempoBlueDaysLowCostPeriod;
         boost::shared_ptr<yApi::historization::CEnergy> m_tempoBlueDaysNormalCostPeriod;
         boost::shared_ptr<yApi::historization::CEnergy> m_tempoRedDaysLowCostPeriod;
         boost::shared_ptr<yApi::historization::CEnergy> m_tempoRedDaysNormalCostPeriod;
         boost::shared_ptr<yApi::historization::CEnergy> m_tempoWhiteDaysLowCostPeriod;
         boost::shared_ptr<yApi::historization::CEnergy> m_tempoWhiteDaysNormalCostPeriod;

         boost::shared_ptr<yApi::historization::CApparentPower> m_apparentPower;

         boost::shared_ptr<specificHistorizers::CPeriod> m_TimePeriod;

         //--------------------------------------------------------------
         /// \brief	TeleInfo Status
         //--------------------------------------------------------------
         boost::shared_ptr<specificHistorizers::CTeleInfoStatus> m_teleInfoStatus;
      };
   }
} // namespace equipments::subdevices