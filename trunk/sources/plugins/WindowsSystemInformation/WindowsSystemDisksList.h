#pragma once

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
   CWindowsSystemDisksList();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemDisksList();

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) CPU load
   /// \return     Temperature in �C
   //--------------------------------------------------------------
   std::vector<std::string>& getList();

private:

   //--------------------------------------------------------------
   /// \brief	    Memory Load in %
   //--------------------------------------------------------------
   std::vector<std::string> DrivesList;
};

