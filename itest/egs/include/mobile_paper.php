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

	/**
	 * 根据模板id获取历史记录，按分类id分组，按时间倒序排列。
	 * @param $tids 分类id,多个分类id用英文逗号分割
	 * @return 按模板分组的，历史记录数组
	 */
	public function  result_list_by_tids($uid, $tids, $num){
		$conn = DB_CONNECT::db_conn();
		$arr = array();
		if(empty($uid) || !is_numeric($uid))
			return NULL;

		$sql = "SELECT ur.`result_id`, ur.`uid`, ur.`paper_id`,ei.`paper_name`, ei.`tid`, et.`name` exam_type, ur.`time_used`, ur.`time_end`, ur.`score`, ur.`is_valid`,ei.`objective_points`,ei.`paper_id`,ei.`points`
				FROM `user_results` ur
				LEFT JOIN `exam_info` ei ON ei.`paper_id`=ur.`paper_id`
				LEFT JOIN `exam_type` et ON ei.`tid`=et.`tid`
				WHERE ur.`is_valid`=1 ";

		$sql_condition = '';

		if(!empty($num)){
			$limitCondition = " LIMIT $num ";
		}
		if($uid != -1)//管理员查询所有人数据
			$sql_condition .= " AND ur.`uid`=".$uid;
		if($tids > 0){
			$tids = str_replace(",", " OR ei.`tid`=", $tids);
			$sql_condition .= " AND (ei.`tid`=$tids)";

		}
		$sql_condition .= "  ORDER BY ur.`time_end` DESC $limitCondition";
		$sql .= $sql_condition;

		$result = $conn->getAll($sql);

		$tid_arr = array();
		foreach ($result as $row){
			$tid = $row->tid;
			if(!in_array($tid, $tid_arr)){
				array_push($tid_arr, $tid);
			}
		}

		foreach ($tid_arr as $index=>$tid){
			$arr[$index] = array("tid"=>$tid);

			foreach ($result as $row){
				if($row->tid == $tid){
					$arr[$index]["exam_type"] = $row->exam_type;
					$arr[$index]["list"][] = array(
							"result_id" => $row->result_id,
							"paper_id" => $row->paper_id,
							"paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
							"uid" =>  $row->uid,
							"time_end"=>date("Y-m-d H:i:s",$row->time_end),
							"time_used"=>$row->time_used,
							"user_score"=>$row->score,
							"objective_points"=>$row->objective_points,
							"rate"=>$row->score/$row->objective_points,
							"exam_points"=>$row->points,
					);
				}
			}

		}

		// 	 $conn->disconnect();
		return $arr;

	}





}





