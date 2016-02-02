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

    $("a#customizeButton i").addClass("fa-spin ");
}

/**
 * End customization and send all configuration to server
 */
function exitCustomization(saveCustomization) {
    if (isNullOrUndefined(saveCustomization))
        saveCustomization = true;

    customization = false;
    waitForRealeaseButtonAfterEnteringCustomization = false;
    $("a#customizeButton i").removeClass("fa-spin");

    //we save all widgets in each page
    $.each(PageManager.pages, function (index, currentPage) {
        PageManager.enableCustomization(currentPage, false);
        if (currentPage.loaded) {
            $.each(currentPage.widgets, function (index, currentWidget) {
                WidgetManager.enableCustomization(currentWidget, false);
                currentWidget.updateDataFromGrid();
            });

            if (saveCustomization) {
                $.ajax({
                    type: "PUT",
                    url: "/rest/page/" + currentPage.id + "/widget",
                    data: JSON.stringify(currentPage.widgets),
                    contentType: "application/json; charset=utf-8",
                    dataType: "json"
                })
                    .done(function (data) {
                        //we parse the json answer
                        if (data.result != "true") {
                            notifyError($.t("mainPage.errors.errorSavingCustomization"), JSON.stringify(data));
                            console.error(data.message);
                        }
                    })
                    .fail(function () {
                        notifyError($.t("mainPage.errors.errorSavingCustomization"))
                    });
            }
        }
    });
}

function createOrUpdatePage(pageId) {
    modals.pageConfigure.load(function (pageId) { return function () { showPageModificationModal(pageId) } }(pageId));
}