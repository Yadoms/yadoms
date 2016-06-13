#pragma once

#include "server/database/IRuleRequester.h"
#include "server/database/IDatabaseRequester.h"

namespace database { namespace common { namespace requesters {

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
         explicit CRule(boost::shared_ptr<IDatabaseRequester> databaseRequester);

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
         /// \Brief		   Reference to IDatabaseRequester
         //--------------------------------------------------------------
         boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
      };

} //namespace requesters
} //namespace common
} //namespace database 

