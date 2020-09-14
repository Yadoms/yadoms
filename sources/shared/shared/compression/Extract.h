#pragma once
#include <Poco/Zip/ZipLocalFileHeader.h>

namespace shared
{
   namespace compression
   {
      //--------------------------------------------------------------
      /// \brief	Archive extraction
      //--------------------------------------------------------------
      class CExtract
      {
      public:
         CExtract();
         virtual ~CExtract() = default;

         //--------------------------------------------------------------
         /// \brief	Extract an archive next to its location
         //--------------------------------------------------------------   
         boost::filesystem::path here(const boost::filesystem::path& downloadedPackage);

         //--------------------------------------------------------------
         /// \brief	Extract an archive into a directory
         //--------------------------------------------------------------   
         boost::filesystem::path to(const boost::filesystem::path& downloadedPackage,
                                    const boost::filesystem::path& extractPath);

      private:
         //------------------------------------------
         ///\brief   A method for receiving unzip errors
         //------------------------------------------
         void onDecompressError(const void* pSender,
                                std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);

         //------------------------------------------
         ///\brief   Indicate if an error occured during package extraction
         //------------------------------------------
         bool m_unzipError;

         //------------------------------------------
         ///\brief   Indicate the error message if ab error occured during package extraction
         //------------------------------------------
         std::string m_unzipErrorMessage;
      };
   } // namespace compression
} // namespace shared
