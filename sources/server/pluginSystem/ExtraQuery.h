#pragma once
#include <shared/plugin/yPluginApi/IExtraQuery.h>


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IExtraQuery implementation
   //-----------------------------------------------------
   class CExtraQuery : public shared::plugin::yPluginApi::IExtraQuery
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] command      The extra command
      ///\param[in] data         The optional command data
      //-----------------------------------------------------
      explicit CExtraQuery(const std::string& command,
                             const shared::CDataContainer& data = shared::CDataContainer::EmptyContainer);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExtraQuery();

      // IExtraQuery implementation
      virtual const std::string& getQuery() const;
      virtual const shared::CDataContainer& getData() const;
      // [END] IExtraQuery implementation

   private:
      //-----------------------------------------------------
      ///\brief               Command
      //-----------------------------------------------------
      std::string m_command;

      //-----------------------------------------------------
      ///\brief               Command data
      //-----------------------------------------------------
      shared::CDataContainer m_data;
   };
} // namespace pluginSystem	


