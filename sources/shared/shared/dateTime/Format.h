#pragma once
#include <Poco/DateTime.h>

namespace shared { namespace dateTime {

   //--------------------------------------------------------------
   /// \brief	Simple container of a data time.
   /// The aim is to make it compatible between boost dateTime and Poco::DateTime
   /// and make boost::posix_time::ptime usable in notification (boost::posix_time::ptime can not be used as template type)
   //--------------------------------------------------------------
	class CFormat
	{
	public:
		//--------------------------------------------------------------
		/// \brief	Format a Poco time with given format 
		/// \return	a string
		//--------------------------------------------------------------
		static std::string format(Poco::DateTime& time, const std::string& boostFormat);

		//--------------------------------------------------------------
		/// \brief	Format a boost time with given format 
		/// \return	a string
		//--------------------------------------------------------------
		static std::string format(boost::posix_time::ptime& time, const std::string& boostFormat);

		//--------------------------------------------------------------
		/// \brief	Format a the NOW dataTime with given format 
		/// \return	a string
		//--------------------------------------------------------------
		static std::string formatNow(const std::string& boostFormat);
	};

} } //namespace shared::dateTime