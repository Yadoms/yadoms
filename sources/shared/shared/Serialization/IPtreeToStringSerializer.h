#pragma once


//--------------------------------------------------------------
/// \class Property tree to string serializer interface
//--------------------------------------------------------------
class IPtreeToStringSerializer
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
   /// \throw              CInvalidParameterException if str is not in the valid format
   //--------------------------------------------------------------
   virtual void deserialize(const std::string& str, boost::property_tree::ptree& pt) = 0;
};

