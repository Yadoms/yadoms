#pragma once
#include "INotificationCenter.h"

namespace notification
{
   //-----------------------------------------------------
   ///\brief The notification center
   //-----------------------------------------------------
   class CNotificationCenter : public INotificationCenter
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CNotificationCenter();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CNotificationCenter();

      // INotificationCenter implementation
      virtual boost::shared_ptr<acquisition::INotifier> acquisitionNotifier();
      virtual boost::shared_ptr<newDevice::INotifier> newDeviceNotifier();
      virtual boost::shared_ptr<configurationUpdate::INotifier> configurationUpdateNotifier();
      virtual boost::shared_ptr<taskProgression::INotifier> taskProgressionNotifier();
      // [END] INotificationCenter implementation

   protected:
      //-----------------------------------------------------
      ///\brief               The notifiers
      //-----------------------------------------------------
      boost::shared_ptr<acquisition::INotifier> m_acquisitionNotifier;
      boost::shared_ptr<newDevice::INotifier> m_newDeviceNotifier;
      boost::shared_ptr<configurationUpdate::INotifier> m_configurationUpdateNotifier;
      boost::shared_ptr<taskProgression::INotifier> m_taskProgressionNotifier;
   };

} // namespace notification