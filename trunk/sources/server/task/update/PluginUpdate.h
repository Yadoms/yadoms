#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update an existing plugin
   //-----------------------------------------
   class CPluginUpdate : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] pluginName     The plugin name to update
      ///\param [in] downloadUrl    The download url
      //------------------------------------------
      CPluginUpdate(const std::string & pluginName, const std::string & downloadUrl);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CPluginUpdate();

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
      ///\brief   The plugin name
      //------------------------------------------
      std::string m_pluginName;

      //------------------------------------------
      ///\brief   The download url
      //------------------------------------------
      std::string m_downloadUrl;
   };


} //namespace update
} //namespace task
