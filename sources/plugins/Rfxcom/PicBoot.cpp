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

/****************************************************************************

    PROGRAM:	PICBOOT.c

    PURPOSE:	The purpose of this DLL is to provide a communications base for 
            quick and easy higher level development.

    FUNCTIONS:	HANDLE  OpenPIC(LPSTR, DWORD, DWORD)
            INT  ClosePIC(HANDLE)
            INT  getPacket(HANDLE, BYTE *, WORD)
            INT  sendPacket(HANDLE, BYTE *, WORD)
            INT  SendGetPacket(HANDLE, BYTE *, WORD, WORD, WORD)
            DWORD  WritePIC(HANDLE, PIC *, BYTE *)
            DWORD  readPIC(HANDLE, PIC *, BYTE *)     

*******************************************************************************/
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
                   boost::posix_time::time_duration readTimeOut)
{
   m_port = boost::make_shared<shared::communication::CAsyncSerialPort>(comPort,
                                                                        boost::asio::serial_port_base::baud_rate(38400));
   m_port->subscribeForConnectionEvents(m_eventHandler, kEvtPicBootPortConnection);

   auto receiveBufferHandler = boost::make_shared<CPicBootReceiveBufferHandler>(m_eventHandler,
                                                                                kEvtPicBootPortDataReceived,
                                                                                readTimeOut);
   m_port->setReceiveBufferHandler(receiveBufferHandler);

   m_port->start();

   if (m_eventHandler.waitForEvents(boost::posix_time::seconds(10)) != kEvtPicBootPortConnection)
      throw std::runtime_error("Unable to open serial port");

   receiveBufferHandler->flush();
}

CPicBoot::~CPicBoot()
{
}


boost::shared_ptr<const std::vector<unsigned char>> CPicBoot::getPacket(unsigned int byteLimit)
{
   if (m_eventHandler.waitForEvents(boost::posix_time::seconds(2)) != kEvtPicBootPortDataReceived)
      throw std::runtime_error("Fail to get packet");

   auto message = boost::make_shared<const std::vector<unsigned char>>(m_eventHandler.getEventData<const std::vector<unsigned char>>());

   // Check message limit (TODO utile en C++ ?)
   if (message->size() > byteLimit)
      throw std::runtime_error("Reached read limit (received message too large)");

   return message;
}

void CPicBoot::sendPacket(boost::shared_ptr<const std::vector<unsigned char>> packetData) const
{
   if (packetData->size() > kMAX_PACKET)
      throw std::runtime_error((boost::format("CPicBoot::sendPacket : packetData is too large : %1% bytes, %2% max expected") % packetData->size() % kMAX_PACKET).str());

   std::vector<unsigned char> buffer;

   // Insert the 2 STX
   buffer.push_back(kSTX);
   buffer.push_back(kSTX);

   // Insert data, and escape characters if needed
   unsigned long checksum = 0;
   for (const auto data:*packetData)
   {
      switch (data)
      {
      case kSTX:
      case kETX:
      case kDLE:
         buffer.push_back(kDLE);
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
   }
   buffer.push_back(static_cast<unsigned char>(checksum));

   // Insert the ETX
   buffer.push_back(kETX);

   // Send
   m_port->send(shared::communication::CByteBuffer(buffer));
}

boost::shared_ptr<const std::vector<unsigned char>> CPicBoot::sendGetPacket(boost::shared_ptr<const std::vector<unsigned char>> packetToSend,
                                                                            unsigned int receiveByteLimit,
                                                                            unsigned int numOfRetrys)
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
   while (--numOfRetrys);

   throw std::runtime_error("Fail to send/get packet");
}

boost::shared_ptr<std::vector<unsigned char>> CPicBoot::readPIC(const CPic& pic)
{
   switch (pic.BootCmd)
   {
   case kCommandReadPm:
   case kCommandReadEe:
   case kCommandReadCfg:
      {
         //Limit to 1 to 3 bytes per addr
         if (!pic.BytesPerAddr)
            throw std::invalid_argument((boost::format("CPicBoot::readPIC : bytes per address value too small %1%") % pic.BytesPerAddr).str());
         if (pic.BytesPerAddr > 3)
            throw std::invalid_argument((boost::format("CPicBoot::readPIC : bytes per address value too big %1%") % pic.BytesPerAddr).str());

         if (pic.BootDatLen / pic.BytesPerBlock > kMAX_PACKET)
            throw std::invalid_argument((boost::format("CPicBoot::readPIC : packet too big %1%, expected %2% max") % (pic.BootDatLen / pic.BytesPerBlock) % kMAX_PACKET).str());

         auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

         //Build header
         messageToSend->push_back(pic.BootCmd);
         messageToSend->push_back(static_cast<unsigned char>(pic.BootDatLen / pic.BytesPerBlock));
         messageToSend->push_back(static_cast<unsigned char>(pic.BootAddr & 0xFF));
         messageToSend->push_back(static_cast<unsigned char>((pic.BootAddr & 0xFF00) >> 8));
         messageToSend->push_back(static_cast<unsigned char>((pic.BootAddr & 0xFF0000) >> 16));

         const auto receivedMessage = sendGetPacket(messageToSend, kMAX_PACKET, pic.MaxRetrys);

         // Remove header from answer
         return boost::make_shared<std::vector<unsigned char>>(receivedMessage->begin() + 5,
                                                               receivedMessage->end());
      }
   case kCommandReadVer:
      {
         auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

         //Build header
         messageToSend->push_back(pic.BootCmd);
         messageToSend->push_back(0x02);

         const auto receivedMessage = sendGetPacket(messageToSend, kMAX_PACKET, pic.MaxRetrys);

         // Remove header from answer
         return boost::make_shared<std::vector<unsigned char>>(receivedMessage->begin() + 2,
                                                               receivedMessage->end());
      }
   default:
      throw std::invalid_argument((boost::format("CPicBoot::readPIC : Invalid command %1%") % pic.BootCmd).str());
   }
}

void CPicBoot::writePIC(const CPic& pic,
                        boost::shared_ptr<std::vector<unsigned char>> packetData)
{
   switch (pic.BootCmd)
   {
   case kCommandWritePm:
   case kCommandWriteEe:
   case kCommandWriteCfg:
      break;
   default:
      throw std::invalid_argument((boost::format("CPicBoot::writePIC : Invalid command %1%") % pic.BootCmd).str());
   }

   //Limit to 1 to 3 bytes per addr
   if (!pic.BytesPerAddr)
      throw std::invalid_argument((boost::format("CPicBoot::writePIC : bytes per address value too small %1%") % pic.BytesPerAddr).str());
   if (pic.BytesPerAddr > 3)
      throw std::invalid_argument((boost::format("CPicBoot::writePIC : bytes per address value too big %1%") % pic.BytesPerAddr).str());

   if (pic.BootDatLen / pic.BytesPerBlock > kMAX_PACKET - 6)
      throw std::invalid_argument((boost::format("CPicBoot::writePIC : packet too big %1%, expected %2% max") % (pic.BootDatLen / pic.BytesPerBlock) % (kMAX_PACKET - 6)).str());

   if (pic.BootAddr * pic.BytesPerAddr % pic.BytesPerBlock)
      throw std::invalid_argument((boost::format("CPicBoot::writePIC : packet bad aligned, bytes number (%1%) is not a multiple of block size (%2%)") % (pic.BootAddr * pic.BytesPerAddr) % pic.BytesPerBlock).str());
   if (pic.BootDatLen % pic.BytesPerBlock)
      throw std::invalid_argument((boost::format("CPicBoot::writePIC : packet bad aligned, data size (%1%) is not a multiple of block size (%2%)") % pic.BootDatLen % pic.BytesPerBlock).str());


   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   //Build header
   messageToSend->push_back(pic.BootCmd);
   messageToSend->push_back(static_cast<unsigned char>(pic.BootDatLen / pic.BytesPerBlock));
   messageToSend->push_back(static_cast<unsigned char>(pic.BootAddr & 0xFF));
   messageToSend->push_back(static_cast<unsigned char>((pic.BootAddr & 0xFF00) >> 8));
   messageToSend->push_back(static_cast<unsigned char>((pic.BootAddr & 0xFF0000) >> 16));

   messageToSend->insert(messageToSend->end(), packetData->begin(), packetData->end());

   const auto receivedMessage = sendGetPacket(messageToSend, kMAX_PACKET, pic.MaxRetrys);

   if (receivedMessage->size() != 1 || receivedMessage->back() != pic.BootCmd)
      throw std::runtime_error((boost::format("CPicBoot::writePIC : answer message has bad size (%1%) or bad command value (%2%)") % receivedMessage->size() % receivedMessage->back()).str());
}

void CPicBoot::erasePIC(unsigned int PicAddr,
                        unsigned int nBlock,
                        unsigned int nRetry)
{
   auto messageToSend = boost::make_shared<std::vector<unsigned char>>();

   //Build header
   messageToSend->push_back(kCommandErasePm);
   messageToSend->push_back(static_cast<unsigned char>(nBlock));
   messageToSend->push_back(static_cast<unsigned char>(PicAddr & 0xFF));
   messageToSend->push_back(static_cast<unsigned char>((PicAddr & 0xFF00) >> 8));
   messageToSend->push_back(static_cast<unsigned char>((PicAddr & 0xFF0000) >> 16));

   const auto receivedMessage = sendGetPacket(messageToSend, kMAX_PACKET, nRetry);

   if (receivedMessage->size() != 1 || receivedMessage->back() != kCommandErasePm)
      throw std::runtime_error((boost::format("CPicBoot::erasePIC : answer message has bad size (%1%) or bad command value (%2%)") % receivedMessage->size() % receivedMessage->back()).str());
}

bool CPicBoot::verifyPIC(const CPic& pic,
                         boost::shared_ptr<std::vector<unsigned char>> refPacketData)
{
   switch (pic.BootCmd)
   {
   case kCommandReadPm:
   case kCommandReadEe:
   case kCommandReadCfg:
      break;
   default:
      throw std::invalid_argument((boost::format("CPicBoot::verifyPIC : Invalid command %1%") % pic.BootCmd).str());
   }

   const auto readData = readPIC(pic);

   return std::equal(readData->begin(), readData->end(), refPacketData->begin());
}

