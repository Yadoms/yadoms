/**
 * Created by nicolasHILAIRE on 13/02/14.
 */

function WidgetPackage() {

   /**
    * This item is used to indicate if the view and the viewModel have been already downloaded
    * @type {boolean}
    */
   this.viewAnViewModelHaveBeenDownloaded = false;

   /**
    * Use this object to construct an instance of the viewModel
    */
   this.viewModelCtor = null;

   /**
    * Corresponding to package.json data
    */
   this.packageInformation = null;
}