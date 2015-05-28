#pragma once
#include <Poco/Path.h>

namespace update {
   namespace worker {

      class CPlugin
      {
      public:
         //---------------------------------
         ///\brief Define a function prototype for updating the worker progress
         //---------------------------------
         typedef boost::function3<void, bool, boost::optional<float>, std::string > WorkerProgressFunc;

         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] progressCallback The progress callback
         //---------------------------------------------
         CPlugin(WorkerProgressFunc progressCallback);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CPlugin();

         
         //---------------------------------------------
         ///\brief   Install a new plugin
         ///\param [in] downloadUrl The plugin package url
         //---------------------------------------------
         void install(const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Update a plugin
         ///\param [in] pluginName  The plugin name
         ///\param [in] downloadUrl The plugin package url
         //---------------------------------------------
         void update(const std::string & pluginName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Remove a plugin
         ///\param [in] pluginName  The plugin name
         //---------------------------------------------
         void remove(const std::string & pluginName);

      private:
         //---------------------------------------------
         ///\brief   Download a package
         ///\param [in] downloadUrl  The downloaded package URL
         ///\return The package local path
         //---------------------------------------------
         Poco::Path downloadPackage(const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Deploy a package to plugin folder
         ///\param [in] downloadedPackage  The downloaded package
         ///\return The plugin directory
         //---------------------------------------------
         Poco::Path deployPackage(Poco::Path downloadedPackage);

         //---------------------------------------------
         ///\brief   The progress callback
         //---------------------------------------------
         WorkerProgressFunc m_progressCallback;
      };

   } // namespace worker
} // namespace update
