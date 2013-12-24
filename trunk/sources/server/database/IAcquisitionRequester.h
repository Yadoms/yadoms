#pragma once

#include "entities/Acquisition.h"

class IAcquisitionRequester
{
public:
   //--------------------------------------------------------------
   /// \brief           Add new acquisition 
   /// \param [in] newAcquisition   New acquisition informations
   /// \throw           CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual void addAcquisition(boost::shared_ptr<CAcquisition> newAcquisition) = 0;

   //--------------------------------------------------------------
   /// \brief           Get acquisition informations
   /// \param [in]      acquisitionId   Acquisition  Id
   /// \throw           CInvalidParameterException if acquisitionId is unknown
   //--------------------------------------------------------------
   virtual boost::shared_ptr<CAcquisition> getAcquisition(int acquisitionId) = 0;

   //--------------------------------------------------------------
   /// \brief        List all acquisitions for a source and a keyword
   /// \param [in]   source   the source to filter
   /// \param [in]   keyword  the keyword to filter
   /// \return       List of all acquisitions for a source and a keyword
   //--------------------------------------------------------------
   virtual std::vector<boost::shared_ptr<CAcquisition> > getAcquisitions(const std::string & source, const std::string & keyword) = 0;

   //--------------------------------------------------------------
   /// \brief           Remove acquisition 
   /// \param [in]      acquisitionId   Acquisition  Id
   /// \throw           CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual void removeAcquisition(int acquisitionId) = 0;

public:
   virtual ~IAcquisitionRequester()
   {
   }
};
