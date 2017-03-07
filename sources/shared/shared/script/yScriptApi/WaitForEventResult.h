#pragma once

namespace shared
{
   namespace script
   {
      namespace yScriptApi
      {
         //--------------------------------------------------------------
         /// \brief		Class wich contains results for IYScriptApi.WaitforEvent
         //--------------------------------------------------------------
         class CWaitForEventResult
         {
         public:
            //-----------------------------------------------------
            ///\brief Constructor
            //-----------------------------------------------------
            CWaitForEventResult()
               : m_type(kTimeout), m_keywordId(-1)
            {
            }

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~CWaitForEventResult()
            {
            }

            //--------------------------------------------------------------
            /// \brief		Eumeration of result types
            //--------------------------------------------------------------
            typedef enum
            {
               //--------------------------------------------------------------
               /// \brief		Wait for event returns from a timeout
               //--------------------------------------------------------------
               kTimeout = 0,
               //--------------------------------------------------------------
               /// \brief		Wait for event returns from a keyword update
               //--------------------------------------------------------------
               kKeyword = 1,
               //--------------------------------------------------------------
               /// \brief		Wait for event returns from a date/time update
               //--------------------------------------------------------------
               kDateTime = 2
            } EResultType;

            //--------------------------------------------------------------
            /// \brief		Get the result type
            /// \return		The result type
            //--------------------------------------------------------------
            EResultType getType() const
            {
               return m_type;
            }

            //--------------------------------------------------------------
            /// \brief		         Set the type
            /// \param [in]   type	The result type
            //--------------------------------------------------------------
            void setType(EResultType type)
            {
               m_type = type;
            }

            //--------------------------------------------------------------
            /// \brief		Get the result keyword id (only available for type=kKeyword)
            /// \return		The result keyword id
            //--------------------------------------------------------------
            int getKeywordId() const
            {
               return m_keywordId;
            }

            //--------------------------------------------------------------
            /// \brief		               Set the keywordId
            /// \param [in]   keywordId	The result keywordId
            //--------------------------------------------------------------
            void setKeywordId(const int keywordId)
            {
               m_keywordId = keywordId;
            }

            //--------------------------------------------------------------
            /// \brief		Get the result value
            /// \return		The result value
            //--------------------------------------------------------------
            std::string getValue() const
            {
               return m_value;
            }

            //--------------------------------------------------------------
            /// \brief		            Set the result value
            /// \param [in]   value		The result value
            //--------------------------------------------------------------
            void setValue(const std::string& value)
            {
               m_value = value;
            }


         private:

            //--------------------------------------------------------------
            /// \brief		Result Type
            //--------------------------------------------------------------
            EResultType m_type;

            //--------------------------------------------------------------
            /// \brief		Result keywordId
            //--------------------------------------------------------------
            int m_keywordId;

            //--------------------------------------------------------------
            /// \brief		Result value (maybe keywordValue, or datetime value, depending on the type)
            //--------------------------------------------------------------
            std::string m_value;
         };
      }
   }
} // namespace shared::script::yScriptApi
