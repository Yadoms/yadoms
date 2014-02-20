//
// Custom validators for startup options loader
//
#pragma once

namespace startupOptions
{

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
      /// \note       Needed for implementation of boost::program_options::value::default_value()
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

} // namespace startupOptions