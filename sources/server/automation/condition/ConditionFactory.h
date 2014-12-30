#pragma once
#include "IConditionFactory.h"
#include <shared/DataContainer.h>
#include "../../database/IAcquisitionRequester.h"

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief The condition factory
   //-----------------------------------------------------
   class CConditionFactory : public IConditionFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      //-----------------------------------------------------
      CConditionFactory(boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CConditionFactory();

   protected:
      // IConditionFactory Implementation
      virtual boost::shared_ptr<ICondition> createCondition(const shared::CDataContainer& configuration) const;
      // [END] IConditionFactory Implementation

      //-----------------------------------------------------
      ///\brief               Database acquisition requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;
   };
	
} } // namespace automation::condition	
	
	