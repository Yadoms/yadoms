#pragma once

#include "IAction.h"

namespace notification
{
   namespace action
   {
      //-----------------------------
      ///\brief Provide a notification using a function pointer
      ///\template T The notification content type
      //-----------------------------
      template <class T>
      class CFunctionPointerNotifier : public IAction<T>
      {
      public:
         //---------------------------------
         ///\brief Define a function prototype for updating a download progress
         //---------------------------------
         typedef boost::function1<void, boost::shared_ptr<T>> FunctionPtr;


         //-----------------------------
         ///\brief Constructor
         ///\param [in] functor  The function pointer
         //-----------------------------
         explicit CFunctionPointerNotifier(FunctionPtr functor)
            : m_functor(functor)
         {
         }

         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CFunctionPointerNotifier()
         {
         }

         //IAction<T> implementation
         void sendNotification(boost::shared_ptr<T> notification) override
         {
            m_functor(notification);
         }

         // [END] - IAction<T> implementation

      private:
         //-----------------------------
         ///\brief The function pointer
         //-----------------------------
         FunctionPtr m_functor;
      };
   } // namespace action
} // namespace notification


