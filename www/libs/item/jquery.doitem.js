(function($){
	$.fn.do_ques=function(id){
		
	var that=this;
	$(that).html('loading...');
	
	$.getJSON('/itest/egs/index.php?op=item_info&type=detail&id='+id,function(data){	
	
		var ques_array=	data[id];
		
		var serial_name=ques_array['serial_name'];
		var serial_number=ques_array['serial_number'];
		
		var name=ques_array['name'];
		//var created=ques_array['created'];
		//var uid=ques_array['uid'];	//ps whx user
		
		var points=ques_array['points'];
		var time_limit=ques_array['time_limit'];		
		var question_body=ques_array['body'];
		var description=ques_array['description'];
		
		//var is_random=ques_array['is_random'];//子题目是否随机 
		//var is_objective=ques_array['is_objective'];//填空题是否为客观填空题
		var choice_multi=ques_array['choice_multi'];//选择题是否为多选题
		//var choice_random=ques_array['choice_random'];//选择题答案顺序是否随机显示
		
		var answers_array=ques_array['answers'];//ps whx answers		
		var files=ques_array['files'];		//ps whx files
		
		//var type=ques_array['type'];
		var basic_type=ques_array['basic_type'];
		
	
		//var basic_type_name='';
		var basic_type_id=1;//1:选择题；2：填空题；3：描述类；4：父级题目
		$.each(basic_type,function(id,name){
			basic_type_id=id;
		//	basic_type_name=name;
		});
		
		
			
		var ques_pre='<div id="ques_display_'+id+'" class="ques_display"><form id="do_ques" action="" method="post">';
		var ques_suf='<a href="javascript:tijiao();">提交</a></form></div>';
		
		var ques_name='',ques_serial_name='',ques_serial_number='',ques_time_limit='',ques_points='',ques_body='',ques_description='',ques_answers='',ques_files='';
		
		var ques_serial_name_body='';
		
		ques_time_limit=time_limit;
		
		ques_points=points;
		
		
		if(serial_name!=undefined&&serial_name!='') ques_serial_name='<span class="ques_serial_name"> '+serial_name+'</span>';
		if(serial_number!=undefined&&serial_number!='') ques_serial_number='<span class="ques_serial_number"> '+serial_number+'</span>';
		
		if(question_body!=undefined&&question_body!='') ques_body='<div class="ques_body"> '+question_body+'</div>';
		
		if(description!=undefined&&description!='') ques_description='<div class="ques_description">'+description+'</div> ';
			
		if(basic_type_id==3){
			if(name!=undefined&&name!='')	ques_name='<span class="ques_name"> '+name+'</span>';
			
			if(ques_serial_name!=''||ques_serial_number!=''||ques_name!='')	ques_serial_name_body='<div class="serial_name_num_name">'+ques_serial_name+ques_serial_number+ques_name+'</div>'+ques_body;
				
				ques_description='';
			
		}
		else {
			
			if(ques_serial_name!=''||ques_serial_number!=''||ques_body!='')	ques_serial_name_body='<div class="serial_name_num_body">'+ques_serial_name+ques_serial_number+ques_body+'</div>';
			
			
			if(basic_type_id==1){
				
				ques_answers ='<table id="ques_answer">  <tbody>';
				
				var serial_number={"1":"A)","2":"B)","3":"C)","4":"D)","5":"E)","6":"F)","7":"G)","8":"H)","9":"I)","10":"J)","11":"K)","12":"L)","13":"M)","14":"N)","15":"O)","16":"P)","17":"Q)","18":"R)","19":"S)","20":"T)","21":"U)","22":"V)","23":"W)","24":"X)","25":"Y)","26":"Z)"};
				var i=0;//为计算ABCD
				
				if(choice_multi==1){ 
					$.each(answers_array,function(index,answer){
						i++;
						var input_name='answer['+index+'][useranswer]';
						ques_answers +='<tr><td><input class="useranswer" type="checkbox" name="'+input_name+'"  value="'+index+'"/></td><td>'+serial_number[i]+'</td>    <td>'+answer["answer"]+'</td>  </tr> ';
					});
							
				}
				else if(choice_multi==0){
					$.each(answers_array,function(index,answer){
						i++;
						ques_answers +='<tr><td><input class="useranswer" type="radio" name="answer[useranswer]" value="'+index+'"/></td><td>'+serial_number[i]+'</td>    <td>'+answer["answer"]+'</td>  </tr> ';
					});
				}
				
				ques_answers +='</tbody></table>';
							
			}
			else 
			if(basic_type_id==2){
				ques_answers ='<div class="ques_answer"><input class="useranswer" type="text" name="answer[useranswer]" class="underline_blank"/> </div>';
													
			}
		}
		
		
		
		
		if(files!=undefined&&files!='') ques_files='<div class="ques_files">	<label >附件：</label>	<span class="ques_has_files" >'+files+'	</span></div>';
		
		
		$(that).html(ques_pre+ques_serial_name_body+ques_description+ques_answers+ques_files+ques_suf);
		
	});
	
	//return false;
}
})(jQuery);