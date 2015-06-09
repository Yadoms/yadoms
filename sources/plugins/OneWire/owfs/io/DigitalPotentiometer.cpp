#include "stdafx.h"
#include "DigitalPotentiometer.h"
#include "Common.h"

namespace owfs { namespace io {

CDigitalPotentiometer::CDigitalPotentiometer(const boost::filesystem::path& devicePath)
   :m_modePath(devicePath / boost::filesystem::path("chargepump")),
   m_dimPath(devicePath / boost::filesystem::path("wiper"))
{
}

CDigitalPotentiometer::~CDigitalPotentiometer()
{
}

bool CDigitalPotentiometer::readPotentiometerMode() const
{
   std::string readValue = CCommon::read(m_modePath);

   // Potentiometer mode enabled/disabled
   // 0 = variable resistance, 1 = potentiometer mode
   return readValue == "1";
}

void CDigitalPotentiometer::writePotentiometerMode(bool potentiometerMode) const
{
   // Potentiometer mode enabled/disabled
   // 0 = variable resistance, 1 = potentiometer mode
   CCommon::write(m_modePath, potentiometerMode ? "1" : "0");
}

int CDigitalPotentiometer::readDim() const
{
   std::string readValue = CCommon::read(m_dimPath);
   if (readValue.empty())
      return 0;

   // Value from 0 to 255 in OWFS, from 0 to 100 in Yadoms
   return atoi(readValue.c_str()) * 100 / 255;
}

void CDigitalPotentiometer::writeDim(int dim) const
{
   // Value from 0 to 255 in OWFS, from 0 to 100 in Yadoms
   CCommon::write(m_dimPath, boost::lexical_cast<std::string>(dim * 255 / 100));
}

} } // namespace owfs::io