(function($){
	$.fn.view_ques=function(id,options){
		var settings=$.extend({
			add_ques : false,
			ques_content_div : '',
			add_ques_link_div : ''
		},options||{});
		
	var that=this;
	$(that).html('<div class="loadging"><img width="25px" height="25px" src="images/loading.gif"></div>');
	
	$.getJSON('/itest/egs/index.php?op=item_info&type=detail&id='+id,function(data){	
		var ques_array=	data[id];
		
		var name=ques_array['name'];
		var created=ques_array['created'];
		var uid=ques_array['uid'];	//ps whx user
		
		var points=ques_array['points'];
		var time_limit=ques_array['time_limit'];		
		var ques_body=ques_array['body'];
		var description=ques_array['description'];
		var blank_size=ques_array['blank_size'];
		
		var is_random=ques_array['is_random'];//子题目是否随机 
		var is_objective=ques_array['is_objective'];//填空题是否为客观填空题
		var choice_multi=ques_array['choice_multi'];//选择题是否为多选题
		var choice_random=ques_array['choice_random'];//选择题答案顺序是否随机显示
		
		var answers_array=ques_array['answers'];//ps whx answers		
		var files=ques_array['files'];		//ps whx files
		
		var type=ques_array['type'];
		var basic_type=ques_array['basic_type'];
		
		var tags_array=ques_array['tags'];	

		var tags='';
		$.each(tags_array,function(index,tag){
			tags +=tag+',';
		});
		
		
		var basic_type_name='';
		var basic_type_id=1;
		$.each(basic_type,function(id,name){
			basic_type_id=id;
			basic_type_name=name;
		});
		var type_name='';
		var type_id=1;
		$.each(type,function(id,name){
			type_id=id;
			type_name=name;
		});
		
			
		var ques_pre='<div id="ques_display_'+id+'" class="ques_display">';
		var ques_suf='</div>';
		
		var ques_name='<div class="ques_name_score clearfix">	<div class="ques_name fleft"> <label >标题：</label> <span class="ques_name" >'+name+'</span></div>';
		var ques_created='<div class="ques_created fright"> <label >发布时间：</label> <span  class="ques_created" >'+created+'</span>	 </div>';
		var ques_user='<div class="ques_user fright"> <label >出题人：</label> <span  class="ques_user" >'+uid+'</span></div> </div>';
		
		var ques_basic_type='<div class="ques_basetype_type clearfix"><div class="ques_basic_type fleft"> <label >所属基本题型：</label> <span class="ques_basic_type" >'+basic_type_name+'</span></div>';
		var ques_type='<div class="ques_type"> <label >所属题型：</label> <span  class="ques_type" >'+type_name+'</span>	 </div> </div>';
		
		
		var ques_time_limit='<div class="ques_name_score clearfix">	<div class="ques_time fleft"> <label >时间：</label> <span class="ques_time" >'+time_limit+'</span> <span class="unit">分钟</span>    </div>';
		var ques_points=' <div class="ques_points"> <label>分值：</label> <span class="points" >'+points+'</span> <span class="unit">分</span>    </div> </div>';
		
		var ques_body='<div class="ques_body"> <label>题干：</label> <span class="ques_body" >'+ques_body+'</span></div>';
		
		var ques_description='<div class="ques_description">	 <label >描述：</label>	 <span class="ques_description" >'+description+'</span> </div> ';
		
		
		
		
		var ques_is_random='';
		var ques_answers='';
		var ques_multi_object='';
		var ques_blank_size='';
		
		if(basic_type_id==3||basic_type_id==4){
			
			var is_random_name='';
			if(is_random==1) is_random_name ='是'; else if(is_random==2) is_random_name ='否';
			ques_is_random='<div class="subques_random"><label>子题目顺序是否随机：<span class="subques_random">'+is_random_name+'</span>  </div>';
			
			if(basic_type_id==3){
				ques_description='';
			}
		}else 
		if(basic_type_id==1){
			
			var choice_multi_name='';
			if(choice_multi==1) choice_multi_name ='是'; else if(choice_multi==0) choice_multi_name ='否';
			ques_multi_object ='<div class="ques_is_multichoice">  <label for="ques_is_multichoice">是否多选题：</label>   <span class="ques_is_multichoice">'+choice_multi_name+'</span> </div>';
								
									
			var choice_random_name='';
			if(choice_random==1) choice_random_name ='是'; else if(choice_random==0) choice_random_name ='否';
			ques_answers ='<div class="answer_is_random"> <label>答案是否随机：</label> <span class="answer_is_random" >'+choice_random_name+'</span></div>';								
			 
			ques_answers +='<table id="ques_answer">  <caption >答案</caption>  <thead><th class="answer_correct">正确</th><th class="answer_option">选项</th><th class="answer_analyse">答案解析</th></thead>  <tbody>';
						$.each(answers_array,function(index,answer){
							var correct_img='';
							//alert(answer);
							if(answer['is_correct']==1) correct_img='<img src="resource/images/correct.png"/>';
							
							ques_answers +='<tr><td>'+correct_img+'</td><td>'+answer["answer"]+'</td>    <td>'+answer["feedback"]+'</td>  </tr> ';
						});
						ques_answers +='</tbody></table>';
		}
		else 
		if(basic_type_id==2){
			
			var is_objective_name='';
			if(is_objective==1) is_objective_name ='是'; else if(is_objective==0) is_objective_name ='否';
			ques_multi_object ='<div class="ques_is_objective">  <label for="ques_is_objective">是否客观填空题：</label>   <span class="ques_is_objective">'+is_objective_name+'</span> </div>';
				
			if(blank_size!=undefined){
				if(blank_size==null || blank_size==0) blank_size='默认大小';
				else if(blank_size==1) blank_size='一个单词';
				else if(blank_size==2) blank_size='一句话';
				else if(blank_size==3) blank_size='一段文字';
				else blank_size='';
				
				ques_blank_size='<div class="ques_blank_size">  <label for="ques_blank_size">填空题空大小：</label>   <span class="ques_blank_size">'+blank_size+'</span> </div>';		
			}	
						
			$.each(answers_array,function(index,answer){
				ques_answers ='<div class="ques_answer">    	<div class="answer_correct">	<label>正确答案：</label>   <span class="answer_correct">'+answer["answer"]+'</span>   </div>      <div class="answer_analyse">   <label>答案解析：</label> <span class="answer_analyse">'+answer["feedback"]+'</span>    </div>     </div>  ';
			});
									
		}
		
		
		
		
		
		var ques_files='<div class="ques_files">	<label >附件：</label>	<span class="ques_has_files" >'+files+'	</span></div>';
		
		var ques_tags='<div class="ques_tags">	<label class="ques_tags_label">标签：</label> <span class="tags" >'+tags+'</span>	 </div>';
		
		
		
		$(that).html(ques_pre+ques_name+ques_created+ques_user+ques_basic_type+ques_type+ques_time_limit+ques_points+ques_body+ques_description+ques_is_random+ques_multi_object+ques_blank_size+ques_answers+ques_files+ques_tags+ques_suf);
		
	
	if(basic_type_id==1||basic_type_id==2) $(settings.add_ques_link_div).empty();//当前题目是选择题和描述题时，清空add_ques_link_div，也就是查询的列表页
	
	if(settings.add_ques!=false&&basic_type_id!=1&&basic_type_id!=2) //当查看题目时，需要创建子题目（即settings.add_ques==true时），并且当前题目不是选择题和描述题时，显示添加子题目的链接
		$(settings.add_ques_link_div).html('<div id="basetype_button" style="display:block;">	<a href="javascript:void(0);" onclick=\'open_basetype('+id+',{ques_content_div:"'+settings.ques_content_div+'",add_ques_link_div:"'+settings.add_ques_link_div+'"})\'>请选择子题目试题类型</a><span class="error"></span></div>');
	
	});
	return false;
}
})(jQuery);
