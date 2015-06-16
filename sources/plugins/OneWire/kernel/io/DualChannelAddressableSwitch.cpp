#include "stdafx.h"
#include "DualChannelAddressableSwitch.h"
#include "Common.h"
#include <shared/Log.h>
#include "../../OneWireException.hpp"

namespace kernel { namespace io {

CDualChannelAddressableSwitch::CDualChannelAddressableSwitch(const boost::filesystem::path& devicePath)
   :m_devicePath(devicePath / boost::filesystem::path("w1_slave"))
{
}

CDualChannelAddressableSwitch::~CDualChannelAddressableSwitch()
{
}

unsigned char CDualChannelAddressableSwitch::readPort() const
{
   static const unsigned char cmdReadPioRegisters[] = { 0xF5 };
   unsigned char answer[1];
   try
   {
      CCommon::sendAndReceive(m_devicePath, cmdReadPioRegisters, sizeof(cmdReadPioRegisters), answer, sizeof(answer));

      // Check received data : datasheet says that b[7-4] is complement to b[3-0]
      if ((answer[0] & 0xF0) != (((~answer[0]) & 0x0F) << 4))
         throw COneWireException("Invalid data when reading from " + m_devicePath.string());

      return answer[0];
   }
   catch (shared::exception::CException& ex)
   {
      throw COneWireException("Error reading from " + m_devicePath.string() + ", " + ex.what());
   }
}

bool CDualChannelAddressableSwitch::read(unsigned char unit) const
{
   // Caution : 0 means 'transistor active', we have to invert
   return (unit == 0) ?
      ((readPort() & 0x01) ? false : true)       // unit == 0
      : ((readPort() & 0x04) ? false : true);    // unit == 1
}

void CDualChannelAddressableSwitch::write(unsigned char unit, bool state) const
{
   // First read actual state, to only change the correct unit
   unsigned char readAnswer = readPort();
   // Caution : 0 means 'transistor active', we have to invert
   unsigned char otherPioState = 0;
   otherPioState |= (readAnswer & 0x01) ? 0x00 : 0x01;
   otherPioState |= (readAnswer & 0x04) ? 0x00 : 0x02;

   unsigned char cmdWritePioRegisters[] = { 0x5A, 0x00, 0x00 };
   // 0 to activate the transistor
   unsigned char newPioState = state ? (otherPioState | (0x01 << unit)) : (otherPioState & ~(0x01 << unit));
   cmdWritePioRegisters[1] = ~newPioState;
   cmdWritePioRegisters[2] = ~cmdWritePioRegisters[1];

   unsigned char answer[2];
   CCommon::sendAndReceive(m_devicePath, cmdWritePioRegisters, sizeof(cmdWritePioRegisters), answer, sizeof(answer));

   // Check for transmission errors
   // Expected first byte = 0xAA
   // Expected second Byte = b[7-4] is complement to b[3-0]
   if (answer[0] != 0xAA || (answer[1] & 0xF0) != (((~answer[1]) & 0x0F) << 4))
      throw COneWireException("Error writing to " + m_devicePath.string());
}

} } // namespace kernel::io