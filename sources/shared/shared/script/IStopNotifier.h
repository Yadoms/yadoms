#pragma once

namespace shared { namespace script
{
   //-----------------------------------------------------
   ///\brief The script stop notifier interface
   //-----------------------------------------------------
   class IStopNotifier
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IStopNotifier() {}

      //TODO faire le tri des méthodes utiles
      //-----------------------------------------------------
      ///\brief               Signal a normal stop on a rule
      //-----------------------------------------------------
      virtual void notifyNormalStop() = 0;

      //-----------------------------------------------------
      ///\brief               Signal a normal stop on a rule, and disable it
      //-----------------------------------------------------
      virtual void notifyNormalStopAndDisable() = 0;

      //-----------------------------------------------------
      ///\brief               Signal an error on a rule
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void notifyError(const std::string& error) = 0;

      //-----------------------------------------------------
      ///\brief               Signal error on starting rules
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void notifyStartError(const std::string& error) = 0;
   };
	
} } // namespace shared::script
	
	