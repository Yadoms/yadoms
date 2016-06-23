#pragma once

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief A plugin instance
   //-----------------------------------------------------
   class IInstanceStartErrorObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IInstanceStartErrorObserver() {}

      //-----------------------------------------------------
      ///\brief               Signal that instance failed to start
      //\param[in] error      Error description
      //-----------------------------------------------------
      virtual void signalStartError(const std::string& error) = 0;
   };

} // namespace pluginSystem	

