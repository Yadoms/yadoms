#include "stdafx.h"
#include "HighPrecisionLiBatteryMonitor.h"
#include "SingleSwitch.h"
#include "Temperature.hpp"
#include "Voltage.hpp"

namespace owfs { namespace io {

CHighPrecisionLiBatteryMonitor::CHighPrecisionLiBatteryMonitor(const boost::filesystem::path& devicePath)
   :m_binaryIo(boost::make_shared<CSingleSwitch>(devicePath)),
   m_temperatureIo(boost::make_shared<CTemperature>(devicePath)),
   m_visIo(boost::make_shared<CVoltage>(devicePath, "vis")),
   m_voltIo(boost::make_shared<CVoltage>(devicePath, "volt"))
{
}

CHighPrecisionLiBatteryMonitor::~CHighPrecisionLiBatteryMonitor()
{
}

bool CHighPrecisionLiBatteryMonitor::readIo() const
{
   return m_binaryIo->read();
}

void CHighPrecisionLiBatteryMonitor::writeIo(bool state) const
{
   m_binaryIo->write(state);
}

double CHighPrecisionLiBatteryMonitor::readTemperature() const
{
   return m_temperatureIo->read();
}

double CHighPrecisionLiBatteryMonitor::readVis() const
{
   return m_visIo->read();
}

double CHighPrecisionLiBatteryMonitor::readVolt() const
{
   return m_voltIo->read();
}

} } // namespace owfs::io