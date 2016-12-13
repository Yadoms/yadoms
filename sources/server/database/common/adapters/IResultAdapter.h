#pragma once

#include "database/common/IResultHandler.h"

namespace database
{
   namespace common
   {
      namespace adapters
      {
         //--------------------------------------------------------------
         /// \Brief		Interface for result adapters
         ///				The aim is to provide method to adapt sqlite results
         ///				to a list of entities
         //--------------------------------------------------------------
         class IResultAdapter
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------   
            virtual ~IResultAdapter()
            {
            }

            //--------------------------------------------------------------
            /// \Brief		   Adapt a database results (single row) to entity
            /// \param [in]	column         Number of columns in results
            /// \param [in]	columnName     The column values
            /// \param [in]	columnName     The column names
            /// \return       true if adapt is realized with success, else false
            //--------------------------------------------------------------
            virtual bool adapt(boost::shared_ptr<IResultHandler> resultHandler) = 0;
         };

         //--------------------------------------------------------------
         /// \Brief		Interface for result adapters
         ///				The aim is to provide method to adapt sqlite results
         ///				to a list of entities
         //--------------------------------------------------------------
         template <class T>
         class IResultAdapterEx : public IResultAdapter
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~IResultAdapterEx()
            {
            }

            //--------------------------------------------------------------
            /// \Brief		   Adapt a database results (single row) to entity
            /// \param [in]	column         Number of columns in results
            /// \param [in]	columnName     The column values
            /// \param [in]	columnName     The column names
            /// \return       true if adapt is realized with success, else false
            //--------------------------------------------------------------
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override = 0;

            //--------------------------------------------------------------
            /// \Brief		   Get the full results (vector of entities)
            //--------------------------------------------------------------
            virtual std::vector<T> getResults() = 0;
         };
      } //namespace adapters
   } //namespace common
} //namespace database 


