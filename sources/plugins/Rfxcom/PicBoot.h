/******************************************************************************\
* These sources (PICBOOT.h and PICBOOT.cpp) are very inspired from Microchip PIC24F Programmer
* PICBOOT.dll official sources.
* They have just been transformed to C++, adapted to be cross-platform
* and compatible with our RFXCom plugin.
\******************************************************************************/

/******************************************************************************\
*	PIC24F Serial Bootloader Communications Handler (header file)
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
#pragma once

#include <shared/communication/AsyncSerialPort.h>

class CPicBoot
{
public:
   //Error conditions
   enum EError
   {
      kERROR_GEN_READWRITE = -1,
      kERROR_READ_TIMEOUT = -2,
      kERROR_READ_LIMIT = -3,
      kERROR_BAD_CHKSUM = -4,
      kERROR_RETRY_LIMIT = -5,
      kERROR_INVALID_COMMAND = -6,
      kERROR_BLOCK_TOO_SMALL = -7,
      kERROR_PACKET_TOO_BIG = -8,
      kERROR_OFF_BOUNDRY = -9,
      kERROR_BPA_TOO_SMALL = -10,
      kERROR_BPA_TOO_BIG = -11,
      kERROR_VERIFY_FAILED = -12,
      kERROR_OFF_BOUNDRY2 = -13
   };


   //Limits
   enum ELimits
   {
      kMAX_PACKET = 261
   };


   enum EBootLoaderCommand : unsigned char
   {
      kCommandReadVer= 0,
      kCommandReadPm = 1,
      kCommandWritePm = 2,
      kCommandErasePm = 3,
      kCommandReadEe = 4,
      kCommandWriteEe = 5,
      kCommandReadCfg = 6,
      kCommandWriteCfg = 7,
      kCommandVerifyOk = 8
   };

   enum EDeviceType
   {
      kUnknown = 0,
      kPIC24F = 1,
      kPIC24FJ = 2
   };

   //PIC structure used for some functions
   struct CPic //TODO passer en privé ?
   {
      // Bootloader command
      EBootLoaderCommand BootCmd;

      //Number of bytes to read/write
      unsigned int BootDatLen;

      //24 bit memory address (Prog or EE)
      unsigned int BootAddr;

      //Number of bytes in a command block (page, row, instruction, etc)
      unsigned int BytesPerBlock;

      //Number of bytes per address
      unsigned int BytesPerAddr;

      // Device type
      EDeviceType DeviceType;
   };


   CPicBoot(const std::string& comPort,
            boost::posix_time::time_duration readTimeOut,
            unsigned int maxRetrys);
   virtual ~CPicBoot();

   boost::shared_ptr<std::vector<unsigned char>> readPic(const CPic& pic);
   std::string readPicVersion();
   void writePic(const CPic& pic,
                 boost::shared_ptr<std::vector<unsigned char>> packetData);
   void erasePic(unsigned int PicAddr,
                 unsigned int nBlock);
   bool verifyPic(const CPic& pic,
                  boost::shared_ptr<std::vector<unsigned char>> refPacketData);
   void reBootPic() const;
   void erasePicProgramMemory(unsigned int firstAddress,
                              unsigned int lastAddress);
private:

   boost::shared_ptr<const std::vector<unsigned char>> getPacket(unsigned int byteLimit);
   void sendPacket(boost::shared_ptr<const std::vector<unsigned char>> packetData) const;
   boost::shared_ptr<const std::vector<unsigned char>> sendGetPacket(boost::shared_ptr<const std::vector<unsigned char>> packetToSend,
                                                                     unsigned int receiveByteLimit);


   boost::shared_ptr<shared::communication::CAsyncSerialPort> m_port;
   shared::event::CEventHandler m_eventHandler;
   boost::posix_time::ptime m_lastReceivedTime;
   unsigned int m_maxRetrys;
};

