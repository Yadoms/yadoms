#include "stdafx.h"
#include "FT2XManager.h"
#include <shared/Log.h>

const unsigned char CFT2XManager::m_mask_port1 = 0x11;
const unsigned char CFT2XManager::m_mask_port2 = 0x22;
const unsigned char CFT2XManager::m_mask_no_port = 0x00;
const unsigned char CFT2XManager::m_enableBitBang = 0x20;

CFT2XManager::CFT2XManager()
{
   FT_STATUS	ftStatus;

   //f_ftopen  FT_Open = (f_ftopen)GetProcAddress(hGetProcIDDLL, "FT_Open");

   ftStatus = FT_Open(0, &ftHandle);
   if (ftStatus != FT_OK) {
      // TODO : Send a exception - perhaps fail because the driver is not installed
   }
}

CFT2XManager::~CFT2XManager()
{
   //f_ftclose FT_Close = (f_ftclose)GetProcAddress(hGetProcIDDLL, "FT_Close");

   if (ftHandle != NULL) {
      FT_Close(ftHandle);
      ftHandle = NULL;
   }
}

void CFT2XManager::activateGPIO(const int GPIONumber)
{
   unsigned char ucMask = 0xFF;
   unsigned char ucMode;
   FT_STATUS	ftStatus;

   //f_ftsetBitMode FT_SetBitMode = (f_ftsetBitMode)GetProcAddress(hGetProcIDDLL, "FT_SetBitMode");
   //f_ftgetBitMode FT_GetBitMode = (f_ftgetBitMode)GetProcAddress(hGetProcIDDLL, "FT_GetBitMode");
   //f_ftsetbaudRate FT_SetBaudRate = (f_ftsetbaudRate)GetProcAddress(hGetProcIDDLL, "FT_SetBaudRate");

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
   //FT_SetBaudRate(ftHandle, FT_BAUD_1200);

   ftStatus = FT_GetBitMode(ftHandle, &ucMode);
   if (ftStatus != FT_OK) {
      YADOMS_LOG(error) << "Failed to get bit mode";
   }
}

void CFT2XManager::desactivateGPIO()
{
   // No GPIO activate => No access
   activateGPIO(0);
}