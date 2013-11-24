#pragma once

#include "IHardwareRequester.h"
#include <vector>

class IDatabaseProvider
{
public:
	virtual ~IDatabaseProvider()
	{
	}

	//--------------------------------------------------------------
	/// \Brief		Load the database system
	/// \return 	true if database is loaded with success
	/// \throws 		
	//--------------------------------------------------------------
	virtual bool load() = 0;

	//--------------------------------------------------------------
	/// \Brief	Unoad the database system
	//--------------------------------------------------------------
	virtual void unload() = 0;

	//--------------------------------------------------------------
	/// \Brief		get the hardware requester
	/// \return 	the hardware requester
	/// \throws 		
	//--------------------------------------------------------------  
   IHardwareRequester * getHardwareRequester();


   //--------------------------------------------------------------
	/// \brief	Use do define one row from database
	//--------------------------------------------------------------
   typedef std::vector<std::string>  QueryRow;

   //--------------------------------------------------------------
	/// \brief	Use do define a list of rows from database
	//--------------------------------------------------------------
   typedef std::vector<QueryRow>  QueryResults;

	//--------------------------------------------------------------
	/// \brief		      execute a single statement (create, update, delete) which returns the number of affected lines
	/// \param [in]	   the query (with vaargs)
	/// \return 	      the number of affected lines
	//--------------------------------------------------------------  
   int queryStatement(const std::string & queryFormat, ...);

	//--------------------------------------------------------------
	/// \brief		      query only one databse row
	/// \param [in]	   the query (with vaargs)
	/// \return 	      the row values
	//--------------------------------------------------------------  
   QueryRow querySingleLine(const std::string & queryFormat, ...);

	//--------------------------------------------------------------
	/// \brief		      query databse
	/// \param [in]	   the query (with vaargs)
	/// \return 	      a list of rows
	//--------------------------------------------------------------  
   QueryResults query(const std::string & queryFormat, ...);

protected:
	IDatabaseProvider()
	{
	}
};
