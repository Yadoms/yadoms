#pragma once
#include "FrameBase.h"
#include "server/task/InstanceNotificationData.h"

namespace web
{
   namespace ws
   {
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
         explicit CTaskUpdateNotificationFrame(boost::shared_ptr<task::CInstanceNotificationData> taskProgression);

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
         ///\brief The task exception
         //-----------------------------
         static const std::string m_taskException;

         //-----------------------------
         ///\brief The task custom data
         //-----------------------------
         static const std::string m_taskData;
      };
   } //namespace ws
} //namespace web


