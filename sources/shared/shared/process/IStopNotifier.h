#pragma once

//TODO virer
namespace shared { namespace process
{
   //-----------------------------------------------------
   ///\brief The process stop notifier interface
   //-----------------------------------------------------
   class IStopNotifier
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IStopNotifier() {}

      //-----------------------------------------------------
      ///\brief               Signal a normal stop of the process
      //-----------------------------------------------------
      virtual void notifyNormalStop() = 0;

      //-----------------------------------------------------
      ///\brief               Signal an error on a process
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void notifyError(const std::string& error) = 0;

      //-----------------------------------------------------
      ///\brief               Signal error on starting process
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void notifyStartError(const std::string& error) = 0;
   };
	
} } // namespace shared::process
	
	