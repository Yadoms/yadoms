#pragma once

namespace automation
{
   //-----------------------------------------------------
   ///\brief A plugin instance
   //-----------------------------------------------------
   class IRuleStartErrorObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRuleStartErrorObserver() {}

      //-----------------------------------------------------
      ///\brief               Signal that rule failed to start
      //\param[in] error      Error description
      //-----------------------------------------------------
      virtual void notifyStartError(const std::string& error) = 0;
   };

} // namespace automation	

