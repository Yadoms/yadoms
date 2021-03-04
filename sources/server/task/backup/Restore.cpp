#include "stdafx.h"
#include "task/ITask.h"
#include "Restore.h"

#include <boost/process/environment.hpp>

#include <Poco/Zip/Compress.h>
#include <Poco/Zip/ZipException.h>
#include <Poco/Delegate.h>
#include <Poco/Process.h>
#include <Poco/Zip/Decompress.h>

#include <utility>

#include "i18n/ClientStrings.h"
#include "shared/compression/Extract.h"
#include "shared/Log.h"
#include "shared/process/SoftwareStop.h"
#include "shared/tools/Filesystem.h"
#include "tools/OperatingSystem.h"

namespace task
{
   namespace backup
   {
      const std::string CRestore::TaskName("backup");

      CRestore::CRestore(std::string backupFileName,
                         boost::shared_ptr<const IPathProvider> pathProvider)
         : m_backupFileName(std::move(backupFileName)),
           m_pathProvider(std::move(pathProvider))
      {
      }

      const std::string& CRestore::getName() const
      {
         return TaskName;
      }

      void CRestore::doWork(TaskProgressFunc pFunctor)
      {
         boost::filesystem::path tempDirectory;

         try
         {
            const auto backupFile = m_pathProvider->backupPath() / m_backupFileName;
            if (!exists(backupFile))
               throw std::runtime_error("file do not exists");

            // Create/clean temp directory
            notifyProgress(pFunctor, 0, i18n::CClientStrings::RestorePrepare);
            tempDirectory = shared::tools::CFilesystem::createTemporaryFolder();

            // Unzip in temp directory
            notifyProgress(pFunctor, 5, i18n::CClientStrings::RestoreDecompress);
            shared::compression::CExtract extract;
            auto extractedPackageLocation = extract.to(backupFile,
                                                       tempDirectory);

            // Start restore script
            notifyProgress(pFunctor, 95, i18n::CClientStrings::RestoreSuccess);
            runRestoreScript(tempDirectory);

            // Exit Yadoms
            notifyProgress(pFunctor, 100, i18n::CClientStrings::RestoreRestarting);
            restartYadoms();
         }
         catch (const std::exception& e)
         {
            YADOMS_LOG(error) << "Error restoring backup, " << e.what();
            shared::tools::CFilesystem::remove(tempDirectory);
            throw;
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Unknown error restoring backup";
            shared::tools::CFilesystem::remove(tempDirectory);
            throw;
         }
      }

      void CRestore::notifyProgress(const TaskProgressFunc& pFunctor,
                                    int progressPercentage,
                                    const std::string& message) const
      {
         pFunctor(true, progressPercentage / 100.0, message, std::string(), shared::CDataContainer::make());
      }

      void CRestore::runRestoreScript(const boost::filesystem::path& tempDirectory) const
      {
         static const auto ExecutablePath(BACKUP_RESTORATION_SCRIPT_FILENAME);

         Poco::Process::Args args;
         args.push_back(std::to_string(boost::this_process::get_id()));
         args.push_back(tempDirectory.string());

         YADOMS_LOG(debug) << "Launch script \"" << ExecutablePath << "\" with args " << boost::algorithm::join(args, ", ");
         const auto handle = tools::COperatingSystem::launchNativeScript(ExecutablePath, args);

         // As script is waiting Yadoms stops, wait for 5 seconds to ensure it doesn't stop prematurely
         boost::this_thread::sleep(boost::posix_time::seconds(5));

         if (!Poco::Process::isRunning(handle))
            throw std::runtime_error("The update command script terminated prematurely");
      }

      void CRestore::restartYadoms() const
      {
         YADOMS_LOG(information) << "Restarting Yadoms...";

         // Wait 1 sec to ensure clients receive last notification
         boost::this_thread::sleep(boost::posix_time::seconds(1));

         // Ask to close application (asynchronously as Yadoms will want to free all resources now and CSoftwareStop::stop blocks)
         boost::thread asyncStop(&shared::process::CSoftwareStop::stop);
      }
   } //namespace backup
} //namespace task
