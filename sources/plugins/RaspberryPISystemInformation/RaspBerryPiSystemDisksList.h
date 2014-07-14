#pragma once

//--------------------------------------------------------------
/// \brief	CPU Load of the RaspBerryPi System
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
   /// \brief	    Returns read (computed) CPU load
   /// \return     Temperature in �C
   //--------------------------------------------------------------
   std::vector<std::string>& getList();

private:

std::vector<std::string> ExecuteCommandAndReturn(const std::string &szCommand);

   //--------------------------------------------------------------
   /// \brief	    Memory Load in %
   //--------------------------------------------------------------
   std::vector<std::string> DrivesList;
};

