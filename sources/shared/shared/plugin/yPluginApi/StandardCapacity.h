#pragma once
#include <shared/plugin/yPluginApi/KeywordDataType.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //--------------------------------------------------------------
         /// \brief		Inner class which describe a standard capacity
         //--------------------------------------------------------------
         class CStandardCapacity
         {
         public:
            //--------------------------------------------------------------
            /// \brief		Constructor
            /// \param [in] name          the capacity name
            /// \param [in] unit          the capacity unit
            /// \param [in] type          the capacity type
            //--------------------------------------------------------------
            CStandardCapacity(const std::string& name,
                              const std::string& unit,
                              const EKeywordDataType& type);

            //--------------------------------------------------------------
            /// \brief		Destructor
            //--------------------------------------------------------------
            virtual ~CStandardCapacity();

            //--------------------------------------------------------------
            /// \brief		get the capacity name
            /// \return 	the capacity name
            //--------------------------------------------------------------
            const std::string& getName() const;

            //--------------------------------------------------------------
            /// \brief		get the capacity unit
            /// \return 	the capacity unit
            //--------------------------------------------------------------
            const std::string& getUnit() const;

            //--------------------------------------------------------------
            /// \brief		get the capacity type
            /// \return 	the capacity type
            //--------------------------------------------------------------
            const EKeywordDataType& getType() const;

            //--------------------------------------------------------------
            /// \brief		Implicit std::string operator (give the capacity name)
            /// \return 	the capacity name
            //--------------------------------------------------------------
            operator const std::string &() const;

         private:
            //--------------------------------------------------------------
            /// \brief		the capacity name
            //--------------------------------------------------------------
            std::string m_name;

            //--------------------------------------------------------------
            /// \brief		the capacity unit
            //--------------------------------------------------------------
            std::string m_unit;

            //--------------------------------------------------------------
            /// \brief		the capacity type
            //--------------------------------------------------------------
            EKeywordDataType m_type;
         };
      }
   }
} // namespace shared::plugin::yPluginApi

#define DECLARE_CAPACITY(cppName, name, unit, keywordDataType) \
   const shared::plugin::yPluginApi::CStandardCapacity& cppName() \
   { \
      static const shared::plugin::yPluginApi::CStandardCapacity capacity( \
         name, \
         unit, \
         keywordDataType); \
      return capacity; \
   }


