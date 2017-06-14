#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/Period.h"
#include "../specificHistorizers/Color.h"
#include "../specificHistorizers/TeleInfoStatus.h"
#include "../specificHistorizers/deviceStatus.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   namespace subdevices
   {
      //-----------------------------------------------------
      ///\brief                     Contracts list
      //-----------------------------------------------------
      typedef enum {
         Base,
         HpHc,
         Ejp,
         Tempo,
         BT4SUP36,
         BJEJP,
         NotAvailable
      }ContractAvailable;

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
         ///\param[in] counterId      The counter Id with 12 digits
         ///\param[in] contract       The contract subscribe
         //-----------------------------------------------------
         CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& deviceName,
              const std::string& counterId,
              const ContractAvailable contract);

         //-----------------------------------------------------
         ///\brief                                      updateFromDevice
         ///\param[in] api                             Yadoms API
         ///\param[in] newState                        state of the device
         ///\param[in] contractName                    contract Name
         ///\param[in] timePeriod                      the time period of the moment
         ///\param[in] apparentPower                   apparent power of the counter
         ///\param[in] counter1                        1st counter sent by the wes
         ///\param[in] counter2                        2nd counter sent by the wes
         ///\param[in] counter3                        3td counter sent by the wes
         ///\param[in] counter4                        4th counter sent by the wes
         ///\param[in] counter5                        5th counter sent by the wes
         ///\param[in] counter6                        6th counter sent by the wes
         //-----------------------------------------------------
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                               specificHistorizers::EWESdeviceStatus newState,
                               const ContractAvailable contractName,
                               const std::string& counterId,
                               const int timePeriod,
                               const unsigned int apparentPower,
                               const Poco::Int64& counter1,
                               const Poco::Int64& counter2,
                               const Poco::Int64& counter3,
                               const Poco::Int64& counter4,
                               const Poco::Int64& counter5,
                               const Poco::Int64& counter6,
                               const specificHistorizers::EColor newColor);

         //-----------------------------------------------------
         ///\brief                     remove
         ///\param[in]   api          Yadoms API
         //-----------------------------------------------------
         void remove(boost::shared_ptr<yApi::IYPluginApi> api);

         //-----------------------------------------------------
         ///\brief                     remove
         ///\param[in]   api          Yadoms API
         ///\param[in]   newState     new state of the device
         //-----------------------------------------------------
         void setDeviceState(boost::shared_ptr<yApi::IYPluginApi> api,
                             specificHistorizers::EWESdeviceStatus newState);

         //-----------------------------------------------------
         ///\brief                     get the device name
         ///\return                    the TIC device Name
         //-----------------------------------------------------
         std::string name();

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CTIC();

      private:
         //-----------------------------------------------------
         ///\brief                     initializeTIC
         ///\param[in]   api          Yadoms API
         ///\param[in]   counterId    the counterId with 12 digits
         //-----------------------------------------------------
         void initializeTIC(boost::shared_ptr<yApi::IYPluginApi> api,
                            const std::string& counterId);

         //-----------------------------------------------------
         ///\brief                     set the state of the device only when this one change
         ///\param[in]   newState          Yadoms API
         //-----------------------------------------------------
         void setDeviceState(specificHistorizers::EWESdeviceStatus newState);

         //-----------------------------------------------------
         ///\brief                     set the period time. The value depends of the contract
         ///\param[in]   period          the period number
         //-----------------------------------------------------
         void setPeriodTime(const int period);

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
         ContractAvailable m_contractName;

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
         boost::shared_ptr<specificHistorizers::CColor> m_Color;

         //--------------------------------------------------------------
         /// \brief	TeleInfo Status
         //--------------------------------------------------------------
         boost::shared_ptr<specificHistorizers::CTeleInfoStatus> m_teleInfoStatus;

         //--------------------------------------------------------------
         /// \brief	status of the device
         //--------------------------------------------------------------
         boost::shared_ptr<specificHistorizers::CdeviceStatus> m_deviceStatus;
      };
   }
} // namespace equipments::subdevices