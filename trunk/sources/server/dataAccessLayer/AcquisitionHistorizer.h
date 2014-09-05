#pragma once
#include "IAcquisitionHistorizer.h"
#include "database/IAcquisitionRequester.h"

namespace dataAccessLayer {

   class CAcquisitionHistorizer : public IAcquisitionHistorizer
   {
   public:
      CAcquisitionHistorizer(boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester);

      virtual void saveData(const int keywordId, const std::string & data);
      virtual void saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime);

   private:
      boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
   };
 
} //namespace dataAccessLayer 
