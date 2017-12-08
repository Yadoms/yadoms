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
#include "picConfigurations/IPicConfiguration.h"
#include <shared/communication/BufferLogger.h>

class CPicBoot
{
public:
   //Error conditions
   enum EError
   {
      kErrorGenReadwrite = -1,
      kErrorReadTimeout = -2,
      kErrorReadLimit = -3,
      kErrorBadChksum = -4,
      kErrorRetryLimit = -5,
      kErrorInvalidCommand = -6,
      kErrorBlockTooSmall = -7,
      kErrorPacketTooBig = -8,
      kErrorOffBoundry = -9,
      kErrorBpaTooSmall = -10,
      kErrorBpaTooBig = -11,
      kErrorVerifyFailed = -12,
      kErrorOffBoundry2 = -13
   };


   //Limits
   enum ELimits
   {
      kMaxPacket = 261
   };

   // Memory type
   enum EMemoryKind
   {
      kProgramMemory,
      kEepromMemory,
      kConfigurationFlagsMemory
   };


   enum EBootLoaderCommand : unsigned char //TODO passer en privé ?
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

   //PIC structure used for some functions
   struct CPic //TODO passer en privé ?
   {
      // Bootloader command
      EBootLoaderCommand bootCmd;

      //Number of bytes to read/write (for a block)
      unsigned int bootDatLen;

      //24 bit memory address (Prog or EE)
      unsigned int bootAddr;

      //Number of bytes in a command block (page, row, instruction, etc)
      unsigned int bytesPerBlock;

      //Number of bytes per address
      unsigned int bytesPerAddr;

      // Device type
      picConfigurations::IPicConfiguration::EDeviceType deviceType;
   };


   CPicBoot(const std::string& comPort,
            boost::posix_time::time_duration readTimeOut,
            unsigned int maxRetrys);
   virtual ~CPicBoot();

   void setPicConfiguration(boost::shared_ptr<picConfigurations::IPicConfiguration> picConfiguration);

   // High level functions
   boost::shared_ptr<std::vector<unsigned char>> readPic(const EMemoryKind memory,
                                                         const unsigned long blockAddress,
                                                         const unsigned int nbBytesToRead);
   unsigned int readPicDeviceId();
   std::string readPicVersion();
   void writePic(const EMemoryKind memory,
                 unsigned long blockAddress,
                 const std::vector<unsigned char>& blockData);
   void writePicVerifyOk();
   void erasePic(unsigned int picAddr,
                 unsigned int nBlock);
   bool verifyPic(const EMemoryKind memory,
                  const unsigned long blockAddress,
                  const std::vector<unsigned char>& refPacketData);
   void reBootPic();
   void erasePicProgramMemory();

private:
   // Low level functions
   boost::shared_ptr<const std::vector<unsigned char>> getPacket(unsigned int byteLimit);
   void sendPacket(boost::shared_ptr<const std::vector<unsigned char>> packetData);
   boost::shared_ptr<const std::vector<unsigned char>> sendGetPacket(
      boost::shared_ptr<const std::vector<unsigned char>> packetToSend,
      unsigned int receiveByteLimit);


   boost::shared_ptr<shared::communication::CAsyncSerialPort> m_port;
   shared::event::CEventHandler m_eventHandler;
   boost::posix_time::ptime m_lastReceivedTime;
   shared::communication::CBufferLogger m_logger;
   unsigned int m_maxRetrys;
   boost::shared_ptr<picConfigurations::IPicConfiguration> m_picConfiguration;
};
