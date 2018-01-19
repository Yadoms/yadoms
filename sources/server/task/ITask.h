#pragma once

#include <shared/DataContainer.h>

namespace task
{
   //------------------------------
   ///\brief Interface for tasks
   //------------------------------
   class ITask
   {
   public:
      //---------------------------------
      ///\brief Define a function prototype for updating a task progress
      //
      // The function:
      //    -> return : void
      //    -> param 1 : bool                      : tells if the task if still active (true: running/success, false : error)
      //    -> param 2 : float                     : the task progression (as percent)
      //    -> param 3 : std::string               : the task step message (should be i18n messages)
      //    -> param 4 : std::string               : the exception message (should not be i18n messages; only when param1 is false)
      //    -> param 5 : shared::CDataContainer    : Some free data provided by task implementation (specific for each task)
      //---------------------------------
      typedef boost::function5<void, bool, boost::optional<float>, std::string, std::string, shared::CDataContainer> TaskProgressFunc;

      //------------------------------
      ///\brief Get the task name
      ///\return the task name
      //------------------------------
      virtual const std::string& getName() const = 0;

      //------------------------------
      ///\brief Method called when task instance is ready to start
      ///\param [in] taskId : The taskid
      //------------------------------
      virtual void onSetTaskId(const std::string& taskId)
      {
      }

      //------------------------------
      ///\brief The task core working method
      ///\param [in] pFunctor : a fucntion pointer to call on task update
      //------------------------------
      virtual void doWork(TaskProgressFunc pFunctor) = 0;

      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~ITask()
      {
      }
   };
} //namespace task


