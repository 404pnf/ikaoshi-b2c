<?php
header("Content-type: text/html; charset=utf-8");
require_once 'DB.php';
require_once 'include/mobile_user.php';
	$mu = new MobileUser();

	$username = $_REQUEST['username'];
	$password = $_REQUEST['password'];
	$arr = $mu->free_login($username, $password);
	if($arr['status']==1){
		$arr['status'] = 'success';
		$arr['message'] = '成功';
		$arr['infor'] = array(
			'useid'=>$username,
			'username'=>$username,
			'userole'=>'0',
			'classinfor'=>'0'
		);
	}
	else{
		$arr = array(
			'status'=>'fail', 
			'message'=>'密码输入错误');
	}
	
	echo json_encode($arr);

?>
