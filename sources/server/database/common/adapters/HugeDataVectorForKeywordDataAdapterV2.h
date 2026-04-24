#pragma once

#include "IResultAdapter.h"
#include "database/common/IResultHandler.h"

namespace database::common::adapters
{
   //--------------------------------------------------------------
   ///\Brief		Class which adapt in single string, using a json like output [[data],[data],....]
   //--------------------------------------------------------------
   class CHugeDataVectorForKeywordDataAdapterV2 final : public IResultAdapterEx<std::string>
   {
   public:
      CHugeDataVectorForKeywordDataAdapterV2() = default;
      ~CHugeDataVectorForKeywordDataAdapterV2() override = default;

      CHugeDataVectorForKeywordDataAdapterV2(const CHugeDataVectorForKeywordDataAdapterV2&) = delete;
      CHugeDataVectorForKeywordDataAdapterV2(CHugeDataVectorForKeywordDataAdapterV2&&) = delete;
      CHugeDataVectorForKeywordDataAdapterV2& operator=(const CHugeDataVectorForKeywordDataAdapterV2&) = delete;
      CHugeDataVectorForKeywordDataAdapterV2& operator=(CHugeDataVectorForKeywordDataAdapterV2&&) = delete;

      // ISQLiteResultAdapter implementation
      bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override;
      std::vector<std::string> getResults() override;
      // [END] ISQLiteResultAdapter implementation

      //--------------------------------------------------------------
      /// \Brief		Get the result (raw format)
            /// \return		The result (raw format) using a json like output [[data],[data],....]
      //--------------------------------------------------------------
      std::string getRawResults() const;

   private:
      std::string m_internalValue;
   };
}
