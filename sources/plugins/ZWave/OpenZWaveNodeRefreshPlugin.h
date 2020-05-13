#pragma once

#include "IOpenZWaveNodePlugin.h"
#include <value_classes/Value.h>

class COpenZWaveNode;
//--------------------------------------------------------------
/// \brief     Class used to encapsulate a ZWave network node (from OpenZWave)
///            Provide access to capacities and keywords
//--------------------------------------------------------------
class COpenZWaveNodeRefreshPlugin : public IOpenZWaveNodePlugin
{
public:

   //--------------------------------------------------------------
   /// \brief	      Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeRefreshPlugin(const uint32 homeId, const uint8 nodeId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------   
   virtual ~COpenZWaveNodeRefreshPlugin();

   // IOpenZWaveNodePlugin implementation
   bool isKeywordManagedByPlugin(OpenZWave::ValueID& value, const uint32 homeId, const uint8 nodeId) override;
   boost::shared_ptr<IOpenZWaveNodeKeyword> createKeyword(OpenZWave::ValueID& vID, uint32 homeId, uint8 nodeId, bool includeSystemKeywords) override;
   std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > onKeywordValueUpdated(OpenZWave::ValueID& value) override;
   void getExtraQueries(std::vector<std::string> & extraQueriesNames) override;
   std::string getName() override;
   bool onExtraQuery(const std::string & query, const boost::shared_ptr<shared::CDataContainer> &data) override;
   // [END] IOpenZWaveNodePlugin implementation
   
private:
   const uint32 m_homeId;
   const uint8 m_nodeId;
};

