#pragma once
#include <Poco/Path.h>

namespace update {
   namespace worker {

      class CWorkerTools
      {
      public:
         //---------------------------------------------
         ///\brief   Download a package
         ///\param [in] downloadUrl  The downloaded package URL
         ///\return The package local path
         //---------------------------------------------
         static Poco::Path downloadPackage(const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Deploy a package
         ///\param [in] downloadedPackage  The downloaded package
         ///\param [in] outputDirectory  The output base path (plugins folder for example)
         ///\return The deployed directory
         //---------------------------------------------
         static Poco::Path deployPackage(Poco::Path downloadedPackage, const std::string & outputDirectory);

         //---------------------------------------------
         ///\brief   Deploy a plugin package
         ///\param [in] downloadedPackage  The downloaded package
         ///\return The deployed directory
         //---------------------------------------------
         static Poco::Path deployPluginPackage(Poco::Path downloadedPackage);
         //---------------------------------------------
         ///\brief   Deploy a widget package
         ///\param [in] downloadedPackage  The downloaded package
         ///\return The deployed directory
         //---------------------------------------------
         static Poco::Path deployWidgetPackage(Poco::Path downloadedPackage);
         //---------------------------------------------
         ///\brief   Deploy a script interpreter package
         ///\param [in] downloadedPackage  The downloaded package
         ///\return The deployed directory
         //---------------------------------------------
         static Poco::Path deployScriptInterpreterPackage(Poco::Path downloadedPackage);

         //---------------------------------------------
         ///\brief   Get the plugins base path
         ///\return The path
         //---------------------------------------------
         static std::string getPluginBasePath();
         //---------------------------------------------
         ///\brief   Get the widget base path
         ///\return The path
         //---------------------------------------------
         static std::string getWidgetBasePath();
         //---------------------------------------------
         ///\brief   Get the script interpreter base path
         ///\return The path
         //---------------------------------------------
         static std::string getScriptInterpreterBasePath();
      };

   } // namespace worker
} // namespace update
