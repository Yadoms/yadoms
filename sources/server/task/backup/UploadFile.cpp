#include "stdafx.h"
#include "task/ITask.h"
#include "UploadFile.h"

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

namespace task
{
   namespace backup
   {
      const std::string CUploadFile::TaskName("uploadFile");

      CUploadFile::CUploadFile(std::string backupFileName,
                         boost::shared_ptr<const IPathProvider> pathProvider)
         : m_backupFileName(std::move(backupFileName)),
           m_pathProvider(std::move(pathProvider))
      {
      }

      const std::string& CUploadFile::getName() const
      {
         return TaskName;
      }

      void CUploadFile::doWork(TaskProgressFunc pFunctor)
      {
         boost::filesystem::path tempDirectory;

         try
         {
            //TODO
            //const auto backupFile = m_pathProvider->backupPath() / m_backupFileName;
            //if (!exists(backupFile))
            //   throw std::runtime_error("file do not exists");

            //// Create/clean temp directory
            //notifyProgress(pFunctor, 0, i18n::CClientStrings::RestorePrepare);
            //tempDirectory = shared::tools::CFilesystem::createTemporaryFolder();

            //// Unzip in temp directory
            //notifyProgress(pFunctor, 5, i18n::CClientStrings::RestoreDecompress);
            //shared::compression::CExtract extract;
            //auto extractedPackageLocation = extract.to(backupFile,
            //                                           tempDirectory);

            //notifyProgress(pFunctor, 100, i18n::CClientStrings::RestoreRestarting);
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

      void CUploadFile::notifyProgress(const TaskProgressFunc& pFunctor,
                                    int progressPercentage,
                                    const std::string& message) const
      {
         pFunctor(true, progressPercentage / 100.0, message, std::string(), shared::CDataContainer::make());
      }
   } //namespace backup
} //namespace task
