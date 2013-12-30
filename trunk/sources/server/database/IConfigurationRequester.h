#pragma once

#include "entities/Configuration.h"

class IConfigurationRequester
{
public:
   //--------------------------------------------------------------
   /// \brief      Create a new configuration entry
   /// \param [in] configurationToCreate  New configuration informations
   /// \throw      CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual void create(boost::shared_ptr<CConfiguration> configurationToCreate) = 0;

   //--------------------------------------------------------------
   /// \brief      Read a configuration entry
   /// \param [in] section    the configuration SECTION
   /// \param [in] name       the configuration NAME
   /// \return     The configuration entry found
   /// \throw      CEmptyResultException if fails
   //--------------------------------------------------------------
	virtual boost::shared_ptr<CConfiguration> getConfiguration(const std::string & section, const std::string & name) = 0;
	
   //--------------------------------------------------------------
   /// \brief      Update a configuration entry
   /// \param [in] configurationToUpdate  Configuration informations
   /// \throw      CEmptyResultException if fails
   //--------------------------------------------------------------
	virtual void updateConfiguration(boost::shared_ptr<CConfiguration> configurationToUpdate) = 0;
   
   //--------------------------------------------------------------
   /// \brief      Remove a configuration entry
   /// \param [in] configurationToRemove  The configuration to delete
   /// \throw      CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual void removeConfiguration(boost::shared_ptr<CConfiguration> configurationToRemove) = 0;

   //--------------------------------------------------------------
   /// \brief       Destructor
   //--------------------------------------------------------------
	virtual ~IConfigurationRequester()
	{
	}
};
