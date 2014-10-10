#pragma once
#include <shared/plugin/yadomsApi/historization/IHistorizable.h>

namespace dataAccessLayer {

   class IAcquisitionHistorizer
   {
   public:
      virtual ~IAcquisitionHistorizer()
      {
      }

      //--------------------------------------------------------------
      /// \brief           Save a new data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      data        The data
      //--------------------------------------------------------------
      virtual void saveData(const int keywordId, const shared::plugin::yadomsApi::historization::IHistorizable & data) = 0;

      //--------------------------------------------------------------
      /// \brief           Save a new data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      data        The data
      /// \param [in]      dataTime    The datetime of the data
      //--------------------------------------------------------------
      virtual void saveData(const int keywordId, const shared::plugin::yadomsApi::historization::IHistorizable & data, boost::posix_time::ptime & dataTime) = 0;
   };
 
} //namespace dataAccessLayer 
