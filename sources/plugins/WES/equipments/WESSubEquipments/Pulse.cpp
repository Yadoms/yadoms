#include "stdafx.h"
#include "Pulse.h"
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CPulse::CPulse(boost::shared_ptr<yApi::IYPluginApi> api,
                     std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                     const std::string& deviceName,
                     const std::string& keywordName,
                     const EUnit unitName) :
         m_deviceName(deviceName),
         m_keywordName(keywordName),
         m_unitName(unitName)
      {
         initializePulse(api, keywordsToDeclare, keywordName);
      }

      void CPulse::initializePulse(boost::shared_ptr<yApi::IYPluginApi> api, 
                                   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                                   const std::string& keywordName)
      {
         switch (m_unitName)
         {
         case undefined:
            YADOMS_LOG(information) << "this input (" << m_keywordName << ") is not used. It is desactivated";
            break;
         case Wh:
         case KWh:
            m_pulseEnergy = boost::make_shared<yApi::historization::CEnergy>(keywordName,
                                                                             yApi::EKeywordAccessMode::kGet);
            keywordsToDeclare.push_back(m_pulseEnergy);
            break;
         case litre:
         case m3:
            m_pulseVolume = boost::make_shared<yApi::historization::CVolume>(keywordName,
                                                                             yApi::EKeywordAccessMode::kGet,
                                                                             yApi::historization::EMeasureType::kCumulative);
            keywordsToDeclare.push_back(m_pulseVolume);
            break;
         default:
            YADOMS_LOG(information) << "No keywords created for pulse equipments";
            break;
         }
      }

      void CPulse::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                       const EUnit unitName)
      {
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

         switch (unitName)
         {
         case undefined:
            YADOMS_LOG(trace) << "this input (" << m_keywordName << ") is desactivated";
            break;
         case Wh:
         case KWh:
            if (!m_pulseEnergy)
            {
               if (m_pulseVolume)
               {
                  YADOMS_LOG(trace) << m_pulseVolume->getKeyword() << " removed";
                  api->removeKeyword(m_deviceName, m_keywordName);
                  m_pulseVolume.reset();
               }

               m_pulseEnergy = boost::make_shared<yApi::historization::CEnergy>(m_keywordName,
                                                                                yApi::EKeywordAccessMode::kGet);

               keywordsToDeclare.push_back(m_pulseEnergy);
            }
            break;
         case litre:
         case m3:
            if (!m_pulseVolume)
            {
               if (m_pulseEnergy)
               {
                  YADOMS_LOG(trace) << m_pulseEnergy->getKeyword() << " removed";
                  api->removeKeyword(m_deviceName, m_keywordName);
                  m_pulseEnergy.reset();
               }

               m_pulseVolume = boost::make_shared<yApi::historization::CVolume>(m_keywordName,
                                                                                yApi::EKeywordAccessMode::kGet);
            }
            break;
         default:
            YADOMS_LOG(error) << "This unit is not handle";
            break;
         }

         api->declareKeywords(m_deviceName, keywordsToDeclare);
      }

      void CPulse::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                                    const EUnit unitName,
                                    const double& total)
      {
         if (m_unitName != unitName)
            updateConfiguration(api, unitName);

         switch (unitName)
         {
         case undefined:
            YADOMS_LOG(trace) << "this input (" << m_keywordName << ") is desactivated";
            break;
         case Wh:
            m_pulseEnergy->set(total);
            keywordsToHistorize.push_back(m_pulseEnergy);
            YADOMS_LOG(trace) << m_pulseEnergy->getKeyword() << " set to " << total << "Wh";
            break;
         case KWh:
            m_pulseEnergy->set(total*1000);
            keywordsToHistorize.push_back(m_pulseEnergy);
            YADOMS_LOG(trace) << m_pulseEnergy->getKeyword() << " set to " << total << "Kwh";
            break;
         case litre:
            m_pulseVolume->set(total/1000);
            keywordsToHistorize.push_back(m_pulseVolume);
            YADOMS_LOG(trace) << m_pulseVolume->getKeyword() << " set to " << total << "liter";
            break;
         case m3:
            m_pulseVolume->set(total);
            keywordsToHistorize.push_back(m_pulseVolume);
            YADOMS_LOG(trace) << m_pulseVolume->getKeyword() << " set to " << total << "m3";
            break;
         default:
            YADOMS_LOG(error) << "This unit is not handle";
            break;
         }
      }

      std::string CPulse::name()
      {
         return m_keywordName;
      }

      CPulse::~CPulse()
      {}
   }
}// namespace equipments::subdevices