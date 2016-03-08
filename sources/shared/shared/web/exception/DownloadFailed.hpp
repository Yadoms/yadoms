#pragma once

#include <shared/exception/Exception.hpp>

namespace shared { namespace web { namespace exception {

   //--------------------------------------------------------------
   /// \class Exception for faulty download
   //--------------------------------------------------------------
   class CDownloadFailed : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit CDownloadFailed(const Poco::URI & failToDownload)
         :shared::exception::CException(std::string("Fail to download " + failToDownload.toString()))
      {
      }      

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit CDownloadFailed(const std::string & failToDownload)
         :shared::exception::CException(std::string("Fail to download " + failToDownload))
      {
      }    

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDownloadFailed(const Poco::URI & failToDownload, const std::string & reason)
         :shared::exception::CException(std::string("Fail to download " + failToDownload.toString() + " " + reason))
      {
      }
      
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDownloadFailed(const Poco::URI & failToDownload, const boost::system::error_code & error)
         :shared::exception::CException(std::string("Fail to download " + failToDownload.toString() + " " + (boost::format("Message : %1% \nCategory : %2%\nValue : %3%") % error.message() % error.category().name() % error.value()).str()))
      {
      }
      
      
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDownloadFailed(const std::string & failToDownload, const std::string & reason)
         :shared::exception::CException(std::string("Fail to download " + failToDownload + " " + reason))
      {
      }    

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDownloadFailed(const std::string & failToDownload, const boost::system::error_code & error)
         :shared::exception::CException(std::string("Fail to download " + failToDownload + " " + (boost::format("Message : %1% \nCategory : %2%\nValue : %3%") % error.message() % error.category().name() % error.value()).str()))
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CDownloadFailed() throw()
      {
      }
   };

} } } // namespace shared::web::exception