#pragma once
#include "ITask.h"
#include "TaskStatus.h"

namespace task {

   //------------------------------
   ///\brief Class which handle one task
   //------------------------------
   class IInstance
   {
   public:
      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~IInstance(){}

      //---------------------------------
      ///\brief Get the unique identifier of the instance
      //---------------------------------
      virtual std::string getGuid() const = 0;

      //---------------------------------
      ///\brief Get the current progression
      //---------------------------------
      virtual boost::optional<float> getProgression() const = 0;

      //---------------------------------
      ///\brief Get the current message
      //---------------------------------
      virtual std::string getMessage() const = 0;   
      
      //---------------------------------
      ///\brief Get the current exception message
      //---------------------------------
      virtual std::string getExceptionMessage() const = 0;

      //------------------------------------------
      ///\brief   Obtain Status
      //------------------------------------------
      virtual ETaskStatus getStatus() const = 0;

      //---------------------------------
      ///\brief Get the current name of the task
      //---------------------------------
      virtual std::string getName() const = 0;

      //-------------------------------------
      /// \brief	Obtain the task custom data
      //-------------------------------------
      virtual boost::shared_ptr<shared::CDataContainer> getTaskData() const = 0;

      //---------------------------------
      ///\brief Get the creation date of the task
      //---------------------------------
      virtual boost::posix_time::ptime getCreationDate() const = 0;
   };

} //namespace task
