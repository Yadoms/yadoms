//
// Custom validators for startup options loader
//
#pragma once

namespace po = boost::program_options;


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

//--------------------------------------------------------------
/// \class Invalid option exception
// Exception thrown if there is an invalid value given
// Catched as po::invalid_option_value in CStartupOptionsLoader::ctor
// This class add some information in error display
//--------------------------------------------------------------
class CInvalidOptionException : public po::invalid_option_value
{
public:
   CInvalidOptionException(const std::string& value, const std::string& message)
      :invalid_option_value(value + " : " + message)
   {}
};
//--------------------------------------------------------------
/// \class Special ip address option
/// This class check if the ip address has a valid form
//--------------------------------------------------------------
class CValidIpAddressOption
{
public:
   //--------------------------------------------------------------
   /// \brief	         Constructor
   /// \param[in] ipAddress   Ip address
   //--------------------------------------------------------------
   CValidIpAddressOption(const std::string& ipAddress)
      :m_ipAddress(ipAddress)  {}

   //--------------------------------------------------------------
   /// \brief	    Default constructor
   //--------------------------------------------------------------
   CValidIpAddressOption() {}

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CValidIpAddressOption() {}

   //--------------------------------------------------------------
   /// \brief	    Affectation operator
   /// \note       Needed for implementation of po::value::default_value()
   //--------------------------------------------------------------
   void operator=(const CValidIpAddressOption& src)
   { m_ipAddress = src.m_ipAddress; }

public:
   //--------------------------------------------------------------
   /// \brief	    Check the provided ip address
   /// \param[in] ipAddress   Ip address
   /// \return     true if Ip address valid
   //--------------------------------------------------------------
   static bool validate(const std::string& ipAddress)
   {
      boost::system::error_code ec;
      boost::asio::ip::address::from_string(ipAddress, ec);
      return ec==0;
   }

   //--------------------------------------------------------------
   /// \brief	    Ip address getter
   /// \return     Ip address
   //--------------------------------------------------------------
   const std::string& get() const
   { return m_ipAddress; }

protected:
   std::string m_ipAddress;
};

