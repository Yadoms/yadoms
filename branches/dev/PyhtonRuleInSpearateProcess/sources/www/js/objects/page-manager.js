/**
 * Created by nicolasHILAIRE on 25/02/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function PageManager(){}

/**
 * Array of pages
 * @type {Array}
 */
PageManager.pages = [];

PageManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.pageOrder), "json.pageOrder must be defined");

   return new Page(json.id, decodeURIComponent(json.name), json.pageOrder);
};

PageManager.getAll = function(callback) {
   $.getJSON("rest/page")
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.PageManager.errorDuringGettingPages"), JSON.stringify(data));
            return;
         }
         //we add it to the page list
         PageManager.pages = [];

         if (!isNullOrUndefinedOrEmpty(data.data.page)) {
            //we sort page by pageOrder
            data.data.page.sort(function(a, b) { return a.pageOrder > b.pageOrder; });

            $.each(data.data.page, function(index, value) {
               PageManager.addPage(PageManager.factory(value));
            });
         }

         if ($.isFunction(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.PageManager.errorDuringGettingPages"));});
};

/**
 * Get Page with its id in the page. return null if it's not exist
 * @param pageId
 * @returns {Page}
 */
PageManager.getPage = function(pageId) {
   assert(!isNullOrUndefined(pageId), "pageId must be defined");
   var res = $.grep(PageManager.pages, function(item) {
      return (item.id == pageId);
   });
   if (res.length != 1)
      return null;
   return res[0];
};


PageManager.createPage = function(pageName, pageOrder, callback) {
   assert(!isNullOrUndefined(pageName), "pageName must be defined");
   assert(!isNullOrUndefined(pageOrder), "pageOrder must be defined");

   $.ajax({
      type: "POST",
      url: "/rest/page/",
      data: JSON.stringify({ name: encodeURIComponent(pageName), pageOrder: pageOrder }),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorCreating", {objectName : pageName}), JSON.stringify(data));
            return;
         }

         //we add the page dynamically
         var p = PageManager.factory(data.data);

         if ($.isFunction(callback))
            callback(p)
      })
      .fail(function() {notifyError($.t("objects.generic.errorCreating", {objectName : pageName}));});
};

PageManager.addPage = function(page) {
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

   /*var container  = $("div#pageMenu").find("ul").append(
      "<li class=\"tabPagePills\" page-id=\"" + page.id + "\">" +
         "<a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" +
         "<span>" + page.name + "</span>" +
         "<div class=\"pageCustomizationToolbar btn-group btn-group-sm customization-item pull-right hidden\">" +
         "<button type=\"button\" class=\"btn btn-default move-left-page\" title=\"Move to left\" data-i18n=\"[title]mainPage.customization.moveToLeft\"><i class=\"glyphicon glyphicon-arrow-left\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default move-right-page\" title=\"Move to right\" data-i18n=\"[title]mainPage.customization.moveToRight\"><i class=\"glyphicon glyphicon-arrow-right\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default rename-page\" title=\"Rename\" data-i18n=\"[title]mainPage.customization.rename\"><i class=\"glyphicon glyphicon-pencil\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default delete-page\" title=\"Delete\" data-i18n=\"[title]mainPage.customization.delete\"><i class=\"glyphicon glyphicon-trash\"></i></button>" +
         "</div>" +
         "</a>" +
         "</li>");*/

   var container  =  $("<li class=\"tabPagePills\" page-id=\"" + page.id + "\">" +
         "<a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" +
         "<span>" + page.name + "</span>" +
         "<div class=\"pageCustomizationToolbar btn-group btn-group-sm customization-item pull-right hidden\">" +
         "<button type=\"button\" class=\"btn btn-default add-widget\" data-i18n=\"[title]mainPage.customization.addNewWidget\"><i class=\"fa fa-puzzle-piece\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default move-left-page\" title=\"Move to left\" data-i18n=\"[title]mainPage.customization.moveToLeft\"><i class=\"glyphicon glyphicon-arrow-left\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default move-right-page\" title=\"Move to right\" data-i18n=\"[title]mainPage.customization.moveToRight\"><i class=\"glyphicon glyphicon-arrow-right\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default rename-page\" title=\"Rename\" data-i18n=\"[title]mainPage.customization.rename\"><i class=\"glyphicon glyphicon-pencil\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default delete-page\" title=\"Delete\" data-i18n=\"[title]mainPage.customization.delete\"><i class=\"glyphicon glyphicon-trash\"></i></button>" +
         "</div>" +
         //"<div class=\"pageCustomizationToolbar customization-item pull-right hidden\">" +
         //   "<a href=\"#\" class=\"active\" id=\"\" data-i18n=\"[title]mainPage.menu.customization\"><i class=\"fa fa-wrench\"></i></a>" +
         //"</div>" +
         "</a>" +
         "</li>").insertBefore($("li#btn-add-page"));

   page.$tab = $(".page-tabs").find("li[page-id=\"" + page.id + "\"]");

   //i18n of page tab
   page.$tab.i18n();

   //page creation
   container = $("div#tabContainer").find(".tab-content").append(
      "<div class=\"widgetPage tab-pane active\" id=\"" + tabIdAsText + "\" page-id=\"" + page.id + "\">" +
         "<div class=\"gridster\">" +
         "<ul></ul>" +
         "</div>" +
         "</div>");

   //we save the content of the page dom node
   page.$content = container.find("div#" + tabIdAsText);

   //gridster creation
   page.gridster = page.$content.find("ul").gridster({
      widget_margins: [gridMargin, gridMargin],
      widget_base_dimensions: [gridWidth, gridWidth],
      min_cols: numberOfColumns,
      max_cols: numberOfColumns,
      resize: {
         enabled: true,
         resize: function(e, ui, $widget) {
            var widgetObject = WidgetManager.getFromGridsterElement($widget);
            try
            {
               if (widgetObject.viewModel.resized !== undefined)
                  widgetObject.viewModel.resized();
            }
            catch (e)
            {
               notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.type, methodName : 'resized'}));
               console.warn(e);
            }
         },
         stop: function(e, ui, $widget) {
            var widgetObject = WidgetManager.getFromGridsterElement($widget);
            try
            {
               if (widgetObject.viewModel.resized !== undefined)
                  widgetObject.viewModel.resized();
            }
            catch (e)
            {
               notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.type, methodName : 'resized'}));
               console.warn(e);
            }
         }
      }
   }).data('gridster');

   //we remove the active class of hidden gridster
   $("div#" + tabIdAsText).removeClass("active");

   $("div#" + tabIdAsText + " .gridster").width(numberOfColumns * (gridWidth + gridMargin * 2));

   //we check if we are in customization we must apply customization on the new page
   if (customization)
   {
      page.$tab.find(".customization-item").removeClass("hidden");
   }

   //we initialize page events

   //we listen click event on tab click
   page.$tab.find("a").bind('click', function (e) {
      return tabClick($(e.currentTarget).parent().attr("page-id")); } );

   //we listen click event on add new widget
   page.$tab.find('button.add-widget').bind('click', function () {
      modals.widgetAdd.load(function() {askWidgetPackages();});
   } );

   //we listen click event on rename click
   page.$tab.find('button.rename-page').bind('click', function (e) {
      createOrUpdatePage($(e.currentTarget).parents("li.tabPagePills").attr("page-id")); } );

   //we listen click event on rename click
   page.$tab.find('button.delete-page').bind('click', function (e) {
      var pageId = $(e.currentTarget).parents("li.tabPagePills").attr("page-id");
      modals.pageDelete.load(function (pageId) {return function() {showDeletePageModal(pageId)}}(pageId));
   } );

   //we listen click event on move left click
   page.$tab.find('button.move-left-page').bind('click', function (e) {
      var pageToMove = PageManager.getPage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"));
      assert(!isNullOrUndefined(pageToMove), "page doesn't exist in PageManager");
      PageManager.movePage(pageToMove, "left");
   } );

   //we listen click event on move right click
   page.$tab.find('button.move-right-page').bind('click', function (e) {
      var pageToMove = PageManager.getPage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"));
      assert(!isNullOrUndefined(pageToMove), "page doesn't exist in PageManager");
      PageManager.movePage(pageToMove, "right");
   } );

   //we listen for click event with no code inside to help event transmission until exit customization
   page.$tab.bind('click', function (e) {
   } );
};

PageManager.movePage = function(page, direction) {
   assert(!isNullOrUndefined(page), "pageToMove must be defined");
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
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
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
      .fail(function() {notifyError($.t("mainPage.errors.errorDuringSavingPagePosition"));});
   }
};

PageManager.ensureOnePageIsSelected = function() {
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
PageManager.getCurrentPage = function() {
   var pageId = $("ul.page-tabs li.active").attr("page-id");
   if (isNullOrUndefined(pageId))
      return null;
   return PageManager.getPage(pageId);
};