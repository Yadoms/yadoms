#pragma once
#include "IPathProvider.h"
#include "database/IDataBackup.h"
#include "task/exportData/IExportDataHandler.h"

namespace task
{
   namespace backup
   {
      //------------------------------------------
      ///\brief   Backup task (db, scripts, data)
      //-----------------------------------------
      class CExportBackupHandler final : public exportData::IExportDataHandler
      {
      public:
         explicit CExportBackupHandler(boost::shared_ptr<const IPathProvider> pathProvider,
                                       boost::shared_ptr<database::IDataBackup> dataBackupInterface);

         ~CExportBackupHandler() override = default;

         // IExportDataHandler implementation
         std::string taskName() override;
         int maxTries() override;
         bool checkEnoughSpace(const boost::filesystem::path& tempFolder) override;
         void collectDataTo(const boost::filesystem::path& tempFolder,
                            std::function<void(int)> onProgressionPercentFct) override;
         // [END] IExportDataHandler implementation

      private:
         boost::shared_ptr<const IPathProvider> m_pathProvider;
         boost::shared_ptr<database::IDataBackup> m_dataBackupInterface;
      };
   } //namespace backup
} //namespace task
