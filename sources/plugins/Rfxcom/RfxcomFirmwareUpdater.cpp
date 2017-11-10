#include "stdafx.h"
#include "RfxcomFirmwareUpdater.h"
#include <shared/encryption/Base64.h>

//TODO doit-on appliquer ces décalages (code en provenance de domoticz)
//int AddressLow = PKT_pmrangelow;
//int AddressHigh = PKT_pmrangehigh;
//if (HwdType == HTYPE_RFXtrx868)
//{
//	AddressLow = PKT_pmrangelow868;
//	AddressHigh = PKT_pmrangehigh868;
//}

class CPicBoot;

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

   const auto base64firmware = m_extraQuery->getData()->data().get<std::string>("fileContent");
   const auto firmwareContent = shared::encryption::CBase64::decode(base64firmware);

   // Load input file
   m_extraQuery->reportProgress(0.0f, "customLabels.firmwareUpdate.loadInputFile");//TODO traduire
   loadFile(firmwareContent);

   boost::shared_ptr<CPicBoot> picBoot;
   try
   {
      m_extraQuery->reportProgress(5.0f, "customLabels.firmwareUpdate.flash");//TODO traduire

      picBoot = boost::make_shared<CPicBoot>(m_serialPort,
                                             boost::posix_time::seconds(1));

      rfxcomSwitchToBootloaderMode(picBoot);

      rfxcomReadVersion(picBoot);
      rfxcomClearMemory(picBoot);
      rfxcomWritingMemory(picBoot);
      m_extraQuery->reportProgress(5.0f, "customLabels.firmwareUpdate.verify");//TODO traduire
      rfxcomVerifyMemory(picBoot);
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

void CRfxcomFirmwareUpdater::loadFile(const std::string& fileContent) const
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

         // Check the line type, only Extended Linear Address Record type is supported.
         // The End Of File and Extended Linear Address Record will be tested later, so ignore it here.
         const auto lineType = hexStringToUInt(line.substr(kIdxLineType, 2));
         if (lineType != kEOF && lineType != kExtendedLinearAddressRecord && lineType != kData)
            throw std::invalid_argument((boost::format("Line %1% : Invalid line type %2%, expected %3%") % lineCount % lineType % kData).str());

         // Check the line checksum
         const auto readChecksum = hexStringToUInt(line.substr(kIdxData + (readDataSize * 2), 2));
         const auto computedChecksum = computeLineChecksum(line);
         if (readChecksum != computedChecksum)
            throw std::invalid_argument((boost::format("Line %1% : Invalid line checksum %2%, expected %3%") % lineCount % computedChecksum % readChecksum).str());

         if (lineCount == 0)
         {
            // Check the first line
            if (lineType != kExtendedLinearAddressRecord)
               throw std::invalid_argument((boost::format("Line %1% : Invalid first line type %2%, expected %3%") % lineCount % lineType % kExtendedLinearAddressRecord).str());
            addressMostSignificantWordMask = hexStringToUInt(line.substr(kIdxData, 4)) << 16;
         }
         else
         {
            // Process other lines
            if (lineType == kData)
            {
               const auto address = addressMostSignificantWordMask | hexStringToUInt(line.substr(kIdxAddress, 4));
               //TODO finir de lire le fichier
            }
         }

         ++lineCount;
      }

      // Check that last line is from End Of File type
      const auto lineType = hexStringToUInt(line.substr(kIdxLineType, 2));
      if (lineType != kEOF)
         throw std::invalid_argument((boost::format("Line %1% : Invalid line type %2%, expected last line type %3%") % lineCount % lineType % kEOF).str());
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "RfxcomFirmwareUpdater, invalid input file : " << e.what();
      throw std::runtime_error("customLabels.firmwareUpdate.ErrorInvalidInputFile");
   }

   YADOMS_LOG(debug) << "Input file is loaded";
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

void CRfxcomFirmwareUpdater::rfxcomSwitchToBootloaderMode(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Switch to bootloader mode...";

   //TODO basculer en bootloader
   // TODO tracer version du bootloader

   YADOMS_LOG(debug) << "RFXCom is in bootloader mode";
}

void CRfxcomFirmwareUpdater::rfxcomReadVersion(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Read RFXCom current version...";
   //TODO
   YADOMS_LOG(debug) << "RFXCom current version is " << "TODO";
}

void CRfxcomFirmwareUpdater::rfxcomClearMemory(boost::shared_ptr<CPicBoot> picBoot)
{
   YADOMS_LOG(debug) << "Clear RFXCom memory...";
   //TODO
   YADOMS_LOG(debug) << "RFXCom memory cleared";
}

void CRfxcomFirmwareUpdater::rfxcomWritingMemory(boost::shared_ptr<CPicBoot> picBoot)
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
   //TODO
}

