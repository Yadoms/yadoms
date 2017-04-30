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
         m_unitName(unitName)
      {
         // TODO : Add need to declare
         initializePulse(api, keywordsToDeclare, keywordName);
      }

      void CPulse::initializePulse(boost::shared_ptr<yApi::IYPluginApi> api, 
                                   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                                   const std::string& keywordName)
      {
         // TODO : To be continued
         if (m_unitName.compare("Wh")==0 || m_unitName.compare("KWh") == 0)
         {
            boost::shared_ptr<yApi::historization::CEnergy> temp = boost::make_shared<yApi::historization::CEnergy>(keywordName,
                                                                                                                    yApi::EKeywordAccessMode::kGet);
            m_pulseEnergy = temp;
            keywordsToDeclare.push_back(temp);
         }
         else if (m_unitName.compare("Litre") == 0 || m_unitName.compare("m3") == 0)
         {
            boost::shared_ptr<yApi::historization::CVolume> temp = boost::make_shared<yApi::historization::CVolume>(keywordName,
                                                                                                                    yApi::EKeywordAccessMode::kGet);
            m_pulseVolume = temp;
            keywordsToDeclare.push_back(temp);
         }
         else
         {
            YADOMS_LOG(information) << "No keywords created for pulse equipments";
         }
      }

      void CPulse::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                                    const std::string& unitName,
                                    const Poco::Int64& flowValue,
                                    const std::string& total)
      {
         //TODO : If deviceName or contractName are different then create a new device
         //initializeTIC(api);

         if (m_unitName.compare(unitName) == 0)
         {
            if (m_unitName.compare("Wh") == 0 || m_unitName.compare("KWh") == 0)
            {
               m_pulseEnergy->set(flowValue);
               keywordsToHistorize.push_back(m_pulseEnergy);
               YADOMS_LOG(trace) << m_pulseEnergy->getKeyword() << " set to " << flowValue;
            }
            else if (m_unitName.compare("Litre") == 0 || m_unitName.compare("m3") == 0)
            {
               double totalValue = 0;
               boost::regex regtotal("([+-]?([0-9]*[.])?[0-9]+) (.+)");

               boost::smatch match;
               if (boost::regex_search(total, match, regtotal))
               {
                  totalValue = boost::lexical_cast<double>(match[1]);
               }

               m_pulseVolume->set(totalValue);
               keywordsToHistorize.push_back(m_pulseVolume);
               YADOMS_LOG(trace) << m_pulseVolume->getKeyword() << " set to " << totalValue;
            }
         }
      }

      CPulse::~CPulse()
      {}
   }
}// namespace equipments::subdevices