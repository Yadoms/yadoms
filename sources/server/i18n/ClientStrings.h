#pragma once

namespace i18n
{
   //-----------------------------------------------------------------------------
   /// \class   Class which contains i18n strings that a client can use
   //----------------------------------------------------------------------------   
   class CClientStrings
   {
   public:
      static std::string ScanForUpdates;
      static std::string ScanForUpdatesFailed;
      static std::string ScanForUpdatesSuccess;

      static std::string UpdatePluginInstall;
      static std::string UpdatePluginUpdate;
      static std::string UpdatePluginRemove;
      static std::string UpdatePluginRemoveFailed;
      static std::string UpdatePluginDownload;
      static std::string UpdatePluginDownloadFailed;
      static std::string UpdatePluginDeploy;
      static std::string UpdatePluginDeployFailed;
      static std::string UpdatePluginFinalize;
      static std::string UpdatePluginSuccess;

      static std::string UpdateWidgetInstall;
      static std::string UpdateWidgetUpdate;
      static std::string UpdateWidgetRemove;
      static std::string UpdateWidgetRemoveFailed;
      static std::string UpdateWidgetDownload;
      static std::string UpdateWidgetDownloadFailed;
      static std::string UpdateWidgetDeploy;
      static std::string UpdateWidgetDeployFailed;
      static std::string UpdateWidgetSuccess;

      static std::string UpdateScriptInterpreterInstall;
      static std::string UpdateScriptInterpreterUpdate;
      static std::string UpdateScriptInterpreterRemove;
      static std::string UpdateScriptInterpreterRemoveFailed;
      static std::string UpdateScriptInterpreterDownload;
      static std::string UpdateScriptInterpreterDownloadFailed;
      static std::string UpdateScriptInterpreterDeploy;
      static std::string UpdateScriptInterpreterDeployFailed;
      static std::string UpdateScriptInterpreterSuccess;
      static std::string UpdateScriptInterpreterFinalize;

      static std::string UpdateYadomsUpdate;
      static std::string UpdateYadomsUpdateFailed;
      static std::string UpdateYadomsUpdateAvailable;
      static std::string UpdateYadomsUpToDate;
      static std::string UpdateYadomsDownload;
      static std::string UpdateYadomsDownloadFailed;
      static std::string UpdateYadomsExtract;
      static std::string UpdateYadomsExtractFailed;
      static std::string UpdateYadomsDeploy;
      static std::string UpdateYadomsDeployFailed;
      static std::string UpdateYadomsExit;

      static std::string DatabaseBackupInProgress;
      static std::string DatabaseBackupSuccess;
      static std::string DatabaseBackupFail;
      
      static std::string BackupPrepare;
      static std::string BackupCopyFile;
      static std::string BackupCompress;
      static std::string BackupClean;
      static std::string BackupSuccess;

      static std::string PackLogsPrepare;
      static std::string PackLogsCopyFile;
      static std::string PackLogsCompress;
      static std::string PackLogsClean;
      static std::string PackLogsSuccess;

      static std::string ExportDataPrepare;
      static std::string ExportDataCreateFile;
      static std::string ExportDataCompress;
      static std::string ExportDataClean;
      static std::string ExportDataSuccess;

      static std::string ExtraQuerySuccess;
      static std::string ExtraQueryFail;

      static std::string TaskInterrupted;
      static std::string TaskCrashed;
      static std::string TaskUnkownError;
      static std::string TaskEnd;
      static std::string TaskStarted;
   };
   
} // namespace i18n
