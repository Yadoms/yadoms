#pragma once
#include <shared/Export.h>
#include <Poco/DateTime.h>

namespace shared { namespace dateTime {

   //--------------------------------------------------------------
   /// \brief	Simple container of a data time.
   /// The aim is to make it compatible between boost dateTime and Poco::DateTime
   /// and make boost::posix_time::ptime usable in notification (boost::posix_time::ptime can not be used as template type)
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CDateTimeContainer
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor (initialized to now)
      //--------------------------------------------------------------
      CDateTimeContainer();

      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]   time  The initialization value
      //--------------------------------------------------------------
      CDateTimeContainer(boost::posix_time::ptime & time);

      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]   time  The initialization value
      //--------------------------------------------------------------
      CDateTimeContainer(Poco::DateTime & time);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CDateTimeContainer();

      //--------------------------------------------------------------
      /// \brief	Get the date time as boost posix_time format
      /// \return	the date time as boost posix_time format
      //--------------------------------------------------------------
      boost::posix_time::ptime getBoostDateTime();

      //--------------------------------------------------------------
      /// \brief	Get the date time as Poco::DateTime format
      /// \return	the date time as boost posix_time format
      //--------------------------------------------------------------
      Poco::DateTime getPocoDateTime();

   private:
      //--------------------------------------------------------------
      /// \brief	The time container
      //--------------------------------------------------------------
      boost::posix_time::ptime m_time;
   };

} } //namespace shared::dateTime