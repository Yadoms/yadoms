#pragma once

//--------------------------------------------------------------
/// \brief	Configuration of the xpl plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CDongle
{
private:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CDongle();
   
public:
   //--------------------------------------------------------------
   /// \brief	      Create a CDongle instance from an hello command answer
   /// \param [in]   helloCommandAnswer  The hello command answer
   /// \return       The create instance
   //--------------------------------------------------------------
   static boost::shared_ptr<CDongle> create(const std::string & helloCommandAnswer);
   
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CDongle();

   //--------------------------------------------------------------
   /// \brief	    Get the dongle type
   //--------------------------------------------------------------
   const std::string & getType() const;

   //--------------------------------------------------------------
   /// \brief	    Get the dongle model
   //--------------------------------------------------------------
   const std::string & getModel() const;

   //--------------------------------------------------------------
   /// \brief	    Get the firmware version
   //--------------------------------------------------------------
   const std::string & getFirmwareVersion() const;

   //--------------------------------------------------------------
   /// \brief	    Get the MAC address
   //--------------------------------------------------------------
   const std::string & getMacAddress() const;

   //--------------------------------------------------------------
   /// \brief	    Get the 433 freq
   //--------------------------------------------------------------
   const std::string & getFreq433() const;

   //--------------------------------------------------------------
   /// \brief	    Get the 866 freq
   //--------------------------------------------------------------
   const std::string & getFreq868() const;

   //--------------------------------------------------------------
   /// \brief	    Get the eu/us mode
   //--------------------------------------------------------------
   const std::string & getMode() const;

private:
   //--------------------------------------------------------------
   /// \brief	    The dongle type
   //--------------------------------------------------------------
   std::string m_type;
   
   //--------------------------------------------------------------
   /// \brief	    The dongle model
   //--------------------------------------------------------------
   std::string m_model;

   //--------------------------------------------------------------
   /// \brief	    The firmware version
   //--------------------------------------------------------------
   std::string m_firmwareVersion;

   //--------------------------------------------------------------
   /// \brief	    The MAC address
   //--------------------------------------------------------------
   std::string m_macAddress;

   //--------------------------------------------------------------
   /// \brief	    The 433 freq
   //--------------------------------------------------------------
   std::string m_freq433;

   //--------------------------------------------------------------
   /// \brief	    The 868 freq
   //--------------------------------------------------------------
   std::string m_freq868;

   //--------------------------------------------------------------
   /// \brief	    The freq mode (eu or us)
   //--------------------------------------------------------------
   std::string m_mode;
};


