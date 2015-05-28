#pragma once

namespace update {
   namespace worker {

      class CWidget
      {
      public:
         //---------------------------------
         ///\brief Define a function prototype for updating the worker progress
         //---------------------------------
         typedef boost::function3<void, bool, boost::optional<float>, std::string > WorkerProgressFunc;

         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] progressCallback The progress callback
         //---------------------------------------------
         CWidget(WorkerProgressFunc progressCallback);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CWidget();

         //---------------------------------------------
         ///\brief   Install a new widget
         ///\param [in] downloadUrl The plugin package url
         //---------------------------------------------
         void install(const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Update a widget
         ///\param [in] widgetName  The widget name
         ///\param [in] downloadUrl The widget package url
         //---------------------------------------------
         void update(const std::string & widgetName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Remove a widget
         ///\param [in] widgetName  The widget name
         //---------------------------------------------
         void remove(const std::string & widgetName);

      private:

         //---------------------------------------------
         ///\brief   The progress callback
         //---------------------------------------------
         WorkerProgressFunc m_progressCallback;
      };

   } // namespace worker
} // namespace update
