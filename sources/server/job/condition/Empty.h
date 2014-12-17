#pragma once
#include "ICondition.h"
#include <shared/shared/DataContainer.h>

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief A condition
   //-----------------------------------------------------
   class CEmpty : public ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CEmpty();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CEmpty();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      // [END] ICondition Implementation
   };
	
} } // namespace job::condition	
	
	