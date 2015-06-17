#include "stdafx.h"
#include "Common.h"
#include <fstream>

namespace owfs { namespace io {

std::string CCommon::read(const boost::filesystem::path& valuePath)
{
   std::ifstream file(valuePath.string());
   if (!file.good())
      return std::string();//TODO lever des OneWireReadException sur des erreurs de lecture

   std::string sLine;
   getline(file, sLine);
   return sLine;
}

void CCommon::write(const boost::filesystem::path& valuePath, const std::string& value)
{
   std::ofstream file(valuePath.string());
   if (!file.good())
      return;//TODO lever des OneWireWriteException sur des erreur d'écriture

   file << value;
}

} } // namespace owfs::io
