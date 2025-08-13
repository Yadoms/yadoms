#pragma once
#include "ResponseReceivedMessage.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean dongle version RESPONSE message
   ///
   /// This class manages a dongle version RESPONSE EnOcean message.
   //--------------------------------------------------------------
   class CDongleVersionResponseReceivedMessage
   {
   public:
      class Version final
      {
      public:
         Version(unsigned int main,
                 unsigned int beta,
                 unsigned int alpha,
                 unsigned int build);
         virtual ~Version();

         std::string toString() const;

      private:
         unsigned int m_main;
         unsigned int m_beta;
         unsigned int m_alpha;
         unsigned int m_build;
      };

      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] response                The response message
      //--------------------------------------------------------------
      explicit CDongleVersionResponseReceivedMessage(const CResponseReceivedMessage& response);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CDongleVersionResponseReceivedMessage();

      std::string chipId() const;
      std::string fullVersion() const;

   private:
      const Version m_appVersion;
      const Version m_apiVersion;
      const std::string m_chipId;
      const unsigned int m_chipVersion;
      std::string m_appDescription;
   };
} // namespace message
