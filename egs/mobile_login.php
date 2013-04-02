<?php
header("Content-type: text/html; charset=utf-8");
require_once 'DB.php';
require_once 'include/mobile_user.php';

$mu = new MobileUser();

$op = $_REQUEST['op'];
$return_type = !empty($_REQUEST['return_type'])?$_REQUEST['return_type']:'json';

$username = $_REQUEST['username'];
$password = $_REQUEST['password'];

switch($op){
	case "itest":
		$arr = $mu->itest_user_login($username, $password);
		break;
	case "back":
		$arr = $mu->backstage_login($username, $password);
		break;
}

if($return_type == 'php')
	echo $arr;
else
	echo json_encode($arr);




?>
