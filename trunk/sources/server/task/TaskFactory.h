#pragma once

#include "ITask.h"

namespace task { 

   //------------------------------------------
   ///\brief   Factory used to create Task
   //-----------------------------------------
   class CTaskFactory
   {
   public:
      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CTaskFactory();

      //------------------------------------------
      ///\brief   Create a task using the type provided
      ///\param [in] type : The type of task to create
      /// \return    	A Shared ptr to the task created
      //------------------------------------------
      static boost::shared_ptr<ITask> createTask(const std::string & type);

	private:
	  //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CTaskFactory();
   };

} //namespace task
