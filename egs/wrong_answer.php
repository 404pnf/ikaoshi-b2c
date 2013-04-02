<?php
header("Content-type: text/html; charset=utf-8");
require_once 'DB.php';
require_once 'include/user_wrong_answer.php';
$uwa = new UserWrongAnswer();

$op = $_REQUEST['op'];
$return_type = !empty($_REQUEST['return_type'])?$_REQUEST['return_type']:'json';

$uid = $_REQUEST['uid'];


switch($op){
	case "delete":
		$item_id = $_POST['item_id'];
		$arr = $uwa->delete_one_user_wrong_answer($uid, $item_id);
		break;
	case "view":
		$arr = $uwa->get_user_all_wrong_answer($uid);
		break;
}

if($return_type == 'php')
	echo $arr;
else
	echo json_encode($arr);


?>

