//服务与 viewpaper.js view_userresult.js view_wrong_answer.js
function needfun(){

	$(".disnext").css("cursor","pointer").click(function(){
		$(this).parent().parent().hide().next().show('slow');
	});
	$(".disprev").css("cursor","pointer").click(function(){
		$(this).parent().parent().hide().prev().show('slow');
	});

	$(".jplayer").myjplayer();


	$(".serial_name_num_name").toggle(
		function(){$(this).nextAll('.allSubques').slideUp('slow');},
		function(){$(this).nextAll('.allSubques').slideDown('slow');}
	);


	$(".description_label").click(function(){
		if($(this).hasClass("right")){
			$(this).next(".ques_body").show();
			$(this).removeClass("right");
		}
		else{
			$(this).next(".ques_body").hide();
			$(this).addClass("right");
		}
	});
}

function userwrong_edit(){
	$(".edit").click(function(){
		if($(this).html()=="完成"){
			$(".delete_btn").fadeOut(600);
			$(".paper").animate({"margin-left":"30px"},500);
			$(this).html("编辑");

		}
		else{
			$(".delete_btn").fadeIn(100);
			$(".paper").animate({"margin-left":"140px"},500);
			$(this).html("完成");
		}

	});
}