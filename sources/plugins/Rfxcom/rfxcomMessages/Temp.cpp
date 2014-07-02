#include "stdafx.h"
#include "Temp.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CTemp::CTemp(const RBUF& buffer)
{
   // Some integrity controls
   if (buffer.TEMP.packetlength != TEMP_size)
      throw shared::exception::CInvalidParameter("TEMP size");
   if (buffer.TEMP.packettype != pTypeTEMP)
      throw shared::exception::CInvalidParameter("TEMP packettype");

   m_subType = buffer.TEMP.subtype;

   m_id = buffer.TEMP.id1 | (buffer.TEMP.id2 << 8);

   m_temperature = (float)((buffer.TEMP.temperatureh << 8) | buffer.TEMP.temperaturel) / 10;
   if (buffer.TEMP.tempsign)
      m_temperature = -m_temperature;

   m_batteryLevel = buffer.TEMP.battery_level == 0x09 ? 100 : 0;

   m_rssi = buffer.TEMP.rssi * 100 / 0x0F;

   buildDeviceName();
   buildDeviceModel();
}

CTemp::~CTemp()
{
}

const CByteBuffer CTemp::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   // Nothing to do (message type is read-only)
   return CByteBuffer();
}

void CTemp::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   if (!context->deviceExists(m_deviceName))
   {
      context->declareDevice(m_deviceName, m_deviceModel);
      context->declareKeyword(m_deviceName, "temperature", yApi::CStandardCapacities::Temperature);
      context->declareKeyword(m_deviceName, "battery", yApi::CStandardCapacities::BatteryLevel);
      context->declareKeyword(m_deviceName, "rssi", yApi::CStandardCapacities::Rssi);
   }

   context->historizeData(m_deviceName, "temperature", m_temperature);
   context->historizeData(m_deviceName, "battery", m_batteryLevel);
   context->historizeData(m_deviceName, "rssi", m_rssi);
}

void CTemp::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_id;
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
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages