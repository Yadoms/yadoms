/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

/**
 * Indicate if customization is in progress or not
 * @type {boolean}
 */
var customization = false;

/**
 * Indicate if we already have released the click button after entering in customization with long press
 * @type {boolean}
 */
var waitForRealeaseButtonAfterEnteringCustomization = false;

/**
 * Callback of the click on the customize button
 */
$("a#customizeButton").click(function () {
    customization = !customization;

    if (customization) {
        enterCustomization();
    }
    else
        exitCustomization();
});

function enterCustomization() {
    customization = true;

    $.each(PageManager.pages, function (index, currentPage) {
        PageManager.enableCustomization(currentPage, true);
        $.each(currentPage.widgets, function (index, currentWidget) {
            WidgetManager.enableCustomization(currentWidget, true);
        });
    });

    //we show / hide customization items
    if (customization)
        $(".customization-item").removeClass("hidden");
    else
        $(".customization-item").addClass("hidden");

    $("a#customizeButton i").addClass("fa-spin ");
}

/**
 * End customization and send all configuration to server
 */
function exitCustomization(saveCustomization) {
    if (isNullOrUndefined(saveCustomization))
        saveCustomization = true;

    waitForRealeaseButtonAfterEnteringCustomization = false;
    $("a#customizeButton i").removeClass("fa-spin");
    
    //we save all widgets in each page
    $.each(PageManager.pages, function (index, currentPage) {
        PageManager.enableCustomization(currentPage, false);
        if (currentPage.loaded) {
           
            $.each(currentPage.widgets, function (index, currentWidget) {
                WidgetManager.enableCustomization(currentWidget, false);
            });

            if (saveCustomization) {
               PageManager.saveCustomization(currentPage)
               .fail(function (error) {
                  notifyError($.t("mainPage.errors.errorSavingCustomization"), error);
               });
            }
        }
    });
}

function createOrUpdatePage(pageId) {
   Yadoms.modals.pageConfigure.loadAsync()
   .done(function () {
      Yadoms.showPageModificationModal(pageId);
    });
}