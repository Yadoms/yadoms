#pragma once

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The rules state management interface
   //-----------------------------------------------------
   class IInstanceStateHandler
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Type of instance manager event
      //--------------------------------------------------------------
      enum EEventType//TODO conserver ?
      {
         kAbnormalStopped = 0,      // Instance abnormal stopped
         kStopped
      };

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IInstanceStateHandler() {}

      //-----------------------------------------------------
      ///\brief               Signal a normal stop
      //-----------------------------------------------------
      virtual void signalNormalStop() = 0;

      //-----------------------------------------------------
      ///\brief               Signal an error
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void signalError(const std::string& error) = 0;

      //-----------------------------------------------------
      ///\brief               Signal error when starting instance
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void signalStartError(const std::string& error) = 0;
   };
	
} // namespace pluginSystem	
	
	