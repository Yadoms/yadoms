#pragma once
#include "IManager.h"
#include "IInterpreterLibrary.h"
#include "../../database/IAcquisitionRequester.h"
#include "../../database/IDeviceRequester.h"
#include "../../database/IRecipientRequester.h"
#include "../../communication/ISendMessageAsync.h"
#include "../../dataAccessLayer/IConfigurationManager.h"
#include "../../dataAccessLayer/IKeywordManager.h"
#include "IGeneralInfo.h"
#include <IPathProvider.h>
#include <shared/ILocation.h>

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script runner manager
      //-----------------------------------------------------
      class CManager : public IManager
      {
      public:
         //-----------------------------------------------------
         ///\brief               Constructor
         ///\param[in] pathProvider  Yadoms paths provider
         ///\param[in] pluginGateway Plugin access to do actions on plugins
         ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
         ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
         ///\param[in] dbAcquisitionRequester  Database acquisition requester
         ///\param[in] dbDeviceRequester  Database device requester
         ///\param[in] keywordAccessLayer  Database keyword access layer
         ///\param[in] dbRecipientRequester  Database recipient requester
         ///\param[in] location  The location provider
         //-----------------------------------------------------
         CManager(const IPathProvider& pathProvider,
                  boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                  boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                  boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                  boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                  boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                  boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                  boost::shared_ptr<shared::ILocation> location);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CManager();

      protected:
         // IManager Implementation
         std::vector<std::string> getAvailableInterpreters() override;
         boost::shared_ptr<shared::script::IInterpreter> getAssociatedInterpreter(const std::string& interpreterName) override;
         void unloadInterpreter(const std::string& interpreterName) override;
         boost::shared_ptr<IProperties> createScriptProperties(boost::shared_ptr<const database::entities::CRule> ruleData) override;
         std::string getScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData) override;
         std::string getScriptTemplateFile(const std::string& interpreterName) override;
         void updateScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code) override;
         void deleteScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, bool doBackup = true) override;
         std::string getScriptLogFile(boost::shared_ptr<const database::entities::CRule> ruleData) override;
         boost::shared_ptr<shared::process::ILogger> createScriptLogger(boost::shared_ptr<const database::entities::CRule> ruleData) override;
         boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> createScriptContext(boost::shared_ptr<shared::process::ILogger> scriptLogger) override;
         boost::shared_ptr<shared::process::IProcessObserver> createStopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler, int ruleId) override;
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
         std::vector<boost::filesystem::path> findInterpreterDirectories() const;

         //-----------------------------------------------------
         ///\brief               Get the full path of the script log file 
         /// \param[in] ruleId   Rule id
         ///\return              Full path of the script log file
         //-----------------------------------------------------
         boost::filesystem::path scriptLogFile(int ruleId) const;

      private:
         //-----------------------------------------------------
         ///\brief               The Yadoms paths provider
         //-----------------------------------------------------
         const IPathProvider& m_pathProvider;

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
         boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordAccessLayer;

         //-----------------------------------------------------
         ///\brief               Database recipient requester
         //-----------------------------------------------------
         boost::shared_ptr<database::IRecipientRequester> m_dbRecipientRequester;

         //-----------------------------------------------------
         ///\brief               List of loaded interpreters
         ///\details key is the library file name (without path and extension)
         //-----------------------------------------------------
         std::map<std::string, boost::shared_ptr<IInterpreterLibrary>> m_loadedInterpreters;

         //--------------------------------------------------------------
         /// \brief			      The interpreters map mutex
         //--------------------------------------------------------------
         mutable boost::recursive_mutex m_loadedInterpretersMutex;

         //-----------------------------------------------------
         ///\brief               General information requester
         //-----------------------------------------------------
         boost::shared_ptr<IGeneralInfo> m_generalInfo;
      };
   }
} // namespace automation::script


