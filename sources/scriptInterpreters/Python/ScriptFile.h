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
   CScriptFile(const std::string& scriptPath);

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

private:
   //--------------------------------------------------------------
   /// \brief	Full path of script file
   //--------------------------------------------------------------
   const boost::filesystem::path m_scriptFile;
};



