//
// NetworkHelper.h
//
// Platforms network helper
//
#pragma once

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	this class is used to provide helpers in networking
   //--------------------------------------------------------------
   class CNetworkHelper
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CNetworkHelper();

      //--------------------------------------------------------------
      /// \brief			Return all locals ip
      /// \return a list of ip detected on the computer
      //--------------------------------------------------------------
      static std::vector<boost::asio::ip::address> getLocalIps();

   private:
      //--------------------------------------------------------------
      /// \brief			Constructor
      //--------------------------------------------------------------
      CNetworkHelper();
   };

} // namespace shared