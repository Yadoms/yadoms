#pragma once

#include "IOpenZWaveNodeKeyword.h"
#include <value_classes/Value.h>
#include "OpenZwaveNodeConfiguration.h"

//--------------------------------------------------------------
/// \brief     Class used to encapsulate a ZWave network node (from OpenZWave)
///            Provide access to capacities and keywords
//--------------------------------------------------------------
class COpenZWaveNode
{
public:

   //--------------------------------------------------------------
   /// \brief	      Constructor
   //--------------------------------------------------------------
   COpenZWaveNode();

   //--------------------------------------------------------------
   /// \brief	      Constructor
   /// \param [in]   homeId   The home id
   /// \param [in]   nodeId   The node id
   //--------------------------------------------------------------
   COpenZWaveNode(const uint32 homeId, const uint8 nodeId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------   
   virtual ~COpenZWaveNode();

   //--------------------------------------------------------------
   /// \brief	      Register a keyword
   /// \param [in]   value                   The ValueID associated to the keyword
   /// \param [in]   includeSystemKeywords   true if system keywords are supported
   //--------------------------------------------------------------   
   void registerKeyword(OpenZWave::ValueID& value, bool includeSystemKeywords);

   //--------------------------------------------------------------
   /// \brief	      Update a keyword value
   /// \param [in]   value                   The ValueID associated to the keyword
   /// \param [in]   includeSystemKeywords   true if system keywords are supported
   //--------------------------------------------------------------   
   boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> updateKeywordValue(OpenZWave::ValueID& value, bool includeSystemKeywords);

   //--------------------------------------------------------------
   /// \brief	      Get the keyword matching the ValueID, or create it if needed
   /// \param [in]   value                   The ValueID associated to the keyword
   /// \param [in]   includeSystemKeywords   true if system keywords are supported
   //--------------------------------------------------------------   
   boost::shared_ptr<IOpenZWaveNodeKeyword> getKeyword(OpenZWave::ValueID& value, bool includeSystemKeywords);

   //--------------------------------------------------------------
   /// \brief	      Send a command to a keyword
   /// \param [in]   classIdentifier   The class identifier for the keyword
   /// \param [in]   keyword           The keyword name
   /// \param [in]   commandData       The command data
   //--------------------------------------------------------------   
   bool sendCommand(const std::string& keyword, const std::string& commandData);

   //--------------------------------------------------------------
   /// \brief	      get the last value of a keyword
   /// \param [in]   classIdentifier   The class identifier for the keyword
   /// \param [in]   keyword           The keyword name
   //--------------------------------------------------------------   
   const boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue(const std::string& keyword);

   //--------------------------------------------------------------
   /// \brief	      Check if this node match to a pair [home,node]
   /// \param [in]   homeId   The home id
   /// \param [in]   nodeId   The node id
   /// \return       true if current node as the same homeId and nodeId
   //--------------------------------------------------------------     
   const bool match(const uint32 homeId, const uint8 nodeId);

   //--------------------------------------------------------------
   /// \brief	      Get the homeId
   /// \return       The homeId
   //--------------------------------------------------------------      
   const uint32 getHomeId();
   //--------------------------------------------------------------
   /// \brief	      Get the nodeId
   /// \return       The nodeId
   //--------------------------------------------------------------      
   const uint8 getNodeId();

   //--------------------------------------------------------------
   /// \brief	      Get the configuration schema
   /// \return       the configuration schema
   //--------------------------------------------------------------      
   shared::CDataContainer getConfigurationSchema();

   //--------------------------------------------------------------
   /// \brief	      Get the configuration values
   /// \return       the configuration values
   //--------------------------------------------------------------      
   shared::CDataContainer getConfigurationValues();


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
   /// \brief	      Type for keyword list
   //--------------------------------------------------------------    
   typedef std::map<std::string, boost::shared_ptr<IOpenZWaveNodeKeyword> > KeywordsContainer;

   //--------------------------------------------------------------
   /// \brief	      The keyword list
   //--------------------------------------------------------------    
   KeywordsContainer m_keywords;
   
   //--------------------------------------------------------------
   /// \brief	      The configuration items list list
   //--------------------------------------------------------------    
   COpenZWaveNodeConfiguration m_configuration;
};

