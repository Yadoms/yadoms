//
// NetworkHelper.h
//
// Platforms network helper
//
#pragma once
#include <shared/Export.h>

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	this class is used to provide helpers in networking
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CNetworkHelper
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