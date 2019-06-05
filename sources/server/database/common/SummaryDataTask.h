#pragma once

#include <Poco/Util/TimerTask.h>
#include "database/IAcquisitionRequester.h"
#include "database/IKeywordRequester.h"

namespace database { 
namespace common { 

   class CSummaryDataTask : public Poco::Util::TimerTask
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param acquisitionRequester
      /// \param keywordRequester
      //--------------------------------------------------------------
      CSummaryDataTask(boost::shared_ptr<IAcquisitionRequester> acquisitionRequester, boost::shared_ptr<IKeywordRequester> keywordRequester);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSummaryDataTask();

      // BEGIN Poco::Util::TimerTask implementation 
      void run() override;
      // END Poco::Util::TimerTask implementation 

   private:
      //--------------------------------------------------------------
      /// \Brief		   Method for implementing first run process
      //--------------------------------------------------------------
      void executeFirstRunPass() const;

      //--------------------------------------------------------------
      /// \Brief		   Method for implementing common process (not first run)
      //--------------------------------------------------------------
      void executeCommonPass();
      
      //--------------------------------------------------------------
      /// \Brief		   Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<IAcquisitionRequester> m_acquisitionRequester;

      //--------------------------------------------------------------
      /// \Brief		   Keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<IKeywordRequester> m_keywordRequester;

      //--------------------------------------------------------------
      /// \Brief		   Keep the last date time used to compute summary day data
      //--------------------------------------------------------------
      bool m_firstRun;
   };

} //namespace common
} //namespace database 

