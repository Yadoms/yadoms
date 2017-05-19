#include "stdafx.h"
#include "SingleSwitch.h"
#include "Common.h"

namespace owfs
{
   namespace io
   {
      CSingleSwitch::CSingleSwitch(const boost::filesystem::path& devicePath)
         :m_readDevicePath(devicePath / boost::filesystem::path("sensed")),
         m_writeDevicePath(devicePath / boost::filesystem::path("PIO"))
      {
      }

      CSingleSwitch::~CSingleSwitch()
      {
      }

      bool CSingleSwitch::read() const
      {
         std::string readValue = CCommon::read(m_readDevicePath);

         // Caution : read value correspond to voltage level, inverted from the transistor state
         // We have to invert the read value
         return readValue != "1";
      }

      void CSingleSwitch::write(bool state) const
      {
         CCommon::write(m_writeDevicePath, state ? "1" : "0");
      }
   }
} // namespace owfs::io
