#include "stdafx.h"
#include "QuadAdConverter.h"
#include "Common.h"
#include <shared/exception/NotSupported.hpp>

namespace owfs
{
   namespace io
   {
      CQuadAdConverter::CQuadAdConverter(const boost::filesystem::path& devicePath)
         :m_devicePath(devicePath)
      {
      }

      CQuadAdConverter::~CQuadAdConverter()
      {
      }

      double CQuadAdConverter::read(unsigned char unit) const
      {
         std::string filename("volt.");
         filename.append(1, 'A' + unit);
   
         boost::filesystem::path readPath = m_devicePath / boost::filesystem::path(filename);
         std::string readValue = CCommon::read(readPath);

         return boost::lexical_cast<double>(readValue);
      }
   }
} // namespace owfs::io