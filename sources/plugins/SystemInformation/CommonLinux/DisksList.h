#pragma once

//--------------------------------------------------------------
/// \brief	CPU Load of the Linux System
/// \note   return the list of availables disks with Linux Operating System
//--------------------------------------------------------------
class CDisksList
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CDisksList();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CDisksList();

   //--------------------------------------------------------------
   /// \brief	    Returns the Drive List
   //--------------------------------------------------------------
   const std::vector<std::string>& getList() const;

private:

   //--------------------------------------------------------------
   /// \brief	    Drive List vector
   //--------------------------------------------------------------
   std::vector<std::string> DrivesList;
};

