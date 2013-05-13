<?php
header("Content-type: text/html; charset=utf-8");
require_once 'DB.php';

$class = $_REQUEST['c'];
$func = $_REQUEST['f'];
$return_type = !empty($_REQUEST['return_type'])?$_REQUEST['return_type']:'json';

switch($class){

	case "user":
		require_once 'include/mobile_user.php';
		$mu = new MobileUser();

		switch ($func){
			case "itest":
				$username = $_REQUEST['username'];
				$password = $_REQUEST['password'];
				$arr = $mu->itest_user_login($username, $password);
				break;
			case "insert":
				$itestuserid = $_REQUEST['itestuid'];
				$username = $_REQUEST['username'];
				$userrole = $_REQUEST['userrole'];
				$classinfor = $_REQUEST['classinfor'];
				$arr = $mu->insert_or_update_user($itestuserid, $username, $userrole, $classinfor);
				break;
			case "back":
				$username = $_REQUEST['username'];
				$password = $_REQUEST['password'];
				$arr = $mu->backstage_login($username, $password);
				break;
		}
		break;
	case "paper":
		require_once 'include/mobile_paper.php';
		$mp = new MobilePaper();

		switch ($func){
			case "list_by_sid":
				$sid = $_REQUEST['sid'];
				$num = $_REQUEST['num'];
				$arr = $mp->paper_list_by_sid($sid, $num);
				break;
			case "result_list":
				$uid = $_REQUEST['uid'];
				$tids = $_REQUEST['tids'];
				$num = $_REQUEST['num'];
				$arr = $mp->result_list_by_tids($uid, $tids, $num);
				break;
		}
		break;
	case "wrong_answer":
		require_once 'include/user_wrong_answer.php';
		$uwa = new UserWrongAnswer();
		$uid = $_REQUEST['uid'];

		switch ($func){
			case "delete":
				$item_id = $_POST['item_id'];
				$arr = $uwa->delete_one_user_wrong_answer($uid, $item_id);
				break;
			case "view":
				$arr = $uwa->get_user_all_wrong_answer($uid);
				break;
		}
		break;


}

if($return_type == 'php')
	echo $arr;
else
	echo json_encode($arr);




?>
