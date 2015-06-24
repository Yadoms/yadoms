#pragma once

#include "OpenZWaveNodeCapacity.h"
#include "OpenZWaveCommandClass.h"
#include <Value.h>

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
   /// \brief	      Get a capacity
   /// \param [in]   classIdentifier   The class identifier
   /// \return       The class identifier
   //--------------------------------------------------------------   
   boost::shared_ptr<COpenZWaveNodeCapacity> getCapacity(ECommandClass classIdentifier);
   
   //--------------------------------------------------------------
   /// \brief	      Register a capacity for this node
   /// \param [in]   classIdentifier   The class identifier to register
   //--------------------------------------------------------------   
   void registerCapacity(ECommandClass classIdentifier);

   //--------------------------------------------------------------
   /// \brief	      Register a keyword
   /// \param [in]   classIdentifier   The class identifier to register
   /// \param [in]   keyword           The keyword to register
   /// \param [in]   value             The ValueID associated to the keyword
   //--------------------------------------------------------------   
   void registerKeyword(ECommandClass classIdentifier, const std::string & keyword, OpenZWave::ValueID & value);
   
   //--------------------------------------------------------------
   /// \brief	      Send a command to a keyword
   /// \param [in]   classIdentifier   The class identifier for the keyword
   /// \param [in]   keyword           The keyword name
   /// \param [in]   commandData       The command data
   //--------------------------------------------------------------   
   bool sendCommand(ECommandClass classIdentifier, const std::string & keyword, const std::string & commandData);

   //--------------------------------------------------------------
   /// \brief	      get the last value of a keyword
   /// \param [in]   classIdentifier   The class identifier for the keyword
   /// \param [in]   keyword           The keyword name
   //--------------------------------------------------------------   
   const shared::plugin::yPluginApi::historization::IHistorizable & getLastKeywordValue(ECommandClass classIdentifier, const std::string & keyword);

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
   const uint8	getNodeId();

private:
   //--------------------------------------------------------------
   /// \brief	      homeId
   //--------------------------------------------------------------      
   uint32		m_homeId;

   //--------------------------------------------------------------
   /// \brief	      nodeId
   //--------------------------------------------------------------      
   uint8			m_nodeId;

   //--------------------------------------------------------------
   /// \brief	      Type for capacity list
   //--------------------------------------------------------------      
   typedef std::map<ECommandClass, boost::shared_ptr<COpenZWaveNodeCapacity> > CapacityListType;

   //--------------------------------------------------------------
   /// \brief	      The capacity list
   //--------------------------------------------------------------      
   CapacityListType m_nodeCapacities;
};





