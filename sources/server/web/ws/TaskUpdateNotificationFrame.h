#pragma once

#include "FrameBase.h"
#include "database/entities/Entities.h"
#include "notifications/TaskProgressionNotification.h"


namespace web { namespace ws {

   //-----------------------------
   ///\brief Frame which can be send to GUI to notify for task update
   //-----------------------------
   class CTaskUpdateNotificationFrame : public CFrameBase
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in]    content     The acquisition to send to GUI (shared_ptr)
      //-----------------------------
      CTaskUpdateNotificationFrame(boost::shared_ptr<notifications::CTaskProgressionNotification> notificationData);

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CTaskUpdateNotificationFrame();

   protected:
      //-----------------------------
      ///\brief The task name
      //-----------------------------
      static const std::string m_taskName;
      
      //-----------------------------
      ///\brief The task guid
      //-----------------------------
      static const std::string m_taskGuid;

      //-----------------------------
      ///\brief The task state
      //-----------------------------
      static const std::string m_taskState;

      //-----------------------------
      ///\brief The task progression
      //-----------------------------
      static const std::string m_taskProgression;
      
      //-----------------------------
      ///\brief The task message
      //-----------------------------
      static const std::string m_taskMessage;

   };

} //namespace ws
} //namespace web