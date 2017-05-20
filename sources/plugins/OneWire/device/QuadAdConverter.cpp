#include "stdafx.h"
#include "QuadAdConverter.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
   CQuadAdConverter::CQuadAdConverter(EOneWireFamily family,
                                      const std::string& id,
                                      boost::shared_ptr<ioInterfaces::IQuadAdConverter> io)
      :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2450")),
      m_io(io)
   {
      BOOST_ASSERT_MSG(m_identification->family() == kQuadAdConverter, "Invalid family number");

      for (auto i = 0; i < NbAdConverter; ++i)
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

   void CQuadAdConverter::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const shared::CDataContainer& configuration)
   {
      YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
   }

   void CQuadAdConverter::read() const
   {
      for (auto i = 0; i < NbAdConverter; ++i)
         m_keyword[i]->set(m_io->read(i));
   }

   void CQuadAdConverter::write(const std::string& keyword, const std::string& command)
   {
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
   }
} // namespace device
