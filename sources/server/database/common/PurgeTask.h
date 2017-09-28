#pragma once

#include <Poco/Util/TimerTask.h>
#include "database/IAcquisitionRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      class CPurgeTask : public Poco::Util::TimerTask
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param [in]	acquisitionRequester the acquisition requester 
         /// \param [in]	sqlRequester         the sql requester 
         //--------------------------------------------------------------
         CPurgeTask(boost::shared_ptr<IAcquisitionRequester> acquisitionRequester, boost::shared_ptr<IDatabaseRequester> sqlRequester);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CPurgeTask();

         // Poco::Util::TimerTask implementation 
         void run() override;
         // [END] Poco::Util::TimerTask implementation 

      private:
         //--------------------------------------------------------------
         /// \Brief		   Acquisition requester
         //--------------------------------------------------------------
         boost::shared_ptr<IAcquisitionRequester> m_acquisitionRequester;

         //--------------------------------------------------------------
         /// \Brief		   Sql requester
         //--------------------------------------------------------------
         boost::shared_ptr<IDatabaseRequester> m_sqlRequester;

         //--------------------------------------------------------------
         /// \Brief		   Acquisition lifetime (in days)
         //--------------------------------------------------------------
         int m_acquisitionLifetimeDays;
      };
   } //namespace common
} //namespace database 


