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
   /// \param[in] scriptName   Script name (without path and extension)
   //--------------------------------------------------------------
   CScriptFile(const std::string& scriptName);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CScriptFile();

   // IScriptFile Implementation
   virtual const boost::filesystem::path& pathName() const;
   virtual const boost::filesystem::path abslouteParentPath() const;
   virtual const std::string& module() const;
   virtual const std::string& entryPoint() const;
   virtual bool exists() const;
   virtual std::string read() const;
   virtual void write(const std::string& content) const;
   // [END] IScriptFile Implementation

   //--------------------------------------------------------------
   /// \brief	Return a template of a script file
   //--------------------------------------------------------------
   std::string fromTemplate() const;

private:
   //--------------------------------------------------------------
   /// \brief	Full path of script file
   //--------------------------------------------------------------
   const boost::filesystem::path m_scriptFile;

   //--------------------------------------------------------------
   /// \brief	Module name
   //--------------------------------------------------------------
   const std::string m_module;
};



