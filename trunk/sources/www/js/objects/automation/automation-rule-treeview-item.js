/**
 * Created by nhilaire on 20/01/2015.
 */

function AutomationRuleTreeviewItem(configuration) {
   var self = this;
   this.selected = null;
   this.operatorChanged = null;
   this.uuid = createUUID();
   this.childs = [];

   //from the configuration we create all needed objects
   //we create the right operator
   this.operator = AutomationRuleConditionFactory.createItem(configuration, this);

   //for operator that have children the configuration contains a "children" array
   var children = AutomationRuleConditionFactory.getContentOfConfiguration(configuration).children;
   if ($.isArray(children)) {
      $.each(children, function (index, value) {
         //we've got a sub node so we create a sub TreeviewItem
         var treeview = new AutomationRuleTreeviewItem(children[index]);

         //we forward the event of child nodes
         treeview.operatorChanged = function(treeviewItem, newOperator) {self.operatorChanged(treeviewItem, newOperator)};
         treeview.selected = function(treeviewItem) {self.selected(treeviewItem)};

         self.childs.push(treeview);
      });
   }
}

AutomationRuleTreeviewItem.prototype.addToDom = function($container) {
   var self = this;

   var item =  "<li id=\"" + self.uuid + "\" class=\"automation-rule-sub-item\">" +
                  "<span class=\"automation-rule-item\">" +
                     "<div class=\"btn-group\">" +
                        "<button class=\"btn btn-default btn-xs dropdown-toggle operator-selection\" data-toggle=\"dropdown\">" + self.operator.getName() + "&nbsp;<span class=\"caret\"></span></button>" +
                        "<ul class=\"operator-list dropdown-menu\" role=\"menu\">" +
                        "</ul>" +
                     "</div>" +
                     "<div class=\"content\">" +
                     "</div>" +
                  "</span>" +
                  "<ul class=\"automation-rule-sub-item\"></ul>" +
               "</li>";

   $container.append(item);

   //we add operators to the dropdown menu
   var $op = self.locateInDOM();

   var $menu = $op.find(".operator-list:first");

   $.each(AutomationRuleHelper.getAvailableOperators(), function (key, value) {
      $menu.append("<li><a href=\"#\">" + value + "</a></li>")
   });

   var $childContainer = $op.find("ul.automation-rule-sub-item:first");

   //we add child if exists
   $.each(self.childs, function (key, value) {
      value.addToDom($childContainer);
   });

   //we manage events

   $op.find(".content:first").bind("click", function (evt) {
      if ($.isFunction(self.selected))
         self.selected(self);
      event.stopPropagation();
   });

   /*$op.find("> .automation-rule-item > button.operator-selection").bind("change", function (evt) {
      if ($.isFunction(self.operatorChanged)) {
         //we look for the new operator asked
         self.operatorChanged(self, this.val());
      }

      event.stopPropagation();
   });*/

   //we listen for events
   $op.bind("change", function () {
      //we update content of the operator
      var $content = $op.find(".content:first");
      $content.html(self.operator.toString());

      if (self.operator.isValid()) {
         $op.find(".automation-rule-item:first").removeClass("invalid");
      }
      else {
         $op.find(".automation-rule-item:first").addClass("invalid");
      }
   });

   //we force event to be fired
   $op.change();
};

AutomationRuleTreeviewItem.prototype.locateInDOM = function() {
   return $("li#" + this.uuid);
};