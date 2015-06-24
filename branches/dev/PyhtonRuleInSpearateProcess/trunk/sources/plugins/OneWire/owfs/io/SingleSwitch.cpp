#include "stdafx.h"
#include "SingleSwitch.h"
#include "Common.h"

namespace owfs { namespace io {

CSingleSwitch::CSingleSwitch(const boost::filesystem::path& devicePath)
   :m_devicePath(devicePath / boost::filesystem::path("sensed"))
{
}

CSingleSwitch::~CSingleSwitch()
{
}

bool CSingleSwitch::read() const
{
   std::string readValue = CCommon::read(m_devicePath);

   // Caution : read value correspond to voltage level, inverted from the transistor state
   // We have to invert the read value
   return readValue != "1";
}

void CSingleSwitch::write(bool state) const
{
   std::string filename("PIO");

   boost::filesystem::path writePath = m_devicePath / boost::filesystem::path(filename);
   CCommon::write(writePath, state ? "yes" : "no");
}

} } // namespace owfs::io