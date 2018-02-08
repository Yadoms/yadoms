#pragma once

#include "entities/Entities.h"

namespace database { 

   class IConfigurationRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief      Create a new configuration entry
      /// \param [in] configurationToCreate  New configuration informations
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void create(entities::CConfiguration& configurationToCreate) = 0;

      //--------------------------------------------------------------
      /// \brief      Checkl a a configuration entry already exists
      /// \param [in] section    the configuration SECTION
      /// \param [in] name       the configuration NAME
      /// \return     true if the configuration exists, false other cases
      //--------------------------------------------------------------
      virtual bool exists(const std::string & section, const std::string & name) = 0;
     
      //--------------------------------------------------------------
      /// \brief      Read a configuration entry
      /// \param [in] section    the configuration SECTION
      /// \param [in] name       the configuration NAME
      /// \return     The configuration entry found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CConfiguration> getConfiguration(const std::string & section, const std::string & name) = 0;
      
      //--------------------------------------------------------------
      /// \brief      Read all configuration entries for one section
      /// \param [in] section    the configuration SECTION
      /// \return     The entries found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CConfiguration> > getConfigurations(const std::string& section) = 0;

      //--------------------------------------------------------------
      /// \brief      Read all configuration entries 
      /// \return     The entries found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CConfiguration> > getConfigurations() = 0;

      //--------------------------------------------------------------
      /// \brief      Update a configuration entry
      /// \param [in] configurationToUpdate  Configuration informations
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateConfiguration(entities::CConfiguration& configurationToUpdate) = 0;
      
      //--------------------------------------------------------------
      /// \brief      Remove a configuration entry
      /// \param [in] configurationToRemove  The configuration to delete
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeConfiguration(entities::CConfiguration& configurationToRemove) = 0;
      
      //--------------------------------------------------------------
      /// \brief      Remove a entire configuration section
      /// \param [in] section  The configuration section to delete
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeConfigurations(const std::string& section) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IConfigurationRequester()
      {
      }
   };
   
 
} //namespace database 
