#pragma once
#include "acquisition/INotifier.h"
#include "newDevice/INotifier.h"
#include "configurationUpdate/INotifier.h"
#include "taskProgression/INotifier.h"

namespace notification
{
   //-----------------------------------------------------
   ///\brief The notification center interface
   //-----------------------------------------------------
   class INotificationCenter
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~INotificationCenter() {}

      //-----------------------------------------------------
      ///\brief               Get the acquisition notifier
      ///\return              The acquisition notifier
      //-----------------------------------------------------
      virtual boost::shared_ptr<acquisition::INotifier> acquisitionNotifier() = 0;

      //-----------------------------------------------------
      ///\brief               Get the "new device" notifier
      ///\return              The "new device" notifier
      //-----------------------------------------------------
      virtual boost::shared_ptr<newDevice::INotifier> newDeviceNotifier() = 0;

      //-----------------------------------------------------
      ///\brief               Get the configuration update notifier
      ///\return              The configuration update notifier
      //-----------------------------------------------------
      virtual boost::shared_ptr<configurationUpdate::INotifier> configurationUpdateNotifier() = 0;

      //-----------------------------------------------------
      ///\brief               Get the task progression notifier
      ///\return              The task progression notifier
      //-----------------------------------------------------
      virtual boost::shared_ptr<taskProgression::INotifier> taskProgressionNotifier() = 0;
   };

} // namespace notification
