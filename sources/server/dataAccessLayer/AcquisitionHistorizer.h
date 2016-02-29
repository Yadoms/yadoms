#pragma once
#include "IAcquisitionHistorizer.h"
#include "database/IDataProvider.h"

namespace dataAccessLayer
{

   class CAcquisitionHistorizer : public IAcquisitionHistorizer
   {
   public:
      explicit CAcquisitionHistorizer(boost::shared_ptr<database::IDataProvider> dataProvider);

      virtual void saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data);
      virtual void saveData(std::vector<int> KeywordIdVect, const std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect);
      virtual void saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data, boost::posix_time::ptime & dataTime);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
   };
 
} //namespace dataAccessLayer 
