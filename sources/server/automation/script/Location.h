#pragma once
#include "ILocation.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include "IObjectFactory.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The location helper
   //-----------------------------------------------------
   class CLocation : public ILocation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      /// \throw shared::exception::CEmptyResult if location is undefined
      //-----------------------------------------------------
      CLocation(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CLocation();

   protected:
      // ILocation Implementation
      virtual double latitude() const;
      virtual double longitude() const;
      virtual double altitude() const;
      // [END] ILocation Implementation

      
      //--------------------------------------------------------------
      /// \brief           Try to auto locate
      ///\param[in] factory  Object factory
      /// \return the acquired location
      /// \throw shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      shared::CDataContainer tryAutoLocate(boost::shared_ptr<IObjectFactory> factory) const;

      //--------------------------------------------------------------
      /// \brief           Update the current location in database
      /// \param[in] location The new location to write in database
      //--------------------------------------------------------------
      void updateLocation(const shared::CDataContainer& location) const;

      //--------------------------------------------------------------
      /// \brief           Get the current location
      /// \return          The current location (from database, or auto-located if not found in database)
      /// \throw shared::exception::CEmptyResult if location not available
      //--------------------------------------------------------------
      shared::CDataContainer getLocation() const;

   private:
      //--------------------------------------------------------------
      /// \brief           The configuration manager
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
   };

} } // namespace automation::script
	
	