<?php

require_once 'mysql.inc';

class EM_STRUCTURE extends DB_CONNECT {



static function query_structure_info($sid){

	$conn = parent::db_conn();
	if($sid>0){
		$sql = "SELECT  `tid`, `uid`, `title`, `description`, `points`, `time_limit`, `created`, `status`
	            FROM `exam_structure_info`
	            WHERE `sid` = ".$sid." LIMIT 1";

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{

			  $arr = $res->fetchRow(DB_FETCHMODE_ASSOC);
	 	}

   }
     //$conn->disconnect();
     return  $arr;

  }

static function  query_structure_parent_detail($sid){

	$conn = parent::db_conn();
	if($sid>0){
		$sql = "SELECT `structure_id`,`parent` FROM `exam_structure`  WHERE `sid` = ".$sid." ORDER BY `parent` ASC";
		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
			while($row = $res->fetchRow()){
				$arr[$row->structure_id] = $row->parent;

			}

		}

	}
	//$conn->disconnect();
    return  $arr;

}


/*
 *
 * 获取符合试卷模板要求的试题
 *
 * 返回 试题的id
 */


static function  get_exam_items_by_parent_id($arr,$parent_id,$paper_items){

$conn = parent::db_conn();
if(is_array($arr)){
	$total = '';
	$total = $arr['total']>0?$arr['total']:1;
	$sql = "SELECT h.`item_id`
			FROM `item_hierarchy`  h
			LEFT JOIN `item_info` i
			ON i.`item_id` = h.`item_id`
			WHERE ";
	if($parent_id!==''|$parent_id>0){
		$sql .=	"`parent`=".$parent_id." AND ";

		$random_id_sql = 'SELECT `is_random` FROM `item_info` WHERE `item_id` ='.$parent_id  ;
	}

	if($arr['type']>0)
			$sql .=	 " i.`type` =".$arr['type']." AND ";
	if($arr['basic_type']>0)
			$sql .=	 " i.`basic_type` =".$arr['basic_type']." AND ";

	 if(is_array($paper_items)&$paper_items!=null){
	       $items_exists = implode(",",$paper_items);
	       $sql .= " i.`item_id` NOT IN (".$items_exists.") AND ";
     }

    if(substr($sql,-4) == "AND "){
      	$sql = substr($sql,0,-4);
    }

    $res = $conn->query($random_id_sql);
     while($row = $res->fetchRow()){
				$random_id = $row->is_random;
	}
	if($random_id == 2)
		$sql .=	" ORDER BY `item_id` LIMIT ".$total;
    else

    	$sql .=	" ORDER BY rand() LIMIT ".$total;

   file_put_contents(aaa.txt, 'get_exam_items_by_parent_id :\r\n'.$sql.'\r\n', FILE_APPEND);


	$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
			  while($row = $res->fetchRow()){
				$array[] = $row->item_id;
			}
		}
  }
    //$conn->disconnect();
    return $array;
}



/**
 * 寻找题目
 *
 * $conditions  选项设置  是否有上下级，是否下级满足模板要求
 * $num 需要的题目数量
 * $paper_id 试卷id
 *
 */

static function find_items_by_conditions($arr,$conditions,$check_parent,$num,$parent_id,$paper_items){

	//$check_parent = 'NotNeedParent';//IgnoreParent:不检查need_parent项；NeedParent:唯一依赖父级题目；NotNeedParent：不唯一依赖与父级题目

	$conn = parent::db_conn();

		$query  = "SELECT i.`item_id` , i.`is_random`
				   FROM `item_info` i
				   LEFT JOIN `item_hierarchy` hy ON hy.`item_id` = i.`item_id`";
    //whx here 去掉 i.`is_random`
    file_put_contents(aaa.txt, 'find_items_by_conditions:$row:::::\r\n'.var_export($arr, TRUE).'\r\n\r\n', FILE_APPEND);

    if($arr['term_id']){
    	$temp_term_id = str_replace("item_id:","",$arr['term_id'],$count_i);
    	if($count_i > 0){
    		if(strpos($temp_term_id,",")){
				 $item_id_arr = explode(",",$temp_term_id);
				 $tag_condition = " AND i.item_id IN";
				 $count = count($item_id_arr);
				 for($i=0;$i<$count;$i++){

					if($i==0)
						$tag_condition .= "( ".$item_id_arr[$i];
					else
						 $tag_condition .= ", ".$item_id_arr[$i];


				 }

				 $tag_condition .=" )";
				//file_put_contents(aaa.txt, 'find_items_by_conditions:$arr["term_id"]_item_id:$tag_condition:::::\r\n'.$tag_condition.'\r\n\r\n', FILE_APPEND);
			}else{

                $item_id = $temp_term_id;

			    $tag_condition = " AND i.item_id = ".$item_id;

             //  file_put_contents(aaa.txt, 'find_items_by_conditions:$arr["term_id"]_item_id:$tag_condition:::::\r\n'.$tag_condition.'\r\n\r\n', FILE_APPEND);
			}
    	}
       else if(strpos($arr['term_id'],",")){
             $tag_arr = explode(",",$arr['term_id']);
             $tag_condition = " AND i.item_id IN ";
             $count = count($tag_arr);
             for($i=0;$i<$count;$i++){
                if($i==0){

                 	$tag_condition .= " (SELECT a".$i.".item_id FROM `term_item` a".$i;
                 	$tag_where = " WHERE a".$i.".`term_id`=".$tag_arr[$i];

                }else {

		             $tag_condition .= ",`term_item` a".$i;
		             $tag_where .= " AND a".$i.".`term_id`=".$tag_arr[$i]." AND a".$i.".`item_id`=a0.`item_id`";

                 	}
                 }

                 $tag_condition .=$tag_where." )";
			//   file_put_contents(aaa.txt, 'find_items_by_conditions:$arr["term_id"]_true:$tag_condition:::::\r\n'.$tag_condition.'\r\n\r\n', FILE_APPEND);
			}else{
				if(strpos($arr['term_id'],"-"))
                     $tags = str_replace("-",",",$arr['term_id']);
                else
                     $tags = $arr['term_id'];

			    $tag_condition = " AND i.item_id IN (SELECT `item_id` FROM `term_item` WHERE `term_id` IN (".$tags."))";

            //    file_put_contents(aaa.txt, 'find_items_by_conditions:$arr["term_id"]_false:$tag_condition:::::\r\n'.$tag_condition.'\r\n\r\n', FILE_APPEND);
			}
         }

        if($parent_id!==''|$parent_id>0){
			$random_id_sql = 'SELECT `is_random` FROM `item_info` WHERE `item_id` ='.$parent_id  ;

			$res = $conn->query($random_id_sql);
			 while($row = $res->fetchRow()){
						$random_id = $row->is_random;
			}
		}
		else{
			$random_id == $conditions['ques_is_random'];
		}

		if($random_id == 2)
			$order_by = " `item_id` ";
		else
			$order_by = " rand() ";


           if($conditions['check_child_type']){

	          $query .= "LEFT JOIN `item_hierarchy` ih ON ih.`parent` = i.`item_id`
	                     LEFT JOIN `item_info` c ON c.`item_id` = ih.`item_id`";

           }

          if($conditions['check_is_have_parent']){

          	$have_parent = '';

		    $have_parent = self::is_have_parent($arr['structure_id'],'structure');

            if($have_parent){

             	   $parent_condition = " AND i.item_id IN (SELECT `item_id` FROM `item_hierarchy` WHERE `parent`>0)";

             }else{

             	   $parent_condition = " AND i.item_id IN (SELECT `item_id` FROM `item_hierarchy` WHERE `parent`=0)";

              }
          //    file_put_contents(aaa.txt, 'find_items_by_conditions:check_is_have_parent:::::\r\n'.$parent_condition.'\r\n\r\n', FILE_APPEND);
            }

           if($conditions['check_is_have_child']){

           	  $have_child = '';

			  $have_child = self::is_have_child($arr['structure_id'],'structure');

	          if($have_child){

             	   $child_condition = " AND i.item_id IN (SELECT `parent` FROM `item_hierarchy` WHERE `parent`>0)";

               }else{

             	   $child_condition = " AND i.item_id NOT IN (SELECT `parent` FROM `item_hierarchy` WHERE `parent`>0)";

               }

          //      file_put_contents(aaa.txt, 'find_items_by_conditions:check_is_have_child:::::\r\n'.$child_condition.'\r\n\r\n', FILE_APPEND);
             }

             $query .= "WHERE i.`basic_type`=".$arr['basic_type']." AND i.`type`=".$arr['item_type']." " ;


             if($conditions['check_child_type']){

             	    if($arr['child_item_type']==null){
             	      $query .= " AND c.`type` IS NULL";
             	    }else{
            	      $query .= " AND c.`type`=".$arr['child_item_type'];
                  }

                 if($arr['child_basic_type']==null){
             	       $query .= " AND c.`basic_type` IS NULL";
             	    }else{
            	       $query .= "  AND c.`basic_type`=".$arr['child_basic_type'];
                  }


           // 	   file_put_contents(aaa.txt, 'find_items_by_conditions:check_child_type:::::\r\n'.$query.'\r\n\r\n', FILE_APPEND);
             }

             if(strlen($tag_condition)>4)
                     $query .= $tag_condition;

             if(strlen($child_condition)>4)
                     $query .= $child_condition;

              if(strlen($parent_condition)>4)
                     $query .= $parent_condition;

            //试题在本套卷子中不重复出现

            //$query .= "AND NOT EXISTS (SELECT 1 FROM `exam_item` WHERE `item_id`=i.`item_id` AND `paper_id`=".$paper_id.")";
             if(is_array($paper_items)&$paper_items!=null){
	              $items_exists = implode(",",$paper_items);
	              $query .= " AND i.`item_id` NOT IN (".$items_exists.")";


	       //       file_put_contents(aaa.txt, 'find_items_by_conditions:$$paper_items_!null:$query:::::\r\n'.$query.'\r\n\r\n', FILE_APPEND);
             }

            //if($check_parent == 'IgnoreParent')
 			if($check_parent == 'NeedParent')
            	$query .= " AND hy.`need_parent` = 1";
            elseif($check_parent == 'NotNeedParent')
            	$query .= " AND hy.`need_parent` = 0";

            if($num>0)
                    $query .= " GROUP BY i.`item_id` ORDER BY ".$order_by." LIMIT ".$num;     //随机获得数据
            $result = $conn->query($query);

    file_put_contents("aaa.txt", 'find_items_by_conditions:\r\n'.$query.'\r\n\r\n', FILE_APPEND);

	       if(DB::isError ($result)){
		      die ("SELECTfailed: " . $result->getMessage () . "\n");
	        	}else{

			       while($array = $result->fetchRow()){
			    	 $output[]= $array->item_id;
			    	 //if($array->item_id==212)

			    }

	}

	//$conn->disconnect();
    return  $output;

}



static function  is_have_parent($id,$type){

	$conn = parent::db_conn();
	if($id>0){
	    if($type=='structure')
		    $sql = "SELECT `parent` FROM `exam_structure`  WHERE `structure_id` = ".$id;
	    if($type=='item')
		    $sql = "SELECT  `parent` FROM `item_hierarchy`  WHERE `item_id` = ".$id;


		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
			if($res->numRows()){
				while($row = $res->fetchRow()){

				if($row->parent==0){

                      $have_parent = 0;

				}elseif($row->parent>0){

					   $have_parent = 1;

				}


			}

		}else
		     $have_parent = 0;
	  }
	}else
	   $have_parent = 0;

	////$conn->disconnect();
    return  $have_parent;


}


static function  is_have_child($id,$type){

	$conn = parent::db_conn();
	if($id>0){

		if($type=='structure')
		    $sql = "SELECT `structure_id` id FROM `exam_structure`  WHERE `parent` = ".$id;
	    if($type=='item')
		    $sql = "SELECT  `item_id` id FROM `item_hierarchy`  WHERE `parent` = ".$id;

		$res = $conn->query($sql);
		if(DB::isError ($res)){
			$error_messsage = mysql_error();
			self::halt($error_messsage,$sql);
		}else{
			if($res->numRows()){
				while($row = $res->fetchRow()){

					if($row->id==0){

	                      $have_child = 0;

					}elseif($row->id>0){

						  $have_child = 1;
					}
				}
			}else
			   $have_child = 0;
		}

	}else
	   $have_child = 0;

	////$conn->disconnect();
    return  $have_child;


}




}




?>