#include "stdafx.h"
#include "Pulse.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CPulse::CPulse(boost::shared_ptr<yApi::IYPluginApi> api,
                     std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                     const std::string& deviceName,
                     const std::string& keywordName,
                     const std::string& unitName) :
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
         if (m_unitName.compare("Wh")==0 || m_unitName.compare("KWh") == 0)
         {
            m_pulseEnergy = boost::make_shared<yApi::historization::CEnergyDouble>(keywordName,
                                                                                   yApi::EKeywordAccessMode::kGet);
            keywordsToDeclare.push_back(m_pulseEnergy);
         }
         else if (m_unitName.compare("Litre") == 0 || m_unitName.compare("m3") == 0)
         {
            m_pulseVolume = boost::make_shared<yApi::historization::CVolume>(keywordName,
                                                                             yApi::EKeywordAccessMode::kGet);
            keywordsToDeclare.push_back(m_pulseVolume);
         }
         else
         {
            YADOMS_LOG(information) << "No keywords created for pulse equipments";
         }
      }

      void CPulse::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                       const std::string& unitName)
      {
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

         if (m_unitName.compare("Wh") == 0 || m_unitName.compare("KWh") == 0)
         {
            if (!m_pulseEnergy)
            {
               if (m_pulseVolume)
               {
                  YADOMS_LOG(trace) << m_pulseVolume->getKeyword() << " removed.";
                  api->removeKeyword(m_deviceName, m_keywordName);
                  m_pulseVolume.reset();
               }

               m_pulseEnergy = boost::make_shared<yApi::historization::CEnergyDouble>(m_keywordName,
                                                                                      yApi::EKeywordAccessMode::kGet);

               keywordsToDeclare.push_back(m_pulseEnergy);
            }
         }
         else if (m_unitName.compare("Litre") == 0 || m_unitName.compare("m3") == 0)
         {
            if (!m_pulseVolume)
            {
               if (m_pulseEnergy)
               {
                  YADOMS_LOG(trace) << m_pulseEnergy->getKeyword() << " removed.";
                  api->removeKeyword(m_deviceName, m_keywordName);
                  m_pulseEnergy.reset();
               }

               m_pulseVolume = boost::make_shared<yApi::historization::CVolume>(m_keywordName,
                                                                                yApi::EKeywordAccessMode::kGet);
            }
         }

         api->declareKeywords(m_deviceName, keywordsToDeclare);
      }

      void CPulse::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                                    const std::string& unitName,
                                    const double& total)
      {
         if (m_unitName != unitName)
            updateConfiguration(api, unitName);

         if (m_unitName.compare("Wh") == 0 || m_unitName.compare("KWh") == 0)
         {
            m_pulseEnergy->set(total);
            keywordsToHistorize.push_back(m_pulseEnergy);
            YADOMS_LOG(trace) << m_pulseEnergy->getKeyword() << " set to " << total;
         }
         else if (m_unitName.compare("Litre") == 0 || m_unitName.compare("m3") == 0)
         {
            //double totalValue = 0;
            //boost::regex regtotal("([+-]?([0-9]*[.])?[0-9]+) (.+)");  // TODO : to be checked

            //boost::smatch match;
            //if (boost::regex_search(total, match, regtotal))
            //{
            //   totalValue = boost::lexical_cast<double>(match[1]);
            //}

            m_pulseVolume->set(total);
            keywordsToHistorize.push_back(m_pulseVolume);
            YADOMS_LOG(trace) << m_pulseVolume->getKeyword() << " set to " << total;
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