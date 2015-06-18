#pragma once

#include "server/database/IRuleRequester.h"

namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Rule requester for SQLite database
      //--------------------------------------------------------------
      class CRule : public IRuleRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CRule(boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CRule();

         // IRuleRequester implementation
         virtual std::vector<boost::shared_ptr<entities::CRule> > getRules() const;
         virtual std::vector<boost::shared_ptr<entities::CRule> > getRules(const std::string & interpreterName) const;
         virtual boost::shared_ptr<entities::CRule> getRule(int ruleId) const;
         virtual int addRule(boost::shared_ptr<const entities::CRule> ruleData);
         virtual void updateRule(boost::shared_ptr<const entities::CRule> ruleData);
         virtual void deleteRule(int ruleId);
         // [END] IRuleRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 

