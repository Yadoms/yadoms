#pragma once

//--------------------------------------------------------------
/// \brief  Disk list of the RaspBerryPi System
/// \note   return the list of availables disks with RaspBerryPi Operating System
//--------------------------------------------------------------
class CRaspBerryPiSystemDisksList
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CRaspBerryPiSystemDisksList();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRaspBerryPiSystemDisksList();

   //--------------------------------------------------------------
   /// \brief	    Returns a list of disks
   //--------------------------------------------------------------
   std::vector<std::string>& getList();

private:

std::vector<std::string> ExecuteCommandAndReturn(const std::string &szCommand);

   //--------------------------------------------------------------
   /// \brief	    Disk List
   //--------------------------------------------------------------
   std::vector<std::string> DrivesList;
};

