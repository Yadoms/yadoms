#include "stdafx.h"
#include "Extract.h"
#include "shared/exception/NotSupported.hpp"

#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/Delegate.h>

namespace shared { namespace compression { 

   CExtract::CExtract()
   {
   }
   
   CExtract::~CExtract()
   {
   }
   
   Poco::Path CExtract::here(Poco::Path & downloadedPackage)
   {
      //verification of the extension
      std::string extension = downloadedPackage.getExtension();
      if ((!boost::iends_with(extension, "zip")) && (!boost::iends_with(extension, "tar.gz")))
         throw exception::CNotSupported("Invalid extension package : " + downloadedPackage.toString() + ". Only zip or tar.gz supported. " + extension);

      //pour l'instant on prend ce qu'il y a dans temp sans faire l'extraction en attendant poco
      Poco::Path extractPath(downloadedPackage.parent());
      extractPath.pushDirectory(downloadedPackage.getBaseName());

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
         //fail to unzip
         //ensure folder is removed
         Poco::File toDelete(extractPath);
         if (toDelete.exists() && extractPath.isDirectory())
            toDelete.remove(true);
         throw shared::exception::CException("Fail to uncompress package");
      }

      //extraction done with success, now delete downloaded file
      Poco::File toDelete(downloadedPackage);
      if (toDelete.exists())
         toDelete.remove();

      return extractPath;      
   }

   void CExtract::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
   {
      m_unzipError = true;
      m_unzipErrorMessage = info.first.getFileName() + " : " + info.second;
   }

   
} // namespace compression
} // namespace shared
