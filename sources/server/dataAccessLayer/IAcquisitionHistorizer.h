#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>

namespace dataAccessLayer
{
   class IAcquisitionHistorizer
   {
   public:
      virtual ~IAcquisitionHistorizer() = default;

      //--------------------------------------------------------------
      /// \brief           Save a new data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      data        The data
      //--------------------------------------------------------------
      virtual void saveData(int keywordId,
                            const shared::plugin::yPluginApi::historization::IHistorizable& data) = 0;

      //--------------------------------------------------------------
      /// \brief           Save a new list of data into base
      /// \param [in]      KeywordIdVect   The list of keyword id
      /// \param [in]      dataVect        The list of data
      //--------------------------------------------------------------
      virtual void saveData(std::vector<int> KeywordIdVect,
                            const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& dataVect) = 0;

      //--------------------------------------------------------------
      /// \brief           Save a new data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      data        The data
      /// \param [in]      dataTime    The datetime of the data
      //--------------------------------------------------------------
      virtual void saveData(int keywordId,
                            const shared::plugin::yPluginApi::historization::IHistorizable& data,
                            boost::posix_time::ptime& dataTime) = 0;
   };
} //namespace dataAccessLayer 
