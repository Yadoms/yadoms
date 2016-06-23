#pragma once
#include <shared/plugin/yPluginApi/IExtraCommand.h>


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IExtraCommand implementation
   //-----------------------------------------------------
   class CExtraCommand : public shared::plugin::yPluginApi::IExtraCommand
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] command      The extra command
      ///\param[in] data         The optional command data
      //-----------------------------------------------------
      explicit CExtraCommand(const std::string& command,
                             const shared::CDataContainer& data = shared::CDataContainer::EmptyContainer);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExtraCommand();

      // IExtraCommand implementation
      virtual const std::string& getCommand() const;
      virtual const shared::CDataContainer& getData() const;
      // [END] IExtraCommand implementation

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


