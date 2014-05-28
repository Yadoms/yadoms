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
//                      CField< int  > Id;
//                	   CField< int  > IdPage;
//                	   CField< std::string  > Name;
//                	   CField< int  > SizeX;
//                	   CField< int  > SizeY;
//                	   CField< int  > PositionX;
//                	   CField< int  > PositionY;
//                	   CField< std::string  > Configuration;
//                };
//                

#define ENTITY_COLUMN_NAME   0
#define ENTITY_COLUMN_TYPE   1
#define ENTITY_COLUMN_DEFAULT   2

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
   CField< fieldType > ENTITY_FIELD(fieldName);

      
//
/// \brief Macro which initialize a field filled status to false (the field is not yet filled)
//
#define DECLARE_ENTITY_FILLED_INITIALISER(r, data, idx, elem) \
   BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(idx,0)) \
   ENTITY_FIELD(BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_NAME, elem))(BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_DEFAULT, elem))
      
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
   DELCARE_FIELD(_classname, BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_TYPE, elem), BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_NAME, elem))
 
//
/// \brief  Macro which declare all fields and its filled status
///         For each ID in input sequence, call DECLARE_ENTITY_FILLED_INITIALISER
//
#define DECLARE_ENTITY_FIELDS(_classname, _seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_ENTITY_FIELD, _classname, _seq)      



   
//
/// \brief  Declare a table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//
#define DECLARE_ENTITY_CLASS(_classname, _seq)                               \
class ENTITY_CLASSNAME(_classname)                                           \
{                                                                            \
   public:                                                                   \
      ENTITY_CLASSNAME(_classname)()                                         \
        : DECLARE_ENTITY_FILLED_INITIALISERS(_seq)                           \
      {                                                                      \
      }                                                                      \
      virtual ~ENTITY_CLASSNAME(_classname)()                                \
      {                                                                      \
      }                                                                      \
public:                                                                      \
   DECLARE_ENTITY_FIELDS(_classname, _seq)                                   \
};
