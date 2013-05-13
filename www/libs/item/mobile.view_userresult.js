var serial=serial_number;


(function($){
	$.fn.view_userresult=function(id,options){
		var settings=$.extend({
			titleContent:'.title',
			operation: 'detail',
			user_id : 0,
			backto:''
		},options||{});
		var that=this;
		$(that).addClass('user_result_content');


if(settings.operation=='result'){
	//显示用户整个试卷历史记录
	url = '/itest/egs/index.php?op=user_result_detail&id='+id;

	$.ajax({
		url: url,
		dataType: 'json',
		//data: data,
		timeout:20000,// 设置请求超时时间（毫秒）。
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。
			$(that).html(" 请求超时! textStatus: " + textStatus + ', errorThrown: ' + errorThrown);
		 },
		success: function(resp)
		{  //请求成功后回调函数。

			try{

			$(that).addClass('main_result');
			var output='';

		//-----------试卷基本信息------------------
			var paper_data=resp['paper_info'];
			var sid=0,examType='';
			$.each(paper_data,function(index,paper_array){
				examType = paper_array['exam_type'];
				sid = paper_array['sid'];
			});
			$(settings.titleContent).html(examType+" 成绩分析");

		//-----------用户试卷信息的输出------------------
			var user_results_array=resp['user_results_info'][id];

			var score=parseFloat(user_results_array['user_score']).toFixed(0);
			var objective_points=parseFloat(user_results_array['objective_points']).toFixed(0);;

			output +='<div class="clearfix"> <div class="user_score">正确率：<span class="user_total_score">'+score+'</span>/'+objective_points+'</div>'+
			'<a href="create.html?sid='+sid+'&backto='+settings.backto+'">再做一套</a>	</div>';
			$(that).html(output);

	//-----------试卷具体题目的输出--------------
			var subques_array=resp['item'];

			var output_items='<div class="paper">';
			if(subques_array!=-1)	output_items +=resultoutput(subques_array,{operation:settings.operation});
				output_items +='</div>';

			$(that).append(output_items);

			needfun();
			backToTop();

		}catch (err)
		{
			resp = undefined;
			$(that).append('服务器交互失败，请重试'+err);
		}

		}
	});

}
else if(settings.operation=='lishi'){
	//显示用户整个试卷历史记录
	url = '/itest/egs/index.php?op=user_result_detail&id='+id;

	$.ajax({
		url: url,
		dataType: 'json',
		//data: data,
		timeout:20000,// 设置请求超时时间（毫秒）。
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。
			$(that).html(" 请求超时! textStatus: " + textStatus + ', errorThrown: ' + errorThrown);
		 },
		success: function(resp)
		{  //请求成功后回调函数。

			try{
			var output='';

		//-----------试卷基本信息------------------
			var paper_data=resp['paper_info'];
			var paper_name='';
			$.each(paper_data,function(index,paper_array){
				paper_name = paper_array['paper_name'];
			});
			$(settings.titleContent).html(paper_name);

		//-----------用户试卷信息的输出------------------
			var user_results_array=resp['user_results_info'][id];

			var score=parseFloat(user_results_array['user_score']).toFixed(0);
			var objective_points=parseFloat(user_results_array['objective_points']).toFixed(0);;

			var user_score = '<div class="user_score">得分：'+score+'</span>/'+objective_points+'</div>';
			var user_time_used = '<div class="user_time_used">用时：'+  timeFormater(user_results_array['time_used']) +'</div>';

			output +='<div class="clearfix">' +user_score + user_time_used + '</div>';

			$(that).html(output);

	//-----------试卷具体题目的输出--------------
			var subques_array=resp['item'];

			var output_items='<div class="paper">';
			if(subques_array!=-1)	output_items +=resultoutput(subques_array,{operation:settings.operation});
				output_items +='</div>';

			$(that).append(output_items);

			needfun();
			backToTop();

		}catch (err)
		{
			resp = undefined;
			$(that).append('服务器交互失败，请重试'+err);
		}

		}
	});

}
else if(settings.operation=='DiagnosticReport'){
	//显示用户试卷诊断报告 DiagnosticReport
	url = '/itest/egs/index.php?op=user_diagnostic_report&id='+id;

	$.ajax({
		url: url,
		dataType: 'json',
		//data: data,
		timeout:20000,// 设置请求超时时间（毫秒）。
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。
			$(that).html(" 请求超时! textStatus: " + textStatus + ', errorThrown: ' + errorThrown);
		 },
		success: function(data)
		{  //请求成功后回调函数。

			try
			{
	//--总分诊断报告
		var output='<div class="main_diagnosis">';
		var paper_array=data['exam_info'];

		var user_score = '<div class="user_score">得分：'+parseInt(paper_array['user_score'])+'</div>';
		var user_time_used = '<div class="user_time_used">用时：'+  timeFormater(paper_array['time_used']) +'</div>';

		$(settings.titleContent).html(paper_array['exam_type']+" 诊断报告");

		user_level_resources = output_level_resources(paper_array);

		output +='<div class="clearfix">' +user_score + user_time_used + '</div>';

		output +='<div class="user_level_resources">'+user_level_resources+'</div>';


		var opjs = 'javascript:(function(){$(\'.user_result_content\').view_userresult('+id+',{operation:\'result\',titleContent:\'.paper_title\',backto:\''+settings.backto+'\'})})()';
		output +='<div class="user_result_operate">'+
					'<a href="javascript:void(0)"  data-gotourl="'+opjs+'" data-needlogin="1">成绩分析</a>'+
					'<a href="javascript:void(0)"  data-gotourl="userwrong.html" data-needlogin="1" >查看我的错题库</a></div>';
		output +='</div>';

		$(that).html(output);

				/* 需要登录才能访问的链接 */
				$('*[data-needlogin=1]').attr("data-gotourl",function(index, urlvalue){
					if(urlvalue.length>0){
						$(this).click(function(){
							if(user_id == 0 || user_id == null || user_id == undefined)
								confirmlogin(urlvalue);
							else
								location.href = urlvalue;

						});
					}
				});

		}

		catch (err)
			{
				data = undefined;
				$(that).append('服务器交互失败，请重试'+err);
			}

		}
	});	//----end $.ajax()----

}


};//---------end $.fn.view_userresult----
})(jQuery);








function timeFormater(secondtime){
	temtime=Math.abs(secondtime);
	hours = Math.floor(temtime/3600);
	minutes = Math.floor(temtime%3600/60);
	seconds = Math.floor(temtime%60);

	//if(maxtime<0) negetive='超时';
	if(hours<10) hours='0'+hours;
	if(minutes<10) minutes='0'+minutes;
	if(seconds<10) seconds='0'+seconds;

	var formatTime = hours+":"+minutes+":"+seconds;
	return formatTime;
}

var serial_total_view=0;//设置全局的题目编号

function resultoutput(subques_array,options){
	//用来递归显示所有试题的函数，加入了html标签，并且通过基本题型（basic_type）的判断，试题格式显示为不同的样式。
	var settings=$.extend({
			title_display : "no",
			operation:"detail"
		},options||{});
		//alert(settings.operation);
	var question='<div class="allSubques">';
	var layout=0;

	$.each(subques_array,function(index,items){

		//----获得基本的试题信息----
		layout++;
		var item_id=items['item_id'];


		var serial_name=items['serial_str'];

		var serial_num_id=items['serial_num'];//数据库中存储的serial_num样式的id号
		var serial_num = '';
		if(serial_num_id!=undefined)
			serial_num=serial[serial_num_id][layout];//根据以上id从serial_number.js的serial_number数组中获取对应的样式表，并根据本题所属层级（layout）获得具体的样式编号。


		var ques_array=items;

		var name=ques_array['name'];
		var points=parseFloat(ques_array['points']).toFixed(1);
		var time_limit=ques_array['time_limit'];
		var question_body=ques_array['body'];
		var description=ques_array['description'];

		var is_objective=ques_array['is_objective'];//填空题是否为客观填空题
		var choice_multi=ques_array['choice_multi'];//选择题是否为多选题
		//var choice_random=ques_array['choice_random'];//选择题答案顺序是否随机显示

		var points_awarded=parseFloat(ques_array['points_awarded']).toFixed(1);
			if(points_awarded==undefined)	points_awarded=0;
		var sum_score = parseFloat(ques_array['score']).toFixed(1);
		var ques_score = parseFloat(ques_array['score']).toFixed(1);
		//描述类与题干类的题目需要此score来汇总所有小题目的分数总和

		var user_answer_is_correct=ques_array['user_answer_is_correct'];
		var user_answer=ques_array['user_answer'];

		var answers_array=ques_array['answers'];

		var files=ques_array['files'];
		var file_status = files['file_status'];
		var fileurl = "";
			if(file_status == 1){
				fileurl = files['fileurl'];
			}

		var type=ques_array['type'];

		var type_name='';//Writing Listening Cloze ......
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
		var ques_pre='<div id="ques_display_'+item_id+'" class="ques_display ques_display_'+layout+'">';//默认显示所有试题
		if(settings.operation=='wrongQues') //如果显示错题集
		{
			if((user_answer_is_correct!=undefined && user_answer_is_correct!=0)||(is_objective!=undefined && is_objective==0))
			//主观题is_objective==0、没有做的题user_answer_is_correct==-1、做对的题user_answer_is_correct==1都不显示
				ques_pre='<div id="ques_display_'+item_id+'" class="ques_user_answer_not_wrong ques_display ques_display_'+layout+'">';
		}

		//<input type="hidden" name="answer['+item_id+']" value="'+item_id+'"/>
		var ques_suf='</div>';

		var ques_score='',ques_name='',ques_serial_name='',ques_serial_number='',ques_time_limit='',ques_points='',ques_body='',ques_description='',ques_answers='',ques_files='';

		var ques_point_serial_name_body='';


		ques_points=points;


		if(serial_name!=undefined&&serial_name!=''&&serial_name!=0) ques_serial_name='<span class="ques_serial_name"> '+serial_name+'</span>';
		if(serial_num!=undefined&&serial_num!='') ques_serial_number='<span class="ques_serial_number"> '+serial_num+'</span>';
		if(time_limit!=undefined&&time_limit!=''&&time_limit!=0)
			ques_time_limit='<span class="ques_time_limit">( '+time_limit+' min )</span>';
		if(points!=undefined&&points!='') ques_points='<span class="ques_score">（'+points+'分）</span>';
		if(question_body!=undefined&&question_body!='') ques_body='<span class="ques_body"> '+question_body+'</span>';

		if(description!=undefined&&description!='') ques_description='<div class="ques_description">'+description+'</div> ';

		if(file_status==1&&fileurl.length>0){
			ques_files='<div class="jplayer_wrap"><a id="jplayer-'+item_id+'" class="mp3 jplayer notprint" src='+fileurl+'></a></div> ';

			ques_pre='<div id="ques_display_'+item_id+'" class="ques_display hasmp3 ques_display_'+layout+'">';

		}


		if(basic_type_id==4&&question_body!=undefined&&question_body!=''){
			//题干类试题的题干中，可能会含有与子题目编号相对应的编号，通过以下方法把样式的标记转换成编号

			var txt=question_body;
			var regSign=/<\d>/;
			var regSerNum=/\d/;
			var i=serial_total_view+1;
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
				ques_body='<span class="ques_body"> '+question_body+'</span>';

			}

		}



		var ques_point_serial_name_body_answer = '';

		if(basic_type_id==3){
		//为描述类的题设定 本部分试题总分+特殊的编号+名称+body+description 的表现形式
			if(settings.title_display == 'yes' && type_name!=undefined&&type_name!='')	ques_name='<span class="ques_name" > '+type_name+'</span>';

			if(ques_body!=''&&ques_body!=undefined)
				ques_body = '<div class="ques_body_description"><span class="description_label">Directions</span>'+ques_body+'</div>';

			if(sum_score!=undefined)	ques_score='<span class="ques_score"> （分数/满分：'+sum_score+'/'+points+'）</span>';
			if(ques_serial_name!=''||ques_serial_number!=''||ques_name!=''){
				if(ques_files != '')
					ques_point_serial_name_body_answer ='<div class="clearfix">' + ques_files + '<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+ques_score+'</span></div></div>';
				else
					ques_point_serial_name_body_answer='<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+ques_score+'</span></div>';
			}
			ques_point_serial_name_body_answer = ques_point_serial_name_body_answer + ques_body;

		}

		else if(basic_type_id==4)
			//为题干类试题生成 分数+编号+名称+body+description的表现形式
		{
			//if(sum_score!=undefined)	ques_score='<span class="ques_score"> 共'+sum_score+'分</span>';
			if(ques_serial_name!=''||ques_serial_number!=''||ques_body!='' || ques_files!='')	ques_point_serial_name_body='<div class="serial_name_num_body">'+ques_files+ques_serial_name+ques_serial_number+ques_body+'</div>';

			//试题材料解析
			var answer_answer='',answer_feedback='';
			$.each(answers_array,function(index,answer){

				if(answer["answer"]!=undefined)
					answer_answer='<span class="answer_correct">'+answer["answer"]+'</span>';
				if(answer["feedback"]!=undefined && answer["feedback"]!=null && answer["feedback"]!='')
					answer_feedback='<div class="answer_analyse">   <label>[试题材料]</label> <span>'+answer["feedback"]+'</span>    </div>  ';
			});

			ques_answers ='<div class="ques_answer">'+answer_feedback+'</div>';

			ques_point_serial_name_body_answer = ques_point_serial_name_body  + ques_answers;
		}

		else{

			/* --------  答案 --------*/

			if(basic_type_id==1){
				//选择题的答案的输出形式

				var serial_number={"1":"A) ","2":"B) ","3":"C) ","4":"D) ","5":"E) ","6":"F) ","7":"G) ","8":"H) ","9":"I) ","10":"J) ","11":"K) ","12":"L) ","13":"M) ","14":"N) ","15":"O) ","16":"P) ","17":"Q) ","18":"R) ","19":"S) ","20":"T) ","21":"U) ","22":"V) ","23":"W) ","24":"X) ","25":"Y) ","26":"Z) "};
				var i=0;//为计算ABCD

				var ques_answers_analyse ='';
				ques_answers ='<div id="ques_answer">';

				$.each(answers_array,function(index,answer){
					i++;
					var answer_class='';
					var options_img='';
					if(answer['is_correct']==1) {
						if(user_answer_is_correct==1){
							answer_class='right';
						}
						else{
							answer_class='should';
							options_img='<span class="options_img"></span>';
						}
						options_img='<span class="options_img"><img src="../images/correct.png"/></span>';
					}
					else if(user_answer_is_correct==0&&user_answer==index){
							answer_class='myanswer';
							options_img='<span class="options_img"><img src="../images/wrong.png"/></span>';
					}

					ques_answers +='<div class="options '+answer_class+'"><span>'+serial_number[i]+answer["answer"]+'</span>'+options_img+'</div> ';
					if(answer["feedback"].length>0) {
						ques_answers_analyse += '<span>'+answer["feedback"]+'</span>';

						if(ques_body.length>0)
							ques_answers_analyse =  '<div class="answer_analyse">   <label>答案解析：</label>'+ques_answers_analyse+' </div>';
						else
							ques_answers_analyse =  '<span class="answer_analyse">   <label>答案解析：</label>'+ques_answers_analyse+' </span>';

					}

				});
				ques_answers +='</div>';

				if(ques_answers_analyse.length>0)	ques_answers = ques_answers_analyse+ques_answers;


			}
			else 	if(basic_type_id==2){
				//填空题的答案的输出形式

				if(user_answer==undefined)	user_answer='&nbsp;&nbsp;';
				var answer_answer='',answer_feedback='';
				$.each(answers_array,function(index,answer){

					if(answer["answer"]!=undefined)
						answer_answer='<span class="answer_correct">'+answer["answer"]+'</span>';
					if(answer["feedback"]!=undefined && answer["feedback"]!=null && answer["feedback"]!='')
						answer_feedback='<div class="answer_analyse">   <label>答案解析:</label> <span>'+answer["feedback"]+'</span>    </div>  ';
					});
					if(is_objective==1) {
						if(user_answer_is_correct==1)
							ques_answers ='<div class="ques_answer correct"> <div class="answer_correct"><label>正确答案:</label><span class="answer_correct answer_user">'+user_answer+'</span></div> '+answer_feedback+'</div>';
						else
							ques_answers ='<div class="ques_answer"> <div class="answer_user"><label>用户答案:</label><span class="answer_user">'+user_answer+'</span></div> <div class="answer_correct"> <label>正确答案:</label>'+answer_answer+'</div>'+answer_feedback+'</div>';

					}
					else if(is_objective==0){
							ques_answers ='<div class="ques_answer is_subjective">    	<div class="answer_user"> <label>用户答案:</label><span class="answer_user">'+user_answer+'</span></div>	 <div class="answer_correct"> <label>参考答案:</label>'+answer_answer+'</div> '+answer_feedback+'</div>';


					}

			}
			/* -------- end 答案 --------*/

			//为选择题与填空题生成对错+编号+名称+分数+body+description+answer的表现形式


			//选择题与填空题在试题前面加"对勾"与"叉号"与分数
			if(basic_type_id==1||basic_type_id==2){

				var answer_img='';
				if(is_objective==0) {

					answer_img='<span class="answer_img is_subjective is_correct"></span>';

				}
				else{

					if(user_answer_is_correct==1) answer_img='<span class="answer_img is_objective is_correct"></span>';
					else answer_img='<span class="answer_img is_objective is_wrong"></span>';

				}

				ques_serial_number=++serial_total_view+'.&nbsp;';

				ques_point_serial_name_body_answer='<div class="ques_point_serial_name_body">'+ques_files+'<span class="serial_total">'+ques_serial_name+ques_serial_number+'</span><div class="ques_content">'+answer_img+ques_body+ques_answers+'</div></div>';
			//ques_score提供值后，删除points_awarded
			}



			//ques_point_serial_name_body_answer = ques_point_serial_name_body  + ques_answers;
		}



		question +=ques_pre+ques_point_serial_name_body_answer;



		var subques_array=ques_array['subque'];


		if(subques_array!=-1&&subques_array!=undefined)	question +=resultoutput(subques_array,{title_display:'no',operation:settings.operation});
		//如果本题目有下一级的子题目，则递归显示子题目。
		question +=ques_suf;//子题目输出完后闭合本试题。

	});
	question +='</div>';
	return question;//本道试题全部输出完毕（包括其子题目的所有层级）后，返回本试题的所有html
}


function output_level_resources(user_results_array){
	var user_level='';

	var level = user_results_array['level'];

	if(level!=undefined && level!='' && level!=null){
		var level_name = level['level_name'];
		var level_description = level['description'];


		if(level_name != undefined && level_name !='' && level_description != undefined && level_description != ''){//评语
			user_level ='<div class="user_level"><div class="level_name_bg"><span class="level_name">'+level_name+':</span></div><span class="level_description">'+level_description+'</span></div>';
		}
	}


	return user_level;
}



