#include "stdafx.h"
#include "TIC.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CTIC::CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& deviceName) :
         m_deviceName(deviceName),
         m_baseCounter(boost::make_shared<yApi::historization::CEnergy>("BaseCounter")),
         m_lowCostCounter(boost::make_shared<yApi::historization::CEnergy>("LowCostCounter")),
         m_normalCostCounter(boost::make_shared<yApi::historization::CEnergy>("NormalCostCounter")),
         m_EJPPeakPeriod(boost::make_shared<yApi::historization::CEnergy>("EJPPeakPeriod")),
         m_EJPNormalPeriod(boost::make_shared<yApi::historization::CEnergy>("EJPNormalPeriod")),
         m_tempoBlueDaysLowCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoBlueDaysLowCostPeriod")),
         m_tempoBlueDaysNormalCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoBlueDaysNormalCostPeriod")),
         m_tempoRedDaysLowCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoRedDaysLowCostPeriod")),
         m_tempoRedDaysNormalCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoRedDaysNormalCostPeriod")),
         m_tempoWhiteDaysLowCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoWhiteDaysLowCostPeriod")),
         m_tempoWhiteDaysNormalCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoWhiteDaysNormalCostPeriod")),
         m_apparentPower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower")),
         m_TimePeriod(boost::make_shared<specificHistorizers::CPeriod>("RunningPeriod")),
         m_teleInfoStatus(boost::make_shared<specificHistorizers::CTeleInfoStatus>("Status"))
      {
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
         shared::CDataContainer details;

         //Déclaration of all IOs
         api->declareDevice(m_deviceName, "TeleInfo", keywordsToDeclare, details);
      }

      std::string CTIC::getDeviceName() const
      {
         return m_deviceName;
      }

      void CTIC::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api)
      {
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;
         std::string CGXfileName = "WESVALUES.CGX";
      }

      CTIC::~CTIC()
      {}
   }
}// namespace equipments::subdevices