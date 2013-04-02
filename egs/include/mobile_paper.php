<?php
require_once 'mysql.inc';

/**
 * @author wanghaixia
 *
 */
class MobilePaper{


	/**
	 * 根据模板id获取试卷内容，按答卷人数多少排序
	 * @param $sid 模板id
	 * @param $num 试卷数
	 * @return 试卷信息数组
	 */
	public function  paper_list_by_sid($sid,$num = 10){

		$conn = DB_CONNECT::db_conn();
		$num = $num >0 ? $num : 10;
		if(!empty($num)){
			$limitCondition = " LIMIT $num ";
		}
	    $sql = "SELECT ei.`paper_id` , `paper_name` , count(ei.`paper_id`) count,`sid` ,`tid` term_id
				FROM `exam_info` ei
					LEFT JOIN `user_results` ur ON ur.`paper_id` = ei.`paper_id`
				WHERE `sid`= ?
				GROUP BY ei.`paper_id`
				ORDER BY count DESC $limitCondition";

	     $conn->setFetchMode(DB_FETCHMODE_ASSOC);
	     $arr = $conn->getAll($sql,array($sid));

	     return  $arr;
	}


}





