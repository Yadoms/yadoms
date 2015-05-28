#pragma once
#include "task/Scheduler.h"
#include <shared/DataContainer.h>

namespace update
{
   //-----------------------------------------------------------------------------
   /// \brief  Provide methods for starting update and check for updates
   //-----------------------------------------------------------------------------   
   class CUpdateManager
   {
   public:
      //-----------------------------------------------------------------------------
      /// \brief  Constructor
      /// \param [in]   taskScheduler        The task scheduler
      //-----------------------------------------------------------------------------   
      CUpdateManager(boost::shared_ptr<task::CScheduler> & taskScheduler);

      //-----------------------------------------------------------------------------
      /// \brief  Destructor
      //-----------------------------------------------------------------------------   
      virtual ~CUpdateManager();


      //-----------------------------------------------------------------------------
      /// \brief  Update a plugin (async process)
      /// \param [in]   pluginName        The plugin name to update
      /// \param [in]   downloadUrl       The plugin package download url
      //-----------------------------------------------------------------------------   
      void updatePluginAsync(const std::string & pluginName, const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Install a plugin (async process)
      /// \param [in]   downloadUrl       The plugin package download url
      //-----------------------------------------------------------------------------   
      void installPluginAsync(const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Remove a plugin (async process)
      /// \param [in]   pluginName       The plugin name to remove
      //-----------------------------------------------------------------------------   
      void removePluginAsync(const std::string & pluginName);

      //-----------------------------------------------------------------------------
      /// \brief  Update a widget (async process)
      /// \param [in]   widgetName        The widget name to update
      /// \param [in]   downloadUrl       The widget package download url
      //-----------------------------------------------------------------------------   
      void updateWidgetAsync(const std::string & widgetName, const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Install a widget (async process)
      /// \param [in]   downloadUrl       The widget package download url
      //-----------------------------------------------------------------------------   
      void installWidgetAsync(const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Remove a widget (async process)
      /// \param [in]   widgetName       The widget name to remove
      //-----------------------------------------------------------------------------   
      void removeWidgetAsync(const std::string & widgetName);
      
      //-----------------------------------------------------------------------------
      /// \brief  Start a check for update for Yadoms (asynchronous; check for update result is provided by webservice)
      //-----------------------------------------------------------------------------   
      void checkForYadomsUpdateAsync();

      //-----------------------------------------------------------------------------
      /// \brief  Start an update of Yadoms (asynchronous; update result is provided by webservice)
      /// \param [in]   versionToInstall        The yadoms lastVersion.json to install (can be lastVersion.json, or any other one)
      //-----------------------------------------------------------------------------   
      void updateYadomsAsync(const shared::CDataContainer & versionToInstall);

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \return result (true/false)
      //-----------------------------------------------------------------------------   
      bool startTask(boost::shared_ptr<task::ITask> task);

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Task scheduler
      //-----------------------------------------------------------------------------
      boost::shared_ptr<task::CScheduler> m_taskScheduler;
   };
   
} // namespace update
