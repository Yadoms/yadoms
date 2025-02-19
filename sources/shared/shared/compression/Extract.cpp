#include "stdafx.h"
#include "Extract.h"
#include <shared/exception/NotSupported.hpp>
#include <shared/exception/Extract.hpp>

#include <Poco/FileStream.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/Delegate.h>

namespace shared
{
   namespace compression
   {
      CExtract::CExtract()
         : m_unzipError(false)
      {
      }

      boost::filesystem::path CExtract::here(const boost::filesystem::path& compressedFile)
      {
         return to(compressedFile,
                   compressedFile.parent_path() / compressedFile.stem());
      }

      boost::filesystem::path CExtract::to(const boost::filesystem::path& compressedFile,
                                           const boost::filesystem::path& extractPath)
      {
         //verification of the extension
         const auto extension = compressedFile.extension();
         if ((!boost::iends_with(extension.string(), "zip")) && (!boost::iends_with(extension.string(), "tar.gz")))
            throw exception::CNotSupported(
               "Invalid extension package : " + compressedFile.string() + ". Only zip or tar.gz supported. " +
               extension.string());

         Poco::FileStream inp(compressedFile.string(), std::ios::binary);

         // decompress to current working dir
         Poco::Zip::Decompress dec(inp, extractPath.string());
         // if an error happens invoke the ZipTest::onDecompressError method
         m_unzipError = false;
         dec.EError += Poco::Delegate<CExtract, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(
            this, &CExtract::onDecompressError);
         dec.decompressAllFiles();
         dec.EError -= Poco::Delegate<CExtract, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(
            this, &CExtract::onDecompressError);
         inp.close();
         if (m_unzipError)
         {
            throw exception::CExtract("Fail to unzip package : " + m_unzipErrorMessage);
         }

         return extractPath;
      }

      void CExtract::onDecompressError(const void* pSender,
                                       std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
      {
         m_unzipError = true;
         m_unzipErrorMessage = info.first.getFileName() + " : " + info.second;
      }
   } // namespace compression
} // namespace shared
