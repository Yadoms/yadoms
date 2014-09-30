#pragma once

//--------------------------------------------------------------
/// \brief	CPU Load of the Raspberry Pi System
/// \note   return the list of availables disks with Raspberry Pi Operating System
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

