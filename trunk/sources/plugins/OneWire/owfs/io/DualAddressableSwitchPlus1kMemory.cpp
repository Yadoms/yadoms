#include "stdafx.h"
#include "DualAddressableSwitchPlus1kMemory.h"
#include "Common.h"
#include <shared/exception/NotSupported.hpp>

namespace owfs { namespace io {

CDualAddressableSwitchPlus1kMemory::CDualAddressableSwitchPlus1kMemory(const boost::filesystem::path& devicePath)
   :m_devicePath(devicePath)
{
}

CDualAddressableSwitchPlus1kMemory::~CDualAddressableSwitchPlus1kMemory()
{
}

unsigned int CDualAddressableSwitchPlus1kMemory::nbChannels() const
{
   boost::filesystem::path readPath = m_devicePath / boost::filesystem::path("channels");
   std::string readValue = CCommon::read(readPath);

   if (readValue.empty())
      return 0;
   return atoi(readValue.c_str());
}

bool CDualAddressableSwitchPlus1kMemory::read(unsigned char unit) const
{
   std::string filename("sensed.");
   filename.append(1, 'A' + unit);

   boost::filesystem::path readPath = m_devicePath / boost::filesystem::path(filename);
   std::string readValue = CCommon::read(readPath);

   // Caution : read value correspond to voltage level, inverted from the transistor state
   // We have to invert the read value
   return readValue != "1";
}

void CDualAddressableSwitchPlus1kMemory::write(unsigned char unit, bool state) const
{
   std::string filename("PIO.");
   filename.append(1, 'A' + unit);

   boost::filesystem::path writePath = m_devicePath / boost::filesystem::path(filename);
   CCommon::write(writePath, state ? "yes" : "no");
}

} } // namespace owfs::io