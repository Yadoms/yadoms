#pragma once

#include <shared/Export.h>
#include <boost/property_tree/ptree.hpp>

namespace shared { namespace serialization
{

   //--------------------------------------------------------------
   /// \class Property tree to string serializer interface
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT IPtreeToStringSerializer
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IPtreeToStringSerializer() {}

      //--------------------------------------------------------------
      /// \brief              Serialize the provided property tree
      /// \par pt             Property tree to serialize
      /// \return             Result of serialization, as string
      //--------------------------------------------------------------
      virtual const std::string serialize(const boost::property_tree::ptree& pt) const = 0;

      //--------------------------------------------------------------
      /// \brief              Deserialize string to property tree
      /// \par str            Input string
      /// \par [out] pt       Property tree populated
      /// \throw              CInvalidParameter if str is not in the valid format
      //--------------------------------------------------------------
      virtual void deserialize(const std::string& str, boost::property_tree::ptree& pt) = 0;
   };

} } // namespace shared::serialization
