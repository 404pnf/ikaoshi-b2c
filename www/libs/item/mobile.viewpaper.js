var serial=serial_number;
var paper_maincontent;
var adminid;
var time_limit=0;
var paper_backto = '';

(function($){
	$.fn.view_paper=function(id,options){
		var settings=$.extend({
			titleContent:'.title',
			user_id:'0',
			backto:''
		},options||{});
	var that=this;
	$(that).html('');
	paper_maincontent=that;
	paper_backto = settings.backto;

	$.getJSON('/itest/egs/index.php?op=paper_info&type=detail&id='+id,function(data){

		var paper_array=data[id];

		var paper_name=paper_array['paper_name'];
		var examType=paper_array['exam_type'];
		var sid = paper_array['sid'];

		var description=paper_array['description'];
		var points=parseInt(paper_array['points']);
		var time_limit=paper_array['time_limit'];
		var amount=paper_array['amount'];

		$(settings.titleContent).html('<span id="title_f">'+paper_name+'</span>');

		var	huanyitao = '<a class="fright" href="create.html?sid='+sid+'&backto='+settings.backto+'" >换一套</a>';

		var output='<div class="paper_info">'+
		'<div class="score">'+huanyitao+'总分：'+points+' 总题量：'+amount+'</div>'+
		'<div class="atime">时长：<span class="time_limit">'+time_limit+'</span><a id="stop" href="javascript:stop_t();" > 暂停 </a><a id="start" href="javascript:start_t();" style="display:none;"> 启动 </a></div>'+
		'<div class="paper_description">'+description+'</div></div>';

		$(that).html(output);

		window.setTimeout(
			$('.time_limit').jQueryTimer(time_limit)
		,300);//时间倒计时

		var url = '/itest/egs/index.php?op=paper_item&id='+id;

		$.getJSON(url,function(resp){


			try
			{

				var paper_array=resp;
				var output='<div class="paper"><form id="do_ques_form" action="/itest/egs/user_answer_post.php" method="post"><input type="hidden" name="paper_id" value="'+id+'"/><input type="hidden" name="user_id" value="'+settings.user_id+'"/><input type="hidden" name="use_time" class="use_time" value="0"/><div class="allSubques">';
				var layout=0;
				var sumlay=0;
				$.each(paper_array,function(index,items){
						sumlay++;
				});
				$.each(paper_array,function(index,items){
					var subques_array=items;
					var pid=0;
						layout++;
					if(subques_array!=-1)	output +=partsubmit(subques_array,pid,layout,sumlay);
				});
					output +='</div><input type="submit" class="submit notprint" name="submit" value="Submit"/></form></div>';

				$(that).append(output);


				needfun();
				backToTop();


				//add_ques_form 的提交
				var add_ques = {
					url:		'/itest/egs/user_answer_post.php',
					type:		'post',
					target:   	'#globalmessage',// target element to update
					dataType: 	'json',
					beforeSubmit:  clearInterval(timer),  // pre-submit callback
					//beforeSubmit:  showRequest,
					success:     open_by_status //提交后进行返回状态的判断，并确定是否创建子题目
				};
				$('#do_ques_form').ajaxForm(add_ques);

				start_t(time_limit);


					}
					catch (err)
					{
						resp = undefined;
						$(that).append('服务器交互失败，请重试'+err);
					}


			});//试题详细信息输出

		});//试卷基本信息的输出

   };
})(jQuery);






function showRequest(formData, jqForm) {
   $("#globalmessage").html('About to submit: \n\n' + $.param(formData));
	return true;
}

function open_by_status(data){

	var status=data.status;
	var id=data.id;

	if(status==1) {
		serial_total=0;
		clearInterval(timer);
		$(paper_maincontent).view_userresult(id,{operation:"DiagnosticReport",titleContent:".paper_title",backto:encodeURIComponent(paper_backto)});
		//location.href='paperdiagnosis.html?id='+id;
	}
	else if(status==0)
		$("#globalmessage").html('保存失败，名称重复或缺少必要信息');
	else
		$("#globalmessage").html('服务器交互失败，请重试');
}




var serial_total=0;//设置全局的题目编号




function partsubmit(subques_array,pid,layout,sumlay){

	var question='';

	var ques_array = subques_array;
	var item_id = ques_array['item_id'];

	var serial_name = ques_array['serial_str'];
	var serial_num_id = ques_array['serial_num'];
		//数据库中存储的serial_num样式的id号
	if(serial_num_id!=undefined)
		var serial_num = serial[serial_num_id][layout];
		//根据以上id从serial_number.js的serial_number数组中获取对应的样式表，并根据本题所属层级（layout）获得具体的样式编号。

	var name=ques_array['name'];
	var points=parseFloat(ques_array['points']).toFixed(1);
	var time_limit=ques_array['time_limit'];
	var question_body=ques_array['body'];

	var is_objective=ques_array['is_objective'];//填空题是否为客观填空题
	var blank_size=ques_array['blank_size'];//填空题空格大小
	var choice_multi=ques_array['choice_multi'];//选择题是否为多选题
	//var choice_random=ques_array['choice_random'];//选择题答案顺序是否随机显示
	var answers_array=ques_array['answers'];

	var files=ques_array['files'];
	 	file_status = files['file_status'];
		if(file_status == 1){
			file_type = files['file_type'];
			fileurl = files['fileurl'];
		}

	var type=ques_array['type'];

	var type_name="";//听力、阅读、完形、改错等
	if(type!=undefined&&type!='')
		$.each(type,function(tid,tname){
			type_name=tname;
		});

	var basic_type=ques_array['basic_type'];

	var basic_type_id=1;//1:选择题；2：填空题；3：描述类；4：父级题目
	if(basic_type!=undefined&&basic_type!='')
		$.each(basic_type,function(tid,tname){
			basic_type_id=tid;
		});
	//-----end 基本的试题信息------



	//-----为试题信息添加html标签------
	var ques_pre='<div id="ques_display_'+item_id+'" class="ques_display ques_display_part ques_display_'+layout+'"><input type="hidden" name="answer['+item_id+'][pid]" value="'+pid+'"/>';

	var ques_suf='</div>';

	var ques_name='',ques_serial_name='',ques_serial_number='',ques_time_limit='',ques_points='',ques_body='',ques_description='',ques_answers='',ques_files='';


	if(serial_name!=undefined&&serial_name!=''&&serial_name!=0)
		ques_serial_name='<span class="ques_serial_name"> '+serial_name+'</span>';
	if(serial_num!=undefined&&serial_num!='')
		ques_serial_number='<span class="ques_serial_number"> '+serial_num+'</span>';
	if(time_limit!=undefined&&time_limit!=''&&time_limit!=0)
		ques_time_limit='<span class="ques_time_limit">( '+time_limit+' min )</span>';
	if(points!=undefined&&points!='')
		ques_points='<span class="ques_score">（'+points+'分）</span>';
	if(question_body!=undefined&&question_body!='')
		ques_body='<span class="ques_body"> '+question_body+'</span>';

	if(file_status==1&&fileurl.length>0){
		ques_files='<div class="jplayer_wrap"><a id="jplayer-'+item_id+'" class="mp3 jplayer notprint" src='+fileurl+'></a></div> ';

		ques_pre='<div id="ques_display_'+item_id+'" class="ques_display hasmp3 ques_display_part ques_display_'+layout+'"><input type="hidden" name="answer['+item_id+'][pid]" value="'+pid+'"/>';

	}


	if(basic_type_id==4&&question_body!=undefined&&question_body!=''){
		//题干类试题的题干中，可能会含有与子题目编号相对应的编号，通过以下方法把样式的标记转换成编号

		var txt=question_body;
		var regSign=/<\d>/;
		var regSerNum=/\d/;
		var i=serial_total+1;
		var num=regSerNum.exec(regSign.exec(txt));
		var serNums;

		if(num!=null){

			serNums=serial[num];
			var retxt=txt.replace(regSign.exec(txt),serNums[i]);
			do{
				i++;
				txt=retxt;

				num=regSerNum.exec(regSign.exec(txt));
				if(num!=null) serNums=serial[num];

				retxt=retxt.replace(regSign.exec(txt),serNums[i]);

			}while(txt!=retxt);

			question_body=txt;
			ques_body='<div class="ques_body"> '+question_body+'</div>';

		}

	}

	var ques_serial_ques_name='';

	if(basic_type_id==3){
	//为描述类的题设定特殊的编号+名称+body的表现形式
		if(type_name!=undefined&&type_name!='')	ques_name='<span class="ques_name"> '+type_name+'</span>';

		if(ques_body!=''&&ques_body!=undefined)
			ques_body = '<div class="ques_body_description"><span class="description_label">Directions</span>'+ques_body+'</div>';

		if(ques_serial_name!=''||ques_serial_number!=''||ques_files != ''){
			ques_serial_ques_name = ques_files + '<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+'</span></div>';
		}
		if((ques_serial_name!=''||ques_serial_number!=''||ques_files != '')&&(ques_body==''||ques_body==undefined)){

				ques_serial_ques_name = ques_files + '<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+'</span></div>';

		}

	}
	else {

			//为非描述类试题生成统一的编号样式
			if(basic_type_id == 4 && (ques_serial_name!=''||ques_serial_number!=''||ques_files != ''))	ques_serial_ques_name='<div class="serial_name_num">'+ques_files+ques_serial_name+ques_serial_number+'</div>';

			if(basic_type_id == 1 || basic_type_id == 2){
				ques_serial_number = ++serial_total+'.&nbsp;';
				ques_serial_ques_name = ques_files+'<span class="serial_total">'+ques_serial_name+ques_serial_number+'</span>';
			}

			//选择题与填空题的答案形式
			if(basic_type_id==1){
				//选择题的答案的输出形式，包括多选与单选题，它们的input的type与name不同
				ques_answers ='<div id="ques_answer">';

				ques_answers +='<input type="hidden" name="answer['+item_id+'][basic_type]" value="'+basic_type_id+'"/><input type="hidden" name="answer['+item_id+'][choice_multi]" value="'+choice_multi+'"/>';

				var serial_number={"1":"A) ","2":"B) ","3":"C) ","4":"D) ","5":"E) ","6":"F) ","7":"G) ","8":"H) ","9":"I) ","10":"J) ","11":"K) ","12":"L) ","13":"M) ","14":"N) ","15":"O) ","16":"P) ","17":"Q) ","18":"R) ","19":"S) ","20":"T) ","21":"U) ","22":"V) ","23":"W) ","24":"X) ","25":"Y) ","26":"Z) "};
				var i=0;//为计算ABCD

				if(choice_multi==1){
					$.each(answers_array,function(index,answer){
						i++;
						var input_name='answer['+item_id+'][useranswer]['+index+']';
						ques_answers +='<div class="options"><input id="answer_'+index+'"  class="useranswer" type="checkbox" name="'+input_name+'"  value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
					});

				}
				else if(choice_multi==0){
					$.each(answers_array,function(index,answer){
						i++;
						ques_answers +='<div class="options"><input id="answer_'+index+'"  class="useranswer" type="radio" name="answer['+item_id+'][useranswer]" value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
					});
				}

				ques_answers +='</div>';

			}
			else
			if(basic_type_id==2){

				ques_answers ='<input type="hidden" name="answer['+item_id+'][basic_type]" value="'+basic_type_id+'"/><input type="hidden" name="answer['+item_id+'][is_objective]" value="'+is_objective+'"/>';


				//if(answers_array!=undefined&&answers_array!=''){

							if(blank_size=='' || blank_size==null || blank_size==0)	ques_answers+='<span class="ques_answer"><input class="useranswer underline_blank underline_blank_20" size="20" type="text" name="answer['+item_id+'][useranswer]" /></span>';
							else if(blank_size==1)  ques_answers+='<span class="ques_answer"><input class="useranswer underline_blank underline_blank_12" size="12" type="text" name="answer['+item_id+'][useranswer]" /></span>';
							else if(blank_size==2)  ques_answers+='<div class="ques_answer"><input class="useranswer underline_blank underline_blank_100" size="100" type="text" name="answer['+item_id+'][useranswer]" /></div>';
							else if(blank_size==3)
							ques_answers+='<div class="ques_answer notprint"><textarea rows="10" cols="70"  class="ckeditor_basic"  name="answer['+item_id+'][useranswer]" type="text"> </textarea></div>';
							//ques_answers+='<div class="ques_answer"><textarea rows="10" cols="70" class="useranswer"  name="answer['+item_id+'][useranswer]" ></textarea></div>';
							else ques_answers +='<span class="ques_answer"><input class="useranswer underline_blank" type="text" name="answer['+item_id+'][useranswer]" /> </span>';


				//}



			}
		}



	if(basic_type_id == 1 || basic_type_id == 2)
		question +=ques_pre+ques_serial_ques_name+'<div class="ques_content">'+ques_body+ques_answers+'</div>';
	else
		question +=ques_pre+ques_serial_ques_name+ques_body;



	var subques_array=ques_array['subque'];


	if(subques_array!=-1&&subques_array!=undefined)
		question +=quesoutput(subques_array,item_id);
	//如果本题目有下一级的子题目，则递归显示子题目。
	question +=ques_suf;//子题目输出完后闭合本试题。


	var is_submit=ques_array['is_submit'];//是否为提交点
	if(is_submit==1 && sumlay != 1)
	{
		//tempdata=new Array();
		//tempdata.push(items);
		//

			if(layout == 1)
				question +='<div class="submit_button  notprint"><a class="disnext" href="javascript:void(0);">下一页</a></div></div><div class="allSubques" style="display:none;">';
			else if(layout == sumlay)
				question +='<div class="submit_button notprint"><a class="disprev" href="javascript:void(0);">上一页</a></div>';
			else	question +='<div class="submit_button notprint"><a class="disprev" href="javascript:void(0);">上一页</a><a class="disnext" href="javascript:void(0);">下一页</a></div></div><div class="allSubques" style="display:none;">';

		return question;

	}

	//question +='<div class="disprev">上一步</div>';
	return question;
	//本道试题全部输出完毕（包括其子题目的所有层级）后，返回本试题的所有html

}













function quesoutput(subques_array,pid){
	//用来递归显示所有试题的函数，加入了html标签，并且通过基本题型（basic_type）的判断，试题格式显示为不同的样式。

	var question='<div class="allSubques">';
	var layout=0;


	$.each(subques_array,function(index,items){

		//----获得基本的试题信息----
		layout++;
		//alert(layout);

		var item_id=items['item_id'];

		var serial_name=items['serial_str'];
		var serial_num_id=items['serial_num'];//数据库中存储的serial_num样式的id号
		if(serial_num_id!=undefined)
			var serial_num=serial[serial_num_id][layout];//根据以上id从serial_number.js的serial_number数组中获取对应的样式表，并根据本题所属层级（layout）获得具体的样式编号。

		var ques_array=items;

		var name=ques_array['name'];
		var points=parseFloat(ques_array['points']).toFixed(1);
		var time_limit=ques_array['time_limit'];
		var question_body=ques_array['body'];
		var description=ques_array['description'];

		var is_objective=ques_array['is_objective'];//填空题是否为客观填空题
		var choice_multi=ques_array['choice_multi'];//选择题是否为多选题
		var blank_size=ques_array['blank_size'];
		//var choice_random=ques_array['choice_random'];//选择题答案顺序是否随机显示

		var answers_array=ques_array['answers'];
		var files=ques_array['files'];		//ps whx files
			file_status = files['file_status'];
			if(file_status == 1){
				file_type = files['file_type'];		fileurl = files['fileurl'];
			}

		var basic_type=ques_array['basic_type'];

		var basic_type_id=1;//1:选择题；2：填空题；3：描述类-试题说明；4：试题材料
		if(basic_type!=undefined&&basic_type!='')
			$.each(basic_type,function(tid,tname){
				basic_type_id=tid;
			});
		//-----end 基本的试题信息------



		//-----为试题信息添加html标签------
		var ques_pre='<div id="ques_display_'+item_id+'" class="ques_display ques_display_'+layout+'"><input type="hidden" name="answer['+item_id+'][pid]" value="'+pid+'"/>';

		var ques_suf='</div>';

		var ques_serial_name='',ques_serial_number='',ques_time_limit='',ques_points='',ques_body='',ques_description='',ques_answers='',ques_files='';

		var ques_serial_ques_name='';


		if(serial_name!=undefined&&serial_name!=''&&serial_name!=0) ques_serial_name='<span class="ques_serial_name"> '+serial_name+'</span>';
		if(serial_num!=undefined&&serial_num!='') ques_serial_number='<span class="ques_serial_number"> '+serial_num+'</span>';
		if(points!=undefined&&points!='') ques_points='<span class="ques_score">（'+points+'分）</span>';
		if(question_body!=undefined&&question_body!='') ques_body='<span class="ques_body"> '+question_body+'</span>';

		if(file_status==1&&fileurl.length>0){
			ques_files='<div class="jplayer_wrap"><a id="jplayer-'+item_id+'" class="mp3 jplayer notprint" src='+fileurl+'></a></div> ';
			ques_pre='<div id="ques_display_'+item_id+'" class="ques_display hasmp3 ques_display_'+layout+'"><input type="hidden" name="answer['+item_id+'][pid]" value="'+pid+'"/>';

		}

		if(basic_type_id==4&&question_body!=undefined&&question_body!=''){
			//题干类试题的题干中，可能会含有与子题目编号相对应的编号，通过以下方法把样式的标记转换成编号

			var txt=question_body;
			var regSign=/<\d>/;
			var regSerNum=/\d/;
			var i=serial_total+1;
			var num=regSerNum.exec(regSign.exec(txt));
			var serNums;

			if(num!=null){

				serNums=serial[num];
				var retxt=txt.replace(regSign.exec(txt),serNums[i]);
				do{
					i++;
					txt=retxt;

					num=regSerNum.exec(regSign.exec(txt));
					if(num!=null) serNums=serial[num];

					retxt=retxt.replace(regSign.exec(txt),serNums[i]);

				}while(txt!=retxt);

				question_body=txt;
				ques_body='<div class="ques_body"> '+question_body+'</div>';

			}

		}

		if(basic_type_id==3){
		//为描述类的题设定特殊的编号+名称+body+description的表现形式
		//	if(name!=undefined&&name!='')	ques_name='<span class="ques_name"> '+name+'</span>';

			if(ques_body!=''&&ques_body!=undefined)
				ques_body = '<div class="ques_body_description"><span class="description_label">Directions:</span>'+ques_body+'</div>';
			if(ques_serial_name!=''||ques_serial_number!=''||ques_files != ''){
				ques_serial_ques_name='<div class="serial_name_num_name">'+ques_files+ques_serial_name+ques_serial_number+'</div>';
			}
			if((ques_serial_name!=''||ques_serial_number!=''||ques_files != '')&&(ques_body==''||ques_body==undefined))
				ques_serial_ques_name='<div class="serial_name_num_name">'+ques_files+ques_serial_name+ques_serial_number+'</div>';


		}
		else {

			//为非描述类试题生成统一的编号样式
			if(basic_type_id == 4 && (ques_serial_name!=''||ques_serial_number!=''||ques_files != ''))		ques_serial_ques_name='<div class="serial_name_num">'+ques_files+ques_serial_name+ques_serial_number+'</div>';

			if(basic_type_id == 1 || basic_type_id == 2){
				ques_serial_number = ++serial_total+'.&nbsp;';
				ques_serial_ques_name = ques_files+'<span class="serial_total">'+ques_serial_name+ques_serial_number+'</span>';
			}

			//选择题与填空题的答案形式
			if(basic_type_id==1){
				//选择题的答案的输出形式，包括多选与单选题，它们的input的type与name不同
				ques_answers ='<div id="ques_answer">';


				ques_answers +='<input type="hidden" name="answer['+item_id+'][basic_type]" value="'+basic_type_id+'"/><input type="hidden" name="answer['+item_id+'][choice_multi]" value="'+choice_multi+'"/>';

				var serial_number={"1":"A) ","2":"B) ","3":"C) ","4":"D) ","5":"E) ","6":"F) ","7":"G) ","8":"H) ","9":"I) ","10":"J) ","11":"K) ","12":"L) ","13":"M) ","14":"N) ","15":"O) ","16":"P) ","17":"Q) ","18":"R) ","19":"S) ","20":"T) ","21":"U) ","22":"V) ","23":"W) ","24":"X) ","25":"Y) ","26":"Z) "};
				var i=0;//为计算ABCD

				if(choice_multi==1){
					$.each(answers_array,function(index,answer){
						i++;
						var input_name='answer['+item_id+'][useranswer]['+index+']';
						ques_answers +='<div class="options"><input id="answer_'+index+'"  class="useranswer" type="checkbox" name="'+input_name+'"  value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
					});

				}
				else if(choice_multi==0){
					$.each(answers_array,function(index,answer){
						i++;
						ques_answers +='<div class="options"><input id="answer_'+index+'"  class="useranswer" type="radio" name="answer['+item_id+'][useranswer]" value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
					});
				}

				ques_answers +='</div>';

			}
			else
			if(basic_type_id==2){

				ques_answers ='<input type="hidden" name="answer['+item_id+'][basic_type]" value="'+basic_type_id+'"/><input type="hidden" name="answer['+item_id+'][is_objective]" value="'+is_objective+'"/>';


				//if(answers_array!=undefined&&answers_array!=''){

							if(blank_size=='' || blank_size==null || blank_size==0)	ques_answers+='<span class="ques_answer"><input class="useranswer underline_blank underline_blank_20" size="20" type="text" name="answer['+item_id+'][useranswer]" /></span>';
							else if(blank_size==1)  ques_answers+='<span class="ques_answer"><input class="useranswer underline_blank underline_blank_12" size="12" type="text" name="answer['+item_id+'][useranswer]" /></span>';
							else if(blank_size==2)  ques_answers+='<div class="ques_answer"><input class="useranswer underline_blank underline_blank_100" size="100" type="text" name="answer['+item_id+'][useranswer]" /></div>';
							else if(blank_size==3)
							ques_answers+='<div class="ques_answer notprint"><textarea rows="10" cols="70"  class="ckeditor_basic"  name="answer['+item_id+'][useranswer]" type="text"> </textarea></div>';
							//ques_answers+='<div class="ques_answer"><textarea rows="10" cols="70" class="useranswer"  name="answer['+item_id+'][useranswer]" ></textarea></div>';
							else ques_answers +='<span class="ques_answer"><input class="useranswer underline_blank" type="text" name="answer['+item_id+'][useranswer]" /> </span>';

				//}



			}
		}


		if(basic_type_id == 1 || basic_type_id == 2)
			question +=ques_pre+ques_serial_ques_name+'<div class="ques_content">'+ques_body+ques_answers+'</div>';
		else
			question +=ques_pre+ques_serial_ques_name+ques_body;

			var subques_array=ques_array['subque'];


		if(subques_array!=-1&&subques_array!=undefined)
			question +=quesoutput(subques_array,item_id);
			//如果本题目有下一级的子题目，则递归显示子题目。

		question +=ques_suf;//子题目输出完后闭合本试题。



	});

	question +='</div>';


	return question;
	//本道试题全部输出完毕（包括其子题目的所有层级）后，返回本试题的所有html
}

