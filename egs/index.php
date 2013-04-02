<?php

header("Content-type: text/html; charset=utf-8");
require 'config.php';
require 'include/exam_structure.inc';

$op = $_REQUEST['op'];
$return_type = !empty($_REQUEST['return_type'])?$_REQUEST['return_type']:'json';

$type = !empty($_REQUEST['type'])?$_REQUEST['type']:'';
$id = !empty($_REQUEST['id'])||$_REQUEST['id']==0?$_REQUEST['id']:'';

$uid = $_REQUEST['uid']>0?$_REQUEST['uid']:0;
$name = !empty($_REQUEST['name'])?urldecode($_REQUEST['name']):'';
$type_id = !empty($_REQUEST['type_id'])?$_REQUEST['type_id']:'';
$num  = $_REQUEST['num']>0?$_REQUEST['num']:0;

$time_start = !empty($_REQUEST['time_start'])?urldecode($_REQUEST['time_start']):'';
$time_end = !empty($_REQUEST['time_end'])?urldecode($_REQUEST['time_end']):'';
//$attr_id = !empty($_REQUEST['attr_id'])?$_REQUEST['attr_id']:'';
$user_rate = !empty($_REQUEST['user_rate'])?urldecode($_REQUEST['user_rate']):'';
$aid = !empty($_REQUEST['aid'])||$_REQUEST['aid']==0?$_REQUEST['aid']:'';

//排序方式
$sortBy = !empty($_REQUEST['sortBy'])?$_REQUEST['sortBy']:'';


$api = new API();	
$delete  = new DELETE();



switch($op){
	case "exam":
		$arr = $api->query_exam_type($type,$id,$name);
	    break;
	case "item_type":
		$arr = $api->query_exam_item_type($type,$type_id,$id,$name);
	    break;
	case "basic":
		$arr = $api->query_basic_item_type($type,$id,$name);
	    break;	 
	case "tag_info":
	    $arr = $api->query_tag_info_by_tag($type,$id,$name);	
	    break;	 
	case "attribution_info":
	    $arr = $api->query_item_attribution($type,$id,$name);	
	    break;
	case "item_info":
		$arr = $api->query_item_info($type,$id,$name,'by_item');
	    break;	 
    case "item_info_by_type":
		$arr = $api->query_item_info($type,$type_id,'','by_type');
	    break;	   
	case "item_info_by_tag":
		$arr = $api->query_item_info($type,$id,$name,'by_tag');
	    break;	    
	case "get_mulichoice_answers":    
		$arr = $api->query_multichoice_by_id($id);    
		break;
	case "get_paper":
	    $arr = $api->get_paper_by_paper_id($type,$id,$uid);    
		break;
	case "paper_info":
	    $arr = $api->query_paper_info_by_paper_id($type,$id,$name);    
		break;
	case "paper_item":
	    $arr = $api->query_paper_items_by_paper_id($id);    
		break;	
	case "paper_info_by_term_id":
	    $arr = $api->query_paper_info_by_term_id($type,$id,$num);    
		break; 
	 case "paper_detail_info":
	    $arr = $api->query_paper_detail_by_paper_id($id);    
		break;
	 case "user_results_list":
	    $arr = $api->query_user_results_by_uid($type,$id,$type_id,$num);    
	    break;	 
	 case "user_result_info":
	    $arr = $api->query_user_results_by_result_id($type,$id);    
	    break;	    
	case "user_attribute_result_id":
	    $arr = $api->query_user_attribute_results_by_result_id($type,$id);
	    break;
	case "user_result_detail":
	    $arr = $api->query_user_result_paper_items_by_result_id($id);    
		break;	
	 case "user_result_each_part_points":
	    $arr = $api->query_user_result_each_part_points($id);    
		break;	
	 case "user_result_summary_info":
	    $arr = $api->query_user_result_and_summary_info($id);    
		break;
	case "user_diagnostic_report":
	    $arr = $api->query_user_diagnostic_report_by_result_id($id);
	    break;				
	 case "exam_structure_list":
	    $arr = $api->query_exam_structure_list($type,'',$id,$name,$num);    
		break;	
	 case "exam_structure":
	    $arr = $api->query_exam_structure_by_sid($type,$id,$name);    
		break;	
	 case "user_rate_rank":
	    $arr = $api->query_user_rate_by_tid($id,$num);    
	 	break;	
	 case "user_rate_trend":
	    $arr = $api->query_user_rate_trend_by_tid($id,$type_id,$num);    
	 	break;			
     case "exam_comments":
	    $arr = $api->query_exam_comments($id,$name);    	    
	 	break;
	 case "attr_comments":
	    $arr = $api->query_attribution_comment($type,$id,$aid);  
	 	break;
	case "comments_level":
	    $arr = $api->query_exam_comments_level($id,$name);    	    
	 	break;
	case "attribution_comments_level":
	    $arr = $api->query_attribution_comments_level($id, $name);    	    
		break;	 
	case "active_users":
	    $arr = $api->query_active_users($num);    	    
	 	break;	
	case "structure_comment":
	    $arr = $api->query_structure_comment($type,$id, $user_rate);  
	 	break;
	case "structure_comment_global":
	    $arr = $api->query_structure_comment_global($type,$id, $user_rate);  
	 	break;
	case "structure_attr_comment":
	    $arr = $api->query_structure_attribution_comment($type, $id, $aid,  $user_rate);  
	 	break;
	case "structure_attr_comments_global":
	//$type='list' or 'single'，默认list，按id排序，single，则random显示。
	    $arr = $api->query_structure_attribution_comment_global($type,$aid,$user_rate);  
	 	break;
	case "paper_delete":
	    $arr = $api->delete_paper_step_by_paper_id($type,$id);
	    break;
	case "liushui":
	    $arr = $api->query_liushui_list($name);
	    break;
	case "liushui_delete":
	    $arr = $delete->delete_items_by_liushui($id);
	    break;
	case "tongji":
	    $arr = $api->query_paper_count_by_type_and_name($type_id,$name,$time_start,$time_end, $sortBy);  
	 	break;	 
	//echo common::my_json_encode($arr);
}
	if($return_type == 'php')
		echo $arr;
	else
		echo COMMON::my_json_encode($arr);
		
?>