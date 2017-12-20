/******************************************************************************\
* These sources (PICBOOT.h and PICBOOT.cpp) are very inspired from Microchip PIC24F Programmer
* PICBOOT.dll official sources.
* They have just been transformed to C++, adapted to be cross-platform
* and compatible with our RFXCom plugin.
\******************************************************************************/

/******************************************************************************\
*	PIC24F Serial Bootloader Communications Handler
*	original code by Ross M. Fosler
*   updated for PIC24F by Brant Ivey
*	Microchip Technology Incorporated
*
* Software License Agreement:
*
* The software supplied herewith by Microchip Technology Incorporated
* (the “Company”) for its PICmicro® Microcontroller is intended and
* supplied to you, the Company’s customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FrOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
\******************************************************************************/

#include "stdafx.h"
#include "PicBoot.h"
#include "PicBootReceiveBufferHandler.h"
#include "PicBootPacketControlCharacters.h"
#include <shared/Log.h>


enum
{
   kEvtPicBootPortConnection = shared::event::kUserFirstId,
   kEvtPicBootPortDataReceived
};


CPicBoot::CPicBoot(const std::string& comPort,
                   boost::posix_time::time_duration readTimeOut,
                   const unsigned int maxRetrys)
   : m_logger("debug"),
     m_maxRetrys(maxRetrys)
{
   m_port = boost::make_shared<shared::communication::CAsyncSerialPort>(comPort,
                                                                        boost::asio::serial_port_base::baud_rate(
                                                                           38400));
   m_port->subscribeForConnectionEvents(m_eventHandler, kEvtPicBootPortConnection);

   auto receiveBufferHandler = boost::make_shared<CPicBootReceiveBufferHandler>(m_eventHandler,
                                                                                kEvtPicBootPortDataReceived,
                                                                                readTimeOut,
                                                                                m_logger);
   m_port->setReceiveBufferHandler(receiveBufferHandler);

   m_port->start();

   if (! (m_eventHandler.waitForEvents(boost::posix_time::seconds(10)) == kEvtPicBootPortConnection
      && m_eventHandler.getEventData<boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>>()->
                        isConnected()))
      throw std::runtime_error("Unable to open serial port");

   receiveBufferHandler->flush();
}

CPicBoot::~CPicBoot()
{
}

void CPicBoot::setPicConfiguration(const boost::shared_ptr<picConfigurations::IPicConfiguration> picConfiguration)
{
   m_picConfiguration = picConfiguration;

   //Limit to 1 to 3 bytes per addr
   if (!m_picConfiguration->bytesPerAddr())
      throw std::invalid_argument(
         (boost::format("CPicBoot::setPicConfiguration : bytes per address value too small %1%") % m_picConfiguration->
            bytesPerAddr()).str());
   if (m_picConfiguration->bytesPerAddr() > 3)
      throw std::invalid_argument(
         (boost::format("CPicBoot::setPicConfiguration : bytes per address value too big %1%") % m_picConfiguration->
            bytesPerAddr()).str());
}

boost::shared_ptr<std::vector<unsigned char>> CPicBoot::readPic(const EMemoryKind memory,
                                                                const unsigned long blockAddress,
                                                                const unsigned int nbBytesToRead)
{
   EBootLoaderCommand command;

   switch (memory)
   {
   case kProgramMemory:
      command = kCommandReadPm;
      break;
   case kEepromMemory:
      command = kCommandReadEe;
      break;
   case kConfigurationFlagsMemory:
      command = kCommandReadCfg;
      break;
   default:
      throw std::runtime_error((boost::format("CPicBoot::readPic : Invalid memory kind : %1%") % memory).str());
   }

   if (nbBytesToRead / m_picConfiguration->readBlockSize() > kMaxPacket)
      throw std::invalid_argument(
         (boost::format("CPicBoot::readPic : packet too big %1%, expected %2% max") % (nbBytesToRead /
            m_picConfiguration->readBlockSize()) % kMaxPacket).str());

   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   // Build header
   messageToSend->push_back(command);
   messageToSend->push_back(static_cast<unsigned char>(nbBytesToRead / m_picConfiguration->readBlockSize()));
   messageToSend->push_back(static_cast<unsigned char>(blockAddress & 0xFF));
   messageToSend->push_back(static_cast<unsigned char>((blockAddress & 0xFF00) >> 8));
   messageToSend->push_back(static_cast<unsigned char>((blockAddress & 0xFF0000) >> 16));

   const auto receivedMessage = sendGetPacket(messageToSend,
                                              kMaxPacket);

   // Remove header from answer
   if (receivedMessage->size() < 6)
      throw std::invalid_argument(
         (boost::format("CPicBoot::readPic : received message too small %1%, expected 6 min") % receivedMessage->size()).str());

   return boost::make_shared<std::vector<unsigned char>>(receivedMessage->begin() + 5,
                                                         receivedMessage->end());
}

unsigned int CPicBoot::readPicDeviceId()
{
   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   //Device ID is in program memory at 0xFF0000
   static const auto DeviceIdAddress = 0xFF0000;
   messageToSend->push_back(kCommandReadPm);
   messageToSend->push_back(0x01);
   messageToSend->push_back(DeviceIdAddress & 0xFF);
   messageToSend->push_back(DeviceIdAddress >> 8 & 0xFF);
   messageToSend->push_back(DeviceIdAddress >> 16 & 0xFF);

   const auto receivedMessage = sendGetPacket(messageToSend,
                                              kMaxPacket);

   if (receivedMessage->size() != 9)
      throw std::runtime_error(
         (boost::format("CPicBoot::readPicDeviceId : answer message has bad size (%1%), expected (%2%)") %
            receivedMessage->size() % 9).str());

   if (receivedMessage->at(0) != kCommandReadPm || receivedMessage->at(1) != 0x01)
      throw std::runtime_error("CPicBoot::readPicDeviceId : wrong message answer (inconsistent header)");

   const auto deviceIdAddressReceived = receivedMessage->at(2) + (receivedMessage->at(3) << 8) + (receivedMessage->at(4)
      << 16);
   if (deviceIdAddressReceived != DeviceIdAddress)
      throw std::runtime_error(
         (boost::format("CPicBoot::readPicDeviceId : wrong received address %1$#06x, expected %2$#06x") %
            deviceIdAddressReceived % DeviceIdAddress).str());

   return receivedMessage->at(5) + (receivedMessage->at(6) << 8);
}

std::string CPicBoot::readPicVersion()
{
   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   //Build header
   messageToSend->push_back(kCommandReadVer);
   messageToSend->push_back(0x02);

   const auto receivedMessage = sendGetPacket(messageToSend,
                                              kMaxPacket);

   if (receivedMessage->size() != 4)
      throw std::runtime_error(
         (boost::format("CPicBoot::readPicVersion : answer message has bad size (%1%), expected (%2%)") %
            receivedMessage->size() % 4).str());

   if (receivedMessage->at(0) != kCommandReadVer || receivedMessage->at(1) != 0x02)
      throw std::runtime_error("CPicBoot::readPicVersion : wrong message answer (inconsistent header)");

   const auto verL = receivedMessage->at(2);
   const auto verH = receivedMessage->at(3);

   std::stringstream ss;
   ss << static_cast<unsigned int>(verH) << "." << static_cast<unsigned int>(verL);
   return ss.str();
}

void CPicBoot::writePic(const EMemoryKind memory,
                        const unsigned long blockAddress,
                        const std::vector<unsigned char>& blockData)
{
   EBootLoaderCommand command;

   switch (memory)
   {
   case kProgramMemory:
      command = kCommandWritePm;
      break;
   case kEepromMemory:
      command = kCommandWriteEe;
      break;
   case kConfigurationFlagsMemory:
      command = kCommandWriteCfg;
      break;
   default:
      throw std::runtime_error((boost::format("CPicBoot::writePic : Invalid memory kind : %1%") % memory).str());
   }

   if (blockData.size() / m_picConfiguration->writeBlockSize() > kMaxPacket - 6)
      throw std::invalid_argument(
         (boost::format("CPicBoot::writePic : packet too big %1%, expected %2% max") % (blockData.size() /
            m_picConfiguration->writeBlockSize()) % (kMaxPacket - 6)).str());

   if (blockAddress * m_picConfiguration->bytesPerAddr() % m_picConfiguration->writeBlockSize())
      throw std::invalid_argument(
         (boost::format(
            "CPicBoot::writePic : packet bad aligned, bytes number (%1%) is not a multiple of block size (%2%)") % (
            blockAddress * m_picConfiguration->bytesPerAddr()) % m_picConfiguration->writeBlockSize()).str());
   if (blockData.size() % m_picConfiguration->writeBlockSize())
      throw std::invalid_argument(
         (boost::format(
               "CPicBoot::writePic : packet bad aligned, data size (%1%) is not a multiple of block size (%2%)") %
            blockData.size() % m_picConfiguration->writeBlockSize()).str());


   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   // Build header
   messageToSend->push_back(command);
   messageToSend->push_back(static_cast<unsigned char>(blockData.size() / m_picConfiguration->writeBlockSize()));
   messageToSend->push_back(static_cast<unsigned char>(blockAddress & 0xFF));
   messageToSend->push_back(static_cast<unsigned char>((blockAddress & 0xFF00) >> 8));
   messageToSend->push_back(static_cast<unsigned char>((blockAddress & 0xFF0000) >> 16));

   messageToSend->insert(messageToSend->end(), blockData.begin(), blockData.end());

   const auto receivedMessage = sendGetPacket(messageToSend,
                                              kMaxPacket);

   if (receivedMessage->size() != 1 || receivedMessage->back() != command)
      throw std::runtime_error(
         (boost::format("CPicBoot::writePic : answer message has bad size (%1%) or bad command value (%2%)") %
            receivedMessage->size() % receivedMessage->back()).str());
}

void CPicBoot::writePicVerifyOk()
{
   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   //Build header
   messageToSend->push_back(kCommandVerifyOk);

   const auto receivedMessage = sendGetPacket(messageToSend,
                                              kMaxPacket);

   if (receivedMessage->size() != 1)
      throw std::runtime_error(
         (boost::format("CPicBoot::writePicVerifyOk : answer message has bad size (%1%), expected (%2%)") %
            receivedMessage->size() % 1).str());

   if (receivedMessage->at(0) != kCommandVerifyOk)
      throw std::runtime_error("CPicBoot::writePicVerifyOk : wrong message answer");
}

void CPicBoot::erasePic(const unsigned int picAddr,
                        const unsigned int nBlock)
{
   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   //Build header
   messageToSend->push_back(kCommandErasePm);
   messageToSend->push_back(static_cast<unsigned char>(nBlock));
   messageToSend->push_back(static_cast<unsigned char>(picAddr & 0xFF));
   messageToSend->push_back(static_cast<unsigned char>((picAddr & 0xFF00) >> 8));
   messageToSend->push_back(static_cast<unsigned char>((picAddr & 0xFF0000) >> 16));

   const auto receivedMessage = sendGetPacket(messageToSend,
                                              kMaxPacket);

   if (receivedMessage->size() != 1 || receivedMessage->back() != kCommandErasePm)
      throw std::runtime_error(
         (boost::format("CPicBoot::erasePic : answer message has bad size (%1%) or bad command value (%2%)") %
            receivedMessage->size() % receivedMessage->back()).str());
}

void CPicBoot::reBootPic()
{
   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   // To reset the Pic, send any command of 0 length
   //Build header
   messageToSend->push_back(kCommandReadVer);
   messageToSend->push_back(0x00);

   sendPacket(messageToSend);
}

void CPicBoot::erasePicProgramMemory()
{
   //TODO à comparer avec les relevés d'un flashage officiel. Parce
   auto nbBlocksToErase =
      (m_picConfiguration->programAddressEnd() - m_picConfiguration->programAddressStart() + 1) * m_picConfiguration->
      bytesPerAddr()
      / m_picConfiguration->eraseBlockSize();
   auto currentAddress = m_picConfiguration->programAddressStart();
   while (nbBlocksToErase > 0)
   {
      const auto nbBlocksToEraseForThisLoop = nbBlocksToErase > 255 ? 255 : nbBlocksToErase;

      erasePic(currentAddress,
               nbBlocksToEraseForThisLoop);

      nbBlocksToErase -= nbBlocksToEraseForThisLoop;

      currentAddress += m_picConfiguration->eraseBlockSize() / m_picConfiguration->bytesPerAddr() *
         nbBlocksToEraseForThisLoop;
   }
}

bool CPicBoot::verifyPic(const EMemoryKind memory,
                         const unsigned long blockAddress,
                         const std::vector<unsigned char>& refPacketData)
{
   const auto readData = readPic(memory,
                                 blockAddress,
                                 refPacketData.size());

   return equal(readData->begin(),
                readData->end(),
                refPacketData.begin());
}

boost::shared_ptr<const std::vector<unsigned char>> CPicBoot::getPacket(const unsigned int byteLimit)
{
   if (m_eventHandler.waitForEvents(boost::posix_time::seconds(2)) != kEvtPicBootPortDataReceived)
      throw std::runtime_error("Fail to get packet");

   auto message = boost::make_shared<const std::vector<unsigned char>>(
      m_eventHandler.getEventData<const std::vector<unsigned char>>());

   // Check message limit (TODO utile en C++ ?)
   if (message->size() > byteLimit)
      throw std::runtime_error("Reached read limit (received message too large)");

   return message;
}

void CPicBoot::sendPacket(const boost::shared_ptr<const std::vector<unsigned char>> packetData)
{
   if (packetData->size() > kMaxPacket)
      throw std::runtime_error(
         (boost::format("CPicBoot::sendPacket : packetData is too large : %1% bytes, %2% max expected") % packetData->
            size() % kMaxPacket).str());

   std::vector<unsigned char> buffer;

   // Insert the 2 STX
   buffer.push_back(kSTX);
   buffer.push_back(kSTX);

   // Insert data, and escape characters if needed
   unsigned long checksum = 0;
   for (const auto data : *packetData)
   {
      switch (data)
      {
      case kSTX:
      case kETX:
      case kDLE:
         buffer.push_back(kDLE);
         break;
      default:
         break;
      }
      buffer.push_back(data);
      checksum += data;
   }

   // Insert checksum
   checksum = (~checksum + 1) & 0xFF;
   switch (checksum)
   {
   case kSTX:
   case kETX:
   case kDLE:
      buffer.push_back(kDLE);
      break;
   default:
      break;
   }
   buffer.push_back(static_cast<unsigned char>(checksum));

   // Insert the ETX
   buffer.push_back(kETX);

   // Send
   const auto buf = shared::communication::CByteBuffer(buffer);
   m_logger.logSent(buf);
   m_port->send(buf);
}

boost::shared_ptr<const std::vector<unsigned char>> CPicBoot::sendGetPacket(
   const boost::shared_ptr<const std::vector<unsigned char>> packetToSend,
   const unsigned int receiveByteLimit)
{
   do
   {
      sendPacket(packetToSend);

      try
      {
         return getPacket(receiveByteLimit);
      }
      catch (std::exception& exception)
      {
         YADOMS_LOG(warning) << "Error receiveing packet, " << exception.what();
      }
   }
   while (--m_maxRetrys);

   throw std::runtime_error("Fail to send/get packet");
}
