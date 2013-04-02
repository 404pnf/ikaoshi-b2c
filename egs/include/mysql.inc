<?php

class DB_CONNECT{
	private static $DBInstance;

 public static function db_conn(){
	if(!self::$DBInstance){
		$dsn =  "mysql://root:whxroot@localhost/egs_itest";
		self::$DBInstance = & DB::connect($dsn);
		self::$DBInstance->setFetchMode(DB_FETCHMODE_OBJECT);
		self::$DBInstance->query('set names utf8');
	}
	return self::$DBInstance;
}

/**
 * 数据库报错信息
 *
 * @param string $message 错误信息
 * @param string $sql    sql语句
 */

static function halt($message = '', $sql = '') {
		$s = '<b>Error:</b>'.$message.'<br />';
		$s .= '<b>SQL:</b>:'.$sql;
		exit($s);
}



}
?>
