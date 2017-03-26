#pragma once
#include "AsciiFrame.h"
#include "BinaryFrame.h"
#include "FrameType.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
namespace yApi = shared::plugin::yPluginApi;

namespace frames {

   //--------------------------------------------------------------
   /// \brief container class for ziblue frames
   //--------------------------------------------------------------
   class CFrame
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                  Construct a Frame from a binary buffer
      /// \param[in] binaryBuffer   The binary buffer
      //--------------------------------------------------------------
      CFrame(boost::shared_ptr<CBinaryFrame> binaryBuffer);

      //--------------------------------------------------------------
      /// \brief	                  Construct a Frame from an Ascii buffer
      /// \param[in] asciiBuffer    The ascii buffer
      //--------------------------------------------------------------
      CFrame(boost::shared_ptr<CAsciiFrame> asciiBuffer);

      //--------------------------------------------------------------
      /// \brief	               Destructor
      //--------------------------------------------------------------
      virtual ~CFrame();

      //--------------------------------------------------------------
      /// \brief	               Tells if frame is binary
      /// \return                true for binary frame, false other cases
      //--------------------------------------------------------------
      bool isBinary() const;

      //--------------------------------------------------------------
      /// \brief	               Tells if frame is ascii
      /// \return                true for ascii frame, false other cases
      //--------------------------------------------------------------
      bool isAscii() const;

      //--------------------------------------------------------------
      /// \brief	               Obtains the binary frame
      /// \return                The binary frame (null if not binary frame)
      //--------------------------------------------------------------
      boost::shared_ptr<CBinaryFrame> getBinary() const;

      //--------------------------------------------------------------
      /// \brief	               Obtains the ascii frame
      /// \return                The ascii frame (null if not ascii frame)
      //--------------------------------------------------------------
      boost::shared_ptr<CAsciiFrame> getAscii() const;

      //--------------------------------------------------------------
      /// \brief	               Obtains the string representation of frame (debug purpose only)
      /// \return                The frame as string
      //--------------------------------------------------------------
      const std::string toString() const;


      void printToLog(std::ostream & out) const;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;

   private:
      //--------------------------------------------------------------
      /// \brief	               The frame type
      //--------------------------------------------------------------
      EFrameType m_frameType;

      //--------------------------------------------------------------
      /// \brief	               The binary buffer
      //--------------------------------------------------------------
      boost::shared_ptr<CBinaryFrame> m_binaryBuffer;

      //--------------------------------------------------------------
      /// \brief	               The ascii buffer
      //--------------------------------------------------------------
      boost::shared_ptr<CAsciiFrame> m_asciiBuffer;
   };
   
} //namespace frames



