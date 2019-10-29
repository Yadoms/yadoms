#pragma once
#include <string>

namespace notificationState
{
   class CNotificationPriority
   {
   public:
      enum EPriorityType
      {
         kInfo,
         kWarning,
         kCritical
      };

      static void getPriorityType(const EPriorityType priorityType, std::string& priorityMessage)
      {
         switch (priorityType)
         {
         case kInfo:
            priorityMessage = "info";
         case kWarning:
            priorityMessage = "warning";
            break;
         case kCritical:
            priorityMessage = "critical";
            break;
         default:
            priorityMessage = "";
         }
      }
   };

   class CNotificationIcon
   {
   public:
      enum EIconType
      {
         kNone,
         kInfo,
         kAlert
      };

      static void getIconType(const EIconType iconType, std::string& iconToDisplay)
      {
         switch (iconType)
         {
         case kNone:
            iconToDisplay = "none";
         case kInfo:
            iconToDisplay = "info";
            break;
         case kAlert:
            iconToDisplay = "alert";
            break;
         default:
            iconToDisplay = "none";
         }
      }
   };
}
