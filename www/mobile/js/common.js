//基于jquery.js txgenerictools.js


var user_id=0, userroleid=1, username='', clienttype = '', adminid=0;

var temp_uid=tx.setCookie("iks_uid"),
	temp_username=tx.setCookie("iks_username"),
	temp_clienttype=tx.setCookie("iks_clienttype"),
	temp_userroleid=tx.setCookie("iks_userroleid");

	if(temp_uid!=null)
		user_id = temp_uid;
	if(temp_username!=null)
		username = temp_username;
	if(temp_clienttype!=null)
		clienttype = temp_clienttype;
	if(temp_userroleid!=null){
		userroleid = temp_userroleid;
		if(temp_userroleid == 21 || temp_userroleid == 22)
			adminid = 1;
	}

//var clienttype = "iPad";

function is_iPad(){
    var ua = navigator.userAgent.toLowerCase();
    if(ua.match(/iPad/i)=="ipad") {
       return true;
    } else {
       return false;
    }
}

function is_iPhone(){
    var ua = navigator.userAgent.toLowerCase();
    if(ua.match(/iPhone/i)=="iphone") {
       return true;
    } else {
       return false;
    }
}

function is_android(){
    var ua = navigator.userAgent.toLowerCase();
    if(ua.match(/android/i)=="android") {
       return true;
    } else {
       return false;
    }
}

$(function(){

	if(is_iPhone()){
			$('head').append('<meta name="viewport" content="user-scalable=yes, width=device-width, initial-scale=0.5, maximum-scale=1"/>');
	}
	else if(is_iPad()){
		$('head').append('<meta name="viewport" content="user-scalable=yes, width=device-width, initial-scale=1, maximum-scale=2"/>');
	}
	else if(is_android()){

		if(window.orientation == 0 || window.orientation == 180)
			$('head').append('<meta name="viewport" content="user-scalable=yes, width=device-width, initial-scale=0.5, maximum-scale=1"/>');
		else
			$('head').append('<meta name="viewport" content="user-scalable=yes, width=device-width, initial-scale=0.7, maximum-scale=2"/>');

	}

	/*  控制是否显示导航条  */
	if(clienttype == null || clienttype == undefined || clienttype.length<=0){
		$("#header").css({"display":"block"});
		$("#container").css({"margin-top":"102px"});
	}

	/* 需要登录才能访问的链接 */
	$('*[data-needlogin=1]').attr("data-gotourl",function(index, urlvalue){
		if(urlvalue.length>0){
			$(this).click(function(){
				if(user_id == 0 || user_id == null || user_id == undefined)
					confirmlogin(urlvalue);
				else
					location.href = urlvalue;

			});
		}
	});
	$('*[data-needlogin=0]').attr("data-gotourl",function(index, urlvalue){
		if(urlvalue.length>0){
			$(this).click(function(){
				location.href = urlvalue;
			});
		}
	});

	if(user_id>0){
		$('.username').replaceWith('<div class="username">'+username+'</div>');
		$('.usermsg').append('<a href="javascript:logout()" class="loginout">退出</a>');
	}

	backToTop();
 });

function dist_url(url){
	//如 历史、错题集、其他未登录用户不可以做的试题
	//得先判断是否已经登录，已经登录的直接跳转，未登录的才涉及到 与框架的交互 或者 是网页的登录页

	if(user_id == 0 || user_id == null || user_id == undefined){
		//未登录，涉及到与客户端框架交互时 或 跳转到登录页。
		switch(clienttype.toLowerCase()){
			case 'iphone':
			case 'ipad':
			case 'android':
				url = 'news://login?destination='+url;

				break;
			default:
				url = 'login.html';
				//url = 'login.html?destination='+url;
				break;
		}

	}


	location.href = url;

}


function confirmlogin(url){
	var msg =[ "登录用户才能访问！","登录","取消"];

	confirmPop("", msg, function(){
		dist_url(url);

	} );

}


function logout(){
	 tx.setCookie("iks_uid",null);
	 tx.setCookie("iks_username",null);
	 tx.setCookie("iks_userroleid",null);
	 location.href = "index.html";
}

function backToTop() {
	var $backToTopEle = $(".backToTop");
	if($backToTopEle.length==0){
		var $backToTopTxt = "返回顶部";
		$backToTopEle = $('<div class="backToTop"></div>').appendTo($("body"))
			.text($backToTopTxt).attr("title", $backToTopTxt).click(function() {
				$("html, body").animate({ scrollTop: 0 }, 120);
	    });
	}

	var $backToTopFun = function() {
	        var st = $(document).scrollTop(), winh = $(window).height();
	        (st > 500)? $backToTopEle.show(): $backToTopEle.hide();
	        //IE6下的定位
	        if (!window.XMLHttpRequest) {
	            $backToTopEle.css("top", st + winh - 166);
	        }
	    };
	$(window).bind("scroll", $backToTopFun);
    //$(function() { $backToTopFun(); });

};