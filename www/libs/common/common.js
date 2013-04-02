//基于jquery.js txgenerictools.js
var clienttype = "iPad";

var user_id=0, userroleid=1, username='', clienttype = '', adminid=0;

var temp_uid=tx.setCookie("uid"),
	temp_username=tx.setCookie("username"),
	temp_clienttype=tx.setCookie("clienttype"),
	temp_userroleid=tx.setCookie("userroleid");

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

if(clienttype == null || clienttype == undefined || clienttype.length<=0){
	$(".hidenav").css({"display":"block"});
}


$(function(){
	$('*[data-needlogin=1]').attr("data-gotourl",function(index, urlvalue){
		if(urlvalue.length>0){
			$(this).click(function(){
				dist_url(urlvalue);

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
 });

function dist_url(url){
	//如 历史、错题集、其他未登录用户不可以做的试题
	//得先判断是否已经登录，已经登录的直接跳转，未登录的才涉及到 与框架的交互 或者 是网页的登录页

	if(user_id == 0 || user_id == null || user_id == undefined){
		//未登录，涉及到与客户端框架交互时 或 跳转到登录页。

		if(url.indexOf("lishi")){
			switch(clienttype.toLowerCase()){
			case 'iphone':
			case 'ipad':
			case 'android':
				url = 'tx://lishi';
				break;
			default:
//				url = '/itest/www/mobile/login.html?destination='+url;
				break;
			}
		}

		else if(url.indexOf("cuoti")){
			switch(clienttype.toLowerCase()){
			case 'iPhone':
			case 'iPad':
			case 'android':
				url = 'tx://cuoti';
				break;
			default:
//				url = '/itest/www/mobile/login.html?destination='+url;
				break;
			}
		}

		url = '/itest/www/mobile/login.html?destination='+url;
	}


	//alert(url);
	location.href = url;

}

