#pragma once
//
// This file provide macros used to declare header and implementation for JSON entity behavior
//  -> it generates the class that defines the id attached to an entity propertt (the id the the text used in json data to identify a property)
//  -> it generates a serializer class which enable to serialize and deserialize JSON data
//
// ==========================================================================
//        HEADER PART
// ==========================================================================
//
// Example : Plugin entity => class CPluginEntityJsonDefinition and CPluginEntitySerializer
//
//   Input :
//             DECLARE_JSON_ENTITY_HEADER(Plugin, 
//                (Id)
//                (Name)
//                (PluginName)
//                (Configuration)
//                (Enabled)
//             )
//
//    Ouptut :
//             class CPluginEntityJsonDefinition
//             {
//             private:
//                CPluginEntityJsonDefinition() {}
//                virtual ~CPluginEntityJsonDefinition() {}
//             
//             public:
//                static const std::string & getIdIdentifier() { return m_pluginIdIdentifier; }
//                static const std::string & getNameIdentifier() { return m_pluginNameIdentifier; }
//                static const std::string & getPluginNameIdentifier() { return m_pluginPluginNameIdentifier; }
//                static const std::string & getConfigurationIdentifier() { return m_pluginConfigurationIdentifier; }
//                static const std::string & getEnabledIdentifier() { return m_pluginEnabledIdentifier; }
//             
//             private:
//                static std::string m_pluginIdIdentifier;
//                static std::string m_pluginNameIdentifier;
//                static std::string m_pluginPluginNameIdentifier;
//                static std::string m_pluginConfigurationIdentifier;
//                static std::string m_pluginEnabledIdentifier;
//             };
//             
//             class CPluginEntitySerializer : public IEntitySerializer<CPlugin>, public IEntityDeserializer<CPlugin>
//             {
//             public:
//                CPluginEntitySerializer();
//                virtual ~CPluginEntitySerializer();
//                
//                // IEntitySerializer implementation
//                CJson serialize(const CPlugin & object);
//                // [END] IEntitySerializer implementation
//             
//                // IEntityDeserializer implementation
//                CPlugin deserialize(const CJson & object);
//                // [END] IEntityDeserializer implementation
//             }
//         
//
// ==========================================================================
//        SOURCE PART
// ==========================================================================
//
//
// Example : Plugin entity => implementation of class CPluginEntityJsonDefinition and CPluginEntitySerializer
//
//   Input :
//          DECLARE_JSON_ENTITY_IMPLEMENTATION(Plugin, "Plugin",
//             ((Id)("id")JSON_INT)
//             ((Name)("name")JSON_STRING)
//             ((PluginName)("pluginName")JSON_STRING)
//             ((Configuration)("configuration")JSON_STRING)
//             ((Enabled)("enabled")JSON_BOOL)
//          )
//
//    Ouptut :
//             //implementation of class CPluginEntityJsonDefinition
//             std::string CPluginEntityJsonDefinition::m_pluginIdIdentifier = "Id";
//             std::string CPluginEntityJsonDefinition::m_pluginNameIdentifier = "name";
//             std::string CPluginEntityJsonDefinition::m_pluginPluginNameIdentifier = "pluginName";
//             std::string CPluginEntityJsonDefinition::m_pluginConfigurationIdentifier = "configuration";
//             std::string CPluginEntityJsonDefinition::m_pluginEnabledIdentifier = "enabled";
//
//             //implementation of class CPluginEntitySerializer
//             CPluginEntitySerializer::CPluginEntitySerializer()
//             {
//             }
//             
//             CPluginEntitySerializer::~CPluginEntitySerializer()
//             {
//             }
//             
//             // IEntitySerializer implementation
//             CJson CPluginEntitySerializer::serialize(const CPlugin & object)
//             {
//                CJson result;
//                result.put(CPluginEntityJsonDefinition::getIdIdentifier(), object.getId());
//                result.put(CPluginEntityJsonDefinition::getEnabledIdentifier(), object.getEnabled());
//                result.put(CPluginEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
//                result.put(CPluginEntityJsonDefinition::getNameIdentifier(), object.getName());
//                result.put(CPluginEntityJsonDefinition::getPluginNameIdentifier(), object.getPluginName());
//                return result;
//             }
//             // [END] IEntitySerializer implementation
//             
//             
//             // IEntityDeserializer implementation
//             CPlugin CPluginEntitySerializer::deserialize(const CJson & object)
//             {
//                CPlugin entity;
//             
//                if(object.find(CPluginEntityJsonDefinition::getIdIdentifier()) != object.not_found())
//                   entity.setId(object.get<int>(CPluginEntityJsonDefinition::getIdIdentifier()));
//             
//                if(object.find(CPluginEntityJsonDefinition::getEnabledIdentifier()) != object.not_found())
//                   entity.setEnabled(object.get<bool>(CPluginEntityJsonDefinition::getEnabledIdentifier()));
//             
//                if(object.find(CPluginEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found())
//                   entity.setConfiguration(object.get<std::string>(CPluginEntityJsonDefinition::getConfigurationIdentifier()));
//             
//                if(object.find(CPluginEntityJsonDefinition::getNameIdentifier()) != object.not_found())
//                   entity.setName(object.get<std::string>(CPluginEntityJsonDefinition::getNameIdentifier()));
//             
//                if(object.find(CPluginEntityJsonDefinition::getPluginNameIdentifier()) != object.not_found())
//                   entity.setPluginName(object.get<std::string>(CPluginEntityJsonDefinition::getPluginNameIdentifier()));
//             
//                return entity;
//             }



//
/// \brief The column identifier in input sequence
//
#define JSON_COLUMN_ID 0
//
/// \brief The column name in input sequence
//
#define JSON_COLUMN_NAME 1
//
/// \brief The column name in input sequence
//
#define JSON_COLUMN_TYPE 2
//
/// \brief The column converter FROM in input sequence
//
#define JSON_COLUMN_CONVERT_FROM 3
//
/// \brief The column converter TO in input sequence
//
#define JSON_COLUMN_CONVERT_TO 4


#define JSON_NO_CONV(x) x
#define JSON_FROM_DATE(x) boost::posix_time::time_from_string(x)
#define JSON_TO_DATE(x) boost::posix_time::to_simple_string(x)

#define JSON_INT (int)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_FLOAT (float)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_DOUBLE (double)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_BOOL (bool)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_DATE (std::string)(JSON_FROM_DATE)(JSON_TO_DATE)
#define JSON_STRING (std::string)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_ENUM(_e) (int)((_e))((int))

//-------------------------------------------------------
///\brief Macro which provide the entity class:
///       Example for _classname= Plugin -> CPlugin
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_CLASS(_classname) BOOST_PP_CAT(database::entities::C,_classname)

//-------------------------------------------------------
///\brief Macro which provide the entity definition class:
///       Example for _classname= Plugin -> CPluginEntityJsonDefinition
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname) BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntityJsonDefinition))

//-------------------------------------------------------
///\brief Macro which provide the entity serializer class:
///       Example for _classname= Plugin -> CPluginEntitySerializer
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname) BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntitySerializer))

//-------------------------------------------------------
/// \brief Macro which declares the static member
//-------------------------------------------------------
#define DECLARE_JSON_FIELD_NAME(r, data, elem) static std::string BOOST_PP_CAT(BOOST_PP_CAT(m_, elem), Identifier);

//-------------------------------------------------------
/// \brief Macro which declares a getter (used by foreach)
//-------------------------------------------------------
#define DECLARE_JSON_FIELD_NAME_GETTER(r, data, elem) \
   static const std::string & BOOST_PP_CAT(BOOST_PP_CAT(get, elem), Identifier()) { return BOOST_PP_CAT(BOOST_PP_CAT(m_, elem), Identifier); }

//-------------------------------------------------------
/// \brief  Macro which declares all the getters
///         For each ID in input sequence, call DECLARE_JSON_FIELD_NAME_GETTER
//-------------------------------------------------------
#define DECLARE_JSON_GETTERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_FIELD_NAME_GETTER, _, _seq)

//-------------------------------------------------------
/// \brief  Macro which declares all the getters
///         For each ID in input sequence, call DECLARE_JSON_COLUMN_NAME
//-------------------------------------------------------
#define DECLARE_JSON_MEMBERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_FIELD_NAME, _, _seq)


//-------------------------------------------------------
/// \brief  Declare one static value (used by BOOST_PP_SEQ_FOR_EACH_I)
//-------------------------------------------------------
#define DELCARE_STATIC_JSON_IDENTIFIER(r, classname, elem) \
   std::string BOOST_PP_CAT(BOOST_PP_CAT(C##classname##EntityJsonDefinition::m_ , BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier) = BOOST_PP_SEQ_ELEM(JSON_COLUMN_NAME, elem);


//-------------------------------------------------------
/// \brief  Declare a EntityJsonDefinition
/// \param  name  the EntityJsonDefinition name
/// \param  _seq  the sequence of columns
//-------------------------------------------------------
#define DECLARE_JSON_DEFINITION(name, _seq) \
class BOOST_PP_CAT(C, BOOST_PP_CAT(name, EntityJsonDefinition)) \
{ \
public:\
   DECLARE_JSON_GETTERS(_seq) \
   \
private:\
   DECLARE_JSON_MEMBERS(_seq) \
};

//-------------------------------------------------------
/// \brief  Initialize all static members of the table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//-------------------------------------------------------
#define DECLARE_JSON_IDENTIFIER(_classname, _tablename, _seq) \
   BOOST_PP_SEQ_FOR_EACH(DELCARE_STATIC_JSON_IDENTIFIER, _classname, _seq)


//-------------------------------------------------------
///\brief   Declare the JSON serializer DEFINITION
///       write the class header 
//-------------------------------------------------------   
#define DECLARE_JSON_SERIALIZER(_classname) \
class BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntitySerializer)) : public IEntitySerializer< DECLARE_JSON_ENTITY_CLASS(_classname) >, public IEntityDeserializer< DECLARE_JSON_ENTITY_CLASS(_classname) > \
{ \
public:\
   BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntitySerializer))(); \
   virtual BOOST_PP_CAT(~C, BOOST_PP_CAT(_classname, EntitySerializer))(); \
   \
   CJson serialize(const DECLARE_JSON_ENTITY_CLASS(_classname) & object); \
   boost::shared_ptr< DECLARE_JSON_ENTITY_CLASS(_classname) > deserialize(const CJson & object); \
 };   

 
//-------------------------------------------------------
///\brief   Declare the JSON serializer for one field IMPLEMENTATION
///       write the serialize() part for one field
//-------------------------------------------------------
#define DECLARE_JSON_SERIALIZER_FIELD_CONTENT(r, _classname, elem) \
   result.put(DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname)::BOOST_PP_CAT(BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier()), BOOST_PP_SEQ_ELEM(JSON_COLUMN_CONVERT_TO, elem)(object.BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem))()));

//-------------------------------------------------------
///\brief   Declare the JSON serializer IMPLEMENTATION
///       write the ctor and dtor  method
///       write the serialize() method
//-------------------------------------------------------
#define DECLARE_JSON_SERIALIZER_CONTENT(_classname, _seq) \
 DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)() { } \
 DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::~DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)() { } \
CJson DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::serialize(const DECLARE_JSON_ENTITY_CLASS(_classname) & object) \
{ \
   CJson result; \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_SERIALIZER_FIELD_CONTENT, _classname, _seq) \
   return result; \
}
 
 
//-------------------------------------------------------
///\brief   Declare the JSON deserializer ofr one field IMPLEMENTATION
///       write the deserialize() part for one field
//-------------------------------------------------------
#define DECLARE_JSON_DESERIALIZER_FIELD_CONTENT(r, _classname, elem) \
    if(object.find(DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname)::BOOST_PP_CAT(BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier())) != object.not_found())\
      entity->BOOST_PP_CAT(set, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem))(BOOST_PP_SEQ_ELEM(JSON_COLUMN_CONVERT_FROM, elem)(object.get<BOOST_PP_SEQ_ELEM(JSON_COLUMN_TYPE, elem)>(DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname)::BOOST_PP_CAT(BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier()))));\
  
//-------------------------------------------------------
///\brief   Declare the JSON deserializer IMPLEMENTATION
///       write the deserialize() method
//-------------------------------------------------------
#define DECLARE_JSON_DESERIALIZER_CONTENT(_classname, _seq) \
boost::shared_ptr< DECLARE_JSON_ENTITY_CLASS(_classname) > DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::deserialize(const CJson & object)\
{\
   boost::shared_ptr< DECLARE_JSON_ENTITY_CLASS(_classname) > entity(new DECLARE_JSON_ENTITY_CLASS(_classname)() );\
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_DESERIALIZER_FIELD_CONTENT, _classname, _seq) \
   return entity;\
}

//-------------------------------------------------------
///\brief   Declare the JSON serializer IMPLEMENTATION
///            -> declare serializer implementation
///            -> declare deserializer implementation
//-------------------------------------------------------
#define DECLARE_JSON_SERIALIZER_IMPLEMENTATION(_classname, _seq)\
   DECLARE_JSON_SERIALIZER_CONTENT(_classname, _seq)\
   DECLARE_JSON_DESERIALIZER_CONTENT(_classname, _seq)
 

//-------------------------------------------------------
///\brief   Declare the JSON entity classes header
///         -> a class CxxxxEntityJsonDefinition which contains the name of each field (static declaration)
///         Example : for entity class CPlugin :
///               class CPluginEntityJsonDefinition 
///               { 
///               public: 
///               	static const std::string & getIdIdentifier() { return m_IdIdentifier; } 
///               	static const std::string & getNameIdentifier() { return m_NameIdentifier; } 
///               	static const std::string & getPluginNameIdentifier() { return m_PluginNameIdentifier; } 
///               	static const std::string & getConfigurationIdentifier() { return m_ConfigurationIdentifier; } 
///               	static const std::string & getEnabledIdentifier() { return m_EnabledIdentifier; } 
///               private: 
///               	static std::string m_IdIdentifier; 
///               	static std::string m_NameIdentifier; 
///               	static std::string m_PluginNameIdentifier; 
///               	static std::string m_ConfigurationIdentifier; 
///               	static std::string m_EnabledIdentifier; 
///               };
///
///
///
///         -> a class CxxxxEntitySerializer which contains the entity serializer
///               class CPluginEntitySerializer : public IEntitySerializer<CPlugin>, public IEntityDeserializer<CPlugin> 
///               { 
///               public: 
///               	CPluginEntitySerializer(); 
///               	virtual ~CPluginEntitySerializer(); 
///               	
///               	CJson serialize(const CPlugin & object); 
///               	boost::shared_ptr<CPlugin> deserialize(const CJson & object); 
///               };
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_HEADER(_classname, _seq)\
   DECLARE_JSON_DEFINITION(_classname, _seq)\
   DECLARE_JSON_SERIALIZER(_classname)

//-------------------------------------------------------
///\brief   Declare the JSON entity classes implementation
///         -> the class CxxxxEntityJsonDefinition static fields definition 
///             Example : for entity class CPlugin :
///                   std::string CPluginEntityJsonDefinition::m_IdIdentifier = "id" ;
///                   std::string CPluginEntityJsonDefinition::m_NameIdentifier = "name" ;
///                   std::string CPluginEntityJsonDefinition::m_PluginNameIdentifier = "pluginName" ;
///                   std::string CPluginEntityJsonDefinition::m_ConfigurationIdentifier = "configuration" ;
///                   std::string CPluginEntityJsonDefinition::m_EnabledIdentifier = "enabled" ;
///                   
///               
///         -> the class CxxxxxEntitySerializer implementation
///             Example : for entity class CPlugin : 
///                  CPluginEntitySerializer::CPluginEntitySerializer()
///                  {
///                  }
///                  
///                  CPluginEntitySerializer::~CPluginEntitySerializer()
///                  {
///                  }
///                  
///                  // IEntitySerializer implementation
///                  CJson CPluginEntitySerializer::serialize(const CPlugin & object)
///                  {
///                     CJson result;
///                     result.put(CPluginEntityJsonDefinition::getIdIdentifier(), object.getId());
///                     result.put(CPluginEntityJsonDefinition::getEnabledIdentifier(), object.getEnabled());
///                     result.put(CPluginEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
///                     result.put(CPluginEntityJsonDefinition::getNameIdentifier(), object.getName());
///                     result.put(CPluginEntityJsonDefinition::getPluginNameIdentifier(), object.getPluginName());
///                     return result;
///                  }
///                  
///                  boost::shared_ptr<CPlugin> CPluginEntitySerializer::deserialize(const CJson & object)
///                  {
///                     CPlugin entity;
///                  
///                     if(object.find(CPluginEntityJsonDefinition::getIdIdentifier()) != object.not_found())
///                        entity.setId(object.get<int>(CPluginEntityJsonDefinition::getIdIdentifier()));
///                  
///                     if(object.find(CPluginEntityJsonDefinition::getEnabledIdentifier()) != object.not_found())
///                        entity.setEnabled(object.get<bool>(CPluginEntityJsonDefinition::getEnabledIdentifier()));
///                  
///                     if(object.find(CPluginEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found())
///                        entity.setConfiguration(object.get<std::string>(CPluginEntityJsonDefinition::getConfigurationIdentifier()));
///                  
///                     if(object.find(CPluginEntityJsonDefinition::getNameIdentifier()) != object.not_found())
///                        entity.setName(object.get<std::string>(CPluginEntityJsonDefinition::getNameIdentifier()));
///                  
///                     if(object.find(CPluginEntityJsonDefinition::getPluginNameIdentifier()) != object.not_found())
///                        entity.setPluginName(object.get<std::string>(CPluginEntityJsonDefinition::getPluginNameIdentifier()));
///                  
///                     return entity;
///                  }
#define DECLARE_JSON_ENTITY_IMPLEMENTATION(_classname, _usualname, _seq)\
   DECLARE_JSON_IDENTIFIER(_classname, _usualname, _seq)\
   DECLARE_JSON_SERIALIZER_IMPLEMENTATION(_classname, _seq)


