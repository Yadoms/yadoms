#pragma once
#include <Poco/Notification.h>
#include <shared/Log.h>
#include "../database/entities/Entities.h"
#include "task/IInstance.h"

namespace notifications {

   //-----------------------------
   ///\brief Notification for a task progression update
   //-----------------------------
   class CTaskProgressionNotification : public Poco::Notification
   {
   public:

      //-----------------------------
      ///\brief Constructor
      ///\param [in]  instance   The task instance
      CTaskProgressionNotification(boost::shared_ptr<task::IInstance> instance)
         :m_taskInstance(instance)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CTaskProgressionNotification()
      {
      }

      //-----------------------------
      ///\brief Get the task name
      ///\return  the task name
      //-----------------------------
      boost::shared_ptr<task::IInstance> getTaskInstance() const { return m_taskInstance; }

   private:
      //-----------------------------
      ///\brief The task name
      //-----------------------------
      boost::shared_ptr<task::IInstance>  m_taskInstance;
   };

} //namespace notifications 
