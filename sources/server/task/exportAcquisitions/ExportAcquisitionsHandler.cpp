#include "stdafx.h"
#include "ExportAcquisitionsHandler.h"
#include <utility>
#include <Poco/Zip/Compress.h>
#include "database/IAcquisitionRequester.h"

namespace task
{
   namespace exportAcquisitions
   {
      CExportAcquisitionsHandler::CExportAcquisitionsHandler(boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                                                             boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                                                             int keywordId)
         : m_keywordRequester(std::move(keywordRequester)),
           m_acquisitionRequester(std::move(acquisitionRequester)),
           m_keywordId(keywordId)
      {
      }

      std::string CExportAcquisitionsHandler::taskName()
      {
         return "acquisitions";
      }

      int CExportAcquisitionsHandler::maxTries()
      {
         return 1;
      }

      bool CExportAcquisitionsHandler::checkEnoughSpace(const boost::filesystem::path& tempFolder)
      {
         return true;
      }

      void CExportAcquisitionsHandler::collectDataTo(const boost::filesystem::path& tempFolder,
                                                     std::function<void(int)> onProgressionPercentFct)
      {
         std::ofstream outfile((tempFolder / "exportAcquisitions.csv").string());

         // Header line
         outfile << "keyword,date,value";

         // Data lines
         const auto keywords = m_keywordRequester->getKeyword(m_keywordId);
         if (keywords->HistoryDepth() == shared::plugin::yPluginApi::EHistoryDepth::kNoHistory)
         {
            outfile << std::endl << m_keywordId << "," << keywords->LastAcquisitionDate() << "," << keywords->LastAcquisitionValue();
            onProgressionPercentFct(100);
            return;
         }

         auto nbLinesDone = 0;
         m_acquisitionRequester->exportAcquisitions(
            m_keywordId,
            [this, &outfile, &nbLinesDone, &onProgressionPercentFct](const boost::posix_time::ptime& date,
                                                                     const std::string& value,
                                                                     const int nbTotalLines)
            {
               outfile << std::endl << m_keywordId << "," << date << "," << value;
               ++nbLinesDone;
               onProgressionPercentFct(nbLinesDone * 100 / nbTotalLines);
            });
      }
   } //namespace exportAcquisitions
} //namespace task
