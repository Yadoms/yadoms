#pragma once

//--------------------------------------------------------------
/// \brief	Python script file interface
//--------------------------------------------------------------
class IScriptFile
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IScriptFile()
   {
   }

   //--------------------------------------------------------------
   /// \brief	Get the path name
   /// \return The path name
   //--------------------------------------------------------------
   virtual const boost::filesystem::path& pathName() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get the parent path, as absolute path
   /// \return The parent path
   //--------------------------------------------------------------
   virtual boost::filesystem::path abslouteParentPath() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get the Python module name
   /// \return The module name
   //--------------------------------------------------------------
   virtual const std::string& module() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get the script entry point name (main function)
   /// \return The entry point name
   //--------------------------------------------------------------
   virtual const std::string& entryPoint() const = 0;

   //--------------------------------------------------------------
   /// \brief	Check if script file exists
   /// \return true if exists
   //--------------------------------------------------------------
   virtual bool exists() const = 0;

   //--------------------------------------------------------------
   /// \brief	Read the script content
   /// \return Script content
   ///\throw CInvalidParameter if file not found or unreadable
   //--------------------------------------------------------------
   virtual std::string read() const = 0;

   //--------------------------------------------------------------
   /// \brief	Write the script content
   /// \param[in] content Script content
   //--------------------------------------------------------------
   virtual void write(const std::string& content) const = 0;
};
