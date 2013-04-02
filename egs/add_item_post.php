<?php


if(!empty($_POST['body'])){
	
    require 'config.php';    
    $type = $_POST['op'];

	$arr['body'] = isset($_POST['body'])?$_POST['body']:'';
	$arr['description'] = isset($_POST['description'])?$_POST['description']:'';
	$arr['type'] = isset($_POST['type'])?(int)$_POST['type']:'';
	$arr['name'] = isset($_POST['name'])?$_POST['name']:API::item_rename($arr['type']);
	$arr['point'] = $_POST['point']>0?$_POST['point']:0;
	$arr['uid'] = $_POST['uid']>0?$_POST['uid']:1;
	$arr['basic_type'] = isset($_POST['id'])?(int)$_POST['id']:0;
	$arr['time_limit'] = isset($_POST['time'])?(int)$_POST['time']:0;
	$arr['blank_size'] = $_POST['answer_blank']>0?(int)$_POST['answer_blank']:0;
	$files['fileurl'] = $_POST['filepath'];
	if($files['fileurl'] != ''){
		$arr['file_id'] = INSERT::files_insert('link',$files); 
	}else{
		$arr['file_id'] = 0;		
	}	
	
	$pid = $_POST['pid']>0?$_POST['pid']:0;
	$arr['is_random'] = $_POST['subques_random']=='on'?1:2;	
	$need_parent = $_POST['only_pid']=='on'?1:0;	
	$answer = !empty($_POST['answer'])?$_POST['answer']:'';
	
	
	$tags = !empty($_POST['tags'])?$_POST['tags']:'';	
	$attributes = !empty($_POST['attributes'])?$_POST['attributes']:'';
	$choice_multi = $_POST['is_multichoice']=='on'?1:0;
	$choice_random = $_POST['answer_is_random']=='on'?1:0;
	$is_objective = $_POST['is_objective']=='on'?1:0;
	
   //$item_id =1;
    $item_id = INSERT::item_info_insert($arr);     //

 if($item_id>0){		
 
     INSERT::item_parent_insert($item_id,$pid,$need_parent,0);   //添加试题上级关系 	  
 	
 	  if($type == 'multichoice'){ 		
 
	      INSERT::multichoice_answers($item_id,$answer);    //添加选择题选项
	      INSERT::multichoice_properties_insert($item_id,$choice_multi,$choice_random);
 	  }
 
 	  if($type == 'filling'){ 		
	      INSERT::blank_filling_answers($item_id,$is_objective,$answer);   //添加填空题选项  
          INSERT::blank_filling_properties($item_id,$arr['blank_size']);
 	  }
  
  	  if($type == 'material'){ 		
          if($answer['explain']!=''){	      
	          INSERT::material_items_feedback_insert($item_id,$answer['explain']);  //题目材料解析录入
	     }
  	  
  	  }
 	  

	   
 	  if($tags!=''){
	      $term = INSERT::tags_insert_and_get_id($tags);      //插入标签，如果标签已存在则返回其term_id
      }
     
  	  if($attributes!=''){
	       $attributes_arr = INSERT::attributes_insert_and_get_id($attributes);      //插入标签，如果标签已存在则返回其term_id
      }
      
      
      if(is_array($term)){
	      foreach($term as $value){
            INSERT::tags_item_realtionship_insert($item_id,$value);     //插入试题和标签对应关系
	      }
	  } 

      if(is_array($attributes_arr)){
	      foreach($attributes_arr as $value){
            INSERT::attributes_item_realtionship_insert($item_id,$value);     //插入试题和标签对应关系
	      }
	  } 
	  
      
      if(isset($_POST['btn_submit_subques'])){
      	  $status = 1;      	
      }  

      if(isset($_POST['btn_submit'])){
      	  $status = 2;      	
      }   
      
      
}else{ 
	
	 $item_id = '';
	 $parent_id = '';
     $status = 0;   

}	

     $pid = $pid>0 ? $pid : -1;
     
     $arr = array("id" => $item_id,
                  "pid" => $pid,
                  "status" => $status
          );
     
	 echo COMMON::my_json_encode($arr);
}

?>