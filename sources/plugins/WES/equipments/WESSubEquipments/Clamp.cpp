#include "stdafx.h"
#include "Clamp.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CClamp::CClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                     std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                     const bool isInstantCurrentClampRegistered,
                     const std::string& deviceName,
                     const std::string& keywordName) :
         m_deviceName(deviceName),
         m_keywordName(keywordName),
         m_instantCurrentRegistered(isInstantCurrentClampRegistered)
      {
         initializeClamp(api, keywordsToDeclare, keywordName);
      }

      void CClamp::initializeClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                                   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                                   const std::string& keywordName)
      {
         if (m_instantCurrentRegistered)
         {
            YADOMS_LOG(information) << "create keyword " << "I - " + keywordName;
            m_CurrentClamp = boost::make_shared<yApi::historization::CCurrent>("I - " + keywordName,
                                                                               yApi::EKeywordAccessMode::kGet);
            keywordsToDeclare.push_back(m_CurrentClamp);
         }

         m_CounterClamp = boost::make_shared<yApi::historization::CEnergy>("Wh - " + keywordName,
                                                                           yApi::EKeywordAccessMode::kGet);
         keywordsToDeclare.push_back(m_CounterClamp);
      }

      void CClamp::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                       const bool isInstantCurrentClampRegistered)
      {
         m_instantCurrentRegistered = isInstantCurrentClampRegistered;

         // Declare or remove the keyword independantly
         if (m_instantCurrentRegistered)
         {
            m_CurrentClamp = boost::make_shared<yApi::historization::CCurrent>("I - " + m_keywordName,
                                                                               yApi::EKeywordAccessMode::kGet);
            api->declareKeyword(m_deviceName, m_CurrentClamp);
            YADOMS_LOG(trace) << "Create keyword " << "I - " + m_keywordName;
         }
         else
         {
            if (m_CurrentClamp)
            {
               if (api->keywordExists(m_deviceName, m_CurrentClamp))
               {
                  api->removeKeyword(m_deviceName, "I - " + m_keywordName);
                  m_CurrentClamp.reset();
                  YADOMS_LOG(trace) << "Delete keyword " << "I - " + m_keywordName;
               }
            }
         }
      }

      void CClamp::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                                    const double& instantCurrentValue,
                                    const Poco::Int64& energyClampValue)
      {
         if (m_instantCurrentRegistered)
         {
            m_CurrentClamp->set(instantCurrentValue);
            keywordsToHistorize.push_back(m_CurrentClamp);
            YADOMS_LOG(trace) << m_CurrentClamp->getKeyword() << " set to " << instantCurrentValue;
         }

         m_CounterClamp->set(energyClampValue);
         keywordsToHistorize.push_back(m_CounterClamp);
         YADOMS_LOG(trace) << m_CounterClamp->getKeyword() << " set to " << energyClampValue;
      }

      std::string CClamp::name()
      {
         return m_keywordName;
      }

      CClamp::~CClamp()
      {}
   }
}// namespace equipments::subdevices