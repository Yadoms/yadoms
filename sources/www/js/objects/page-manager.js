/**
 * Created by nicolasHILAIRE on 25/02/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function PageManager() { }

/**
 * Array of pages
 * @type {Array}
 */
PageManager.pages = [];

PageManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.id), "json.id must be defined");
    assert(!isNullOrUndefined(json.name), "json.name must be defined");
    assert(!isNullOrUndefined(json.pageOrder), "json.pageOrder must be defined");

    return new Page(json.id, json.name, json.pageOrder);
};

PageManager.getAll = function (callback) {
    $.getJSON("rest/page")
       .done(function (data) {
           //we parse the json answer
           if (data.result != "true") {
               notifyError($.t("objects.PageManager.errorDuringGettingPages"), JSON.stringify(data));
               return;
           }
           //we add it to the page list
           PageManager.pages = [];

           if (!isNullOrUndefinedOrEmpty(data.data.page)) {
               //we sort page by pageOrder
               data.data.page.sort(function (a, b) { return a.pageOrder > b.pageOrder; });

               $.each(data.data.page, function (index, value) {
                   PageManager.addPage(PageManager.factory(value));
               });
           }

           if ($.isFunction(callback))
               callback();
       })
       .fail(function () { notifyError($.t("objects.PageManager.errorDuringGettingPages")); });
};

/**
 * Get Page with its id in the page. return null if it's not exist
 * @param pageId
 * @returns {Page}
 */
PageManager.getPage = function (pageId) {
    assert(!isNullOrUndefined(pageId), "pageId must be defined");
    var res = $.grep(PageManager.pages, function (item) {
        return (item.id == pageId);
    });
    if (res.length != 1)
        return null;
    return res[0];
};


PageManager.createPage = function (pageName, pageOrder, callback) {
    assert(!isNullOrUndefined(pageName), "pageName must be defined");
    assert(!isNullOrUndefined(pageOrder), "pageOrder must be defined");

    $.ajax({
        type: "POST",
        url: "/rest/page/",
        data: JSON.stringify({ name: pageName, pageOrder: pageOrder }),
        contentType: "application/json; charset=utf-8",
        dataType: "json"
    })
       .done(function (data) {
           //we parse the json answer
           if (data.result != "true") {
               notifyError($.t("objects.generic.errorCreating", { objectName: pageName }), JSON.stringify(data));
               return;
           }

           //we add the page dynamically
           var p = PageManager.factory(data.data);

           if ($.isFunction(callback))
               callback(p)
       })
       .fail(function () { notifyError($.t("objects.generic.errorCreating", { objectName: pageName })); });
};

PageManager.addPage = function (page) {
    assert(!isNullOrUndefined(page), "page must be defined");
    //we look if it isn't already in PageManager
    var res = PageManager.getPage(page.id);
    assert(res == null, "Page has already been added to the container");
    PageManager.pages.push(page);
};

PageManager.addToDom = function (page) {
    assert(!isNullOrUndefined(page), "page must be defined");
    var tabIdAsText = "tab-" + page.id;
    //pill creation
    var dataI18nOptions = { "pageName": page.name };


    $("<li class=\"tabPagePills\" page-id=\"" + page.id + "\">" +
          "<a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" +
             "<span>" + page.name + "</span>" +
             "<div class=\"customizationToolbar pageCustomizationToolbar customization-item hidden\">" +
                "<div class=\"customizationButton pageCustomizationButton add-widget\" data-i18n=\"[title]mainPage.customization.addNewWidget\"><i class=\"fa fa-lg fa-puzzle-piece\"></i></div>" +
                "<div class=\"customizationButton pageCustomizationButton move-left-page\" title=\"Move to left\" data-i18n=\"[title]mainPage.customization.moveToLeft\"><i class=\"fa fa-lg fa-arrow-left\"></i></div>" +
                "<div class=\"customizationButton pageCustomizationButton move-right-page\" title=\"Move to right\" data-i18n=\"[title]mainPage.customization.moveToRight\"><i class=\"fa fa-lg fa-arrow-right\"></i></div>" +
                "<div class=\"customizationButton pageCustomizationButton rename-page\" title=\"Rename\" data-i18n=\"[title]mainPage.customization.rename\"><i class=\"fa fa-lg fa-pencil\"></i></div>" +
                "<div class=\"customizationButton pageCustomizationButton delete-page\" title=\"Delete\" data-i18n=\"[title]mainPage.customization.delete\"><i class=\"fa fa-lg fa-trash-o\"></i></div>" +
             "</div>" +
          "</a>" +
          "<div class=\"hidden tabPagePillsDropper\" data-i18n=\"mainPage.customization.dropHereToMovePage\" data-i18n-options=\'" + JSON.stringify(dataI18nOptions) + "\'></div>" +
          "</li>").insertBefore($("li#btn-add-page"));

    page.$tab = $(".page-tabs").find("li[page-id=\"" + page.id + "\"]");

    //i18n of page tab
    page.$tab.i18n();

    //page creation
    var container = $("div#tabContainer").find(".tab-content").append(
       "<div class=\"widgetPage tab-pane active\" id=\"" + tabIdAsText + "\" page-id=\"" + page.id + "\">" +
          "<div class=\"grid grid-stack\">" +
          "</div>" +
          "</div>");

    //we save the content of the page dom node
    page.$content = container.find("div#" + tabIdAsText);

    var options = {
        width: numberOfColumns,
        float: true,
        always_show_resize_handle: true,
        cell_height: gridWidth,
        vertical_margin: gridMargin,
        min_width: 0
    };

    page.$grid = page.$content.find(".grid-stack");
    page.grid = page.$grid.gridstack(options).data('gridstack');

    page.$grid.on('resizestop', widgetResized);

    page.$grid.on('dragstop', function (event, ui) {
        //we remove the page overlay
        $(".tabPagePills .tabPagePillsDropper").addClass("hidden");

        //we look if the widget has been dropped onto another page pill
        if ((!event.toElement) || (!event.target))
            return;
        var $page = $(event.toElement).parent();
        if (!$page)
            return;
        var $widget = $(event.target);
        if (!$widget)
            return;
        var targetPageId = $page.attr("page-id");
        if (!targetPageId)
            return;
        var targetPage = PageManager.getPage(targetPageId);
        //the widget that move is on the current page
        if (page.id == targetPageId)
            return;

        //a widget has been dropped onto antoher page pill
        //we move the widget to the other pill
        var widgetId = $widget.attr("widget-id");
        var widgetToMove = page.getWidget(widgetId);
        widgetToMove.positionX = -1;
        widgetToMove.positionY = -1;
        widgetToMove.idPage = targetPageId;

        //we remove it from current page
        page.grid.remove_widget(widgetToMove.$gridWidget, true);
        page.widgets.splice($.inArray(widgetToMove, page.widgets), 1);

        //we update the widget on the server
        WidgetManager.updateToServer(widgetToMove, function() {
            //the widget has been moved successfully
            //we add it to the new page
            if (targetPage.loaded) {
                //if the page has been already loaded we add it to the page
                WidgetManager.loadWidget(widgetToMove, targetPage)
                    .done(function () {
                        //we update the filter for the websocket
                        updateWebSocketFilter();
                    })
                    .fail(function (errorMessage) {
                        console.error(errorMessage);
                        notifyError($.t("modals.add-widget.unableToCreateWidgetOfType", { "widgetType": widgetMoved.type }));
                    });
            }

        });
    });

    page.$grid.on('dragstart', function (event, ui) {
        //we reveal the overlay on all other page to indicate that the user can move widget onto other pages
        $(".tabPagePills").not("[page-id=\"" + page.id + "\"]").find(".tabPagePillsDropper").removeClass("hidden");
    });

    PageManager.enableCustomization(page, customization);

    //we initialize page events

    //we listen click event on tab click
    page.$tab.find("a").bind('click', function (e) {
        return tabClick($(e.currentTarget).parent().attr("page-id"));
    });

    //we listen click event on add new widget
    page.$tab.find('div.add-widget').bind('click', function () {
        modals.widgetAdd.load(function () { askWidgetPackages(); });
    });

    //we listen click event on rename click
    page.$tab.find('div.rename-page').bind('click', function (e) {
        createOrUpdatePage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"));
    });

    //we listen click event on delete click
    page.$tab.find('div.delete-page').bind('click', function (e) {
        var pageId = $(e.currentTarget).parents("li.tabPagePills").attr("page-id");
        modals.pageDelete.load(function (pageId) { return function () { showDeletePageModal(pageId) } }(pageId));
    });

    //we listen click event on move left click
    page.$tab.find('div.move-left-page').bind('click', function (e) {
        var pageToMove = PageManager.getPage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"));
        assert(!isNullOrUndefined(pageToMove), "page doesn't exist in PageManager");
        PageManager.movePage(pageToMove, "left");
    });

    //we listen click event on move right click
    page.$tab.find('div.move-right-page').bind('click', function (e) {
        var pageToMove = PageManager.getPage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"));
        assert(!isNullOrUndefined(pageToMove), "page doesn't exist in PageManager");
        PageManager.movePage(pageToMove, "right");
    });

    //we listen for click event with no code inside to help event transmission until exit customization
    page.$tab.bind('click', function (e) {
    });
};

PageManager.movePage = function (page, direction) {
    assert(!isNullOrUndefined(page), "page must be defined");
    assert(!isNullOrUndefined(direction), "direction must be defined");

    var nearestId = null;
    var nearestPageOrder;

    if (direction == "right") {
        //we search the nearest upper pageOrder than our
        nearestPageOrder = Infinity;

        $.each(PageManager.pages, function (index, currentPage) {
            //if the current pageOrder is greater than our and smaller than the nearest it's the new nearest
            if ((currentPage.pageOrder > page.pageOrder) && (currentPage.pageOrder < nearestPageOrder)) {
                nearestPageOrder = currentPage.pageOrder;
                nearestId = currentPage.id;
            }
        });
    }
    else {
        //we search the nearest lower pageOrder than our
        nearestPageOrder = -Infinity;

        $.each(PageManager.pages, function (index, currentPage) {
            //if the current pageOrder is greater than our and smaller than the nearest it's the new nearest
            if ((currentPage.pageOrder < page.pageOrder) && (currentPage.pageOrder > nearestPageOrder)) {
                nearestPageOrder = currentPage.pageOrder;
                nearestId = currentPage.id;
            }
        });
    }

    if (nearestId != null) {

        var nearestPage = PageManager.getPage(nearestId);
        assert(nearestPage != null, "Unable to find nearest page to move");

        //we can move pageOrder
        //we move it into the array and send the complete collection to rest server
        nearestPage.pageOrder = page.pageOrder;
        page.pageOrder = nearestPageOrder;

        //we make an array of pages to send to rest server
        $.ajax({
            type: "PUT",
            url: "/rest/page",
            data: JSON.stringify(PageManager.pages),
            contentType: "application/json; charset=utf-8",
            dataType: "json"
        })
        .done(function (data) {
            //we parse the json answer
            if (data.result != "true") {
                notifyError($.t("mainPage.errors.errorDuringSavingPagePosition"), JSON.stringify(data));
                console.error(data.message);
                return;
            }

            //we move the tab dynamically
            var tabDOMElement = page.$tab.detach();
            if (direction == "right") {
                tabDOMElement.insertAfter(nearestPage.$tab);
            }
            else {
                tabDOMElement.insertBefore(nearestPage.$tab);
            }
        })
        .fail(function () { notifyError($.t("mainPage.errors.errorDuringSavingPagePosition")); });
    }
};

PageManager.ensureOnePageIsSelected = function () {
    //if there is no page selected we select the first one
    if (PageManager.getCurrentPage() == null) {
        //we select the first page
        if (PageManager.pages.length > 0)
            PageManager.pages[0].$tab.find("a").trigger("click");
    }
};

/**
 * Return the current displayed page
 * @returns {Page}
 */
PageManager.getCurrentPage = function () {
    var pageId = $("ul.page-tabs li.active").attr("page-id");
    if (isNullOrUndefined(pageId))
        return null;
    return PageManager.getPage(pageId);
};

function widgetResized(event, ui) {
    var grid = this;
    var $widget = $(event.target);
    var widgetObject = WidgetManager.getFromGridElement($widget);
    //we compute the target size
    var newWidth = Math.round(widgetObject.width() / 100.0) * 100;
    var newHeight = Math.round(widgetObject.height() / 100.0) * 100;
    //we change the size of grid element to allow widget to use the end size not the dropped element size (388px -> 400px)
    widgetObject.setWidth(newWidth);
    widgetObject.setHeight(newHeight);
    try {
        if (widgetObject.viewModel.resized !== undefined)
            widgetObject.viewModel.resized();
    }
    catch (e) {
        notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", { widgetName: widgetObject.type, methodName: 'resized' }));
        console.warn(e);
    }
}

/**
 * Enable or disable customization on widget
 * @param enable
 */
PageManager.enableCustomization = function (page, enable) {
    assert(!isNullOrUndefined(page), "page must be defined");

    if (enable) {
        $(".customization-item").removeClass("hidden");
        page.$tab.find("a").first().addClass("pageCustomizationPill");
    } else {
        $(".customization-item").addClass("hidden");
        page.$tab.find("a").first().removeClass("pageCustomizationPill");
    }
};