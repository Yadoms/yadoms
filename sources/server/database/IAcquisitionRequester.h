#pragma once

#include "entities/Acquisition.h"

namespace server { 
namespace database { 

   class IAcquisitionRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief           Add new acquisition 
      /// \param [in] newAcquisition   New acquisition informations
      /// \throw           CEmptyResultException if fails
      //--------------------------------------------------------------
      virtual void addAcquisition(const entities::CAcquisition & newAcquisition) = 0;

      //--------------------------------------------------------------
      /// \brief           Get acquisition informations
      /// \param [in]      acquisitionId   Acquisition  Id
      /// \throw           CInvalidParameterException if acquisitionId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CAcquisition> getAcquisition(int acquisitionId) = 0;

      //--------------------------------------------------------------
      /// \brief        List all acquisitions for a source and a keyword
      /// \param [in]   source   the source to filter
      /// \param [in]   keyword  the keyword to filter
      /// \return       List of all acquisitions for a source and a keyword
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CAcquisition> > getAcquisitions(const std::string & source, const std::string & keyword) = 0;

      //--------------------------------------------------------------
      /// \brief        List the last acquisitions for a source (the last acquisition for each keyword)
      /// \param [in]   source   the source to filter
      /// \return       List of all acquisitions for a source
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CAcquisition> > getLastAcquisitions(const std::string & source) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove acquisition 
      /// \param [in]      acquisitionId   Acquisition  Id
      /// \throw           CEmptyResultException if fails
      //--------------------------------------------------------------
      virtual void removeAcquisition(int acquisitionId) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IAcquisitionRequester()
      {
      }
   };

} //namespace server 
} //namespace database 
