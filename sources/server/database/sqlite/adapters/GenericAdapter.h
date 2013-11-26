#pragma once

#include "ISQLiteResultAdapter.h"
#include <map>
#include <string>


//--------------------------------------------------------------
/// \Brief		Adapter for generic table results
//--------------------------------------------------------------
class CGenericAdapter : public ISQLiteResultAdapter<std::map<std::string, std::string>>
{
public:
   //--------------------------------------------------------------
   /// \Brief		Constructor
   //--------------------------------------------------------------
   CGenericAdapter();
   
   //--------------------------------------------------------------
   /// \Brief		Destructor
   //--------------------------------------------------------------
   virtual ~CGenericAdapter();
   
   // ISQLiteResultAdapter implementation
   bool adapt(int column, char** columValues, char** columnName);
   std::vector<std::map<std::string, std::string>> getResults(); 
   // [END] ISQLiteResultAdapter implementation

private:
   //--------------------------------------------------------------
   /// \Brief		Contains the list of Hardware entities
   //--------------------------------------------------------------
   std::vector<std::map<std::string, std::string> > m_results;
};