#include "stdafx.h"
#include "HighPrecisionDigitalThermometer.h"
#include <fstream>
#include <shared/Log.h>

namespace kernel
{
   namespace io
   {
      CHighPrecisionDigitalThermometer::CHighPrecisionDigitalThermometer(const boost::filesystem::path& devicePath)
         :m_devicePath(devicePath / boost::filesystem::path("w1_slave"))
      {
      }

      CHighPrecisionDigitalThermometer::~CHighPrecisionDigitalThermometer()
      {
      }

      double CHighPrecisionDigitalThermometer::read() const
      {
         // The only native-supported device (by w1-therm module)

         static const std::string tag("t=");

         std::ifstream file(m_devicePath.string());
         if (!file.is_open())
         {
            YADOMS_LOG(error) << "1-Wire, Unable to read device " << m_devicePath;
            return 0.0;
         }

         std::string line;
         bool bFoundCrcOk = false;
         std::string data;
         while (getline(file, line))
         {
            unsigned int tpos;
            if (line.find("crc=") != std::string::npos)
            {
               if (line.find("YES") != std::string::npos)
                  bFoundCrcOk = true;
            }
            else if ((tpos = line.find(tag)) != std::string::npos)
            {
               data = line.substr(tpos + tag.length());
            }
         }

         if (bFoundCrcOk)
            return (float)atoi(data.c_str()) / 1000.0f; // Temperature given by kernel is in thousandths of degrees

         YADOMS_LOG(error) << "1-Wire, wrong CRC when reading device value";
         return 0.0;
      }
   }
} // namespace kernel::io
