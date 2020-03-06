#pragma once
#include <shared/ILocation.h>
#include "dataAccessLayer/IConfigurationManager.h"
#include "IAutoLocation.h"

namespace location
{
   //-----------------------------------------------------
   ///\brief The location helper
   //-----------------------------------------------------
   class CLocation : public shared::ILocation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      ///\param[in] autoLocationService   Auto-location service
      /// \throw shared::exception::CEmptyResult if location is undefined
      //-----------------------------------------------------
      CLocation(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                boost::shared_ptr<IAutoLocation> autoLocationService);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CLocation() = default;

   protected:
      // ILocation Implementation
      double latitude() const override;
      double longitude() const override;
      double altitude() const override;
      // [END] ILocation Implementation


      //--------------------------------------------------------------
      /// \brief           Try to auto locate
      /// \return the acquired location
      /// \throw shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      void tryAutoLocate() const;

      //--------------------------------------------------------------
      /// \brief           Get the current location
      /// \return          The current location (from database, or auto-located if not found in database)
      /// \throw shared::exception::CEmptyResult if location not available
      //--------------------------------------------------------------
      shared::CDataContainerSharedPtr getLocation() const;

   private:
      //--------------------------------------------------------------
      /// \brief           The configuration manager
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;

      //--------------------------------------------------------------
      /// \brief           The auto-location service
      //--------------------------------------------------------------
      boost::shared_ptr<IAutoLocation> m_autoLocationService;
   };
} // namespace location


