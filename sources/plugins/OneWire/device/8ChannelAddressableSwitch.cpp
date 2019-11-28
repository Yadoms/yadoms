#include "stdafx.h"
#include "8ChannelAddressableSwitch.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
const boost::posix_time::time_duration C8ChannelAddressableSwitch::HistorizationPeriod(boost::posix_time::seconds(5));

C8ChannelAddressableSwitch::C8ChannelAddressableSwitch(EOneWireFamily family,
                                                       const std::string &id,
                                                       boost::shared_ptr<ioInterfaces::IMultiSwitch> io)
    : m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2408")),
      m_io(io)
{
   BOOST_ASSERT_MSG(m_identification->family() == k8ChannelAddressableSwitch, "Invalid family number");

   for (unsigned char i = 0; i < NbIo; ++i)
   {
      std::string keywordName("io_");
      keywordName.append(1, '0' + i);

      m_keyword[i] = boost::make_shared<yApi::historization::CSwitch>(keywordName);
      m_keywords.push_back(m_keyword[i]);
   }
}

void C8ChannelAddressableSwitch::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                  const shared::CDataContainer &configuration)
{
   YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
}

void C8ChannelAddressableSwitch::read() const
{
   for (unsigned char i = 0; i < NbIo; ++i)
      m_keyword[i]->set(m_io->read(i));
}

void C8ChannelAddressableSwitch::write(const std::string &keyword, const std::string &command)
{
   for (unsigned char i = 0; i < NbIo; ++i)
   {
      if (m_keyword[i]->getKeyword() == keyword)
      {
         if (m_keyword[i]->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
         {
            YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
            return;
         }
         m_keyword[i]->setCommand(command);
         m_io->write(i, m_keyword[i]->get());
      }
   }
}

bool C8ChannelAddressableSwitch::hasRelevantValue()
{
   if (m_keywords.empty())
      return false;

   const auto now = shared::currentTime::Provider().now();

   if (now < (m_lastHistorizationDate + HistorizationPeriod))
      return false;

   m_lastHistorizationDate = now;
   return true;
}
} // namespace device
