<?php
header("Content-type:text/html;charset=utf-8");

$op = $_REQUEST['op'];
if($op=='user_login'){
	$uid = $_REQUEST['uid'];
	$session = $_REQUEST['sess_id'];
	$app_key = $_REQUEST['key']!=''?$_REQUEST['key']:'aa';
	
	$url = "http://2u4u.com.cn/query/user/";
	if($app_key!=''&&$session!=''&&$uid>0){
		$url .= $app_key."/".$session."/".$uid;
		$content = file_get_contents($url);
		echo $content;	
	}else{
	    echo '失败';	
	}
}
if($op=='user_name'){
	$uid = $_REQUEST['uid'];
	$app_key = $_REQUEST['key']!=''?$_REQUEST['key']:'aa';	
	$url = "http://2u4u.com.cn/query/username/";
	if($app_key!=''&&$uid>0){
		$url .= $app_key."/".$uid;
		$content = file_get_contents($url);
		echo $content;	
	}else{
	    echo '失败';	
	}
}





?>