#pragma once
#include "IAction.h"
#include <server/communication/ISendMessageAsync.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief A simple waiter
   //-----------------------------------------------------
   class CWait : public IAction
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Action configuration
      //-----------------------------------------------------
      CWait(const shared::CDataContainer& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CWait();

   protected:
      // IAction Implementation
      virtual void doAction() const;
      // [END] IAction Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Delay to wait
      //-----------------------------------------------------
      boost::posix_time::time_duration m_delay;
   };
	
} } // namespace automation::action
	
	