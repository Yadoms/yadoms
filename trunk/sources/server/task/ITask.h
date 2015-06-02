#pragma once

#include <shared/DataContainer.h>

namespace task {

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
      //    -> param 1 : bool                      : tells if the task if still active (true: running/sucess, false : stopped/error)
      //    -> param 2 : float                     : the task progression (as percent)
      //    -> param 3 : std::string               : the task step message (should be i18n messages)
      //    -> param 4 : dashared::CDataContainerta: Some free data provided by task implementation (specific for each task)
      //---------------------------------
      typedef boost::function4<void, bool, boost::optional<float>, std::string, shared::CDataContainer > TaskProgressFunc;

      //------------------------------
      ///\brief Get the task name
      ///\return the task name
      //------------------------------
      virtual const std::string & getName() = 0;
   
      //------------------------------
      ///\brief The task core working method
      ///\param [in] pFunctor : a fucntion pointer to call on task update
      ///\return : true if the task has been succeeded, false otherwise
      //------------------------------
      virtual bool doWork(TaskProgressFunc pFunctor) = 0;

      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~ITask() {}
   };

} //namespace task
