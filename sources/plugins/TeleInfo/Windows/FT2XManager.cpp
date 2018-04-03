#include "stdafx.h"
#include "FT2XManager.h"
#include <shared/Log.h>
#include <windows.h>

const unsigned char CFT2XManager::m_mask_port1 = 0x11;
const unsigned char CFT2XManager::m_mask_port2 = 0x22;
const unsigned char CFT2XManager::m_mask_no_port = 0x00;
const unsigned char CFT2XManager::m_enableBitBang = 0x20;

typedef FT_STATUS(__stdcall *f_ftopen)(int deviceNumber, FT_HANDLE *handle);
typedef FT_STATUS(__stdcall *f_ftclose)(FT_HANDLE handle);
typedef FT_STATUS(__stdcall *f_ftsetBitMode)(FT_HANDLE handle, UCHAR ucMask, UCHAR ucEnabled);
typedef FT_STATUS(__stdcall *f_ftgetBitMode)(FT_HANDLE handle, PUCHAR pucMode);
typedef FT_STATUS(__stdcall *f_ftsetbaudRate)(FT_HANDLE handle, ULONG baudRate);

CFT2XManager::CFT2XManager()
{
   FT_STATUS	ftStatus;

   hGetProcIDDLL = LoadLibraryA("ftd2xx.dll");
   
   if (!hGetProcIDDLL) {
      YADOMS_LOG(error) << "could not load the dynamic library";
   }
   else
   {
      f_ftopen  FT_Open = (f_ftopen)GetProcAddress(hGetProcIDDLL, "FT_Open");

      ftStatus = FT_Open(0, &ftHandle);
      if (ftStatus != FT_OK) {
         // TODO : Send a exception - perhaps fail because the driver is not installed
      }
   }

}

//--------------------------------------------------------------
/// \brief	    Destructor
//--------------------------------------------------------------
CFT2XManager::~CFT2XManager()
{
   f_ftclose FT_Close = (f_ftclose)GetProcAddress(hGetProcIDDLL, "FT_Close");

   if (ftHandle != NULL) {
      FT_Close(ftHandle);
      ftHandle = NULL;
   }
}

//--------------------------------------------------------------
/// \brief	    Activate the GPIO pin in parameter
/// \param[in] GPIONumber          The GPIO Pin to activate
//--------------------------------------------------------------
void CFT2XManager::activateGPIO(const int GPIONumber)
{
   unsigned char ucMask = 0xFF;
   unsigned char ucMode;
   DWORD dwBytesInQueue = 0;
   FT_STATUS	ftStatus;

   f_ftsetBitMode FT_SetBitMode = (f_ftsetBitMode)GetProcAddress(hGetProcIDDLL, "FT_SetBitMode");
   f_ftgetBitMode FT_GetBitMode = (f_ftgetBitMode)GetProcAddress(hGetProcIDDLL, "FT_GetBitMode");
   f_ftsetbaudRate FT_SetBaudRate = (f_ftsetbaudRate)GetProcAddress(hGetProcIDDLL, "FT_SetBaudRate");

   switch (GPIONumber)
   {
   case 1:
      ucMask = m_mask_port1;
      break;
   case 2:
      ucMask = m_mask_port2;
      break;
   default:
      ucMask = m_mask_no_port;
   }

   ucMode = m_enableBitBang;

   ftStatus = FT_SetBitMode(ftHandle, ucMask, ucMode);

   if (ftStatus != FT_OK)  
      YADOMS_LOG(error) << "Failed to set bit mode for port: " << GPIONumber;

   // TODO : Récupérer la configuration à partor du constructeur & du Factory
   FT_SetBaudRate(ftHandle, FT_BAUD_1200);

   //FT_Write(ftHandle, &ucMode, 1, &dwBytesInQueue);

   ftStatus = FT_GetBitMode(ftHandle, &ucMode);
   if (ftStatus != FT_OK) {
      YADOMS_LOG(error) << "Failed to get bit mode";
   }
}

//--------------------------------------------------------------
/// \brief	    Desactivate the GPIO pin in parameter
/// \param[in] GPIONumber          The GPIO Pin to desactivate
//--------------------------------------------------------------
void CFT2XManager::desactivateGPIO(const int GPIONumber)
{
   unsigned char ucMask = 0xFF;
   unsigned char ucMode;
   DWORD dwBytesInQueue = 0;
   FT_STATUS	ftStatus;

   f_ftsetBitMode FT_SetBitMode = (f_ftsetBitMode)GetProcAddress(hGetProcIDDLL, "FT_SetBitMode");
   f_ftgetBitMode FT_GetBitMode = (f_ftgetBitMode)GetProcAddress(hGetProcIDDLL, "FT_GetBitMode");
   f_ftsetbaudRate FT_SetBaudRate = (f_ftsetbaudRate)GetProcAddress(hGetProcIDDLL, "FT_SetBaudRate");

   switch (GPIONumber)
   {
   case 1:
      ucMask = m_mask_port1;
      break;
   case 2:
      ucMask = m_mask_port2;
      break;
   default:
      ucMask = m_mask_no_port;
   }

   ucMode = m_enableBitBang;

   ftStatus = FT_SetBitMode(ftHandle, ucMask, ucMode);

   if (ftStatus != FT_OK)
      YADOMS_LOG(error) << "Failed to set bit mode for port: " << GPIONumber;

   FT_SetBaudRate(ftHandle, FT_BAUD_1200);

   //FT_Write(ftHandle, &ucMode, 1, &dwBytesInQueue);

   ftStatus = FT_GetBitMode(ftHandle, &ucMode);
   if (ftStatus != FT_OK) {
      YADOMS_LOG(error) << "Failed to get bit mode";
   }
}