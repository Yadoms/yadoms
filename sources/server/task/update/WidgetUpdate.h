#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Widget update task. The aim si to update an existing widget
   //-----------------------------------------
   class CWidgetUpdate : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] widgetName     The widget name to update
      ///\param [in] downloadUrl    The download url
      //------------------------------------------
      CWidgetUpdate(const std::string & widgetName, const std::string & downloadUrl);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CWidgetUpdate();

   public:
      // ITask implementation
      virtual const std::string & getName();
      virtual void doWork(TaskProgressFunc pFunctor);
      // ITask implementation

   private:
      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;

      //------------------------------------------
      ///\brief   The widget name
      //------------------------------------------
      std::string m_widgetName;

      //------------------------------------------
      ///\brief   The download url
      //------------------------------------------
      std::string m_downloadUrl;
   };


} //namespace update
} //namespace task
