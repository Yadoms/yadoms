#pragma once
#include "database/entities/Entities.h"

//TODO supprimer les fonctions génériques

namespace dataAccessLayer
{
   class IConfigurationManager
   {
   public:

      //--------------------------------------------------------------
      /// \brief      Load system configuration
      /// \param [in] keyName the configuration value name (all system configuration if not provided)
      /// \return     The stored configuration if exist, else the default values
      //--------------------------------------------------------------
      virtual std::string getSystemConfiguration(const std::string& keyName) const = 0;
      virtual boost::shared_ptr<const shared::CDataContainer> getSystemConfiguration() = 0;
      
      //--------------------------------------------------------------
      /// \brief      Reset system configuration
      //--------------------------------------------------------------
      virtual void resetSystemConfiguration() = 0;


      //--------------------------------------------------------------
      /// \brief      Check a a configuration entry already exists
      /// \param [in] section    the configuration SECTION
      /// \param [in] name       the configuration NAME
      /// \return     true if the configuration exists, false other cases
      //--------------------------------------------------------------
      virtual bool exists(const std::string& section, const std::string& name) = 0;

      //--------------------------------------------------------------
      /// \brief      Read a configuration entry
      /// \param [in] section    the configuration SECTION
      /// \param [in] name       the configuration NAME
      /// \return     The configuration entry found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CConfiguration> getConfiguration(const std::string& section, const std::string& name) = 0;

      //--------------------------------------------------------------
      /// \brief      Read all configuration entries for one section
      /// \param [in] section    the configuration SECTION
      /// \return     The entries found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CConfiguration>> getConfigurations(const std::string& section) = 0;

      //--------------------------------------------------------------
      /// \brief      Read all configuration entries 
      /// \return     The entries found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CConfiguration>> getConfigurations() = 0;

      //--------------------------------------------------------------
      /// \brief      Update a configuration entry
      /// \param [in] configurationToUpdate  Configuration informations
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateConfiguration(database::entities::CConfiguration& configurationToUpdate) = 0;

      //--------------------------------------------------------------
      /// \brief      Remove a configuration entry
      /// \param [in] configurationToRemove  The configuration to delete
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeConfiguration(database::entities::CConfiguration& configurationToRemove) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IConfigurationManager()
      {
      }
   };
} //namespace dataAccessLayer 
