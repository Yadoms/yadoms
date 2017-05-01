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
         ///\param[in] contract       The contract subscribe
         //-----------------------------------------------------
         CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& deviceName,
              const std::string& contract);

         //-----------------------------------------------------
         ///\brief                                      updateFromDevice
         ///\param[in] api                             Yadoms API
         ///\param[in] contractName                    contract Name
         ///\param[in] counter1                        1st counter sent by the wes
         ///\param[in] counter2                        2nd counter sent by the wes
         ///\param[in] counter3                        3td counter sent by the wes
         ///\param[in] counter4                        4th counter sent by the wes
         ///\param[in] counter5                        5th counter sent by the wes
         ///\param[in] counter6                        6th counter sent by the wes
         //-----------------------------------------------------
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& contractName,
                               const Poco::Int64& counter1,
                               const Poco::Int64& counter2,
                               const Poco::Int64& counter3,
                               const Poco::Int64& counter4,
                               const Poco::Int64& counter5,
                               const Poco::Int64& counter6);

         //-----------------------------------------------------
         ///\brief                     remove
         ///\param[in]   api          Yadoms API
         //-----------------------------------------------------
         void remove(boost::shared_ptr<yApi::IYPluginApi> api);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CTIC();

      private:
         //-----------------------------------------------------
         ///\brief                     initializeTIC
         ///\param[in]   api          Yadoms API
         //-----------------------------------------------------
         void initializeTIC(boost::shared_ptr<yApi::IYPluginApi> api);

         //-----------------------------------------------------
         ///\brief                     The device name
         //-----------------------------------------------------
         std::string m_deviceName;

         //-----------------------------------------------------
         ///\brief                     The device type
         //-----------------------------------------------------
         std::string m_deviceType;

         //-----------------------------------------------------
         ///\brief                     The contract name
         //-----------------------------------------------------
         std::string m_contractName;

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