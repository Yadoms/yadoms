#include "stdafx.h"
#include "ExportLogsHandler.h"
#include <utility>
#include <Poco/Delegate.h>
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/ZipException.h>
#include <shared/Log.h>
#include "shared/tools/Filesystem.h"
#include "task/ITask.h"

namespace task
{
   namespace exportLogs
   {
      CExportLogsHandler::CExportLogsHandler(boost::shared_ptr<const IPathProvider> pathProvider)
         : m_pathProvider(std::move(pathProvider))
      {
      }

      std::string CExportLogsHandler::taskName()
      {
         return "logs";
      }

      int CExportLogsHandler::maxTries()
      {
         return 3;
      }

      bool CExportLogsHandler::checkEnoughSpace(const boost::filesystem::path& tempFolder)
      {
         uintmax_t neededSpace = 0;

         YADOMS_LOG(information) << "Check space needed...";

         const auto needed = shared::tools::CFilesystem::directorySize(m_pathProvider->logsPath());
         YADOMS_LOG(information) << "  - Logs : " << needed;
         neededSpace += needed;

         YADOMS_LOG(information) << "  Total : " << neededSpace;

         // Apply 5% margin
         return boost::filesystem::space(tempFolder).available > (neededSpace * 105 / 100);
      }

      void CExportLogsHandler::collectDataTo(const boost::filesystem::path& tempFolder,
                                             std::function<void(int)> onProgressionPercentFct)
      {
         if (!shared::tools::CFilesystem::copyDirectoryRecursivelyTo(m_pathProvider->logsPath(),
                                                                     tempFolder / "logs"))
            throw std::runtime_error("Fail to copy logs to temp folder");

         onProgressionPercentFct(100);
      }
   } //namespace exportLogs
} //namespace task
