<?php
require 'config.php';

if(isset($_POST['btn_submit'])&!empty($_POST['name'])){
		
	$name = $_POST['name'];
	$body = $_POST['body'];
	
	$rs = INSERT::basic_item_type_insert($name,$body);	
		 
	if($rs)
	       echo "添加成功";
	else 
	       echo "添加失败";

	
	
}

?>