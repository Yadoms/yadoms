#include "stdafx.h"
#include "Clamp.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CClamp::CClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                     std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                     const std::string& deviceName,
                     const std::string& keywordName) :
         m_deviceName(deviceName),
         m_keywordName(keywordName)
      {
         initializeClamp(api, keywordsToDeclare, keywordName);
      }

      void CClamp::initializeClamp(boost::shared_ptr<yApi::IYPluginApi> api,
                                   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                                   const std::string& keywordName)
      {
         m_CounterClamp = boost::make_shared<yApi::historization::CEnergy>(keywordName,
                                                                           yApi::EKeywordAccessMode::kGet);
         keywordsToDeclare.push_back(m_CounterClamp);
      }

      void CClamp::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                                    const Poco::Int64& energyClampValue)
      {
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