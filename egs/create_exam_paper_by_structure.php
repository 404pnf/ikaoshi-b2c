<?php
header("Content-type: text/html; charset=utf-8");
require 'config.php';
require 'include/exam_structure.inc';
$id = (int)$_GET['id'];
$uid = (int)$_GET['uid'];
if($id>0){
	$paper_id = INSERT::exam_info_insert($id,$uid);
	if($paper_id>0){
	
	    $info = INSERT::insert_exam_items_by_structure($paper_id,$id);
	    if(count($info)>0){
	    	$arr = array("id" => $paper_id,    
                         "status" => 1
                  );
	    }else{ 
	      	$arr = array("status" => 0,
	      				"message"=>"count(info)<=0"
                 	 );
	    }
	}else{
			$arr = array("status" => 0,
	      				"message"=>"paper_id<=0".$paper_id
                  );
		
	}
}else{
	$arr = array("status" => 0,
	      		"message"=>"id<=0"
                 );
	
}

	echo COMMON::my_json_encode($arr);

?>