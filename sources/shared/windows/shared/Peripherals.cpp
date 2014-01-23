#include "stdafx.h"

#include "DynamicLibrary.hpp"
#include "../../shared/Exceptions/Exception.hpp"
#include "../../shared/Log.h"
#include "Peripherals.h"
#include <windows.h>


// Defines From MsPorts.h original file
DECLARE_HANDLE(HCOMDB);
typedef HCOMDB *PHCOMDB;

/* ReportType flags for ComDBGetCurrentPortUsage */
#define CDB_REPORT_BITS  0x0
#define CDB_REPORT_BYTES 0x1
// [END] Defines From MsPorts.h original file


class CMsPortsLibrary : protected CDynamicLibrary
{
protected:
   typedef LONG (CALLBACK* ComDBOpenFunctionType)(PHCOMDB);
   ComDBOpenFunctionType m_ComDBOpen;

   typedef LONG (CALLBACK* ComDBCloseFunctionType)(HCOMDB);
   ComDBCloseFunctionType m_ComDBClose;

   typedef LONG (CALLBACK* ComDBGetCurrentPortUsageFunctionType)(HCOMDB, PBYTE, DWORD, DWORD, LPDWORD);
   ComDBGetCurrentPortUsageFunctionType m_ComDBGetCurrentPortUsage;

   typedef LONG (CALLBACK* ComDBClaimNextFreePortFunctionType)(HCOMDB, LPDWORD);
   ComDBClaimNextFreePortFunctionType m_ComDBClaimNextFreePort;

public:
   CMsPortsLibrary()
   {
      if (!load("MsPorts.dll"))
         throw CException("MsPorts.dll not found");

      m_ComDBOpen = (ComDBOpenFunctionType) GetFunctionPointer("ComDBOpen");
      m_ComDBClose = (ComDBCloseFunctionType) GetFunctionPointer("ComDBClose");
      m_ComDBGetCurrentPortUsage = (ComDBGetCurrentPortUsageFunctionType) GetFunctionPointer("ComDBGetCurrentPortUsage");
      m_ComDBClaimNextFreePort = (ComDBClaimNextFreePortFunctionType) GetFunctionPointer("ComDBClaimNextFreePort");

      if(m_ComDBOpen == NULL || m_ComDBClose == NULL || m_ComDBGetCurrentPortUsage == NULL || m_ComDBClaimNextFreePort == NULL)
      {
         unload();
         throw CException("MsPorts.dll is invalid");
      }
   }

   virtual ~CMsPortsLibrary()
   {
      unload();
   }

   LONG ComDBOpen(HCOMDB* phComDB)
   {
      BOOST_ASSERT(m_ComDBOpen);
      if(m_ComDBOpen != NULL)
         return m_ComDBOpen(phComDB);
      return false;
   }

   LONG ComDBClose(HCOMDB phComDB)
   {
      BOOST_ASSERT(m_ComDBClose);
      if(m_ComDBClose != NULL)
         return m_ComDBClose(phComDB);
      return false;
   }

   LONG ComDBGetCurrentPortUsage(HCOMDB hComDB, PBYTE Buffer, DWORD BufferSize, DWORD ReportType, LPDWORD MaxPortsReported)
   {
      BOOST_ASSERT(m_ComDBGetCurrentPortUsage);
      if(m_ComDBGetCurrentPortUsage != NULL)
         return m_ComDBGetCurrentPortUsage(hComDB, Buffer, BufferSize, ReportType, MaxPortsReported);
      return false;
   }

   LONG ComDBClaimNextFreePort(HCOMDB hComDB, LPDWORD ComNumber)
   {
      BOOST_ASSERT(m_ComDBClaimNextFreePort);
      if(m_ComDBClaimNextFreePort != NULL)
         return m_ComDBClaimNextFreePort(hComDB, ComNumber);
      return false;
   }
};


CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

const boost::shared_ptr<CPeripherals::SerialPortsMap> CPeripherals::getSerialPorts()
{
   boost::shared_ptr<SerialPortsMap> serialPorts(new SerialPortsMap);

   HKEY serialcommKey;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_QUERY_VALUE, &serialcommKey) != ERROR_SUCCESS)
   {
      // Unable to access registry ==> HARDWARE\\DEVICEMAP\\SERIALCOMM key does not exist ==> no serial port on this machine
      return serialPorts;
   }

   boost::shared_ptr<std::set<std::string> > usedSerialPorts = getUsedSerialPorts();

   DWORD valueIndex = 0;
   static const DWORD serialPortNameMaxLength = 1000;    // Should be enough
   TCHAR serialPortName[serialPortNameMaxLength];
   DWORD serialPortNameLength = serialPortNameMaxLength;
   DWORD dataType = 0;
   static const DWORD mountPointMaxLength = 1000;    // Should be enough
   BYTE mountPoint[mountPointMaxLength];
   DWORD mountPointLength = mountPointMaxLength;
   while (RegEnumValue(serialcommKey, valueIndex, serialPortName, &serialPortNameLength, NULL, &dataType, mountPoint, &mountPointLength) == ERROR_SUCCESS)
   {
      if (dataType == REG_SZ)
      {
         std::string portName((char*)mountPoint);
         (*serialPorts)[portName] = portName;
         if (usedSerialPorts->find(portName) != usedSerialPorts->end())
         {
            // Port is in use
            (*serialPorts)[portName] += " (in use)";
         }
      }

      valueIndex++;
   }

   RegCloseKey(serialcommKey);
   return serialPorts;
}

const boost::shared_ptr<std::set<std::string> > CPeripherals::getUsedSerialPorts()
{
   boost::shared_ptr<std::set<std::string> > usedSerialPorts(new std::set<std::string>);

   try
   {
      CMsPortsLibrary msPortLibrary;

      HCOMDB hComDB;
      if (msPortLibrary.ComDBOpen(&hComDB) != ERROR_SUCCESS)
         throw CException("Unable to open COM database");

      // First get the ports number
      DWORD maxPortsReported = 0;
      if (msPortLibrary.ComDBGetCurrentPortUsage(hComDB, NULL, 0, 0, &maxPortsReported) != ERROR_SUCCESS)
         throw CException("Unable to get serial ports number");

      // Now get the serial port usage
      unsigned char* portsInUse = new unsigned char[maxPortsReported]();
      if (msPortLibrary.ComDBGetCurrentPortUsage(hComDB, portsInUse, maxPortsReported, CDB_REPORT_BYTES, NULL) != ERROR_SUCCESS)
         throw CException("Unable to get serial ports usage");

      for (DWORD idxPort = 0 ; idxPort < maxPortsReported ; idxPort ++)
      {
         if (portsInUse[idxPort])
         {
            std::ostringstream os;
            os << "COM" << (idxPort + 1);
            usedSerialPorts->insert(os.str());
         }
      }

      delete[] portsInUse;
      msPortLibrary.ComDBClose(hComDB);

      return usedSerialPorts;
   }
   catch (CException& e)
   {
      YADOMS_LOG(error) << "unable to load MsPorts.dll : " << e.what();
      return usedSerialPorts;
   }

   return usedSerialPorts;
}

