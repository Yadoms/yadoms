#pragma once
#include "IRunnerFactory.h"
#include "IInterpreterLibrary.h"

namespace automation { namespace action { namespace script
{
   //-----------------------------------------------------
   ///\brief The script runner factory
   //-----------------------------------------------------
   class CRunnerFactory : public IRunnerFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] interpretersPath Path where are located interpreters
      //-----------------------------------------------------
      CRunnerFactory(const std::string& interpretersPath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRunnerFactory();

   protected:
      // IRunnerFactory Implementation
      virtual boost::shared_ptr<shared::script::IRunner> createScriptRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration);
      // [END] IRunnerFactory Implementation

      //-----------------------------------------------------
      ///\brief               Load the interpreters
      //-----------------------------------------------------
      void loadInterpreters();

      //--------------------------------------------------------------
      /// \brief        Returns all interpreter directories installed
      /// \return       a list of all found interpreter directories
      /// \note         This function just lists interpreter directory names.
      ///               It doesn't check if interpreter is valid (export expected functions)
      //--------------------------------------------------------------
      std::vector<boost::filesystem::path> findInterpreterDirectories();

      //-----------------------------------------------------
      ///\brief               Get the interpreter needed to run a script
      ///\param[in] scriptPath Script full path
      ///\return              The first interpreter found supporting this script
      ///\throw CScriptInterpreterNotFound No corresponding script interpreter was found
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::IInterpreter> getScriptInterpreter(const std::string& scriptPath);

   private:
      //-----------------------------------------------------
      ///\brief               Path where are located interpreters
      //-----------------------------------------------------
      const boost::filesystem::path m_interpretersPath;

      //-----------------------------------------------------
      ///\brief               List of loaded interpreters
      ///\details key is the library file name (without path and extension)
      //-----------------------------------------------------
      std::map<std::string, boost::shared_ptr<IInterpreterLibrary> > m_LoadedInterpreters;
   };

} } } // namespace automation::action::script
	
	