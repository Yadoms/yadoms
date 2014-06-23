#pragma once

#include <shared/Export.h>

namespace shared { namespace serialization {

   //--------------------------------------------------------------
   /// \brief	    Interface for a file serializable object
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT IDataFileSerializable
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IDataFileSerializable() {}

      //--------------------------------------------------------------
      /// \brief	    Serialize this CDataContainer instance
      /// \return     the data serialized
      //--------------------------------------------------------------
      virtual void serializeToFile(const std::string & filename) const = 0;

      //--------------------------------------------------------------
      /// \brief	    Deserialize a string and set data into this CDataContainer instance
      /// \param [in] data    data to deserialize
      //--------------------------------------------------------------
      virtual void deserializeFromFile(const std::string & filename) = 0;
   };


} // namespace serialization   
} // namespace shared   