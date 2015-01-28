#include "stdafx.h"
#include "../../../../../sources/server/database/IAcquisitionRequester.h"


class CDatabaseAcquisitionRequesterMock : public database::IAcquisitionRequester
{
public:
   virtual boost::shared_ptr<database::entities::CAcquisition> saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime) { return boost::shared_ptr<database::entities::CAcquisition>(); }
   virtual boost::shared_ptr<database::entities::CAcquisition> incrementData(const int keywordId, const std::string & increment, boost::posix_time::ptime & dataTime) { return boost::shared_ptr<database::entities::CAcquisition>(); }
   virtual LastSummaryData saveSummaryData(const int keywordId, boost::posix_time::ptime & dataTime) { return LastSummaryData(); }
   virtual void removeKeywordData(const int keywordId) {}
   virtual boost::shared_ptr<database::entities::CAcquisition> getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time) { return boost::shared_ptr<database::entities::CAcquisition>(); }
   virtual boost::shared_ptr<database::entities::CAcquisition> getKeywordLastData(const int keywordId)
   {
      boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition());
      acq->KeywordId = keywordId;
      acq->Value = m_value;
      return acq;
   }
   virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string> > getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::vector< boost::tuple<boost::posix_time::ptime, std::string> >(); }
   virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> >(); }
   virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> >(); }
   virtual std::string getKeywordHighchartData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::string(); }
   virtual std::string getKeywordHighchartDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::string(); }
   virtual std::string getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::string(); }
   std::string m_value;
};


class NotificationObserverForRulesManagerMock : public automation::INotificationObserverForRulesManager
{
public:
   virtual ~NotificationObserverForRulesManagerMock() {}
   virtual void registerKeywordUpdater(boost::shared_ptr<automation::condition::IKeywordUpdater> keywordUpdater, boost::shared_ptr<automation::condition::IConditionRootUpdater> conditionRootNotifier){}
   virtual void unregisterKeywordUpdater(boost::shared_ptr<automation::condition::IKeywordUpdater> keywordUpdater){}
};


class ConditionRootUpdaterMock : public automation::condition::IConditionRootUpdater
{
public:
   ConditionRootUpdaterMock() :m_wasRequested(false) {}
   virtual ~ConditionRootUpdaterMock() {}
   virtual void requestEvaluation() { m_wasRequested = true; }
   bool wasRequested() { bool w = m_wasRequested; m_wasRequested = false; return w; }
private:
   bool m_wasRequested;
};