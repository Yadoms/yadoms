#include "stdafx.h"
#include "Common.h"
#include "../../OneWireException.hpp"

namespace owfs { namespace io {

std::string CCommon::read(const boost::filesystem::path& valuePath)
{
   std::ifstream file(valuePath.string());
   if (!file.good())
      throw COneWireException("1-wire read : Error opening " + valuePath.string());

   std::string sLine;
   getline(file, sLine);
   return sLine;
}

void CCommon::write(const boost::filesystem::path& valuePath, const std::string& value)
{
   std::ofstream file(valuePath.string());
   if (!file.good())
      throw COneWireException("1-wire write : Error opening " + valuePath.string());

   file << value;
}

} } // namespace owfs::io
