#pragma once

#include <Poco/Util/TimerTask.h>
#include "database/IAcquisitionRequester.h"
#include "SQLiteRequester.h"

namespace database { 
namespace sqlite { 

   class CSQLitePurgeTask : public Poco::Util::TimerTask
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]	acquisitionRequester the acquisition requester 
      /// \param [in]	sqlRequester         the sql requester 
      //--------------------------------------------------------------
      CSQLitePurgeTask(boost::shared_ptr<IAcquisitionRequester> acquisitionRequester, boost::shared_ptr<CSQLiteRequester> sqlRequester);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSQLitePurgeTask();

      // BEGIN Poco::Util::TimerTask implementation 
      virtual void run();
      // END Poco::Util::TimerTask implementation 

private:
      //--------------------------------------------------------------
      /// \Brief		   Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<IAcquisitionRequester> m_acquisitionRequester;

      //--------------------------------------------------------------
      /// \Brief		   Sql requester
      //--------------------------------------------------------------
      boost::shared_ptr<CSQLiteRequester> m_sqlRequester;

      //--------------------------------------------------------------
      /// \Brief		   Acquisition lifetime (in days)
      //--------------------------------------------------------------
      int m_acquisitionLifetimeDays;
   };

} //namespace sqlite
} //namespace database 

