// JavaScript Document
(function($){
	$.fn.overtr=function(id,options){
		
		var settings=$.extend({
			//add_ques : false
		},options||{});
		
		var that=this;
		window.setTimeout(function(){
			$("this").hover(
				  function () {
					$(this).children().addClass("over");
				  },
				  function () {
					$(this).children().removeClass("over");
				  }
			);
		},3000);
		
		return false;
	}
})(jQuery);
