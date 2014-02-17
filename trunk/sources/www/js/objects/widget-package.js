/**
 * Created by nicolasHILAIRE on 13/02/14.
 */

function WidgetPackage() {

   /**
    * This item is used to indicate if the view has been already downloaded
    * @type {boolean}
    */
   this.viewHasBeenDownloaded = false;

   /**
    * Use this object to contruct an instance of the viewModel
    */
   this.viewModelCtor = null;

   /**
    * Corresponding to package.json data
    */
   this.packageInformation;
}