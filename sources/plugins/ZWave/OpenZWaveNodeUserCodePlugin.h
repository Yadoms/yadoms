#pragma once

#include "IOpenZWaveNodePlugin.h"
#include <value_classes/Value.h>

//--------------------------------------------------------------
/// \brief     Class used to encapsulate a ZWave network node (from OpenZWave)
///            Provide access to capacities and keywords
//--------------------------------------------------------------
class COpenZWaveNodeUserCodePlugin : public IOpenZWaveNodePlugin
{
public:

   //--------------------------------------------------------------
   /// \brief	      Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeUserCodePlugin();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------   
   virtual ~COpenZWaveNodeUserCodePlugin();

   // IOpenZWaveNodePlugin implementation
   bool isKeywordManagedByPlugin(OpenZWave::ValueID& value, const uint32 homeId, const uint8 nodeId) override;
   boost::shared_ptr<IOpenZWaveNodeKeyword> createKeyword(OpenZWave::ValueID& vID, uint32 homeId, uint8 nodeId, bool includeSystemKeywords) override;
   void onKeywordValueUpdated(OpenZWave::ValueID& value) override;
   void getExtraQueries(std::vector<shared::CDataContainer> & extraQueriesNames) override;
   std::string getName() override;
   bool onExtraQuery(const std::string & query, const shared::CDataContainer &data) override;
   // [END] IOpenZWaveNodePlugin implementation

private:
   bool m_bEnrollmentMode;

   int findFirstFreeSlot();
   std::map<int, OpenZWave::ValueID> m_codeSlots;
};

