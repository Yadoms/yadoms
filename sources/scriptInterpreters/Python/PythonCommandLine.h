#pragma once
#include <shared/process/ICommandLine.h>


//--------------------------------------------------------------
/// \brief	Command line generator
//--------------------------------------------------------------
class CPythonCommandLine : public shared::process::ICommandLine
{
   public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPythonCommandLine(const boost::filesystem::path& workingDirectory,
                      const std::string& executableName,
                      const std::vector<std::string> parameters);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPythonCommandLine();

   // ICommandLine Implementation
   std::string executable() const override;
   const boost::filesystem::path& workingDirectory() const override;
   const std::vector<std::string>& args() const override;
   // [END] ICommandLine Implementation

   private:
   const boost::filesystem::path m_workingDirectory;
   const std::string m_pluginName;
   const std::vector<std::string> m_args;
};
