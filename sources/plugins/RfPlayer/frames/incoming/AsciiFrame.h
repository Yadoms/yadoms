#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
namespace yApi = shared::plugin::yPluginApi;

namespace frames {
namespace incoming {

   class CAsciiFrame
   {
   public:
      //--------------------------------------------------------------
      /// \brief	               The header size of ascii frame
      //--------------------------------------------------------------
      static const int HeaderSize = 5;

      //--------------------------------------------------------------
      /// \brief	               Constructor
      /// \param[in] sourceDest  The sourceDest byte from header
      /// \param[in] qualifier1  The qualifier1 byte from header
      /// \param[in] qualifier2  The qualifier2 byte from header
      /// \param[in] data        The frame content as string
      //--------------------------------------------------------------
      CAsciiFrame(char sourceDest, char qualifier1, char qualifier2, const std::string & data);

      //--------------------------------------------------------------
      /// \brief	               Destructor
      //--------------------------------------------------------------
      virtual ~CAsciiFrame();

      //--------------------------------------------------------------
      /// \brief	               Get the sourceDest of frame
      /// \return                The sourceDest byte of frame
      //--------------------------------------------------------------
      const char getSourceDest() const; 
      
      //--------------------------------------------------------------
      /// \brief	               Get the qualifier of frame
      /// \param [in] index      The qualifier index (0 for qualifier 1, and 1 for qualifier2)
      /// \return                The sourceDest byte of frame
      //--------------------------------------------------------------
      const char getQualifier(const int index) const;

      //--------------------------------------------------------------
      /// \brief	               Get the string content of frame
      /// \return                The string content of frame
      //--------------------------------------------------------------
      const std::string & getContent() const;

      void printToLog(std::ostream & out) const;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;
   private:
      //--------------------------------------------------------------
      /// \brief	               The sourceDest of frame
      //--------------------------------------------------------------
      char m_sourceDest;

      //--------------------------------------------------------------
      /// \brief	               The qualifiers of frame
      //--------------------------------------------------------------
      char m_qualifiers[2];

      //--------------------------------------------------------------
      /// \brief	               The content of frame
      //--------------------------------------------------------------
      std::string  m_content;
   };

} //namespace incoming
} //namespace frames



