<?php

require_once 'mysql.inc';

class DELETE extends DB_CONNECT {



/**
 *  试卷结构基本信息插入
 *
 * $array 数组
 *
 * 返回插入的id号
 */

static function exam_structure_info_delete($array){

	$points = isset($array['exam_score'])?(int)$array['exam_score']:0;
	$title = !empty($array['exam_title'])?$array['exam_title']:'';
	$description = !empty($array['exam_description'])?$array['exam_description']:'';
	$time_limit =  isset($array['exam_limittime'])?(int)$array['exam_limittime']:0;
    $tid = isset($array['tid'])?(int)$array['tid']:1;
    $time = time();
    $uid = 1;
    $status = 1 ;
	$conn = parent::db_conn();


	$sql = "DELETE INTO `exam_structure_info` (`sid`, `tid`, `uid`, `title`, `description`, `points`,`time_limit`, `created`, `status`)
            SELECT '',".$tid.",".$uid.",'".$title."','".htmlspecialchars(addslashes(htmlspecialchars_decode($description)))."',".$points.",".$time_limit.",".$time.",".$status."
            FROM `exam_structure_info`
	        WHERE
            NOT EXISTS (SELECT 1 FROM `exam_structure_info` WHERE `title` = '".$title."') LIMIT 1";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id =  mysql_delete_id();
	}
     //$conn->disconnect();
     return  $id;

  }


 static function exam_structure_delete($array){

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



	$sql = "DELETE INTO `exam_structure` (`structure_id`,`sid`, `term_id`,`basic_type`,`item_type`, `parent`,`check_relation`, `total`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`)
	        VALUES ('',".$sid.",'".$term_id."',".$basic_type.",".$item_type.",".$parent.",".$check_relation.",".$total.",".$points.",".$time_limit.",'".$serial_str."','".$serial_num."',".$is_submit.")";


    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id =  mysql_delete_id();
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


static function item_info_delete($arr){

    $conn = parent::db_conn();
    $uid = $arr['uid']>0?$arr['uid']:1;
	$sql = "DELETE INTO
	       `item_info`(`item_id`,`name`,`type`,`basic_type`, `body`, `description`, `points`, `time_limit`, `created`, `file_id`, `uid`, `status`,`is_random`)
           VALUES('','".htmlspecialchars(addslashes($arr['name']))."',".(int)$arr['type'].",".(int)$arr['basic_type'].",'".htmlspecialchars(addslashes(htmlspecialchars_decode(stripslashes($arr['body']))))."','".htmlspecialchars(addslashes(htmlspecialchars_decode($arr['description'])))."',".(int)$arr['point'].",".(int)$arr['time_limit'].",".time().",".$arr['file_id'].",".$uid.",1,".$arr['is_random'].")";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $item_id =  mysql_delete_id(); //获得试题的Id
	}

    //$conn->disconnect();
    return $item_id;

}



/**
 * 导入试卷时，插入试题对应的本试卷的流水号
 *
 * 输入试题id，流水号，excel中的序号，状态值（完全插入成功，状态为1，默认为0）
 * 返回影响的行数值
 *
 */
static function item_excel_liushui_delete($item_id,$excel_liushui,$excel_id,$status=1){

    $conn = parent::db_conn();
	$sql = "DELETE `item_excel_liushui` SET `status` = ".$status." AND `excel_id` = ".$excel_id."
	       WHERE `item_id`=".$item_id." AND `excel_liushui`='".$excel_liushui."' LIMIT 1";

    $res = $conn->query($sql);
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
 * 插入试题父子关系
 *
 * 输入数组
 * 返回插入试题的id值
 *
 */


static function item_parent_delete($item_id){

    $conn = parent::db_conn();
    $status = true;
	$sql = "DELETE FROM	 `item_hierarchy`
			WHERE `item_id`=".$item_id;

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
		$status = false;

	}else{
 	   $status = true;
	}
    //$conn->disconnect();
    return $status;

}





/**
 * 插入选择题选项
 *
 *
 */


static function multichoice_answers_delete($item_id){
    $conn = parent::db_conn();
    $status = true;
    $query = "DELETE FROM `multichoice_answers` WHERE  `item_id`= ".$item_id;

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
		$status = false;
	}else{
 	    $status = true;
	}

    //$conn->disconnect();
    return $status;
 }


 /**
 * 插入选择题设置信息
 *
 *
 */


 static function multichoice_properties_delete($item_id,$choice_multi,$choice_random){

    $conn = parent::db_conn();

    $query = "DELETE INTO `multichoice_properties` (`item_id`, `choice_multi`, `choice_random`)
              SELECT ".$item_id.",".$choice_multi.",".$choice_random."
              FROM `multichoice_properties`
              WHERE NOT EXISTS (SELECT 1 FROM `multichoice_properties` WHERE `item_id`=".$item_id.") LIMIT 1";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = $conn->affectedRows();
	}
    //$conn->disconnect();
    return $id;
 }







 /**
 * 插入填空答案
 *
 *
 */


static function blank_filling_answers_delete($item_id){

	$conn = parent::db_conn();
	$status = true;

    $query = "DELETE FROM `blank_filling_answers` WHERE `item_id` = ".$item_id;

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
		$status = FALSE;
	}else{
 	    $status = true;
	}

    //$conn->disconnect();
    return $status;
 }

static function blank_filling_properties($item_id,$blank_size){

	$conn = parent::db_conn();

    $query = "DELETE INTO `blank_filling_properties` (`item_id`, `blank_size`)
              VALUES (".$item_id.",".$blank_size.")";
    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = 1;
	}

    //$conn->disconnect();
    return $id;
 }




/**
 * 插入标签，以,为分隔符
 * 输入可以是数组也可以是字符串
 * 返回term_id数组
 *
 */


static function tags_delete_and_get_id($array){

    $conn = parent::db_conn();
    if(is_array($array)){
	    $tags = $array['tags'];
	    $description = $array['description'];
	    $weight = $array['weight'];
    }else
        $tags = $array;

    $tags_arr = array();
    if(strpos($tags,","))
    	$tags_arr = explode(",",$tags);
    else
        $tags_arr[0] = $tags;

	foreach($tags_arr as $tag){
	    if (trim($tag) != "") {
	    	$sql = "SELECT * FROM `term_data` WHERE `name`='".$tag."' LIMIT 1";
	    	$rs = $conn->query($sql);

	        $row = @$rs->fetchRow();

	        if($row->tid<1){
	             $id = self::single_tag_delete($tag,$description,$weight);
	             if($id>0)
	                  $term_id[] = $id ;
	        }else{
	  	          $term_id[] =  $row->tid;
	        }
	     }
	  }

	//$conn->disconnect();
    return $term_id;
 }



 static function single_tag_delete($tag,$description,$weight){

 	 $conn = parent::db_conn();
 	 $query = "DELETE INTO `term_data`(`tid`, `name`,`description`,`weight`)
 	           SELECT '','".$tag."','".htmlspecialchars(addslashes(htmlspecialchars_decode($description)))."','".$weight."'
 	           FROM `term_data`
 	           WHERE
 	           NOT EXISTS (SELECT 1 FROM `term_data` WHERE `name`='".$tag."') LIMIT 1";

 	$res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = mysql_delete_id();
	}

     //$conn->disconnect();
     return  $id;


 }






//插入标签和试题的关系
 static function tags_item_realtionship_delete($item_id){
    $conn = parent::db_conn();

  	$status = true;

  	$sql = "SELECT COUNT(*) num FROM `term_item`  WHERE  `item_id`=".$item_id;
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();
 	if($row->num>0){
	    $query = "DELETE FROM `term_item` WHERE
	     	   `item_id`=".$item_id;

		$res = $conn->query($query);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
			 $status = false;
		}else{
			 $status = true;
		}

	}

	//$conn->disconnect();

    return $status;

 }


/**
 * 插入考点，以,为分隔符
 * 输入可以是数组也可以是字符串
 * 返回aid数组
 *
 */


static function attributes_delete_and_get_id($input){

    $conn = parent::db_conn();
    if(is_array($input)){
	    $attributes = $input['attribute'];
	    $description = $input['description'];
	    $weight = $input['weight'];
    }else
        $attributes = $input;

    $attributes_arr = array();
    if(strpos($attributes,","))
    	$attributes_arr = explode(",",$input);
    else
        $attributes_arr[0] = $attributes;


	foreach($attributes_arr as $attribute){
	    if (trim($attribute) != "") {
	    	$sql = "SELECT * FROM `item_attribute_detail` WHERE `name`='".$attribute."' LIMIT 1";

	    	$rs = $conn->query($sql);

	        $row = @$rs->fetchRow();
	        if($row->aid<1){
	             $id = self::single_attribute_delete($attribute,$description,0);
	             if($id>0)
	                  $attribute_id[] = $id ;
	        }else{
	  	          $attribute_id[] =  $row->aid;
	        }
	     }
	  }

	//$conn->disconnect();
    return $attribute_id;
 }



 static function single_attribute_delete($attribute,$description,$weight){

 	 $conn = parent::db_conn();

 	 $sql = "SELECT COUNT(*) num FROM `item_attribute_detail`";
 	 $rs = $conn->query($sql);
 	 $row = @$rs->fetchRow();
 	 $description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
 	 if($row->num>1){
	 	 $query = "DELETE INTO `item_attribute_detail`(`aid`, `name`,`description`,`weight`)
	 	           SELECT '','".$attribute."','".$description."','".$weight."'
	 	           FROM `term_data`
	 	           WHERE
	 	           NOT EXISTS (SELECT 1 FROM `item_attribute_detail` WHERE `name`='".$attribute."') LIMIT 1";
 	 }else{
 	 	 $query = "DELETE INTO `item_attribute_detail`(`aid`, `name`,`description`,`weight`)
	 	           VALUES ('','".$attribute."','".$description."','".$weight."')";
 	 }
 	$res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = mysql_delete_id();
	}

     //$conn->disconnect();
     return  $id;


 }


//插入考点和试题的关系
 static function attributes_item_realtionship_delete($item_id){
    $conn = parent::db_conn();
    $status = true;

  	$sql = "SELECT COUNT(*) num FROM `item_attribute_hierarchy`  WHERE  `item_id`=".$item_id;
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();
 	if($row->num>0){
	    $query = "DELETE FROM `item_attribute_hierarchy` WHERE
	     	   `item_id`=".$item_id;

		$res = $conn->query($query);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
			 $status = false;
		}else{
			 $status = true;
		}

	}

	//$conn->disconnect();

    return $status;
 }


 /**
  * 题目材料解析的插入
  *
  * @param unknown_type $term_id
  * @param unknown_type $item_id
  * @return unknown
  */

 static function material_items_feedback_delete($item_id){
 	$conn = parent::db_conn();
 	$status = true;
  	$sql = "DELETE FROM `material_items_feedback` WHERE `item_id` = ".$item_id;

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
		$status = FALSE;
	}else{
 	     $status = true;
	}

    //$conn->disconnect();
    return $status;


 }



 /**
  * 附件的插入
  *
  * @param unknown_type $term_id
  * @param unknown_type $item_id
  * @return unknown
  */

 static function files_delete($type,$info){
 	$conn = parent::db_conn();
 	$uid = $info['uid']>0?$info['uid']:1;
 	$info['filename'] = htmlspecialchars(addslashes($info['filename']));
 	$info['fileurl'] = addslashes(trim($info['fileurl']));
 	if($info['fileurl']!=''){
  	   $sql = "SELECT fid FROM `files` WHERE `fileurl`='".$info['fileurl']."' LIMIT 1";
       $rs = $conn->query($sql);
 	   $row = @$rs->fetchRow();
 	}

 	if($row->fid<1){
	   $query = "DELETE INTO `files` (`fid`, `type`, `uid`, `filename`, `fileurl`, `filepath`, `filemime`, `filesize`, `status`, `created`)
		 		    VALUES('','".$type."',".$uid.",'".$info['filename']."','".$info['fileurl']."','".$info['filepath']."','".$info['filemime']."','".$info['filesize']."',1,".time().")";
       $res = $conn->query($query);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
	 	     $id = mysql_delete_id();
		}
 	 }else{
	 	$id = $row->fid;
	 }

    //$conn->disconnect();
    return $id;


 }



//插入试题之间的关系
 static function item_hierarchy_delete($term_id,$item_id){

    $conn = parent::db_conn();
    $sql = "DELETE INTO `term_hierarchy`(`term_id`,`parent`)
     	    SELECT ".$term_id.",".$item_id."
     	    FROM `term_hierarchy`
     	    WHERE
     	    NOT EXISTS (SELECT 1 FROM `term_hierarchy` WHERE `term_id`=".$term_id." AND `parent`=".$item_id.") LIMIT 1";

    $res = $conn->query($sql);
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
 *  基本题型（选择、填空）的插入
 *
 * $name 题型名称
 * $description  题型描述
 * 返回插入的记录数
 */
static function basic_item_type_delete($name,$description){

		$conn = parent::db_conn();
		$name = htmlspecialchars(addslashes($name));;
		$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
		$sql = "DELETE INTO `basic_item_type` (`basic_type_id`,`name`,`description`)
                SELECT '','$name','".$description."'
                FROM `basic_item_type`
                WHERE
                NOT EXISTS (SELECT 1 FROM `basic_item_type` WHERE `name`='".$name."') LIMIT 1";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_delete_id();
	}

    //$conn->disconnect();
    return $id;

}



static function exam_comments_delete($name,$description,$weight){

	$level_id = self::exam_comments_level_delete($name);
	$weight = $weight>0?$weight:0;
	$conn = parent::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	if($level_id>0){
		$sql = "DELETE INTO `exam_comments` (`cid`,`level_id`, `description`, `weight`) VALUES ('',".$level_id.",'".$description."',".$weight.")";
	    $res = $conn->query($sql);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
	 	    $id = mysql_delete_id();
		}
	}
    //$conn->disconnect();
    return $id;

}


static function exam_comments_level_delete($name){

	$conn = parent::db_conn();
   	$sql = "SELECT `level_id` FROM `exam_comments_level` WHERE `level_name`='".$name."' LIMIT 1";
   	$rs = $conn->query($sql);
    $row = @$rs->fetchRow();
    if($row->level_id<1){
    	$query = "DELETE INTO `exam_comments_level`(`level_name`) VALUES ('".$name."')";
	    $res = $conn->query($query);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
	 	    $id = mysql_delete_id();
		}
    }else{
    	 $id = $row->level_id;
    }

    //$conn->disconnect();
    return $id;

}

static function structure_comments_delete($scid,$sid,$level_id,$min_rate,$max_rate,$resources){

    $conn = parent::db_conn();
	$resources = htmlspecialchars(addslashes(htmlspecialchars_decode($resources)));

    $sql = "DELETE `exam_structure_comment` set `sid` = ".$sid.",`level_id`= ".$level_id.", `min_rate`= ".$min_rate.", `max_rate`= ".$max_rate.", `resources`= '".$resources."' WHERE `scid` = ".$scid;

    file_put_contents(aaa.txt, 'sql:::::\r\n'.$sql.'\r\n\r\n', FILE_APPEND);

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = $scid;
	}

    //$conn->disconnect();
    return $id;

}





/**
 *  考试（四级、六级等）的插入
 *
 * $name 考试名称
 * $description  考试描述
 * 返回插入的记录数
 */

static function exam_type_delete($name,$description){
	$conn = parent::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	$sql = "DELETE INTO `exam_type` (`tid`,`name`,`description`)
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
 	     $id = mysql_delete_id();
	}

    //$conn->disconnect();
    return $id;
  }


/**
 *  题型（阅读理解、完形填空等）的插入
 *
 * $name 题型名称
 * $description  题型描述
 * 返回插入的记录数
 */

static function item_type_delete($name,$description){
	$conn = parent::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	$sql = "DELETE INTO `item_type` (`type_id`,`tid`,`name`,`description`)
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
 	     $id = mysql_delete_id();
	}

    //$conn->disconnect();
    return $id;
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

static function  delete_exam_item_by_item($paper_id,$parent_id,$item_id,$item_info){
     $conn = parent::db_conn();
     $query = "DELETE INTO `exam_item`(`exam_item_id`, `paper_id`, `parent_id`, `item_id`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`, `status`)
               VALUES ('',".$paper_id.",".$parent_id.",".$item_id.",".$item_info['points'].",".$item_info['time_limit'].",'".$item_info['serial_str']."','".$item_info['serial_num']."',".$item_info['is_submit'].",1)";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_delete_id();
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



static function  user_result_delete($paper_id,$user_id,$result_info){
     $conn = parent::db_conn();
     $time_used = $result_info['time_used']>0?$result_info['time_used']:0;
     $score = $result_info['score']>0?$result_info['score']:0;

     $query = "DELETE INTO `user_results` (`result_id`, `paper_id`, `uid`, `time_used`, `time_end`, `score`)
               VALUES('',".$paper_id.",".$user_id.",".$time_used.",".time().",".$score.")";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_delete_id();
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


static function  multichoice_user_answer_delete($item_id,$result_id,$multi_answer_id,$score){
     $conn = parent::db_conn();
     $query = "DELETE INTO  `multichoice_user_answer`( `user_answer_id`, `item_id`, `result_id`, `multi_answer_id`, `points_awarded` )";

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
 	     $id = mysql_delete_id();
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


static function  blankfilling_user_answer_delete($item_id,$result_id,$user_answer,$score){
     $conn = parent::db_conn();
     $user_answer = htmlspecialchars(addslashes(htmlspecialchars_decode(stripslashes($user_answer))));
     $query = "DELETE INTO  `blank_filling_user_answer`( `answer_id`, `item_id`, `result_id`, `user_answer`, `points_awarded` )
               VALUES('',".$item_id.",".$result_id.",'".$user_answer."',".$score.")";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_delete_id();
	}

     //$conn->disconnect();
     return  $id;

}




/**
* 删除生成的试卷信息（试卷基本信息，试题信息，用户此卷子做题信息）
*
* @param unknown_type $paper_id
* @return unknown
*/

static function  delete_paper_by_paper_id($paper_id,$result){

	$arr['status'] = 0;

	$conn = parent::db_conn();

	 if($result['count'] != 0)
	 {
		 $result_id = $result['result_id'];

		$result_id = str_ireplace(","," or `result_id` = ",$result_id);

		$sql_1 = "DELETE FROM `exam_info` WHERE `paper_id` =  ".$paper_id;
		$sql_2 = "DELETE FROM `exam_item` WHERE `paper_id` =  ".$paper_id;
		$sql_3 = " DELETE FROM `user_results` WHERE `result_id` =  ".$result_id;
		$sql_4 = "DELETE FROM `user_result_detail_points` WHERE `result_id` =  ".$result_id;
		$sql_5 = "DELETE FROM `blank_filling_user_answer` WHERE `result_id` =  ".$result_id;
		$sql_6 = "DELETE FROM `multichoice_user_answer` WHERE `result_id` =  ".$result_id;

		$res_1 = $conn-> query($sql_1);
    	$res_2 = $conn-> query($sql_2);
    	$res_3 = $conn-> query($sql_3);
    	$res_4 = $conn-> query($sql_4);
    	$res_5 = $conn-> query($sql_5);
    	$res_6 = $conn-> query($sql_6);

		if(DB::isError ($res_1) or DB::isError ($res_2) or DB::isError ($res_3) or DB::isError ($res_4) or DB::isError ($res_5) or DB::isError ($res_6)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$sql_1.$sql_2.$sql_3.$sql_4.$sql_5.$sql_6);
		 }else{
				$arr['status'] = 1;
		 }
   	}
   	else{
   		//$sql = "DELETE FROM `exam_info` WHERE `paper_id` =  ".$paper_id."; DELETE FROM `exam_item` WHERE `paper_id` =  ".$paper_id;
   		$sql_1 = "DELETE FROM `exam_info` WHERE `paper_id` =  ".$paper_id;
   		$sql_2 = "DELETE FROM `exam_item` WHERE `paper_id` =  ".$paper_id;
   		$res_1 = $conn-> query($sql_1);
    	$res_2 = $conn-> query($sql_2);

		if(DB::isError ($res_1) or DB::isError ($res_2)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$sql_1.$sql_2);
		 }else{
				$arr['status'] = 1;
		 }
   	}


     //$conn->disconnect();

     return $arr;

 }


/**
* 删除此试题的所有信息（试卷基本信息，试题信息，用户此卷子做题信息）
*
* @param  $item_id
* @return True or False
*/
static function  delete_allcontent_by_item_id($item_id){

	$conn = parent::db_conn();


	$sql_1 = "DELETE FROM `item_info` WHERE `item_id` =  ".$item_id;
	$sql_2 = "DELETE FROM `item_attribute_hierarchy` WHERE `item_id` =  ".$item_id;
	$sql_3 = "DELETE FROM `item_hierarchy` WHERE `item_id` =  ".$item_id;
	$sql_4 = " DELETE FROM `term_item` WHERE `item_id` =  ".$item_id;

	$sql_5 = "DELETE FROM `blank_filling_answers` WHERE `item_id` =  ".$item_id;
	$sql_6 = "DELETE FROM `blank_filling_properties` WHERE `item_id` =  ".$item_id;
	$sql_7 = " DELETE FROM `blank_filling_user_answer` WHERE `item_id` =  ".$item_id;
	$sql_8 = "DELETE FROM `material_items_feedback` WHERE `item_id` =  ".$item_id;
	$sql_9 = "DELETE FROM `multichoice_answers` WHERE `item_id` =  ".$item_id;
	$sql_10 = " DELETE FROM `multichoice_properties` WHERE `item_id` =  ".$item_id;
	$sql_11 = "DELETE FROM `multichoice_user_answer` WHERE `item_id` =  ".$item_id;

	$sql_12 = "DELETE FROM `exam_item` WHERE `item_id` =  ".$item_id;
	$sql_13 = "DELETE FROM `user_result_detail_points` WHERE `item_id` =  ".$item_id;
	$sql_14 = "DELETE FROM `item_excel_liushui` WHERE `item_id` =  ".$item_id;

	$res_1 = $conn-> query($sql_1);
	$res_2 = $conn-> query($sql_2);
	$res_3 = $conn-> query($sql_3);
	$res_4 = $conn-> query($sql_4);
	$res_5 = $conn-> query($sql_5);
	$res_6 = $conn-> query($sql_6);
	$res_7 = $conn-> query($sql_7);
	$res_8 = $conn-> query($sql_8);
	$res_9 = $conn-> query($sql_9);
	$res_10 = $conn-> query($sql_10);
	$res_11 = $conn-> query($sql_11);
	$res_12 = $conn-> query($sql_12);
	$res_13 = $conn-> query($sql_13);
	$res_14 = $conn-> query($sql_14);



	if(DB::isError ($res_1) or DB::isError ($res_2) or DB::isError ($res_3) or DB::isError ($res_4) or DB::isError ($res_5) or DB::isError ($res_6) or DB::isError ($res_7) or DB::isError ($res_8) or DB::isError ($res_9) or DB::isError ($res_10) or DB::isError ($res_11) or DB::isError ($res_12) or DB::isError ($res_13) or DB::isError ($res_14)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql_1.$sql_2.$sql_3.$sql_4.$sql_5.$sql_6.$sql_7.$sql_8.$sql_9.$sql_10.$sql_11.$sql_12.$sql_13.$sql_14);

			$status = false;
	 }else{

			$status = TRUE;
	 }

     //$conn->disconnect();

     return $status;

 }


static function  delete_items_by_liushui($id){

	$conn = parent::db_conn();

	$status = TRUE;

    if(!empty($id)){

    	$sql = "SELECT `item_id` FROM `item_excel_liushui`  WHERE `excel_liushui` = '".$id."'";

		$res = $conn->query($sql);

		if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
		}else{
			while($row = $res->fetchRow()){
				 $delete_status = self::delete_allcontent_by_item_id($row->item_id);

				 if(!$delete_status){
				 	$status = false;
				 	break;
				 }

			}
		}


	 }else{

			$status = FALSE;
	 }

     //$conn->disconnect();

     return $status;

}


}
?>