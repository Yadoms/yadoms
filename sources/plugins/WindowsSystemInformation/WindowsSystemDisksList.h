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
   /// \brief	    Returns the Drive List
   //--------------------------------------------------------------
   std::vector<std::string>& getList();

private:

   //--------------------------------------------------------------
   /// \brief	    Drive List vector
   //--------------------------------------------------------------
   std::vector<std::string> DrivesList;
};

