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
   struct CPic
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

      //Number of retries before failure
      unsigned int MaxRetrys;

      // Device type
      EDeviceType DeviceType;
   };



   CPicBoot(const std::string& comPort,
            boost::posix_time::time_duration readTimeOut);
   virtual ~CPicBoot();


   /****************************************************************************

       FUNCTION:	getPacket

       PURPOSE:	This function captures data from the opened source and 
               strips out special control characters. The length of the 
               packet is returned.

   ****************************************************************************/
   boost::shared_ptr<const std::vector<unsigned char>> getPacket(unsigned int byteLimit);

   /****************************************************************************

       FUNCTION:	sendPacket

       PURPOSE:	This function translates and transmitts a packet of data to 
               communicate with the bootloader firmware on the PIC
               microcontroller.

   ****************************************************************************/
   void sendPacket(boost::shared_ptr<const std::vector<unsigned char>> packetData) const;

   /****************************************************************************

       FUNCTION:	SendGetPacket

       PURPOSE:	This function is a combined function of the sendPacket and
               getPacket functions. A retry option has been added to allow
               retransmission and reception in the event of normal 
               communications failure. 

   ****************************************************************************/
   boost::shared_ptr<const std::vector<unsigned char>> sendGetPacket(boost::shared_ptr<const std::vector<unsigned char>> packetToSend,
                                                                     unsigned int receiveByteLimit,
                                                                     unsigned int numOfRetrys);

   /****************************************************************************

       FUNCTION:	readPIC

       PURPOSE:	This is a simple read function. 

   ****************************************************************************/
   boost::shared_ptr<std::vector<unsigned char>> readPIC(const CPic& pic);

   /****************************************************************************

       FUNCTION:	

       PURPOSE: 	This is a simple write function. 

   ****************************************************************************/
   void writePIC(const CPic& pic,
                 boost::shared_ptr<std::vector<unsigned char>> packetData);

   /****************************************************************************

       FUNCTION:	ErasePIC

       PURPOSE:	Simple erase function.

   ****************************************************************************/
   void erasePIC(unsigned int PicAddr,
                 unsigned int nBlock,
                 unsigned int nRetry);

   /****************************************************************************

       FUNCTION:	

       PURPOSE:   Simple verify function

   ****************************************************************************/
   bool verifyPIC(const CPic& pic,
                  boost::shared_ptr<std::vector<unsigned char>> refPacketData);

private:
   boost::shared_ptr<shared::communication::CAsyncSerialPort> m_port;
   shared::event::CEventHandler m_eventHandler;
   boost::posix_time::ptime m_lastReceivedTime;
};

