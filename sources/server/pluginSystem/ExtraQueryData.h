#pragma once
#include <shared/plugin/yPluginApi/IExtraQueryData.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IExtraQueryData implementation
   //-----------------------------------------------------
   class CExtraQueryData final : public shared::plugin::yPluginApi::IExtraQueryData
   {
   public:
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] query              The query
      ///\param[in] data               Query data
      ///\param[in] deviceId           Device ID to apply query
      //-----------------------------------------------------
      CExtraQueryData(std::string query,
                      boost::shared_ptr<shared::CDataContainer> data,
                      std::string deviceId);
      ~CExtraQueryData() override = default;

      // IExtraQueryData implementation
      const std::string& query() const override;
      const boost::shared_ptr<shared::CDataContainer>& data() const override;
      const std::string& device() const override;
      // [END] IExtraQueryData implementation

   private:
      std::string m_query;
      boost::shared_ptr<shared::CDataContainer> m_data;
      std::string m_deviceId;
   };
} // namespace pluginSystem	
