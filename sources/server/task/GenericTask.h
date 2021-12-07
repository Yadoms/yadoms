#pragma once
#include "ITask.h"

namespace task
{
   //------------------------------
   ///\brief Class which handle generic task
   //------------------------------
   class CGenericTask final : public ITask
   {
   public:
      //------------------------------
      ///\brief The function pointer for task worker
      //------------------------------
      typedef std::function<void (TaskProgressFunc)> TaskFunc;

      //------------------------------
      ///\brief                  Constructor
      ///\param [in] name        The task name (ie: yadoms.update)
      ///\param [in] pFunctor    The function pointer to use (task real content)
      //------------------------------      
      CGenericTask(std::string name,
                   TaskFunc pFunctor);

      ~CGenericTask() override = default;

      // ITask implementation
      const std::string& getName() const override;
      void doWork(TaskProgressFunc pFunctor) override;
      void onSetTaskId(const std::string& taskId) override;
      bool isCancellable() const override;
      // ITask implementation

   private:
      //------------------------------
      ///\brief The task name (ie: yadoms.update)
      //------------------------------      
      std::string m_taskName;

      //------------------------------
      ///\brief The function pointer to use (task real content)
      //------------------------------         
      TaskFunc m_pFunctor;
   };
} //namespace task
