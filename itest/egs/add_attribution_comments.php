<?php
header("Content-type:text/html;charset:utf-8");
require 'config.php';
if(isset($_POST['btn_submit'])&!empty($_POST['name'])){
		
	$name = $_POST['name'];
	$attribution = $_POST['attribution'];
	$body = trim($_POST['body']);
    $weight = trim($_POST['weight']);
    
	//whx here
	$rs = INSERT::attribution_comments_insert($name, $attribution, $body, $weight);
	 
	 
	if($rs)
	       echo "添加成功";
	    else 
	       echo "添加失败";
}else{
	 echo "提交失败";	
}
?>