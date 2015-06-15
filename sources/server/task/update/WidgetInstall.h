#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Widget install task. The aim si to install a new widget
   //-----------------------------------------
   class CWidgetInstall : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] downloadUrl    The download url
      //------------------------------------------
      CWidgetInstall(const std::string & downloadUrl);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CWidgetInstall();

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
      ///\brief   The download url
      //------------------------------------------
      std::string m_downloadUrl;
   };

} //namespace update
} //namespace task
