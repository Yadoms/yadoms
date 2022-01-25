#pragma once
#include "IAcquisitionHistorizer.h"
#include "database/IDataProvider.h"
#include "observers/IAcquisitionObservers.h"

namespace dataAccessLayer
{
   class CAcquisitionHistorizer final : public IAcquisitionHistorizer
   {
   public:
      explicit CAcquisitionHistorizer(boost::shared_ptr<database::IDataProvider> dataProvider);
      ~CAcquisitionHistorizer() override = default;

      void saveData(int keywordId,
                    const shared::plugin::yPluginApi::historization::IHistorizable& data) override;
      void saveData(std::vector<int> keywordIdVect,
                    const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& dataVect) override;
      void saveData(int keywordId,
                    const shared::plugin::yPluginApi::historization::IHistorizable& data,
                    boost::posix_time::ptime& dataTime) override;
      boost::shared_ptr<observers::IAcquisitionObservers> acquisitionObservers() override;

   private:
      void postNotification(boost::shared_ptr<database::entities::CAcquisition> acquisition) const;
      void postNotification(const std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary>>& acquisitionSummary);

      boost::shared_ptr<database::IDataProvider> m_dataProvider;

      boost::shared_ptr<observers::IAcquisitionObservers> m_acquisitionObservers;
   };
} //namespace dataAccessLayer 
