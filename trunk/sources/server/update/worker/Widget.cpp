#include "stdafx.h"
#include "Widget.h"
#include "WorkerTools.h"
#include <Poco/File.h>
#include "i18n/ClientStrings.h"
#include <shared/Log.h>

namespace update {
   namespace worker {

      CWidget::CWidget(CWorkerTools::WorkerProgressFunc progressCallback)
         :m_progressCallback(progressCallback)
      {

      }

      CWidget::~CWidget()
      {

      }

      void CWidget::install(const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Installing new widget from " << downloadUrl;

         shared::CDataContainer callbackData;
         callbackData.set("downloadUrl", downloadUrl);

         m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetInstall, callbackData);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading widget package";

            m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetDownload, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, m_progressCallback, i18n::CClientStrings::UpdateWidgetDownload, 0.0, 90.0);
            YADOMS_LOG(information) << "Downloading widget package with sucess";


            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy widget package " << downloadedPackage.toString();
               m_progressCallback(true, 90.0f, i18n::CClientStrings::UpdateWidgetDeploy, callbackData);
               Poco::Path widgetPath = CWorkerTools::deployWidgetPackage(downloadedPackage);
               YADOMS_LOG(information) << "Widget deployed with success";
               m_progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, shared::CDataContainer::EmptyContainer);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy widget package : " << ex.what();
               callbackData.set("exception", ex.what());
               m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDeployFailed, callbackData);
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
            callbackData.set("exception", ex.what());
            m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDownloadFailed, callbackData);
         }
      }



      void CWidget::update(const std::string & widgetName, const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Updating widget " << widgetName << " from " << downloadUrl;

         shared::CDataContainer callbackData;
         callbackData.set("widgetName", widgetName);
         callbackData.set("downloadUrl", downloadUrl);

         m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetUpdate, callbackData);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading widget package";
            m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetDownload, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, m_progressCallback, i18n::CClientStrings::UpdateWidgetDownload, 0.0, 90.0);
            YADOMS_LOG(information) << "Downloading widget package with sucess";



            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               m_progressCallback(true, 50.0f, i18n::CClientStrings::UpdatePluginDeploy, callbackData);
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               YADOMS_LOG(information) << "Plugin deployed with success";



               YADOMS_LOG(information) << "Deploy widget package " << downloadedPackage.toString();
               m_progressCallback(true, 90.0f, i18n::CClientStrings::UpdateWidgetDeploy, callbackData);
               Poco::Path widgetPath = CWorkerTools::deployWidgetPackage(downloadedPackage);

               YADOMS_LOG(information) << "Widget installed with success";
               m_progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, callbackData);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy widget package : " << ex.what();
               callbackData.set("exception", ex.what());
               m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDeployFailed, callbackData);
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
            callbackData.set("exception", ex.what());
            m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDownloadFailed, callbackData);
         }
      }

      void CWidget::remove(const std::string & widgetName)
      {
         YADOMS_LOG(information) << "Removing widget " << widgetName;

         shared::CDataContainer callbackData;
         callbackData.set("widgetName", widgetName);

         m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetRemove, callbackData);


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

            m_progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, callbackData);
         }
         catch (std::exception & ex)
         {
            //fail to remove package
            YADOMS_LOG(error) << "Fail to delete widget : " << widgetName << " : " << ex.what();
            callbackData.set("exception", ex.what());
            m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetRemoveFailed, callbackData);
         }
      }


   } // namespace worker
} // namespace update
