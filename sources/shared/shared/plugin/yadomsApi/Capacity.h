#pragma once

#include <shared/Export.h>


namespace shared { namespace plugin { namespace yadomsApi
{
   //--------------------------------------------------------------
   /// \brief		Container class for a capacity
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CCapacity
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param	[in]	label    The capacity label
      /// \param	[in]	canRead  If true then this capacity can be read by device
      /// \param	[in]	canWrite If true then this capacity can be sent to device as a command
      //--------------------------------------------------------------
      CCapacity(const std::string & label, bool canRead, bool canWrite)
      :m_label(label), m_canRead(canRead), m_canWrite(canWrite)
      {
      }
      
      //--------------------------------------------------------------
      /// \brief		Destructor
      //--------------------------------------------------------------
      virtual ~CCapacity()
      {
      }
      
      //--------------------------------------------------------------
      /// \brief		Get the capacity label
      /// \return		The capacity label
      //--------------------------------------------------------------      
      const std::string & getLabel()
      {
         return m_label;
      }
      
      //--------------------------------------------------------------
      /// \brief		Get the READ capacity
      /// \return		The READ capacity
      //--------------------------------------------------------------      
      bool canRead()
      {
         return m_canRead;
      }
      
      //--------------------------------------------------------------
      /// \brief		Get the WRITE capacity
      /// \return		The WRITE capacity
      //--------------------------------------------------------------      
      bool canWrite()
      {
         return m_canWrite;
      }
      
   private:
      //--------------------------------------------------------------
      /// \brief		The capacity label
      /// \note      Can be a known capacity ("temperature", "humidity"...) or a specific capacity (must be declare at declareNewDevice call)
      //--------------------------------------------------------------
      std::string m_label;

      //--------------------------------------------------------------
      /// \brief		The READ capacity (true if the associated device have the capacity in a read way)
      /// \note      Typically a sensor (read-only) like a temperature sensor
      //--------------------------------------------------------------
      bool m_canRead;

      //--------------------------------------------------------------
      /// \brief		The WRITE capacity (true if the associated device have the capacity in a command way)
      /// \note      Typically a command (like a lamp, a send of message...)
      //--------------------------------------------------------------
      bool m_canWrite;
   };

} } } // namespace shared::plugin::yadomsApi
