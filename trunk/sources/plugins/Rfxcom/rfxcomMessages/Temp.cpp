#include "stdafx.h"
#include "Temp.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CTemp::CTemp(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_temperature("temperature"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeTEMP, GET_RBUF_STRUCT_SIZE(TEMP), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.TEMP.subtype;

   m_id = rbuf.TEMP.id1 | (rbuf.TEMP.id2 << 8);

   m_temperature.set(NormalizeTemperature(rbuf.TEMP.temperatureh, rbuf.TEMP.temperaturel, rbuf.TEMP.tempsign == 1));
   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.TEMP.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.TEMP.rssi));

   Init(context);
}

CTemp::~CTemp()
{
}

void CTemp::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeTEMP);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_temperature);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CTemp::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Temp is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Temp is a read-only message, can not be encoded");
}

void CTemp::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_temperature);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CTemp::getDeviceName() const
{
   return m_deviceName;
}                     

void CTemp::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CTemp::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeTEMP1: ssModel << "THR128/138, THC138"; break;
   case sTypeTEMP2: ssModel << "THC238/268,THN132,THWR288,THRN122,THN122,AW129/131"; break;
   case sTypeTEMP3: ssModel << "THWR800"; break;
   case sTypeTEMP4: ssModel << "RTHN318"; break;
   case sTypeTEMP5: ssModel << "La Crosse TX2, TX3, TX4, TX17"; break;
   case sTypeTEMP6: ssModel << "TS15C"; break;
   case sTypeTEMP7: ssModel << "Viking 02811"; break;
   case sTypeTEMP8: ssModel << "La Crosse WS2300"; break;
   case sTypeTEMP9: ssModel << "Rubicson"; break;
   case sTypeTEMP10: ssModel << "TFA 30.3133"; break;
   case sTypeTEMP11: ssModel << "WT0122"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages