 /* 
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

#include "stdafx.h"
#include "cWebemTable.h"
#include "examples/webemCalendar/sqlitedatabase.h"

cWebemTable::cWebemTable() 
		: myFlagReadTable(1),
		myName(0),
		pdb(0)
	{}

/**

  Generate HTML to display database table

  @return HML string to display table

  The table has a DELETE button.  When clicked on
  this will invoke the cWebem action table_<tablename>_action

*/
char * cWebemTable::Display()
{

	static string ret;
	char buf[1000];

	if( ! myName || !pdb ) {
		ret = "Table undefined";
		return (char*)ret.c_str();
	}
	ret.clear();
	// read table
	if( myFlagReadTable ) {
		sprintf(buf,"SELECT * FROM %s;", myName);
		myRowCount = pdb->Query( buf );
		if( myRowCount < 1 ) {
			ret = "Table ";
			ret += myName;
			ret += " is empty";
			return (char*)ret.c_str();
		}
	}

	//  DELETE button
	sprintf( buf,
		"<form name=form_table_%s method=GET action=\"table_%s_action.webem\">",
		myName, myName );
	ret = buf;
	ret += "<input type=submit value=\"DELETE\" ><p>";


	ret += "<table RULES=ALL FRAME=BOX CELLPADDING=5>";

	// column headers
	if ( myHeads.size() == myColCount ) {
		ret += "<tr><th></th>";
		for( int k = 0; k < myColCount; k++ ) {
			sprintf(buf,"<th>%s</th>",myHeads[k].c_str());
			ret += buf;
		}
		ret += "</tr>\n";
	}

	// loop over records, outputing s table row for each
	for( int k = 0; k < myRowCount; k++ ) {

		// set the rows to class oddrow and evenro alternately
		// so the html style can differentiate alternate rows
		if( k%2 )
			ret += "<tr CLASS=\"evenrow\">";
		else
			ret += "<tr CLASS=\"oddrow\">";

		// the selection radio button
		ret += "<td><input type=radio name=select value=";
		sprintf(buf,"\"%s\"",pdb->myRows[myColCount*k + 0].c_str());
		ret += buf;
		ret += "></td>";

		// the columns of data
		for ( int col=0; col < myColCount; col++ ) {
			ret += "<td>";
			ret += pdb->myRows[myColCount*k + col];
			ret += "</td>";
		}
		ret += "</tr>\n";
	}
	ret += "</table></form>";
	return (char*)ret.c_str();

}
