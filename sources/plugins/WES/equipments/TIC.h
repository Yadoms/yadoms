#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "specificHistorizers/Color.h"
#include "specificHistorizers/TeleInfoStatus.h"
#include "specificHistorizers/deviceStatus.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   //-----------------------------------------------------
   ///\brief                     Contracts list
   //-----------------------------------------------------
   typedef enum {
      Base,
      HpHc,
      Ejp,
      Tempo,
	  Producteur,
	  HC_Et_WE,
	  HC_SEM_WE_MERCR,
	  TJEJP,
      BT4SUP36,
	  BT5SUP36,
      BASEA5,
	  HTA5,
      NotAvailable
   }ContractAvailable;

   //-----------------------------------------------------
   ///\brief WES equipment
   //-----------------------------------------------------
   class CTIC
   {
   public:
	   //-----------------------------------------------------
	   ///\brief                     Nb counters handle by the class
	   //-----------------------------------------------------
	   static const Poco::Int32 TICCountersNb;

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
      ///\brief                     Constructor when in error at startup
      ///\param[in]   api          Yadoms API
      ///\param[in] device         The device name
      ///\param[in] parentName     The contract subscribe
      //-----------------------------------------------------
      CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
           const std::string& deviceName);

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
		                      const std::vector<Poco::Int64>& counters,
                              const int newColor);

	  //-----------------------------------------------------
	  ///\brief                     remove
	  ///\param[in]   api          Yadoms API
	  //-----------------------------------------------------
	  void initialize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& counterId);

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
      ///\brief                     set the state of the device only when this one change
      ///\param[in]   newState          Yadoms API
      //-----------------------------------------------------
      void setDeviceState(specificHistorizers::EWESdeviceStatus newState);

      //-----------------------------------------------------
      ///\brief                     set the period time. The value depends of the contract
	  ///\param[in]   contract        the contract number
      ///\param[in]   period          the period number
      //-----------------------------------------------------
      void setPeriodTime(const ContractAvailable contractName, const int period);

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
      /// \brief  Keywords
      //--------------------------------------------------------------
	  boost::shared_ptr<yApi::historization::CEnergy> m_Counter[10];
      boost::shared_ptr<yApi::historization::CApparentPower> m_apparentPower;
	  boost::shared_ptr<yApi::historization::IHistorizable> m_runningPeriod;
      boost::shared_ptr<specificHistorizers::CColor> m_tomorrowColor;

      //--------------------------------------------------------------
      /// \brief	TeleInfo Status
      //--------------------------------------------------------------
      boost::shared_ptr<specificHistorizers::CTeleInfoStatus> m_teleInfoStatus;

      //--------------------------------------------------------------
      /// \brief	status of the device
      //--------------------------------------------------------------
      boost::shared_ptr<specificHistorizers::CdeviceStatus> m_deviceStatus;

      //--------------------------------------------------------------
      /// \brief  Keywords list
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

	  //--------------------------------------------------------------
	  /// \brief  Counter keyword list names
	  //--------------------------------------------------------------
	  static const std::map<ContractAvailable, std::vector<std::string>> m_counterNames;
   };
} // namespace equipments::subdevices