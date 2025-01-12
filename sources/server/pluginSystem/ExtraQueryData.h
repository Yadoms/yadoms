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
      ///\param data
      ///\param deviceId
      //-----------------------------------------------------
      explicit CExtraQueryData(const std::string& query,
                               const boost::shared_ptr<shared::CDataContainer>& data,
                               const std::string& deviceId);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExtraQueryData();

      // IExtraQueryData implementation
      const std::string& query() const override;
      const boost::shared_ptr<shared::CDataContainer>& data() const override;
      const std::string& device() const override;
      // [END] IExtraQueryData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Query
      //-----------------------------------------------------
      std::string m_query;

      //-----------------------------------------------------
      ///\brief               Data
      //-----------------------------------------------------
      boost::shared_ptr<shared::CDataContainer> m_data;

      //-----------------------------------------------------
      ///\brief               Query for devices ?
      //-----------------------------------------------------
      std::string m_device;
   };
} // namespace pluginSystem	


