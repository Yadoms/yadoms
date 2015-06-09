#include "stdafx.h"
#include "QuadAdConverter.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {

CQuadAdConverter::CQuadAdConverter(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IQuadAdConverter> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2450")), m_context(context), m_io(io)
{
   BOOST_ASSERT_MSG(m_identification->family() == kQuadAdConverter, "Invalid family number");


   for (unsigned char i = 0; i < NbAdConverter; ++i)
   {
      std::string keywordName("channel_");
      keywordName.append(1, '0' + i);

      m_keyword[i] = boost::make_shared<yApi::historization::CVoltage>(keywordName);
      m_keywords.push_back(m_keyword[i]);
   }
}

CQuadAdConverter::~CQuadAdConverter()
{
}

boost::shared_ptr<const IIdentification> CQuadAdConverter::ident() const
{
   return m_identification;
}

void CQuadAdConverter::declare()
{
   m_identification->declare(m_context);

   for (unsigned char i = 0; i < NbAdConverter; ++i)
      if (!m_context->keywordExists(m_identification->deviceName(), *m_keyword[i]))
         m_context->declareKeyword(m_identification->deviceName(), *m_keyword[i]);
}

void CQuadAdConverter::historize()
{
   for (unsigned char i = 0; i < NbAdConverter; ++i)
      m_keyword[i]->set(m_io->read(i));

   m_context->historizeData(ident()->deviceName(), m_keywords);
}

void CQuadAdConverter::set(const std::string& keyword, const std::string& command)
{
   YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
}

} // namespace device