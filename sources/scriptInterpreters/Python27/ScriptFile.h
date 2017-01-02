#pragma once
#include "IScriptFile.h"

//--------------------------------------------------------------
/// \brief	Python script file
//--------------------------------------------------------------
class CScriptFile : public IScriptFile
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] scriptPath   Script path (without name)
   //--------------------------------------------------------------
   explicit CScriptFile(const std::string& scriptPath);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CScriptFile();

   // IScriptFile Implementation
   const boost::filesystem::path& pathName() const override;
   boost::filesystem::path abslouteParentPath() const override;
   const std::string& module() const override;
   const std::string& entryPoint() const override;
   bool exists() const override;
   std::string read() const override;
   void write(const std::string& content) const override;
   // [END] IScriptFile Implementation

   //--------------------------------------------------------------
   /// \brief	Read Python file
   //--------------------------------------------------------------
   static std::string PythonFileRead(const std::string& filename);

private:
   //--------------------------------------------------------------
   /// \brief	Full path of script file
   //--------------------------------------------------------------
   const boost::filesystem::path m_scriptFile;

   //--------------------------------------------------------------
   /// \brief	Some internal constant key strings
   //--------------------------------------------------------------
   static const std::string ScriptName;         ///< Be careful, in Python, filename is also the Python module name, and modules are globals.
   static const std::string Module;
   static const std::string PythonExtension;    ///< Standard Python scripts extension
   static const std::string ScriptEntryPoint;   ///< Function name of the Python script entry point
};



