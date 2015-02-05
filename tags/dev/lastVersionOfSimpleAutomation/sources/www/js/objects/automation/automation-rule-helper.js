
/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function AutomationRuleHelper(){}

AutomationRuleHelper.getAvailableOperators = function () {
   return ["IS", "BECOME", "NOT", "OR", "AND"];
}
/*
AutomationRuleHelper.createTreeViewItem = function(operator, $container, operatorChanged, operatorSelected) {

   var item = "<span class=\"automation-rule-item label label-default\" conditionItemId=\"" + operator.uuid + "\">" +
            //"<span class=\"title badge\"><i>" + operator.getName() + "</i></span>" +
               "<div class=\"btn-group\">" +
                  "<button class=\"btn btn-default btn-xs dropdown-toggle operator-selection\" data-toggle=\"dropdown\">" + operator.getName() + "<span class=\"caret\"></span></button>" +
                  "<ul class=\"dropdown-menu\" role=\"menu\">" +
                  "</ul>" +
               "</div>" +
               "<span class=\"content\"></span>" +
            "</span>";

   $container.append(item);

   //we add operators to the dropdown menu
   var $op = operator.locateInDOM();

   var $menu = $op.find("> .btn-group > .dropdown-menu");

   $.each(AutomationRuleHelper.getAvailableOperators(), function (key, value) {
      $menu.append("<li><a href=\"#\">" + value + "</a></li>")
   });

   $op.find("> .btn-group > button.operator-selection").bind("click", function (evt) {
      operatorChanged(operator);
      event.stopPropagation();
   });

   $op.bind("click", function (evt) {
      operatorSelected(operator);
      event.stopPropagation();
   };

   //we listen for events
   $op.bind("change", function () {
      //we update content of the operator
      var $content = $op.find("> .content");
      $content.html(operator.toString());

      if (operator.isValid()) {
         $content.removeClass("label-danger").addClass("label-success");
      }
      else {
         $content.addClass("label-danger").removeClass("label-success");
      }
   });

   //we force event to be fired
   $op.change();
};
*/
AutomationRuleHelper.setRuleAsActivated = function (treeview) {
   //we unselect all
   $("ul.automation-rule-first-item .automation-rule-item").removeClass("bg-info");
   //we select the current
   treeview.locateInDOM().find("> .automation-rule-item").addClass("bg-info");
};