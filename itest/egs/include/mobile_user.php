<?php
require_once 'mysql.inc';

class MobileUser{


	/**
	 * 去itest验证用户，如果域名有修改，此处需要修改
	 * @param $username
	 * @param $password
	 * @return mixed
	 */
	private function validate_itest_user($username, $password){
		$password = md5($password);
		$url = "http://itest.fltrp.com:813/otherWebSiteLoginAccess.jsp?username=$username&password=$password";
		$content = file_get_contents($url);
// 		{ status:'success', message:'成功', infor:{ useid:'03c3750a00194365908c89115c0e5469', username:'st71',userole:'4028808e23e5c5030123e5c5c3e60002',classinfor:'演示班级1'} }
//		{ status:'fail', message:'密码输入错误'}

		$userInfo = json_decode($content,true);

		return $userInfo;
	}


	/**
	 * 从itest中更新用户信息到本系统中
	 * @param $itestuserid
	 * @param $username
	 * @param $password
	 * @param $userole
	 * @param $classinfor
	 * @return multitype:number unknown
	 */
	public function insert_or_update_user($itestuserid, $username, $userole = '', $classinfor = ''){
		$status = "0";
		$uid = "0";

		if(!empty($itestuserid) && !empty($username)){
			$password = '123456';

			$conn = DB_CONNECT::db_conn();
			$sql = "INSERT INTO `users`
				(`itestid`,`username`,`password`,`userole`,`classinfor`,`role`)
				VALUES(?,?,?,?,?,2)";
			$res = $conn->query($sql,array($itestuserid,$username,$password,$userole,$classinfor));

			if($res->code == DB_ERROR_ALREADY_EXISTS){
			//如果插入已有数据，则更新
				$sql = "UPDATE `users` SET `username` = ? , `userole` = ?, `classinfor` = ?
						WHERE  `itestid` = ? ";
				$res = $conn->query($sql,array($username,$userole,$classinfor, $itestuserid));

				if(!DB::isError($res)){
					$userid_sql = "SELECT `uid` FROM `users` WHERE `itestid` = ? ";
					$status = "1";
					$uid = $conn->getOne($userid_sql,array($itestuserid));
				}

			}
			elseif(!DB::isError($res)){
				$status = "1";
				$uid = mysql_insert_id();
			}
		}
		$arr = array('status'=>$status,
					'uid'=>$uid,
					"username" => $username,
					"role" => "2");
		return $arr;

	}
	
	public function regist($username, $password, $truename, $email){
		$status = "0";
		$uid = "0";

		if(!empty($username) && !empty($password) && !empty($email)){

			$conn = DB_CONNECT::db_conn();
			$sql = "INSERT INTO `users`
				(`username`,`password`,`email`,`role`,`itestid`,`truename`)
				VALUES(?,?,?,2,?,?)";
			$res = $conn->query($sql,array($username,$password,$email,$username,$truename));

			if($res->code == DB_ERROR_ALREADY_EXISTS){
				$userid_sql = "SELECT count(*) FROM `users` WHERE `username` = ? ";
				$c = $conn->getOne($userid_sql,array($username));
				if($c >0)
					$status = 3;//用户名重复
				else
					$status = 4;//邮箱重复
			}
			elseif(!DB::isError($res)){
				$status = "1";
				$uid = mysql_insert_id();
			}
		}
		$arr = array('status'=>$status,
					'uid'=>$uid,
					"username" => $username,
					"role" => "2");
		return $arr;

	}


	/**
	 * itest用户登录
	 * @param $username
	 * @param $password
	 * @return Ambigous <multitype:number , multitype:number, multitype:number unknown >
	 */
	public function itest_user_login($username, $password){
		$user = array('status'=>0);

		if(!empty($username) && !empty($password)){

			$itestUsers = self::validate_itest_user($username, $password);

			if(strtolower($itestUsers['status']) == 'success'){
				$useid = $itestUsers['infor']['useid'];
				$userole = $itestUsers['infor']['userole'];
				$classinfor =  $itestUsers['infor']['classinfor'];

				$user = self::insert_or_update_user($useid, $username, $userole, $classinfor);
			}
		}

		return $user;
	}

	/**
	 * 后台用户登录，如管理员
	 * @param unknown_type $username
	 * @param unknown_type $password
	 * @return multitype:number
	 */
	public function backstage_login($username, $password){
		$arr = array("status" => 0);

		if(!empty($username) && !empty($password)){

			$conn = DB_CONNECT::db_conn();
			$sql = "SELECT `uid`,`username`,`role` FROM `users` WHERE username=? AND password=? LIMIT 1";

			$conn->setFetchMode(DB_FETCHMODE_ASSOC);
			$user = $conn->getRow($sql,array($username, $password));

			if(!DB::isError ($user)&&!empty($user)){
				$arr=$user;
				$arr['status'] = 1;
			}
		}
		return $arr;
	}


	/**
	 * 散户(普通)用户通过客户端入口登录,模仿itest用户验证方式
	 * @param unknown_type $username
	 * @param unknown_type $password
	 * @return multitype:number
	 */
	public function free_login($username, $password){
		$arr = array("status" => 0);

		if(!empty($username) && !empty($password)){

			$conn = DB_CONNECT::db_conn();
			$sql = "SELECT `uid`,`username`, `password`,`role` FROM `users` WHERE username=? LIMIT 1";

			$conn->setFetchMode(DB_FETCHMODE_ASSOC);
			$user = $conn->getRow($sql,array($username));

			$pass = md5($user['password']);
			unset($user['password']);
			if(!DB::isError ($user)&&!empty($user) && strtoupper($pass) == strtoupper($password)){
				$arr=$user;
				$arr['status'] = 1;
			}
		}
		return $arr;
	}



}





