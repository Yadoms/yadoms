/**
 * Created by nicolasHILAIRE on 25/02/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function PageManager() {}

/**
 * Array of pages
 * @type {Array}
 */
PageManager.pages = [];

/**
 * The packery options
 */
PageManager.packeryOptions = {
    itemSelector: ".widget",
    columnWidth: 100,
    //transitionDuration: '0.6s',
    gutter: 0
};

PageManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.id), "json.id must be defined");
    assert(!isNullOrUndefined(json.name), "json.name must be defined");
    assert(!isNullOrUndefined(json.pageOrder), "json.pageOrder must be defined");

    return new Page(json.id, json.name, json.pageOrder);
};

/**
 * Get all the defined pages
 * @returns {Promise} 
 */
PageManager.getAll = function () {
    var d = new $.Deferred();

    RestEngine.getJson("rest/page")
        .done(function (data) {
            //we add it to the page list
            PageManager.pages = [];

            if (!isNullOrUndefinedOrEmpty(data.page)) {
                //we sort page by pageOrder
                data.page.sort(function (a, b) {
                    return a.pageOrder > b.pageOrder;
                });

                $.each(data.page, function (index, value) {
                    PageManager.addPage(PageManager.factory(value));
                });
            }
            d.resolve();
        })
        .fail(d.reject);
    return d.promise();
};

/**
 * Get Page with its id in the page. return null if it's not exist
 * @param pageId
 * @returns {Page}
 */
PageManager.getPage = function (pageId) {
    assert(!isNullOrUndefined(pageId), "pageId must be defined");
    var res = $.grep(PageManager.pages, function (item) {
        return (item.id === pageId);
    });
    if (res.length !== 1)
        return null;
    return res[0];
};


/**
 * Create a new page
 * @param {} pageName The new page name
 * @param {} pageOrder The new page order
 * @returns {Promise} 
 */
PageManager.createPage = function (pageName, pageOrder) {
    assert(!isNullOrUndefined(pageName), "pageName must be defined");
    assert(!isNullOrUndefined(pageOrder), "pageOrder must be defined");

    var d = new $.Deferred();

    RestEngine.postJson("rest/page", {
            data: JSON.stringify({
                name: pageName,
                pageOrder: pageOrder
            })
        })
        .done(function (data) {
            //we add it to the page list
            //we add the page dynamically
            d.resolve(PageManager.factory(data));
        })
        .fail(d.reject);
    return d.promise();
};

/**
 * Update a page
 * @param {String|Integer} pageId The page id
 * @param {String} pageName The page new name
 * @param {String|Integer} pageOrder The page new order
 * @returns {Promise} 
 */
PageManager.updatePage = function (pageId, pageName, pageOrder) {
    assert(!isNullOrUndefined(pageId), "pageId must be defined");
    assert(!isNullOrUndefined(pageName), "pageId must be defined");
    return RestEngine.putJson("/rest/page/" + pageId, {
        data: JSON.stringify({
            id: pageId,
            name: pageName,
            pageOrder: pageOrder
        })
    });
};

/**
 * Delete a page
 * @param {Object} pageToDelete The page to dleete
 * @returns {Promise} 
 */
PageManager.deletePage = function (pageToDelete) {
    return RestEngine.deleteJson("/rest/page/" + pageToDelete.id);
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
    var dataI18NOptions = {
        "pageName": page.name
    };


    $("<li class=\"tabPagePills\" page-id=\"" + page.id + "\">" +
        "<a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" +
        "<span>" + page.name + "</span>" +
        "<div class=\"customizationToolbar pageCustomizationToolbar customization-item hidden\">" +
        "<div class=\"customizationButton pageCustomizationButton move-left-page\" title=\"Move to left\" data-i18n=\"[title]mainPage.customization.moveToLeft\"><i class=\"fa fa-lg fa-arrow-left\"></i></div>" +
        "<div class=\"customizationButton pageCustomizationButton move-right-page\" title=\"Move to right\" data-i18n=\"[title]mainPage.customization.moveToRight\"><i class=\"fa fa-lg fa-arrow-right\"></i></div>" +
        "<div class=\"customizationButton pageCustomizationButton rename-page\" title=\"Rename\" data-i18n=\"[title]mainPage.customization.rename\"><i class=\"fa fa-lg fa-pencil\"></i></div>" +
        "<div class=\"customizationButton pageCustomizationButton delete-page\" title=\"Delete\" data-i18n=\"[title]mainPage.customization.delete\"><i class=\"fa fa-lg fa-trash-o\"></i></div>" +
        "</div>" +
        "</a>" +
        "<div class=\"hidden tabPagePillsDropper\" data-i18n=\"mainPage.customization.dropHereToMovePage\" data-i18n-options=\'" + JSON.stringify(dataI18NOptions) + "\'></div>" +
        "</li>").insertBefore($("li#btn-add-page"));

    page.$tab = $(".page-tabs").find("li[page-id=\"" + page.id + "\"]");

    //i18n of page tab
    page.$tab.i18n();

    //page creation
    var container = $("div#tabContainer").find(".tab-content").append(
        "<div class=\"widgetPage tab-pane active\" id=\"" + tabIdAsText + "\" page-id=\"" + page.id + "\">" +
        "<div class=\"grid\">" +
        "</div>" +
        "</div>");

    //we save the content of the page dom node
    page.$content = container.find("div#" + tabIdAsText);

    page.$grid = page.$content.find(".grid");

    page.$grid.packery(PageManager.packeryOptions);

    page.$grid.on('dragstop', function (event) {
        console.log(event.target);
        //we remove the page overlay
        $(".tabPagePills .tabPagePillsDropper").addClass("hidden");

        var targetPill = event.toElement || event.originalEvent.target;

        //we look if the widget has been dropped onto another page pill
        if ((!targetPill) || (!event.target))
            return;
        var $page = $(targetPill).parent();
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
        if (page.id === targetPageId)
            return;

        //a widget has been dropped onto antoher page pill
        //we move the widget to the other pill
        var widgetId = $widget.attr("widget-id");
        var widgetToMove = page.getWidget(widgetId);
        widgetToMove.setInitialPosition(1000); //we indicate that the widget has never been placed and must be placed to the end
        widgetToMove.idPage = targetPageId;

        //we remove it from current page
        page.$grid.packery("remove", $widget);

        page.widgets.splice($.inArray(widgetToMove, page.widgets), 1);

        //we update the widget on the server
        WidgetManager.updateToServer(widgetToMove)
            .done(function () {
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
                            notifyError($.t("modals.add-widget.unableToCreateWidgetOfType", {
                                "widgetType": widgetToMove.type
                            }));
                        });
                }

            });
    });

    page.$grid.on('dragstart', function () {
        if (customization) {
            //we reveal the overlay on all other page to indicate that the user can move widget onto other pages
            $(".tabPagePills").not("[page-id=\"" + page.id + "\"]").find(".tabPagePillsDropper").removeClass("hidden");
        }
    });

    PageManager.enableCustomization(page, customization);

    //manage all fitText classes
    page.$tab.on('shown.bs.tab', function (e) {
        var pid = $(e.currentTarget).attr("page-id");
        var p = PageManager.getPage(pid);
        p.$grid.find('.textfit').fitText();
        p.$grid.packery('layout');
    });

    //we listen click event on tab click
    page.$tab.find("a").bind('click', function (e) {
        return tabClick($(e.currentTarget).parent().attr("page-id"));
    });

    //we listen click event on rename click
    page.$tab.find('div.rename-page').bind('click', function (e) {
        createOrUpdatePage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"));
    });

    //we listen click event on delete click
    page.$tab.find('div.delete-page').bind('click', function (e) {
        var pageId = $(e.currentTarget).parents("li.tabPagePills").attr("page-id");

        if ($("ul.page-tabs").find("li.tabPagePills").length === 1) {
            notifyWarning($.t("modals.delete-page.deletingLastPageNotAllowed"));
        } else {
            Yadoms.modals.pageDelete.loadAsync()
                .done(function () {
                    Yadoms.showDeletePageModal(pageId);
                });
        }
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
    page.$tab.bind('click', function () {});
};

PageManager.movePage = function (page, direction) {
    assert(!isNullOrUndefined(page), "page must be defined");
    assert(!isNullOrUndefined(direction), "direction must be defined");

    var nearestId = null;
    var nearestPageOrder;

    if (direction === "right") {
        //we search the nearest upper pageOrder than our
        nearestPageOrder = Infinity;

        $.each(PageManager.pages, function (index, currentPage) {
            //if the current pageOrder is greater than our and smaller than the nearest it's the new nearest
            if ((currentPage.pageOrder > page.pageOrder) && (currentPage.pageOrder < nearestPageOrder)) {
                nearestPageOrder = currentPage.pageOrder;
                nearestId = currentPage.id;
            }
        });
    } else {
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
        if (nearestPage) {
            //we can move pageOrder
            //we move it into the array and send the complete collection to rest server
            nearestPage.pageOrder = page.pageOrder;
            page.pageOrder = nearestPageOrder;

            //we make an array of pages to send to rest server
            RestEngine.putJson("/rest/page", {
                    data: JSON.stringify(PageManager.pages)
                })
                .done(function () {
                    //we move the tab dynamically
                    var tabDomElement = page.$tab.detach();
                    if (direction === "right") {
                        tabDomElement.insertAfter(nearestPage.$tab);
                    } else {
                        tabDomElement.insertBefore(nearestPage.$tab);
                    }
                })
                .fail(function (error) {
                    notifyError($.t("mainPage.errors.errorDuringSavingPagePosition"), error);
                });
        }
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

PageManager.selectFirstPage = function () {
    if (PageManager.pages.length > 0)
        PageManager.pages[0].$tab.find("a").trigger("click");
};

PageManager.selectPageId = function (pageId) {
    $.each(PageManager.pages, function (index, currentPage) {
        if (currentPage.id == pageId)
            currentPage.$tab.find("a").trigger("click");
    });
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

/**
 * Enable or disable customization on widget
 * @param enable
 */
PageManager.enableCustomization = function (page, enable) {
    assert(!isNullOrUndefined(page), "page must be defined");

    if (enable) {
        $(".customization-item").removeClass("hidden");
        page.$tab.find("a").first().addClass("pageCustomizationPill");
        $("ul.page-tabs").removeClass("nav-justified");
    } else {
        $(".customization-item").addClass("hidden");
        page.$tab.find("a").first().removeClass("pageCustomizationPill");
        $("ul.page-tabs").addClass("nav-justified");
    }
};

/**
 * Save a page customization to database
 * @param {Object} page The page to save
 * @returns {Promise} 
 */
PageManager.saveCustomization = function (page) {
    return RestEngine.putJson("/rest/page/" + page.id + "/widget", {
        data: JSON.stringify(page.widgets)
    });
};

/**
 * Update the layout of all widgets on the page
 * @param {Object} page The page to update
 */
PageManager.updateWidgetLayout = function (page) {
    $.each(page.widgets, function (index, value) {
        WidgetManager.updateWidgetLayout(value);
    });
};

/**
 * Execute onWakeUp instruction for all widgets
 * @param {Object} page The page to wakeup
 */
PageManager.onWakeUp = function (page) {
    $.each(page.widgets, function (index, value) {
        WidgetManager.onWakeUp(value);
    });
};