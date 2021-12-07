#pragma once
#include "IManager.h"
#include "IFactory.h"
#include <IPathProvider.h>

namespace automation
{
   namespace interpreter
   {
      //-----------------------------------------------------
      ///\brief The script runner manager
      //-----------------------------------------------------
      class CManager final : public IManager
      {
      public:
         //-----------------------------------------------------
         ///\brief               Constructor
         ///\param[in] pathProvider  Yadoms paths provider
         //-----------------------------------------------------
         explicit CManager(boost::shared_ptr<const IPathProvider> pathProvider);

         ~CManager() override;

      protected:
         // IManager Implementation
         std::vector<std::string> getLoadedInterpreters(bool loadIfNecessary = true) override;
         std::vector<std::string> getAvailablenterpreters(bool loadIfNecessary = true) override;
         std::map<std::string, bool> getInterpreters(bool includeNotAvailable,
                                                     bool loadIfNecessary = true) override;
         std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>> getAvailableInterpretersInformation() override;
         std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>> getLoadedInterpretersInformation() override;
         boost::shared_ptr<IInstance> getAvailableInterpreterInstance(const std::string& interpreterType) override;
         boost::shared_ptr<IInstance> getLoadedInterpreterInstance(const std::string& interpreterType) override;
         void unloadInterpreter(const std::string& interpreterType) override;
         void onInterpreterUnloaded(const std::string& interpreterType);
         std::string getScriptContent(const std::string& interpreterType,
                                      const std::string& scriptPath) override;
         std::string getScriptTemplateContent(const std::string& interpreterType) override;
         boost::filesystem::path getScriptLogFilename(int ruleId) const override;
         void updateScriptFile(const std::string& interpreterType,
                               const std::string& scriptPath,
                               const std::string& code) override;
         void deleteScriptFile(const std::string& interpreterType,
                               const std::string& scriptPath,
                               bool doBackup = true) override;
         std::string getScriptLogContent(int ruleId) override;
         void deleteLog(int ruleId) override;
         void setOnScriptStoppedFct(boost::function2<void, int, const std::string&> onScriptStoppedFct) override;
         // [END] IManager Implementation

      protected:
         //-----------------------------------------------------
         ///\brief               Load/Unload the interpreters
         //-----------------------------------------------------
         void loadInterpreters();
         void unloadInterpreters();

         //-----------------------------------------------------
         /// \brief              Check platform compatibility for interpreter
         /// \param[in] interpreterType Interpreter type (ie "python")
         /// \return true if compatible, false if not
         /// \throw shared::exception::CInvalidParameter if fails to determine compatibility
         //-----------------------------------------------------
         bool isInterpreterCompatibleWithPlatform(const std::string& interpreterType) const;

         //--------------------------------------------------------------
         /// \brief        Returns all interpreter directories installed
         /// \return       a list of all found interpreter directories
         /// \note         This function just lists interpreter directory names.
         ///               It does not check if interpreter is valid (export expected functions)
         //--------------------------------------------------------------
         std::vector<boost::filesystem::path> findInterpreterDirectories() const;

      private:
         //-----------------------------------------------------
         ///\brief               The Yadoms paths provider
         //-----------------------------------------------------
         boost::shared_ptr<const IPathProvider> m_pathProvider;

         //-----------------------------------------------------
         ///\brief               The Interpreters factory
         //-----------------------------------------------------
         boost::shared_ptr<IFactory> m_factory;

         //-----------------------------------------------------
         ///\brief               List of loaded interpreters
         ///\details key is the interpreter type (= directory name = interpreter file name without extension)
         //-----------------------------------------------------
         std::map<std::string, boost::shared_ptr<IInstance>> m_loadedInterpreters;

         //--------------------------------------------------------------
         /// \brief			      The interpreters map mutex
         //--------------------------------------------------------------
         mutable boost::recursive_mutex m_loadedInterpretersMutex;

         //--------------------------------------------------------------
         /// \brief			      Functor to call when rule is notified as stopped
         //--------------------------------------------------------------
         boost::function2<void, int, const std::string&> m_onScriptStoppedFct;
      };
   }
} // namespace automation::interpreter
