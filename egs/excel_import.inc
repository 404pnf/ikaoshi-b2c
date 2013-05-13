<?php

function excel_read_and_insert($filename){

require_once 'upload/phpexcel/Classes/PHPExcel.php';
require_once 'upload/phpexcel/Classes/PHPExcel/IOFactory.php';
require_once 'upload/phpexcel/Classes/PHPExcel/Reader/Excel5.php';

/**/
$objReader = PHPExcel_IOFactory::createReader('Excel5');//use excel2007 for 2007 format

//echo $filename;


try {
    $objPHPExcel = $objReader->load($filename);
} catch (Exception $e) {
    print $e->getMessage();
    exit();
}
/*
$objReader = PHPExcel_IOFactory::createReader('Excel5');

$objPHPExcel = $objReader->load($filename);
*/
//



$sheet = $objPHPExcel->getSheet(0);
$highestRow = $sheet->getHighestRow(); // 取得总行数
//$highestColumn = $sheet->getHighestColumn(); // 取得总列数



$has_excel_liushui = $objPHPExcel->getActiveSheet()->getCell("A1")->getValue();//流水号
if($has_excel_liushui != '流水号'){
	echo "请在第一行插入流水号，第一次导入，流水号为0";
	return false;
}
$excel_liushui = $objPHPExcel->getActiveSheet()->getCell("B1")->getValue();//流水号
if($excel_liushui == '' || $excel_liushui == 0){
	$excel_liushui = date("Ymd").'-'.time();
}

echo "流水号：".$excel_liushui."；请用此流水号更新本EXCEL表，此流水号是中断续传、更新修改的唯一标志。<br>";
//判断是否已经有此流水号，做相应操作query_item_liushui($item_id,$excel_liushui,$tmp_id,0);  $item_id,$tmp_id都为空，$excel_liushui不为空时，查找所有此流水号的记录，返回记录个数。


	$has_liushui = API::query_item_liushui('',$excel_liushui,'','');


	if($has_liushui >0 ){

	  //echo '<br>流水号个数:'.$has_liushui;

	  $k = 0;
	  for($j=3;$j<=$highestRow;$j++)
	  {//循环读取excel文件,读取一条,插入一条


			$tmp_id = 0;
			$tmp_id = $objPHPExcel->getActiveSheet()->getCell("A".$j)->getValue();//excel中编号

			$name = '';
			$name = $objPHPExcel->getActiveSheet()->getCell("B".$j)->getValue();//标题
			$item['name'] =trim($name);

			$item_parent_id = 0;
			$item_parent_id = $objPHPExcel->getActiveSheet()->getCell("C".$j)->getValue();//父级ID


			 $item_type = '';
			 $item_type = $objPHPExcel->getActiveSheet()->getCell("D".$j)->getValue();//考试题型
			 $item_type =trim($item_type);
			 if($item_type!=''){
				$item['type'] = handle_item_type($item_type);   //如果没有此题型,则插入;如有,返回id.
			 }else{
				$item['type'] = 0;
			 }

			 $item_basic_type = '';
			 $item_basic_type = $objPHPExcel->getActiveSheet()->getCell("E".$j)->getValue();//基本题型
			 $item_basic_type = trim($item_basic_type);
			 if($item_basic_type!=''){
				$item['basic_type'] = get_basic_type($item_basic_type);
			 }else{
				$item['basic_type'] = 0;
			 }

			$body = '';
			$body = $objPHPExcel->getActiveSheet()->getCell("F".$j)->getValue();//题干
			$item['body'] = trim($body);

		   //是否依赖父级题
			$need_parent = $need_parent>0?$need_parent:0;
			$options =  $objPHPExcel->getActiveSheet()->getCell("G".$j)->getValue();
			//$options = trim($options);

			$options_is_random = '';
			$options_is_random = $objPHPExcel->getActiveSheet()->getCell("H".$j)->getValue();  //选项是否随机
			$options_is_random = trim($options_is_random);
			$options_is_random = $options_is_random === ''?1:$options_is_random;

			$att_data = '';
			$att_data = $objPHPExcel->getActiveSheet()->getCell("I".$j)->getValue();  //考点
			$att_data = trim($att_data);

			$tags = $objPHPExcel->getActiveSheet()->getCell("J".$j)->getValue(); //标签
			$tags = trim($tags);

			$point = $objPHPExcel->getActiveSheet()->getCell("K".$j)->getValue();//分值
			$item['point'] =trim($point);
			$item['point'] = $item['point']>0?(float)$item['point']:0;

			 $time_limit =$objPHPExcel->getActiveSheet()->getCell("L".$j)->getValue();//时间限制
			 $item['time_limit'] =trim($time_limit);
			$item['time_limit'] = $item['time_limit']>0?(int)$item['time_limit']:0;


			//是否必须依赖父级关系
			$need_parent = 0;
			$need_parent = $objPHPExcel->getActiveSheet()->getCell("M".$j)->getValue();
			$need_parent = trim($need_parent);

			 $is_random = $objPHPExcel->getActiveSheet()->getCell("N".$j)->getValue();//子题目是否随机
			 $item['is_random'] = trim($is_random);
			 $item['is_random'] = $item['is_random']==1?1:2;

			 $fileurl = '';
			 $fileurl = $objPHPExcel->getActiveSheet()->getCell("O".$j)->getValue();   //附件处理
			 $files['fileurl'] = trim($fileurl);

			//处理附件
			 $item['file_id'] = 0;
			 if($files['fileurl']!=''){
				$item['file_id'] = handle_files($files);
			 }
			$item['file_id']=$item['file_id']>0?$item['file_id']:0;

			$blank_size = 0;
			$blank_size =  $objPHPExcel->getActiveSheet()->getCell("P".$j)->getValue();
			$blank_size = trim($blank_size);
			if($blank_size=='') $blank_size = 0;


	//根据流水号、$excel_id，返回$item_id 以及 $status，无记录返回null query_item_status_by_liushui($excel_liushui,$tmp_id);
	$item_status_liushui = API::query_item_status_by_liushui($excel_liushui,$tmp_id);

	 if($item_status_liushui == null){


		 if(is_array($item)){
			 $item['id'] = $item['id']>0?(int)$item['id']:0;
			 $item_id = INSERT::item_info_insert($item); //试题基本信息录入
		 }

		 if($item_id>0){

			$k++;
			$item = insert_allcontent($item, $item_id, $excel_liushui, $tmp_id, $att_data, $tags, $tem_arr, $item_parent_id, $need_parent, $options, $item_basic_type, $options_is_random, $blank_size);

		 }else{
			echo $item['title']."录入出错<br>";
		 }
	  }
	  else {
		$item_id = $item_status_liushui['item_id'];
		$status = $item_status_liushui['status'];

		$tem_arr[$tmp_id] = $item_id;  	//记录excel中的父子关系

		//echo "<br>item_id:: ".var_export($item_status_liushui,true)."  tem_arr::".var_export($tem_arr,true);

		if($status == 0){

			//删除此试题的所有信息，然后重新插入
			$delete_success = delete::delete_allcontent_by_item_id($item_id);

			if($delete_success)	{

				 if(is_array($item)){
					 $item['id'] = $item['id']>0?(int)$item['id']:0;
					 $item_id = INSERT::item_info_insert($item); //试题基本信息录入
				 }

				if($item_id>0){

					$k++;
					$item = insert_allcontent($item, $item_id, $excel_liushui, $tmp_id, $att_data, $tags, $tem_arr, $item_parent_id, $need_parent, $options, $item_basic_type, $options_is_random, $blank_size);

				 }else{
					echo $item['title']."重新录入出错<br>";
				 }

				$tem_arr[$tmp_id] = $item_id;

		   }
		   else
				echo $item['title']."删除未完全录入的数据时出错<br>";
		}
		else if($status == 1){
			//更新此试题的信息，不同的数据库表处理方式不同
			/**
			 * exam_type,item_type,files 读入EXCEL内容时，做处理
			 * term_data、item_attribute_detail添加新内容，item_attribute_hierarchy,term_item,item_hierarchy 删除后插入，结合处理
			 * item_info,blank_filling_answers,blank_filling_properties,material_items_feedback,multichoice_answers,multichoice_properties 直接修改，multichoice涉及到多条记录，按answer_id排序后修改。
			 *
			 */
			$is_update =  $objPHPExcel->getActiveSheet()->getCell("Q".$j)->getValue();
			$is_update = trim($is_update)>0 ? trim($is_update) : 0 ;


			if($is_update > 0){
				$k++;

				 if(is_array($item)){
					 $item['id'] = $item_id;
					 UPDATE::item_info_update($item_id,$item); //试题基本信息更新
				 }
				//echo '<br> item_id::'.$item['id'].'<br>'.var_export($item,true).' <br> is_update::'.$is_update;
				//echo '<br> item_info_update :: success';

				//标签、考点先删除后插入新内容
				if($att_data!=''){
					handle_attributes($item_id,$att_data);
				}//处理标签
				if($tags!=''){
					handle_tags($item_id,$tags);
				}//考点

				//echo '<br> handle_tags :: success';

				//处理试题父子关系
				$parent_id = $tem_arr[$item_parent_id]>0?$tem_arr[$item_parent_id]:0;
				DELETE::item_parent_delete($item_id);
				INSERT::item_parent_insert($item_id,$parent_id,$need_parent,0);  //插入层级关系

				//echo '<br> item_parent_delete :: success';


				if($item_basic_type == '选择题'){

					if($options == ''){
						DELETE::multichoice_answers_delete($item_id);
					}
					else{
						$answer = choice_option_handle($options);
						UPDATE::multichoice_answers_update($item_id,$answer['options']);  //选择题选项更新
						UPDATE::multichoice_properties_update($item_id,$answer['choice_multi'],$options_is_random);  //选择题设置更新
					}
				}

				if($item_basic_type == '客观填空题'||$item_basic_type == '主观填空题'||$item_basic_type == '试题材料'){

					if($item_basic_type == '客观填空题'){
						$is_objective = 1;
					}
					if($item_basic_type == '主观填空题'){
						$is_objective = 0;
					}

				  $answer = blank_filling_handle($options);

				  if($item_basic_type == '试题材料'){

					 $feedback = '';
					 if($answer['explain']!=''){

						$feedback = $answer['explain'];
						UPDATE::material_items_feedback_update($item_id,$feedback);  //题目材料解析更新
					 }
					 else
					 	DELETE::material_items_feedback_delete($item_id);
				  }else{

						if($options == '')
							DELETE::blank_filling_answers_delete($item_id);
						else
							UPDATE::blank_filling_answers_update($item_id,$is_objective,$answer);  //填空题答案更新
					UPDATE::blank_filling_properties_update($item_id, $blank_size);//填空题属性设置更新
				  }


				}


			  echo "第".$tmp_id."行已更新<br>";
			}

		}

	  }
	}
	 echo "共影响".$k."道试题<br>";

}

else{
//没有此流水号记录，或流水号为0，说明此表是第一次导入。


  $k = 0;
  for($j=3;$j<=$highestRow;$j++)
  {//循环读取excel文件,读取一条,插入一条


   	$tmp_id = 0;
	$tmp_id = $objPHPExcel->getActiveSheet()->getCell("A".$j)->getValue();//excel中编号

	$name = '';
	$name = $objPHPExcel->getActiveSheet()->getCell("B".$j)->getValue();//标题
	$item['name'] =trim($name);

	$item_parent_id = 0;
	$item_parent_id = $objPHPExcel->getActiveSheet()->getCell("C".$j)->getValue();//父级ID


	 $item_type = '';
	 $item_type = $objPHPExcel->getActiveSheet()->getCell("D".$j)->getValue();//考试题型
	 $item_type =trim($item_type);
	 if($item_type!=''){
		$item['type'] = handle_item_type($item_type);   //如果没有此题型,则插入;如有,返回id.
	 }else{
		$item['type'] = 0;
	 }

	 $item_basic_type = '';
	 $item_basic_type = $objPHPExcel->getActiveSheet()->getCell("E".$j)->getValue();//基本题型
	 $item_basic_type = trim($item_basic_type);
	 if($item_basic_type!=''){
		$item['basic_type'] = get_basic_type($item_basic_type);
	 }else{
		$item['basic_type'] = 0;
	 }

	$body = '';
	$body = $objPHPExcel->getActiveSheet()->getCell("F".$j)->getValue();//题干
	$item['body'] = trim($body);

   //是否依赖父级题
	$need_parent = $need_parent>0?$need_parent:0;
	$options =  $objPHPExcel->getActiveSheet()->getCell("G".$j)->getValue();
	$options = trim($options);

	$options_is_random = '';
	$options_is_random = $objPHPExcel->getActiveSheet()->getCell("H".$j)->getValue();  //选项是否随机
	$options_is_random = trim($options_is_random);
	$options_is_random = $options_is_random === ''?1:$options_is_random;

	$att_data = '';
	$att_data = $objPHPExcel->getActiveSheet()->getCell("I".$j)->getValue();  //考点
	$att_data = trim($att_data);

	$tags = $objPHPExcel->getActiveSheet()->getCell("J".$j)->getValue(); //标签
	$tags = trim($tags);

	$point = $objPHPExcel->getActiveSheet()->getCell("K".$j)->getValue();//分值
	$item['point'] =trim($point);
	$item['point'] = $item['point']>0?(float)$item['point']:0;

	 $time_limit =$objPHPExcel->getActiveSheet()->getCell("L".$j)->getValue();//时间限制
	 $item['time_limit'] =trim($time_limit);
	$item['time_limit'] = $item['time_limit']>0?(int)$item['time_limit']:0;


	//是否必须依赖父级关系
	$need_parent = 0;
	$need_parent = $objPHPExcel->getActiveSheet()->getCell("M".$j)->getValue();
	$need_parent = trim($need_parent);

	 $is_random = $objPHPExcel->getActiveSheet()->getCell("N".$j)->getValue();//子题目是否随机
	 $item['is_random'] = trim($is_random);
	 $item['is_random'] = $item['is_random']==1?1:2;

	 $fileurl = '';
	 $fileurl = $objPHPExcel->getActiveSheet()->getCell("O".$j)->getValue();   //附件处理
	 $files['fileurl'] = trim($fileurl);
	//处理附件
	 $item['file_id'] = 0;
	 if($files['fileurl']!=''){
		$item['file_id'] = handle_files($files);
	 }
	 $item['file_id']=$item['file_id']>0?$item['file_id']:0;


	$blank_size = 0;
	$blank_size =  $objPHPExcel->getActiveSheet()->getCell("P".$j)->getValue();
	$blank_size = trim($blank_size);
	if($blank_size=='') $blank_size = 0;


	 if(is_array($item)){
		 $item['id'] = $item['id']>0?(int)$item['id']:0;
		 $item_id = INSERT::item_info_insert($item); //试题基本信息录入
	 }

	 if($item_id>0){

		$k++;
		$item = insert_allcontent($item, $item_id, $excel_liushui, $tmp_id, $att_data, $tags, $tem_arr,  $item_parent_id, $need_parent, $options, $item_basic_type, $options_is_random, $blank_size);

	 }else{
		echo $item['title']."录入出错<br>";
	 }

   }
   echo "共导入".$k."道试题<br>";

}


}


/**
 * 插入所有的内容
 *
 * @param unknown_type $name
 * @param unknown_type $type
 * @return unknown
 */
function insert_allcontent($item, $item_id, $excel_liushui, $tmp_id, $att_data, $tags, &$tem_arr, $item_parent_id, $need_parent, $options, $item_basic_type, $options_is_random, $blank_size){

	 if($item_id>0){
		INSERT::item_excel_liushui_insert($item_id,$excel_liushui,$tmp_id,0);  //插入item_excel_liushui,stauts=0

		if($att_data!=''){
			handle_attributes($item_id,$att_data);
		}//处理标签

		if($tags!=''){
			handle_tags($item_id,$tags);
		}//考点



		//处理试题父子关系
		$tem_arr[0] = 0;
		$tem_arr[$tmp_id] = $item_id;
		$parent_id = $tem_arr[$item_parent_id]>0?$tem_arr[$item_parent_id]:0;
		INSERT::item_parent_insert($item_id,$parent_id,$need_parent,0);  //插入层级关系

		//if($options!=''){   //选项和解析处理(没有答案、解析的填空题，options==''，但是有空大小属性值，此时也应录入)

			if($item_basic_type == '选择题' && $options!=''){

				$answer = choice_option_handle($options);
				INSERT::multichoice_answers($item_id,$answer['options']);  //选择题选项录入
				INSERT::multichoice_properties_insert($item_id,$answer['choice_multi'],$options_is_random);  //选择题设置
			}

			if($item_basic_type == '客观填空题'||$item_basic_type == '主观填空题'||$item_basic_type == '试题材料'){

				if($item_basic_type == '客观填空题'){
					$is_objective = 1;
				}
				if($item_basic_type == '主观填空题'){
					$is_objective = 0;
				}

			  $answer = blank_filling_handle($options);

			  if($item_basic_type == '试题材料'){

				 $feedback = '';
				 if($answer['explain']!=''){

					$feedback = $answer['explain'];

					if($options!='')
						INSERT::material_items_feedback_insert($item_id,$feedback);  //题目材料解析录入
				 }

			  }else{

				if(!empty($options))
					INSERT::blank_filling_answers($item_id,$is_objective,$answer);  //填空题设置

				INSERT::blank_filling_properties($item_id, $blank_size);//填空题属性设置
			  }


			}


		 //}
		   echo "第".$tmp_id."行已导入<br>";

		   //更新item_excel_liushui,stauts=1
		   UPDATE::item_excel_liushui_update($item_id,$excel_liushui,$tmp_id,1);

	 }else{
		echo $item['title']."录入出错<br>";
	 }

	return $item;

}




function handle_attributes($item_id,$data){

     $data_id = INSERT::attributes_insert_and_get_id($data);  //插入考点，不能够获取相应Id
	DELETE::attributes_item_realtionship_delete($item_id);

    if(is_array($data_id)){
     	 foreach($data_id as $data_value){
     	 	INSERT::attributes_item_realtionship_insert($item_id,$data_value); //插入考点与试题对应关系
     	 }
     }

 }

 function handle_tags($item_id,$tags){

     $tags_id = INSERT::tags_insert_and_get_id($tags);  //插入标签，不能够获取相应Id
	DELETE::tags_item_realtionship_delete($item_id);

    if(is_array($tags_id)){
     	 foreach($tags_id as $tags_value){
             INSERT::tags_item_realtionship_insert($item_id,$tags_value); //插入标签与试题对应关系
     	 }
     }

 }

function handle_files($files){

   return INSERT::files_insert('link',$files);  //多个附件处理是否考虑到,fid以,隔开

}

function get_basic_type($type){

	switch($type){

		case '选择题':
		     return 1;
		     break;
	    case '主观填空题':
	    case '客观填空题':
		     return 2;
		     break;
		case '试题说明':
		     return 3;
		     break;
		case '试题材料':
		     return 4;
		     break;
	}

}



function handle_item_type($item_type){

	$type_id = API::query_exam_item_type('type_id',0,0,$item_type);
	if($type_id>0){
		//查询该题型是否存在
        $id = $type_id;

    }else{
	   $id =  INSERT::item_type_insert($item_type,'');

	}

 	return $id ;
}




function choice_option_handle($options){
//选择题选项处理
    $i = 0;
	if(strpos($options,"&&")){    //存在多个选项
		$option = explode("&&",$options);
        foreach ($option as $value){
            if(strpos($value,"||")){   //存在解析
            	$answer = explode("||",$value);
            	if($answer[0]!=''){   //选项不为空
                     $arr = choice_option_choose($answer[0]);
            	}
             	if($answer[1]!=''){
            		$arr['explain'] = $answer[1];
            	}
            	else $arr['explain'] = '123测试解析';
        	}else{    //没有||分隔符全部当成选项
        		 $arr = choice_option_choose($value);
        	}
        	if($arr['is_correct']==1){
        		$i++;
        	}

        	$return_arr['options'][] = $arr;
        }


	    if($i>1){
	    	$return_arr['choice_multi'] = 1;
	    }else{
	    	$return_arr['choice_multi'] = 0;
	    }

	}else{
		$arr['explain'] = $options;
		$return_arr['options'][] = $arr;
	}
	return  $return_arr;

}



function  choice_option_choose($answer){

    if(strpos($answer,"--")){  //选项是否正确
   			$option_arr = explode("--",$answer);
   			if($option_arr[0]!=''){
   			    $arr['is_correct'] = $option_arr[0];
   			}else{
   				$arr['is_correct'] = 0;
   			}

   		   if($option_arr[1]!=''){  //选项不为空
   			    $arr['answer'] = $option_arr[1];

   			}else{
   				 $arr['answer'] = '';
   			}

     }else{
     	    $arr['is_correct'] = 0;
     		$arr['answer'] = $answer;
    }


    return $arr;

}





function blank_filling_handle($answer){
//填空题选项处理

    $correct_answer = '';
	if(strpos($answer,"||")!==false){
		$answer_arr = explode("||",$answer);
		if($answer_arr[1]!=''){
			$arr['explain'] = $answer_arr[1];
		}else{
			$arr['explain'] = '';
         }

		if($answer_arr[0]!=''){
			$correct_answer = choice_option_choose($answer_arr[0]);  //分离答案
			if($correct_answer['answer']!=''){
			   $arr['answer'] = $correct_answer['answer'];
			}else{
			   $arr['answer'] = '';
            }

        }else{
			$arr['answer'] = '';
         }

	}else{
		$correct_answer = choice_option_choose($answer);  //分离答案

		if($correct_answer['answer']!=''){
			   $arr['answer'] = $correct_answer['answer'];
		}else{
			   $arr['answer'] = '';
        }
	}

	return  $arr;

}



/**
 * 判断上传的文件类型
 *
 * @param unknown_type $name
 * @param unknown_type $type
 * @return unknown
 */

function check_filetype($name,$type){
	 $extension = fileext($name);
	 if($extension == 'xls'|$extension == 'xlsx'){
	 	return true;
	 }else{
	 	return false;
	 }
}


/**
 * 获取文件后缀名
 *
 * @param unknown_type $filename
 * @return unknown
 */
function fileext($filename) {
    return trim(substr(strrchr($filename, '.'), 1, 10));
}



/**
 * 检查文件是否存在，如果存在就修改文件名（加上当前时间）
 *
 * @param unknown_type $filename
 * @return 返回文件完整路径名
 */

function check_file_exists($filename){

  $file_path = dirname(__FILE__)."/upload/excel_files/";
  $file=$file_path.$filename;
  if(file_exists($file)){
  	$filename = explode(".",$file);
	$time = date("YmdHis");
	$filename[0] .="-".$time;
	$file = implode(".",$filename);
   }
   return $file;
}






?>