#pragma once

#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the xpl plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CZWaveConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CZWaveConfiguration();

   //--------------------------------------------------------------
   /// \brief	    The serial port
   //--------------------------------------------------------------
   std::string getSerialPort() const;

   //--------------------------------------------------------------
   /// \brief	    Initialize configuration
   //--------------------------------------------------------------
   void initializeWith(shared::CDataContainer container);

   //--------------------------------------------------------------
   /// \brief	    Get the library path
   /// \return	    The library path
   //--------------------------------------------------------------
   const std::string getPath();

   //--------------------------------------------------------------
   /// \brief	    Set the library path
   /// \param [in] path    The library path
   //--------------------------------------------------------------
   void setPath(const std::string & path);
private :
   //--------------------------------------------------------------
   /// \brief	    The container
   //--------------------------------------------------------------
   shared::CDataContainer m_container;
};

