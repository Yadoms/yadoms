#pragma once

#include "IOpenZWaveNodeKeyword.h"
#include <value_classes/Value.h>

//--------------------------------------------------------------
/// \brief     Class used to encapsulate a ZWave network node (from OpenZWave)
///            Provide access to capacities and keywords
//--------------------------------------------------------------
class COpenZWaveNodeConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	      Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeConfiguration(const uint32 homeId, const uint8 nodeId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------   
   virtual ~COpenZWaveNodeConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Register a configuration item (generally called when a zwave node send a configuration value)
   //--------------------------------------------------------------   
   void registerConfiguration(OpenZWave::ValueID& value);

   //--------------------------------------------------------------
   /// \brief	      Get the keyword matching the ValueID, or create it if needed
   /// \param [in]   value                   The ValueID associated to the keyword
   //--------------------------------------------------------------   
   boost::shared_ptr<IOpenZWaveNodeKeyword> getConfigurationItem(OpenZWave::ValueID& value);

   //--------------------------------------------------------------
   /// \brief	      Send a command to a keyword
   /// \param [in]   classIdentifier   The class identifier for the keyword
   /// \param [in]   keyword           The keyword name
   /// \param [in]   commandData       The command data
   //--------------------------------------------------------------   
   bool sendCommand(const std::string& keyword, const std::string& commandData);


   //--------------------------------------------------------------
   /// \brief	      Generate the configuration schema for the device
   /// \return       The configuration schema
   //--------------------------------------------------------------   
   shared::CDataContainer generateConfigurationSchema();

   //--------------------------------------------------------------
   /// \brief	      Generate the configuration values for the device
   /// \return       The configuration values (all conf-keyword values)
   //--------------------------------------------------------------   
   shared::CDataContainer saveValuesToDatabase();
private:
   //--------------------------------------------------------------
   /// \brief	      homeId
   //--------------------------------------------------------------      
   uint32 m_homeId;

   //--------------------------------------------------------------
   /// \brief	      nodeId
   //--------------------------------------------------------------      
   uint8 m_nodeId;

   //--------------------------------------------------------------
   /// \brief	      Type for configuration item list
   //--------------------------------------------------------------    
   typedef std::map<std::string, boost::shared_ptr<IOpenZWaveNodeKeyword> > ConfigurationItemsContainer;

   //--------------------------------------------------------------
   /// \brief	      The configuration items list list
   //--------------------------------------------------------------    
   ConfigurationItemsContainer m_configurationItems;
};

