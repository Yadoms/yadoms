#pragma once

#include <shared/DataContainer.h>
#include <shared/exception/NotSupported.hpp>


#include "OpenZWaveNodeKeywordBase.h"
#include "OpenZWaveNodeKeywordGeneric.h"
#include "OpenZWaveNodeKeywordDimmable.h"

//--------------------------------------------------------------
/// \brief	Interface for ZWave controller
//--------------------------------------------------------------
class CConfigurationSchemaFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	Virtual destructor
   //--------------------------------------------------------------
   virtual ~CConfigurationSchemaFactory() {}

   template<class T>
   static inline shared::CDataContainer generateForHistorizer(const T & historizer);

private:
   static shared::CDataContainer generateForDouble(const std::string & name, const std::string & description, double min, double max, double units, double precision);
};


template<class T>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const T & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}

template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<double> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
   //return CConfigurationSchemaFactory::generateForDouble(historizer.getKeyword()->getKeyword());
}

template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<Poco::Int8> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}

template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<Poco::Int16> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}

template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<Poco::Int32> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}


template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<Poco::UInt8> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}

template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<Poco::UInt16> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}

template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<Poco::UInt32> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}



template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<bool> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}



template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<std::string> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}




template<>
inline shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(const COpenZWaveNodeKeywordGeneric<COpenZWaveEnumHandler> & historizer)
{
   throw shared::exception::CNotSupported("This historizer");
}


