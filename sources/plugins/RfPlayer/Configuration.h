#pragma once

#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the xpl plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CConfiguration();

   //--------------------------------------------------------------
   /// \brief	    The serial port
   //--------------------------------------------------------------
   std::string getSerialPort() const;
   
   class CProtocols
   {
   public:
      CProtocols(unsigned short protocols = kNone)
         :m_protocols(protocols)
      {

      }

      CProtocols(const CProtocols & rhs)
         :m_protocols(rhs.m_protocols)
      {

      }

      CProtocols & operator =(const CProtocols & rhs)
      {
         m_protocols = rhs.m_protocols;
         return *this;
      }


      virtual ~CProtocols()
      {

      }

      void setupX10(bool active) { addflag(kX10, active); }
      void setupRTS(bool active) { addflag(kRts, active); }
      void setupVisonic(bool active) { addflag(kVisonic, active); }
      void setupBlyss(bool active) { addflag(kBlyss, active); }
      void setupChacon(bool active) { addflag(kChacon, active); }
      void setupOregonV1(bool active) { addflag(kOregonV1, active); }
      void setupOregonV2(bool active) { addflag(kOregonV2, active); }
      void setupOregonV3(bool active) { addflag(kOregonV3Owl, active); }
      void setupDomia(bool active) { addflag(kDomia, active); }
      void setupX2D(bool active) { addflag(kX2d, active); }
      void setupKD101(bool active) { addflag(kKd101, active); }
      void setupParrot(bool active) { addflag(kParrot, active); }

      bool isX10Active() const { return hasflag(kX10); }
      bool isRTSActive() const { return hasflag(kRts); }
      bool isVisonicActive() const { return hasflag(kVisonic); }
      bool isBlyssActive() const { return hasflag(kBlyss); }
      bool isChaconActive() const { return hasflag(kChacon); }
      bool isOregonV1Active() const { return hasflag(kOregonV1); }
      bool isOregonV2Active() const { return hasflag(kOregonV2); }
      bool isOregonV3Active() const { return hasflag(kOregonV3Owl); }
      bool isDomiaActive() const { return hasflag(kDomia); }
      bool isX2DActive() const { return hasflag(kX2d); }
      bool isKD101Active() const { return hasflag(kKd101); }
      bool isParrotActive() const { return hasflag(kParrot); }

      //--------------------------------------------------------------
      /// \brief	    Internal enumeration of protocols (values not linked to specification, can be updated)
      //--------------------------------------------------------------
      typedef enum
      {
         kNone = 0,
         kX10 = 1,
         kRts = 2,
         kVisonic = 4,
         kBlyss = 8,
         kChacon = 16,
         kOregonV1 = 32,
         kOregonV2 = 64,
         kOregonV3Owl = 128,
         kDomia = 256,
         kX2d = 512,
         kKd101 = 1024,
         kParrot = 2048
      }ProtocolFlags;

   private:
      void addflag(unsigned short flag, bool active)
      {
         if (active) 
            m_protocols |= flag;
         else
            m_protocols &= (~flag);
      }

      bool hasflag(unsigned short flag) const
      {
         return (m_protocols & flag) == flag;
      }

      unsigned short m_protocols;
   };


   
   //--------------------------------------------------------------
   /// \brief	    Get the receiver active protocols
   /// \return     Container of activated protocols
   //--------------------------------------------------------------
   CProtocols getReceiverActiveProtocols() const;

   //--------------------------------------------------------------
   /// \brief	    Tells if the repeater is active
   /// \return     true is repeater is active
   //--------------------------------------------------------------
   bool isRepeaterActive() const;

   //--------------------------------------------------------------
   /// \brief	    Get the repeater active protocols
   /// \return     Container of activated protocols
   //--------------------------------------------------------------
   CProtocols getRepeaterActiveProtocols() const;

   //--------------------------------------------------------------
   /// \brief	    Tells is LED is active or not
   /// \return     true is LED is active
   //--------------------------------------------------------------
   bool getLedActivity() const;

   //--------------------------------------------------------------
   /// \brief	    Initialize configuration
   //--------------------------------------------------------------
   void initializeWith(shared::CDataContainer container);

private :
   //--------------------------------------------------------------
   /// \brief	    The container
   //--------------------------------------------------------------
   shared::CDataContainer m_container;
};

