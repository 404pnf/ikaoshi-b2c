<?php

require_once 'mysql.inc';

class INSERT {



/**
 *  试卷结构基本信息插入
 *
 * $array 数组
 *
 * 返回插入的id号
 */

static function exam_structure_info_insert($array){

	$points = isset($array['exam_score'])?(int)$array['exam_score']:0;
	$title = !empty($array['exam_title'])?$array['exam_title']:'';
	$description = !empty($array['exam_description'])?$array['exam_description']:'';
	$time_limit =  isset($array['exam_limittime'])?(int)$array['exam_limittime']:0;
    $tid = isset($array['tid'])?(int)$array['tid']:1;
    $time = time();
    $uid = 1;
    $status = 1 ;
	$conn = DB_CONNECT::db_conn();


	$sql = "INSERT INTO `exam_structure_info` ( `tid`, `uid`, `title`, `description`, `points`,`time_limit`, `created`, `status`)
            SELECT ".$tid.",".$uid.",'".$title."','".htmlspecialchars(addslashes(htmlspecialchars_decode($description)))."',".$points.",".$time_limit.",".$time.",".$status."
            FROM `exam_structure_info`
	        WHERE
            NOT EXISTS (SELECT 1 FROM `exam_structure_info` WHERE `title` = '".$title."') LIMIT 1";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id =  mysql_insert_id();
	}
     // 	 $conn->disconnect();
     return  $id;

  }


 static function exam_structure_insert($array){

 	$conn = DB_CONNECT::db_conn();
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



	$sql = "INSERT INTO `exam_structure` (`sid`, `term_id`,`basic_type`,`item_type`, `parent`,`check_relation`, `total`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`)
	        VALUES (".$sid.",'".$term_id."',".$basic_type.",".$item_type.",".$parent.",".$check_relation.",".$total.",".$points.",".$time_limit.",'".$serial_str."','".$serial_num."',".$is_submit.")";


    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id =  mysql_insert_id();
	}
    // 	 $conn->disconnect();
    return $id;

 }
/**
 * 插入试题信息
 *
 * 输入数组
 * 返回插入试题的id值
 *
 */


static function item_info_insert($arr){

    $conn = DB_CONNECT::db_conn();
    $uid = $arr['uid']>0?$arr['uid']:1;
	$sql = "INSERT INTO
	       `item_info`(`name`,`type`,`basic_type`, `body`, `description`, `points`, `time_limit`, `created`, `file_id`, `uid`, `status`,`is_random`)
           VALUES('".htmlspecialchars(addslashes($arr['name']))."',".(int)$arr['type'].",".(int)$arr['basic_type'].",'".htmlspecialchars(addslashes(htmlspecialchars_decode(stripslashes($arr['body']))))."','".htmlspecialchars(addslashes(htmlspecialchars_decode($arr['description'])))."',".(float)$arr['point'].",".(int)$arr['time_limit'].",".time().",".$arr['file_id'].",".$uid.",1,".$arr['is_random'].")";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $item_id =  mysql_insert_id(); //获得试题的Id
	}

    // 	 $conn->disconnect();
    return $item_id;

}




/**
 * 导入试卷时，插入试题对应的本试卷的流水号
 *
 * 输入试题id，流水号，excel中的序号，状态值（完全插入成功，状态为1，默认为0）
 * 返回影响的行数值
 *
 */
static function item_excel_liushui_insert($item_id,$excel_liushui,$excel_id,$status=0){

    $conn = DB_CONNECT::db_conn();
	$sql = "INSERT INTO	 `item_excel_liushui` (`item_id`, `excel_liushui`, `excel_id`, `status`)
	        VALUES( ?,?,?,?)";


    $res = $conn->query($sql,array($item_id, $excel_liushui, $excel_id, $status));
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = $conn->affectedRows();
	}
    // 	 $conn->disconnect();
    return $id;

}







/**
 * 插入试题父子关系
 *
 * 输入数组
 * 返回插入试题的id值
 *
 */


static function item_parent_insert($item_id,$pid,$need_parent,$weight=0){

    $conn = DB_CONNECT::db_conn();
	$sql = "INSERT INTO	 `item_hierarchy` (`item_id`, `parent`, `need_parent`, `weight`)
	        SELECT ".$item_id.",".$pid.",".$need_parent.",".$weight."
	        FROM `item_hierarchy`
	        WHERE NOT EXISTS (SELECT 1 FROM `item_hierarchy` WHERE `item_id`=".$item_id." AND `parent`=".$pid.") LIMIT 1";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = $conn->affectedRows();
	}
    // 	 $conn->disconnect();
    return $id;

}





/**
 * 插入选择题选项
 *
 *
 */


static function multichoice_answers($item_id,$answer){
    $conn = DB_CONNECT::db_conn();
    $query = "INSERT INTO `multichoice_answers`( `item_id`, `answer`, `feedback`, `is_correct`) VALUES ";

    foreach($answer as $value){
	    if (trim($value['answer']) != "") {
	    	if(isset($value['checkbox'])&&!isset($value['is_correct'])){
	    	   $is_correct =  $value['checkbox']=='on'?1:0;
	    	}else{
	    	   $is_correct =  $value['is_correct']>0?$value['is_correct']:0;
	    	}
            $query .="(".$item_id.",'".htmlspecialchars(addslashes(htmlspecialchars_decode($value['answer'])))."','".htmlspecialchars(addslashes(htmlspecialchars_decode($value['explain'])))."',".$is_correct."),";
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

    // 	 $conn->disconnect();
    return $id;
 }


 /**
 * 插入选择题设置信息
 *
 *
 */


 static function multichoice_properties_insert($item_id,$choice_multi,$choice_random){

    $conn = DB_CONNECT::db_conn();

    $query = "INSERT INTO `multichoice_properties` (`item_id`, `choice_multi`, `choice_random`)
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
    // 	 $conn->disconnect();
    return $id;
 }







 /**
 * 插入填空答案
 *
 *
 */


static function blank_filling_answers($item_id,$is_objective,$answer){

	$conn = DB_CONNECT::db_conn();

	$correct_answer = isset($answer['answer'])?htmlspecialchars(addslashes(htmlspecialchars_decode($answer['answer']))):'';
	$feedbak = isset($answer['explain'])?htmlspecialchars(addslashes(htmlspecialchars_decode($answer['explain']))):'';


    $query = "INSERT INTO `blank_filling_answers` (`item_id`, `correct_answer`, `feedback`, `is_objective`)
              SELECT ".$item_id.",'".$correct_answer."','".$feedbak."',".$is_objective."
              FROM `blank_filling_answers`
              WHERE NOT EXISTS (SELECT 1 FROM `blank_filling_answers` WHERE `item_id`=".$item_id.") LIMIT 1";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;
 }

static function blank_filling_properties($item_id,$blank_size){

	$conn = DB_CONNECT::db_conn();

    $query = "INSERT INTO `blank_filling_properties` (`item_id`, `blank_size`)
              VALUES (".$item_id.",".$blank_size.")";
    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = 1;
	}

    // 	 $conn->disconnect();
    return $id;
 }




/**
 * 插入标签，以,为分隔符
 * 输入可以是数组也可以是字符串
 * 返回term_id数组
 *
 */


static function tags_insert_and_get_id($array){

    $conn = DB_CONNECT::db_conn();
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
	    	//echo $sql."<br>";
	        $row = @$rs->fetchRow();

	        // file_put_contents(aaa.txt, 'tags_insert_and_get_id:$row::::<br>'. $row.'<br><br>', FILE_APPEND);

				//if($row->tid<1){
			if(empty($row)) {
	             $id = self::single_tag_insert($tag,$description,$weight);
	             if($id>0)
	                  $term_id[] = $id ;
	        }else{
	  	          $term_id[] =  $row->tid;
	        }
	     }
	  }

	// 	 $conn->disconnect();
    return $term_id;
 }



 static function single_tag_insert($tag,$description,$weight = 0){

 	 $conn = DB_CONNECT::db_conn();
 	 if(empty($description)) $description = '';
 	 if(empty($weight)) $weight = 0;
 	 $query = "INSERT INTO `term_data`(`name`,`description`,`weight`)
 	           SELECT '".$tag."','".htmlspecialchars(addslashes(htmlspecialchars_decode($description)))."',".$weight."
 	           FROM `term_data`
 	           WHERE
 	           NOT EXISTS (SELECT 1 FROM `term_data` WHERE `name`='".$tag."') LIMIT 1";

 	$res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = mysql_insert_id();
	}

     // 	 $conn->disconnect();
     return  $id;


 }






//插入标签和试题的关系
 static function tags_item_realtionship_insert($item_id,$term_id){
    $conn = DB_CONNECT::db_conn();

    $sql = "INSERT INTO `term_item`(`term_id`,`item_id`)
     	        SELECT ".$term_id.",".$item_id."
     	        FROM `term_item`
     	        WHERE
     	        NOT EXISTS (SELECT 1 FROM `term_item` WHERE `term_id`=".$term_id." AND `item_id`=".$item_id.") LIMIT 1";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = $conn->affectedRows();
	}

    // 	 $conn->disconnect();
    return $id;
 }


/**
 * 插入考点，以,为分隔符
 * 输入可以是数组也可以是字符串
 * 返回aid数组
 *
 */


static function attributes_insert_and_get_id($input){

    $conn = DB_CONNECT::db_conn();
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
	    	//echo $sql."<br>";
	        $row = @$rs->fetchRow();
	        if($row->aid<1){
	             $id = self::single_attribute_insert($attribute,$description,0);
	             if($id>0)
	                  $attribute_id[] = $id ;
	        }else{
	  	          $attribute_id[] =  $row->aid;
	        }
	     }
	  }

	// 	 $conn->disconnect();
    return $attribute_id;
 }



 static function single_attribute_insert($attribute,$description,$weight){

 	 $conn = DB_CONNECT::db_conn();

 	 $sql = "SELECT COUNT(*) num FROM `item_attribute_detail`";
 	 $rs = $conn->query($sql);
 	 $row = @$rs->fetchRow();
 	 $description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
 	 if($row->num>1){
	 	 $query = "INSERT INTO `item_attribute_detail`(`name`,`description`,`weight`)
	 	           SELECT '".$attribute."','".$description."','".$weight."'
	 	           FROM `term_data`
	 	           WHERE
	 	           NOT EXISTS (SELECT 1 FROM `item_attribute_detail` WHERE `name`='".$attribute."') LIMIT 1";
 	 }else{
 	 	 $query = "INSERT INTO `item_attribute_detail`( `name`,`description`,`weight`)
	 	           VALUES ('".$attribute."','".$description."','".$weight."')";
 	 }
 	$res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = mysql_insert_id();
	}

     // 	 $conn->disconnect();
     return  $id;


 }


//插入考点和试题的关系
 static function attributes_item_realtionship_insert($item_id,$aid){
    $conn = DB_CONNECT::db_conn();
  	$sql = "SELECT COUNT(*) num FROM `item_attribute_hierarchy`";
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();
 	if($row->num>1){
	    $query = "INSERT INTO `item_attribute_hierarchy`(`aid`,`item_id`)
	     	    SELECT ".$aid.",".$item_id."
	     	    FROM `item_attribute_hierarchy`
	     	    WHERE
	     	    NOT EXISTS (SELECT 1 FROM `item_attribute_hierarchy` WHERE `aid`=".$aid." AND `item_id`=".$item_id.") LIMIT 1";
	}else{
 		$query = "INSERT INTO `item_attribute_hierarchy`(`aid`,`item_id`)
	     	    VALUES (".$aid.",".$item_id.")";
 	}

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;
 }


 /**
  * 题目材料解析的插入
  *
  * @param unknown_type $term_id
  * @param unknown_type $item_id
  * @return unknown
  */

 static function material_items_feedback_insert($item_id,$feedback){
 	$conn = DB_CONNECT::db_conn();
  	$sql = "SELECT COUNT(*) num FROM `material_items_feedback`";
 	$rs = $conn->query($sql);
 	$row = @$rs->fetchRow();
 	$feedback = htmlspecialchars(addslashes(htmlspecialchars_decode($feedback)));
  	if($row->num>0){
	    $sql = "INSERT INTO `material_items_feedback`(`item_id`,`feedback`)
	     	    SELECT ".$item_id.",'$feedback'
	     	    FROM `item_attribute_hierarchy`
	     	    WHERE
	     	    NOT EXISTS (SELECT 1 FROM `material_items_feedback` WHERE `item_id`=".$item_id.") LIMIT 1";
	}else{
 		$sql = "INSERT INTO `material_items_feedback`(`item_id`,`feedback`)
	     	    VALUES (".$item_id.",'$feedback')";
 	}

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;


 }



 /**
  * 附件的插入
  *
  * @param unknown_type $term_id
  * @param unknown_type $item_id
  * @return unknown
  */

 static function files_insert($type,$info){
 	$conn = DB_CONNECT::db_conn();
 	$uid = $info['uid']>0?$info['uid']:1;
 	$info['filename'] = htmlspecialchars(addslashes($info['filename']));
 	$info['fileurl'] = addslashes(trim($info['fileurl']));
 	if($info['fileurl']!=''){
  	   $sql = "SELECT fid FROM `files` WHERE `fileurl`='".$info['fileurl']."' LIMIT 1";
       $rs = $conn->query($sql);
 	   $row = @$rs->fetchRow();
 	}

 	if($row->fid<1){
	   $query = "INSERT INTO `files` ( `type`, `uid`, `filename`, `fileurl`, `filepath`, `filemime`, `filesize`, `status`, `created`)
		 		    VALUES('".$type."',".$uid.",'".$info['filename']."','".$info['fileurl']."','".$info['filepath']."','".$info['filemime']."','".$info['filesize']."',1,".time().")";
       $res = $conn->query($query);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
	 	     $id = mysql_insert_id();
		}
 	 }else{
	 	$id = $row->fid;
	 }

    // 	 $conn->disconnect();
    return $id;


 }



//插入试题之间的关系
 static function item_hierarchy_insert($term_id,$item_id){

    $conn = DB_CONNECT::db_conn();
    $sql = "INSERT INTO `term_hierarchy`(`term_id`,`parent`)
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

    // 	 $conn->disconnect();
    return $id;
 }

/**
 *  基本题型（选择、填空）的插入
 *
 * $name 题型名称
 * $description  题型描述
 * 返回插入的记录数
 */
static function basic_item_type_insert($name,$description){

		$conn = DB_CONNECT::db_conn();
		$name = htmlspecialchars(addslashes($name));;
		$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
		$sql = "INSERT INTO `basic_item_type` (`name`,`description`)
                SELECT '$name','".$description."'
                FROM `basic_item_type`
                WHERE
                NOT EXISTS (SELECT 1 FROM `basic_item_type` WHERE `name`='".$name."') LIMIT 1";

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;

}



static function exam_comments_insert($name,$description,$weight){

	$level_id = self::exam_comments_level_insert($name);
	$weight = $weight>0?$weight:0;
	$conn = DB_CONNECT::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	if($level_id>0){
		$sql = "INSERT INTO `exam_comments` (`level_id`, `description`, `weight`) VALUES (".$level_id.",'".$description."',".$weight.")";
	    $res = $conn->query($sql);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
	 	    $id = mysql_insert_id();
		}
	}
    // 	 $conn->disconnect();
    return $id;

}
static function exam_comments_level_insert($name){

	$conn = DB_CONNECT::db_conn();
   	$sql = "SELECT `level_id` FROM `exam_comments_level` WHERE `level_name`='".$name."' LIMIT 1";
   	$rs = $conn->query($sql);
    $row = @$rs->fetchRow();
    if($row->level_id<1){
    	$query = "INSERT INTO `exam_comments_level`(`level_name`) VALUES ('".$name."')";
	    $res = $conn->query($query);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
	 	    $id = mysql_insert_id();
		}
    }else{
    	 $id = $row->level_id;
    }

    // 	 $conn->disconnect();
    return $id;

}


static function structure_comments_global_insert($exam_type_id,$level_id,$min_rate,$max_rate,$resources_tags1,$or_resources_tags2){

    $conn = DB_CONNECT::db_conn();

    $sql = "INSERT INTO `exam_structure_comment_global` (`exam_type_id`, `min_rate`, `max_rate`,`level_id`, `resources_tags1`,`or_resources_tags2`) VALUES (".$exam_type_id.",".$min_rate.",".$max_rate.",".$level_id.",'".$resources_tags1."','".$or_resources_tags2."')";

    //echo '<br>sql::'.$sql.'<br>';

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;

}

static function structure_comments_relate_insert($sid,$global_examType_detail_mark,$same_mark,$only_attribute){

    $conn = DB_CONNECT::db_conn();

    $sql = "INSERT INTO `exam_structure_comment_relate` (`sid`, `global_examType_detail_mark`, `same_mark`,`only_attribute`) VALUES (".$sid.",".$global_examType_detail_mark.",".$same_mark.",".$only_attribute.")";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;

}

//whx here
static function attribution_comments_insert($name, $attribution, $description, $weight){

	$level_id = self::attribution_comments_level_insert($name);
	$attribution = $attribution>0? $attribution:0;
	$weight = $weight>0?$weight:0;

	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));


	$conn = DB_CONNECT::db_conn();


	if($level_id>0){
		$sql = "INSERT INTO `attribute_comments` (`level_id`, `description`, `weight`,`aid`) VALUES (".$level_id.",'".$description."',".$weight.",".$attribution.");";

	    $res = $conn->query($sql);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
	 	    $id = mysql_insert_id();
		}
	}
    // 	 $conn->disconnect();
    return $id;

}
static function attribution_comments_level_insert($name){

	$conn = DB_CONNECT::db_conn();
   	$sql = "SELECT `level_id` FROM `attribute_comments_level` WHERE `level_name`='".$name."' LIMIT 1";


   	$rs = $conn->query($sql);
    $row = @$rs->fetchRow();
    if($row->level_id<1){
    	$query = "INSERT INTO `attribute_comments_level`(`level_name`) VALUES ('".$name."')";
	    $res = $conn->query($query);
	    if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$query);
		}else{
	 	    $id = mysql_insert_id();
		}
    }else{
    	 $id = $row->level_id;
    }

    // 	 $conn->disconnect();
    return $id;

}


static function structure_attr_comments_global_insert($aid,$level_id,$min_rate,$max_rate,$resources_tags1,$or_resources_tags2){

    $conn = DB_CONNECT::db_conn();

    $sql = "INSERT INTO `attribute_exam_structure_comment_global` (`aid`, `min_rate`, `max_rate`,`level_id`, `resources_tags1`,`or_resources_tags2`) VALUES (".$aid.",".$min_rate.",".$max_rate.",".$level_id.",'".$resources_tags1."','".$or_resources_tags2."')";

    //echo '<br>sql::'.$sql.'<br><br>';

    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;

}

static function structure_attr_comments_relate_insert($sid,$aid,$same_mark){

    $conn = DB_CONNECT::db_conn();

    $sql = "INSERT INTO `attribute_exam_structure_comment_relate` (`sid`, `aid`, `same_mark`) VALUES (".$sid.",".$aid.",".$same_mark.")";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;

}


static function structure_comments_insert($sid,$level_id,$min_rate,$max_rate,$resources,$resources_tags1,$or_resources_tags2){

    $conn = DB_CONNECT::db_conn();
	$resources = htmlspecialchars(addslashes(htmlspecialchars_decode($resources)));
    $sql = "INSERT INTO `exam_structure_comment` (`sid`,`level_id`, `min_rate`, `max_rate`, `resources`,`resources_tags1`,`or_resources_tags2`) VALUES (".$sid.",".$level_id.",".$min_rate.",".$max_rate.",'".$resources."','".$resources_tags1."','".$or_resources_tags2."')";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;

}


static function structure_attr_comments_insert($sid,$aid,$level_id,$min_rate,$max_rate,$resources,$resources_tags1,$or_resources_tags2){

    $conn = DB_CONNECT::db_conn();
	$resources = htmlspecialchars(addslashes(htmlspecialchars_decode($resources)));
    $sql = "INSERT INTO `attribute_exam_structure_comment` (`sid`,`aid`,`level_id`, `min_rate`, `max_rate`, `resources`,`resources_tags1`,`or_resources_tags2`) VALUES (".$sid.",".$aid.",".$level_id.",".$min_rate.",".$max_rate.",'".$resources."','".$resources_tags1."','".$or_resources_tags2."')";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	    $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;

}



/**
 *  考试（四级、六级等）的插入
 *
 * $name 考试名称
 * $description  考试描述
 * 返回插入的记录数
 */

static function exam_type_insert($name,$description){
	$conn = DB_CONNECT::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	$sql = "INSERT INTO `exam_type` (`name`,`description`)
                SELECT '$name','$description'
                FROM `exam_type`
                WHERE
                NOT EXISTS (SELECT 1 FROM `exam_type` WHERE `name`='".$name."') LIMIT 1
                 ";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;
  }


 /**
 *  考点：听力能力、阅读、写作水平 等 的插入
 *
 * $name 考点名称
 * $description  考点描述
 * 返回插入的记录数
 */

static function attribution_insert($name,$description){
	$conn = DB_CONNECT::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	$sql = "INSERT INTO `item_attribute_detail` (`name`,`description`)
                SELECT '$name','$description'
                FROM `exam_type`
                WHERE
                NOT EXISTS (SELECT 1 FROM `item_attribute_detail` WHERE `name`='".$name."') LIMIT 1
                 ";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;
  }
/**
 *  题型（阅读理解、完形填空等）的插入
 *
 * $name 题型名称
 * $description  题型描述
 * 返回插入的记录数
 */

static function item_type_insert($name,$description){
	$conn = DB_CONNECT::db_conn();
	$description = htmlspecialchars(addslashes(htmlspecialchars_decode($description)));
	$sql = "INSERT INTO `item_type` (`tid`,`name`,`description`)
                SELECT 1,'$name','$description'
                FROM `item_type`
                WHERE
                NOT EXISTS (SELECT 1 FROM `item_type` WHERE `name`='".$name."') LIMIT 1
                 ";
    $res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
 	     $id = mysql_insert_id();
	}

    // 	 $conn->disconnect();
    return $id;
  }




  /**
   * 根据模板生成试卷基本信息
   *
   * @param unknown_type $sid
   * @return unknown
   */


  static function exam_info_insert($sid,$uid){

  	$id = 0;

	$arr = EM_STRUCTURE::query_structure_info($sid);

	$conn = DB_CONNECT::db_conn();

	if($sid>0){


		if(is_array($arr)){

		   $query = "SELECT COUNT(*) num FROM `exam_info` WHERE `paper_name`like'".$arr['title']."%'";

		   $res = $conn->query($query);
		   if(DB::isError ($res)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$query);
			}else{
				 $row = $res->fetchRow();

				 if(!$row) exit('query exam_info paper_name false');
			}


	       $arr['description'] = htmlspecialchars(addslashes(htmlspecialchars_decode($arr['description'])));

		   if($row->num>0){

			 // $paper_name = $arr['title']."-".date("YmdHi",time())."-".time()."-".($row->num+1);
			 //试题的重命名 函数
			 $paper_name = $arr['title']." ".($row->num+1);

			  $sql = "INSERT INTO  `exam_info` (`paper_name`, `description`, `tid`, `sid`,`points`,`objective_points`, `time_limit`, `created`, `uid`, `status`)
                        SELECT '".$paper_name."','".$arr['description']."',".$arr['tid'].",".$sid.",".$arr['points'].",0,".$arr['time_limit'].",".time().",".$uid.",1
                        FROM `exam_info`
                        WHERE NOT EXISTS (SELECT 1 FROM `exam_info` WHERE `paper_name`='".$paper_name."' AND `tid`=".$arr['tid']." AND `sid`=".$sid.") LIMIT 1";


			}else{

			 $sql = "INSERT INTO  `exam_info` ( `paper_name`, `description`, `tid`, `sid`, `points`, `objective_points`,`time_limit`, `created`, `uid`,  `status`)
                      VALUES ('".$arr['title']." 1','".$arr['description']."',".$arr['tid'].",".$sid.",".$arr['points'].",0,".$arr['time_limit'].",".time().",".$uid.",1)";

			}


		}
		else
			exit('query_structure_info::not array!!');


		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
			 $id = mysql_insert_id();
			// exit('id:'.$id.' sql'.$sql);
		}

	}
	else
		exit('sid < 0');

    // 	 $conn->disconnect();
	return  $id;

  }






 /**
 * 根据模板生成试卷试题(批量)
 *
 * @param unknown_type $sid
 * @return unknown
 */

static function  insert_exam_items_by_structure($paper_id,$sid){

	$conn = DB_CONNECT::db_conn();

	if($sid>0){

		$sql_sname = "SELECT et.`name`
FROM `exam_type` et
LEFT JOIN `exam_structure_info` esi ON et.`tid` = esi.`tid`
WHERE esi.`sid` =".$sid;

	    $sql = "SELECT p.`structure_id`,p.`sid`, p.`term_id`, p.`check_relation`,p.`basic_type`, p.`item_type`, p.`parent`, p.`total`, p.`points`, p.`time_limit`, p.`serial_str`, p.`serial_num`, p.`is_submit`, c.`basic_type` child_basic_type,
		        c.`item_type` child_item_type
		        FROM `exam_structure`  p
		        LEFT JOIN `exam_structure` c
		        ON c.`parent`= p.`structure_id`
	            WHERE p.`sid`=".$sid." GROUP BY p.`structure_id` ORDER BY p.`structure_id`,p.`parent` ";

file_put_contents("aaa.txt", 'insert_exam_items_by_structure:\r\n'.$sql.'\r\n\r\n');

		$res = $conn->query($sql_sname);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
			if($row = $res->fetchRow(DB_FETCHMODE_ASSOC))
				if(strpos($row['name'], '真题'))
					$ques_is_random = 0;
				else
					$ques_is_random = 1;
		}

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{

			$check_child_type = 1;
			$check_is_have_parent = 1;
			$check_is_have_child = 1;

			$conditions = array("check_child_type" => $check_child_type,
								"check_is_have_parent" => $check_is_have_parent,
								"check_is_have_child" => $check_is_have_child,
								"ques_is_random" => $ques_is_random
			);

			$all_item_info = array();
			$paper_items = '';

			while($row = $res->fetchRow(DB_FETCHMODE_ASSOC)){


            //试题寻找
			  $arr = array();
			  $items_arr = array();

			  if($row['parent']==0){

              	$check_parent = 'NotNeedParent';//IgnoreParent:不检查need_parent项；NeedParent:唯一依赖父级题目；NotNeedParent：不唯一依赖与父级题目
			      $arr = EM_STRUCTURE::find_items_by_conditions($row,$conditions, $check_parent, $row['total'], '', $paper_items);
                  $sid_arr[] = $row['structure_id'];
                  $items_arr = $arr ;



                  if(is_array($arr)){
                    foreach($arr as $value){
                           $paper_items[$row['structure_id']] = $value;
                    }

                  }


              }elseif(in_array($row['parent'],$sid_arr)){

              	  if($row['check_relation']>0){
              	        $arr = EM_STRUCTURE::get_exam_items_by_parent_id($row,$paper_items[$row['parent']],$paper_items);
                   }else{

                   	    $check_parent = 'NotNeedParent';
                   	    //IgnoreParent:不检查need_parent项；NeedParent:唯一依赖父级题目；NotNeedParent：不唯一依赖与父级题目
                   	    $arr = EM_STRUCTURE::find_items_by_conditions($row,$conditions, $check_parent, $row['total'],$paper_items[$row['parent']],$paper_items);
                   }

						//file_put_contents(aaa.txt, var_export($arr, TRUE) . 'AAAAAA', FILE_APPEND);

                   if($arr!=null){
              	        $items_arr = $arr ;
                       	if(is_array($arr)){
                       		$i =  0;
							foreach($arr as $value){
								$i +=1;
								if($i>1)
                            		$structure_id_i = $row['structure_id'].'_'.$i;
                            	else
                            		$structure_id_i = $row['structure_id'];


								$paper_items[$structure_id_i] = $value;
							}

						 }


                    }

                    //试题库不够，则随机挑选试题
                    $items_num = $row['total']-count($arr);
              	   if($items_num>0){

              	   	   $new_arr = array();

					   if($row['check_relation']>0){
							$new_arr = array();
					   }else{

							$check_parent = 'NotNeedParent';//IgnoreParent:不检查need_parent项；NeedParent:唯一依赖父级题目；NotNeedParent：不唯一依赖与父级题目
							$new_arr = EM_STRUCTURE::find_items_by_conditions($row,$conditions, $check_parent, $items_num,'',$paper_items);
					   }

              	        if($new_arr!=null){
	              	        if(is_array($new_arr)){
	              	        	$i =  0;
								foreach($new_arr as $value){
									$i +=1;
									if($i>1)
										$structure_id_i = $row['structure_id'].'_new'.$i;
									else
										$structure_id_i = $row['structure_id'];

									$paper_items[$structure_id_i] = $value;
								}
	              	       }
                  	 	}

              	 } //end 试题库不够

              	 $sid_arr[] = $row['structure_id'];



               }


              $items_info = array();
              foreach ($items_arr as $id){
              	 $points = '';
				 $time_limit = '';
				 $points = $row['points']>0?$row['points']:0;
			     $time_limit = $row['time_limit']>0?$row['time_limit']:0; //时间限制
                 $items_info = array();

                 if($id>0){
              		   $items_info[$id] = array( "structure_id" => $row['structure_id'],
              		                              "parent" => $row['parent'],
			    	                             "points" => $points,
			    	                              "serial_str" => $row['serial_str'],
			    	                             "serial_num" => str_pad($row['serial_num'], 5, '0', STR_PAD_LEFT),
			    	                             "is_submit" => $row['is_submit'],
			    	                             "time_limit" =>$time_limit
			    	 ) ;

              	 }
              //if(!in_array($items_info[$id],$all_item_info)){

                     array_push($all_item_info,$items_info);
              //}

              //
   		  }
		}


	  //file_put_contents(aaa.txt, 'insert_exam_items_by_structure:$all_item_info:::::\r\n'.var_export($all_item_info, TRUE).'\r\n\r\n', FILE_APPEND);

		  $str_id = array();
		  $sid_array = array();
          $objective_points = 0;
   		  foreach ($all_item_info as $items_info) {
			  if(is_array($items_info)){
				  foreach($items_info as $key=>$value){
				  	// echo $key."<br>";

				  	 if($key>0){
				  	 	$item_basic_type_info = '';
				  	 	$item_basic_type_info = API::query_item_basic_type($key);
				  	 	if($item_basic_type_info['is_objective']==1){
				  	 		$objective_points += $value['points'];
				  	 	}



				  	 	if($value['parent']==0){

				  	      //echo "root".$row->structure_id."-------------".$value['parent']."<br>";
				  	 	  $sid_array[$value['structure_id']] = self::insert_exam_item_by_item($paper_id,0,$key,$value);
					  	  if(!in_array($value['structure_id'],$str_id))
				  	 		    $str_id[] = $value['structure_id'];

				  	 	}
				  	 	elseif(in_array($value['parent'],$str_id)){

				  	 		$sid_array[$value['structure_id']] = self::insert_exam_item_by_item($paper_id,$sid_array[$value['parent']],$key,$value);
					  	 	if(!in_array($value['structure_id'],$str_id))
				  	 		        $str_id[] = $value['structure_id'];
				  	 		//echo "child".$row->structure_id."-------------".$value['parent']."<br>";
				  	 	 }/*
				  	 	 else{

				  	 		$sid_array[$value['structure_id']] = self::insert_exam_item_by_item($paper_id,$sid_array[$value['parent']],$key,$value);
					  	 	if(!in_array($value['structure_id'],$str_id))
				  	 		        $str_id[] = $value['structure_id'];
				  	 		//echo "child".$row->structure_id."-------------".$value['parent']."<br>";
				  	 	 }*/
					 }
				  }

	     }

   	 }
   	 $objective_points = $objective_points>0?$objective_points:0;
   	 self::update_exam_objective_points($paper_id,$objective_points);
   			  //    echo "<br>";
		   //var_dump($str_id);
	   }
	}

	// 	 $conn->disconnect();

  	return $sid_array;

 }



 /**
  * 更新试卷的客观题得分
  *
  * @param unknown_type $paper_id
  * @param unknown_type $objective_points
  */
 function update_exam_objective_points($paper_id,$objective_points){

 	$conn = DB_CONNECT::db_conn();
    $query = "UPDATE `exam_info` SET `objective_points`=".$objective_points." WHERE `paper_id`=".$paper_id." LIMIT 1";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	    $id = $conn->affectedRows();
	}

    // 	 $conn->disconnect();
    return  $id;

 }


 function insert_items_by_arr_info($arr,$paper_id,$structure_id){

 	$str_id = array();
 	$sid_arr = array();

	  if(is_array($arr)){

		  foreach($arr as $key=>$value){
		  //	echo 	$row->structure_id."<br>";

		  	 if($key>0){
		  	 	if($value['parent']==0){

		  	      //echo "root".$row->structure_id."-------------".$value['parent']."<br>";
		  	 	  $sid_arr[$structure_id] = self::insert_exam_item_by_item($paper_id,0,$key,$value);
			  	  if(!in_array($structure_id,$str_id))
		  	 		        $str_id[] = $structure_id;

		  	 	}elseif(in_array($value['parent'],$str_id)){

		  	 		$sid_arr[$structure_id] = self::insert_exam_item_by_item($paper_id,$sid_arr[$value['parent']],$key,$value);
			  	 	if(!in_array($structure_id,$str_id))
		  	 		        $str_id[] = $structure_id;
		  	 		//echo "child".$row->structure_id."-------------".$value['parent']."<br>";
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

static function  insert_exam_item_by_item($paper_id,$parent_id,$item_id,$item_info){
     $conn = DB_CONNECT::db_conn();
     $query = "INSERT INTO `exam_item`( `paper_id`, `parent_id`, `item_id`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`, `status`)
               VALUES (".$paper_id.",".$parent_id.",".$item_id.",".$item_info['points'].",".$item_info['time_limit'].",'".$item_info['serial_str']."','".$item_info['serial_num']."',".$item_info['is_submit'].",1)";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_insert_id();
	}

     // 	 $conn->disconnect();
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



static function  user_result_insert($paper_id,$user_id,$result_info){
     $conn = DB_CONNECT::db_conn();
     $time_used = $result_info['time_used']>0?$result_info['time_used']:0;
     $score = $result_info['score']>0?$result_info['score']:0;

     $query = "INSERT INTO `user_results` (`paper_id`, `uid`, `time_used`, `time_end`, `score`)
               VALUES(".$paper_id.",".$user_id.",".$time_used.",".time().",".$score.")";


    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_insert_id();
	}

     // 	 $conn->disconnect();
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


static function  multichoice_user_answer_insert($item_id,$result_id,$multi_answer_id,$score){
     $conn = DB_CONNECT::db_conn();
     $query = "INSERT INTO  `multichoice_user_answer`( `item_id`, `result_id`, `multi_answer_id`, `points_awarded` )";

     if(is_array($multi_answer_id)){

     	$num = count($multi_answer_id);
     	$i = 1;
     	foreach ($multi_answer_id as $value){

     		$query .= " VALUES(".$item_id.",".$result_id.",".$value.",".$score.")";
        	if($i<$num){
        	    $query .= ",";
        	    $i++;
        	}
     	}

     }else{

     	$query .= " VALUES(".$item_id.",".$result_id.",".$multi_answer_id.",".$score.")";

     }

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_insert_id();
	}

     // 	 $conn->disconnect();
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
 static function  blankfilling_user_answer_insert($item_id,$result_id,$user_answer,$score){
     $conn = DB_CONNECT::db_conn();

     $user_answer = isset($user_answer)?htmlspecialchars(addslashes(htmlspecialchars_decode($user_answer))):'';

    // $user_answer = htmlspecialchars(addslashes(htmlspecialchars_decode(stripslashes($user_answer))));
     $query = "INSERT INTO  `blank_filling_user_answer`( `item_id`, `result_id`, `user_answer`, `points_awarded` )
               VALUES(".$item_id.",".$result_id.",'".$user_answer."',".$score.")";

    $res = $conn->query($query);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$query);
	}else{
 	     $id = mysql_insert_id();
	}

     // 	 $conn->disconnect();
     return  $id;

}


/**
 * 用户各部分得分
 *
 * @param unknown_type $id
 * @param 数组 $array
 * @return unknown
 */

static function user_result_detail_points_insert($id,$array){

	 $conn = DB_CONNECT::db_conn();
	 if(is_array($array)){

	 	$num = count($array);
	 	$query = "INSERT INTO 	`user_result_detail_points` ( `result_id`, `item_id`, `score`) VALUES ";
	 	$i = 1;
	 	foreach($array as $key => $value){

	 	   $key = $key>0?$key:0;
	 	   $value = $value>0?$value:0;

	 		if($i<$num){
	 		    $query .="(".$id.",".$key.",".$value."),";
	 		}else{
	 			$query .="(".$id.",".$key.",".$value.")";
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

     // 	 $conn->disconnect();
     return  $row;

}





}
?>