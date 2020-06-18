#pragma once
#include <shared/DataContainer.h>
#include <shared/http/FileDownloader.h>
#include <functional>
#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace update
{
   namespace worker
   {
      class CWorkerHelpers
      {
      public:
         CWorkerHelpers() = delete;
         ~CWorkerHelpers() = default;

         //---------------------------------
         ///\brief Define a function prototype for updating the worker progress
         //    -> return : void
         //    -> param 1 : bool                      : tells if the task if still active (true: running/success, false : error)
         //    -> param 2 : float                     : the task progression (as percent)
         //    -> param 3 : std::string               : the task step message (should be i18n messages)
         //    -> param 4 : std::string               : the exception message (should not be i18n messages; only when param1 is false)
         //    -> param 5 : shared::CDataContainer    : Some free data provided by task implementation (specific for each task)
         //---------------------------------
         typedef std::function<void(bool, boost::optional<float>, std::string, std::string, boost::shared_ptr<shared
                                       ::CDataContainer>)> WorkerProgressFunc;

         //---------------------------------------------
         ///\brief   Download a package
         ///\param [in] downloadUrl The downloaded package URL
         ///\param [in] callback    The callback to use
         ///\param [in] function    The i18n string to send to callback (ex: update.plugin.download)
         ///\param [in] min         The global progression when download start
         ///\param [in] max         The global progression when download ends
         ///\return The package local path
         //---------------------------------------------
         static boost::filesystem::path downloadPackage(const std::string& downloadUrl,
                                                        WorkerProgressFunc callback,
                                                        const std::string& function,
                                                        float min,
                                                        float max);

         //---------------------------------------------
         ///\brief   Download a package
         ///\param [in] downloadUrl The downloaded package URL
         ///\param [in] md5Hash     The expected md5hash
         ///\param [in] callback    The callback to use
         ///\param [in] function    The i18n string to send to callback (ex: update.plugin.download)
         ///\param [in] min         The global progression when download start
         ///\param [in] max         The global progression when download ends
         ///\return The package local path
         //---------------------------------------------
         static boost::filesystem::path downloadPackageAndVerify(const std::string& downloadUrl,
                                                                 const std::string& md5Hash,
                                                                 WorkerProgressFunc callback,
                                                                 const std::string& function,
                                                                 float min,
                                                                 float max);

         //---------------------------------------------
         ///\brief   Download a package (report progress to log)
         ///\param [in] downloadUrl  The downloaded package URL
         ///\return The package local path
         //---------------------------------------------
         static boost::filesystem::path downloadPackage(const std::string& downloadUrl);

         //---------------------------------------------
         ///\brief   Download a package
         ///\param [in] downloadUrl       The downloaded package URL
         ///\param [in] progressReporter  The progress report callback
         ///\return The package local path
         //---------------------------------------------
         static boost::filesystem::path downloadPackage(const std::string& downloadUrl,
                                                        shared::http::CFileDownloader::onProgressFunc progressReporter);

         //---------------------------------------------
         ///\brief   Download a package
         ///\param [in] downloadUrl       The downloaded package URL
         ///\param [in] md5Hash           The expected md5 hash
         ///\param [in] progressReporter  The progress report callback
         ///\return The package local path
         //---------------------------------------------
         static boost::filesystem::path downloadPackageAndVerify(const std::string& downloadUrl,
                                                                 const std::string& md5Hash,
                                                                 shared::http::CFileDownloader::onProgressFunc
                                                                 progressReporter);

         //---------------------------------------------
         ///\brief   Deploy a package
         ///\param [in] downloadedPackage  The downloaded package
         ///\param [in] outputDirectory  The output base path (plugins folder for example)
         ///\return The deployed directory
         //---------------------------------------------
         static boost::filesystem::path deployPackage(const boost::filesystem::path& downloadedPackage,
                                                      const std::string& outputDirectory);

      private:
         //---------------------------------------------
         ///\brief   Report the progression of a download
         ///\param [in] file        The file which is currently downloading
         ///\param [in] progress    The file download progression
         ///\param [in] callback    The callback to use
         ///\param [in] function    The i18n string to send to callback (ex: update.plugin.download)
         ///\param [in] min         The global progression when download start
         ///\param [in] max         The global progression when download ends
         //---------------------------------------------
         static void reportDownloadProgress(const std::string& file,
                                            float progress,
                                            WorkerProgressFunc callback,
                                            const std::string& function,
                                            float min,
                                            float max);
      };
   } // namespace worker
} // namespace update
