#pragma once
#include "ITask.h"

namespace task
{
   //------------------------------
   ///\brief Class which handle generic task
   //------------------------------
   class CGenericTask : public ITask
   {
   public:

      //------------------------------
      ///\brief The function pointer for task worker
      //------------------------------
      typedef boost::function1<void, TaskProgressFunc> TaskFunc;

      //------------------------------
      ///\brief                  Constructor
      ///\param [in] name        The task name (ie: yadoms.update)
      ///\param [in] pFunctor    The function pointer to use (task real content)
      //------------------------------      
      CGenericTask(const std::string& name, TaskFunc pFunctor);

      //------------------------------
      ///\brief Destructor
      //------------------------------        
      virtual ~CGenericTask();

      // ITask implementation
      const std::string& getName() const override;
      void doWork(TaskProgressFunc pFunctor) override;
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


