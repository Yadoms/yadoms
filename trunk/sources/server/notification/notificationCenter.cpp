#include "stdafx.h"
#include "notificationCenter.h"
#include "acquisition/notifier.h"
#include "newDevice/notifier.h"
#include "configurationUpdate/notifier.h"
#include "taskProgression/notifier.h"

namespace notification
{
CNotificationCenter::CNotificationCenter()
   :m_acquisitionNotifier(new acquisition::CNotifier),
   m_newDeviceNotifier(new newDevice::CNotifier),
   m_configurationUpdateNotifier(new configurationUpdate::CNotifier),
   m_taskProgressionNotifier(new taskProgression::CNotifier)
{
}

CNotificationCenter::~CNotificationCenter()
{
}

boost::shared_ptr<acquisition::INotifier> CNotificationCenter::acquisitionNotifier()
{
   return m_acquisitionNotifier;
}

boost::shared_ptr<newDevice::INotifier> CNotificationCenter::newDeviceNotifier()
{
   return m_newDeviceNotifier;
}

boost::shared_ptr<configurationUpdate::INotifier> CNotificationCenter::configurationUpdateNotifier()
{
   return m_configurationUpdateNotifier;
}

boost::shared_ptr<taskProgression::INotifier> CNotificationCenter::taskProgressionNotifier()
{
   return m_taskProgressionNotifier;
}

} // namespace notification
