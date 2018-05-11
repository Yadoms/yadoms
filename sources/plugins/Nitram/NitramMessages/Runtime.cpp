#include "stdafx.h"
#include "Runtime.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include "MessagesDefinition.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   CRuntime::CRuntime(std::string deviceName)
      : m_deviceName(deviceName),
      m_RuntimeDuration(boost::make_shared<yApi::historization::CDuration>("Runtime")),
      m_keywords({ m_RuntimeDuration }),
      m_RunTime(boost::posix_time::not_a_date_time),
      m_shouldBeHistorize(true)
   {
      m_RuntimeDuration->set(boost::posix_time::minutes(0));
   }

   void CRuntime::decode(boost::shared_ptr<yApi::IYPluginApi> api,
                         const shared::communication::CByteBuffer& buffer)
   {
      m_RunTime = boost::posix_time::seconds(buffer[2] + buffer[3] * 256);
      YADOMS_LOG(information) << "Runtime : " << m_RuntimeDuration->get();
      m_shouldBeHistorize = true;
   }

   CRuntime::~CRuntime()
   {}

   boost::shared_ptr<shared::communication::CByteBuffer> CRuntime::encode() const
   {
      shared::communication::CByteBuffer message(1);

      // The ID of the message
      message[0] = nitramMessages::ENitramMessageID::Runtime;
      return boost::make_shared<shared::communication::CByteBuffer>(message);
   }

   void CRuntime::historizeData(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      static bool firstHistorization = true;

      if ((m_shouldBeHistorize || firstHistorization) && (m_RuntimeDuration->get() != m_RunTime))
      {
         m_RuntimeDuration->set(m_RunTime);
         api->historizeData(m_deviceName, m_keywords);
         firstHistorization = false;
         m_shouldBeHistorize = false;
      }
   }

   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CRuntime::keywords() const
   {
      return m_keywords;
   }

   void CRuntime::setRunTimeIndefined()
   {
      m_RunTime = boost::posix_time::not_a_date_time;
      m_shouldBeHistorize = true;
   }

   bool CRuntime::onlyAck() const
   {
      return false;
   }
} // namespace nitramMessages