#pragma once

#include "ISQLiteResultAdapter.h"


namespace database { 
namespace sqlite { 
namespace adapters { 


   //--------------------------------------------------------------
   /// \Brief		Adapter for generic table results
   //--------------------------------------------------------------
   class CGenericAdapter : public ISQLiteResultAdapter<std::map<std::string, std::string> >
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
      bool adapt(sqlite3_stmt * pStatement);
      std::vector<std::map<std::string, std::string> > getResults(); 
      // [END] ISQLiteResultAdapter implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		Contains the list of Plugin entities
      //--------------------------------------------------------------
      std::vector<std::map<std::string, std::string> > m_results;
   };

} //namespace adapters
} //namespace sqlite
} //namespace database 

