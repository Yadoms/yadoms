#pragma once

namespace task {

   //------------------------------
   ///\brief Interface for tasks
   //------------------------------
   class ITask
   {
   public:
      //---------------------------------
      ///\brief Define a function prototype for updating a task progress
      //---------------------------------
      typedef boost::function1<void, float> TaskProgressFunc;

      //------------------------------
      ///\brief Get the task name
      ///\return the task name
      //------------------------------
      virtual const std::string & getName() = 0;
   
      //------------------------------
      ///\brief The task core working method
      ///\param [in] pFunctor : a fucntion pointer to call on task update
      //------------------------------
      virtual void doWork(TaskProgressFunc pFunctor) = 0;
   
      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~ITask() {}
   };

} //namespace task
