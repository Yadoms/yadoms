#pragma once
#include "FrameBase.h"
#include "database/entities/Entities.h"

namespace web
{
   namespace ws
   {
      //-----------------------------
      ///\brief Frame which can be send to GUI to notify for new acquisitions
      //-----------------------------
      class CAcquisitionSummaryUpdateFrame : public CFrameBase
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param[in] acquisitionSummaries     The acquisition summaries to send to GUI
         //-----------------------------
         explicit CAcquisitionSummaryUpdateFrame(const std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary> >& acquisitionSummaries);

         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CAcquisitionSummaryUpdateFrame();

      protected:
         //-----------------------------
         ///\bruef The acquisition field name
         //-----------------------------
         static const std::string m_acquisitionString;
      };
   } //namespace ws
} //namespace web


