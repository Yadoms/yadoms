#pragma once
#include "IFactory.h"
#include "IInterpreterLibrary.h"
#include "../../database/IAcquisitionRequester.h"
#include "../../communication/ISendMessageAsync.h"
#include <shared/notification/NotificationCenter.h>

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The script runner factory
   //-----------------------------------------------------
   class CFactory : public IFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] interpretersPath Path where are located interpreters
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      //-----------------------------------------------------
      CFactory(const std::string& interpretersPath,
         boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter,
         boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CFactory();

   protected:
      // IFactory Implementation
      virtual std::vector<std::string> getAvailableInterpreters();
      virtual boost::shared_ptr<IProperties> createScriptProperties(boost::shared_ptr<const database::entities::CRule> ruleData);
      virtual void createScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code);
      virtual boost::shared_ptr<shared::script::IRunner> createScriptRunner(boost::shared_ptr<const IProperties> scriptProperties);
      virtual boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> createScriptContext();
      // [END] IFactory Implementation

      //-----------------------------------------------------
      ///\brief               Load the interpreters
      //-----------------------------------------------------
      void loadInterpreters();

      //--------------------------------------------------------------
      /// \brief        Return full path of interpreter library, from interpreter name
      /// \param[in] interpreterName Interpreter name (ie "python")
      /// \return       Interpreter library full path (ie "scriptInterperters/python/python.dll")
      //--------------------------------------------------------------
      boost::filesystem::path toLibraryPath(const std::string& interpreterName) const;

      //--------------------------------------------------------------
      /// \brief        Returns all interpreter directories installed
      /// \return       a list of all found interpreter directories
      /// \note         This function just lists interpreter directory names.
      ///               It doesn't check if interpreter is valid (export expected functions)
      //--------------------------------------------------------------
      std::vector<boost::filesystem::path> findInterpreterDirectories();

      //-----------------------------------------------------
      ///\brief               Get the interpreter needed to run a script
      ///\param[in] interpreterName The interpreter name
      ///\return              The first interpreter found supporting this script
      ///\throw CScriptInterpreterNotFound No corresponding script interpreter was found
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::IInterpreter> getAssociatedInterpreter(const std::string& interpreterName);

   private:
      //-----------------------------------------------------
      ///\brief               Path where are located interpreters
      //-----------------------------------------------------
      const boost::filesystem::path m_interpretersPath;

      //-----------------------------------------------------
      ///\brief               The plugin access (to send commands to plugins)
      //-----------------------------------------------------
      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;

      //-----------------------------------------------------
      ///\brief               The notification center
      //-----------------------------------------------------
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;

      //-----------------------------------------------------
      ///\brief               Database acquisition requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;

      //-----------------------------------------------------
      ///\brief               List of loaded interpreters
      ///\details key is the library file name (without path and extension)
      //-----------------------------------------------------
      std::map<std::string, boost::shared_ptr<IInterpreterLibrary> > m_LoadedInterpreters;
   };

} } // namespace automation::script
	
	