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
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CZWaveConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CZWaveConfiguration();

   //--------------------------------------------------------------
   /// \brief	    The serial port
   //--------------------------------------------------------------
   std::string getSerialPort() const;

   //--------------------------------------------------------------
   /// \brief	    Include system keywords
   //--------------------------------------------------------------
   bool getIncludeSystemKeywords() const;
   
   //--------------------------------------------------------------
   /// \brief	    Initialize configuration
   //--------------------------------------------------------------
   void initializeWith(shared::CDataContainerSharedPtr container);

   //--------------------------------------------------------------
   /// \brief	    Get the library path
   /// \return	    The library path
   //--------------------------------------------------------------
   std::string getPath() const;

   //--------------------------------------------------------------
   /// \brief	    Set the library path
   /// \param [in] path    The library path
   //--------------------------------------------------------------
   void setPath(const std::string & path);


   //--------------------------------------------------------------
   /// \brief	    Get the data path
   /// \return	    The data path
   //--------------------------------------------------------------
   std::string getDataPath() const;

   //--------------------------------------------------------------
   /// \brief	    Set the data path
   /// \param [in] dataPath    The data path
   //--------------------------------------------------------------
   void setDataPath(const std::string & dataPath);

   //--------------------------------------------------------------
   /// \brief	    Get the initialization timeout
   /// \return	    The initialization timeout
   //--------------------------------------------------------------
   int getInitTimeout() const;

   //--------------------------------------------------------------
   /// \brief	    Set the initialization timeout
   /// \param [in] timeOut    The initialization timeout (seconds)
   //--------------------------------------------------------------
   void setInitTimeout(int timeOut);

private :
   //--------------------------------------------------------------
   /// \brief	    The container
   //--------------------------------------------------------------
   shared::CDataContainerSharedPtr m_container;
};

