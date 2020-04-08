#pragma once

#include "IOpenZWaveNodeKeyword.h"
#include <value_classes/Value.h>
#include "OpenZWaveNodeConfiguration.h"
#include "IOpenZWaveNodePlugin.h"

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
   /// \return       All keyword historizers involved by current update (in case of a keyword update, change another keyword by software choice : enrolment example : the Usercode.Enrollment code is assigned to a UserCode entry)
   //--------------------------------------------------------------   
   std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > updateKeywordValue(OpenZWave::ValueID& value, bool includeSystemKeywords);

   //--------------------------------------------------------------
   /// \brief	      Get the keyword matching the ValueID, or create it if needed
   /// \param [in]   value                   The ValueID associated to the keyword
   /// \param [in]   includeSystemKeywords   true if system keywords are supported
   //--------------------------------------------------------------   
   boost::shared_ptr<IOpenZWaveNodeKeyword> getKeyword(OpenZWave::ValueID& value, bool includeSystemKeywords);

   //--------------------------------------------------------------
   /// \brief	      Send a command to a keyword
   /// \param [in]   keyword           The keyword name
   /// \param [in]   commandData       The command data
   //--------------------------------------------------------------   
   bool sendCommand(const std::string& keyword, const std::string& commandData);

   //--------------------------------------------------------------
   /// \brief	      get the last value of a keyword
   /// \param [in]   keyword           The keyword name
   //--------------------------------------------------------------   
   boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue(
      const std::string& keyword);

   //--------------------------------------------------------------
   /// \brief	      Check if this node match to a pair [home,node]
   /// \param [in]   homeId   The home id
   /// \param [in]   nodeId   The node id
   /// \return       true if current node as the same homeId and nodeId
   //--------------------------------------------------------------     
   bool match(const uint32 homeId, const uint8 nodeId) const;

   //--------------------------------------------------------------
   /// \brief	      Get the homeId
   /// \return       The homeId
   //--------------------------------------------------------------
   uint32 getHomeId() const;
   //--------------------------------------------------------------
   /// \brief	      Get the nodeId
   /// \return       The nodeId
   //--------------------------------------------------------------      
   uint8 getNodeId() const;

   //--------------------------------------------------------------
   /// \brief	      Get the configuration schema
   /// \return       the configuration schema
   //--------------------------------------------------------------      
   shared::CDataContainerSharedPtr getConfigurationSchema();

   //--------------------------------------------------------------
   /// \brief	      Get the configuration values
   /// \return       the configuration values
   //--------------------------------------------------------------      
   shared::CDataContainerSharedPtr getConfigurationValues();

   //--------------------------------------------------------------
   /// \brief	      Set the configuration values
   /// \param [in]   configuration     the configuration values
   //--------------------------------------------------------------      
   void setConfigurationValues(const shared::CDataContainerSharedPtr&configuration);

   //--------------------------------------------------------------
   /// \brief	Update device configuration (self update, from a device notification.) => updates only configuration container
   /// \param [in] 	   keyword        The targetted keyword
   /// \param [in] 	   value          The new value
   /// \param [in-out]  configuration  The initial and output configuration values
   //--------------------------------------------------------------
   void updateNodeConfiguration(const std::string& keyword, const std::string& value, shared::CDataContainerSharedPtr& configuration);

   //--------------------------------------------------------------
   /// \brief	Get the node extra query from plugins
   /// \param [in-out] extraQueries The dataContainer of extra queries definitions
   //--------------------------------------------------------------
   void getPluginExtraQueries(std::vector<std::string> & extraQueries);

   //--------------------------------------------------------------
   /// \brief	Extra query call
   /// \param [in] 	   query          The query name
   /// \param [in] 	   data           The query data (may be empty)
   /// \return true if successfull
   //--------------------------------------------------------------
   bool onExtraQuery(const std::string & query, const shared::CDataContainerSharedPtr&data);

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

   //--------------------------------------------------------------
   /// \brief	      The active plugin list
   //--------------------------------------------------------------    
   std::vector< boost::shared_ptr<IOpenZWaveNodePlugin> > m_plugins;
};

