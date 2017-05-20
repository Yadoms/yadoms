#pragma once
#include "FrameBase.h"

namespace web
{
   namespace ws
   {
      //-----------------------------
      ///\brief Frame which can be received from GUI to filter the notifications
      //-----------------------------
      class CAcquisitionFilterFrame : public CFrameBase
      {
      public:
         //-----------------------------
         ///\brief Constructor (deserialization)
         ///\param [in]    content     The frame content that will be deserialized
         //-----------------------------
         explicit CAcquisitionFilterFrame(const std::string& content);

         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CAcquisitionFilterFrame();

         //-----------------------------
         ///\brief Get the acquisition filter provided by GUI
         ///\return  the acquisition filter provided by GUI
         //-----------------------------
         std::vector<int> getFilter() const;
      };
   } //namespace ws
} //namespace web


