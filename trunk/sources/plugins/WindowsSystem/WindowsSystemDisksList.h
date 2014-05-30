#pragma once

//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/uniform_int_distribution.hpp>

//--------------------------------------------------------------
/// \brief	CPU Load of the Windows System
/// \note   return the list of availables disks with Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemDisksList
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CWindowsSystemDisksList(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemDisksList();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) CPU load
   /// \return     Temperature in �C
   //--------------------------------------------------------------
   std::vector<std::string>& getList();

private:

   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Memory Load in %
   //--------------------------------------------------------------
   std::vector<std::string> DrivesList;
};

