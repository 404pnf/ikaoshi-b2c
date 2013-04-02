<?php
require 'config.php';
if(isset($_POST['btn_submit'])&!empty($_POST['name'])){
		
		
	$name = isset($_POST['name'])?trim($_POST['name']):'';
	$body = isset($_POST['body'])?trim($_POST['body']):'';
	$weight = isset($_POST['weight'])?trim($_POST['weight']):0;
     
	$rs = INSERT::single_tag_insert($name,$body,$weight);
	 
	if($rs)
	       echo "添加成功";
	    else 
	       echo "添加失败";


	
}

?>