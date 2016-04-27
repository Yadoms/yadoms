#pragma once
#include <shared/plugin/yPluginApi/IBindingQueryData.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IBindingQueryData implementation
   //-----------------------------------------------------
   class CBindingQueryData : public shared::plugin::yPluginApi::IBindingQueryData
   {
   public:
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] query              The query
      //-----------------------------------------------------
      explicit CBindingQueryData(const std::string& query);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CBindingQueryData();

      // IBindingQueryData implementation
      virtual const std::string& getQuery() const;
      // [END] IBindingQueryData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Query
      //-----------------------------------------------------
      std::string m_query;
   };  
} // namespace pluginSystem	
	
	