#pragma once
#include "IManager.h"
#include "IInterpreterLibrary.h"
#include "../../database/IAcquisitionRequester.h"
#include "../../database/IDeviceRequester.h"
#include "../../database/IKeywordRequester.h"
#include "../../database/IRecipientRequester.h"
#include "../../communication/ISendMessageAsync.h"
#include "notification/NotificationCenter.h"
#include "../../dataAccessLayer/IConfigurationManager.h"
#include "IGeneralInfo.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The script runner manager
   //-----------------------------------------------------
   class CManager : public IManager
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] interpretersPath Path where are located interpreters
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\param[in] dbDeviceRequester  Database device requester
      ///\param[in] dbKeywordRequester  Database keyword requester
      ///\param[in] dbRecipientRequester  Database recipient requester
      //-----------------------------------------------------
      CManager(const std::string& interpretersPath,
         boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
         boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
         boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
         boost::shared_ptr<database::IKeywordRequester> dbKeywordRequester,
         boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CManager();

   protected:
      // IManager Implementation
      virtual std::vector<std::string> getAvailableInterpreters();
      virtual void unloadInterpreter(const std::string& interpreterName);
      virtual boost::shared_ptr<IProperties> createScriptProperties(boost::shared_ptr<const database::entities::CRule> ruleData);
      virtual std::string getScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData);
      virtual std::string getScriptTemplateFile(const std::string& interpreterName);
      virtual void updateScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code);
      virtual void deleteScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, bool doBackup = true);
      virtual std::string getScriptLogFile(boost::shared_ptr<const database::entities::CRule> ruleData);
      virtual boost::shared_ptr<shared::process::IRunner> createScriptRunner(
         boost::shared_ptr<const IProperties> scriptProperties,
         boost::shared_ptr<shared::process::ILogger> scriptLogger,
         boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
         boost::shared_ptr<shared::process::IStopNotifier> stopNotifier);
      virtual boost::shared_ptr<shared::process::ILogger> createScriptLogger(const std::string& scriptPath);
      virtual boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> createScriptContext(boost::shared_ptr<shared::process::ILogger> scriptLogger);
      virtual boost::shared_ptr<shared::process::IStopNotifier> createStopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler, int ruleId);
      // [END] IManager Implementation

      //-----------------------------------------------------
      ///\brief               Load the interpreters
      //-----------------------------------------------------
      void loadInterpreters();
      
      //-----------------------------------------------------
      /// \brief              Check platform compatibility for interpreter
      /// \param[in] interpreterName Interpreter name (ie "python")
      /// \return true if compatible, false if not
      /// \throw shared::exception::CInvalidParameter if fails to determine compatibility
      //-----------------------------------------------------
      bool isInterpreterCompatibleWithPlatform(const std::string& interpreterName) const;

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
      ///\brief               The Yadoms configuration manager access
      //-----------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;

      //-----------------------------------------------------
      ///\brief               Database acquisition requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;

      //-----------------------------------------------------
      ///\brief               Database device requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IDeviceRequester> m_dbDeviceRequester;

      //-----------------------------------------------------
      ///\brief               Database keyword requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IKeywordRequester> m_dbKeywordRequester;

      //-----------------------------------------------------
      ///\brief               Database recipient requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IRecipientRequester> m_dbRecipientRequester;

      //-----------------------------------------------------
      ///\brief               List of loaded interpreters
      ///\details key is the library file name (without path and extension)
      //-----------------------------------------------------
      std::map<std::string, boost::shared_ptr<IInterpreterLibrary> > m_loadedInterpreters;

      //--------------------------------------------------------------
      /// \brief			      The interpreters map mutex
      //--------------------------------------------------------------
      mutable boost::recursive_mutex m_loadedInterpretersMutex;

      //-----------------------------------------------------
      ///\brief               General information requester
      //-----------------------------------------------------
      boost::shared_ptr<IGeneralInfo> m_generalInfo;
   };

} } // namespace automation::script
	
	