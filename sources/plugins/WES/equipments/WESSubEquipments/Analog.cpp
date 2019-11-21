#include "stdafx.h"
#include "Analog.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CAnalog::CAnalog(boost::shared_ptr<yApi::IYPluginApi> api,
                       std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                       const std::string& type,
                       const std::string& deviceName,
                       const std::string& keywordName) :
         m_deviceName(deviceName),
         m_keywordName(keywordName),
         m_type(type)
      {
		 boost::shared_ptr<yApi::historization::IHistorizable> temp;

		 if (type == "temperature") {
			 m_TemperatureHistorizer = boost::make_shared<yApi::historization::CTemperature>(keywordName, yApi::EKeywordAccessMode::kGet);
			 temp = m_TemperatureHistorizer;
		 }
		 else if (type == "ampere") {
			 m_CurrentHistorizer = boost::make_shared<yApi::historization::CCurrent>(keywordName, yApi::EKeywordAccessMode::kGet);
			 temp = m_CurrentHistorizer;
		 }
		 else if (type == "voltage") {
			 m_VoltageHistorizer = boost::make_shared<yApi::historization::CVoltage>(keywordName, yApi::EKeywordAccessMode::kGet);
			 temp = m_VoltageHistorizer;
		 }
		 else if (type == "watthour") {
			 m_EnergyHistorizer = boost::make_shared<yApi::historization::CEnergy>(keywordName, yApi::EKeywordAccessMode::kGet);
			 temp = m_EnergyHistorizer;
		 }
		 keywordsToDeclare.push_back(temp);
      }

      void CAnalog::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                     std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                                     const double& Value)
      {
         boost::shared_ptr<yApi::historization::IHistorizable> temp;

         if (m_type == "temperature") {
            m_TemperatureHistorizer->set(Value);
            temp = m_TemperatureHistorizer;
         }
         else if (m_type == "ampere") {
            m_CurrentHistorizer->set(Value);
            temp = m_CurrentHistorizer;
         }
         else if (m_type == "voltage") {
            m_VoltageHistorizer->set(Value);
            temp = m_VoltageHistorizer;
         }
         else if (m_type == "watthour") {
            m_EnergyHistorizer->set((Poco::Int64)Value);
            temp = m_EnergyHistorizer;
         }

         keywordsToHistorize.push_back(temp);
         YADOMS_LOG(trace) << temp->getKeyword() << " set to " << Value;
      }

      std::string CAnalog::name()
      {
         return m_keywordName;
      }

      std::string CAnalog::type()
      {
         return m_type;
      }

      CAnalog::~CAnalog()
      {}
   }
}// namespace equipments::subdevices