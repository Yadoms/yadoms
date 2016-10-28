#pragma once
#include <shared/plugin/yPluginApi/IExtraQueryData.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IExtraQueryData implementation
   //-----------------------------------------------------
   class CExtraQueryData : public shared::plugin::yPluginApi::IExtraQueryData
   {
   public:
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] query              The query
      //-----------------------------------------------------
      explicit CExtraQueryData(const std::string& query,
                               const shared::CDataContainer& data);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExtraQueryData();

      // IExtraQueryData implementation
      const std::string& query() const override;
      const shared::CDataContainer& data() const override;
      // [END] IExtraQueryData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Query
      //-----------------------------------------------------
      std::string m_query;

      //-----------------------------------------------------
      ///\brief               Data
      //-----------------------------------------------------
      shared::CDataContainer m_data;
   };
} // namespace pluginSystem	


