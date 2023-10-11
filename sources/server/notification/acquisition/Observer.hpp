#pragma once

#include "../IObserver.h"
#include "../action/IAction.h"
#include "Notification.hpp"
#include "../change/Observer.hpp"

namespace notification
{
   namespace acquisition
   {
      //-----------------------------
      ///\brief Observer for acquisition
      //-----------------------------
      class CObserver final : public change::CObserver<CNotification>
      {
      public:
         //-----------------------------
         ///\brief Create an observer for any new acquisition on all keywords
         ///\param [in] action The action (what to do when the good notification appears)
         //-----------------------------
         explicit CObserver(const boost::shared_ptr<action::IAction<CNotification>>& action)
            : change::CObserver<CNotification>(change::EChangeType::kCreate,
                                               action)
         {
         }

         CObserver(const CObserver&) = delete;
         CObserver(const CObserver&&) = delete;
         CObserver operator=(CObserver&) = delete;
         CObserver operator=(CObserver&&) = delete;

         ~CObserver() override = default;

         //-----------------------------
         ///\brief Add a keyword id to filter
         ///\param [in] keywordId The keyword id to filter
         //-----------------------------
         void addKeywordIdFilter(const int keywordId)
         {
            //search the keywordId in list
            const auto findIterator = std::find(m_allowedKeywordId.begin(), m_allowedKeywordId.end(),
                                                keywordId);

            //if keyword is not found, add it to filter
            if (findIterator == m_allowedKeywordId.end())
               m_allowedKeywordId.push_back(keywordId);
         }

         //-----------------------------
         ///\brief Reset the filter with a new filter list
         ///\param [in] keywordsToListen The list of all keyword id to filter (if empty, all keywords are filtered)
         //-----------------------------
         void resetKeywordIdFilter(const std::vector<int>& keywordsToListen)
         {
            clearKeywordIdFilter();
            for (const int it : keywordsToListen)
               addKeywordIdFilter(it);
         }
         //-----------------------------
         ///\brief Reset the filter with a new filter list
         ///\param [in] keywordsToListen The list of all keyword id to filter (if empty, all keywords are filtered)
         //-----------------------------
         void resetKeywordIdFilter(const std::set<int>& keywordsToListen)
         {
            clearKeywordIdFilter();
            for (const int it : keywordsToListen)
               addKeywordIdFilter(it);
         }

         //-----------------------------
         ///\brief Delete a keyword id to filter
         ///\param [in] keywordId The keyword id to remove from filter
         //-----------------------------
         void removeKeywordIdFilter(const int keywordId)
         {
            //search the keywordId in list
            const auto findIterator = std::find(m_allowedKeywordId.begin(),
                                                m_allowedKeywordId.end(),
                                                keywordId);

            //if keywordId is found, remove it
            if (findIterator != m_allowedKeywordId.end())
               m_allowedKeywordId.erase(findIterator);
         }

         //-----------------------------
         ///\brief Clear the filter (all keyword will be observed)
         //-----------------------------
         void clearKeywordIdFilter()
         {
            m_allowedKeywordId.clear();
         }


         //IObserver implementation
         void observe(const boost::shared_ptr<INotification> n) override
         {
            //check notification is good type
            const auto notif = boost::dynamic_pointer_cast<CNotification>(n);
            if (notif)
            {
               //check keyword id is not filtered
               //empty allowedKeywordId => all keywords are allowed
               if (m_allowedKeywordId.empty() || std::find(m_allowedKeywordId.begin(),
                                                           m_allowedKeywordId.end(),
                                                           notif->getAcquisition()->KeywordId()) != m_allowedKeywordId.end())
               {
                  //do notification
                  m_action->sendNotification(notif);
               }
            }
         }

         // [END] - IObserver implementation

      private:
         std::vector<int> m_allowedKeywordId;
      };
   } // namespace acquisition
} // namespace notification
