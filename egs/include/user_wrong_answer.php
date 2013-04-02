<?php
require_once 'mysql.inc';

class UserWrongAnswer{

	public function test(){
		$conn = DB_CONNECT::db_conn();
		$sql = "SELECT * FROM user_wrong_answer";
		$conn->setFetchMode(DB_FETCHMODE_ASSOC);
/* 		$res = $conn->query($sql);
		$row = $res->fetchRow(); */
		$row = $conn->getAssoc($sql);
		return $row;
	}

	/**
	 * 记录用户错题库，主观题不记录（由调用此接口的程序负责过滤）
	 * @param $uid 用户id
	 * @param array $answer_array 用户答案，实例 array( array("item_id"=>1,"result_id"=>2,"paper_id"=>1,"is_correct"=>1),array("item_id"=>2,"result_id"=>2,"paper_id"=>1,"is_correct"=>0))
	 * @return NULL
	 */
	public function record_user_wrong_answer($uid, $answer_array){
		if($uid<=0||empty($answer_array))	return FALSE;
		$user_wrong_answer_ids = self::get_user_wrong_answer_ids($uid);

		foreach ($answer_array as $answer){
			if($answer['is_correct']){
				//正确：如果试题在错题库中，则删除；否则不做任何操作
				$item_id = $answer['item_id'];
				if(in_array($item_id, $user_wrong_answer_ids)){

					//如果此题在错题库中，则删除
					$count = self::delete_one_user_wrong_answer($uid, $item_id);

					//如果删除成功，则从数组$user_wrong_answer_ids中删除此item_id;
					if(is_numeric($count)&&$count>0){
						foreach ($user_wrong_answer_ids as $index => $had_item_id){
							if($item_id == $had_item_id){
								unset($user_wrong_answer_ids[$index]);
							}
						}
					}
				}
			}
			else{
				//错误，插入错题库
				$item_id = $answer['item_id'];
				$result_id = $answer['result_id'];
				$paper_id = $answer['paper_id'];
				$arr = self::insert_one_user_wrong_answer($uid, $item_id, $result_id, $paper_id);

				if($arr['count']>0 && $arr['op']>0){
					//插库成功则增加此试题id到错题库id（$user_wrong_answer_ids数组）中
					array_push($user_wrong_answer_ids,$item_id);
				}
			}
		}

		return true;

	}

	/**
	 * 把错题插入错题库：如果已经有 uid、item_id 一致的数组 ，则更新错题的时间与用户答案id
	 * @param $uid
	 * @param $item_id
	 * @param $result_id
	 * @param $paper_id
	 * @return 数组：插入OR更新数据的条数，更新 还是 插入
	 */
	public function insert_one_user_wrong_answer($uid, $item_id, $result_id, $paper_id){
		$count = 0;
		$op = 0;//0 更新，1 插入

		if($uid>0 && $item_id>0 && $result_id>0){
			$conn = DB_CONNECT::db_conn();

			$now = date('Y-m-d H:i:s');
			$sql = "INSERT INTO `user_wrong_answer`
				(`uid`,`item_id`,`result_id`,`paper_id`,`createtime`)
				VALUES(?,?,?,?,?)";
			$res = $conn->query($sql,array($uid,$item_id,$result_id,$paper_id,$now));

			if($res->code == DB_ERROR_ALREADY_EXISTS){
			//如果插入已有数据，则更新
				$sql = "UPDATE `user_wrong_answer` SET `result_id` = ? , `paper_id` = ? , `createtime` = ?
						WHERE  `uid` = ? AND `item_id` =?";

				$res = $conn->query($sql,array($result_id, $paper_id, $now, $uid,$item_id));
				$op = 0;
			}
			else{
				$op = 1;
			}

			$count = $conn->affectedRows();
		}

		return array("count"=>$count,"op"=>$op);
	}


	/**
	 * 删除错题库的某条数据，是否判断当前用户与uid是否一致（这个由调接口的人员负责判断吧）
	 * @param $uid 用户id
	 * @param $item_id 试题id
	 * @return 删除的数据条数
	 */
	public function delete_one_user_wrong_answer($uid, $item_id){
		$count = 0;
		$conn = DB_CONNECT::db_conn();

		$sql = "DELETE FROM user_wrong_answer WHERE uid = ? AND item_id = ?";
		$conn->query($sql,array($uid,$item_id));
		$count = $conn->affectedRows();

		return $count;
	}

	/**
	 * 得到用户错题库的所有试题id数组
	 * @param $uid 用户id
	 * @return array 试题数组
	 */
	public function get_user_wrong_answer_ids($uid){
		$conn = DB_CONNECT::db_conn();
		$sql = "SELECT item_id FROM user_wrong_answer WHERE uid = ?";
		$res= $conn->getCol($sql,'item_id',array($uid));

		return $res;

	}


	public function get_user_all_wrong_answer($uid){
		//只能得到最多两个层级的试题关系，并且不调取无实际意义的试题说明（basic_type = 3）

		include_once 'api.inc';
		$output = "";

		$conn = DB_CONNECT::db_conn();
		$sql = "SELECT ei.`parent_id`, ei.`item_id` , ur.`result_id`, ur.`uid`, ur.`paper_id`, ei.`points`,
				     i.`name` item_name,i.`type`, t.name type_name, i.`basic_type`, bi.`name` basic_name, i.`body`, i.`description`,
				     i.`is_random`,i.`file_id`, f.`fileurl`
				FROM `user_wrong_answer` ur
				    LEFT JOIN `exam_item` ei on ei.`paper_id`=ur.`paper_id` AND ur.`item_id` = ei.`item_id`
	                JOIN `item_info` i ON i.`item_id` = ei.`item_id` AND i.`basic_type` in (1,2)
	                LEFT JOIN `basic_item_type` bi ON bi.`basic_type_id` = i.`basic_type`
	                LEFT JOIN `item_type` t ON t.`type_id` = i.`type`
	                LEFT JOIN `files` f ON f.`fid` = i.`file_id`
				 WHERE ur.`uid`=? ORDER BY ur.`createtime` DESC";

		$res = $conn->query($sql,array($uid));

		while($row = $res->fetchRow()){
			$item_id = $row->item_id;
			$parent_id = $row->parent_id;
			$result_id = $row->result_id;
			// -- 与答案有关 --
			$user_answer = array();
			$answer_properties = array();
			$answers = '';
			if($row->basic_type==1 || $row->basic_type==2){

				$answer_properties_arr = API::query_item_answers_properties($row->basic_type,$item_id);
				$answer_properties = $answer_properties_arr[$item_id];
				//答案的属性，选择题选项是否随机显示，填空题是否是客观题，空的大小长度。

				if($row->basic_type == 1){
					$user_answer = API::query_multichoice_user_answers_by_result_id($result_id,$item_id);
				}
				elseif($row->basic_type==2){
					$user_answer = API::query_blankfilling_user_answers_by_result_id($result_id,$item_id);
				}
				//用户的答案

				$answers = API::query_item_answers($row->basic_type,$item_id,$answer_properties["choice_random"]);
				//录入的答案与试题解析

			}
			// -- end 与答案有关 --

			if($row->fileurl!='')
				$files = array("file_status" => 1, "fileurl"=>$row->fileurl);
			else
				$files = array("file_status" => 0);
			$item = array(
					"item_id" => $row->item_id,
					"name" => stripslashes(htmlspecialchars_decode($row->item_name)),
					"uid" => $row->uid,
					"body" => stripslashes(htmlspecialchars_decode($row->body)),
					"description" => stripslashes(htmlspecialchars_decode($row->description)),
					"files" => $files,
					"type" => array($row->type => $row->type_name ),
					"basic_type" =>array($row->basic_type=> $row->basic_name ),
					"points" => $row->points,
					"answers" => $answers,
					"is_random" => $row->is_random    //子题目是否随机
			) ;

			if($row->basic_type==1 || $row->basic_type==2){
				$item = array_merge($item,$answer_properties,$user_answer);
			}


// 如果父级id不为0，则查询父级试题。如果父级试题不为空，则把当前试题赋值给父级试题的subque，保持父子级之间的层级关系。
			if($parent_id != 0){
				$parent_item = self::get_parent_item_by_exam_item_id($parent_id);
				if(!empty($parent_item)){
					$parent_item['subque'][] = $item;
					$item = $parent_item;
				}
			}

			$output[] = $item;

		}


		return $output;


	}


	/**
	 * 根据 试题的父级试卷试题id 来获得 父级题目内容，只包括试题材料，递归调用，保留父级题目之间的层级关系。
	 * @param $exam_item_id 试卷试题id
	 * @return 多层级的试题数组
	 */
	public function get_parent_item_by_exam_item_id($exam_item_id){
		$item = array();
		$conn = DB_CONNECT::db_conn();
		$sql = "SELECT ei.`parent_id`, ei.`item_id`, ei.`paper_id`, ei.`points`,
				     i.`name` item_name,i.`type`, t.`name` type_name, i.`basic_type`, bi.`name` basic_name, i.`body`, i.`description`,
				     i.`is_random`,i.`file_id`, f.`fileurl`, mif.`mid`, mif.`feedback`
				FROM `exam_item` ei
                    JOIN `item_info` i ON i.`item_id` = ei.`item_id` AND i.`basic_type` = 4
	                LEFT JOIN `basic_item_type` bi ON bi.`basic_type_id` = i.`basic_type`
	                LEFT JOIN `item_type` t ON t.`type_id` = i.`type`
	                LEFT JOIN `files` f ON f.`fid` = i.`file_id`
                    LEFT JOIN `material_items_feedback` mif ON mif.`item_id` = ei.`item_id`
				 WHERE ei.`exam_item_id`=? ";

		$res = $conn->query($sql, array($exam_item_id));
		$row = $res->fetchRow();

		if(!empty($row)){
			if($row->fileurl!='')
				$files = array("file_status" => 1, "fileurl"=>$row->fileurl);
			else
				$files = array("file_status" => 0);

			$item = array(
					"item_id" => $row->item_id,
					"name" => stripslashes(htmlspecialchars_decode($row->item_name)),
					"body" => stripslashes(htmlspecialchars_decode($row->body)),
					"description" => stripslashes(htmlspecialchars_decode($row->description)),
					"type" => array($row->type => $row->type_name ),
					"basic_type" =>array($row->basic_type=> $row->basic_name ),
					"files" => $files,
					"answers" =>array($row->mid => array("feedback" =>stripslashes(htmlspecialchars_decode( $row->feedback)))) ,
					"is_random" => $row->is_random,
					"score" => $row->points
				);

// 			递归：如果当前 item 的父级 id 不为0，则 再次调用本身，并且保留层级关系；否则 返回 当前 item
// 				根据父级id 找到 父级试题，一直递归，直到父级id为0 或者 父级试题为空。
			$parent_exam_item_id = $row->parent_id;
			if($parent_exam_item_id!=0){
				$parent_item = self::get_parent_item_by_exam_item_id($parent_exam_item_id);
				if(!empty($parent_item)){
					$parent_item['subque'][] = $item;
				}
			}

		}

		return $item;


	}

}





