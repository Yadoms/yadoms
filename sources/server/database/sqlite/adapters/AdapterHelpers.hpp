#pragma once

//
/// \brief Macro used by adapter to map a column name to an entity setter
//
#define MAP_COLMUN_ADAPTER_IF(currentColumnName, fieldColumnName, entity, fieldName, adapter) \
   if(boost::iequals(std::string(fieldColumnName), std::string(currentColumnName)))\
      entity->set##fieldName( adapter(columValues[i]) );

#define MAP_COLMUN_IF(currentColumnName, fieldColumnName, entity, fieldName) \
   if(boost::iequals(std::string(fieldColumnName), std::string(currentColumnName)))\
      entity->set##fieldName( columValues[i] );