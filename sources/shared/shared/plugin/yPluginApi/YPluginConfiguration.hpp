#pragma once
#include <shared/DataContainer.h>
#include <shared/FileSystemExtension.h>


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief Manage a plugin configuration. This class is an helper
         /// to retrieve a configuration value. If value is not present in the current
         /// configuration (probably because of plugin version was updated),
         /// this helper try to get a default value from package.json file of plugin.
         //-----------------------------------------------------
         class YPluginConfiguration
         {
         public:
            //-----------------------------------------------------
            ///\brief Constructor
            //-----------------------------------------------------
            YPluginConfiguration()
               :m_configurationData(shared::CDataContainer::make()), m_packageData(shared::CDataContainer::make())
            {
               
            }

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~YPluginConfiguration() = default;

            //--------------------------------------------------------------
            /// \brief		   Load configuration data
            /// \param [in] configurationData The raw configuration data (from Yadoms database)
            /// \param [in] packagePath The package path (default to "package.json" from module path)
            //--------------------------------------------------------------
            void initializeWith(const boost::shared_ptr<CDataContainer>& configurationData,
                                const boost::filesystem::path& packagePath = CFileSystemExtension::getModulePath() / boost::filesystem::path("package.json"))
            {
               // Reload package file
               m_packageData->deserializeFromFile(packagePath.string());

               m_configurationData->initializeWith(configurationData);
            }

            //--------------------------------------------------------------
            /// \brief	    Check if parameter is present (only in the configuration)
            /// \param [in] parameterName    Name of the parameter
            /// \return     true if parameter found
            //--------------------------------------------------------------
            bool exists(const std::string& parameterName) const
            {
               return m_configurationData->exists(parameterName);
            }

            //--------------------------------------------------------------
            /// \brief	    Get a value from configuration
            /// \param[in] parameterName Parameter to retrieve configuration
            /// \return The parameter value from configuration. If not found, search for
            /// a default value in the package.json file of plugin.
            /// \throw      shared::exception::CInvalidParameter if parameter is not found
            //--------------------------------------------------------------
            template <typename T>
            inline T get(const std::string& parameterName) const
            {
               try
               {
                  return m_configurationData->get<T>(parameterName);
               }
               catch (exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData->get<T>(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }

            //--------------------------------------------------------------
            /// \brief	    Get a enum value from configuration
            /// \param[in] parameterName Parameter to retrieve configuration
            /// \return The parameter value from configuration. If not found, search for
            /// a default value in the package.json file of plugin.
            /// \throw      shared::exception::CInvalidParameter if parameter is not found
            //--------------------------------------------------------------
            template <typename EnumType>
            EnumType getEnumValue(const std::string& parameterName,
                                  const CDataContainer::EnumValuesNames& valuesNames) const
            {
               try
               {
                  return m_configurationData->getEnumValue<EnumType>(parameterName, valuesNames);
               }
               catch (exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData->getEnumValue<EnumType>(std::string("configurationSchema.") + parameterName + (".defaultValue"), valuesNames);
            }

         private:
            //--------------------------------------------------------------
            /// \brief	    Configuration raw data
            //--------------------------------------------------------------
            boost::shared_ptr<CDataContainer> m_configurationData;

            //--------------------------------------------------------------
            /// \brief	    Package raw data
            //--------------------------------------------------------------
            boost::shared_ptr<CDataContainer> m_packageData;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


