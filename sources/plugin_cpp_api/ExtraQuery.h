#pragma once
#include <toPlugin.pb.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>


namespace plugin_cpp_api
{
   //-----------------------------------------------------
   ///\brief The IExtraQuery implementation
   //-----------------------------------------------------
   class CExtraQuery : public shared::plugin::yPluginApi::IExtraQuery
   {
   public:
      explicit CExtraQuery(const toPlugin::ExtraQuery& msg);
      virtual ~CExtraQuery();

      // IExtraQuery implementation
      const std::string& getQuery() const override;
      const shared::CDataContainer& getData() const override;
      // [END] IExtraQuery implementation

   private:
      std::string m_query;
      shared::CDataContainer m_data;
   };
} // namespace plugin_cpp_api	


