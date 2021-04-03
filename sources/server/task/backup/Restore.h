#pragma once
#include "task/ITask.h"
#include "IPathProvider.h"
#include <Poco/Zip/ZipLocalFileHeader.h>

namespace task
{
   namespace backup
   {
      //------------------------------------------
      ///\brief   Restore backup task
      //-----------------------------------------
      class CRestore : public ITask
      {
      public:
         explicit CRestore(std::string backupFileName,
                           boost::shared_ptr<const IPathProvider> pathProvider);
         virtual ~CRestore() = default;

         // ITask implementation
         const std::string& getName() const override;
         void doWork(TaskProgressFunc pFunctor) override;
         // [END] ITask implementation

      private:
         void notifyProgress(const TaskProgressFunc& pFunctor,
                             int progressPercentage,
                             const std::string& message) const;
         void runRestoreScript(const boost::filesystem::path& tempDirectory) const;
         void restartYadoms() const;

         const std::string m_backupFileName;
         boost::shared_ptr<const IPathProvider> m_pathProvider;
         static const std::string TaskName;
      };
   } //namespace backup
} //namespace task
