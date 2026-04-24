#pragma once
#include "IPathProvider.h"
#include "task/exportData/IExportDataHandler.h"

namespace task
{
   namespace exportLogs
   {
      //------------------------------------------
      ///\brief   Logs packing handler
      //-----------------------------------------
      class CExportLogsHandler final : public exportData::IExportDataHandler
      {
      public:
         explicit CExportLogsHandler(boost::shared_ptr<const IPathProvider> pathProvider);
         ~CExportLogsHandler() override = default;

         // IExportDataHandler implementation
         std::string taskName() override;
         int maxTries() override;
         bool checkEnoughSpace(const boost::filesystem::path& tempFolder) override;
         void collectDataTo(const boost::filesystem::path& tempFolder,
                            std::function<void(int)> onProgressionPercentFct) override;
         // [END] IExportDataHandler implementation

      private:
         boost::shared_ptr<const IPathProvider> m_pathProvider;
      };
   } //namespace exportLogs
} //namespace task
