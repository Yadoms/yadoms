#pragma once
#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for faulty download
   //--------------------------------------------------------------
   class CDownloadFailed final : public CException
   {
   public:
      explicit CDownloadFailed(const std::string& failToDownload)
         : CException(std::string("Fail to download ")
            + failToDownload)
      {
      }


      CDownloadFailed(const std::string& failToDownload,
                      const std::string& reason)
         : CException(std::string("Fail to download ")
            + failToDownload + " "
            + reason)
      {
      }

      CDownloadFailed(const std::string& failToDownload,
                      const boost::system::error_code& error)
         : CException(std::string("Fail to download ")
            + failToDownload
            + " Message : "
            + error.message()
            + " \nCategory : "
            + error.category().name()
            + "\nValue : "
            + std::to_string(error.value()))
      {
      }

      ~CDownloadFailed() throw() override
      {
      }
   };
}
