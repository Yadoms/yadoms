#include "stdafx.h"
#include "MicrolanCoupler.h"
#include "Common.h"

namespace owfs
{
   namespace io
   {
      CMicrolanCoupler::CMicrolanCoupler(const boost::filesystem::path& devicePath)
         :m_devicePath(devicePath / boost::filesystem::path("control"))
      {
      }

      CMicrolanCoupler::~CMicrolanCoupler()
      {
      }

      bool CMicrolanCoupler::read() const
      {
         std::string readValue = CCommon::read(m_devicePath);

         // 1 = Unconditionally off (non-conducting)
         // 2 = Unconditionally on (conducting)
         int iValue = atoi(readValue.c_str()) == 2;
         if (iValue != 1 && iValue != 2)
            return false;
         else
            return iValue == 2;
      }

      void CMicrolanCoupler::write(bool state) const
      {
         // 1 = Unconditionally off (non-conducting)
         // 2 = Unconditionally on (conducting)
         CCommon::write(m_devicePath, state ? "2" : "1");
      }
   }
} // namespace owfs::io
