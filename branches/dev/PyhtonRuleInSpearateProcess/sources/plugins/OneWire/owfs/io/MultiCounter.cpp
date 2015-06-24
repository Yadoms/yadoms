#include "stdafx.h"
#include "MultiCounter.h"
#include "Common.h"
#include <shared/exception/NotSupported.hpp>

namespace owfs { namespace io {

CMultiCounter::CMultiCounter(const boost::filesystem::path& devicePath)
   :m_devicePath(devicePath)
{
}

CMultiCounter::~CMultiCounter()
{
}

int CMultiCounter::read(unsigned char unit) const
{
   std::string readValue;

   // Depending on OWFS version, file can be "counter" or "counters". So try both.
   readValue = read("counter.", unit);
   if (!readValue.empty())
      return boost::lexical_cast<int>(readValue);

   readValue = read("counters.", unit);
   if (!readValue.empty())
      return boost::lexical_cast<int>(readValue);

   return 0;
}

std::string CMultiCounter::read(const std::string& filenamePrefix, unsigned char unit) const
{
   std::string filename(filenamePrefix);
   filename.append(1, 'A' + unit);

   boost::filesystem::path readPath = m_devicePath / boost::filesystem::path(filename);
   return CCommon::read(readPath);
}


} } // namespace owfs::io
