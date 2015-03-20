#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IRfxcomConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IRfxcomConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer &data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Check if connection is configured as Ethernet
   /// \return     true if Ethernet, false if serial port
   //--------------------------------------------------------------
   virtual bool comIsEthernet() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the RFXLan server address (IP address or DNS name)
   /// \return     The IP address or DNS name of the RFXLan server
   //--------------------------------------------------------------
   virtual std::string getEthernetAddress() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the RFXLan server TCP port number
   /// \return     The TCP port
   //--------------------------------------------------------------
   virtual std::string getEthernetPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the rfxcom
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the protocols configured by user
   /// \return     The configured transceiver status
   //--------------------------------------------------------------
   virtual bool isAEenabled        () const = 0;
   virtual bool isRUBICSONenabled  () const = 0;
   virtual bool isFINEOFFSETenabled() const = 0;
   virtual bool isLIGHTING4enabled () const = 0;
   virtual bool isRSLenabled       () const = 0;
   virtual bool isSXenabled        () const = 0;
   virtual bool isRFU6enabled      () const = 0;
   virtual bool isUNDECODEDenabled () const = 0;
   virtual bool isMERTIKenabled    () const = 0;
   virtual bool isLWRFenabled      () const = 0;
   virtual bool isHIDEKIenabled    () const = 0;
   virtual bool isLACROSSEenabled  () const = 0;
   virtual bool isFS20enabled      () const = 0;
   virtual bool isPROGUARDenabled  () const = 0;
   virtual bool isBLINDST0enabled  () const = 0;
   virtual bool isBLINDST1enabled  () const = 0;
   virtual bool isX10enabled       () const = 0;
   virtual bool isARCenabled       () const = 0;
   virtual bool isACenabled        () const = 0;
   virtual bool isHEEUenabled      () const = 0;
   virtual bool isMEIANTECHenabled () const = 0;
   virtual bool isOREGONenabled    () const = 0;
   virtual bool isATIenabled       () const = 0;
   virtual bool isVISONICenabled   () const = 0;
};

