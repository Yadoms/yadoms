#pragma once
#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Exception for faulty download
      //--------------------------------------------------------------
      class CDownloadFailed : public CException
      {
      public:

         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         explicit CDownloadFailed(const std::string& failToDownload)
            : CException(std::string("Fail to download " + failToDownload))
         {
         }


         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         CDownloadFailed(const std::string& failToDownload, const std::string& reason)
            : CException(std::string("Fail to download " + failToDownload + " " + reason))
         {
         }

         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         CDownloadFailed(const std::string& failToDownload, const boost::system::error_code& error)
            : CException(std::string(
               "Fail to download " + failToDownload + " " + (boost::
                  format("Message : %1% \nCategory : %2%\nValue : %3%") % error.message() % error.category().name() %
                  error.value()).str()))
         {
         }

         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CDownloadFailed() throw()
         {
         }
      };
   }
} // namespace shared::exception
