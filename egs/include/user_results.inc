<?php
require_once 'mysql.inc';

class RESULTS extends DB_CONNECT {

/**
 * 多选题验证
 *
 * @param unknown_type $item_id
 * @param unknown_type $multi_answer_id
 * @return unknown
 */
static function  check_multichoice_answers($item_id,$multi_answer_id){

     $conn = DB_CONNECT::db_conn();
     $is_correct = 0;
     if(is_array($multi_answer_id)){

	     foreach ($multi_answer_id as $value){

			$sql ="SELECT `is_correct` FROM `multichoice_answers` WHERE `item_id`=".$item_id." AND `answer_id`=".$value;
			$res =  $conn->query($sql);
			$row = $res->fetchRow();
			if($row->is_correct){
			  	$is_correct = 1;
			}else{
			    return false;
			}
	     }
     }

     // 	 $conn->disconnect();
     return  $is_correct;
}




/**
 * 单选题验证
 *
 * @param unknown_type $item_id
 * @param unknown_type $user_answer_id
 * @return unknown
 */

static function  check_singlechoice_answers($item_id,$user_answer_id){

    $conn = DB_CONNECT::db_conn();
	$sql ="SELECT `is_correct` FROM `multichoice_answers` WHERE `item_id`=".$item_id." AND `answer_id`=".$user_answer_id;
	$res =  $conn->query($sql);
	$row = $res->fetchRow();
	if($row->is_correct){
	  	return true;
	 }else{
	    return false;
	 }

}

/**
 * 客观填空题的验证
 *
 * @param unknown_type $item_id
 * @param unknown_type $user_answer
 * @return unknown
 */
static function  check_blankfilling_answers($item_id,$user_answer){

    $conn = DB_CONNECT::db_conn();

	$sql ="SELECT `filling_answer_id` id
	       FROM `blank_filling_answers`
	       WHERE `item_id`=".$item_id." AND `correct_answer` LIKE '".$user_answer."'";

	$res =  $conn->query($sql);
	$row = $res->fetchRow();
	if($row->id){
	  	return true;
	 }else{
	    return false;
	}

}




/**
 * 获取试卷中试题的分值
 *
 * @param unknown_type $paper_id
 * @param unknown_type $item_id
 * @return unknown
 */

static function get_exam_item_points($paper_id,$item_id){

	 $conn = DB_CONNECT::db_conn();
	 $query = "SELECT `points` FROM `exam_item`  WHERE `item_id`=".$item_id." AND `paper_id`=".$paper_id." LIMIT 1";
     $result =  $conn->query($query);
     $row = $result->fetchRow();
     $id = $row->points;
     // 	 $conn->disconnect();
     return  $id;

}

/**
 * 获取试卷客观题总分
 *
 * @param unknown_type $paper_id
 * @return unknown
 */

static function get_exam_objective_points($paper_id){

	 $conn = DB_CONNECT::db_conn();
	 $query = "SELECT `objective_points`  FROM `exam_info` WHERE `paper_id`=".$paper_id." LIMIT 1";

     $result =  $conn->query($query);
     $row = $result->fetchRow();
     $id = $row->objective_points;
     // 	 $conn->disconnect();
     return  $id;

}



/**
 * 更新用户总得分
 *
 * @param unknown_type $id
 * @param unknown_type $score
 * @return unknown
 */

static function update_user_result_total_points($id,$score,$rate){

	 $conn = DB_CONNECT::db_conn();
	 $query = "UPDATE `user_results` SET `score`=".$score." ,`rate`=".$rate." WHERE `result_id`=".$id." LIMIT 1";

	 $conn->query($query);
     $row = $conn->affectedRows();
     // 	 $conn->disconnect();
     return  $row;

}

/**
 * 找到父级题加上子题目得分
 *
 * @param unknown_type $id
 * @param unknown_type $score
 * @param unknown_type $parent_arr
 * @return unknown
 */

function find_parent_and_add_points($id,$score,$parent_arr){

	if($parent_arr[$id]['pid']==0)
	   $this_arr[$id] = 0;
	else{
		//$this_arr[$id] = $arr[$id];
		$this_arr[$parent_arr[$id]['pid']] += $score;
	    if($parent_arr[$id]['pid']>0)
	      //$this_arr = array_merge($this_arr,find_parent($parent_arr[$id],$score,$parent_arr));
	      // find_parent($parent_arr[$id],$score,$parent_arr);
	       $this_arr += self::find_parent_and_add_points($parent_arr[$id]['pid'],$score,$parent_arr);
	}
	  // var_dump($this_arr);
	   	return $this_arr;
}




}
?>