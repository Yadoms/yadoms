#include "stdafx.h"
#include "RfxcomFirmwareUpdater.h"
#include <shared/encryption/Base64.h>

CRfxcomFirmwareUpdater::CRfxcomFirmwareUpdater(boost::shared_ptr<yApi::IYPluginApi> api,
                                               boost::shared_ptr<yApi::IExtraQuery> extraQuery,
                                               boost::shared_ptr<shared::communication::IAsyncPort> port)
   : m_api(api),
     m_extraQuery(extraQuery),
     m_port(port)
{
   if (!m_port)
   {
      YADOMS_LOG(error) << "CRfxcomFirmwareUpdater : m_port must exist";
      throw std::exception("customLabels.firmwareUpdate.ErrorInternal");
   }
}

CRfxcomFirmwareUpdater::~CRfxcomFirmwareUpdater()
{
}

void CRfxcomFirmwareUpdater::update()
{
   YADOMS_LOG(information) << "Update RFXCom firmware...";

   const auto base64firmware = m_extraQuery->getData()->data().get<std::string>("fileContent");
   const auto firmwareContent = shared::encryption::CBase64::decode(base64firmware);

   m_extraQuery->reportProgress(0.0f, "customLabels.firmwareUpdate.checkInputFile");//TODO traduire
   checkFile(firmwareContent);

   //TODO
   //const std::string stepi18nSendingFile = "customLabels.firmwareUpdate.writeFile"; //TODO utile ?
   //m_messageHandler->sendFile(firmwareContent, [&](float progress)
   //                           {
   //                              //report progression
   //                              m_extraQuery->reportProgress(progress * 75.0f / 100.0f, stepi18nSendingFile);
   //                           });

   ////send "HELLO" command until dongle answers
   ////up to 2minutes timeout
   //const std::string stepi18nWriting = "customLabels.firmwareUpdate.writeFile";
   //m_extraQuery->reportProgress(75.0f, stepi18nWriting);


   //boost::posix_time::ptime timeout = shared::currentTime::Provider().now() + boost::posix_time::minutes(2);

   //shared::event::CEventHandler evtHandler;
   //enum
   //   {
   //      kSendFinished = shared::event::kUserFirstId
   //   };

   //bool isReady = false;
   //while (!isReady && shared::currentTime::Provider().now() < timeout)
   //{
   //   if (m_messageHandler->send(m_transceiver->buildHelloCmd(),
   //                              [&](boost::shared_ptr<const frames::incoming::CFrame> frame)
   //                              {
   //                                 YADOMS_LOG(information) << "Something received";
   //                                 YADOMS_LOG(information) << frame->getAscii()->getContent();

   //                                 //expect restart of dongle
   //                                 bool isRestartFrame = frame->isAscii() && boost::icontains(frame->getAscii()->getContent(), "Ziblue Dongle");

   //                                 if (!isRestartFrame)
   //                                    m_extraQuery->reportProgress(75.0f, frame->getAscii()->getContent());
   //                                 return isRestartFrame;
   //                              },
   //                              [&](boost::shared_ptr<const frames::incoming::CFrame> frame)
   //                              {
   //                                 YADOMS_LOG(information) << "New dongle";

   //                                 //apply the dongle configuration
   //                                 m_dongle = CDongle::create(frame->getAscii()->getContent());

   //                                 if (m_dongle)
   //                                 {
   //                                    YADOMS_LOG(information) << "Dongle :" << m_dongle->getType() << " " << m_dongle->getModel() << " v" << m_dongle->getFirmwareVersion();
   //                                 }
   //                                 else
   //                                 {
   //                                    YADOMS_LOG(information) << "Unknown dongle, or not fully supported firmware";
   //                                 }

   //                                 evtHandler.postEvent(kSendFinished);
   //                              }))
   //   {
   //      switch (evtHandler.waitForEvents())
   //      {
   //      case kSendFinished:
   //         api->setPluginState(yApi::historization::EPluginState::kRunning);
   //         updateDongleConfiguration(api);
   //         m_messageHandler->send(m_transceiver->buildStartListeningData());
   //         isReady = true;
   //         break;
   //      default:
   //         throw std::runtime_error("Unexpected event " + std::to_string(evtHandler.getEventId()));
   //      }
   //   }
   //}


   //const std::string stepi18nUpgradedFail = "customLabels.firmwareUpdate.fail";
   //if (isReady)
   //{
   //   YADOMS_LOG(information) << "RFXCom firmware successufuly updated";
   //   m_extraQuery->sendSuccess(shared::CDataContainer::EmptyContainer);
   //}
   //else
   //{
   //   YADOMS_LOG(error) << "RFXCom firmware update failed";
   //   m_extraQuery->sendError(stepi18nUpgradedFail);
   //}
}

void CRfxcomFirmwareUpdater::checkFile(const std::string& fileContent) const
{
   // File is in Intel HEX 32 (I32HEX) format (see https://fr.wikipedia.org/wiki/HEX_(Intel))
   // Each line is like :
   // :BBAAAATTHHHHHH.....HHHHCC
   // With :
   // - BB is the bytes count of the line
   // - AAAA is the absolute address of line start
   // - TT is the line type, with :
   //    - 00 : 16-bits data/address
   //    - 01 : End-Of-File
   //    - 04 : Enable 32-bits address mode
   // - HH...HHHH is the data
   // - CC is the checksum

   try
   {
      std::istringstream ss(fileContent);
      std::string line;
      auto lineCount = 0;
      while (std::getline(ss, line))
      {
         // check for minimal line size (line without data)
         // ':' + BB + AAAA + TT + CC
         if (line.size() < 11)
            throw std::invalid_argument((boost::format("Line %1% : Invalid minimal line size %2% bytes, expected 11 bytes") % lineCount % line.size()).str());

         // Check line start character
         if (line[0] != ':')
            throw std::invalid_argument((boost::format("Line %1% : Invalid first character : '%2%', expected ':'") % lineCount % line[0]).str());

         // Check data size
         // BB is the data size. So data size is line size minus (':' + BB + AAAA + TT + CC)
         auto readDataSize = hexStringToInt(line.substr(1, 2));
         if (readDataSize != (line.size() - 11) / 2)
            throw std::invalid_argument((boost::format("Line %1% : Invalid data size %2% bytes, expected %3% bytes ('BB' field)") % lineCount % (line.size() - 11) % readDataSize).str());

         // Check the line checksum
         const auto readChecksum = hexStringToInt(line.substr(9 + (readDataSize * 2), 2));
         const auto computedChecksum = computeLineChecksum(line);
         if (readChecksum != computedChecksum)
            throw std::invalid_argument((boost::format("Line %1% : Invalid line checksum %2%, expected %3%") % lineCount % computedChecksum % readChecksum).str());

         ++lineCount;
      }

      // Check that last line is from End Of File type
      const auto lineType = hexStringToInt(line.substr(7, 2));
      if (lineType != 0x01)
         throw std::invalid_argument((boost::format("Line %1% : Invalid line type 0x%2%, expected last line type 0x01") % lineCount % lineType).str());
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "RfxcomFirmwareUpdater, invalid input file : " << e.what();
      throw std::exception("customLabels.firmwareUpdate.ErrorInvalidInputFile");
   }
}

unsigned int CRfxcomFirmwareUpdater::hexStringToInt(const std::string& hexString)
{
   unsigned int x;
   std::stringstream ss;
   ss << std::hex << hexString;
   ss >> x;
   return x;
}

unsigned int CRfxcomFirmwareUpdater::computeLineChecksum(const std::string& line)
{
   unsigned char sum = 0;
   // Ignore ':' first char and checksum byte itself
   for (auto index = 1; index < line.size() - 2 - 1; index += 2)
      sum += static_cast<unsigned char>(hexStringToInt(line.substr(index, 2)));

   return ~sum + 1 & 0x0FF;
}

