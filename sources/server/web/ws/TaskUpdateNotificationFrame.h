#pragma once

#include "FrameBase.h"
#include "database/entities/Entities.h"
#include "../../server/task/IInstance.h"

namespace web { namespace ws {

   //-----------------------------
   ///\brief Frame which can be send to GUI to notify for task update
   //-----------------------------
   class CTaskUpdateNotificationFrame : public CFrameBase
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in]    taskProgression     The task progression data
      //-----------------------------
      CTaskUpdateNotificationFrame(boost::shared_ptr<const task::IInstance> taskProgression);

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
      
      //-----------------------------
      ///\brief The task custom data
      //-----------------------------
      static const std::string m_taskData;

   };

} //namespace ws
} //namespace web