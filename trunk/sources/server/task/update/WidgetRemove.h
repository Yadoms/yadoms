#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Widget update task. The aim si to update an existing widget
   //-----------------------------------------
   class CWidgetRemove : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] widgetName     The widget name to remove
      //------------------------------------------
      CWidgetRemove(const std::string & widgetName);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CWidgetRemove();

   public:
      // ITask implementation
      virtual const std::string & getName();
      virtual bool doWork(TaskProgressFunc pFunctor);
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
   };
} //namespace update
} //namespace task
