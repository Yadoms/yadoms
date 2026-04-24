#pragma once
#include "database/IAcquisitionRequester.h"
#include "database/IKeywordRequester.h"
#include "task/exportData/ExportData.h"

namespace task
{
   namespace exportAcquisitions
   {
      //------------------------------------------
      ///\brief   Export acquisitions handler
      //-----------------------------------------
      class CExportAcquisitionsHandler final : public exportData::IExportDataHandler
      {
      public:
         explicit CExportAcquisitionsHandler(boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                                             boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                                             int keywordId);
         ~CExportAcquisitionsHandler() override = default;

         // IExportDataHandler implementation
         std::string taskName() override;
         int maxTries() override;
         bool checkEnoughSpace(const boost::filesystem::path& tempFolder) override;
         void collectDataTo(const boost::filesystem::path& tempFolder,
                            std::function<void(int)> onProgressionPercentFct) override;
         // [END] IExportDataHandler implementation

      private:
         boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;
         boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
         int m_keywordId;
      };
   } //namespace exportAcquisitions
} //namespace task
