#pragma once

#include "entities/Entities.h"


namespace database { 

   //---------------------------------
   ///\brief Interface fro backing up data to a file
   //---------------------------------
   class IDataBackup
   {
   public:
      //---------------------------------
      ///\brief Define a function prototype for updating the backup progress
      //---------------------------------
      typedef boost::function3<void, int, int, std::string> ProgressFunc;

      //---------------------------------
      ///\brief Indicate if backup is supported by database engine
      ///\return boolean
      //---------------------------------
      virtual bool backupSupported() = 0;

      //---------------------------------
      ///\brief Backup the data provider to a file
      ///\param [in] backupLocation : the backup location
      ///\param [in] reporter : a function pointer for reporting progression
      //---------------------------------
      virtual void backupData(const std::string & backupLocation, ProgressFunc reporter) = 0;


      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IDataBackup()
      {
      }
   };

 
} //namespace database 
