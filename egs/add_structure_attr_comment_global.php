<?php
header("Content-type:text/html;charset=utf-8");
require 'config.php';
$i = 1;
$arr = array("status"=>0);
//file_put_contents('aaa.txt', 'add_structure_attr_comment:\r\::'.var_export($_POST,true).' \r\n\r\n');

$is_correct = 1;
//echo var_export($_POST,true);


if(!empty($_POST['comments'])&&isset($_POST['btn_submit'])){
	
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
	    $aid = $_POST['aid']>0?$_POST['aid']:0;	
	    //如果为0，则表示全局默认评价等级
	    
	    if($aid!=0){
	    	$sid = 0;

			$rid = $_POST['rid']?$_POST['rid']:0;	
			$same_mark  = $_POST['same_mark']=='on'?1:0;	
			
			if($same_mark == 1){
	    		$same_resources_tags1 = trim($_POST['resources_tags1']);
				$same_or_resources_tags2 = trim($_POST['or_resources_tags2']);
			}
			
			//插入、更新 attribute_exam_structure_comment_relate数据库关系，sid=0;sid ,	aid ,	same_mark 
			if($rid > 0)
				$rs = UPDATE::structure_attr_comments_relate_update($rid,$sid,$aid,$same_mark);
			else
				$rs = INSERT::structure_attr_comments_relate_insert($sid,$aid,$same_mark);
			
			//通过tid、sid查询关系及分级评论，返回设置列表；用于调取资源。
			
	    }
	    
	    //$same_resources_tags1 = trim($_POST['resources_tags1']);
		//$same_or_resources_tags2 = trim($_POST['or_resources_tags2']);
	    
		foreach($_POST['comments'] as $value){
			$id = $value['id'];//如果id>0，则修改已提交comments
			if($value['min_rate']>=0&&$value['max_rate']>=0&&$value['level_id']>0){
				
				$min_rate = $value['min_rate']>=0?(float)$value['min_rate']:0;
				$max_rate = $value['max_rate']>0?(float)$value['max_rate']:0;
				
				$level_id =$value['level_id']>0?$value['level_id']:0;
				
				if(isset($same_resources_tags1)||isset($same_or_resources_tags2)){
					$resources_tags1 = $same_resources_tags1;
					$or_resources_tags2 = $same_or_resources_tags2;
				}
				else{
					$resources_tags1 = trim($value['resources_tags1']);
					$or_resources_tags2 = trim($value['or_resources_tags2']);
				}
			
				$tags1_arr = array();    
				if(strpos($resources_tags1,","))        	
					$tags1_arr = explode(",",$resources_tags1);
				else 
					$tags1_arr[0] = $resources_tags1;     
					
				$tags2_arr = array();    
				if(strpos($or_resources_tags2,","))        	
					$tags2_arr = explode(",",$or_resources_tags2);
				else 
					$tags2_arr[0] = $or_resources_tags2;  
					
				
				$resources_tags1 = implode(',', $tags1_arr);
				$or_resources_tags2 = implode(',', $tags2_arr);
			
			
			//echo '<br><br>id::'.$id.'<br>aid::'.$aid.'<br>level_id::'.$level_id.'<br>min_rate::'.$min_rate.'<br>max_rate::'.$max_rate.'<br>resources_tags1::'.$resources_tags1.'<br>or_resources_tags2::'.$or_resources_tags2;
			
				if($id>0) //如果id>0，则修改已提交comments
					$rs = UPDATE::structure_attr_comments_global_update($id,$aid,$level_id,$min_rate,$max_rate,$resources_tags1,$or_resources_tags2);	 
					//whx here 修改名字参数（先查看文件中是否有此函数） 
				else 
					$rs = INSERT::structure_attr_comments_global_insert($aid,$level_id,$min_rate,$max_rate,$resources_tags1,$or_resources_tags2);	    
					//whx here 修改名字参数（先查看文件中是否有此函数）
			}
			$i++;
		}
  }
}


if($i > 1){	
	$arr = array("status"=>1
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