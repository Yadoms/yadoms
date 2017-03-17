#include "stdafx.h"
#include "CartelectronicEncoder.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicEncoder::CCartelectronicEncoder(const RBUF& rbuf,
                                                  size_t rbufSize)
      : m_id(0),
      m_counter1(boost::make_shared<yApi::historization::CCounter>("counter1")),
      m_counter2(boost::make_shared<yApi::historization::CCounter>("counter2")),
      m_keywords({ m_counter1, m_counter2 })
   {
      CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeCEencoder, GET_RBUF_STRUCT_SIZE(CEENCODER), DONT_CHECK_SEQUENCE_NUMBER);

      m_counter1->set((rbuf.CEENCODER.counter1_0 << 24) + (rbuf.CEENCODER.counter1_1 << 16) + (rbuf.CEENCODER.counter1_2 << 8) + (rbuf.CEENCODER.counter1_3));
      m_counter2->set((rbuf.CEENCODER.counter2_0 << 24) + (rbuf.CEENCODER.counter2_1 << 16) + (rbuf.CEENCODER.counter2_2 << 8) + (rbuf.CEENCODER.counter2_3));
   }

   CCartelectronicEncoder::~CCartelectronicEncoder()
   {
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CCartelectronicEncoder::keywords() const
   {
      return m_keywords;
   }

   std::string CCartelectronicEncoder::idFromProtocol(const RBUF& rbuf) const
   {
      unsigned long i_id;
      std::stringstream s_id;

      i_id = static_cast<unsigned long>(rbuf.CEENCODER.id1 << 24) + (rbuf.CEENCODER.id2 << 16) + (rbuf.CEENCODER.id3 << 8) + rbuf.CEENCODER.id4;
      s_id << static_cast<unsigned long>(i_id);

      return s_id.str();
   }

   char CCartelectronicEncoder::BatteryLevelFromProtocol(const RBUF& rbuf)
   {
      return rbuf.CEENCODER.battery_level;
   }

   char CCartelectronicEncoder::signalStrengthFromProtocol(const RBUF& rbuf)
   {
      return rbuf.CEENCODER.signalStrength;
   }

   std::string CCartelectronicEncoder::getModel() const
   {
      return "Encoder Module";
   }
} // namespace rfxcomMessages


