#include "stdafx.h"
#include "8ChannelAddressableSwitch.h"
#include "Common.h"
#include <shared/Log.h>
#include "../../OneWireException.hpp"//TODO faire ce qu'il faut dans le cmake pour virer les ../../

namespace kernel { namespace io {

C8ChannelAddressableSwitch::C8ChannelAddressableSwitch(const boost::filesystem::path& devicePath)
   :m_devicePath(devicePath / boost::filesystem::path("w1_slave"))
{
}

C8ChannelAddressableSwitch::~C8ChannelAddressableSwitch()
{
}

unsigned char C8ChannelAddressableSwitch::readPort() const
{
   static const unsigned char cmdReadPioRegisters[] = { 0xF5 };
   unsigned char answer[33];
   try
   {
      CCommon::sendAndReceive(m_devicePath, cmdReadPioRegisters, sizeof(cmdReadPioRegisters), answer, sizeof(answer));

      // Now check the Crc
      unsigned char crcData[33];
      crcData[0] = cmdReadPioRegisters[0];
      for (int i = 0; i<32; i++)
         crcData[i + 1] = answer[i];
      if (CCommon::crc16(crcData, sizeof(crcData)) != answer[32])
         throw COneWireException("Invalid CRC when reading from " + m_devicePath.string());

      return answer[0];
   }
   catch (shared::exception::CException& ex)
   {
      throw COneWireException("Error reading from " + m_devicePath.string() + ", " + ex.what());
   }
}

bool C8ChannelAddressableSwitch::read(unsigned char unit) const
{
   // Caution : 0 means 'transistor active', we have to invert
   return (readPort() & (0x01 << unit)) ? false : true;
}

void C8ChannelAddressableSwitch::write(unsigned char unit, bool state) const
{
   // First read actual state, to only change the correct unit
   // Caution : 0 means 'transistor active', we have to invert
   unsigned char otherPioState = ~readPort();

   unsigned char cmdWritePioRegisters[] = { 0x5A, 0x00, 0x00 };
   // 0 to activate the transistor
   unsigned char newPioState = state ? (otherPioState | (0x01 << unit)) : (otherPioState & ~(0x01 << unit));
   cmdWritePioRegisters[1] = ~newPioState;
   cmdWritePioRegisters[2] = ~cmdWritePioRegisters[1];

   unsigned char answer[2];
   CCommon::sendAndReceive(m_devicePath, cmdWritePioRegisters, sizeof(cmdWritePioRegisters), answer, sizeof(answer));

   // Check for transmission errors
   // Expected first byte = 0xAA
   // Expected second Byte = inverted new pio state
   if (answer[0] != 0xAA || answer[1] != (unsigned char)(~newPioState))
      throw COneWireException("Error writing to " + m_devicePath.string());
}

} } // namespace kernel::io