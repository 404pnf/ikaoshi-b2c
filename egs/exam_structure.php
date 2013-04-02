<?php

if(isset($_POST['submit_btn'])|isset($_POST['submit_comments'])|!empty($_POST['exam_title'])){
	
   require 'config.php';
   
   	$structure_info['exam_score']= isset($_POST['exam_score'])?(int)$_POST['exam_score']:0;
	$structure_info['exam_title']= !empty($_POST['exam_title'])?trim($_POST['exam_title']):'';
	$structure_info['exam_description'] = !empty($_POST['exam_description'])?trim($_POST['exam_description']):'';	
	$structure_info['exam_limittime'] =  isset($_POST['exam_limittime'])?(int)$_POST['exam_limittime']:0;
    $structure_info['tid'] = isset($_POST['exam_type_form'])?(int)$_POST['exam_type_form']:1;
    
    $id =   INSERT::exam_structure_info_insert($structure_info);
  
 if($id>0&&isset($_POST['part'])){

	 $part = $_POST['part'];
	 $arr = array();
	 foreach($part as $key=>$value){
	 	
 	    $structure_arr = array(); 	    
        $term_id = INSERT::tags_insert_and_get_id($value['tags']);   
        if($value['basetype']>0){
        	
	        $structure_arr['sid'] = $id;
	        $structure_arr['basic_type'] = !empty($value['basetype'])?$value['basetype']:0;
	        $structure_arr['item_type'] = !empty($value['type'])?$value['type']:0; 
	        if(is_array($term_id)){
	        if($value['tags_relate']==1)
	           $structure_arr['term_id'] = implode(",",$term_id);
	        if($value['tags_relate']==2)
	            $structure_arr['term_id'] = implode("-",$term_id);
	        }else
	           $structure_arr['term_id'] = 0;
	          
	        $structure_arr['num'] =  !empty($value['count'])?$value['count']:0; 
	        $structure_arr['points'] = !empty($value['score'])?$value['score']:0; 
	        $structure_arr['time_limit'] = !empty($value['limittime'])?$value['limittime']:0;  
	        $structure_arr['serial_number'] = !empty($value['serial_number'])?$value['serial_number']:0; 
	        $structure_arr['serial_order'] = !empty($value['order'])?$value['order']:0; 
	        $structure_arr['serial_num'] = $structure_arr['serial_order']."-".$structure_arr['serial_number'];	        
	        $structure_arr['check_relation'] = $value['subque_relate']>0?$value['subque_relate']:0;         
	        
	        $structure_arr['serial_str'] =  !empty($value['serial_name'])?$value['serial_name']:'';
	        $structure_arr['is_submit'] =  $value['submit_point']=='on'?1:0;	
	        $structure_id = '';
	        
	        if($value['pid']==-1){
	        	
	           $structure_arr['parent_id'] =  0;                         
	           $structure_id = INSERT::exam_structure_insert($structure_arr); 
	           $part[$key]['structure_id'] = $structure_id;
	           $arr[] = $key;
	       	           
	        }elseif(in_array($value['pid'],$arr)){
	        	
	           $structure_arr['parent_id'] = $part[$value['pid']]['structure_id'];	 
	           $structure_id = INSERT::exam_structure_insert($structure_arr);   
	           $part[$key]['structure_id'] = $structure_id;     
	           $arr[] = $key;           
	          
	        }else{
	        	
	        	echo $value['id']."<br>";
	        	
	        }
      }
   }

   if(isset($_POST['submit_btn'])){
   	 $status = 1;
   	 $str_id = $id;
   	 $str_name = $structure_info['exam_title'];
   	 
   }
   
  if(isset($_POST['submit_comments'])){
   	 $status = 2;
   	 $str_id = $id;
   	 $str_name = $structure_info['exam_title'];   	  
   }
      
  }else{ 
  	
      $status = 0;
  }      
 }else{ 
      $status = 0;
      
}
   $arr = array("id" => $str_id,
                "name" => $str_name,
                "status" => $status
        );
     
	echo COMMON::my_json_encode($arr);




?>