#pragma once
#include <shared/plugin/yadomsApi/ICustomQueryData.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IDeviceCommand implementation
   //-----------------------------------------------------
   class CCustomQueryData : public shared::plugin::yadomsApi::ICustomQueryData
   {
   public:
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] query              The query
      //-----------------------------------------------------
      CCustomQueryData(const std::string& query);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CCustomQueryData();

      // IManuallyDeviceCreationData implementation
      virtual const std::string& getQuery() const;
      // [END] IManuallyDeviceCreationData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Query
      //-----------------------------------------------------
      std::string m_query;
   };  
} // namespace pluginSystem	
	
	