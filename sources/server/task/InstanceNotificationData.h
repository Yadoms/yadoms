#pragma once

#include "TaskStatus.h"
#include "IInstance.h"

namespace task {

   //------------------------------
   ///\brief Class which contains task data
   //------------------------------
   class CInstanceNotificationData
   {
   public:
      //------------------------------
      ///\brief Constructor
      //------------------------------
      explicit CInstanceNotificationData(const IInstance & instance);
      
      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~CInstanceNotificationData();

      std::string getGuid() const;
      boost::optional<float> getProgression() const;
      std::string getMessage() const;
      std::string getExceptionMessage() const;
      ETaskStatus getStatus() const;
      boost::shared_ptr<shared::CDataContainer> getTaskData() const;
      std::string getName() const;
      boost::posix_time::ptime getCreationDate() const;
      
   private:
      //------------------------------------------
      ///\brief   The unique identifier of the task
      //------------------------------------------
      std::string m_guid;

      //--------------------------------------------------------------
      /// \brief			The current task instance progression
      //--------------------------------------------------------------
      boost::optional<float> m_currentProgression;
      
      //--------------------------------------------------------------
      /// \brief			The current message given by the task
      //--------------------------------------------------------------
      std::string m_currentMessage;

      //--------------------------------------------------------------
      /// \brief			The current exception given by the task
      //--------------------------------------------------------------
      std::string m_currentException;      
      
      //------------------------------------------
      ///\brief   The status of the task
      //------------------------------------------
      ETaskStatus m_currentStatus;      
      
      //--------------------------------------------------------------
      /// \brief			The current task custom data
      //--------------------------------------------------------------
      boost::shared_ptr<shared::CDataContainer> m_taskData;      
      
      //--------------------------------------------------------------
      /// \brief			The current exception given by the task
      //--------------------------------------------------------------
      std::string m_name;

      //------------------------------------------
      ///\brief   The creation date
      //------------------------------------------
      boost::posix_time::ptime m_creationDate;
   };

} //namespace task
