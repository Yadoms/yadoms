#pragma once
#include "IAcquisitionHistorizer.h"
#include "database/IDataProvider.h"

namespace dataAccessLayer
{

   class CAcquisitionHistorizer : public IAcquisitionHistorizer
   {
   public:
      explicit CAcquisitionHistorizer(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CAcquisitionHistorizer() = default;

      void saveData(int keywordId,
         const shared::plugin::yPluginApi::historization::IHistorizable & data) override;
      void saveData(std::vector<int> KeywordIdVect,
         const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect) override;
      void saveData(int keywordId,
         const shared::plugin::yPluginApi::historization::IHistorizable & data,
         boost::posix_time::ptime & dataTime) override;

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
   };
 
} //namespace dataAccessLayer 
