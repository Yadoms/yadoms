#pragma once
#include "IAction.h"
#include <server/communication/ISendMessageAsync.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief A action running a script
   //-----------------------------------------------------
   class CRunScript : public IAction
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Action configuration
      //-----------------------------------------------------
      CRunScript(const shared::CDataContainer& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRunScript();

   protected:
      // IAction Implementation
      virtual void doAction() const;
      // [END] IAction Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The script path
      //-----------------------------------------------------
      const std::string m_scriptPath;
   };
	
} } // namespace automation::action
	
	