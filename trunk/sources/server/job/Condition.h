#pragma once
#include "ICondition.h"
#include <shared/shared/DataContainer.h>

namespace job
{
   //-----------------------------------------------------
   ///\brief A job
   //-----------------------------------------------------
   class CCondition : public ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Condition configuration
      //-----------------------------------------------------
      CCondition(const shared::CDataContainer& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CCondition();

   protected:
      // ICondition Implementation
      virtual void wait() const;
      // [END] ICondition Implementation

   private:
   };
	
} // namespace job	
	
	