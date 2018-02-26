#pragma once
#include "database/entities/Entities.h"

namespace dataAccessLayer
{
   class IConfigurationManager
   {
   public:
      virtual ~IConfigurationManager()
      {
      }

      //--------------------------------------------------------------
      /// \brief      Read a configuration entry
      /// \param [in] section    the configuration SECTION
      /// \return     The configuration entry found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::string getExternalConfiguration(const std::string& section) const = 0;

      //--------------------------------------------------------------
      /// \brief      Update a configuration entry
      /// \param [in] section  Section to update
      /// \param [in] value    New value
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void saveExternalConfiguration(const std::string& section,
                                             const shared::CDataContainer& value) = 0;

      //--------------------------------------------------------------
      /// \brief      Load server configuration
      /// \return     The stored configuration if exist, else the default values
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const shared::CDataContainer> getServerConfiguration() const = 0;

      //--------------------------------------------------------------
      /// \brief      Save system configuration
      /// \param [in] newConfiguration the new configuration to save
      //--------------------------------------------------------------
      virtual void saveServerConfiguration(const shared::CDataContainer& newConfiguration) = 0;

      //--------------------------------------------------------------
      /// \brief      Reset system configuration
      //--------------------------------------------------------------
      virtual void resetServerConfiguration() = 0;

      //--------------------------------------------------------------
      /// \brief      Get database version
      /// \return     The current database version
      //--------------------------------------------------------------
      virtual std::string getDatabaseVersion() const = 0;

      //--------------------------------------------------------------
      /// \brief      Subscribe to system configuration changes
      //--------------------------------------------------------------
      virtual void subscribeOnServerConfigurationChanged(
         boost::function1<void, boost::shared_ptr<const shared::CDataContainer>> onServerConfigurationChangedFct) = 0;

      //--------------------------------------------------------------
      /// \brief      High-level server configuration accessors
      //--------------------------------------------------------------
      virtual shared::CDataContainer getLocation() const = 0;
      virtual void saveLocation(const shared::CDataContainer& newLocation) = 0;
      virtual shared::CDataContainer getBasicAuthentication() const = 0;
      virtual void saveBasicAuthentication(const shared::CDataContainer& newBasicAuthentication) = 0;
   };
} //namespace dataAccessLayer 
