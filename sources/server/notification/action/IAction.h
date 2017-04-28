#pragma once

namespace notification
{
   namespace action
   {
      //-----------------------------
      ///\brief Interface for notification Action
      ///\template T The notification content type
      //-----------------------------
      template <class T>
      class IAction
      {
      public:
         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~IAction()
         {
         }

         //-----------------------------
         ///\brief Send notification
         ///\param [in] notification  The notification to send
         //-----------------------------
         virtual void sendNotification(boost::shared_ptr<T> notification) = 0;
      };
   } // namespace action
} // namespace notification


