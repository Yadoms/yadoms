#pragma once
#include "task/ITask.h"
#include "IPathProvider.h"

namespace task
{
   namespace backup
   {
      //------------------------------------------
      ///\brief   Restore backup task
      //-----------------------------------------
      class CUploadFile : public ITask //TODO utile ?
      {
      public:
         explicit CUploadFile(std::string backupFileName,
                           boost::shared_ptr<const IPathProvider> pathProvider);
         virtual ~CUploadFile() = default;

         // ITask implementation
         const std::string& getName() const override;
         void doWork(TaskProgressFunc pFunctor) override;
         // [END] ITask implementation

      private:
         void notifyProgress(const TaskProgressFunc& pFunctor,
                             int progressPercentage,
                             const std::string& message) const;

         const std::string m_backupFileName;
         boost::shared_ptr<const IPathProvider> m_pathProvider;
         static const std::string TaskName;
      };
   } //namespace backup
} //namespace task
