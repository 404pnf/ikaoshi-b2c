<?php
header("Content-type:text/html;charset=utf-8");
require 'config.php';
$i = 1;
$arr = array("status"=>0);
file_put_contents('aaa.txt', 'add_structure_attr_comment1:\r\::'.var_export($_POST,true).' \r\n\r\n');
//echo var_export($_POST,true);
$is_correct = 1;
//file_put_contents('aaa.txt', "$_POST ::".$_POST."\r\n\r\n",FILE_APPEND);

if(!empty($_POST['comments'])&&!empty($_POST['sid'])&&!empty($_POST['aid'])){

/**/
	foreach($_POST['comments'] as $info){

		$is_error = check_value($info['min_rate'],$info['max_rate'],$info['level_id']);


    	if($is_error==2){
			$arr = array("status"=>2);
			echo COMMON::my_json_encode($arr);
			exit;
		}else{
			$is_correct = 1;
		}

 	}
 	

			
	 
	if($is_correct == 1){
	    $sid = $_POST['sid']>0?$_POST['sid']:1;	
	    $aid = $_POST['aid']>0?$_POST['aid']:1;	
	    
		foreach($_POST['comments'] as $value){
			$saCid = $value['saCid'];
			
			//file_put_contents('aaa.txt', "is_correct == 1 foreach:\r\min_rate::".$value['min_rate']." max_rate::".$value['min_rate']." level_id::".$value['level_id']."\r\n\r\n",FILE_APPEND);
			if($value['min_rate']>=0&&$value['max_rate']>=0&&$value['level_id']>0){
				$min_rate = $value['min_rate']>=0?(float)$value['min_rate']:0;
				$max_rate = $value['max_rate']>0?(float)$value['max_rate']:0;
				$resources = trim($value['resources']);
				$resources_tags1 = trim($value['resources_tags1']);
				$or_resources_tags2 = trim($value['or_resources_tags2']);
			    $level_id = $value['level_id']>0?$value['level_id']:0;
			}
			
			if($scid>0) 
				$rs = UPDATE::structure_attr_comments_update($scid,$sid,$aid,$level_id,$min_rate,$max_rate,$resources,$resources_tags1,$or_resources_tags2);	  
			else 
				$rs = INSERT::structure_attr_comments_insert($sid,$aid,$level_id,$min_rate,$max_rate,$resources,$resources_tags1,$or_resources_tags2);	    
			
			$i++;
		}
  }
}
if($i > 1){	
	$str_info = API::query_exam_structure_list('simple',$sid,'','');	
	$title = $str_info[0]['title'];	
	$arr = array("status"=>1,
	             "title" => $title,
	             "sid" =>$sid	
	);
}else{
	$arr = array("status"=>0);
}

echo COMMON::my_json_encode($arr);


//检查各参数是否为空
function check_value($a,$b,$c){
	$i = 1;
	if($a!==''|$a>0){
		$i++;
	}
	if($b!==''|$b>0){
		$i++;
	}
	if($c!==''|$c>0){
		$i++;
	}	
	if($i==1||$i>3){
	  $is_correct = 1;	
	}else{
	  $is_correct = 2;
	}
	return $is_correct;
	
}


?>