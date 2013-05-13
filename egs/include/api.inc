<?php

require_once 'mysql.inc';

class API{

//private $dsn;
//private	$conn;




/**
 *  查询考试种类
 *  如：四级、六级、托福
 *
 * 返回：数组
 */
static function query_exam_type($type,$id,$name){
    $conn = DB_CONNECT::db_conn();
    $type= !empty($type)?$type:'simple';

    if($id>0)
        $sql_condition = " WHERE `tid`=".$id;
    if(!empty($name))
        $sql_condition = " WHERE `name` LIKE '%".$name."%'";

     $sql_condition .= " ORDER BY `weight`";

    $sql = "SELECT `name`,`tid`,`description`,`weight` FROM `exam_type`";

    if(!empty($sql_condition))
            $sql .= $sql_condition;
	$res = $conn->query($sql);
	$arr = array();
	if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
			while($row = $res->fetchRow()){

			if($type=='simple'){

				$arr[] = array("tid" => $row->tid,
								"name"=>$row->name
		        );

				/* $arr[$row->tid] = $row->name;	 */

			}

			if($type=='detail'){
				$arr[] = array("tid" => $row->tid,
								"name"=>$row->name,
								"description"=>stripslashes(htmlspecialchars_decode($row->description)),
		                        "weight"=>$row->weight,
		        );

			}
		}
	}

// 	 // 	 $conn->disconnect();
	 return  $arr;


 }

 /**
 *  查询基本题型
 *  如：选择、填空
 *
 * 返回：数组
 */

static function query_basic_item_type($type,$id,$name){
    $conn = DB_CONNECT::db_conn();

    $type= !empty($type)?$type:'simple';

    if($id>0)
        $sql_condition = " WHERE `basic_type_id`=".$id;
    if(!empty($name))
        $sql_condition = " WHERE `name` LIKE '%".$name."%'";

    $sql_condition .= " ORDER BY `weight`";

    $sql = "SELECT `basic_type_id`,`name`,`description`,`weight` FROM `basic_item_type`";
    if(!empty($sql_condition))
            $sql .= $sql_condition;

	$res = $conn->query($sql);
	$arr = array();
	if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{

		while($row = $res->fetchRow()){

			if($type=='simple'){
		      // $arr[$row->basic_type_id] = stripslashes(htmlspecialchars_decode($row->name));
		      $arr[] = array("tid" => $row->basic_type_id,
								"name"=>$row->name
		        );
			}

            if($type=='detail'){
           	 $arr[] = array("tid"=>$row->basic_type_id,
           	 "name"=>stripslashes(htmlspecialchars_decode($row->name)),
		                                          "description"=>stripslashes(htmlspecialchars_decode($row->description)),
				                                  "weight"=>$row->weight
		            );
			/*
				$arr[$row->basic_type_id] = array("name"=>stripslashes(htmlspecialchars_decode($row->name)),
		                                          "description"=>stripslashes(htmlspecialchars_decode($row->description)),
				                                  "weight"=>$row->weight
		            );
		       */
            }
     	}
	}
	// 	 $conn->disconnect();
	 return $arr;

 }


/**
 *  查询考试题型
 *  如：阅读理解、完形填空
 *
 * 返回：数组
 */


static function query_exam_item_type($type,$type_id,$tid,$name){

    $conn = DB_CONNECT::db_conn();

    $type= !empty($type)?$type:'simple';

    $sql = "SELECT `type_id`,`name`,`tid`,`description`,`weight` FROM `item_type`";

    if($type_id>0)
       $sql_condition = " WHERE type_id=".$type_id;
    if($tid>0)
        $sql_condition = " WHERE tid=".$tid;
    if(!empty($name))
        $sql_condition = " WHERE name LIKE '".$name."'";
    if(!empty($sql_condition))
            $sql .= $sql_condition;

	$res = $conn->query($sql);
	$arr = array();
	if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{
	   if($res->numRows()>0){
		while($row = $res->fetchRow()){

		 if($type=='simple'){

		     $arr[] = array("type_id" => $row->type_id,
							"name"=>$row->name
		        );

		   }

		 if($type=='type_id'){    //查询type_id
		     $arr = $row->type_id;
		 }

		//详细信息

		 if($type=='detail'){
		 	 $arr[] = array("type_id"=>$row->type_id,
							  "tid" =>$row->tid,
							  "name"=>stripslashes(htmlspecialchars_decode($row->name)),
							  "description"=>stripslashes(htmlspecialchars_decode($row->description)),
							  "weight"=>$row->weight
		      ) ;

		 	/*
		     $arr[$row->type_id] = array(
		                              "tid" =>$row->tid,
		                              "name"=>stripslashes(htmlspecialchars_decode($row->name)),
		                              "description"=>stripslashes(htmlspecialchars_decode($row->description)),
		                              "weight"=>$row->weight
		      ) ;
		      */
		   }

		//按考试分类
		/*
		  if($type=='exam'){
		  	   $arr[$row->tid] = array(
		                              "type_id" =>$row->type_id,
		                              "name"=>stripslashes(htmlspecialchars_decode($row->name)),
		                              "description"=>stripslashes(htmlspecialchars_decode($row->description)),
		  	                          "weight"=>$row->weight
		      ) ;
		   }
		   */
		}
	  }else{
	  	$arr = 0;
	  }
	}
	 // 	 $conn->disconnect();


	 return  $arr;

 }




/**
 *  查询考试题型
 *  如：阅读理解、完形填空
 *
 * 返回：数组
 */


static function query_exam_structure_list($type,$sid,$tid,$name,$num = 100){
    $conn = DB_CONNECT::db_conn();

    $num = $num>0?$num:100;
    $type= !empty($type)?$type:'simple';

    $sql = "SELECT it.`sid`,it.`title`,it.`tid`,it.`uid`,it.`description`,it.`created`, ei.`name`
            FROM `exam_structure_info` it
            LEFT JOIN `exam_type` ei ON ei.tid=it.tid WHERE `status`=1 ";
    if($sid>0){
    	$sql_condition .= " AND it.`sid`=".$sid;
    }

    $term_id_array = explode(',',$tid);
    $tid_count = count($term_id_array);

    if($tid_count>1){
    	$sql_condition .= " AND( it.`tid`=".implode($term_id_array,' OR it.`tid`=').")";
    }
    else if($tid>0){
        $sql_condition .= " AND it.`tid`=".$tid;
    }
    if(!empty($name)){
        $sql_condition .= " AND it.`title` LIKE '".$name."%'";
    }
     $sql_condition .= " ORDER BY it.`weight` , it.`created` desc LIMIT ".$num;

    if(!empty($sql_condition))
            $sql .= $sql_condition;

	$res = $conn->query($sql);
	$arr = array();
	if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{

		while($row = $res->fetchRow()){
		 if($type=='simple'){
		 	$arr[] = array("sid" => $row->sid,
						  "title" => $row->title
					);
		    // $arr[$row->sid] = $row->title;
		   }

		//详细信息

		 if($type=='detail'){
		 	$arr[] = array("sid" => $row->sid,
		 				   "uid" => $row->uid,
						  "tid" =>$row->tid,
						  "title" => $row->title,
						  "type_name"=>stripslashes(htmlspecialchars_decode($row->name)),
						  "description"=>stripslashes(htmlspecialchars_decode($row->description)),
						  "created"=>date("Y-m-d H:i:s",$row->created)
		      ) ;
		      /*
		     $arr[$row->sid] = array("uid" => $row->uid,
		                              "tid" =>$row->tid,
		                              "title" => $row->title,
		                              "type_name"=>stripslashes(htmlspecialchars_decode($row->name)),
		                              "description"=>stripslashes(htmlspecialchars_decode($row->description)),
		                              "created"=>date("Y-m-d H:i:s",$row->created)
		      ) ;
		      */
		   }

		 }
	}
	 // 	 $conn->disconnect();
	 return $arr;

 }


 /**
 * 查询试卷基本信息
 *
 * @param unknown_type $paper_id
 * @return unknown
 */

static function  query_exam_structure_by_sid($type,$sid,$title){


	$arr = array();

	$conn = DB_CONNECT::db_conn();
	$type= !empty($type)?$type:'simple';
    $sql = "SELECT `sid`, `tid`, `uid`, `title`, `description`, `points`, `time_limit`, `created`, `status`, `weight`
	            FROM `exam_structure_info`";

    if($sid>0){
    	$sql_condition = " WHERE `sid`=".$sid;
     }

    if(!empty($title)){
        $sql_condition = " WHERE `title` LIKE '".$title."%'";
     }

     if(!empty($sql_condition)){
            $sql .= $sql_condition;
     }


     $res = $conn->query($sql);
     if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
	 }else{
		 while($row = $res->fetchRow()){

		 	if($type=='simple'){
		          $arr[$row->sid] =  stripslashes(htmlspecialchars_decode($row->title));

		       }
		    if($type=='list'){
		          $arr[] =array("sid"=>$row->sid,
		                       "title" => stripslashes(htmlspecialchars_decode($row->title))
		          );

		       }

			if($type=='detail'){
				  $exam_type = '';
				  $exam_type = self::query_exam_type('simple',$row->tid,'');

				  $arr[$row->sid] = array(
						  "sid" => $row->sid,
						  "title" => stripslashes(htmlspecialchars_decode($row->title)),
						  "description" => stripslashes(htmlspecialchars_decode($row->description)),
						  "exam_type" => $exam_type[0]["name"],
						  "tid" => $row->tid,
						  "points" => $row->points,
						  "time_limit" => $row->time_limit,
						  "created" => date("Y-m-d H:i:s",$row->created),
						  "status" => $row->status,
						  "weight" => $row->weight
					);

			}
		}
	}

     // 	 $conn->disconnect();
     return  $arr;


}





/**
 *  查询考试试题
 *  如：阅读理解、完形填空
 *
 * 返回：数组
 */


static function query_item_info($type,$id,$name,$op){

    $conn = DB_CONNECT::db_conn();

    $type= !empty($type)?$type:'simple';

   $sql = "SELECT i.`item_id`, i.`name` item_name, i.`file_id`,i.`type`, i.`basic_type`, i.`body`, i.`description`, i.`points`,
           i.`time_limit`, i.`created`, i.`is_random`,i.`file_id`, i.`uid`,bi.`name` basic_name,
           t.name type_name
           FROM `item_info` i
           LEFT JOIN `basic_item_type` bi ON bi.`basic_type_id` = i.`basic_type`
           LEFT JOIN `item_type` t ON t.`type_id` = i.`type` ";

    if($op=='by_item'){

	    if($id>0)
	        $sql_condition = " WHERE i.`item_id`=".$id;
	    if(!empty($name))
	        $sql_condition = " WHERE i.`name` LIKE '".$name."%'";

    }

    if($op=='by_type'){

	     if($id>0)
	       $sql_condition = " WHERE i.`type`=".$id;

    }


    if(!empty($sql_condition))
            $sql .= $sql_condition;
    $sql .= " AND i.`status`=1";

	$res = $conn->query($sql);

	$arr = array();
	if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{

		while($row = $res->fetchRow()){


		 if($type=='simple'){
		     $arr[$row->item_id] = $row->item_name;
		   }

		//详细信息

		 if($type=='detail'|$type=='paper'){
		 	  $tags = '';
		 	  $tags = self::query_tags_by_item_id($row->item_id);

		 	  if($row->file_id>0){
                  $files = self::query_file_link($row->file_id);
		 	  }else{
		 	  	  $files = array("file_status" => 0);
		 	  }


		 	 $answers = '';
		 	 if($row->basic_type==1|$row->basic_type==2|$row->basic_type==4){
				if($row->basic_type==1|$row->basic_type==2){
				   $new_arr = self::query_item_answers_properties($row->basic_type,$row->item_id);
			   }

		 	   $answers = self::query_item_answers($row->basic_type,$row->item_id,$new_arr[$row->item_id]["choice_random"]);

		 	 }

		 	  if($type=='detail'){
		 		$arr[$row->item_id] = array(
		                              "type" => array($row->type => $row->type_name ),
		                              "basic_type" =>array($row->basic_type=> $row->basic_name ),
		                              "name" => stripslashes(htmlspecialchars_decode($row->item_name)),
		 				              "uid" => $row->uid,
		                              "body" => stripslashes(htmlspecialchars_decode($row->body)),
		                              "description" => stripslashes(htmlspecialchars_decode($row->description)),
		 		                      "tags" => $tags,
		 		                      "files" => $files,
		                              "answers" => $answers,
		                              "points" => $row->points,
		 		                      "created" => date("Y-m-d H:i:s",$row->created),
		                              "time_limit" => $row->time_limit,
		 		                      "is_random" => $row->is_random    //子题目是否随机
		       ) ;
		 	  }

		      if($type=='paper'){  //试卷调取试题时去掉冲突的属性 如：分值、时间限制等

		      			$arr[$row->item_id] = array(
		                              "type" => array($row->type => $row->type_name ),
		                              "basic_type" =>array($row->basic_type=> $row->basic_name ),
		                              "name" => stripslashes(htmlspecialchars_decode($row->item_name)),
		 				              "uid" => $row->uid,
		                              "body" => stripslashes(htmlspecialchars_decode($row->body)),
		                              "description" => stripslashes(htmlspecialchars_decode($row->description)),
		 		                      "tags" => $tags,
		      			              "files" => $files,
		                              "answers" => $answers,
		                       		  "is_random" => $row->is_random    //子题目是否随机
		       ) ;

		      }


		      if(($row->basic_type==1|$row->basic_type==2)&$new_arr[$row->item_id]!=''){
		        $arr[$row->item_id] = array_merge($arr[$row->item_id],$new_arr[$row->item_id]);
		      }



		    }
		 }
	}
	 // 	 $conn->disconnect();
	 return $arr;

 }




 /**
  * 查询附件信息
  *
  * @param unknown_type $file_id
  */


 static  function query_file_link($file_id){
  	 $conn = DB_CONNECT::db_conn();
  	 $sql = "SELECT `fid`, `type`, `uid`, `filename`, `fileurl`, `filepath`, `filemime`, `filesize`, `status`, `created`
  	          FROM `files` WHERE `fid`=".$file_id;
  	 $res = $conn->query($sql);
  	 $array = array();
	 if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	 }else{
	 	while($row = $res->fetchRow()){
	 		if($row->fileurl!=''){
		 		$array[$file_id] = array("file_status" => 1,
		 		                         "file_type"=>$row->type,
		 		                         "uid"=>$row->uid,
		 		                         "filename"=>$row->filename,
		 		                         "fileurl"=>$row->fileurl,
		 		                         "created"=>$row->created
		 		);
	 		}else{
		 		$array[$file_id] = array("file_status" => 0);
	 		}
	 	}
	 }

		return $array[$file_id];

  }





 /**
  * 查询试题的设置
  * 如单选、多选、主观填空
  *
  *
  * @param unknown_type $type
  * @param unknown_type $type_id
  * @param unknown_type $tid
  * @param unknown_type $name
  * @return unknown
  */


 static function query_item_answers_properties($basic_type,$item_id){

   	$conn = DB_CONNECT::db_conn();

   	if($basic_type==1){
  	    $sql = "SELECT `choice_multi`,`choice_random`  FROM `multichoice_properties` WHERE `item_id`=".$item_id;
   	}
  	if($basic_type==2){
  	    $sql = "SELECT is_objective, blank_size
				FROM `blank_filling_answers` ba
				LEFT JOIN `blank_filling_properties` bp ON ba.item_id = bp.item_id
				WHERE ba.item_id =".$item_id;
 	}

  	$res = $conn->query($sql);
    $arr = array();
    if(DB::isError ($res)){
	  $error_messsage = mysql_error();
	  self::halt($error_messsage,$sql);
    }else{
    	while($row = $res->fetchRow()){

    		if($basic_type==1){

			   $arr[$item_id] = array("choice_multi" => $row->choice_multi,
			                          "choice_random" => $row->choice_random
			                           );
    		}

    	    if($basic_type==2){
			   $arr[$item_id] = array("is_objective" => $row->is_objective,
			                           "blank_size" => $row->blank_size
			                          );

    		}
    	}
   }


    // 	 $conn->disconnect();
 	return $arr;

 }





 /**
  * 通过标签查询试题
  *
  * @param unknown_type $type
  * @param unknown_type $tag_id
  * @param unknown_type $tag_name
  * @return unknown
  */


static function query_item_by_tag($type,$tag_id,$tag_name){

    $conn = DB_CONNECT::db_conn();
    $type= !empty($type)?$type:'simple';
    $sql = "SELECT i.`item_id`, i.`name`, i.`type`, i.`basic_type`, i.`body`, i.`description`, i.`points`, i.`time_limit`, i.`created`, i.`file_id`, i.`uid`,td.`tid`,td.`name`
			FROM `item_info` i
			LEFT JOIN `term_item` ti  on ti.item_id = i.item_id
			LEFT JOIN `term_data` td on td.tid = ti.term_id";

    if($tag_id>0)
       $sql_condition = " WHERE td.`tid`=".$tag_id;

    if(!empty($tag_name))
        $sql_condition = " WHERE td.`name` LIKE '".$tag_name."%'";

    if(!empty($sql_condition))
            $sql .= $sql_condition." AND i.status=1";

	$res = $conn->query($sql);

	$arr = array();
	if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{

		while($row = $res->fetchRow()){

		 if($type=='simple'){
		     $arr[$row->item_id] = $row->name;
		   }

		//详细信息

		 if($type=='detail'){
		 	 $tags = '';
		 	 $tags = self::query_tags_by_item_id($row->item_id);

		 	 $answers = '';
		 	 if($row->basic_type>0){
		 	    $answers = self::query_item_answers($row->basic_type,$row->item_id);
		 	 }

		     $arr[$row->item_id] = array(
		                              "type" => $row->type,
		                              "basic_type" =>  $row->basic_type,
		                              "name"=>stripslashes(htmlspecialchars_decode($row->name)),
		                              "uid" => $row->uid,
		                              "body"=>stripslashes(htmlspecialchars_decode($row->body)),
		     		 		          "tags" => $tags,
		                              "answers" => $answers,
		                              "description"=>stripslashes(htmlspecialchars_decode($row->description)),
		                              "points" => $row->points,
		                              "time_limit" => $row->time_limit
		      ) ;
		   }


		 }
	}
	 // 	 $conn->disconnect();
	 return  $arr;

 }


  /**
  * 输入试题id查询标签
  *
  * 输出 数组
  *
  */

 static function query_tags_by_item_id($id){
 	$conn = DB_CONNECT::db_conn();
    if($id>0){

   	  	 $sql = "SELECT td.name, td.tid
                 FROM `term_data` td, term_item ti
                 WHERE td.tid = ti.term_id
                 AND ti.item_id =".$id;
   	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){
	    		if($row->name!='')
				   $arr[$row->tid] = stripslashes(htmlspecialchars_decode($row->name));
			}
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;

 }


  /**
  * 输入试标签 id查询标签
  *
  * 输出 数组
  *
  */

 static function query_tag_info_by_tag($type,$tid='',$name=''){

 	$conn = DB_CONNECT::db_conn();
    $type= !empty($type)?$type:'simple';
    $sql = "SELECT td.`name`,td.`description`,td.`tid`,td.weight
                 FROM `term_data` td";

    if($tid>0){
    	$sql_condition .= " WHERE td.`tid` =".$tid;
     }

    if(!empty($name)){
        $sql_condition = " WHERE td.`name` LIKE '".$name."%'";
     }

     if(!empty($sql_condition)){
            $sql .= $sql_condition;
     }

      	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){

	           if($type=='simple'){
		           $arr[$row->tid] = stripslashes(htmlspecialchars_decode($row->name));
		       }


			    if($type=='detail'){
			          $arr[$row->tid] = array(
			                              "name" => stripslashes(htmlspecialchars_decode($row->name)),
			                              "description" =>  stripslashes(htmlspecialchars_decode($row->description)),
			                              "weight" => $row->weight,
			      ) ;
			   }
			}
	  	}

    // 	 $conn->disconnect();
    return $arr;

 }


/**
  * 输入试题id查询考点
  *
  * 输出 数组
  *
  */

 static function query_item_attribution_by_item_id($id){
 	$conn = DB_CONNECT::db_conn();
    if($id>0){

   	  	 $sql = "SELECT iah.aid, iad.name
					FROM `item_attribute_hierarchy` iah, item_attribute_detail iad
					WHERE iah.`item_id` = iad.`item_id`
						AND iah.`item_id` = ".$id;

   	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){
	    		$arr[] = array(
	    				"aid" => $row->aid,
						"name"=>stripslashes(htmlspecialchars_decode($row->name))
				);
			}
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;

 }

  /**
  * 输入考点id 查询考点
  *
  * 输出 数组
  *
  */

 static function query_item_attribution($type,$aid='',$name=''){


 	$conn = DB_CONNECT::db_conn();
    $type= !empty($type)?$type:'simple';
    $sql = "SELECT *
                 FROM `item_attribute_detail`";


    if(!empty($name)){
        $sql_condition = " WHERE `name` LIKE '".$name."%'";
     }
     if($aid>0){
    	$sql_condition = " WHERE `aid` =".$aid;
     }

     if(!empty($sql_condition)){
            $sql .= $sql_condition;
     }

      	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){

	           if($type=='simple'){
	           		$arr[] = array("aid" => $row->aid,
								"name"=>stripslashes(htmlspecialchars_decode($row->name))
		        	);

		       }


			    if($type=='detail'){

			    	$arr[] = array("aid" => $row->aid,
							"name"=>stripslashes(htmlspecialchars_decode($row->name)),
							"description" =>  stripslashes(htmlspecialchars_decode($row->description)),
	                        "weight" => $row->weight,
		        	);
			   }
			}
	  	}

    // 	 $conn->disconnect();
    return $arr;

 }


 /**
  * 输入模板id查询此模板设置过的考点
  *
  * 输出 数组
  *
  */

 static function query_item_attribution_by_structure_id($sid){
 	$conn = DB_CONNECT::db_conn();
    if($sid>0){

   	  	 $sql = "SELECT `aid` , `name`
					FROM `item_attribute_detail` iad
					WHERE `aid`
					IN (
						SELECT DISTINCT `aid`
						FROM `attribute_exam_structure_comment`
						WHERE sid = ".$sid.
					")";

   	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){
	    		$arr[] = array(
	    				"aid" => $row->aid,
						"name"=>stripslashes(htmlspecialchars_decode($row->name))
				);
			}
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;

 }

  /**
  * 输入试题id数组，查询所有考点
  *
  * 输出 数组
  *
  */

 static function query_item_attribution_by_item_id_array($item_id_arr){
 	$conn = DB_CONNECT::db_conn();
 	$item_id_str = implode(' OR item_id = ',$item_id_arr);
    if($item_id_arr!=null && !empty($item_id_arr)){

   	  	 $sql = "SELECT `aid` , `name`
					FROM `item_attribute_detail` iad
					WHERE `aid`
					IN (
						SELECT DISTINCT `aid`
						FROM `item_attribute_hierarchy`
						WHERE item_id = ".$item_id_str.
					")";

   	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){
	    		$arr[$row->aid] = array(
	    				"aid" => $row->aid,
						"name"=>stripslashes(htmlspecialchars_decode($row->name))
				);
			}
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;

 }
 /**
  * 输入试题id的parent_id
  *
  * 输出 数组
  *
  */

static function query_item_parent_id($id){
 	$conn = DB_CONNECT::db_conn();
    if($id>0){

   	  	$sql = "SELECT `parent` FROM `item_hierarchy` WHERE `item_id` =".$id." LIMIT 1 ";
   	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	$row = $res->fetchRow();
	    	$arr = $row->parent;
	  }
   }

    // 	 $conn->disconnect();

 	return $arr;

 }



 /**
  * 获取选择题选项
  *
  * 输出 数组
  *
  */

 static function query_item_answers($basic_type,$item_id,$condition = 0){

   	$conn = DB_CONNECT::db_conn();

   	/*
   	if($basic_type==1)
  	    $sql = "SELECT `answer_id`, `answer`,`feedback`,`is_correct`  FROM `multichoice_answers` WHERE `item_id`=".$item_id;
  	*/


   	if($basic_type==1){

   		if($condition>0)
  	    	$sql = "SELECT `answer_id`, `answer`,`feedback`,`is_correct`  FROM `multichoice_answers` WHERE `item_id`=".$item_id." ORDER BY rand() ";
  	   else
  	    	$sql = "SELECT `answer_id`, `answer`,`feedback`,`is_correct`  FROM `multichoice_answers` WHERE `item_id`=".$item_id;

  	}
 	if($basic_type==2)
  	    $sql = "SELECT `filling_answer_id`, `correct_answer`, `feedback`, `is_objective` FROM `blank_filling_answers` WHERE  `item_id`=".$item_id;
    if($basic_type==4)
  	    $sql = "SELECT `mid`, `item_id`, `feedback` FROM `material_items_feedback` WHERE  `item_id`=".$item_id;


  	$res = $conn->query($sql);
    $arr = array();
    if(DB::isError ($res)){
	    $error_messsage = mysql_error();
	    self::halt($error_messsage,$sql);
    }else{
    	while($row = $res->fetchRow()){

    		if($basic_type==1&$row->answer!=''){

			   $arr[$row->answer_id] = array("answer" => stripslashes(htmlspecialchars_decode($row->answer)),
			                                 "feedback" => stripslashes(htmlspecialchars_decode($row->feedback)),
			                                 "is_correct" => $row->is_correct
			                                 );
    		}

    	    if($basic_type==2&$row->correct_answer!=''){
			   $arr[$row->filling_answer_id] = array("answer" => stripslashes(htmlspecialchars_decode($row->correct_answer)),
			                                 "feedback" => stripslashes(htmlspecialchars_decode($row->feedback)),
			                                 "is_objective" => $row->is_objective
			                                 );

    		}
    	  if($basic_type==4){
			   $arr[$row->mid] = array("feedback" => stripslashes(htmlspecialchars_decode($row->feedback)),
			                          	       );

    		}
    	}
   }


    // 	 $conn->disconnect();
 	return $arr;


 }


/**
  * 获取选择题选项
  *
  * 输出 数组
  *
  */

 static function query_item_basic_type($item_id){

   	$conn = DB_CONNECT::db_conn();

  	$sql = "SELECT `basic_type` FROM `item_info` WHERE `item_id`=".$item_id;

  	$res = $conn->query($sql);
    if(DB::isError ($res)){
	     $error_messsage = mysql_error();
	     self::halt($error_messsage,$sql);
    }else{
    	while($row = $res->fetchRow()){

    		if($row->basic_type==1){

			   $arr= array("basic_type" => $row->basic_type,
			               "is_objective" => 1
			               );
    		 }

    	    if($row->basic_type==2){

    	       $query = "SELECT `is_objective` FROM `blank_filling_answers` WHERE `item_id`=".$item_id;
          	   $result = $conn->query($query);
          	   $array = $result->fetchRow();
          	   if($array->is_objective==1){
				   $arr= array("basic_type" => $row->basic_type,
				               "is_objective" => 1
				               );
          	   }
    		}
    	}
   }


    // 	 $conn->disconnect();
 	return $arr;

 }


static function query_exam_comments($level_id='',$name=''){

 	$conn = DB_CONNECT::db_conn();
    $sql = "SELECT ec.`cid`,el.`level_name`, ec.`description`,ec.`weight`
			FROM `exam_comments` ec, `exam_comments_level` el";

    if(!empty($name)){
        $sql_condition = " WHERE el.`level_name` LIKE '".$name."%'";
    }

    if($level_id>0){
    	$sql_condition = " WHERE el.`level_id` =".$level_id;
     }

     if(!empty($sql_condition)){
            $sql .= $sql_condition;
            $sql .= " AND ";
     }else{
     	$sql .=  " WHERE ";
     }

     $sql .= " ec.level_id = el.level_id ORDER BY ec.`weight` DESC,`level_name`";

      	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){
			          $arr[$row->cid] = array(
			          		"name" => stripslashes(htmlspecialchars_decode($row->level_name)),
			          		"description" =>  stripslashes(htmlspecialchars_decode($row->description)),
			                "weight" =>  $row->weight
			      ) ;
			}

	  	}

    // 	 $conn->disconnect();
    return $arr;

 }



static function query_exam_comments_level($level_id='',$name=''){

 	$conn = DB_CONNECT::db_conn();
    $sql = "SELECT el.`level_name`, el.`level_id`
			FROM `exam_comments_level` el";

    if($level_id>0){
    	$sql_condition .= " WHERE el.`level_id` =".$level_id;
     }

    if(!empty($name)){
        $sql_condition = " WHERE el.`level_name` LIKE '".$name."%'";
    }

     if(!empty($sql_condition)){
        $sql .= $sql_condition;

     }

    $res = $conn->query($sql);
    $arr = array();
    if(DB::isError ($res)){
	  $error_messsage = mysql_error();
	  self::halt($error_messsage,$sql);
    }else{
    	while($row = $res->fetchRow()){
		   $arr[$row->level_id] = stripslashes(htmlspecialchars_decode($row->level_name));
		}
  	}
    // 	 $conn->disconnect();
    return $arr;

 }




static function query_attribution_comment($type,$level_id='',$attr_id=''){

 	$conn = DB_CONNECT::db_conn();
 	$type= !empty($type)?$type:'simple';

    $sql = "SELECT ad.`aid`, ad.`name`, ac.`aCid`, el.`level_id`,el.`level_name`, ac.`description`, ac.`weight`
			FROM `attribute_comments` ac, `attribute_comments_level` el, `item_attribute_detail` ad WHERE ac.`level_id`=el.`level_id` and ac.`aid`=ad.`aid` ";

    if($attr_id>0){
        $sql_condition .= " AND ac.`aid` =".$attr_id;
    }
    else{
 		$attr_arr = self::query_item_attribution('simple','','');

 		//$attrs = implode(' OR el.`aid` =',$attr_arr);
		//$sql_condition .= " AND (el.`aid` =".$attrs.")";

		foreach ($attr_arr as $value)
		{
			//echo $level_id.':::'.$aid.'<br/>';
		  $arr_all[] = self::query_attribution_comment('detail',$level_id,$value['aid']);
		  //echo var_export($arr_all,true).'<br/>';
		}

		return $arr_all;

 	}

    if($level_id>0){
    	$sql_condition .= " AND el.`level_id` =".$level_id;
     }


     if(!empty($sql_condition)){
            $sql .= $sql_condition;
     }

     $sql .= " ORDER BY ac.`weight` DESC,`level_name`";

	$res = $conn->query($sql);
	$arr = array();
	if(DB::isError ($res)){
	  $error_messsage = mysql_error();
	  self::halt($error_messsage,$sql);
	}else{

		$arr['attribution'] = self::query_item_attribution('simple',$attr_id,'');
		$arr['comment'] = null;

		while($row = $res->fetchRow()){
			if($type == 'single'){
				$arr['comment'][$row->level_id] = stripslashes(htmlspecialchars_decode($row->level_name));

			}
			if($type == 'simple'){
				$arr['comment'][] = array(
						"level_id" => $row->level_id,
				  		"level_name" => stripslashes(htmlspecialchars_decode($row->level_name))
				);
			}
			if($type == 'detail'){
				$arr['comment'][] = array(
						"level_id" => $row->level_id,
				  		"level_name" => stripslashes(htmlspecialchars_decode($row->level_name)),
						 "description" =>  stripslashes(htmlspecialchars_decode($row->description)),
						"weight" =>  $row->weight
				);
			}
		}

	}

    // 	 $conn->disconnect();
    return $arr;

 }

static function query_attribution_comments_level($level_id='',$name=''){

 	$conn = DB_CONNECT::db_conn();
    $sql = "SELECT el.`level_name`, el.`level_id`
			FROM `attribute_comments_level` el";

    if($level_id>0){
    	$sql_condition .= " WHERE el.`level_id` =".$level_id;
     }

    if(!empty($name)){
        $sql_condition = " WHERE el.`level_name` LIKE '".$name."%'";
    }

     if(!empty($sql_condition)){
        $sql .= $sql_condition;

     }

    $res = $conn->query($sql);
    $arr = array();
    if(DB::isError ($res)){
	  $error_messsage = mysql_error();
	  self::halt($error_messsage,$sql);
    }else{
    	while($row = $res->fetchRow()){
		   $arr[$row->level_id] = stripslashes(htmlspecialchars_decode($row->level_name));
		}
  	}
    // 	 $conn->disconnect();
    return $arr;

 }


 /**
  *  试题重命名，根据其试题类型给其命名
  *
  * @param unknown_type $type_id
  */

 static function item_rename($type_id){

          $type = self::query_exam_item_type('',$type_id,'','');

		/*
    	  if(!empty($type[$type_id])){

    	  	 $name = $type[$type_id].date("YmdHis",time());
    	 */
    	 if(!empty($type[0]['name'])){

    	  	 $name = $type[0]['name'].date("YmdHis",time());

    	  }else{

    	  	 $name = date("YmdHis",time());

    	  }

    	  return $name;

   }

static function get_paper_by_paper_id($type,$id,$uid){


	$type = !empty($type)?$type:'simple';
	$uid = $uid>0?$uid:0;

    $arr = array();

    $arr['paper_info'] = self::query_paper_info_by_paper_id($type,$id,'');


	//$latest_visited_times = $arr['paper_info'][$id]['visited_times']+1;

	$conn = DB_CONNECT::db_conn();
	$sql = "INSERT INTO `exam_info_visited`
					(`id` ,`paper_id` ,`visited` ,`uid`)
			VALUES (NULL , ".$id.", ".time().", ".$uid.")";

   	$res = $conn->query($sql);
    if(DB::isError ($res)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}
	else{
		$visited_id = mysql_insert_id();
	}
	//$arr['paper_info'][$id]['visited_times'] = $latest_visited_times;


	$arr['paper_items'] = self::query_paper_items_by_paper_id($id);

	// 	 $conn->disconnect();
	return $arr;
}
/**
 * 查询试卷基本信息
 *
 * @param unknown_type $paper_id
 * @return unknown
 */

static function  query_paper_info_by_paper_id($type,$paper_id,$paper_name){

	$conn = DB_CONNECT::db_conn();
	$type= !empty($type)?$type:'simple';
    $sql = "SELECT `paper_id`, `paper_name`, `description`, `tid`, `sid`, `points`, `time_limit`, `created`,`visited_times`, `uid`, `is_full`
               FROM `exam_info`";

    if($paper_id>0){
    	$sql_condition .= " WHERE `paper_id`=".$paper_id;
     }

    if(!empty($paper_name)){
        $sql_condition = " WHERE `paper_name` LIKE '".$paper_name."%'";
     }

    $sql_condition .= " ORDER BY `created` desc";

     if(!empty($sql_condition)){
            $sql .= $sql_condition;
     }


     $res = $conn->query($sql);
     if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
	 }else{
		 while($row = $res->fetchRow()){

		 	if($type=='simple'){
		          $arr[$row->paper_id] =  stripslashes(htmlspecialchars_decode($row->paper_name));

		       }
		    if($type=='list'){
		          $arr =array("paper_id"=>$row->paper_id,
		                       "paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name))
		          );

		       }

			if($type=='detail'){
				  $exam_type = '';
				  $exam_type = self::query_exam_type('simple',$row->tid,'');

				  $sql = "SELECT count(ii.item_id) count FROM exam_item ei
						  JOIN item_info ii ON ei.item_id = ii.item_id AND (ii.basic_type=1 OR ii.basic_type=2)
						  WHERE paper_id = ? ";
				  $num = $conn->getOne($sql,array($paper_id));

				  $arr[$row->paper_id] = array(
				  	 "paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
					  "description" => stripslashes(htmlspecialchars_decode($row->description)),
					  "exam_type" => $exam_type[0]["name"],
					  "term_id" => $row->tid,
					  "sid" => $row->sid,
					  "points" => $row->points,
					  "time_limit" => $row->time_limit,
		 			  "amount"=>$num,
					  "created" => date("Y-m-d H:i:s",$row->created),
					  "visited_times" => $row->visited_times,
					  "uid" => $row->uid,
					  "is_full" => $row->is_full
				);


		 }

	  }
	 }

     // 	 $conn->disconnect();
     return  $arr;


}


 /**
   * 通过试卷分类查询试卷基本信息
   *
   * @param unknown_type $paper_id
   * @return unknown
   */

static function  query_paper_info_by_term_id($type,$term_id,$num = 100){

	$conn = DB_CONNECT::db_conn();
	$num = $num>0?$num:100;
	$type= !empty($type)?$type:'simple';
    $sql = "SELECT `paper_id` , `paper_name` , ei.`description` , et.`tid` , et.name, `sid` , `points` , `time_limit` , `created` , `uid` , `is_full` , et.`description` summary
			FROM `exam_info` ei
			LEFT JOIN `exam_type` et ON ei.`tid` = et.`tid`";
    $term_id_array = explode(',',$term_id);
    $tid_count = count($term_id_array);

    if($tid_count>1){
    	$sql_condition .= " WHERE ei.`tid`=".implode($term_id_array,' OR ei.`tid`=');
    }
    else if($term_id>0){
    	$sql_condition .= " WHERE ei.`tid`=".$term_id;
     }

     $sql_condition .= " ORDER BY `created` desc LIMIT ".$num;

    if(!empty($sql_condition)){
            $sql .= $sql_condition;
     }

 //echo 'term_id：'.$term_id.'<br>term_id_array::'.var_export($term_id_array,true).'<br><br>'.$sql;

     $res = $conn->query($sql);
     if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
	 }else{
		 while($row = $res->fetchRow()){

		 	if($type=='simple'){
		 		$arr[] = array("paper_id" => $row->paper_id,
		 						"paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
		 						"term_id" => $row->tid,
					            "exam_type" => $row->name
					     );

		        //  $arr[$row->paper_id] =  $row->paper_name;

		       }

			if($type=='detail'){

				  $arr[] = array("paper_id" => $row->paper_id,
				  "paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
					                      "description" => stripslashes(htmlspecialchars_decode($row->description)),
				                          "term_id" => $row->tid,
					                      "exam_type" => $row->name,
				                          "exam_description" => stripslashes(htmlspecialchars_decode($row->summary)),
				                          "sid" => $row->sid,
					                      "points" => $row->points,
					                      "time_limit" => $row->time_limit,
				                          "created" => date("Y-m-d H:i:s",$row->created),
					                      "uid" => $row->uid,
					                      "is_full" => $row->is_full
					                );


		 }
	  }
	 }

     // 	 $conn->disconnect();
     return  $arr;


}



   /**
   * 查询试卷试题信息
   *
   * @param unknown_type $paper_id
   * @return unknown
   */

static function  query_paper_items_by_paper_id($paper_id){

	$conn = DB_CONNECT::db_conn();

    $sql = "SELECT `exam_item_id`, `paper_id`, `parent_id`, `item_id`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`, `status`
            FROM `exam_item` WHERE `paper_id`=".$paper_id." ORDER BY `parent_id`,`serial_num`,`exam_item_id`";

     $res = $conn->query($sql);
     if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
	 }else{
	 	$i = 0;
	 	$j = 0;
		while($row = $res->fetchRow()){

                  $item = '';
	 	          $item = self::query_item_info('paper',$row->item_id,'',"by_item");

	 	          if(strpos($row->serial_num,"-")){

	 	          	$serial_arr = explode("-",$row->serial_num);
	 	          	$serial_num = $serial_arr[1];

	 	          }else{

	 	          	$row->serial_num = $serial_num;

	 	          }

	 	          $arr[$i] = array("item_id" => $row->item_id,
				                     "paper_id" => $row->paper_id,
					              	  "points" => $row->points,
				                      "time_limit" => $row->time_limit,
					                  "serial_str" => $row->serial_str,
					                  "serial_num" => $serial_num,
				                       "is_submit" => $row->is_submit
					                );

                  if($item[$row->item_id]!=''){

				      $arr[$i] = array_merge($arr[$i],$item[$row->item_id]);

                  }
	//		echo $row->item_id."-----".$row->parent_id;
	//		echo "<br>";
	 	          if($row->parent_id == 0){

	 	          	  $output[$j] = $arr[$i];
					  $key[] = $row->exam_item_id;
					  $root[$row->exam_item_id] = $j;
					  $id[$row->exam_item_id] = 1;
					  $j++;

	 	          }elseif(in_array($row->parent_id,$key)){
	 	          	//使用引用的方法生产多维数组
	                $relation[$row->exam_item_id] = $i;
	 	          	if($id[$row->parent_id]==1){

	 	          		$temp[$i] = &$output[$root[$row->parent_id]]['subque'][];

	 	          	}else{

	 	          	    $temp[$i] = &$temp[$relation[$row->parent_id]]['subque'][];

	 	          	}

	 	          	  $temp[$i] = $arr[$i];
	 	              $relation[$row->exam_item_id] = $i;
                   	  $key[] = $row->exam_item_id;

	 	          }

             $i++;

		}

	 }

     // 	 $conn->disconnect();

     return $output;

 }





    /**
   * 查询试卷详细信息，包括其中的试题和试卷简介
   *
   * @param unknown_type $paper_id
   * @return unknown
   */

static function  query_paper_detail_by_paper_id($paper_id){

	$output = array();

    $output['paper_info'] = self::query_paper_info_by_paper_id('detail',$paper_id,'');

    $output['paper_item'] = self::query_paper_items_by_paper_id($paper_id);

    return $output;


}

/**
 * 查询用户做卷的记录（不含做题信息）
 *
 * @param int $uid ($uid == -1,管理员查询所有人数据）
 * @param unknown_type $type
 * @return unknown
 */

 static function query_user_results_by_uid($type,$uid,$type_id,$num){

 	$conn = DB_CONNECT::db_conn();

 	$num = $num>0?$num:100;
    $type= !empty($type)?$type:'simple';

    $sql = "SELECT ur.`result_id`, ur.`uid`, ur.`paper_id`, ur.`time_used`, ur.`time_end`, ur.`score`, ur.`is_valid`,ei.`objective_points`,ei.`paper_id`,ei.`points`,ei.`paper_name`
				FROM `user_results` ur
				LEFT JOIN `exam_info` ei
				ON ei.`paper_id`=ur.`paper_id`
				WHERE ur.`is_valid`=1 ";

   	$sql_condition = '';

	if($uid != -1)//管理员查询所有人数据
 		$sql_condition .= " AND ur.`uid`=".$uid;
    if($type_id > 0){
    	$sql_condition .= " AND ei.`tid`=".$type_id;
    }

     $sql_condition .= "  ORDER BY ur.`time_end` DESC LIMIT ".$num;

    if(!empty($sql_condition))
            $sql .= $sql_condition;
     //echo $sql;

    if($uid>=-1){

	    $res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){

	         	if($type=='simple'){
	         		$arr[$row->result_id] = array("paper_id" => $row->paper_id,
						  "paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
						  "uid" => $row->uid,
						  "time_end"=>date("Y-m-d H:i:s",$row->time_end),
		    		);
	         	}

	         	if($type=='detail'){
		    		$arr[$row->result_id] = array("paper_id" => $row->paper_id,
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

   }

    // 	 $conn->disconnect();
    return $arr;

 }


/**
 * 查询任意用户的某次做卷记录（不含做题信息）
 *
 * @param unknown_type $id
 * @param unknown_type $type
 * @return unknown
 */

 static function query_user_results_by_result_id($type,$id){
 	$conn = DB_CONNECT::db_conn();

    if($id>0){
        $type= !empty($type)?$type:'simple';

   	  	$sql = "SELECT ur.`result_id`,ur.`uid`,ur.`paper_id`, ur.`time_used`,ur.`rate`,ur.`time_end`, ur.`score`, ur.`is_valid`,
   	  					ei.`objective_points`,ei.`paper_id`,ei.`points`,ei.`paper_name`,ei.`sid`,
   	  					et.`tid`, et.`name` exam_type
				FROM `user_results` ur
				LEFT JOIN `exam_info` ei ON ei.`paper_id`=ur.`paper_id`
   	  			LEFT JOIN `exam_type` et ON et.`tid`=ei.`tid`
				WHERE ur.`result_id`=".$id." AND ur.`is_valid`=1";

   	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		     $error_messsage = mysql_error();
		     self::halt($error_messsage,$sql);
	    }else{

	    	while($row = $res->fetchRow()){

	         	if($type=='simple'){

	         		$arr[$row->result_id] = array(
	         			/*
	         			  "paper_id" => $row->paper_id,
						  "paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
						  "uid" => $row->uid,
						  "time_end"=>date("Y-m-d H:i:s",$row->time_end),
						*/
						  "paper_id" => $row->paper_id,
						  "paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
						  "uid" => $row->uid,
						  "time_end"=>date("Y-m-d H:i:s",$row->time_end),
						  "time_used"=>$row->time_used,
						  "user_score"=>$row->score,
						  "objective_points"=>$row->objective_points,
						  "exam_points"=>$row->points,
	         				"tid"=>$row->tid,
	         				"exam_type"=>$row->exam_type
		    		);

	         	}

	         	if($type=='detail'){

	         		$comment = self::query_structure_comment($type,$row->sid, $row->rate);

	         		$comment_array = array();
	         		$comment_only_attr = 0;

	         		if($comment['count']>0)//本模板设置了评论等级
						$comment_array = $comment['comment'];

					else if($comment['count']==0 && $comment['global_comment']['count']>0){
						//本模板未设置评论等级，采用本模板所属分类评语等级或全局等级
						$comment_array = $comment['global_comment']['comment'];
						$comment_only_attr = $comment['global_comment']['only_attribute'];
					}

					if(!empty($comment_array)){

						foreach( $comment_array as $key => $value) {


							$level = $value['level'];
							if($comment_only_attr == 0){
								$resources = array(
									'resources' => $value['resources']['resources'],
									"resources_tags1" => self::getResources($value['resources']['resources_tags1']),
									"or_resources_tags2" => self::getResources($value['resources']['or_resources_tags2'])
								);
							}
						}
					}

		    		$arr[$row->result_id] = array(
		    			  "paper_id" => $row->paper_id,
						  "paper_name" => stripslashes(htmlspecialchars_decode($row->paper_name)),
						  "uid" => $row->uid,
						  "time_end"=>date("Y-m-d H:i:s",$row->time_end),
						  "time_used"=>$row->time_used,
						  "user_score"=>$row->score,
						  "objective_points"=>$row->objective_points,
						  "exam_points"=>$row->points,
		    				"tid"=>$row->tid,
		    				"exam_type"=>$row->exam_type,
						  "level" => $level,
						  "resources" =>$resources
					);
	         	}

	         	if($type=='structure_resources'){
	         		//返回此用户得分下的模板推荐资源，以此来限制考点资源。
	         		$comment = self::query_structure_comment('detail',$row->sid, $row->rate);
	         		$comment_array = array();

					if($comment['count']>0)//本模板设置了评论等级
						$comment_array = $comment['comment'];
					else if($comment['count']==0 && $comment['global_comment']['count']>0)
						$comment_array = $comment['global_comment']['comment'];

					if(!empty($comment_array)){
						foreach( $comment_array as $key => $value) {
							$resources = array(
								"resources_tags1" => $value['resources']['resources_tags1'],
								"or_resources_tags2" => $value['resources']['or_resources_tags2']
							);
						}
					}
		    		$arr["resources"] = $resources;
	         	}
/**/
			}
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;

 }



 /**
 * 查询任意用户的某次做卷记录，关于考点的诊断报告（不含做题信息）
 *
 * @param unknown_type $id
 * @param unknown_type $type
 * @return unknown
 */

 static function query_user_attribute_results_by_result_id($type,$id){


    if($id>0){
    	$structure_resources = self::query_user_results_by_result_id('structure_resources', $id);
    	//根据result_id获取此分数段的推荐资源标签，用于辅助考点资源的获取。
    	$s_resources_tags1 = $structure_resources['resources']['resources_tags1'];
    	$s_or_resources_tags2 = $structure_resources['resources']['or_resources_tags2'];

//echo '1::'.$s_resources_tags1.' 2::'.$s_or_resources_tags2.'<br><br>';

    	$conn = DB_CONNECT::db_conn();
		$attr_level = array();
        $type= !empty($type)?$type:'simple';

   	  	$sql = "SELECT ur.`result_id`,ur.`uid`,ur.`paper_id`, ur.`time_used`,ur.`rate`,ur.`time_end`, ur.`score`, ur.`is_valid`,ei.`objective_points`,ei.`paper_id`,ei.`points`,ei.`paper_name`,ei.`sid`
				FROM `user_results` ur
				LEFT JOIN `exam_info` ei
				ON ei.`paper_id`=ur.`paper_id`
				WHERE ur.`result_id`=".$id." AND ur.`is_valid`=1";

//echo $sql.'<br><br>'; whx here

   	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		     $error_messsage = mysql_error();
		     self::halt($error_messsage,$sql);
	    }else{
	    	while($row = $res->fetchRow()){

	    		$item_id_arr = array();

	    		$query_blank_filling_objective = "SELECT ei.item_id, ii.basic_type, bfa.is_objective
					FROM exam_item ei
					LEFT JOIN item_info ii ON ei.item_id = ii.item_id
					LEFT JOIN blank_filling_answers bfa ON ei.item_id = bfa.item_id
					WHERE ei.paper_id = ".$row->paper_id."
					AND (
						ii.basic_type =2 -- 填空题
						AND bfa.is_objective =1 -- 客观填空题
					)";

				$query_blank_filling_subjective = "SELECT ei.item_id, ii.basic_type, bfa.is_objective
					FROM exam_item ei
					LEFT JOIN item_info ii ON ei.item_id = ii.item_id
					LEFT JOIN blank_filling_answers bfa ON ei.item_id = bfa.item_id
					WHERE ei.paper_id = ".$row->paper_id."
					AND (
						ii.basic_type =2 -- 填空题
						AND bfa.is_objective =0 -- 主观填空题
					)";

	    		$query_multichoice = "SELECT ei.item_id, ii.basic_type, bfa.is_objective
					FROM exam_item ei
					LEFT JOIN item_info ii ON ei.item_id = ii.item_id
					LEFT JOIN blank_filling_answers bfa ON ei.item_id = bfa.item_id
					WHERE ei.paper_id =".$row->paper_id."
					AND ii.basic_type =1 -- 选择题 ";


	    		$rs_b = $conn->query($query_blank_filling_objective);
	    		$rs_b_s = $conn->query($query_blank_filling_subjective);//主观填空题
	    		$rs_m = $conn->query($query_multichoice);
			    if(DB::isError ($rs_b) || DB::isError ($rs_b_s)|| DB::isError ($rs_m)){
				     $error_messsage = mysql_error();
				     self::halt($error_messsage,$query_blank_filling_objective.$query_blank_filling_subjective.$query_multichoice);
			    }else{

					if($rs_b->numRows()>0){
						while($row_b = $rs_b->fetchRow()){
							$item_id_arr[] = $row_b->item_id;
						}
					}

					if($rs_b_s->numRows()>0){
						while($row_b_s = $rs_b_s->fetchRow()){
							$item_id_s_arr[] = $row_b_s->item_id;
						}
					}//主观题所有试题

					if($rs_m->numRows()>0){
						while($row_m = $rs_m->fetchRow()){
							$item_id_arr[] = $row_m->item_id;
						}
					}


				}


				if($item_id_arr!=null)
					$item_id_str =  implode(" OR `item_id` = ",$item_id_arr);


			//	$structure_aid = self::query_item_attribution_by_structure_id($row->sid);


				$items_aids = self::query_item_attribution_by_item_id_array($item_id_arr);

				$items_s_aids = self::query_item_attribution_by_item_id_array($item_id_s_arr);//主观题所在的考点（最终会去掉有客观题的考点）
				$items_s_aids = array_diff_key($items_s_aids,$items_aids);//去除客观题考点之后的主观题考点
//echo '<br>items_s_aids::'.var_export($items_s_aids,true).'<br>items_aids::'.var_export($items_aids,true).'<br><br>';

				//------客观题--------
				foreach ($items_aids as $attr_item)
				{
				   	$conn2 = DB_CONNECT::db_conn();

				   	$aid = $attr_item['aid'];
				   	$attr_name = $attr_item['name'];

				  	$sql_item_id_aid = "SELECT `item_id` from `item_attribute_hierarchy`
				  						WHERE `aid` =  ". $aid ."
										and ( `item_id` = ". $item_id_str ." )";

					$res_i = $conn2->query($sql_item_id_aid);
					$arr_i = array();

					if(DB::isError ($res_i)){
						 $error_messsage = mysql_error()."SELECT failed: " . $res_i->getMessage ();
						 self::halt($error_messsage,$sql_item_id_aid);
					}else{
						$item_count = mysql_affected_rows();
						if($item_count > 0){
						//whx here 可以设置此考点涉及到的题目个数
						//在函数开始处，可以设置这个个数限制。
							while($row_i = $res_i->fetchRow()){
								$arr_i[] = $row_i->item_id;
							}
						}
					}

//echo $sql_item_id_aid.' <br><br>'.var_export($arr_i,true).$item_count.'<br><br>';

					$user_sum_score = 0.0;
					$sql_total_sum_score = 0.0;

				  	if($arr_i!=null){
						$arr_i_str =  implode(" OR item_id = ",$arr_i);

						$sql_user_sum_score_m = "select sum(points_awarded) user_sum_score from multichoice_user_answer where result_id = ". $id ." and (item_id =" . $arr_i_str .")";

						$sql_user_sum_score_b = "select sum(points_awarded) user_sum_score from blank_filling_user_answer where result_id = ". $id ." and (item_id =" . $arr_i_str .")";


						$sql_total_sum_score = "select sum(points) total_sum_score from exam_item where paper_id = ". $row->paper_id ."  and (item_id =" . $arr_i_str .")";

						$res_user_sum_score_m = $conn2->query($sql_user_sum_score_m);
						$res_user_sum_score_b = $conn2->query($sql_user_sum_score_b);
						$res_total_sum_score = $conn2->query($sql_total_sum_score);
						if(DB::isError ($res_user_sum_score_m)||DB::isError ($res_user_sum_score_b)||DB::isError ($res_total_sum_score)){
							 $error_messsage = mysql_error();
							 self::halt($error_messsage,$sql_user_sum_score_m.'<br>'.$sql_user_sum_score_b.'<br>'.$sql_total_sum_score.'<br>');
						}else{
							$user_sum_score_m = 0.0;
							$user_sum_score_b = 0.0;

							if($row_user_sum_score_m = $res_user_sum_score_m->fetchRow()){
								$user_sum_score_m = $row_user_sum_score_m->user_sum_score;
							}
							if($row_user_sum_score_b = $res_user_sum_score_b->fetchRow()){
								$user_sum_score_b = $row_user_sum_score_b->user_sum_score;
							}

							$user_sum_score = $user_sum_score_m + $user_sum_score_b;

							if($row_total_sum_score = $res_total_sum_score->fetchRow()){
								$sql_total_sum_score = $row_total_sum_score->total_sum_score;
							}


						}
					}

					if($sql_total_sum_score!=0)
						$rate = $user_sum_score/$sql_total_sum_score;
					else
						$rate = number_format($rate,2,'.','');

					$conn2->disconnect();

					//echo $rate.'<br><br>';

	         		$comment = self::query_structure_attribution_comment("detail", $row->sid, $aid, $rate);


    				$comment_array = array();
	         		$comment_only_attr = 0;

	         		if($comment['count']>0){//本模板考点设置了评论等级
						$comment_array = $comment['comment'];
						//echo '本模板考点设置了评论等级:<br>';
					}
					else if($comment['count']==0 && $comment['global_comment']['count']>0){
						//本模板未设置考点评论等级，采用考点自己的评语等级或全局等级
						$comment_array = $comment['global_comment']['comment'];
						//echo '本模板未设置考点评论等级，采用考点自己的评语等级或全局等级:<br>';
					}
		//echo 'type:'.$type.' sid: '.$row->sid.' aid: '.$aid.' user_sum_score:: '.$user_sum_score.' sum'.$sql_total_sum_score.' rate:'.$rate.'<br>'.var_export($comment,true).'<br><br>'.var_export($comment['global_comment'],true).'<br><br><br>';


					if(!empty($comment_array)){
						foreach( $comment_array as $key => $value) {
							$resources_tags1 = $value['resources']['resources_tags1'];
							$or_resources_tags1 = $value['resources']['or_resources_tags2'];

							$resources1 = '';$resources2 = '';$resources3 = '';$resources4 = '';
//echo 'resources_tags1::'.$resources_tags1.' or_resources_tags1::'.$or_resources_tags1.'<br><br>';

							if(!empty($resources_tags1)&&$resources_tags1!=''){

								if(!empty($s_resources_tags1)&&$s_resources_tags1!='')
									$resources1 = $resources_tags1 .','.$s_resources_tags1;

								if(!empty($s_or_resources_tags2)&&$s_or_resources_tags2!='')
									$resources2 = $resources_tags1 .','.$s_or_resources_tags2;
							}
							if(!empty($or_resources_tags1)&&$or_resources_tags1!=''){

								if(!empty($s_resources_tags1)&&$s_resources_tags1!='')
									$resources3 .= $or_resources_tags1 .','.$s_resources_tags1;

								if(!empty($s_or_resources_tags2)&&$s_or_resources_tags2!='')
									$resources4 .= $or_resources_tags1 .','.$s_or_resources_tags2;
							}
//echo 'resources1::'.$resources1.' resources2::'.$resources2.' resources3::'.$resources3.' resources4::'.$resources4.'<br><br>';

							$level = $value['level'];
							$resources = array(
								'resources' => $value['resources']['resources'],
								"resources1" => self::getResources($resources1),
								"resources2" => self::getResources($resources2),
								"resources3" => self::getResources($resources3),
								"resources4" => self::getResources($resources4)
							);

						}
					}
					else{
						$level = null;
						$resources = null;
					}

					if($type=='detail'){
						$attr_level[] = array(
							"aid" => $aid,
							"attr_name" => $attr_name,
							"user_score" => $user_sum_score,
							"total_score" => $sql_total_sum_score,
							"rate" => $rate,
							"level" => $level,
							"resources" =>$resources
						);
					}

					if($type=='simple'){
						$attr_level[] = array(
							"aid" => $aid,
							"attr_name" => $attr_name,
							"rate" => $rate,
							"level" => $level
						);
					}

	         	}	//---------客观题 end-----------


	    		//------主观题--------
				foreach ($items_s_aids as $attr_s_item)
				{

				   	$aid = $attr_s_item['aid'];
				   	$attr_name = $attr_s_item['name'];


	         		$comment = self::query_structure_attribution_comment("detail", $row->sid, $aid, '');


    				$comment_array = array();
	         		$comment_only_attr = 0;

	         		if($comment['count']>0){//本模板考点设置了评论等级
						$comment_array = $comment['comment'];
						//echo '本模板考点设置了评论等级:<br>';
					}
					else if($comment['count']==0 && $comment['global_comment']['count']>0){
						//本模板未设置考点评论等级，采用考点自己的评语等级或全局等级
						$comment_array = $comment['global_comment']['comment'];
						//echo '本模板未设置考点评论等级，采用考点自己的评语等级或全局等级:<br>';
					}
		//echo 'type:'.$type.' sid: '.$row->sid.' aid: '.$aid.' user_sum_score:: '.$user_sum_score.' sum'.$sql_total_sum_score.' rate:'.$rate.'<br>'.var_export($comment,true).'<br><br>'.var_export($comment['global_comment'],true).'<br><br><br>';


					if(!empty($comment_array)){
						foreach( $comment_array as $key => $value) {
							$resources_tags1 = $value['resources']['resources_tags1'];
							$or_resources_tags1 = $value['resources']['or_resources_tags2'];

							$resources1 = '';$resources2 = '';$resources3 = '';$resources4 = '';
//echo 'resources_tags1::'.$resources_tags1.' or_resources_tags1::'.$or_resources_tags1.'<br><br>';

							if(!empty($resources_tags1)&&$resources_tags1!=''){

								if(!empty($s_resources_tags1)&&$s_resources_tags1!='')
									$resources1 = $resources_tags1 .','.$s_resources_tags1;

								if(!empty($s_or_resources_tags2)&&$s_or_resources_tags2!='')
									$resources2 = $resources_tags1 .','.$s_or_resources_tags2;
							}
							if(!empty($or_resources_tags1)&&$or_resources_tags1!=''){

								if(!empty($s_resources_tags1)&&$s_resources_tags1!='')
									$resources3 .= $or_resources_tags1 .','.$s_resources_tags1;

								if(!empty($s_or_resources_tags2)&&$s_or_resources_tags2!='')
									$resources4 .= $or_resources_tags1 .','.$s_or_resources_tags2;
							}
//echo 'resources1::'.$resources1.' resources2::'.$resources2.' resources3::'.$resources3.' resources4::'.$resources4.'<br><br>';

							$level = $value['level'];
							$resources = array(
								'resources' => $value['resources']['resources'],
								"resources1" => self::getResources($resources1),
								"resources2" => self::getResources($resources2),
								"resources3" => self::getResources($resources3),
								"resources4" => self::getResources($resources4)
							);

						}
					}
					else{
						$level = null;
						$resources = null;
					}

					if($type=='detail'){
						$attr_level[] = array(
							"aid" => $aid,
							"attr_name" => $attr_name,
							"level" => $level,
							"resources" =>$resources
						);
					}

					if($type=='simple'){
						$attr_level[] = array(
							"aid" => $aid,
							"attr_name" => $attr_name,
							"level" => $level
						);
					}

	         	}//--------主观题 end-----------


			}
		}

   }

    // 	 $conn->disconnect();
    return $attr_level;

 }




static function query_multichoice_user_answers_by_result_id($result_id,$item_id){
 	$conn = DB_CONNECT::db_conn();
    if($item_id>0){

    	$is_correct = '';
  	  	$sql = "SELECT `user_answer_id` answer_id, ma.`item_id`, `choice_multi`,`result_id`, `multi_answer_id`, `points_awarded`
  	  	        FROM `multichoice_user_answer` ma
  	  	        LEFT JOIN `multichoice_properties` mp ON ma.item_id = mp.item_id
  	  	        WHERE `result_id`=".$result_id ." AND ma.`item_id`=".$item_id." LIMIT 1";

  	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	if($res->numRows()>0){
		    	while($row = $res->fetchRow()){

		    		  if($row->points_awarded>0)
	                       $is_correct = 1;
	                  else
		    		       $is_correct = 0;


	                    $arr = array("answer_id" =>$row->answer_id,
		         		             "user_answer" => $row->multi_answer_id, //未处理多选题（以-分割）
		         		             "points_awarded" => $row->points_awarded,
		         		             "user_answer_is_correct" => $is_correct
			    		);
				   }
		    }else{
	    	   $arr = array("user_answer_is_correct" => -1);
	        }
	    }
   }

    // 	 $conn->disconnect();
    return $arr;

 }

static function query_blankfilling_user_answers_by_result_id($result_id,$item_id){
 	$conn = DB_CONNECT::db_conn();
    if($item_id>0){
  	  	$sql = "SELECT `answer_id`, `item_id`, `result_id`, `user_answer`, `points_awarded`
  	  	        FROM `blank_filling_user_answer` WHERE `result_id`=".$result_id ." AND `item_id`=".$item_id;
//echo $sql."<br>";
  	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	if($res->numRows()>0){
	    	while($row = $res->fetchRow()){
	    		 if($row->points_awarded>0)
                       $is_correct = 1;
                 else
	    		       $is_correct = 0;

	    		 $arr = array("answer_id" =>$row->answer_id,
	         		          "user_answer" => stripslashes(htmlspecialchars_decode($row->user_answer)), //未处理多选题（以-分割）
	         		          "points_awarded" => $row->points_awarded,
	         		          "user_answer_is_correct" => $is_correct
			    		);
			     }
	    	 }else{
	    	       $arr = array("user_answer_is_correct" => -1);
	       }
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;

 }



static function query_user_result_detail_points_by_result_id($result_id,$item_id){
 	$conn = DB_CONNECT::db_conn();
    if($item_id>0){
  	  	$sql = "SELECT `score`
  	  	        FROM `user_result_detail_points`
  	  	        WHERE `result_id`=".$result_id ." AND `item_id`=".$item_id." LIMIT 1";
//echo $sql."<br>";
  	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	if($res->numRows()>0){
	    	      $row = $res->fetchRow();
	    		  $arr = array("score" =>$row->score
		    		);
	    	}else{
	    		  $arr = array("score" =>0
		    		);
	    	}

	  	}

   }

    // 	 $conn->disconnect();
    return $arr;

 }

 /**
  * 用户各部分得分
  *
  * @param int $result_id 用户记录id
  * @return array
  */

static function query_user_result_each_part_points($result_id){

 	     $conn = DB_CONNECT::db_conn();
 	     $sql = "SELECT ei.exam_item_id,ei.item_id, uds.score, ei.serial_str,ei.parent_id,ei.serial_num, ii.name
				FROM exam_item ei
				LEFT JOIN item_info ii ON ii.item_id = ei.item_id
				LEFT JOIN `user_results` us ON us.paper_id = ei.paper_id
				LEFT JOIN `user_result_detail_points` uds ON ei.item_id = uds.item_id
				AND uds.result_id = us.result_id
				WHERE us.result_id =".$result_id."
				AND (ei.parent_id =0 OR ei.parent_id
				IN (
				SELECT exam_item_id
				FROM exam_item
				WHERE paper_id = ei.paper_id
				AND parent_id =0
				)
				)
				AND ii.basic_type=3 ORDER BY ei.parent_id,ei.serial_num";


  	  	$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		   	$error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
			if($res->numRows()>0){
			  while($row = $res->fetchRow()){
				 if($row->parent_id==0) {

				  $serial_num = '';
				  $score = '';
				  $serial_arr = explode("-",$row->serial_num);
				  $serial_num = $serial_arr[1];
				  $score = $row->score>0?$row->score:0;
				  $arr[$row->item_id] =
				  	array("name" => $row->name,
						  "score" => $score,
						  "serial_str" => $row->serial_str,
						  "serial_num" => $serial_num,
					);

				   $id_index[] = $row->exam_item_id;
				   $arr_index[$row->exam_item_id] = $row->item_id;

			   }elseif(in_array($row->parent_id,$id_index)){
				   $serial_num = '';
				   $score = '';
				   $serial_arr = explode("-",$row->serial_num);
				   $serial_num = $serial_arr[1];
				   $score = $row->score>0?$row->score:0;
				   $arr[$arr_index[$row->parent_id]]['subque'][$row->item_id] =   array("name" => $row->name,
						 "score" => $score,
						"serial_str"=>$row->serial_str,
						"serial_num" => $serial_num,
					);


				}



			  }
			}else{
	    		  $arr = array("score" =>0
		    		);
	    	}

	  	}


    // 	 $conn->disconnect();
    return $arr;

 }



static function  query_user_result_and_summary_info($result_id){

 	$arr = array();
 	$exam_info = self::query_user_results_by_result_id('detail', $result_id);
 	//$exam_attribute = self::query_user_attribute_results_by_result_id('detail', $result_id);
	$result_info = self::query_user_result_each_part_points($result_id);
	$arr = array(
		"exam_info" => $exam_info[$result_id],
	//	"exam_attribute" => $exam_attribute,
	    "result_info"=>$result_info
	);

 	return $arr;
 }


static function  query_user_diagnostic_report_by_result_id($result_id){

 	$arr = array();
 	$exam_info = self::query_user_results_by_result_id('detail', $result_id);
 	//$result_info = self::query_user_result_each_part_points($result_id);
 	//$exam_attribute = self::query_user_attribute_results_by_result_id('detail', $result_id);

	$arr = array(
		"exam_info" => $exam_info[$result_id]
		//"result_info"=>$result_info,
		//"exam_attribute" => $exam_attribute
	);

 	return $arr;
 }




static function  query_user_result_paper_items_by_result_id($result_id){

	$conn = DB_CONNECT::db_conn();
    $sql = "SELECT `exam_item_id`,ur.`uid`,ei.`paper_id`, `parent_id`, `item_id`, `points`, `time_limit`, `serial_str`, `serial_num`, `is_submit`, `status`
            FROM `user_results` ur
            LEFT JOIN `exam_item` ei on ei.`paper_id`=ur.`paper_id`
            WHERE ur.`result_id`=".$result_id." ORDER BY `parent_id`,`serial_num`,`exam_item_id`";

     $res = $conn->query($sql);
     if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
	 }else{
	 	$i = 0;
	 	$j = 0;
		while($row = $res->fetchRow()){

			  $item = '';
			  $user_answer = '';
			if($i==0){
			  $output['paper_info'] = self::query_paper_info_by_paper_id('detail',$row->paper_id,'');
			  $output['user_results_info'] = self::query_user_results_by_result_id('simple',$result_id);

			  //$output['user_results_info'] = self::query_user_results_by_result_id('detail',$result_id);

			 // $output['exam_attribute'] = self::query_user_attribute_results_by_result_id('detail',$result_id);

			}
			  $item = self::query_item_info('paper',$row->item_id,'',"by_item");

			  foreach ($item[$row->item_id]['basic_type'] as $basic_type_key=>$basic_type_value){

				  if($basic_type_key==1){   //选择题

					  $user_answer = self::query_multichoice_user_answers_by_result_id($result_id,$row->item_id);

				  }elseif($basic_type_key==2){   //填空题

				 //	echo $row->item_id."<br>";
					  $user_answer = self::query_blankfilling_user_answers_by_result_id($result_id,$row->item_id);
				  }else{
					//加入各部分得分

					  $user_answer = self::query_user_result_detail_points_by_result_id($result_id,$row->item_id);

				  }



			  }

			  if(strpos($row->serial_num,"-")){

				$serial_arr = explode("-",$row->serial_num);
				$serial_num = $serial_arr[1];

			  }else{

				$row->serial_num = $serial_num;

			  }

			  $arr[$i] = array("item_id" => $row->item_id,
								 "paper_id" => $row->paper_id,
								  "points" => $row->points,
								  "time_limit" => $row->time_limit,
								  "serial_str" => $row->serial_str,
								  "serial_num" => $serial_num,
								   "is_submit" => $row->is_submit
						  );

			  if($item[$row->item_id]!=''){

				  $arr[$i] = array_merge($arr[$i],$item[$row->item_id]);

			  }

			  if($user_answer!=''){

				  $arr[$i] = array_merge($arr[$i],$user_answer);

			  }



			  if($row->parent_id == 0){

				  $output['item'][$j] = $arr[$i];
				  $key[] = $row->exam_item_id;
				  $root[$row->exam_item_id] = $j;
				  $id[$row->exam_item_id] = 1;
				  $j++;

			  }elseif(in_array($row->parent_id,$key)){
				//使用引用的方法生产多维数组
				$relation[$row->exam_item_id] = $i;
				if($id[$row->parent_id]==1){

					$temp[$i] = &$output['item'][$root[$row->parent_id]]['subque'][];

				}else{

					$temp[$i] = &$temp[$relation[$row->parent_id]]['subque'][];

				}

				  $temp[$i] = $arr[$i];
				  $relation[$row->exam_item_id] = $i;
				  $key[] = $row->exam_item_id;

			  }

             $i++;

		}

	 }

     // 	 $conn->disconnect();

     return $output;

 }




 /**
  * 查询排行榜
  *
  *  $tid  考试分类
  *  $num   查询记录条数
  * @return unknown
  */

 static function query_user_rate_by_tid($tid,$num = 10){
	 $num = $num>0?$num:10;
	 $conn = DB_CONNECT::db_conn();

 	if($tid>0){


 		$uid_arr = array();
		$username_arr = '';
		/*
		$sql_user = "SELECT  u.`uid`
			FROM `user_results` u
			LEFT JOIN `exam_info` e ON e.`paper_id` = u.`paper_id`
			WHERE e.`tid` =".$tid." AND u.`is_valid`=1
			ORDER BY `rate` DESC LIMIT ".$num;

		$result_user = $conn->query($sql_user);
		while($row = $result_user->fetchRow()){
			$uid_arr[] = $row->uid;
		}
		*/
		$sql = "SELECT distinct u.`rate`,u.`uid`,u.`score`
				FROM `user_results` u
				LEFT JOIN `exam_info` e ON e.`paper_id` = u.`paper_id`
				WHERE e.`tid` =".$tid." AND u.`is_valid`=1
				ORDER BY `rate` DESC LIMIT ".$num;
		$res = $conn->query($sql);
		 if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	if($res->numRows()>0){
				while($row = $res->fetchRow()){
					$uid_arr[] = $row->uid;
				}
			}
		}
		if($uid_arr!=null)
			$uid_array =  implode(",",$uid_arr);
		$username_arr = self::query_username_by_uid_array($uid_array);

		/*
		$sql = "SELECT distinct u.`rate`,u.`uid`,u.`score`
				FROM `user_results` u
				LEFT JOIN `exam_info` e ON e.`paper_id` = u.`paper_id`
				WHERE e.`tid` =".$tid." AND u.`is_valid`=1
				ORDER BY `rate` DESC LIMIT ".$num;


			*/
		//echo $sql."<br>";
		$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	if($res->numRows()>0){
	    		while($row = $res->fetchRow()){
		    		if($row->uid>0){

	    			    $user = '';
						//$user[$row->uid] = array_search($row->uid,$username_arr);
							$user[$row->uid] = $username_arr['uid_'.$row->uid];

					 }
	    		    $arr['listcontent'][] = array(
	    		    		"rate" =>$row->rate,
	         		        "score" => $row->score, //未处理多选题（以-分割）
	    		            "info" => $user
	    		    	);

	    		}
	    		  $arr['status'] = 1;
	    	 }else{
	    		  $arr['status'] = 0;
	    	}

	  	}



 	}
 	else if($tid == 0){

 		$arr = array();

	 	//$term_arr = query_exam_type("simple",'','');

	 	$term_arr = self::query_exam_type('simple','','');
	 	//echo($term_arr[0]['tid']);

	 	foreach($term_arr as $index => $term){
	 		$term_id = $term['tid'];
	 		$term_name = $term['name'];

	 		$arr[$index]['term'] = $term;
	 		$temp_arr = array();

	 		$conn = DB_CONNECT::db_conn();

	 		$uid_arr = array();
	 		$username_arr = array();
	 		/*
	 		$sql_user = "SELECT u.`uid`
				FROM `user_results` u
				LEFT JOIN `exam_info` e ON e.`paper_id` = u.`paper_id`
				WHERE e.`tid` =".$term_id." AND u.`is_valid`=1
				ORDER BY `rate` DESC LIMIT ".$num;
			*/
			$sql = "SELECT distinct u.`rate`,u.`uid`,u.`score`
				FROM `user_results` u
				LEFT JOIN `exam_info` e ON e.`paper_id` = u.`paper_id`
				WHERE e.`tid` =".$term_id." AND u.`is_valid`=1
				ORDER BY `rate` DESC LIMIT ".$num;



	 		$res = $conn->query($sql);
			if(DB::isError ($res)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$sql);
			}else{
				if($res->numRows()>0){
					while($row = $res->fetchRow()){
						$uid_arr[] = $row->uid;
					}
				}
			}

			if($uid_arr!=null)
				$uid_array =  implode(",",$uid_arr);
	 		$username_arr = self::query_username_by_uid_array($uid_array);
	 		/*
	 		$sql = "SELECT u.`rate`,u.`uid`,u.`score`,u.`time_end`
				FROM `user_results` u
				LEFT JOIN `exam_info` e ON e.`paper_id` = u.`paper_id`
				WHERE e.`tid` =".$term_id." AND u.`is_valid`=1
				ORDER BY `rate` DESC LIMIT ".$num;

			*/
			$result = $conn->query($sql);


			if(DB::isError ($result)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$sql);
			}else{
				if($result->numRows()>0){
					while($row = $result->fetchRow()){
						if($row->uid>0){
							$user = '';
							//$user[$row->uid] = array_search($row->uid,$username_arr);
							$user[$row->uid] = $username_arr['uid_'.$row->uid];
						}
						$temp_arr[] = array(
								"rate" =>$row->rate,
								"score" => $row->score, //未处理多选题（以-分割）
								"info" => $user
							);

					}
					  $arr[$index]['listcontent'] = $temp_arr;
					  $arr[$index]['status'] = 1;
				 }else{
					  $arr[$index]['status'] = 0;
				}

			}

	 	}


 	}
 	else{
	   $arr['status'] = 0;
 	}

 		// 	 $conn->disconnect();
 	   return $arr;
 }


 static function query_username_by_uid_array($uid_array){

	$arr = array();
	$users = '';
	$url = "http://2u4u.com.cn/query/username/aa/".$uid_array;
	//$url = "http://2u4u.com.cn/query/username/aa/1,5,18561";
	$content = file_get_contents($url);
	$info = json_decode($content);

	if($info->status=='Success'){
		$users = $info->user_info;
	}

	$arr = (array)$users;

 	return $arr;
 }



 static function  query_user_rate_trend_by_tid($uid,$tid,$num = 10){

 	if($uid>0){
	 	$tid = $tid>0?$tid:0;
	 	$num = $num>0?$num:10;
	 	$conn = DB_CONNECT::db_conn();

		$sql = "SELECT u.`rate`,u.`uid`,u.`score`,u.`time_end`
				FROM `user_results` u
				LEFT JOIN `exam_info` e ON e.`paper_id` = u.`paper_id`
				WHERE u.`uid` =".$uid;

		if($tid>0){
			$sql .= " AND e.`tid`=".$tid;
		}

		$sql .= " AND u.`is_valid`=1	ORDER BY `time_end` LIMIT ".$num;


		//echo $sql."<br>";
		$res = $conn->query($sql);
	    $arr = array();
	    if(DB::isError ($res)){
		    $error_messsage = mysql_error();
		    self::halt($error_messsage,$sql);
	    }else{
	    	if($res->numRows()>0){
	    		if($uid>0){
    			    $user = '';
					$url = "http://2u4u.com.cn/query/username/aa/".$uid;
					$content = file_get_contents($url);
					$info = COMMON::my_json_decode($content);
					if($info->status=='Success'){
						$user = $info->user_info;
						$arr['info'] = $user;
					}
				 }
	    		while($row = $res->fetchRow()){
	    		    $arr['list'][] = array("rate" =>$row->rate,
	         		          "score" => $row->score, //未处理多选题（以-分割）
	         		          "time_end" => date("Y-m-d H:i:s",$row->time_end)
	    		    	);
	    		}
	    		  $arr['status'] = 1;
	    	 }else{
	    		  $arr['status'] = 0;
	    	}

	  	}

	   // 	 $conn->disconnect();

 	}else{
	   $arr['status'] = 0;
 	}
 	   return $arr;
 }



 static function query_active_users($num){
 	$conn = DB_CONNECT::db_conn();

 	$num = $num>0?$num:10;

 	$uid_arr = array();
	$username_arr = array();
	$sql_user = "CREATE or REPLACE view active_users
					AS SELECT `uid` , count( `paper_id` ) num
						FROM `exam_info`
						WHERE `uid` >0
						GROUP BY uid
						ORDER BY num DESC
						LIMIT ".$num;
	 $conn->query($sql_user);

	$users = " SELECT * FROM `active_users`";
	$result_user =  $conn->query($users);

	while($row = $result_user->fetchRow()){
		$uid_arr[] = $row->uid;
	}

	if($uid_arr!=null)
		$uid_array =  implode(",",$uid_arr);
	$username_arr = self::query_username_by_uid_array($uid_array);

	$sql = "SELECT  u.`num`, e.`uid` , `paper_id` , `paper_name`
			FROM `exam_info` e , `active_users` u
			WHERE e.`uid` >0
			AND u.uid = e.`uid`
			AND NOT
			EXISTS (
				SELECT *
				FROM `exam_info`
				WHERE paper_id > e.paper_id
				AND uid = e.uid
			)
			ORDER BY num DESC";


 	$res = $conn->query($sql);
	$arr = array();
	if(DB::isError ($res)){
		 $error_messsage = mysql_error();
		 self::halt($error_messsage,$sql);
	}else{

		while($row = $res->fetchRow()){

			$user = '';
			if($row->uid>0){

				$user[$row->uid] = $username_arr['uid_'.$row->uid];
			}

			$arr[] = array("num" => $row->num,
			               "info" => $user,
			               "paper_id" => $row->paper_id,
			               "paper_name" => $row->paper_name
			);
		}
	}
 	return $arr;
 }


 static function query_paper_count_by_type_and_name($type_id, $name, $time_start, $time_end, $sortBy){
 	$conn = DB_CONNECT::db_conn();

 	$arr = array();
 	$type_id = $type_id>0?$type_id:0;
	$name =!empty($name)?$name:'';
	$time_start =!empty($time_start)?$time_start:'';
	$time_end =!empty($time_end)?$time_end:'';

	$sql_condition = '';

	if($type_id>0)
		$sql_condition .= " AND `tid`=".$type_id;
	if(!empty($name))
		$sql_condition .= " AND `title` LIKE '%".$name."%'";

  	$arr['total'] = self::tongji_total_papers_by_type_and_name($type_id, $name, 0, $time_start, $time_end);


	$sql = "SELECT * FROM `exam_structure_info` WHERE `status`=1 ";
	if(!empty($sql_condition))
		$sql .= $sql_condition;

	$res = $conn->query($sql);
	if(DB::isError ($res)||DB::isError ($res_count)){
	  $error_messsage = mysql_error();
	  self::halt($error_messsage,$sql);
	}else{
		while($row = $res->fetchRow()){
			$mytotal = self::tongji_total_papers_by_type_and_name(0, '', $row->sid, $time_start, $time_end);

			if($mytotal['has_effected_paper'] != 0 ){

				$has_effected_paper =  $mytotal['has_effected_paper'];
				$paper_count = $mytotal['paper_count'];
				$paper_visited_times =  $mytotal['paper_visited_times'];
				$do_paper = $mytotal['do_paper'];
				$do_visited_ratio = $mytotal['do_visited_ratio'];
				$ave_score =  $mytotal['ave_score'];
				$ave_rate = $mytotal['ave_rate'];

				$arr['detail'][] = array(
						"sid" => $row->sid,
						"tid" => $row->tid,
						"title" => $row->title,
						"has_effected_paper" => $has_effected_paper,
						"paper_count" => $paper_count,
						"paper_visited_times" => $paper_visited_times,
						"do_paper" => $do_paper,
						"do_visited_ratio" => $do_visited_ratio,
						"ave_score" => $ave_score,
						"ave_rate" => $ave_rate
				);
			}
		}
		//排序sort whx here
		usort($arr['detail'], "self::".$sortBy);
	}

    // 	 $conn->disconnect();
    return $arr;
}

//----专门为统计的排序功能写的排序函数，按访问次数升降序、生成次数升降序、提交次数升降序、平均成绩、平均正确率、模板名升降序
function visited_desc($a, $b)
{	//访问次数 降序
  if ($a['paper_visited_times'] == $b['paper_visited_times']) return 0;
  return ($a['paper_visited_times'] > $b['paper_visited_times']) ? -1 : 1;
}
function visited_asc($a, $b)
{	//访问次数 升序
  if ($a['paper_visited_times'] == $b['paper_visited_times']) return 0;
  return ($a['paper_visited_times'] > $b['paper_visited_times']) ? 1 : -1;
}
function created_desc($a, $b)
{	//生成次数 降序
  if ($a['paper_count'] == $b['paper_count']) return 0;
  return ($a['paper_count'] > $b['paper_count']) ? -1 : 1;
}
function created_asc($a, $b)
{	//生成次数 升序
  if ($a['paper_count'] == $b['paper_count']) return 0;
  return ($a['paper_count'] > $b['paper_count']) ? 1 : -1;
}
function do_desc($a, $b)
{	//提交次数 降序
  if ($a['do_paper'] == $b['do_paper']) return 0;
  return ($a['do_paper'] > $b['do_paper']) ? -1 : 1;
}
function do_asc($a, $b)
{	//提交次数 升序
  if ($a['do_paper'] == $b['do_paper']) return 0;
  return ($a['do_paper'] > $b['do_paper']) ? 1 : -1;
}
function do_visited_desc($a, $b)
{	//提交/点击比率 降序
  if ($a['do_visited_ratio'] == $b['do_visited_ratio']) return 0;
  return ($a['do_visited_ratio'] > $b['do_visited_ratio']) ? -1 : 1;
}
function do_visited_asc($a, $b)
{	//提交/点击比率 升序
  if ($a['do_visited_ratio'] == $b['do_visited_ratio']) return 0;
  return ($a['do_visited_ratio'] > $b['do_visited_ratio']) ? 1 : -1;
}

function ave_score_desc($a, $b)
{	//平均成绩 降序
  if ($a['ave_score'] == $b['ave_score']) return 0;
  return ($a['ave_score'] > $b['ave_score']) ? -1 : 1;
}
function ave_score_asc($a, $b)
{	//平均成绩 升序
  if ($a['ave_score'] == $b['ave_score']) return 0;
  return ($a['ave_score'] > $b['ave_score']) ? 1 : -1;
}
function ave_rate_desc($a, $b)
{	//平均正确率 降序
  if ($a['ave_rate'] == $b['ave_rate']) return 0;
  return ($a['ave_rate'] > $b['ave_rate']) ? -1 : 1;
}
function ave_rate_asc($a, $b)
{	//平均正确率 升序
  if ($a['ave_rate'] == $b['ave_rate']) return 0;
  return ($a['ave_rate'] > $b['ave_rate']) ? 1 : -1;
}
function title_desc($a, $b)
{	//模板标题 降序
  if ($a['title'] == $b['title']) return 0;
  return ($a['title'] > $b['title']) ? -1 : 1;
}
function title_asc($a, $b)
{	//模板标题 升序
  if ($a['title'] == $b['title']) return 0;
  return ($a['title'] > $b['title']) ? 1 : -1;
}





 static function tongji_total_papers_by_type_and_name($type_id, $name, $id, $time_start, $time_end){
 	$conn = DB_CONNECT::db_conn();

 	$arr = array();
 	$type_id = $type_id>0?$type_id:0;
	$name =!empty($name)?$name:'';
	$id = $id>0?$id:0;
	$time_start =!empty($time_start)?$time_start.' 0:0:0':'';
	$time_end =!empty($time_end)?$time_end.' 23:59:59':'';


	$sql_condition = '';
	$time_condition = '';

	if($type_id>0)
		$sql_condition .= " AND `tid`=".$type_id;
	if(!empty($name))
		$sql_condition .= " AND `paper_name` LIKE '%".$name."%'";
	if($id>0)
    	$sql_condition .= " AND `sid` =".$id;
   	if(!empty($time_start))
		$time_condition .= " AND  `created` >= unix_timestamp('".$time_start."')";
	if(!empty($time_end))
		$time_condition .= " AND `created` < unix_timestamp('".$time_end."')";

  	$sql = "SELECT `paper_id` FROM `exam_info` Where `status` = 1 ";
	if(!empty($sql_condition))
    	$sql .= $sql_condition;//根据除时间条件外的其他条件获取所有的paper_id,为其他“访问次数”、“提交次数”提供数据，或者对没有时间限制的“生成试卷次数”的sql。
  	if(!empty($time_condition))
    	$sql_time =$sql.$time_condition;//根据时间限制获取“生成试卷次数”。

   //echo 'sql_1::'.$sql_1;

   	$arr['has_effected_paper'] = 0;

  	$arr['paper_count'] = 0;
   	if(isset($sql_time)){
		$res_time = $conn->query($sql_time);

		if(DB::isError ($res_time)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql_time);
		}else{

			if($rc = mysql_affected_rows()){
				$arr['paper_count'] = $rc;
			}
		}
	}

	$res1 = $conn->query($sql);
	if(DB::isError ($res1)){
		$error_messsage = mysql_error();
		self::halt($error_messsage,$sql);
	}else{

		if($rc = mysql_affected_rows()){
			if(!isset($sql_time))	$arr['paper_count'] = $rc;

			$paper_ids_array = array();

			while($row1 = $res1->fetchRow()){
				$paper_ids_array[] = $row1->paper_id;
			}

			$paper_ids =  "'".implode("','",$paper_ids_array)."'";


			//---点击次数 统计
			$sql_user_visited_time_condition = '';
			if(!empty($time_start))
				$sql_user_visited_time_condition .= " AND  `visited` >= unix_timestamp('".$time_start."')";
			if(!empty($time_end))
				$sql_user_visited_time_condition .= " AND `visited` < unix_timestamp('".$time_end."')";

			$sql_user_visited = "SELECT count(paper_id) count FROM exam_info_visited WHERE paper_id IN(".$paper_ids.")";

			if(!empty($sql_user_visited_time_condition))
				$sql_user_visited .= $sql_user_visited_time_condition;

			//echo $sql_user_visited.'<br><br>';

			$res_user_visited = $conn->query($sql_user_visited);

			if(DB::isError ($res_user_visited)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$sql_user_visited);
			}else{
				if($row_visited = $res_user_visited->fetchRow()){
					$arr['paper_visited_times'] = $row_visited->count;
				}
				else
					$arr['paper_visited_times'] = 0;

			}//---end 点击次数 统计


			//---提交试题次数、平均分、平均正确率 统计
			$sql_user_do_time_condition = '';
			if(!empty($time_start))
				$sql_user_do_time_condition .= " AND  `time_end` >= unix_timestamp('".$time_start."')";
			if(!empty($time_end))
				$sql_user_do_time_condition .= " AND `time_end` < unix_timestamp('".$time_end."')";

			$sql_user_do = "SELECT count(paper_id) count, sum(score) sum_score,sum(rate) sum_rate  FROM user_results WHERE paper_id IN(".$paper_ids.")";

			if(!empty($sql_user_do_time_condition))
				$sql_user_do .= $sql_user_do_time_condition;

			$res_user_do = $conn->query($sql_user_do);
			if(DB::isError ($res_user_do)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$sql_user_do);
			}else{

				if($row = $res_user_do->fetchRow()){
					$arr['do_paper'] = $row->count;
					if($arr['do_paper']==0){
						$arr['ave_score'] = 0;
						$arr['ave_rate'] = 0;
					}
					else{
						$arr['ave_score'] = ($row->sum_score/$arr['do_paper']);
						$arr['ave_rate'] = ($row->sum_rate/$arr['do_paper']);
					}
				}
				else
					$arr['do_paper'] = 0;

			}//---end 提交试题次数、平均分、平均正确率 统计

			$arr['do_visited_ratio'] = $arr['do_paper']/$arr['paper_visited_times'];

		}


	}

	if($arr['paper_count']!=0||$arr['paper_visited_times']!=0||$arr['do_paper']!=0)	$arr['has_effected_paper'] = 1;

	return $arr;

}


 static function query_structure_comment($type, $id, $user_rate){
 	$conn = DB_CONNECT::db_conn();
 	$id = $id>0?$id:0;

    if($id>0){
 		$type= !empty($type)?$type:'simple';

   	  	$sql = "SELECT scid, es.level_id, level_name, description, min_rate, max_rate, resources, resources_tags1, or_resources_tags2
				FROM `exam_structure_comment` es
				LEFT JOIN  `exam_comments_level` ecl ON es.level_id = ecl.level_id
				LEFT JOIN  `exam_comments` ec ON es.level_id = ec.level_id
				WHERE es.`sid` =" . $id;

		$sql_condition = " ORDER BY es.`scid`";

		if($user_rate!==''){
			$sql_condition = " AND es.`min_rate`<=".($user_rate*100)."
						AND es.`max_rate`>".($user_rate*100)." ORDER BY rand( ) LIMIT 1";

			if($user_rate >= 1)
				$sql_condition = " AND es.`min_rate`<=".($user_rate*100)."
						AND es.`max_rate`>=".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
		}

   	  	$res = $conn->query($sql.$sql_condition);

	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
			$arr['sid'] = $id;
    		$arr['count'] = 0;

	    	$row_count = mysql_affected_rows();

	    	if($row_count>0){
				$arr['count'] = $row_count;
				if($type == 'detail'){
					while($row = $res->fetchRow()){
						$level = array(
							'level_id'=>$row->level_id,
							'level_name'=>$row->level_name,
							'description'=>$row->description,
						);
						$resources = array(
							'resources' => stripslashes(htmlspecialchars_decode($row->resources)),
							'resources_tags1' => $row->resources_tags1,
							'or_resources_tags2' => $row->or_resources_tags2
						);

						$arr['comment'][$row->scid]['min_rate'] = $row->min_rate;
						$arr['comment'][$row->scid]['max_rate'] = $row->max_rate;
						$arr['comment'][$row->scid]['level'] = $level;
						$arr['comment'][$row->scid]['resources'] = $resources;
					}

				}

				if($type == 'simple'){
					while($row = $res->fetchRow()){
						$arr['comment'][$row->scid]['level_id'] = $row->level_id;
						$arr['comment'][$row->scid]['min_rate'] = $row->min_rate;
						$arr['comment'][$row->scid]['max_rate'] = $row->max_rate;
					}
				}
	    	}
	    	else{

	    		$structure_info = self::query_exam_structure_by_sid('detail',$id,'');

	    		$tid = $structure_info[$id]['tid'];

	    		$arr['global_comment'] = self::query_structure_comment_global($type, $tid, $user_rate);

	    	}
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;
 }



 static function query_structure_comment_global($type, $exam_type_id, $user_rate){
	$conn = DB_CONNECT::db_conn();

 	$exam_type_id = $exam_type_id>0?$exam_type_id:0;

	$arr = array();

    if($exam_type_id>=0){
 		$type= !empty($type)?$type:'simple';


		if($exam_type_id > 0){

			 $sql_relate = "SELECT `rid` , `sid` , `global_examType_detail_mark` , `same_mark` , `only_attribute`
					FROM `exam_structure_comment_relate`
					WHERE `global_examType_detail_mark` =".$exam_type_id."
					ORDER BY rid desc LIMIT 1 ";

			$res_relate = $conn->query($sql_relate);


			if(DB::isError ($res_relate)){
			  $error_messsage = mysql_error();
			  self::halt($error_messsage,$sql_relate);
			}else{
				$arr['rid'] = 0;
				/*$arr['sid'] = 0; */
				$arr['same_mark'] = 0;
				$arr['only_attribute'] = 0;

				if($row = $res_relate->fetchRow()){
					$arr['rid'] = $row->rid;
					/*$arr['sid'] = $row->sid; */
					$arr['same_mark'] = $row->same_mark;
					$arr['only_attribute'] = $row->only_attribute;
				}

			}

	   }


   	  	 $sql = "SELECT id, esg.level_id, level_name, description, min_rate, max_rate, resources_tags1, or_resources_tags2
				FROM `exam_structure_comment_global` esg
				LEFT JOIN  `exam_comments_level` ecl ON esg.level_id = ecl.level_id
				LEFT JOIN  `exam_comments` ec ON esg.level_id = ec.level_id
				WHERE esg.`exam_type_id` =".$exam_type_id;

		$sql_condition = "	ORDER BY `id` ";

		if($user_rate!==''){
			$sql_condition = " AND esg.`min_rate`<=".($user_rate*100)."
						AND esg.`max_rate`>".($user_rate*100)." ORDER BY rand( ) LIMIT 1";

			if($user_rate >= 1)
				$sql_condition = " AND esg.`min_rate`<=".($user_rate*100)."
						AND esg.`max_rate`>=".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
		}


   	  	$res = $conn->query($sql.$sql_condition);

	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
			$arr['exam_type_id'] = $exam_type_id;
			$arr['count'] = 0;

			$row_count = mysql_affected_rows();

			if($row_count>0){
				$arr['count'] = $row_count;
				if($type == 'detail'){
					while($row = $res->fetchRow()){
						$level = array(
							'level_id'=>$row->level_id,
							'level_name'=>$row->level_name,
							'description'=>$row->description,
						);
						$resources = array(
							'resources' => '',
							'resources_tags1' =>  $row->resources_tags1,
							'or_resources_tags2' => $row->or_resources_tags2
						);

						$arr['comment'][$row->id]['min_rate'] = $row->min_rate;
						$arr['comment'][$row->id]['max_rate'] = $row->max_rate;
						$arr['comment'][$row->id]['level'] = $level;
						$arr['comment'][$row->id]['resources'] = $resources;
					}

				}

				if($type == 'simple'){
					while($row = $res->fetchRow()){
						$arr['comment'][$row->id]['level_id'] = $row->level_id;
						$arr['comment'][$row->id]['min_rate'] = $row->min_rate;
						$arr['comment'][$row->id]['max_rate'] = $row->max_rate;
					}
				}
	    	}
	    	else if($row_count == 0){
	    		// 	 $conn->disconnect();
	    		return self::query_structure_comment_global($type, 0, $user_rate);
	    	}
	  	}

   }


    // 	 $conn->disconnect();
    return $arr;
 }


 static function query_structure_attribution_comment($type, $sid, $aid, $user_rate){
 	$conn = DB_CONNECT::db_conn();
 	$sid = $sid>0?$sid:0;

    if($sid>0 && $aid>0){
 		$type= !empty($type)?$type:'simple';

   	  	$sql = "SELECT saCid,aes.aid, aes.level_id, level_name, description, min_rate, max_rate, resources, resources_tags1, or_resources_tags2
				FROM `attribute_exam_structure_comment` aes
				LEFT JOIN  `attribute_comments_level` acl ON aes.level_id = acl.level_id
				LEFT JOIN  `attribute_comments` ac ON aes.level_id = ac.level_id
				WHERE aes.`sid` =" . $sid ." AND aes.`aid` = ". $aid." AND ac.`aid` =". $aid;

		$sql_condition = " ORDER BY aes.`saCid`";

		if($user_rate!==''){
			$sql_condition = " AND aes.`min_rate`<=".($user_rate*100)."
						AND aes.`max_rate`>".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
			if($user_rate >=1)
				$sql_condition = " AND aes.`min_rate`<=".($user_rate*100)."
						AND aes.`max_rate`>=".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
		}

		//echo $sql.$sql_condition;

   	  	$res = $conn->query($sql.$sql_condition);

	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{
			$arr['sid'] = $sid;
    		$arr['count'] = 0;

	    	$row_count = mysql_affected_rows();

	    	if($row_count>0){
				$arr['count'] = $row_count;
				if($type == 'detail'){
					while($row = $res->fetchRow()){
						$level = array(
							'level_id'=>$row->level_id,
							'level_name'=>$row->level_name,
							'description'=>stripslashes(htmlspecialchars_decode($row->description)),
						);
						$resources = array(
							'resources' => stripslashes(htmlspecialchars_decode($row->resources)),
							'resources_tags1' => $row->resources_tags1,
							'or_resources_tags2' => $row->or_resources_tags2
						);

						$arr['comment'][$row->saCid]['min_rate'] = $row->min_rate;
						$arr['comment'][$row->saCid]['max_rate'] = $row->max_rate;
						$arr['comment'][$row->saCid]['level'] = $level;
						$arr['comment'][$row->saCid]['resources'] = $resources;
					}

				}

				if($type == 'simple'){
					while($row = $res->fetchRow()){
						$arr['comment'][$row->saCid]['level_id'] = $row->level_id;
						$arr['comment'][$row->saCid]['min_rate'] = $row->min_rate;
						$arr['comment'][$row->saCid]['max_rate'] = $row->max_rate;
					}
				}
	    	}
	    	else{
	    		$arr['global_comment'] = self::query_structure_attribution_comment_global("detail", $aid, $user_rate);
	    	}
	  	}

   }

    // 	 $conn->disconnect();
    return $arr;
 }

 static function query_structure_attribution_comment_global($type, $aid, $user_rate){
	$conn = DB_CONNECT::db_conn();
 	$arr = array();

 	$aid = $aid>0?$aid:0;
 	$type= !empty($type)?$type:'simple';

    if($aid>=0){


		if($aid > 0){

			 $sql_relate = "SELECT `rid` , `sid` , `aid` , `same_mark`
					FROM `attribute_exam_structure_comment_relate`
					WHERE `aid` =".$aid."
					ORDER BY rid desc LIMIT 1 ";

			$res_relate = $conn->query($sql_relate);

			if(DB::isError ($res_relate)){
			  $error_messsage = mysql_error();
			  self::halt($error_messsage,$sql_relate);
			}else{
				$arr['rid'] = 0;
				/*$arr['sid'] = 0; */
				$arr['same_mark'] = 0;

				if($row = $res_relate->fetchRow()){
					$arr['rid'] = $row->rid;
					/*$arr['sid'] = $row->sid; */
					$arr['same_mark'] = $row->same_mark;
				}

			}


	   	}

 		if($aid == 0){
 			$sql = "SELECT id, ag.level_id, level_name, description, min_rate, max_rate, resources_tags1, or_resources_tags2
				FROM `attribute_exam_structure_comment_global` ag
				LEFT JOIN  `attribute_comments_level` acl ON ag.level_id = acl.level_id
				LEFT JOIN  `attribute_comments` ac ON ag.level_id = ac.level_id
				WHERE ag.`aid` =".$aid;
 		}
 		else
			$sql = "SELECT id, ag.level_id, level_name, description, min_rate, max_rate, resources_tags1, or_resources_tags2
				FROM `attribute_exam_structure_comment_global` ag
				LEFT JOIN  `attribute_comments_level` acl ON ag.level_id = acl.level_id
				LEFT JOIN  `attribute_comments` ac ON ag.level_id = ac.level_id
				WHERE ag.`aid` =".$aid." AND ac.`aid`=".$aid;

		$sql_condition = "	ORDER BY `id` ";

		if($user_rate!==''){
			$sql_condition = " AND ag.`min_rate`<=".($user_rate*100)."
						AND ag.`max_rate`>".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
			if($user_rate >=1)
				$sql_condition = " AND ag.`min_rate`<=".($user_rate*100)."
						AND ag.`max_rate`>=".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
		}

		//echo $sql.$sql_condition.'<br><br>';

   	  	$res = $conn->query($sql.$sql_condition);

	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{

	    		$arr['aid'] = $aid;
	    		$arr['count'] = 0;

	    		$row_count = mysql_affected_rows();

	    		if($row_count>0){
				   $arr['count'] = $row_count;
					if($type == 'detail'){
						while($row = $res->fetchRow()){
							$level = array(
								'level_id'=>$row->level_id,
								'level_name'=>$row->level_name,
								'description'=>stripslashes(htmlspecialchars_decode($row->description)),
							);
							$resources = array(
								'resources' => '',
								'resources_tags1' => $row->resources_tags1,
								'or_resources_tags2' => $row->or_resources_tags2
							);

							$arr['comment'][$row->id]['min_rate'] = $row->min_rate;
							$arr['comment'][$row->id]['max_rate'] = $row->max_rate;
							$arr['comment'][$row->id]['level'] = $level;
							$arr['comment'][$row->id]['resources'] = $resources;


						}
					}
					if($type == 'simple'){
						while($row = $res->fetchRow()){
							$arr['comment'][$row->id]['level_id'] = $row->level_id;
							$arr['comment'][$row->id]['min_rate'] = $row->min_rate;
							$arr['comment'][$row->id]['max_rate'] = $row->max_rate;
						}
					}
				}
				else if($row_count==0){
					// 	 $conn->disconnect();
		    		return self::query_structure_attribution_comment_global_default("detail", $aid, $user_rate);
				}

	  	}

   }

    // 	 $conn->disconnect();
    return $arr;
 }

/*
 * 用默认全局的等级评价标准来获取某个考点的等级，等级描述则调用本考点的描述。
 *
 */
 static function query_structure_attribution_comment_global_default($type, $aid, $user_rate){
	$conn = DB_CONNECT::db_conn();

 	$aid = $aid>0?$aid:0;

    if($aid>=0){

   	  	 $sql = "SELECT id, ag.level_id, level_name, description, min_rate, max_rate, resources_tags1, or_resources_tags2
				FROM `attribute_exam_structure_comment_global` ag
				LEFT JOIN  `attribute_comments_level` acl ON ag.level_id = acl.level_id
				LEFT JOIN  `attribute_comments` ac ON ag.level_id = ac.level_id
				WHERE ag.`aid` =0 AND ac.`aid` = ".$aid;

		$sql_condition = "	ORDER BY `id` ";

		if($user_rate!==''){
			$sql_condition = " AND ag.`min_rate`<=".($user_rate*100)."
						AND ag.`max_rate`>".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
			if($user_rate >=1)
				$sql_condition = " AND ag.`min_rate`<=".($user_rate*100)."
						AND ag.`max_rate`>=".($user_rate*100)." ORDER BY rand( ) LIMIT 1";
		}
		//echo $sql.$sql_condition;

   	  	$res = $conn->query($sql.$sql_condition);

	    $arr = array();
	    if(DB::isError ($res)){
		  $error_messsage = mysql_error();
		  self::halt($error_messsage,$sql);
	    }else{

	    		$arr['aid'] = 0;
	    		$arr['count'] = 0;

	    		$row_count = mysql_affected_rows();

	    		if($row_count>0){
				   $arr['count'] = $row_count;

					while($row = $res->fetchRow()){
						$level = array(
							'level_id'=>$row->level_id,
							'level_name'=>$row->level_name,
							'description'=>stripslashes(htmlspecialchars_decode($row->description)),
						);
						$resources = array(
							'resources' => '',
							'resources_tags1' => $row->resources_tags1,
							'or_resources_tags2' => $row->or_resources_tags2
						);

						$arr['comment'][$row->id]['min_rate'] = $row->min_rate;
						$arr['comment'][$row->id]['max_rate'] = $row->max_rate;
						$arr['comment'][$row->id]['level'] = $level;
						$arr['comment'][$row->id]['resources'] = $resources;
					}
				}
				else if($row_count==0){
					// 	 $conn->disconnect();
		    		$arr['count'] = 0;
				}

	  	}

   }

    // 	 $conn->disconnect();
    return $arr;
 }

 static function query_liushui_list($name){
 	$conn = DB_CONNECT::db_conn();

 	$name =!empty($name)?$name:'';
    if(!empty($name))
    	$sql_condition = " AND `excel_liushui` like '%".$name."%'";

   	$sql = "SELECT count(item_id) count, `excel_liushui` FROM `item_excel_liushui` WHERE `excel_liushui`  != 0 ";

	if(!empty($sql_condition))
    	$sql .= $sql_condition ;

	$sql .=" GROUP BY `excel_liushui` ORDER BY  `excel_liushui`  DESC";

	 $res = $conn->query($sql);

	$arr = array();
	if(DB::isError ($res)){
	  $error_messsage = mysql_error();
	  self::halt($error_messsage,$sql);
	}else{

		while($row = $res->fetchRow()){
			$arr[] = array(
				"liushui" => $row->excel_liushui,
				"count"=>$row->count
				);
		}
	}

    // 	 $conn->disconnect();
    return $arr;
 }



/**
* 判断是否已经有此流水号，返回记录个数，做相应操作
* $item_id,$excel_id都为空，$excel_liushui不为空时，查找所有此流水号的记录，返回记录个数。
*
* @param unknown_type $paper_id
* @return unknown
*/

static function  query_item_liushui($item_id,$excel_liushui,$excel_id,$status){

	if($excel_liushui > 0){
		$conn = DB_CONNECT::db_conn();

		if(empty($item_id) && empty($excel_id))
			$sql = "SELECT  count(*) count
				FROM `item_excel_liushui`
				WHERE excel_liushui = '".$excel_liushui."'";

		else {
			if(!empty($item_id))
				$sql .= " AND item_id = ".$item_id;
			if(!empty($excel_id))
				$sql .= " AND excel_id = ".$excel_id;
			if(!empty($status))
				$sql .= " AND status = ".$status;
		}

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			 $error_messsage = mysql_error();
			 self::halt($error_messsage,$sql);
		}else{
			$row = $res->fetchRow();
			if(empty($row))
				return 0;
			else{
				$row_count = $row->count;
			}
		}

		// 	 $conn->disconnect();
    	return $row_count;
	}
	else
		return 0;



 }


/**
* 根据流水号、$excel_id，返回$item_id 以及 $status
* 没有记录则返回空
* @param unknown_type $paper_id
* @return unknown
*/

static function  query_item_status_by_liushui($excel_liushui,$excel_id){
	if($excel_liushui > 0 && !empty($excel_id)){
		$conn = DB_CONNECT::db_conn();
		$arr = array();

		$sql = "SELECT `item_id`,`status`
				FROM `item_excel_liushui`
				WHERE `excel_liushui` = ? AND `excel_id` =? LIMIT 1";

		$res = $conn->query($sql,array($excel_liushui, $excel_id));
		if(DB::isError ($res)){
			 $error_messsage = mysql_error();
			 self::halt($error_messsage,$sql);
		}else{
			$row = $res->fetchRow();
			if(empty($row))
				return null;
			else{
				$arr['item_id'] = $row->item_id;
				$arr['status'] = $row->status;
			}
		}

		// 	 $conn->disconnect();
    	return $arr;
	}
	else
		return null;

 }

/**
* 删除生成的试卷信息（试卷基本信息，试题信息，用户此卷子做题信息）
* 分步删除，如果$type='first' 查询并返回用户是否已做过此套卷子
* 			如果$type='confirm'，删除所有相关信息。
* @param unknown_type $paper_id
* @return unknown
*/

static function  delete_paper_step_by_paper_id($type,$paper_id){
	$type= !empty($type)?$type:'first';
	//echo $type;
	if($type == 'first'){
		$conn = DB_CONNECT::db_conn();

		$sql = "SELECT  `result_id`
				FROM  `user_results`
				WHERE  `paper_id` = ".$paper_id;

		$sql_count = "SELECT  count(*) count
					FROM  `user_results`
					WHERE  `paper_id` = ".$paper_id;

		 $arr = array();
		 $res = $conn-> query($sql);
		 $res_count = $conn-> query($sql_count);

		 $row_count = $res_count->fetchRow();
		 $arr['count'] =  $row_count->count;

		 if(DB::isError ($res)){
				$error_messsage = mysql_error();
				self::halt($error_messsage,$sql);
		 }else{

			while($row = $res->fetchRow()){
				$result_id[] = $row->result_id;
			}

			if($result_id!=null)
				$arr['result_id'] =  implode(",",$result_id);

		 }

		 // 	 $conn->disconnect();
		 return $arr;

	 }
	 else if($type =='confirm'){
	 	$arr = self::delete_paper_step_by_paper_id('first',$paper_id);
	 	//return $arr;
	 	$status = DELETE::delete_paper_by_paper_id($paper_id,$arr);

	 	return $status;

	 }


}


static function getResources($tags){
	$content = '';
	if(!empty($tags) && $tags!=null && $tags!='')
		$content =  json_decode(file_get_contents('http://ebook.2u4u.com.cn/resources_by_tags/'.$tags));

	//file_put_contents('aaa.txt',$content);

	return $content;
}


}

?>