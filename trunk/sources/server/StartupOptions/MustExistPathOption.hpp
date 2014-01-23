//
// Custom validators for startup options loader
//
#pragma once

//--------------------------------------------------------------
/// \class Special path option
/// This class check if the path given as option exists
//--------------------------------------------------------------
class CMustExistPathOption
{
public:
   //--------------------------------------------------------------
   /// \brief	         Constructor
   /// \param[in] path  The path
   //--------------------------------------------------------------
   CMustExistPathOption(const std::string& path)
      :m_path(path)  {}

   //--------------------------------------------------------------
   /// \brief	    Default constructor
   //--------------------------------------------------------------
   CMustExistPathOption() {}

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMustExistPathOption() {}

   //--------------------------------------------------------------
   /// \brief	    Affectation operator
   /// \note       Needed for implementation of po::value::default_value()
   //--------------------------------------------------------------
   void operator=(const CMustExistPathOption& src)
   { m_path = src.m_path; }

public:
   //--------------------------------------------------------------
   /// \brief	    Check the provided path
   /// \param[in] path  The path
   /// \return     true if path exists
   //--------------------------------------------------------------
   static bool validate(const std::string& path)
   {
      return boost::filesystem::exists(path);
   }

   //--------------------------------------------------------------
   /// \brief	    Path getter
   /// \return     Path
   //--------------------------------------------------------------
   const std::string& get() const
   { return m_path; }

protected:
   std::string m_path;
};

