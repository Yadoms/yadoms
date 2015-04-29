#pragma once

#include <shared/Export.h>
#include <Poco/Path.h>
#include <Poco/Zip/ZipLocalFileHeader.h>

namespace shared { namespace compression { 

   //--------------------------------------------------------------
   /// \brief	Archive extraction
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CExtract
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------   
      CExtract();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------   
      virtual ~CExtract();
      
      //--------------------------------------------------------------
      /// \brief	Extract an archive next to its location
      //--------------------------------------------------------------   
      Poco::Path here(Poco::Path & downloadedPackage);
      
   private:
      //------------------------------------------
      ///\brief   A method for receivving unzip errors
      //------------------------------------------
      void onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);

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