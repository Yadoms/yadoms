#include "stdafx.h"
#include "Camera1.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCamera1::CCamera1(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& command,
                      const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_deviceDetails(shared::CDataContainer::make()),
        m_camera(boost::make_shared<yApi::historization::CCameraMove>("camera")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_camera , m_signalPower})        
   {
      m_camera->setCommand(command);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType"));
      m_houseCode = static_cast<unsigned char>(deviceDetails->get<unsigned int>("houseCode"));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CCamera1::CCamera1(boost::shared_ptr<yApi::IYPluginApi> api,
                      unsigned int subType,
                      const std::string& name,
                      const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_deviceDetails(shared::CDataContainer::make()),
        m_camera(boost::make_shared<yApi::historization::CCameraMove>("camera")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_camera , m_signalPower})

   {
      m_camera->set(yApi::historization::ECameraMoveCommand::kCenterPosition);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      if (m_subType != sTypeNinja)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_houseCode = static_cast<unsigned char>(manuallyDeviceCreationConfiguration->get<char>("houseCode"));

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CCamera1::CCamera1(boost::shared_ptr<yApi::IYPluginApi> api,
                      const RBUF& rbuf,
                      size_t rbufSize)
      : m_deviceDetails(shared::CDataContainer::make()),
        m_camera(boost::make_shared<yApi::historization::CCameraMove>("camera")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_camera , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeCamera,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(CAMERA1),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.CAMERA1.subtype;
      m_houseCode = rbuf.CAMERA1.housecode;
      m_camera->set(fromProtocolState(rbuf.CAMERA1.cmnd));
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.CAMERA1.rssi));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();
   }

   CCamera1::~CCamera1()
   {
   }

   void CCamera1::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeCamera);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("houseCode", m_houseCode);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CCamera1::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.CAMERA1);

      rbuf.CAMERA1.packetlength = ENCODE_PACKET_LENGTH(CAMERA1);
      rbuf.CAMERA1.packettype = pTypeCamera;
      rbuf.CAMERA1.subtype = m_subType;
      rbuf.CAMERA1.seqnbr = seqNumberProvider->next();
      rbuf.CAMERA1.housecode = m_houseCode;
      rbuf.CAMERA1.cmnd = toProtocolState(*m_camera);
      rbuf.CAMERA1.rssi = 0;
      rbuf.CAMERA1.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(CAMERA1));
   }

   void CCamera1::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CCamera1::filter() const
   {
   }

   void CCamera1::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName,
                         m_deviceModel,
                         m_deviceModel,
                         m_keywords,
                         m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CCamera1::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CCamera1::keywords()
   {
      return m_keywords;
   }

   void CCamera1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<char>(m_houseCode);
      m_deviceName = ssdeviceName.str();
   }

   void CCamera1::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeNinja: ssModel << "X10 Ninja";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CCamera1::toProtocolState(const yApi::historization::CCameraMove& state)
   {
      switch (state.get())
      {
      case yApi::historization::ECameraMoveCommand::kLeftValue: return camera_sLeft;
      case yApi::historization::ECameraMoveCommand::kRightValue: return camera_sRight;
      case yApi::historization::ECameraMoveCommand::kUpValue: return camera_sUp;
      case yApi::historization::ECameraMoveCommand::kDownValue: return camera_sDown;
      case yApi::historization::ECameraMoveCommand::kPosition1Value: return camera_sPosition1;
      case yApi::historization::ECameraMoveCommand::kProgramPosition1Value: return camera_sProgramPosition1;
      case yApi::historization::ECameraMoveCommand::kPosition2Value: return camera_sPosition2;
      case yApi::historization::ECameraMoveCommand::kProgramPosition2Value: return camera_sProgramPosition2;
      case yApi::historization::ECameraMoveCommand::kPosition3Value: return camera_sPosition3;
      case yApi::historization::ECameraMoveCommand::kProgramPosition3Value: return camera_sProgramPosition3;
      case yApi::historization::ECameraMoveCommand::kPosition4Value: return camera_sPosition4;
      case yApi::historization::ECameraMoveCommand::kProgramPosition4Value: return camera_sProgramPosition4;
      case yApi::historization::ECameraMoveCommand::kCenterPositionValue: return camera_sCenter;
      case yApi::historization::ECameraMoveCommand::kProgramCenterPositionValue: return camera_sProgramCenterPosition;
      case yApi::historization::ECameraMoveCommand::kSweepValue: return camera_sSweep;
      case yApi::historization::ECameraMoveCommand::kProgramSweepValue: return camera_sProgramSweep;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(state.get()));
      }
   }

   yApi::historization::ECameraMoveCommand CCamera1::fromProtocolState(unsigned char protocolState)
   {
      switch (protocolState)
      {
      case camera_sLeft: return yApi::historization::ECameraMoveCommand::kLeft;
      case camera_sRight: return yApi::historization::ECameraMoveCommand::kRight;
      case camera_sUp: return yApi::historization::ECameraMoveCommand::kUp;
      case camera_sDown: return yApi::historization::ECameraMoveCommand::kDown;
      case camera_sPosition1: return yApi::historization::ECameraMoveCommand::kPosition1;
      case camera_sProgramPosition1: return yApi::historization::ECameraMoveCommand::kProgramPosition1;
      case camera_sPosition2: return yApi::historization::ECameraMoveCommand::kPosition2;
      case camera_sProgramPosition2: return yApi::historization::ECameraMoveCommand::kProgramPosition2;
      case camera_sPosition3: return yApi::historization::ECameraMoveCommand::kPosition3;
      case camera_sProgramPosition3: return yApi::historization::ECameraMoveCommand::kProgramPosition3;
      case camera_sPosition4: return yApi::historization::ECameraMoveCommand::kPosition4;
      case camera_sProgramPosition4: return yApi::historization::ECameraMoveCommand::kProgramPosition4;
      case camera_sCenter: return yApi::historization::ECameraMoveCommand::kCenterPosition;
      case camera_sProgramCenterPosition: return yApi::historization::ECameraMoveCommand::kProgramCenterPosition;
      case camera_sSweep: return yApi::historization::ECameraMoveCommand::kSweep;
      case camera_sProgramSweep: return yApi::historization::ECameraMoveCommand::kProgramSweep;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
      }
   }
} // namespace rfxcomMessages
