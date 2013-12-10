#pragma once

class CSQLiteDatabase;

/**

  HTML display of a database table with a cWebem delete button

 * Copyright (c) 2008 by James Bremner
 * All rights reserved.
 *
 * Use license: Modified from standard BSD license.
 * 
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation, advertising 
 * materials, Web server pages, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by James Bremner. The name "James Bremner" may not be used to 
 * endorse or promote products derived from this software without 
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.


*/
class cWebemTable
{
public:
	CSQLiteDatabase * pdb;		/// SQLITE database
	char * myName;				/// table name
	int myColCount;				/// number of columns in table
	int myRowCount;
	std::vector<std::string> myHeads;		/// column headers ( optional )
	int myFlagReadTable;		///< true if Display function should do a SELECT * on table, default true

	cWebemTable();

	char * Display();			/// generate HTML to display current contents of table

private:
};
