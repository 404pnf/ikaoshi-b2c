<?php
if(isset($_POST['submit'])&&$_POST['paper_id']>0&&$_POST['user_id']>=0){

	//file_put_contents("aaa.txt", 'user_answer_post :\r\n'.$_POST['answer'].'\r\n');
	//var_dump($_POST);

	require 'config.php';
	require_once 'include/user_results.inc';

   	$result_info['time_used'] = $_POST['use_time']>0?$_POST['use_time']:0;
	$user_id = $_POST['user_id']>0?$_POST['user_id']:0;
	$paper_id = $_POST['paper_id']>0?$_POST['paper_id']:0;
	$score = 0;   //用户得分
    $id =   INSERT::user_result_insert($paper_id,$user_id,$result_info);
	$answer  = $_POST['answer'];

	$detail_points = array();

	if($id>0&is_array($answer)){
		$wrong_arr = array();

		foreach ($answer as $key=>$value){

		  if($key>0){

			$is_correct = 0;
			$user_answer = '';
			$this_item_points = '';
			$award_points = '';

			$this_item_points = RESULTS::get_exam_item_points($paper_id,$key);
			$this_item_points = $this_item_points>0?$this_item_points:0;

			if($value['basic_type']==1|($value['basic_type']==2&&$value['is_objective']==1)){

				$objective_points += $this_item_points;

			}


			if($value['useranswer']!=''){
				if($value['basic_type']==1){  //选择题
					if($value['choice_multi']==1){		  //多选
					    if(is_array($value['useranswer']))
					        $user_answer = implode(",",$value['useranswer']);
					        $is_correct = RESULTS::check_multichoice_answers($key,$value['useranswer']);   //验证选择题答案
					}else{
						$user_answer = $value['useranswer'];
					    $is_correct = RESULTS::check_singlechoice_answers($key,$user_answer);   //验证选择题答案
					}
				}

				if($value['basic_type']==2){  //填空题
				   $user_answer = trim($value['useranswer']);
				   if($value['is_objective']==1){   //客观填空题，验证得分
						$is_correct = RESULTS::check_blankfilling_answers($key,$user_answer) ; //验证填空题答案
				   }else{                //非客观题
			        	$is_correct = 0;
			       }
				}


				if($is_correct){

					$score += $this_item_points;
				    $detail =  RESULTS::find_parent_and_add_points($key,$this_item_points,$answer);
            	    foreach($detail as $detail_key=>$detail_value){
	   	                 $arr[$detail_key]['score'] += $this_item_points;
	                }

	              //  $arr[$key]['score'] = $this_item_points;
	                $award_points = $this_item_points;

				}else{
					$award_points = 0;

				}

			 	if($value['basic_type']==1){

			        INSERT::multichoice_user_answer_insert($key,$id,$user_answer,$award_points);
			 	}

			 	if($value['basic_type']==2){

			        INSERT::blankfilling_user_answer_insert($key,$id,$user_answer,$award_points);

			 	}

			 	if($value['basic_type']==1|($value['basic_type']==2&&$value['is_objective']==1)){

			 		//插入错题库需要的试题数组
			 		$wrong_arr[] = array(
			 				"item_id"=>$key,
			 				"result_id"=>$id,
			 				"paper_id"=>$paper_id,
			 				"is_correct"=>$is_correct
			 		);

			 	}
			  }
		  }
	  }


	if(is_array($arr)){
		foreach ($arr as $key=>$value){
			if($value['score']>0){
				$all_points[$key] = $value['score'];
			}
		}
	}


	  $this_exam_objective_points = RESULTS::get_exam_objective_points($paper_id);
	  $this_exam_objective_points = $this_exam_objective_points>0?$this_exam_objective_points:0;

	  $rate = 0.00000;
	  $rate = $score/$this_exam_objective_points;
	  RESULTS::update_user_result_total_points($id,$score,$rate);
	  //RESULTS::update_user_result_total_points($id,$score);
	  INSERT::user_result_detail_points_insert($id,$all_points);


	  //here 插错题库
		$wrong_status = 0;
		if(!empty($wrong_arr)){
			include_once 'include/user_wrong_answer.php';
			$uwa = new UserWrongAnswer();
			$wrong_result = $uwa->record_user_wrong_answer($user_id, $wrong_arr);
			if($wrong_result) $wrong_status = 1;
		}


		$arr = array("id" => $id,
				"status" => 1,
				"wrong_status" => $wrong_status
		);

	 }else{

	  $arr = array(
                  "status" => 0
        );

	}

	 echo COMMON::my_json_encode($arr);

}














?>