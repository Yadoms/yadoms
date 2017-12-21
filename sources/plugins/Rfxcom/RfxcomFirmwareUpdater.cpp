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
   kRfXTrx = 1095,
   kRfXTrxX = 19538,
   kRfxLan = 4121,
   kRfxSense = 16903,
   kRfxMtr = 16911,
   kRfxPan1 = 2817,
   kRfxPan2 = 2819,
   kRfxSense2 = 17673,
};


CRfxcomFirmwareUpdater::CRfxcomFirmwareUpdater(const boost::shared_ptr<yApi::IYPluginApi> api,
                                               const boost::shared_ptr<yApi::IExtraQuery> extraQuery,
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
   // - 0-4% : load input file
   // - 5% : connect to RFXCom
   // - 6-9% : erase RFXCom
   // - 10-94% : write RFXCom
   // - 95-100% : reboot RFXCom
   //TODO revérifier la progression

   const auto hexFile = m_extraQuery->getData()->data().get<yApi::configuration::CFile>("fileContent");

   // Load input file
   m_extraQuery->reportProgress(0.0f, "customLabels.firmwareUpdate.loadInputFile");//TODO traduire
   CHexData programMemory, eepromMemory, configurationMemory;
   prepareDataForWriteIntoPic(loadFile(hexFile.getContent()),
                              programMemory,
                              eepromMemory,
                              configurationMemory);

   boost::shared_ptr<CPicBoot> picBoot;
   try
   {
      m_extraQuery->reportProgress(5.0f, "customLabels.firmwareUpdate.connect");//TODO traduire

      picBoot = boost::make_shared<CPicBoot>(m_serialPort,
                                             boost::posix_time::seconds(1),
                                             kNbRetry);

      const auto deviceId = rfxcomSwitchToBootloaderMode(picBoot);
      checkFileCompatibility(deviceId, hexFile.getFileName());
      const auto picConfiguration(createPicConfiguration(deviceId));
      picBoot->setPicConfiguration(picConfiguration);

      rfxcomReadBootloaderVersion(picBoot);

      m_extraQuery->reportProgress(6.0f, "customLabels.firmwareUpdate.erase");//TODO traduire
      rfxcomClearMemory(picBoot);

      m_extraQuery->reportProgress(10.0f, "customLabels.firmwareUpdate.write");//TODO traduire
      rfxcomWriteMemory(picBoot,
                        picConfiguration,
                        programMemory,
                        eepromMemory,
                        [&](const float writeProgress) -> void
                        {
                           m_extraQuery->reportProgress(10.0f + ((95.0f - 10.0f) * 100.0f / writeProgress),
                                                        "customLabels.firmwareUpdate.write");//TODO traduire
                        });

      m_extraQuery->reportProgress(95.0f, "customLabels.firmwareUpdate.reboot");//TODO traduire
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

boost::shared_ptr<CRfxcomFirmwareUpdater::CHexData> CRfxcomFirmwareUpdater::loadFile(
   const std::string& fileContent) const
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
      kEof = 1,
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
      while (getline(ss, line))
      {
         // check for minimal line size (line without data)
         // ':' + BB + AAAA + TT + CC
         if (line.size() < 11)
            throw std::invalid_argument(
               (boost::format("Line %1% : Invalid minimal line size %2% bytes, expected 11 bytes") % lineCount % line.
                  size()).str());

         // Check line start character
         if (line[kIdxStartLineChar] != ':')
            throw std::invalid_argument(
               (boost::format("Line %1% : Invalid first character : '%2%', expected ':'") % lineCount % line[
                  kIdxStartLineChar]).str());

         // Check data size
         // BB is the data size. So data size is line size minus (':' + BB + AAAA + TT + CC)
         const auto readDataSize = hexStringToUInt(line.substr(kIdxBytesCount, 2));
         if (readDataSize != (line.size() - 11) / 2)
            throw std::invalid_argument(
               (boost::format("Line %1% : Invalid data size %2% bytes, expected %3% bytes ('BB' field)") % lineCount % (
                  line.size() - 11) % readDataSize).str());

         // Check the line checksum
         const auto readChecksum = hexStringToUInt(line.substr(kIdxData + (readDataSize * 2), 2));
         const auto computedChecksum = computeLineChecksum(line);
         if (readChecksum != computedChecksum)
            throw std::invalid_argument(
               (boost::format("Line %1% : Invalid line checksum %2%, expected %3%") % lineCount % computedChecksum %
                  readChecksum).str());

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
                  throw std::invalid_argument(
                     (boost::format("Line %1% : Address %2% was already defined") % lineCount % address).str());

               for (auto byteIndex = 0; byteIndex < readDataSize; ++byteIndex)
                  (*hexData)[address].push_back(
                     static_cast<unsigned char>(hexStringToUInt(line.substr(kIdxData + 2 * byteIndex, 2))));

               break;
            }
         case kEof: // The End Of File will be tested later, so ignore it here.
            break;
         default:
            throw std::invalid_argument(
               (boost::format("Line %1% : Unsupported line type %2%") % lineCount % lineType).str());
         }

         ++lineCount;
      }

      // Check that last line is from End Of File type
      const auto lineType = hexStringToUInt(line.substr(kIdxLineType, 2));
      if (lineType != kEof)
         throw std::invalid_argument(
            (boost::format("Line %1% : Invalid line type %2%, expected last line type %3%") % lineCount % lineType %
               kEof).str());

      YADOMS_LOG(debug) << "Input file is loaded";
      return hexData;
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "RfxcomFirmwareUpdater, invalid input file : " << e.what();
      throw std::runtime_error("customLabels.firmwareUpdate.ErrorInvalidInputFile");
   }
}

void CRfxcomFirmwareUpdater::razRawDataFromTemplate(unsigned char (& rowData)[256])
{
   for (size_t i = 0; i < sizeof rowData; ++i)
      rowData[i] = (i + 1) % 4 ? 255 : 0;
}

void CRfxcomFirmwareUpdater::prepareDataForWriteIntoPic(const boost::shared_ptr<CHexData> fileData,
                                                        CHexData& programMemory,
                                                        CHexData& eepromMemory,
                                                        CHexData& configurationMemory) const
{
   // Microchip AN1157 extract :
   // Each instruction has three bytes of program data and one byte of zeroes, called the “phantom byte”, for a total of four bytes
   // Write operations are performed in 64 instruction blocks called rows (256 bytes)
   // Erase instructions are performed in blocks of 8 rows called pages (2048 bytes)

   // Init data array with 0xFF (init phantom byte to 0)
   unsigned char rowData[256];
   razRawDataFromTemplate(rowData);

   std::vector<unsigned char> overflowArray;

   programMemory.clear();
   eepromMemory.clear();
   configurationMemory.clear();

   static const unsigned int LowerByteMask = 0xFF;

   static const unsigned int RowStartMask = 0xFFFF00;
   const auto& firstLineAddress = fileData->begin()->first;
   auto rowStartAddress = firstLineAddress & RowStartMask;

   static const unsigned int UpperByteMask2 = 0x1FF0000;

   auto emptyRow = true;
   for (auto& line : *fileData)
   {
      const auto& lineAddress = line.first;
      auto& lineData = line.second;
      const auto lineAddrHigh = (lineAddress & UpperByteMask2) >> 16;
      if (lineAddrHigh >= 0 && lineAddrHigh <= 253)
      {
         // Program memory

         // Is all of current data in new row?
         if (lineAddress - rowStartAddress > 255)
         {
            // Next data is in new Row, so write old row to memory if not empty
            if (!emptyRow)
            {
               std::vector<unsigned char> outLine;
               auto outAddr = rowStartAddress;
               auto rowAddress = outAddr;
               auto inAddr = 0;
               while (inAddr < 256)
               {
                  // Built the data line
                  if (inAddr % 16 == 0)
                  {
                     outLine.clear();
                     rowAddress = outAddr;
                  }

                  outLine.push_back(rowData[inAddr]);
                  ++outAddr;
                  ++inAddr;

                  if (inAddr % 16 == 0 && inAddr != 0)
                     programMemory[rowAddress] = outLine;
               }

               emptyRow = true;
            }

            // Start new row
            rowStartAddress = lineAddress & RowStartMask;
            razRawDataFromTemplate(rowData);

            // Initialize row with overflow of last line, if any
            for (std::vector<unsigned char>::const_iterator overflowIterator = overflowArray.begin();
                 overflowIterator < overflowArray.end();
                 ++ overflowIterator)
            {
               const auto overflowIndex = overflowIterator - overflowArray.begin();
               rowData[overflowIndex] = *overflowIterator;

               // If non-erased data is present, row is not empty
               if ((overflowIndex + 1) % 4 != 0)
                  if (*overflowIterator != 0xFF)
                     emptyRow = false;
            }
            overflowArray.clear();
         }

         // Is a portion of current data in new row ?
         auto rowIndex = lineAddress & LowerByteMask;
         if (lineAddress + lineData.size() - rowStartAddress > 255)
         {
            // Store data in temporary array to use in next row
            const auto overflowLen = rowIndex + lineData.size() - 256;
            copy(lineData.begin(), lineData.begin() + overflowLen, overflowArray.begin());
            lineData.resize(lineData.size() - overflowLen);
         }

         // Get current data
         for (std::vector<unsigned char>::const_iterator lineDataIterator = lineData.begin();
              lineDataIterator < lineData.end();
              ++ lineDataIterator)
         {
            const auto lineDataIndex = lineDataIterator - lineData.begin();

            rowData[rowIndex] = *lineDataIterator;
            ++rowIndex;

            // If non-erased data is present, row is not empty
            if ((lineDataIndex + 1) % 4 != 0)
               if (*lineDataIterator != 0xFF)
                  emptyRow = false;
         }
      }

      else if (lineAddrHigh >= 254 && lineAddrHigh <= 255)
      {
         // EEProm memory
         std::vector<unsigned char> outLine;
         copy(lineData.begin(), lineData.end(), outLine.begin());
         eepromMemory[lineAddress] = lineData;
      }

      else if (lineAddrHigh == 496)
      {
         // Configuration memory
         std::vector<unsigned char> outLine;
         copy(lineData.begin(), lineData.end(), outLine.begin());
         configurationMemory[lineAddress] = lineData;
      }

      else
      {
         YADOMS_LOG(warning) << "Unsupported address (lineAddrHigh = " << lineAddrHigh << ", lineAddress = " <<
            lineAddress << ", ignored";
      }
   }

   // Write last row of PM if it isn't empty
   if (!emptyRow)
   {
      std::vector<unsigned char> outLine;
      auto outAddr = rowStartAddress;
      auto rowAddress = outAddr;
      auto inAddr = 0;
      while (inAddr < 256)
      {
         // Built the data line
         if (inAddr % 16 == 0)
         {
            outLine.clear();
            rowAddress = outAddr;
         }

         outLine.push_back(rowData[inAddr]);
         ++outAddr;
         ++inAddr;

         if (inAddr % 16 == 0 && inAddr != 0)
            programMemory[rowAddress] = outLine;
      }
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

boost::shared_ptr<picConfigurations::IPicConfiguration> CRfxcomFirmwareUpdater::createPicConfiguration(
   const unsigned deviceId)
{
   switch (deviceId)
   {
   case kRfXTrx: return boost::make_shared<picConfigurations::CRfXtrx>();
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
   case kRfXTrx:
   case kRfXTrxX:
      if (!regex_match(fileName, boost::regex("^((RFXtrx)|(RFXrec)).*\\.hex$")))
         throw std::invalid_argument(
            (boost::format("Filename %1% can not be used to flash device ID %2%") % fileName % deviceId).str());
      break;
   case kRfxLan:
      if (!regex_match(fileName, boost::regex("^((RFXLAN)|(RFXxPL)).*\\.hex$")))
         throw std::invalid_argument(
            (boost::format("Filename %1% can not be used to flash device ID %2%") % fileName % deviceId).str());
      break;
   case kRfxMtr:
      if (!regex_match(fileName, boost::regex("^RFXmtr.*\\.hex$")))
         throw std::invalid_argument(
            (boost::format("Filename %1% can not be used to flash device ID %2%") % fileName % deviceId).str());
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

void CRfxcomFirmwareUpdater::rfxcomWriteMemory(boost::shared_ptr<CPicBoot> picBoot,
                                               const boost::shared_ptr<picConfigurations::IPicConfiguration>
                                               picConfiguration,
                                               const CHexData& programMemory,
                                               const CHexData& eepromMemory,
                                               const boost::function1<void, const float> progressFunction) const
{
   YADOMS_LOG(debug) << "Write RFXCom memory...";

   // Write program memory
   rfxcomWriteMemory(picBoot,
                     CPicBoot::kProgramMemory,
                     picConfiguration,
                     programMemory,
                     progressFunction);

   // Write Device EEPROM if concerned
   if (picConfiguration->deviceType() == picConfigurations::IPicConfiguration::kPic24F)
      rfxcomWriteMemory(picBoot,
                        CPicBoot::kEepromMemory,
                        picConfiguration,
                        eepromMemory,
                        [](const float writeProgress) -> void
                        {
                        });

   // Send VerifyOk Command to indicate bootloading finised successfully
   picBoot->writePicVerifyOk();

   YADOMS_LOG(debug) << "RFXCom memory written";
}

void CRfxcomFirmwareUpdater::rfxcomWriteMemory(boost::shared_ptr<CPicBoot> picBoot,
                                               const CPicBoot::EMemoryKind memory,
                                               const boost::shared_ptr<picConfigurations::IPicConfiguration>
                                               picConfiguration,
                                               const CHexData& data,
                                               const boost::function1<void, const float> progressFunction)
{
   const auto lineSize = data.begin()->second.size();

   auto dataBlockIterator = data.begin();
   while (dataBlockIterator != data.end())
   {
      const auto writeBlockAddress = dataBlockIterator->first / picConfiguration->bytesPerAddr();
      std::vector<unsigned char> writeBlockData;
      for (auto idxBlock = 0; idxBlock < picConfiguration->writeBlockSize() / lineSize; ++idxBlock)
      {
         if (dataBlockIterator != data.end())
         {
            writeBlockData.insert(writeBlockData.end(),
                                  dataBlockIterator->second.begin(),
                                  dataBlockIterator->second.end());
            ++dataBlockIterator;
         }
         else
         {
            // No more data available, complete block with 0xFF data
            writeBlockData.insert(writeBlockData.end(),
                                  lineSize,
                                  0xFF);
         }
      }

      // We have now a full block, write it
      picBoot->writePic(memory,
                        writeBlockAddress,
                        writeBlockData);

      // Verify block
      if (!picBoot->verifyPic(memory,
                              writeBlockAddress,
                              writeBlockData))
         throw std::runtime_error(
            (boost::format("Error verifying written data at %1%") % dataBlockIterator->first).str());

      progressFunction(45.0f);//TODO
   }
}

void CRfxcomFirmwareUpdater::rfxcomReboot(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Reset RFXCom...";
   picBoot->reBootPic();
}
