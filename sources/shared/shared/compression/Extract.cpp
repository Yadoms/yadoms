#include "stdafx.h"
#include "Extract.h"
#include <shared/exception/NotSupported.hpp>
#include <shared/exception/Extract.hpp>

#include <Poco/FileStream.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/Delegate.h>

namespace shared { namespace compression { 

   CExtract::CExtract()
   :m_unzipError(false)
   {
   }
   
   CExtract::~CExtract()
   {
   }
   
   Poco::Path CExtract::here(Poco::Path & downloadedPackage)
   {
      //pour l'instant on prend ce qu'il y a dans temp sans faire l'extraction en attendant poco
      Poco::Path extractPath(downloadedPackage.parent());
      extractPath.pushDirectory(downloadedPackage.getBaseName());

      return to(downloadedPackage, extractPath);
   }

   Poco::Path CExtract::to(Poco::Path & downloadedPackage, Poco::Path extractPath)
   {
      //verification of the extension
      std::string extension = downloadedPackage.getExtension();
      if ((!boost::iends_with(extension, "zip")) && (!boost::iends_with(extension, "tar.gz")))
         throw exception::CNotSupported("Invalid extension package : " + downloadedPackage.toString() + ". Only zip or tar.gz supported. " + extension);

      Poco::FileStream inp(downloadedPackage.toString(), std::ios::binary);

      // decompress to current working dir
      Poco::Zip::Decompress dec(inp, extractPath);
      // if an error happens invoke the ZipTest::onDecompressError method
      m_unzipError = false;
      dec.EError += Poco::Delegate<CExtract, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CExtract::onDecompressError);
      dec.decompressAllFiles();
      dec.EError -= Poco::Delegate<CExtract, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CExtract::onDecompressError);
      inp.close();
      if (m_unzipError)
      {
         throw shared::exception::CExtract("Fail to uncompress package : " + m_unzipErrorMessage);
      }

      return extractPath;
   }

   void CExtract::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
   {
      m_unzipError = true;
      m_unzipErrorMessage = info.first.getFileName() + " : " + info.second;
   }

   
} // namespace compression
} // namespace shared
