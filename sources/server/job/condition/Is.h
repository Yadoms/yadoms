#pragma once
#include "ICondition.h"
#include <shared/shared/DataContainer.h>

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The IS operator
   //-----------------------------------------------------
   class CIs : public ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Condition configuration node
      //-----------------------------------------------------
      CIs(const shared::CDataContainer& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CIs();

   protected:
      // ICondition Implementation
      virtual void wait() const;
      // [END] ICondition Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Keyword ID
      //-----------------------------------------------------
      const int m_keywordId;//TODO à conserver ?

      //-----------------------------------------------------
      ///\brief               Expected state
      //-----------------------------------------------------
      const std::string m_expectedState;
   };
	
} } // namespace job::condition	
	
	