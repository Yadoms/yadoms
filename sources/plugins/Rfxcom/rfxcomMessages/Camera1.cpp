#include "stdafx.h"
#include "Camera1.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCamera1::CCamera1(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_camera("camera"), m_rssi("rssi")
{
   m_camera.setCommand(command);
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_houseCode = deviceDetails.get<unsigned char>("houseCode");

   Init(context);
}

CCamera1::CCamera1(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_camera("camera"), m_rssi("rssi")
{
   m_camera.set(yApi::historization::ECameraMoveCommand::kCenterPositionValue);
   m_rssi.set(0);

   m_subType = subType;
   if (m_subType != sTypeNinja)
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

   m_houseCode = (unsigned char) manuallyDeviceCreationConfiguration.get<char>("houseCode");

   Init(context);
}

CCamera1::CCamera1(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_camera("camera"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeCamera, GET_RBUF_STRUCT_SIZE(CAMERA1), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.CAMERA1.subtype;
   m_houseCode = rbuf.CAMERA1.housecode;
   m_camera.set(fromProtocolState(rbuf.CAMERA1.cmnd));
   m_rssi.set(NormalizeRssiLevel(rbuf.CAMERA1.rssi));

   Init(context);
}

CCamera1::~CCamera1()
{
}

void CCamera1::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeCamera);
      details.set("subType", m_subType);
      details.set("houseCode", m_houseCode);

      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_camera);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CCamera1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.CAMERA1);

   rbuf.CAMERA1.packetlength = ENCODE_PACKET_LENGTH(CAMERA1);
   rbuf.CAMERA1.packettype = pTypeCamera;
   rbuf.CAMERA1.subtype = m_subType;
   rbuf.CAMERA1.seqnbr = seqNumberProvider->next();
   rbuf.CAMERA1.housecode = m_houseCode;
   rbuf.CAMERA1.cmnd = toProtocolState(m_camera);
   rbuf.CAMERA1.rssi = 0;
   rbuf.CAMERA1.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(CAMERA1));
}

void CCamera1::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_camera);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CCamera1::getDeviceName() const
{
   return m_deviceName;
}                     

void CCamera1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (char)m_houseCode;
   m_deviceName = ssdeviceName.str();
}

void CCamera1::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeNinja: ssModel << "X10 Ninja"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }
   
   m_deviceModel = ssModel.str();
}

unsigned char CCamera1::toProtocolState(const yApi::historization::CCameraMove& state)
{
   switch(state.get())
   {
   case yApi::historization::ECameraMoveCommand::kLeftValue : return camera_sLeft;
   case yApi::historization::ECameraMoveCommand::kRightValue : return camera_sRight;
   case yApi::historization::ECameraMoveCommand::kUpValue : return camera_sUp;
   case yApi::historization::ECameraMoveCommand::kDownValue : return camera_sDown;
   case yApi::historization::ECameraMoveCommand::kPosition1Value : return camera_sPosition1;
   case yApi::historization::ECameraMoveCommand::kProgramPosition1Value : return camera_sProgramPosition1;
   case yApi::historization::ECameraMoveCommand::kPosition2Value : return camera_sPosition2;
   case yApi::historization::ECameraMoveCommand::kProgramPosition2Value : return camera_sProgramPosition2;
   case yApi::historization::ECameraMoveCommand::kPosition3Value : return camera_sPosition3;
   case yApi::historization::ECameraMoveCommand::kProgramPosition3Value : return camera_sProgramPosition3;
   case yApi::historization::ECameraMoveCommand::kPosition4Value : return camera_sPosition4;
   case yApi::historization::ECameraMoveCommand::kProgramPosition4Value : return camera_sProgramPosition4;
   case yApi::historization::ECameraMoveCommand::kCenterPositionValue : return camera_sCenter;
   case yApi::historization::ECameraMoveCommand::kProgramCenterPositionValue : return camera_sProgramCenterPosition;
   case yApi::historization::ECameraMoveCommand::kSweepValue : return camera_sSweep;
   case yApi::historization::ECameraMoveCommand::kProgramSweepValue : return camera_sProgramSweep;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(state.formatValue());
   }
}

yApi::historization::ECameraMoveCommand CCamera1::fromProtocolState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case camera_sLeft : return yApi::historization::ECameraMoveCommand::kLeftValue;
   case camera_sRight : return yApi::historization::ECameraMoveCommand::kRightValue;
   case camera_sUp : return yApi::historization::ECameraMoveCommand::kUpValue;
   case camera_sDown : return yApi::historization::ECameraMoveCommand::kDownValue;
   case camera_sPosition1 : return yApi::historization::ECameraMoveCommand::kPosition1Value;
   case camera_sProgramPosition1 : return yApi::historization::ECameraMoveCommand::kProgramPosition1Value;
   case camera_sPosition2 : return yApi::historization::ECameraMoveCommand::kPosition2Value;
   case camera_sProgramPosition2 : return yApi::historization::ECameraMoveCommand::kProgramPosition2Value;
   case camera_sPosition3 : return yApi::historization::ECameraMoveCommand::kPosition3Value;
   case camera_sProgramPosition3 : return yApi::historization::ECameraMoveCommand::kProgramPosition3Value;
   case camera_sPosition4 : return yApi::historization::ECameraMoveCommand::kPosition4Value;
   case camera_sProgramPosition4 : return yApi::historization::ECameraMoveCommand::kProgramPosition4Value;
   case camera_sCenter : return yApi::historization::ECameraMoveCommand::kCenterPositionValue;
   case camera_sProgramCenterPosition : return yApi::historization::ECameraMoveCommand::kProgramCenterPositionValue;
   case camera_sSweep : return yApi::historization::ECameraMoveCommand::kSweepValue;
   case camera_sProgramSweep : return yApi::historization::ECameraMoveCommand::kProgramSweepValue;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
   }
}

} // namespace rfxcomMessages