#pragma once

#include "ISQLiteResultAdapter.h"
#include "server/database/entities/Hardware.h"

//--------------------------------------------------------------
/// \Brief		Adapter for Hardware entities
//--------------------------------------------------------------
class CHardwareAdapter : public ISQLiteResultAdapter<CHardware>
{
public:
   //--------------------------------------------------------------
   /// \Brief		Constructor
   //--------------------------------------------------------------
   CHardwareAdapter();
   
   //--------------------------------------------------------------
   /// \Brief		Destructor
   //--------------------------------------------------------------
   virtual ~CHardwareAdapter();
   
   // ISQLiteResultAdapter implementation
   bool adapt(int column, char** columValues, char** columnName);
   std::vector<CHardware> getResults(); 
   // [END] ISQLiteResultAdapter implementation

private:
   //--------------------------------------------------------------
   /// \Brief		Contains the list of Hardware entities
   //--------------------------------------------------------------
   std::vector<CHardware> m_results;
};