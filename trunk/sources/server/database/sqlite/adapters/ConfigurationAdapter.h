#pragma once

#include "ISQLiteResultAdapter.h"
#include "server/database/entities/Configuration.h"

//--------------------------------------------------------------
/// \Brief		Adapter for Configuration entities
//--------------------------------------------------------------
class CConfigurationAdapter : public ISQLiteResultAdapter<boost::shared_ptr<CConfiguration> >
{
public:
   //--------------------------------------------------------------
   /// \Brief		Constructor
   //--------------------------------------------------------------
   CConfigurationAdapter();
   
   //--------------------------------------------------------------
   /// \Brief		Destructor
   //--------------------------------------------------------------
   virtual ~CConfigurationAdapter();
   
   // ISQLiteResultAdapter implementation
   bool adapt(int column, char** columValues, char** columnName);
   std::vector<boost::shared_ptr<CConfiguration> > getResults(); 
   // [END] ISQLiteResultAdapter implementation

private:
   //--------------------------------------------------------------
   /// \Brief		Contains the list of Configuration entities
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<CConfiguration> > m_results;
};