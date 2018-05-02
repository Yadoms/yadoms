#include "stdafx.h"
#include "Temp.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CTemp::CTemp(boost::shared_ptr<yApi::IYPluginApi> api,
                const RBUF& rbuf,
                size_t rbufSize)
      : m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_temperature , m_batteryLevel , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeTEMP,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(TEMP),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.TEMP.subtype;

      m_id = rbuf.TEMP.id1 | (rbuf.TEMP.id2 << 8);

      m_temperature->set(NormalizeTemperature(rbuf.TEMP.temperatureh, rbuf.TEMP.temperaturel, rbuf.TEMP.tempsign == 1));
      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.TEMP.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.TEMP.rssi));

      buildDeviceModel();
      buildDeviceName();
   }

   CTemp::~CTemp()
   {
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CTemp::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Temp is a read-only message, can not be encoded");
   }

   void CTemp::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CTemp::filter() const
   {
   }

   void CTemp::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      shared::CDataContainer details;
      details.set("type", pTypeTEMP);
      details.set("subType", m_subType);
      details.set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      details.printToLog(YADOMS_LOG(information));
   }

   const std::string& CTemp::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CTemp::keywords()
   {
      return m_keywords;
   }

   void CTemp::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CTemp::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeTEMP1: ssModel << "THR128/138, THC138";
         break;
      case sTypeTEMP2: ssModel << "THC238/268,THN132,THWR288,THRN122,THN122,AW129/131";
         break;
      case sTypeTEMP3: ssModel << "THWR800";
         break;
      case sTypeTEMP4: ssModel << "RTHN318";
         break;
      case sTypeTEMP5: ssModel << "La Crosse TX2, TX3, TX4, TX17";
         break;
      case sTypeTEMP6: ssModel << "TS15C";
         break;
      case sTypeTEMP7: ssModel << "Viking 02811, TSS330";
         break;
      case sTypeTEMP8: ssModel << "La Crosse WS2300";
         break;
      case sTypeTEMP9: ssModel << "Rubicson";
         break;
      case sTypeTEMP10: ssModel << "TFA 30.3133";
         break;
      case sTypeTEMP11: ssModel << "WT0122";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
