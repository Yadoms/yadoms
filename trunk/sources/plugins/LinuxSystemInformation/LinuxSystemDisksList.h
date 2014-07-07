#pragma once

//--------------------------------------------------------------
/// \brief	CPU Load of the Linux System
/// \note   return the list of availables disks with Linux Operating System
//--------------------------------------------------------------
class CLinuxSystemDisksList
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CLinuxSystemDisksList();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinuxSystemDisksList();

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

