#include "stdafx.h"
#include "Widget.h"
#include "WorkerTools.h"
#include <Poco/File.h>
#include "i18n/ClientStrings.h"
#include <shared/Log.h>

namespace update {
   namespace worker {

      void CWidget::install(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Installing new widget from " << downloadUrl;

         shared::CDataContainer callbackData;
         callbackData.set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetInstall, shared::CStringExtension::EmptyString, callbackData);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading widget package";

            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetDownload, shared::CStringExtension::EmptyString, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdateWidgetDownload, 0.0, 90.0);
            YADOMS_LOG(information) << "Downloading widget package with sucess";


            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy widget package " << downloadedPackage.toString();
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateWidgetDeploy, shared::CStringExtension::EmptyString, callbackData);
               Poco::Path widgetPath = CWorkerTools::deployWidgetPackage(downloadedPackage);
               YADOMS_LOG(information) << "Widget deployed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, shared::CStringExtension::EmptyString, shared::CDataContainer::EmptyContainer);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy widget package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDeployFailed, ex.what(), callbackData);
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download pwidget ackage : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDownloadFailed, ex.what(), callbackData);
         }
      }



      void CWidget::update(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & widgetName, const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Updating widget " << widgetName << " from " << downloadUrl;

         shared::CDataContainer callbackData;
         callbackData.set("widgetName", widgetName);
         callbackData.set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetUpdate, shared::CStringExtension::EmptyString, callbackData);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading widget package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetDownload, shared::CStringExtension::EmptyString, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdateWidgetDownload, 0.0, 90.0);
            YADOMS_LOG(information) << "Downloading widget package with sucess";



            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               progressCallback(true, 50.0f, i18n::CClientStrings::UpdatePluginDeploy, shared::CStringExtension::EmptyString, callbackData);
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               YADOMS_LOG(information) << "Plugin deployed with success";



               YADOMS_LOG(information) << "Deploy widget package " << downloadedPackage.toString();
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateWidgetDeploy, shared::CStringExtension::EmptyString, callbackData);
               Poco::Path widgetPath = CWorkerTools::deployWidgetPackage(downloadedPackage);

               YADOMS_LOG(information) << "Widget installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, shared::CStringExtension::EmptyString, callbackData);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy widget package : " << ex.what();
                progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDeployFailed, ex.what(), callbackData);
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download pwidget ackage : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDownloadFailed, ex.what(), callbackData);
         }
      }

      void CWidget::remove(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & widgetName)
      {
         YADOMS_LOG(information) << "Removing widget " << widgetName;

         shared::CDataContainer callbackData;
         callbackData.set("widgetName", widgetName);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetRemove, shared::CStringExtension::EmptyString, callbackData);


         try
         {
            /////////////////////////////////////////////
            //1. remove plugin folder
            /////////////////////////////////////////////
            Poco::Path widgetPath(CWorkerTools::getWidgetBasePath());
            widgetPath.append(widgetName);

            Poco::File toDelete(widgetPath);
            if (toDelete.exists())
               toDelete.remove(true);

            progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, shared::CStringExtension::EmptyString, callbackData);
         }
         catch (std::exception & ex)
         {
            //fail to remove package
            YADOMS_LOG(error) << "Fail to delete widget : " << widgetName << " : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetRemoveFailed, ex.what(), callbackData);
         }
      }


   } // namespace worker
} // namespace update
