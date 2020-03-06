#pragma once
#include <shared/DataContainer.h>
#include <shared/versioning/SemVer.h>

namespace shared
{
   namespace plugin
   {
      namespace information
      {
         //--------------------------------------------------------------
         /// \brief		Interface for information about plugins
         //--------------------------------------------------------------
         class IInformation
         {
         public:
            //--------------------------------------------------------------
            /// \brief	    Destructor
            //--------------------------------------------------------------
            virtual ~IInformation()
            {
            }

            //--------------------------------------------------------------
            /// \brief	    get the plugin type
            /// \return     the plugin type
            //--------------------------------------------------------------
            virtual const std::string& getType() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the plugin version
            /// \return     the plugin version
            /// \note       Version : Note that this value is string-comparable
            ///             to check the precedence of a version from a reference version.
            //--------------------------------------------------------------
            virtual const versioning::CSemVer& getVersion() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the plugin author
            /// \return     the plugin author
            //--------------------------------------------------------------
            virtual const std::string& getAuthor() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the plugin URL
            /// \return     the plugin URL
            //--------------------------------------------------------------
            virtual const std::string& getUrl() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get all identity informations in printable format
            /// \return     all plugin informations (plugin name and version)
            //--------------------------------------------------------------
            virtual std::string getIdentity() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get all informations in printable format
            /// \return     all plugin informations
            //--------------------------------------------------------------
            virtual std::string toString() const = 0;


            //--------------------------------------------------------------
            /// \brief	    Check if plugin is supported on this platform
            /// \return     true if supported, false if not
            //--------------------------------------------------------------
            virtual bool isSupportedOnThisPlatform() const = 0;


            //--------------------------------------------------------------
            /// \brief	    indicates if the plugin supports manually created devices
            /// \return     true if the plugin supports manually created devices
            //--------------------------------------------------------------
            virtual bool getSupportManuallyCreatedDevice() const = 0;


            //--------------------------------------------------------------
            /// \brief	    indicates if the plugin supports device removed notification
            /// \return     true if the plugin supports device removed notification
            //--------------------------------------------------------------
            virtual bool getSupportDeviceRemovedNotification() const = 0;


            //--------------------------------------------------------------
            /// \brief	    Provide the package.json content
            /// \return     the package.json content
            //--------------------------------------------------------------
            virtual shared::CDataContainerSharedPtrConst getPackage() const = 0;

            //--------------------------------------------------------------
            /// \brief	    Get the plugin path
            /// \return     the plugin path (folder)
            /// \note       Do not store data in this path, use the instance data path instead (see IYPluginApi::getDataPath)
            //--------------------------------------------------------------
            virtual const boost::filesystem::path& getPath() const = 0;
         };
      }
   }
} // namespace shared::plugin::information


