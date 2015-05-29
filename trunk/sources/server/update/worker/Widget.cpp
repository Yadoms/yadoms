#include "stdafx.h"
#include "Widget.h"
#include "WorkerTools.h"
#include <Poco/File.h>

namespace update {
   namespace worker {

      CWidget::CWidget(WorkerProgressFunc progressCallback)
         :m_progressCallback(progressCallback)
      {

      }

      CWidget::~CWidget()
      {

      }

      void CWidget::install(const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Installing new widget from " + downloadUrl);

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading widget package");
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Widget package downloaded with success");

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               m_progressCallback(true, 50.0f, "Deploy widget package " + downloadedPackage.toString());
               Poco::Path widgetPath = CWorkerTools::deployWidgetPackage(downloadedPackage);
               m_progressCallback(true, 100.0f, "Widget deployed with success");
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy widget package : ") + ex.what());
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download widget package : ") + ex.what());
         }
      }

      void CWidget::update(const std::string & widgetName, const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Updating widget " + widgetName + " from " + downloadUrl);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading widget package");
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Widget package downloaded with success");

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               m_progressCallback(true, 50.0f, "Deploy widget package " + downloadedPackage.toString());
               Poco::Path widgetPath = CWorkerTools::deployWidgetPackage(downloadedPackage);
               m_progressCallback(true, 100.0f, "Widget updated with success");
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy widget package : ") + ex.what());
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download widget package : ") + ex.what());
         }
      }

      void CWidget::remove(const std::string & widgetName)
      {
         m_progressCallback(true, 0.0f, "Removing widget " + widgetName);

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
         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to delete widget : ") + widgetName + " : " + ex.what());
         }
      }


   } // namespace worker
} // namespace update
