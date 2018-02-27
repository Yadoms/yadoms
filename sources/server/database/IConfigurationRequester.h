#pragma once

#include "entities/Entities.h"

namespace database
{
   class IConfigurationRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief      Read a configuration entry
      /// \param [in] section    the configuration SECTION
      /// \return     The configuration entry found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CConfiguration> getConfiguration(const std::string& section) = 0;

      //--------------------------------------------------------------
      /// \brief      Read all configuration entries 
      /// \return     The entries found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CConfiguration>> getConfigurations() = 0;

      //--------------------------------------------------------------
      /// \brief      Update a configuration entry
      /// \param [in] section  Section to update
      /// \param [in] value    New value
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateConfiguration(const std::string& section,
                                       const std::string& value) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IConfigurationRequester()
      {
      }
   };
} //namespace database 
