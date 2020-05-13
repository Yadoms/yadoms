#include "stdafx.h"
#include "Widget.h"
#include "WorkerTools.h"
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>
#include "i18n/ClientStrings.h"
#include <shared/Log.h>
#include "WidgetInformation.h"

namespace update
{
   namespace worker
   {
      void CWidget::install(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& downloadUrl,
                            const boost::filesystem::path& widgetsPath)
      {
         YADOMS_LOG(information) << "Installing new widget from " << downloadUrl;

         boost::shared_ptr<shared::CDataContainer> callbackData = shared::CDataContainer::make();
         callbackData->set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetInstall, std::string(), callbackData);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         Poco::Path downloadedPackage;
         try
         {
            YADOMS_LOG(information) << "Downloading widget package";

            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetDownload, std::string(), callbackData);
            downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdateWidgetDownload,
                                                                         0.0, 90.0);
            YADOMS_LOG(information) << "Downloading widget package with success";


            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy widget package " << downloadedPackage.toString();
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateWidgetDeploy, std::string(), callbackData);
               const auto widgetPath = CWorkerTools::deployPackage(downloadedPackage, widgetsPath.string());
               YADOMS_LOG(information) << "Widget deployed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, std::string(), shared::CDataContainer::make());
            }
            catch (std::exception& ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy widget package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDeployFailed, ex.what(), callbackData);
            }
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download pwidget package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDownloadFailed, ex.what(), callbackData);
         }

         //delete downloaded zip file
         if (!downloadedPackage.toString().empty())
         {
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();
         }
      }


      void CWidget::update(CWorkerTools::WorkerProgressFunc progressCallback,
                           const std::string& widgetName,
                           const std::string& downloadUrl,
                           const boost::filesystem::path& widgetsPath)
      {
         YADOMS_LOG(information) << "Updating widget " << widgetName << " from " << downloadUrl;

         boost::shared_ptr<shared::CDataContainer> callbackData = shared::CDataContainer::make();
         callbackData->set("widgetName", widgetName);
         callbackData->set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetUpdate, std::string(), callbackData);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         Poco::Path downloadedPackage;
         try
         {
            YADOMS_LOG(information) << "Downloading widget package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetDownload, std::string(), callbackData);
            downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdateWidgetDownload,
                                                                         0.0, 90.0);
            YADOMS_LOG(information) << "Downloading widget package with success";


            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy widget package " << downloadedPackage.toString();
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateWidgetDeploy, std::string(), callbackData);
               const auto widgetPath = CWorkerTools::deployPackage(downloadedPackage, widgetsPath.string());

               YADOMS_LOG(information) << "Widget installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, std::string(), callbackData);
            }
            catch (std::exception& ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy widget package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDeployFailed, ex.what(), callbackData);
            }
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download pwidget package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetDownloadFailed, ex.what(), callbackData);
         }

         //delete downloaded zip file
         if (!downloadedPackage.toString().empty())
         {
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();
         }
      }

      void CWidget::remove(CWorkerTools::WorkerProgressFunc progressCallback,
                           const std::string& widgetName,
                           const boost::filesystem::path& widgetsPath)
      {
         YADOMS_LOG(information) << "Removing widget " << widgetName;

         boost::shared_ptr<shared::CDataContainer> callbackData = shared::CDataContainer::make();
         callbackData->set("widgetName", widgetName);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateWidgetRemove, std::string(), callbackData);


         try
         {
            /////////////////////////////////////////////
            //1. remove widget folder
            /////////////////////////////////////////////
            Poco::Path widgetPath(widgetsPath.string());
            widgetPath.append(widgetName);

            Poco::File toDelete(widgetPath);
            if (toDelete.exists())
               toDelete.remove(true);

            progressCallback(true, 100.0f, i18n::CClientStrings::UpdateWidgetSuccess, std::string(), callbackData);
         }
         catch (std::exception& ex)
         {
            //fail to remove package
            YADOMS_LOG(error) << "Fail to delete widget : " << widgetName << " : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateWidgetRemoveFailed, ex.what(), callbackData);
         }
      }

      CWidget::AvailableWidgetMap CWidget::getWidgetList(const boost::filesystem::path& widgetsPath)
      {
         AvailableWidgetMap widgets;

         for (Poco::DirectoryIterator it(Poco::Path(widgetsPath.string())); it != Poco::DirectoryIterator(); ++it)
         {
            try
            {
               const auto widget = boost::make_shared<CWidgetInformation>(it->path());
               widgets[widget->getType()] = widget;
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << "Unknown widget in " << it->path() << ", " << e.what();
            }
         }

         return widgets;
      }
   } // namespace worker
} // namespace update
