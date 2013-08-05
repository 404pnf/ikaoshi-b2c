/**
 * get方式获取json内容
 * url 接口url
 * message 出现错误时的提示消息
 * callback 请求成功后的回调函数
 * */
function getjson(url, message, callback){
	$.ajax({
		type: "GET",// 指定是post还是get,当然此处要提交,当然就要用post了
		cache: "false",// 默认: true,dataType为script时默认为false) jQuery 1.2 新功能，设置为 false 将不会从浏览器缓存中加载请求信息。
		url: url,// 发送请求的地址。
		//data: '&fuid='+pass+'&name='+name,//发送到服务器的数据
		dataType: "json",// 返回纯json
		timeout:200000,// 设置请求超时时间（毫秒）。
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。
			var errorMessage = message+" 请求失败! textStatus: " + textStatus + ', errorThrown: ' + errorThrown;
			$('body').prepend(errorMessage);
		},
		success: function(data)
		{  //请求成功后回调函数。
			callback(data);
		}
	});
}

/**
 * post方式提交内容，并获取json格式的返回值
 * url 接口url
 * postdata 发送到服务器的数据，格式：{name:name,password:pass}
 * message 出现错误时的提示消息
 * callback 请求成功后的回调函数
 * */
function postjson(url, postdata, message, callback){
	$.ajax({
		type: "POST",// 指定是post还是get,当然此处要提交,当然就要用post了
		cache: "false",// 默认: true,dataType为script时默认为false) jQuery 1.2 新功能，设置为 false 将不会从浏览器缓存中加载请求信息。
		url: url,// 发送请求的地址。
		data: postdata,//发送到服务器的数据
		dataType: "json",// 返回纯json
		timeout:200000,// 设置请求超时时间（毫秒）。
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。
			var errorMessage = message+" 请求失败! textStatus: " + textStatus + ', errorThrown: ' + errorThrown;
			$('body').prepend(errorMessage);
		},
		success: function(data)
		{  //请求成功后回调函数。
			callback(data);
		}
	});
}
/**
 * get方式获取text内容
 * url 接口url
 * message 出现错误时的提示消息
 * callback 请求成功后的回调函数
 * */
function gettext(url, message, callback){
	$.ajax({
		type: "GET",// 指定是post还是get,当然此处要提交,当然就要用post了
		cache: "false",// 默认: true,dataType为script时默认为false) jQuery 1.2 新功能，设置为 false 将不会从浏览器缓存中加载请求信息。
		url: url,// 发送请求的地址。
		//data: '&fuid='+pass+'&name='+name,//发送到服务器的数据
		dataType: "text",// 返回纯json
		timeout:200000,// 设置请求超时时间（毫秒）。
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。
			var errorMessage = message+" 请求失败! textStatus: " + textStatus + ', errorThrown: ' + errorThrown;
			$('body').prepend(errorMessage);
		},
		success: function(data)
		{  //请求成功后回调函数。
			callback(data);
		}
	});
}
/**
 * post方式提交内容，并获取json格式的返回值
 * url 接口url
 * data 发送到服务器的数据，格式：'&name='+name+'&password='+pass
 * message 出现错误时的提示消息
 * callback 请求成功后的回调函数
 * */
function posttext(url, postdata, message, callback){
	$.ajax({
		type: "POST",// 指定是post还是get,当然此处要提交,当然就要用post了
		cache: "false",// 默认: true,dataType为script时默认为false) jQuery 1.2 新功能，设置为 false 将不会从浏览器缓存中加载请求信息。
		url: url,// 发送请求的地址。
		data: postdata,//发送到服务器的数据
		dataType: "text",// 返回纯json
		timeout:200000,// 设置请求超时时间（毫秒）。
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。
			var errorMessage = message+" 请求失败! textStatus: " + textStatus + ', errorThrown: ' + errorThrown;
			$('body').prepend(errorMessage);
		},
		success: function(data)
		{  //请求成功后回调函数。
			callback(data);
		}
	});
}

/***
		lis : 每个选项卡标签，数组
		conts：对应每个内容，数组
		activeClass：激活的Class
**/
function TabSwitch(lis,cons,atClass){
	cons.each(function(){$(this).hide();});
	cons.eq(0).show();
	lis.eq(0).addClass(atClass);
	lis.each(function(index){
		$(this).click(function(){
			cons.each(function(){$(this).hide();});
			cons.eq(index).show();
			lis.each(function(){$(this).removeClass(atClass);});
			lis.eq(index).addClass(atClass);
			return false;
		});
	});
}
/**
 * 自动隐藏
 * className 需要隐藏的标签的 class 名
 * secondtime 几秒后隐藏
 * */
function autohide(className, secondtime, callback){
	 setTimeout(function(){
		  $('.'+className).fadeOut();
		  callback();
	 }, secondtime*1000);
}

//JS获取字符串长度(区分中英文) 中文3个字符,英文、数字是1个字符.
function getStrLength(str) {
	 var cArr = str.match(/[^\x00-\xff]/ig);
	 return str.length + (cArr == null ? 0 : cArr.length*2);
}

function errorMsg(errmsg){
	 var _div = document.createElement("div");
	 _div.id = "errorMessage";
	 _div.innerHTML = errmsg;
	 $('body').css({"position":"relative"});
	 document.body.appendChild(_div);
	 setTimeout(function(){
		  $(_div).remove();
	 },2000);
}

function alertPop(errmsg,containerO){
	var div_height,s_top=0,w_height=0,div_top,_div = $('<div class="Alert_tc" style="display:none">'+errmsg+'</div>');
 	if(containerO!=undefined && containerO.length>0){
		$(containerO).css({"position":"relative"});
		$(_div).appendTo(containerO).fadeIn();
	}
	else {
		div_height = $(_div).height();

		s_top=document.documentElement.scrollTop?document.documentElement.scrollTop:document.body.scrollTop;
 		w_height = document.documentElement.clientHeight;
 		w_width = document.documentElement.clientWidth;

	 	div_top = s_top+(w_height-div_height)/2-120;
	 	div_left = (w_width-220)/2;

	 	$(_div).css({"top":div_top+"px","left":div_left+"px"});
		$(_div).appendTo("body").fadeIn();
	}

	 setTimeout(function(){
		  $(_div).fadeOut();
	 },3000);
}
function confirmPop(containerO, msg, yescallback, nocallback, position){
	$("#confirmPop").remove();
	switch(position){
		case "RT":
			pClass = "delate_rt";
			break;
		case "LT":
			pClass = "delate_lt";
			break;
		case "RB":
			pClass = "delate_rb";
			break;
		case "LB":
			pClass = "delate_lb";
			break;
		case "M":
			pClass = "delate_m";
			break;
		default:
			pClass = "delate_m";
			break;
	}
	var message = msg,yesMsg = "是",noMsg = "否";
	if(typeof(msg) == "object"){
		message = msg[0];
		yesMsg = msg[1];
		noMsg = msg[2];
	}
	var _div = $('<div id="confirmPop" class="TC_commom_delate '+pClass+'" style="display:none"></div>');
	var yesBtn = $('<a href="javascript:void(0)" >'+yesMsg+'</a>');
	var noBtn = $('<a href="javascript:void(0)" >'+noMsg+'</a>');
	var html = $('<p >'+message+'</p>' +
			'<div class="fclear">' +
				'<div class="TC_delate_btn no_btn"></div>' +
				'<div class="TC_delate_btn yes_btn"></div>' +
			'</div>');
	$(_div).append(html);
	$(html).find(".yes_btn").append(yesBtn);
	$(html).find(".no_btn").append(noBtn);

	if(containerO.length>0){
		$(containerO).css({"position":"relative"});
		$(_div).appendTo(containerO).fadeTo("normal",0.7);
	}
	else {
		var div_height = $(_div).height(),s_top=0,w_height=0,div_top=0,w_width=0,div_left=0;

		s_top=document.documentElement.scrollTop?document.documentElement.scrollTop:document.body.scrollTop;
 		w_height = document.documentElement.clientHeight;
 		w_width = document.documentElement.clientWidth;

	 	div_top = s_top+(w_height-div_height)/2-120;
	 	div_left = (w_width-280)/2;

	 	$(_div).css({"top":div_top+"px","left":div_left+"px"});
		$(_div).appendTo("body").fadeTo("normal",0.7);


	}

	$(yesBtn).click(function(){
		$(_div).fadeOut('fase');
		yescallback();
		return true;
	});
	$(noBtn).click(function(){
		$(_div).fadeOut('fase');
		nocallback();
		return false;
	});
}

function tipHide(id){
	$("#"+id).focus(function(){
		if($(this).val()==this.defaultValue)
			$(this).val("");
	}).blur(function(){
		 if($(this).val()=="")
			$(this).val(this.defaultValue);
	 });
}

