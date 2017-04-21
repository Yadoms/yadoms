#include "stdafx.h"
#include "Clamp.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CClamp::CClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                     std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                     const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                     const bool isInstantCurrentClampRegistered,
                     const std::string& deviceName,
                     const std::string& keywordName) :
         m_deviceName(deviceName),
         m_keywordName(keywordName)
      {
         // TODO : Add need to declare
         initializeClamp(api, keywordsToDeclare, pluginConfiguration, isInstantCurrentClampRegistered, keywordName);
      }

      void CClamp::initializeClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                                   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                                   const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                                   const bool isInstantCurrentClampRegistered,
                                   const std::string& keywordName)
      {
         if (isInstantCurrentClampRegistered)
         {
            boost::shared_ptr<yApi::historization::CCurrent> tempCurrent = boost::make_shared<yApi::historization::CCurrent>("I - " + keywordName,
                                                                                                                             yApi::EKeywordAccessMode::kGet);
            m_CurrentClamp = tempCurrent;
            keywordsToDeclare.push_back(tempCurrent);
         }
         else
         {
            if (m_CurrentClamp)
            {
               if (api->keywordExists(m_deviceName, m_CurrentClamp))
               {
                  api->removeKeyword(m_deviceName, "I - " + keywordName);
               }
            }
         }
         boost::shared_ptr<yApi::historization::CEnergy>  tempCounter = boost::make_shared<yApi::historization::CEnergy>("Wh - " + keywordName,
                                                                                                                         yApi::EKeywordAccessMode::kGet);

         m_CounterClamp = tempCounter;
         keywordsToDeclare.push_back(tempCounter);
      }

      std::string CClamp::getDeviceName() const
      {
         return m_deviceName;
      }

      void CClamp::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                                    const bool isInstantCurrentClampRegistered,
                                    const std::string& instantCurrent,
                                    const std::string& energyClamp)
      {
         //TODO : If deviceName or contractName are different then create a new device
         //initializeTIC(api);

         if (isInstantCurrentClampRegistered)
         {
            double instantCurrentValue = 0;
            boost::regex regInstantCurrent("([+-]?([0-9]*[.])?[0-9]+) (.+)");

            boost::smatch match;
            if (boost::regex_search(instantCurrent, match, regInstantCurrent))
            {
               instantCurrentValue = boost::lexical_cast<double>(match[1]);
            }

            YADOMS_LOG(trace) << m_CurrentClamp->getKeyword() << " set to " << instantCurrentValue;

            m_CurrentClamp->set(instantCurrentValue);
            keywordsToHistorize.push_back(m_CurrentClamp);
         }

         long energyClampValue = 0;
         boost::regex regEnergy("([+-]?(\\d+)) (.+)");

         boost::smatch match;
         if (boost::regex_search(energyClamp, match, regEnergy))
         {
            energyClampValue = boost::lexical_cast<long>(match[1]);
         }

         YADOMS_LOG(trace) << m_CounterClamp->getKeyword() << " set to " << energyClampValue;

         m_CounterClamp->set(energyClampValue);
         keywordsToHistorize.push_back(m_CounterClamp);
      }

      CClamp::~CClamp()
      {}
   }
}// namespace equipments::subdevices