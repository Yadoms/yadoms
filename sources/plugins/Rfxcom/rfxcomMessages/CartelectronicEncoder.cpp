#include "stdafx.h"
#include "CartelectronicEncoder.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicEncoder::CCartelectronicEncoder(const RBUF& rbuf,
                                                  size_t rbufSize)
      : m_counterKw1(boost::make_shared<yApi::historization::CCounter>("1 - counter")),
        m_energyKw1(boost::make_shared<yApi::historization::CEnergy>("1 - energy")),
        m_waterLiterKw1(boost::make_shared<yApi::historization::CVolume>("1 - water")),
        m_waterQuarterLiterKw1(boost::make_shared<yApi::historization::CVolume>("1 - water")),
        m_gasDeciM3Kw1(boost::make_shared<yApi::historization::CVolume>("1 - gas")),
        m_gasCentiM3Kw1(boost::make_shared<yApi::historization::CVolume>("1 - gas")),
        m_counterKw2(boost::make_shared<yApi::historization::CCounter>("2 - counter")),
        m_energyKw2(boost::make_shared<yApi::historization::CEnergy>("2 - energy")),
        m_waterLiterKw2(boost::make_shared<yApi::historization::CVolume>("2 - water")),
        m_waterQuarterLiterKw2(boost::make_shared<yApi::historization::CVolume>("2 - water")),
        m_gasDeciM3Kw2(boost::make_shared<yApi::historization::CVolume>("2 - gas")),
        m_gasCentiM3Kw2(boost::make_shared<yApi::historization::CVolume>("2 - gas"))
   {
      CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeCEencoder, GET_RBUF_STRUCT_SIZE(CEENCODER), DONT_CHECK_SEQUENCE_NUMBER);

      const unsigned int counter1 =
         (rbuf.CEENCODER.counter1_0 << 24)
         + (rbuf.CEENCODER.counter1_1 << 16)
         + (rbuf.CEENCODER.counter1_2 << 8)
         + rbuf.CEENCODER.counter1_3;
      const unsigned int counter2 =
         (rbuf.CEENCODER.counter2_0 << 24)
         + (rbuf.CEENCODER.counter2_1 << 16)
         + (rbuf.CEENCODER.counter2_2 << 8)
         + rbuf.CEENCODER.counter2_3;

      m_counterKw1->set(counter1);
      m_energyKw1->set(counter1);
      m_waterLiterKw1->set(static_cast<double>(counter1) / 1000.0); // counter gives liters when m_waterLiter is in m3
      m_waterQuarterLiterKw1->set(static_cast<double>(counter1) / 4000.0); // counter gives 1/4 liters when m_waterQuarterLiter is in m3
      m_gasDeciM3Kw1->set(static_cast<double>(counter1) / 10.0); // counter gives 1/10 m3 when m_waterQuarterLiter is in m3
      m_gasCentiM3Kw1->set(static_cast<double>(counter1) / 100.0); // counter gives 1/100 m3 when m_waterQuarterLiter is in m3

      m_counterKw2->set(counter2);
      m_energyKw2->set(counter2);
      m_waterLiterKw2->set(static_cast<double>(counter2) / 1000.0); // counter gives liters when m_waterLiter is in m3
      m_waterQuarterLiterKw2->set(static_cast<double>(counter2) / 4000.0); // counter gives 1/4 liters when m_waterQuarterLiter is in m3
      m_gasDeciM3Kw2->set(static_cast<double>(counter1) / 10.0); // counter gives 1/10 m3 when m_waterQuarterLiter is in m3
      m_gasCentiM3Kw2->set(static_cast<double>(counter1) / 100.0); // counter gives 1/100 m3 when m_waterQuarterLiter is in m3
   }

   CCartelectronicEncoder::CCartelectronicEncoder(const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& deviceConfiguration)
      : m_counterKw1(boost::make_shared<yApi::historization::CCounter>("1 - counter")),
        m_energyKw1(boost::make_shared<yApi::historization::CEnergy>("1 - energy")),
        m_waterLiterKw1(boost::make_shared<yApi::historization::CVolume>("1 - water")),
        m_waterQuarterLiterKw1(boost::make_shared<yApi::historization::CVolume>("1 - water")),
        m_gasDeciM3Kw1(boost::make_shared<yApi::historization::CVolume>("1 - gas")),
        m_gasCentiM3Kw1(boost::make_shared<yApi::historization::CVolume>("1 - gas")),
        m_counterKw2(boost::make_shared<yApi::historization::CCounter>("2 - counter")),
        m_energyKw2(boost::make_shared<yApi::historization::CEnergy>("2 - energy")),
        m_waterLiterKw2(boost::make_shared<yApi::historization::CVolume>("2 - water")),
        m_waterQuarterLiterKw2(boost::make_shared<yApi::historization::CVolume>("2 - water")),
        m_gasDeciM3Kw2(boost::make_shared<yApi::historization::CVolume>("2 - gas")),
        m_gasCentiM3Kw2(boost::make_shared<yApi::historization::CVolume>("2 - gas"))
   {
   }

   CCartelectronicEncoder::~CCartelectronicEncoder()
   {
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CCartelectronicEncoder::keywords(
      const boost::shared_ptr<shared::CDataContainer>& deviceConfiguration)
   {
      m_keywords.clear();

      if (!deviceConfiguration->empty() && deviceConfiguration->exists("counter1Configuration"))
      {
         const auto counter1Configuration = deviceConfiguration->get<std::string>("counter1Configuration");
         if (counter1Configuration == "energyWh")
            m_keywords.push_back(m_energyKw1);
         else if (counter1Configuration == "waterLiter")
            m_keywords.push_back(m_waterLiterKw1);
         else if (counter1Configuration == "waterQuarterLiter")
            m_keywords.push_back(m_waterQuarterLiterKw1);
         else if (counter1Configuration == "gasDeciM3")
            m_keywords.push_back(m_gasDeciM3Kw1);
         else if (counter1Configuration == "gasCentiM3")
            m_keywords.push_back(m_gasCentiM3Kw1);
         else
            m_keywords.push_back(m_counterKw1);

         const auto counter2Configuration = deviceConfiguration->get<std::string>("counter2Configuration");
         if (counter2Configuration == "energyWh")
            m_keywords.push_back(m_energyKw2);
         else if (counter2Configuration == "waterLiter")
            m_keywords.push_back(m_waterLiterKw2);
         else if (counter2Configuration == "waterQuarterLiter")
            m_keywords.push_back(m_waterQuarterLiterKw2);
         else if (counter2Configuration == "gasDeciM3")
            m_keywords.push_back(m_gasDeciM3Kw2);
         else if (counter2Configuration == "gasCentiM3")
            m_keywords.push_back(m_gasCentiM3Kw2);
         else
            m_keywords.push_back(m_counterKw2);
      }
      else
      {
         m_keywords.push_back(m_counterKw1);
         m_keywords.push_back(m_counterKw2);
      }

      return m_keywords;
   }

   std::string CCartelectronicEncoder::idFromProtocol(const RBUF& rbuf) const
   {
      std::stringstream sId;

      const auto iId = static_cast<unsigned long>(rbuf.CEENCODER.id1 << 24) + (rbuf.CEENCODER.id2 << 16) + (rbuf.CEENCODER.id3 << 8) + rbuf.CEENCODER.
                                                                                                                                            id4;
      sId << static_cast<unsigned long>(iId);

      return sId.str();
   }

   char CCartelectronicEncoder::batteryLevelFromProtocol(const RBUF& rbuf)
   {
      return rbuf.CEENCODER.battery_level;
   }

   char CCartelectronicEncoder::signalPowerFromProtocol(const RBUF& rbuf)
   {
      return rbuf.CEENCODER.rssi;
   }

   std::string CCartelectronicEncoder::getModel() const
   {
      return "Encoder Module";
   }
} // namespace rfxcomMessages
