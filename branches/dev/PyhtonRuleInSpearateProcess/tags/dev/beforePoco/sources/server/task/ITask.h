#pragma once

namespace task {

   //------------------------------
   ///\brief Interface for tasks
   //------------------------------
   class ITask
   {
   public:

      enum EStatus
      {
         kStarted,
         kSuccess,
         kFail
      };

      //---------------------------------
      ///\brief Define a function prototype for updating a task progress
      //---------------------------------
      typedef boost::function2<void, boost::optional< float > , std::string > TaskProgressFunc;

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
