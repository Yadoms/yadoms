#include "stdafx.h"
#include "MultiSwitch.h"
#include "Common.h"
#include <shared/exception/NotSupported.hpp>

namespace owfs { namespace io {

CMultiSwitch::CMultiSwitch(const boost::filesystem::path& devicePath, EUnitRepresentation unitRepresentation)
   :m_devicePath(devicePath), m_unitRepresentation(unitRepresentation)
{
}

CMultiSwitch::~CMultiSwitch()
{
}

bool CMultiSwitch::read(unsigned char unit) const
{
   std::string filename("sensed.");
   switch (m_unitRepresentation)
   {
   case kUnitIsZeroBasedNumber:
      filename.append(1, '0' + unit);
      break;
   case kUnitIsUppercaseLetter:
      filename.append(1, 'A' + unit);
      break;
   default:
      throw shared::exception::CNotSupported("Not supported unit representation type");
   }

   boost::filesystem::path readPath = m_devicePath / boost::filesystem::path(filename);
   std::string readValue = CCommon::read(readPath);

   // Caution : read value correspond to voltage level, inverted from the transistor state
   // We have to invert the read value
   return readValue != "1";
}

void CMultiSwitch::write(unsigned char unit, bool state) const
{
   std::string filename("PIO.");
   switch (m_unitRepresentation)
   {
   case kUnitIsZeroBasedNumber:
      filename.append(1, '0' + unit);
      break;
   case kUnitIsUppercaseLetter:
      filename.append(1, 'A' + unit);
      break;
   default:
      throw shared::exception::CNotSupported("Not supported unit representation type");
   }

   boost::filesystem::path writePath = m_devicePath / boost::filesystem::path(filename);
   CCommon::write(writePath, state ? "1" : "0");
}


} } // namespace owfs::io
