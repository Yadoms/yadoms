#pragma once
#include <shared/script/IRunner.h>
#include <shared/shared/DataContainer.h>

namespace automation { namespace action { namespace script
{
   //-----------------------------------------------------
   ///\brief The script factory interface
   //-----------------------------------------------------
   class IFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IFactory() {}

      //-----------------------------------------------------
      ///\brief               Get available interpreters (as user-friendly names)
      ///\return              The list of available interpreters
      //-----------------------------------------------------
      virtual std::vector<std::string> getAvailableInterpreters() = 0;//TODO ajouter le REST permettant d'appeler cette fonction

      //-----------------------------------------------------
      ///\brief               Create a script runner
      ///\param[in] scriptName    Script name
      ///\param[in] configuration Script configuration
      ///\return              The script runner instance
      ///\throw CInvalidParameter if unable to create script runner
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::IRunner> createScriptRunner(const std::string& scriptName, const shared::CDataContainer& scriptConfiguration) = 0;
   };

} } } // namespace automation::action::script

