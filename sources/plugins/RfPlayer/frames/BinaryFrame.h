#pragma once
#include <shared/communication/Buffer.hpp>

namespace frames {

   class CBinaryFrame
   {
   public:
      //--------------------------------------------------------------
      /// \brief	               Constructor
      /// \param[in] sourceDest  The sourceDest byte from header
      /// \param[in] content     Binary buffer
      //--------------------------------------------------------------
      CBinaryFrame(char sourceDest, boost::shared_ptr<shared::communication::CByteBuffer> content);

      //--------------------------------------------------------------
      /// \brief	               Destructor
      //--------------------------------------------------------------
      virtual ~CBinaryFrame();

      //--------------------------------------------------------------
      /// \brief	               The header size of binary frame
      //--------------------------------------------------------------
      static const int HeaderSize = 5;

      //--------------------------------------------------------------
      /// \brief	               Get the sourceDest of frame
      /// \return                The sourceDest byte of frame
      //--------------------------------------------------------------
      const char getSourceDest() const;

      //--------------------------------------------------------------
      /// \brief	               Get the binary content of frame
      /// \return                The binary content of frame
      //--------------------------------------------------------------
      boost::shared_ptr< shared::communication::CByteBuffer > getContent() const;

   private:
      //--------------------------------------------------------------
      /// \brief	               The sourceDest of frame
      //--------------------------------------------------------------
      char m_sourceDest;
      //--------------------------------------------------------------
      /// \brief	               The binary frame content (without header)
      //--------------------------------------------------------------
      boost::shared_ptr< shared::communication::CByteBuffer > m_content;
   };

} //namespace frames

