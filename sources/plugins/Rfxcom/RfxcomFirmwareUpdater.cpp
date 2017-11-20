#include "stdafx.h"
#include "RfxcomFirmwareUpdater.h"
#include <shared/plugin/yPluginApi/configuration/File.h>
#include "picConfigurations/RFXtrx.h"


enum
{
   kNbRetry = 5
};

enum EKnownDeviceIds
{
   kRFXtrx = 1095,
   kRFXtrxX = 19538,
   kRFXLAN = 4121,
   kRFXsense = 16903,
   kRFXmtr = 16911,
   kRFXpan1 = 2817,
   kRFXpan2 = 2819,
   kRFXsense2 = 17673,
};


CRfxcomFirmwareUpdater::CRfxcomFirmwareUpdater(boost::shared_ptr<yApi::IYPluginApi> api,
                                               boost::shared_ptr<yApi::IExtraQuery> extraQuery,
                                               const std::string& serialPort)
   : m_api(api),
     m_extraQuery(extraQuery),
     m_serialPort(serialPort)
{
}

CRfxcomFirmwareUpdater::~CRfxcomFirmwareUpdater()
{
}

void CRfxcomFirmwareUpdater::update()
{
   YADOMS_LOG(information) << "Update RFXCom firmware...";

   // Progress is like :
   // - 0-5% : check input file
   // - 5-10% : erase RFXCom
   // - 10-90% : write RFXCom
   // - 90-100% : verify
   //TODO revérifier la progression

   const auto hexFile = m_extraQuery->getData()->data().get<yApi::configuration::CFile>("fileContent");

   // Load input file
   m_extraQuery->reportProgress(0.0f, "customLabels.firmwareUpdate.loadInputFile");//TODO traduire
   const auto hexData = loadFile(hexFile.getContent());

   boost::shared_ptr<CPicBoot> picBoot;
   try
   {
      m_extraQuery->reportProgress(5.0f, "customLabels.firmwareUpdate.flash");//TODO traduire

      picBoot = boost::make_shared<CPicBoot>(m_serialPort,
                                             boost::posix_time::seconds(1),
                                             kNbRetry);

      const auto deviceId = rfxcomSwitchToBootloaderMode(picBoot);
      checkFileCompatibility(deviceId, hexFile.getFileName());
      picBoot->setPicConfiguration(createPicConfiguration(deviceId));

      rfxcomReadBootloaderVersion(picBoot);
      rfxcomClearMemory(picBoot);
      rfxcomWritingMemory(picBoot, hexData);
      m_extraQuery->reportProgress(5.0f, "customLabels.firmwareUpdate.verify");//TODO traduire
      rfxcomVerifyMemory(picBoot);
      //TODO faut pas écrire le VerifyOK quelque part ?
      rfxcomReboot(picBoot);
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Error flashing RFXCom, " << exception.what();
      YADOMS_LOG(error) << "Will try to reboot RFXCom, but it may be broken...";
      if (picBoot)
         rfxcomReboot(picBoot);
      throw;
   }
}

boost::shared_ptr<CRfxcomFirmwareUpdater::CHexData> CRfxcomFirmwareUpdater::loadFile(const std::string& fileContent) const
{
   YADOMS_LOG(debug) << "Load input file...";

   // File is in Intel HEX 32 (I32HEX) format (see https://fr.wikipedia.org/wiki/HEX_(Intel))
   // Each line is like :
   // :BBAAAATTHHHHHH.....HHHHCC
   // With :
   // - BB is the bytes count of the line (data field only)
   // - AAAA is the absolute address of line start
   // - TT is the line type, with :
   //    - 00 : 16-bits data/address
   //    - 01 : End-Of-File
   //    - 04 : Enable 32-bits address mode
   // - HH...HHHH is the data
   // - CC is the checksum

   enum EFieldPositions
      {
         kIdxStartLineChar = 0,
         kIdxBytesCount = 1,
         kIdxAddress = 3,
         kIdxLineType = 7,
         kIdxData = 9
      };

   enum ELineType
      {
         kData = 0,
         kEOF = 1,
         kExtendedAddress = 2,
         kStartSegmentAddressRecord = 3,
         kExtendedLinearAddressRecord = 4,
         kStartLinearAddressRecord = 5
      };


   try
   {
      std::istringstream ss(fileContent);
      std::string line;
      auto lineCount = 0;
      unsigned int addressMostSignificantWordMask = 0;
      auto hexData = boost::make_shared<CHexData>();
      while (std::getline(ss, line))
      {
         // check for minimal line size (line without data)
         // ':' + BB + AAAA + TT + CC
         if (line.size() < 11)
            throw std::invalid_argument((boost::format("Line %1% : Invalid minimal line size %2% bytes, expected 11 bytes") % lineCount % line.size()).str());

         // Check line start character
         if (line[kIdxStartLineChar] != ':')
            throw std::invalid_argument((boost::format("Line %1% : Invalid first character : '%2%', expected ':'") % lineCount % line[kIdxStartLineChar]).str());

         // Check data size
         // BB is the data size. So data size is line size minus (':' + BB + AAAA + TT + CC)
         const auto readDataSize = hexStringToUInt(line.substr(kIdxBytesCount, 2));
         if (readDataSize != (line.size() - 11) / 2)
            throw std::invalid_argument((boost::format("Line %1% : Invalid data size %2% bytes, expected %3% bytes ('BB' field)") % lineCount % (line.size() - 11) % readDataSize).str());

         // Check the line checksum
         const auto readChecksum = hexStringToUInt(line.substr(kIdxData + (readDataSize * 2), 2));
         const auto computedChecksum = computeLineChecksum(line);
         if (readChecksum != computedChecksum)
            throw std::invalid_argument((boost::format("Line %1% : Invalid line checksum %2%, expected %3%") % lineCount % computedChecksum % readChecksum).str());

         // Process line content
         // Check the line type, only Extended Linear Address Record type is supported.
         // The End Of File and Extended Linear Address Record will be tested later, so ignore it here.
         const auto lineType = hexStringToUInt(line.substr(kIdxLineType, 2));
         switch (lineType)
         {
         case kExtendedLinearAddressRecord:
            {
               addressMostSignificantWordMask = hexStringToUInt(line.substr(kIdxData, 4)) << 16;
               break;
            }
         case kData:
            {
               const auto address = addressMostSignificantWordMask | hexStringToUInt(line.substr(kIdxAddress, 4));
               if (hexData->find(address) != hexData->end())
                  throw std::invalid_argument((boost::format("Line %1% : Address %2% was already defined") % lineCount % address).str());

               for (auto byteIndex = 0; byteIndex < readDataSize; ++byteIndex)
                  (*hexData)[address].push_back(static_cast<unsigned char>(hexStringToUInt(line.substr(kIdxData + 2 * byteIndex, 2))));
               //TODO finir de lire le fichier
               break;
            }
         case kEOF: // The End Of File will be tested later, so ignore it here.
            break;
         default:
            throw std::invalid_argument((boost::format("Line %1% : Unsupported line type %2%") % lineCount % lineType).str());
         }

         ++lineCount;
      }

      // Check that last line is from End Of File type
      const auto lineType = hexStringToUInt(line.substr(kIdxLineType, 2));
      if (lineType != kEOF)
         throw std::invalid_argument((boost::format("Line %1% : Invalid line type %2%, expected last line type %3%") % lineCount % lineType % kEOF).str());

      YADOMS_LOG(debug) << "Input file is loaded";
      return hexData;
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "RfxcomFirmwareUpdater, invalid input file : " << e.what();
      throw std::runtime_error("customLabels.firmwareUpdate.ErrorInvalidInputFile");
   }
}

unsigned int CRfxcomFirmwareUpdater::hexStringToUInt(const std::string& hexString)
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
   for (size_t index = 1; index < line.size() - 2 - 1; index += 2)
      sum += static_cast<unsigned char>(hexStringToUInt(line.substr(index, 2)));

   return (~sum + 1) & 0x0FF;
}

boost::shared_ptr<picConfigurations::IPicConfiguration> CRfxcomFirmwareUpdater::createPicConfiguration(const unsigned deviceId)
{
   switch (deviceId)
   {
   case kRFXtrx: return boost::make_shared<picConfigurations::CRFXtrx>();
      //TODO implémenter les autres
   default:
      throw std::runtime_error((boost::format("Unsupported device (Unknown device ID %d)") % deviceId).str());
   }
}

void CRfxcomFirmwareUpdater::checkFileCompatibility(const unsigned int deviceId,
                                                    const std::string& fileName)
{
   switch (deviceId)
   {
   case kRFXtrx:
   case kRFXtrxX:
      if (!regex_match(fileName, boost::regex("^((RFXtrx)|(RFXrec)).*\\.hex$")))
         throw std::invalid_argument((boost::format("Filename %1% can not be used to flash device ID %2%") % fileName % deviceId).str());
      break;
   case kRFXLAN:
      if (!regex_match(fileName, boost::regex("^((RFXLAN)|(RFXxPL)).*\\.hex$")))
         throw std::invalid_argument((boost::format("Filename %1% can not be used to flash device ID %2%") % fileName % deviceId).str());
      break;
   case kRFXmtr:
      if (!regex_match(fileName, boost::regex("^RFXmtr.*\\.hex$")))
         throw std::invalid_argument((boost::format("Filename %1% can not be used to flash device ID %2%") % fileName % deviceId).str());
      break;
      //TODO implémenter les autres
   default:
      throw std::runtime_error((boost::format("Unsupported device (Unknown device ID %d)") % deviceId).str());
   }
}

unsigned int CRfxcomFirmwareUpdater::rfxcomSwitchToBootloaderMode(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Switch to bootloader mode...";

   // Can look strange but confirmed by the RFXCom manufacturer :
   // the "bootload command" as referenced in "kRFXtrx SDK.pdf" used by
   // the RFXCom firmaware to switch on bootloader mode, looks like a ReadDeviceID bootloader frame :
   //
   // So we can use the dedicate function of picBoot
   const auto deviceId = picBoot->readPicDeviceId();

   YADOMS_LOG(debug) << "RFXCom is in bootloader mode, detected device ID is " << deviceId;

   return deviceId;
}

void CRfxcomFirmwareUpdater::rfxcomReadBootloaderVersion(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Read RFXCom bootloader version...";
   const auto picVersion = picBoot->readPicVersion();
   YADOMS_LOG(debug) << "RFXCom bootloader version is " << picVersion;
}

void CRfxcomFirmwareUpdater::rfxcomClearMemory(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Clear RFXCom memory...";
   picBoot->erasePicProgramMemory();
   YADOMS_LOG(debug) << "RFXCom memory cleared";
}

void CRfxcomFirmwareUpdater::rfxcomWritingMemory(boost::shared_ptr<CPicBoot> picBoot,
                                                 boost::shared_ptr<CHexData> hexData)
{
   YADOMS_LOG(debug) << "Write RFXCom memory...";
   //TODO
   YADOMS_LOG(debug) << "RFXCom memory written";
}

void CRfxcomFirmwareUpdater::rfxcomVerifyMemory(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Verify RFXCom memory...";
   //TODO
   YADOMS_LOG(debug) << "Verification is OK";
}

void CRfxcomFirmwareUpdater::rfxcomReboot(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Reset RFXCom...";
   picBoot->reBootPic();
}

