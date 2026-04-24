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
      ///\brief                  Constructor
      ///\param [in] name        The task name (ie: yadoms.update)
      ///\param [in] taskFct     The function to use (task real content)
      //------------------------------      
      CGenericTask(std::string name,
                   std::function<void(TaskProgressFunc)> taskFct);

      ~CGenericTask() override = default;

      // ITask implementation
      std::string getName() override;
      void doWork(TaskProgressFunc reportProgressFct) override;
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
      std::function<void(TaskProgressFunc)> m_taskFct;
   };
} //namespace task
