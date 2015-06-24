#include "stdafx.h"
#include "TemperatureIo.h"
#include "MultiSwitch.h"
#include "Temperature.hpp"

namespace owfs { namespace io {

CTemperatureIo::CTemperatureIo(const boost::filesystem::path& devicePath)
   :m_binaryIos(boost::make_shared<CMultiSwitch>(devicePath, CMultiSwitch::kUnitIsUppercaseLetter)), m_temperatureIo(boost::make_shared<CTemperature>(devicePath))
{
}

CTemperatureIo::~CTemperatureIo()
{
}

bool CTemperatureIo::readIo(unsigned char unit) const
{
   return m_binaryIos->read(unit);
}

void CTemperatureIo::writeIo(unsigned char unit, bool state) const
{
   m_binaryIos->write(unit, state);
}

double CTemperatureIo::readTemperature() const
{
   return m_temperatureIo->read();
}

} } // namespace owfs::io