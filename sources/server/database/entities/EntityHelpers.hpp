#pragma once

//
// This file provide macros used to declare database entities
//
// An entity is a container class to encapsulate of database row (if using database)
//
// Macros :
//          DECLARE_ENTITY_CLASS(entityName,
//             ((entityField1)(entityField1Type)(entityField1DefaultValue)),
//             ((entityField2)(entityField1Type)(entityField1DefaultValue)),
//             ...
//             )
//
//             generate the entoty class 
//
//
// ==========================================================================
//        HEADER PART
// ==========================================================================
//
// Example :
//
//   Input :
//            DECLARE_ENTITY_CLASS(Widget,
//               ((Id)(int)(0))
//               ((IdPage)(int)(0))
//               ((Name)(std::string)(""))
//               ((SizeX)(int)(1))
//               ((SizeY)(int)(1))
//               ((PositionX)(int)(1))
//               ((PositionY)(int)(1))
//               ((Configuration)(std::string)(""))
//               )
//
//    Ouptut :
//                
//                class CWidget {
//                	public: 
//                      
//                      CWidget() 
//                      :  Id (0 ) , 
//                         IdPage (0 ) , 
//                         Name ("" ) , 
//                         SizeX (1 ) , 
//                         SizeY (1 ) , 
//                         PositionX (1 ) , 
//                         PositionY (1 ) , 
//                         Configuration ("" ) 
//                      {
//                	   }
//                	
//                      virtual ~CWidget() 
//                      {
//                	   }
//                	public: 
//                      shared::CField< int  > Id;
//                	   shared::CField< int  > IdPage;
//                	   shared::CField< std::string  > Name;
//                	   shared::CField< int  > SizeX;
//                	   shared::CField< int  > SizeY;
//                	   shared::CField< int  > PositionX;
//                	   shared::CField< int  > PositionY;
//                	   shared::CField< std::string  > Configuration;
//                };
//                

#define ENTITY_COLUMN_NAME   0
#define ENTITY_COLUMN_TYPE   1
#define ENTITY_COLUMN_DEFAULT   2
#define ENTITY_COLUMN_SERIALIZATION   3

//
/// \brief Macro used to declare the entity class name
//
#define ENTITY_CLASSNAME(_classname) C##_classname


//
/// \brief Macro used to declare the entity field filled status
//
#define ENTITY_FIELD(_fieldName) _fieldName


//
/// \brief Macro used to declare the entity field filled status
//
#define ENTITY_FIELD_FILLED(_fieldName) BOOST_PP_CAT(BOOST_PP_CAT(m_, _fieldName), _filled)

//
/// \brief Macro used to declare a private field with public getter and setter
//
#define DELCARE_FIELD(className, fieldType, fieldName) \
   shared::CField< fieldType > ENTITY_FIELD(fieldName);


#define ENTITY_FIELD_NAME(_seq) BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_NAME, _seq)
#define ENTITY_FIELD_TYPE(_seq) BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_TYPE, _seq)
#define ENTITY_FIELD_DEFAULT(_seq) BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_DEFAULT, _seq)
#define ENTITY_FIELD_SERIALIZATION_IDENTIFIER(_seq) BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_SERIALIZATION, _seq)


//
/// \brief Macro which initialize a field filled status to false (the field is not yet filled)
//
#define DECLARE_ENTITY_FILLED_INITIALISER(r, data, idx, elem) \
   BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(idx,0)) \
   ENTITY_FIELD(ENTITY_FIELD_NAME(elem))(ENTITY_FIELD_DEFAULT(elem))
      
//
/// \brief  Macro which initialize all fields state to false
///         For each ID in input sequence, call DECLARE_ENTITY_FILLED_INITIALISER
//
#define DECLARE_ENTITY_FILLED_INITIALISERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH_I(DECLARE_ENTITY_FILLED_INITIALISER, _, _seq)      


//
/// \brief  Macro which declare one field and its filled status
//
#define DECLARE_ENTITY_FIELD(r, _classname, elem) \
   DELCARE_FIELD(_classname, ENTITY_FIELD_TYPE(elem), ENTITY_FIELD_NAME(elem))
 
//
/// \brief  Macro which declare all fields and its filled status
///         For each ID in input sequence, call DECLARE_ENTITY_FILLED_INITIALISER
//
#define DECLARE_ENTITY_FIELDS(_classname, _seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_ENTITY_FIELD, _classname, _seq)      



 


//
/// \brief  Macro which declare all fields and its filled status
///         For each ID in input sequence, call DECLARE_ENTITY_FILLED_INITIALISER
//
#define DECLARE_ENTITY_DATACONTAINABLE_HEADER()                                      \
      void extractContent(shared::CDataContainer & containerToFill) const override;  \
      void fillFromContent(const shared::CDataContainer & initialData) override;     \
      void fillFromSerializedString(const std::string & serializedData) override;




//-------------------------------------------------------
///\brief   Declare the JSON serializer IMPLEMENTATION
///            -> declare serializer implementation
///            -> declare deserializer implementation
//-------------------------------------------------------
#define DECLARE_ENTITY_DATACONTAINABLE_IMPLEMENTATION(_classname, _seq)             \
   DECLARE_ENTITY_EXTRACT_CONTENT(_classname, _seq)                                 \
   DECLARE_ENTITY_FILL_CONTENT(_classname, _seq)


//
/// \brief  Declare a table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//
#define DECLARE_ENTITY_CLASS_HEADER(_classname, _seq)                                     \
   class ENTITY_CLASSNAME(_classname) : public shared::IDataContainable                   \
   {                                                                                      \
   public:                                                                                \
         ENTITY_CLASSNAME(_classname)();                                                  \
         virtual ~ENTITY_CLASSNAME(_classname)();                                         \
         DECLARE_ENTITY_DATACONTAINABLE_HEADER()                                          \
   public:                                                                                \
         DECLARE_ENTITY_FIELDS(_classname, _seq)                                          \
   };












//-------------------------------------------------------
///\brief   Declare the JSON deserializer ofr one field IMPLEMENTATION
///       write the deserialize() part for one field
//-------------------------------------------------------
#define DECLARE_ENTITY_FILL_FIELD_CONTENT(r, _classname, elem)                                                                                                    \
    if(initialData.exists(ENTITY_FIELD_SERIALIZATION_IDENTIFIER(elem)))                                                                                         \
      ENTITY_FIELD_NAME(elem) = initialData.get< ENTITY_FIELD_TYPE(elem) >(ENTITY_FIELD_SERIALIZATION_IDENTIFIER(elem));



//-------------------------------------------------------
///\brief   Declare the JSON deserializer IMPLEMENTATION
///       write the deserialize() method
//-------------------------------------------------------
#define DECLARE_ENTITY_FILL_CONTENT(_classname, _seq)                                                 \
   void ENTITY_CLASSNAME(_classname)::fillFromContent(const shared::CDataContainer & initialData)     \
   {                                                                                                  \
      BOOST_PP_SEQ_FOR_EACH(DECLARE_ENTITY_FILL_FIELD_CONTENT, _classname, _seq)                      \
   }                                                                                                  \
   void ENTITY_CLASSNAME(_classname)::fillFromSerializedString(const std::string & serializedData)    \
   {                                                                                                  \
      shared::CDataContainer deserializeData(serializedData);                                         \
      fillFromContent(deserializeData);                                                               \
   }





//-------------------------------------------------------
///\brief   Declare the JSON serializer for one field IMPLEMENTATION  when the content is treated as a child
//-------------------------------------------------------
#define DECLARE_ENTITY_EXTRACT_FIELD_CONTENT(r, _classname, elem)                                                                                              \
   containerToFill.set< ENTITY_FIELD_TYPE(elem) >( ENTITY_FIELD_SERIALIZATION_IDENTIFIER(elem), ENTITY_FIELD_NAME(elem)());


//-------------------------------------------------------
///\brief   Declare the JSON serializer IMPLEMENTATION
///       write the ctor and dtor  method
///       write the serialize() method
//-------------------------------------------------------
#define DECLARE_ENTITY_EXTRACT_CONTENT(_classname, _seq)                                              \
   void ENTITY_CLASSNAME(_classname)::extractContent(shared::CDataContainer & containerToFill)  const \
   {                                                                                                  \
      BOOST_PP_SEQ_FOR_EACH(DECLARE_ENTITY_EXTRACT_FIELD_CONTENT, _classname, _seq)                   \
   }








//
/// \brief  Declare a table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//
#define DECLARE_ENTITY_CLASS_IMPLEMENTATION(_classname, _seq)                             \
   ENTITY_CLASSNAME(_classname)::ENTITY_CLASSNAME(_classname)()                           \
   : DECLARE_ENTITY_FILLED_INITIALISERS(_seq)                                             \
   {                                                                                      \
   }                                                                                      \
                                                                                          \
   ENTITY_CLASSNAME(_classname)::~ENTITY_CLASSNAME(_classname)()                          \
   {                                                                                      \
   }                                                                                      \
                                                                                          \
   DECLARE_ENTITY_DATACONTAINABLE_IMPLEMENTATION(_classname, _seq)                             

