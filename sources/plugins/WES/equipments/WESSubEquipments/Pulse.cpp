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
            m_Pulse = temp;
            keywordsToDeclare.push_back(temp);
         }
         else if (m_unitName.compare("Litre") == 0 || m_unitName.compare("m3") == 0)
         {
            boost::shared_ptr<yApi::historization::CVolume> temp = boost::make_shared<yApi::historization::CVolume>(keywordName,
                                                                                                                    yApi::EKeywordAccessMode::kGet);
            m_Pulse = temp;
            keywordsToDeclare.push_back(temp);
         }
         else
         {
            YADOMS_LOG(information) << "No keywords created for pulse equipments";
         }
      }

      std::string CPulse::getDeviceName() const
      {
         return m_deviceName;
      }

      void CPulse::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api)
      {
         //TODO : If deviceName or contractName are different then create a new device
         //initializeTIC(api);
      }

      CPulse::~CPulse()
      {}
   }
}// namespace equipments::subdevices