<?php

require_once 'mysql.inc';

class UPDATE extends DB_CONNECT {



/**
 * 导入试卷时，插入试题对应的本试卷的流水号
 *
 * 输入试题id，流水号，excel中的序号，状态值（完全插入成功，状态为1，默认为0）
 * 返回影响的行数值
 *
 */
static function item_excel_liushui_update($item_id,$excel_liushui,$excel_id,$status=1){

    $conn = parent::db_conn();
	$sql = "UPDATE `item_excel_liushui` SET `status` = ? AND `excel_id` = ?
	       WHERE `item_id`=? AND `excel_liushui`=? LIMIT 1";

    $res = $conn->query($sql,array($status, $excel_id, $item_id, $excel_liushui));
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = $conn->affectedRows();
	}
   //$conn->disconnect();
    return $id;

}



/**
 * 插入试题信息
 *
 * 输入数组
 * 返回插入试题的id值
 *
 */

static function item_info_update($item_id,$arr){

    $conn = parent::db_conn();
  	$sql = "SELECT COUNT(*) num FROM `item_info` WHERE `item_id`=".$item_id;

 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();

  	if($row->num>0){

		$uid = $arr['uid']>0?$arr['uid']:1;

		$sql = "UPDATE `item_info` SET
					`name` = '".htmlspecialchars(addslashes($arr['name']))."',
					`type` = ".(int)$arr['type'].",
					`basic_type` = ".(int)$arr['basic_type'].",
					`body` = '".htmlspecialchars(addslashes(htmlspecialchars_decode(stripslashes($arr['body']))))."',
					`description` = '".htmlspecialchars(addslashes(htmlspecialchars_decode($arr['description'])))."',
					`points` = ".(int)$arr['point'].",
					`time_limit` = ".(int)$arr['time_limit'].",
					`created` = ".time().",
					`file_id` = ".$arr['file_id'].",
					`uid` = ".$uid.",
					`status` = 1,
					`is_random` = ".$arr['is_random']."
				WHERE `item_id` = ". $item_id;

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
			$item_id = $conn->affectedRows();
		}


	}
	else
		$item_id = INSERT::item_info_insert($arr);

   ////$conn->disconnect();
    return $item_id;

}





/**
 * 插入选择题选项
 *
 *
 */

static function multichoice_answers_update($item_id,$answer){
    $conn = parent::db_conn();
    $answer_id_arr = array();

    $multichoice_answer = "SELECT `answer_id`  FROM  `multichoice_answers` WHERE `item_id` = ".$item_id."
    	 ORDER BY `answer_id`";

    $res = $conn->query($multichoice_answer);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$multichoice_answer);
	}else{
		while($row = $res->fetchRow()){
			$answer_id_arr[] = $row->answer_id;
		}

 	    $num = sizeof($answer_id_arr);
	}

    $answer_num = sizeof($answer);


  //  echo "<br>multichoice_answers_update: multichoice_answer::: ".$multichoice_answer.";;;;;;;num = ".$num.";;;;answer_num = ".$answer_num."<br>";

	if($num <= $answer_num){
		$update_num = $num;
	}
	else{
		$update_num = $answer_num;

		$answer_ids = '';

		 for ($i=$answer_num; $i<$num; $i++){
		 	$answer_ids .= $answer_id_arr[$answer_num].',';
		 }
		$answer_ids = substr($answer_ids,0,-1);    //去掉,

		$multichoice_delete = "DELETE FROM  `multichoice_answers` WHERE `answer_id` in (".$answer_ids." )" ;

		//echo "<br>multichoice_answers_update: multichoice_delete::: ".$multichoice_delete;


		$res = $conn->query($multichoice_delete);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$multichoice_num);
		}

	}

	for ($i=0; $i<$update_num; $i++){
		$value = $answer[$i];
		$answer_id = $answer_id_arr[$i];

		 $query = "UPDATE `multichoice_answers` SET ";

	    if (trim($value['answer']) != "") {
	    	if(isset($value['checkbox'])&&!isset($value['is_correct'])){
	    	   $is_correct =  $value['checkbox']=='on'?1:0;
	    	}else{
	    	   $is_correct =  $value['is_correct']>0?$value['is_correct']:0;
	    	}
            $query .=" `answer` = '".htmlspecialchars(addslashes(htmlspecialchars_decode($value['answer'])))."',`feedback` = '".htmlspecialchars(addslashes(htmlspecialchars_decode($value['explain'])))."',`is_correct` = ".$is_correct;
  	    }

		$query .=" WHERE `answer_id` = ".$answer_id;

		//echo "<br>multichoice_answers_update: query::: ".$query;

		$res = $conn->query($query);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
			$id = $conn->affectedRows();
		}
	}

	if($num < $answer_num){
		    $query = "INSERT INTO `multichoice_answers`(`answer_id`, `item_id`, `answer`, `feedback`, `is_correct`) VALUES ";

	   for ($i=$num; $i<$answer_num; $i++){
			$value = $answer[$i];
			if (trim($value['answer']) != "") {
				if(isset($value['checkbox'])&&!isset($value['is_correct'])){
				   $is_correct =  $value['checkbox']=='on'?1:0;
				}else{
				   $is_correct =  $value['is_correct']>0?$value['is_correct']:0;
				}
				$query .="('',".$item_id.",'".htmlspecialchars(addslashes(htmlspecialchars_decode($value['answer'])))."','".htmlspecialchars(addslashes(htmlspecialchars_decode($value['explain'])))."',".$is_correct."),";
			}
		}

		$query = substr($query,0,-1);    //去掉,
		$res = $conn->query($query);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
			$id = $conn->affectedRows();
		}

	}


   ////$conn->disconnect();
    return $item_id;
 }


 /**
 * 插入选择题设置信息
 *
 *
 */

 static function multichoice_properties_update($item_id,$choice_multi,$choice_random){

   $conn = parent::db_conn();
  	$sql = "SELECT COUNT(*) num FROM `multichoice_properties` WHERE `item_id`=".$item_id;
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();

  	if($row->num>0){

		$query = "UPDATE `multichoice_properties` SET
					`choice_multi` = ".$choice_multi.", `choice_random` = ".$choice_random."
					 WHERE `item_id`=".$item_id;

		$res = $conn->query($query);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
			$id = $conn->affectedRows();
		}
	}
	else
		INSERT::multichoice_properties_insert($item_id,$choice_multi,$choice_random);

   ////$conn->disconnect();
    return $id;
 }







 /**
 * 插入填空答案
 *
 *
 */


static function blank_filling_answers_update($item_id,$is_objective,$answer){

	$conn = parent::db_conn();

	$correct_answer = isset($answer['answer'])?htmlspecialchars(addslashes(htmlspecialchars_decode($answer['answer']))):'';
	$feedbak = isset($answer['explain'])?htmlspecialchars(addslashes(htmlspecialchars_decode($answer['explain']))):'';


  	$sql = "SELECT COUNT(*) num FROM `blank_filling_answers` WHERE `item_id`=".$item_id;
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();

  	if($row->num>0){
	    $sql = "UPDATE `blank_filling_answers` SET `correct_answer` = '".$correct_answer."',  `feedback` = '".$feedbak."', `is_objective` = ".$is_objective." WHERE `item_id`=".$item_id;

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}

	}else{
 		INSERT::blank_filling_answers($item_id,$is_objective,$answer);
 	}


   ////$conn->disconnect();
    return $item_id;

 }

static function blank_filling_properties_update($item_id,$blank_size){

	$conn = parent::db_conn();
  	$sql = "SELECT COUNT(*) num FROM `blank_filling_properties` WHERE `item_id`=".$item_id;
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();

  	if($row->num>0){
	    $sql = "UPDATE `blank_filling_properties` SET `blank_size` = '".$blank_size."' WHERE 	`item_id`=".$item_id;

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}

	}else{
 		INSERT::blank_filling_properties($item_id,$blank_size);
 	}


   ////$conn->disconnect();
    return $item_id;
 }


 /**
  * 题目材料解析的插入
  *
  * @param unknown_type $term_id
  * @param unknown_type $item_id
  * @return unknown
  */

 static function material_items_feedback_update($item_id,$feedback){
 	$conn = parent::db_conn();
  	$sql = "SELECT COUNT(*) num FROM `material_items_feedback` WHERE `item_id`=".$item_id;
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();

 	$feedback = htmlspecialchars(addslashes(htmlspecialchars_decode($feedback)));

  	if($row->num>0){
	    $sql = "UPDATE `material_items_feedback` SET `feedback` = '".$feedback."' WHERE 	`item_id`=".$item_id;

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}
	}else{
 		INSERT::material_items_feedback_insert($item_id,$feedback);
 	}

   ////$conn->disconnect();
    return $item_id;

 }




/**
 *  试卷结构基本信息插入
 *
 * $array 数组
 *
 * 返回插入的id号
 */

static function exam_structure_info_update($array){

	$points = isset($array['exam_score'])?(int)$array['exam_score']:0;
	$title = !empty($array['exam_title'])?$array['exam_title']:'';
	$description = !empty($array['exam_description'])?$array['exam_description']:'';
	$time_limit =  isset($array['exam_limittime'])?(int)$array['exam_limittime']:0;
    $tid = isset($array['tid'])?(int)$array['tid']:1;
    $time = time();
    $uid = 1;
    $status = 1 ;
	$conn = parent::db_conn();


	$sql = "UPDATE INTO `exam_structure_info` (`sid`, `tid`, `uid`, `title`, `description`, `points`,`time_limit`, `created`, `status`)
            SELECT '',".$tid.",".$uid.",'".$title."','".htmlspecialchars(addslashes(htmlspecialchars_decode($description)))."',".$points.",".$time_limit.",".$time.",".$status."
            FROM `exam_structure_info`
	        WHERE
            NOT EXISTS (SELECT 1 FROM `exam_structure_info` WHERE `title` = '".$title."') LIMIT 1";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id =  mysql_update_id();
	}
     //$conn->disconnect();
     return  $id;

  }


 static function exam_structure_update($array){

 	$conn = parent::db_conn();
	$sid = isset($array['sid'])?$array['sid']:0;
 	$term_id = isset($array['term_id'])?$array['term_id']:0;   //和个关系，以逗号隔开；或的关系，以-隔开
 	$basic_type = isset($array['basic_type'])?$array['basic_type']:0;
 	$item_type = isset($array['item_type'])?$array['item_type']:0;
 	$parent = isset($array['parent_id'])?$array['parent_id']:0;  //上一级的structure_id
 	$total = isset($array['num'])?$array['num']:0;
 	$points = isset($array['points'])?$array['points']:0;
 	$time_limit  = isset($array['time_limit'])?$array['time_limit']:0;
 	$serial_str = isset($array['serial_str'])?$array['serial_str']:'';
 	$serial_num = isset($array['serial_num'])?$array['serial_num']:0;
 	$is_submit = isset($array['is_submit'])?$array['is_submit']:0;
 	$check_relation = isset($array['check_relation'])?$array['check_relation']:1;



	$sql = "UPDATE INTO `exam_structure` (`structure_id`,`sid`, `term_id`,`basic_type`,`item_type`, `parent`,`check_relation`, `total`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`)
	        VALUES ('',".$sid.",'".$term_id."',".$basic_type.",".$item_type.",".$parent.",".$check_relation.",".$total.",".$points.",".$time_limit.",'".$serial_str."','".$serial_num."',".$is_submit.")";


    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id =  mysql_update_id();
	}
   ////$conn->disconnect();
    return $id;

 }


static function exam_comments_update($name,$description,$weight){

	$level_id = self::exam_comments_level_update($name);
	$weight = $weight>0?$weight:0;
	$conn = parent::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	if($level_id>0){
		$sql = "UPDATE INTO `exam_comments` (`cid`,`level_id`, `description`, `weight`) VALUES ('',".$level_id.",'".$description."',".$weight.")";
	    $res = $conn->query($sql);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
	 	    $id = mysql_update_id();
		}
	}
   ////$conn->disconnect();
    return $id;

}


static function exam_comments_level_update($name){

	$conn = parent::db_conn();
   	$sql = "SELECT `level_id` FROM `exam_comments_level` WHERE `level_name`='".$name."' LIMIT 1";
   	$rs = $conn->query($sql);
    $row = @$rs->fetchRow();
    if($row->level_id<1){
    	$query = "UPDATE INTO `exam_comments_level`(`level_name`) VALUES ('".$name."')";
	    $res = $conn->query($query);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
	 	    $id = mysql_update_id();
		}
    }else{
    	 $id = $row->level_id;
    }

   ////$conn->disconnect();
    return $id;

}

static function structure_comments_global_update($id,$exam_type_id,$level_id,$min_rate,$max_rate,$resources_tags1,$or_resources_tags2){

    $conn = parent::db_conn();


    $sql = "UPDATE `exam_structure_comment_global` set `level_id`= ".$level_id.", `min_rate`= ".$min_rate.", `max_rate`= ".$max_rate.", `resources_tags1`= '".$resources_tags1."', `or_resources_tags2`= '".$or_resources_tags2."' WHERE `id` = ".$id." AND `exam_type_id` = ".$exam_type_id ;


    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}

   ////$conn->disconnect();
    return $id;

}
static function structure_attr_comments_global_update($id,$aid,$level_id,$min_rate,$max_rate,$resources_tags1,$or_resources_tags2){

    $conn = parent::db_conn();


    $sql = "UPDATE `attribute_exam_structure_comment_global` set `level_id`= ".$level_id.", `min_rate`= ".$min_rate.", `max_rate`= ".$max_rate.", `resources_tags1`= '".$resources_tags1."', `or_resources_tags2`= '".$or_resources_tags2."' WHERE `id` = ".$id." AND `aid` = ".$aid ;


    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}

   ////$conn->disconnect();
    return $id;

}


static function structure_comments_relate_update( $rid, $sid, $global_examType_detail_mark, $same_mark, $only_attribute ){
    $conn = parent::db_conn();

    $sql = "UPDATE `exam_structure_comment_relate` SET `sid` = ".$sid.", `global_examType_detail_mark` = ".$global_examType_detail_mark.", `same_mark` = ".$same_mark.",`only_attribute` = ".$only_attribute." WHERE `rid` = ".$rid;

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}

   ////$conn->disconnect();
    return $id;

}

static function structure_attr_comments_relate_update( $rid, $sid, $aid, $same_mark ){
    $conn = parent::db_conn();

    $sql = "UPDATE `attribute_exam_structure_comment_relate` SET `sid` = ".$sid.", `aid` = ".$aid.", `same_mark` = ".$same_mark." WHERE `rid` = ".$rid;

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}

   ////$conn->disconnect();
    return $id;

}



static function structure_comments_update($scid,$sid,$level_id,$min_rate,$max_rate,$resources,$resources_tags1,$or_resources_tags2){

    $conn = parent::db_conn();
	$resources = htmlspecialchars(addslashes(htmlspecialchars_decode($resources)));

    $sql = "UPDATE `exam_structure_comment` set `sid` = ".$sid.",`level_id`= ".$level_id.", `min_rate`= ".$min_rate.", `max_rate`= ".$max_rate.", `resources`= '".$resources."', `resources_tags1`= '".$resources_tags1."', `or_resources_tags2`= '".$or_resources_tags2."' WHERE `scid` = ".$scid;


    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = $scid;
	}

   ////$conn->disconnect();
    return $id;

}



static function structure_attr_comments_update($saCid,$sid,$aid,$level_id,$min_rate,$max_rate,$resources,$resources_tags1,$or_resources_tags2){

    $conn = parent::db_conn();
	$resources = htmlspecialchars(addslashes(htmlspecialchars_decode($resources)));

    $sql = "UPDATE `attribute_exam_structure_comment` set `sid` = ".$sid.",`level_id`= ".$level_id.", `min_rate`= ".$min_rate.", `max_rate`= ".$max_rate.", `resources`= '".$resources."', `resources_tags1`= '".$resources_tags1."', `or_resources_tags2`= '".$or_resources_tags2."' WHERE `saCid` = ".$saCid.' AND `aid`='.$aid;


    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = $scid;
	}

   ////$conn->disconnect();
    return $id;

}



/**
 *  考试（四级、六级等）的插入
 *
 * $name 考试名称
 * $description  考试描述
 * 返回插入的记录数
 */

static function exam_type_update($name,$description){
	$conn = parent::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	$sql = "UPDATE INTO `exam_type` (`tid`,`name`,`description`)
                SELECT '','$name','$description'
                FROM `exam_type`
                WHERE
                NOT EXISTS (SELECT 1 FROM `exam_type` WHERE `name`='".$name."') LIMIT 1
                 ";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_update_id();
	}

   ////$conn->disconnect();
    return $id;
  }


/**
 *  题型（阅读理解、完形填空等）的插入
 *
 * $name 题型名称
 * $description  题型描述
 * 返回插入的记录数
 */

static function item_type_update($name,$description){
	$conn = parent::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	$sql = "UPDATE INTO `item_type` (`type_id`,`tid`,`name`,`description`)
                SELECT '',1,'$name','$description'
                FROM `item_type`
                WHERE
                NOT EXISTS (SELECT 1 FROM `item_type` WHERE `name`='".$name."') LIMIT 1
                 ";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_update_id();
	}

   ////$conn->disconnect();
    return $id;
  }




  /**
   * 根据模板生成试卷基本信息
   *
   * @param unknown_type $sid
   * @return unknown
   */


  static function exam_info_update($sid,$uid){

	$arr = EM_STRUCTURE::query_structure_info($sid);

	$conn = parent::db_conn();

	if($sid>0){


		if(is_array($arr)){



		   $query = "SELECT COUNT(*) num FROM `exam_info` WHERE `paper_name`='".$arr['title']."'";

		   $res = $conn->query($query);
		   $row = $res->fetchRow();
	       $arr['description'] = htmlspecialchars(addslashes(htmlspecialchars_decode($arr['description'])));

		   if($row->num>0){

			  $paper_name = $arr['title']."-".date("YmdHi",time())."-".($row->num+1);  //试题的重命名 函数
			  $sql = "UPDATE INTO  `exam_info` (`paper_id`, `paper_name`, `description`, `tid`, `sid`,`points`,`objective_points`, `time_limit`, `created`, `uid`, `status`)
                        SELECT '','".$paper_name."','".$arr['description']."',".$arr['tid'].",".$sid.",".$arr['points'].",0,".$arr['time_limit'].",".time().",".$uid.",1
                        FROM `exam_info`
                        WHERE NOT EXISTS (SELECT 1 FROM `exam_info` WHERE `paper_name`='".$paper_name."' AND `tid`=".$arr['tid']." AND `sid`=".$sid.") LIMIT 1";


			}else{

			 $sql = "UPDATE INTO  `exam_info` (`paper_id`, `paper_name`, `description`, `tid`, `sid`, `points`, `objective_points`,`time_limit`, `created`, `uid`,  `status`)
                      VALUES ('','".$arr['title']."','".$arr['description']."',".$arr['tid'].",".$sid.",".$arr['points'].",0,".$arr['time_limit'].",".time().",".$uid.",1)";

			}


		}
	}

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_update_id();
	}

   ////$conn->disconnect();
	return  $id;

  }




 /**
  * 更新试卷的客观题得分
  *
  * @param unknown_type $paper_id
  * @param unknown_type $objective_points
  */
 function update_exam_objective_points($paper_id,$objective_points){

 	$conn = parent::db_conn();
    $query = "UPDATE `exam_info` SET `objective_points`=".$objective_points." WHERE `paper_id`=".$paper_id." LIMIT 1";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = $conn->affectedRows();
	}

    //$conn->disconnect();
    return  $id;

 }


 function update_items_by_arr_info($arr,$paper_id,$structure_id){

 	$str_id = array();
 	$sid_arr = array();

	  if(is_array($arr)){

		  foreach($arr as $key=>$value){


		  	 if($key>0){
		  	 	if($value['parent']==0){

		  	 	  $sid_arr[$structure_id] = self::update_exam_item_by_item($paper_id,0,$key,$value);
			  	  if(!in_array($structure_id,$str_id))
		  	 		        $str_id[] = $structure_id;

		  	 	}elseif(in_array($value['parent'],$str_id)){

		  	 		$sid_arr[$structure_id] = self::update_exam_item_by_item($paper_id,$sid_arr[$value['parent']],$key,$value);
			  	 	if(!in_array($structure_id,$str_id))
		  	 		        $str_id[] = $structure_id;

		  	 	 	}
			 }
		  }

	  }



 	return 	$sid_arr;

 }



 /**
  * 生成试卷的单道试题
  *
  *  $paper_id 试卷的id
  *  $parent_id   试题的父级题id
  *  $item_id   试题的id
  *  $value   试题信息的数组
  *  @return 试卷中试题的编号Id
  */

static function  update_exam_item_by_item($paper_id,$parent_id,$item_id,$item_info){
     $conn = parent::db_conn();
     $query = "UPDATE INTO `exam_item`(`exam_item_id`, `paper_id`, `parent_id`, `item_id`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`, `status`)
               VALUES ('',".$paper_id.",".$parent_id.",".$item_id.",".$item_info['points'].",".$item_info['time_limit'].",'".$item_info['serial_str']."','".$item_info['serial_num']."',".$item_info['is_submit'].",1)";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_update_id();
	}

     //$conn->disconnect();
     return  $id;

 }


/**
 * 用户答题信息保存
 *
 * @param unknown_type $paper_id
 * @param unknown_type $parent_id
 * @param unknown_type $item_id
 * @param unknown_type $item_info
 * @return unknown
 */



static function  user_result_update($paper_id,$user_id,$result_info){
     $conn = parent::db_conn();
     $time_used = $result_info['time_used']>0?$result_info['time_used']:0;
     $score = $result_info['score']>0?$result_info['score']:0;

     $query = "UPDATE INTO `user_results` (`result_id`, `paper_id`, `uid`, `time_used`, `time_end`, `score`)
               VALUES('',".$paper_id.",".$user_id.",".$time_used.",".time().",".$score.")";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_update_id();
	}

     //$conn->disconnect();
     return  $id;

}


/**
 * 选择题答案插入
 *
 * @param unknown_type $item_id
 * @param unknown_type $result_id
 * @param unknown_type $multi_answer_id  如果是多选则是数组
 * @param unknown_type $score
 * @return unknown
 */


static function  multichoice_user_answer_update($item_id,$result_id,$multi_answer_id,$score){
     $conn = parent::db_conn();
     $query = "UPDATE INTO  `multichoice_user_answer`( `user_answer_id`, `item_id`, `result_id`, `multi_answer_id`, `points_awarded` )";

     if(is_array($multi_answer_id)){

     	$num = count($multi_answer_id);
     	$i = 1;
     	foreach ($multi_answer_id as $value){

     		$query .= " VALUES('',".$item_id.",".$result_id.",".$value.",".$score.")";
        	if($i<$num){
        	    $query .= ",";
        	    $i++;
        	}
     	}

     }else{

     	$query .= " VALUES('',".$item_id.",".$result_id.",".$multi_answer_id.",".$score.")";

     }

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_update_id();
	}

     //$conn->disconnect();
     return  $id;

}


 /**
  * 填空题插入
  *
  * @param unknown_type $item_id
  * @param unknown_type $result_id
  * @param unknown_type $user_answer
  * @param unknown_type $score
  * @return unknown
  */


static function  blankfilling_user_answer_update($item_id,$result_id,$user_answer,$score){
     $conn = parent::db_conn();
     $user_answer = htmlspecialchars(addslashes(htmlspecialchars_decode(stripslashes($user_answer))));
     $query = "UPDATE INTO  `blank_filling_user_answer`( `answer_id`, `item_id`, `result_id`, `user_answer`, `points_awarded` )
               VALUES('',".$item_id.",".$result_id.",'".$user_answer."',".$score.")";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_update_id();
	}

     //$conn->disconnect();
     return  $id;

}


/**
 * 用户各部分得分
 *
 * @param unknown_type $id
 * @param 数组 $array
 * @return unknown
 */

static function user_result_detail_points_update($id,$array){

	 $conn = parent::db_conn();
	 if(is_array($array)){

	 	$num = count($array);
	 	$query = "UPDATE INTO 	`user_result_detail_points` (`detail_points_id`, `result_id`, `item_id`, `score`) VALUES ";
	 	$i = 1;
	 	foreach($array as $key => $value){

	 	   $key = $key>0?$key:0;
	 	   $value = $value>0?$value:0;

	 		if($i<$num){
	 		    $query .="('',".$id.",".$key.",".$value."),";
	 		}else{
	 			$query .="('',".$id.",".$key.",".$value.")";
	 		}
		    $i++;
	 	}


		$res = $conn->query($query);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
	 	    $row = $conn->affectedRows();
		}

	 }

     //$conn->disconnect();
     return  $row;

}





}
?>