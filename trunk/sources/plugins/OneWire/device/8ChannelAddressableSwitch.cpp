#include "stdafx.h"
#include "8ChannelAddressableSwitch.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {
   
C8ChannelAddressableSwitch::C8ChannelAddressableSwitch(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IMultiSwitch> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2408")), m_context(context), m_io(io)
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

C8ChannelAddressableSwitch::~C8ChannelAddressableSwitch()
{
}

boost::shared_ptr<const IIdentification> C8ChannelAddressableSwitch::ident() const
{
   return m_identification;
}

void C8ChannelAddressableSwitch::declare()
{
   m_identification->declare(m_context);

   for (unsigned char i = 0; i < NbIo; ++i)
      if (!m_context->keywordExists(m_identification->deviceName(), *m_keyword[i]))
         m_context->declareKeyword(m_identification->deviceName(), *m_keyword[i]);
}

void C8ChannelAddressableSwitch::historize()
{
   for (unsigned char i = 0; i < NbIo; ++i)
      m_keyword[i]->set(m_io->read(i));

   m_context->historizeData(ident()->deviceName(), m_keywords);
}

void C8ChannelAddressableSwitch::set(const std::string& keyword, const std::string& command)
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

} // namespace device
