var serial=serial_number;
var attributes_chart = new Array(new Array(),new Array());
//var tempdata=new Array();


(function($){
	$.fn.view_userresult=function(id,options){
		var settings=$.extend({
			titleContent:'.title',
			operation: 'detail',
			rebackUrl:'manage_exam.html'
		},options||{});
		var that=this;
		$(that).addClass('user_result_content');
		$(that).html('<div class="loadging"><img width="25px" height="25px" src="images/loading.gif"></div>');
		paper_maincontent=that;

	serial_total=0;clearInterval(timer);

if(settings.operation=='detail'||settings.operation=='wrongQues'){
	//显示用户整个试卷历史记录
	var sid = 0;
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

			try
			{
		var output='';

	//-----------试卷基本信息的输出------------------
		var paper_data=resp['paper_info'];
		var points = '',time_limit = '',description = '';

		$.each(paper_data,function(index,paper_array){
			var paper_name = paper_array['paper_name'];
			var examType = paper_array['exam_type'];

			description = paper_array['description'];
			points = parseInt(paper_array['points']);
			time_limit = paper_array['time_limit'];
			sid = paper_array['sid'];

			if(examType.indexOf("真题")>=0)
				$(settings.titleContent).html('<span id="title_f">'+paper_name+'</span><a class="btn_05" href="http://answer.2u4u.com.cn/node/add/best-answer" target="_blank">我要提问</a>');
			else
				$(settings.titleContent).html('<span id="title_f">'+paper_name+'</span><a class="btn_05" href="javascript:void(0);" onclick=\'serial_total=0;clearInterval(timer);$(".paper_content").create_paper('+sid+',{titleContent:"'+settings.titleContent+'",user_id:"'+settings.user_id+'"});return false;\'>再做一套试卷</a><a class="btn_05" href="http://answer.2u4u.com.cn/node/add/best-answer" target="_blank">我要提问</a>');

		});


	//-----------用户试卷信息的输出------------------
		var user_results_array=resp['user_results_info'][id];

		var uid=user_results_array['uid'];
		var time_end=user_results_array['time_end'];
		var time_used=user_results_array['time_used'];
			var formattime = timeFormater(time_used);
		var score=parseFloat(user_results_array['user_score']).toFixed(1);
		var objective_points=parseFloat(user_results_array['objective_points']).toFixed(1);;


		output = '<div class="paper_info clearfix">		<div class="DiagnosticReport">	<a href="javascript:void(0)" onclick=\'$(".paper_content").view_userresult('+id+',{operation:"DiagnosticReport",titleContent:".paper_title",rebackUrl:"user_exam.html"});return false;\'>诊断报告</a>	</div>		<div class="basic_info"><table><tr class="paper"><th>基本信息</th><td class="total_score">总分：'+ points +'(分)</td><td class="total_time">时长：'+ time_limit +'(分钟)</td><td class="created" rowspan="2">'+ time_end +'</td></tr><tr class="user"><th>用户('+ uid +')</th><td class="score">得分：<span class="user_total_score">'+ score +'</span>/'+ objective_points +'(客观题)</td><td class="used_time">用时：'+ formattime +'</td></tr></table>	</div>		</div>';


		$(that).html(output);




	//-----------试卷具体题目的输出--------------
		var subques_array=resp['item'];

		var output_items='<div class="paper">';
		if(subques_array!=-1)	output_items +=resultoutput(subques_array,{operation:settings.operation});
			output_items +='</div>';

		$(that).append(output_items);
		//----------外加一个再做一套按钮--------------
		$(that).append('<div id="fonts2"><a class="btn_05" href="javascript:void(0);" onclick=\'$(".paper_content").create_paper('+sid+',{titleContent:"'+settings.titleContent+'",user_id:"'+settings.user_id+'"});serial_total=0;clearInterval(timer);return false;\'>再做一套试卷</a><a class="btn_05" href="http://answer.2u4u.com.cn/node/add/best-answer" target="_blank">我要提问</a></div>');



			$(".serial_name_num_name").toggle(
				function(){$(this).nextAll('.allSubques').slideUp('slow');},
				function(){$(this).nextAll('.allSubques').slideDown('slow');}
			);

			$(".jplayer").myjplayer();


			$('<div style="text-align:right" class="sizecontrol notprint">字体：[<span style="cursor:pointer" class="bigsize"> 大 </span><span style="cursor:pointer" class="middlesize"> 中 </span><span style="cursor:pointer" class="smallsize on"> 小 </span>]</div>').appendTo(".paper_title").hide().slideUp('slow');
		$(".sizecontrol").click(function(event){
			if($(event.target).is('.bigsize'))
			{
				$('.content').css({'font-size':'1.7em','line-height':'2.4em'});
				$(".sizecontrol span").removeClass('on');
				$('.bigsize').addClass('on');
			}
			if($(event.target).is('.middlesize')){
				$('.content').css({'font-size':'1.3em','line-height':'2.3em'});
				$(".sizecontrol span").removeClass('on');
				$('.middlesize').addClass('on');
			}
			if($(event.target).is('.smallsize')){
				$('.content').css({'font-size':'1em','line-height':'2em'});
				$(".sizecontrol span").removeClass('on');
				$('.smallsize').addClass('on');
			}
	    });



	}
			catch (err)
			{
				resp = undefined;
				$(that).append('服务器交互失败，请重试'+err);
			}

		}
	});





}

else if(settings.operation=='summary')
{//显示用户试卷历史summary分数

	url = '/itest/egs/index.php?op=user_result_summary_info&id='+id;

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

	//--总分summary
		var output='';
		var paper_array=data['exam_info'];
		var paper_name = '<div class="paper_name fleft">'+ paper_array['paper_name'] +'</div>';
		var user_time_used = '<div class="user_time_used">用时：'+  timeFormater(paper_array['time_used']) +'</div>';

		level_resources = output_level_resources(paper_array);

		user_level_resources ='<div class="user_level_resources">'+level_resources+'</div>';
		//评语以及推荐资源


		//var user_objectscore = '<div class="user_objectscore">您的客观题总分是<span class="user_score">'+ parseFloat(paper_array['user_score']).toFixed(1)+'</span><span class="sepline">/</span><span class="total_score">'+parseFloat(paper_array['objective_points']).toFixed(1)+'</span>(试卷客观题总分)，详见下表：</div>';
		var user_objectscore = '<div class="user_objectscore">客观题：得分<span class="user_score">'+ parseFloat(paper_array['user_score']).toFixed(1)+'</span><span class="sepline"> / </span><span class="total_score"> '+parseFloat(paper_array['objective_points']).toFixed(1)+'(总分)</span></div>';

		//output +='<div class="clearfix">' + paper_name + user_time_used + '</div>'+ user_objectscore;
		output += user_objectscore;


	//--试卷每一部分summary
		var subques_array=data['result_info'];
		var layout=0;
		if(subques_array!=undefined)	output +='<div class="summary_total clearfix">'+summary_subque_output(layout,subques_array)+'</div>';

		output += user_level_resources;


		open_basetype(id, output, settings.rebackUrl);

		}

		catch (err)
			{
				data = undefined;
				$(that).append('服务器交互失败，请重试'+err);
			}

		}
	});

}


else if(settings.operation=='DiagnosticReport')
{//显示用户试卷诊断报告 DiagnosticReport
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
		var output='';
		var paper_array=data['exam_info'];
		var paper_name = '<div class="paper_name fleft">'+ paper_array['paper_name'] +'</div>';
		var user_time_used = '<div class="user_time_used">用时：'+  timeFormater(paper_array['time_used']) +'</div>';


		$(settings.titleContent).html('<span id="title_f">'+paper_name+'</span><a class="btn_05" href="http://answer.2u4u.com.cn/node/add/best-answer" target="_blank">我要提问</a>');

		//user_level_resources = '';

		//user_level_resources = output_level_resources(paper_array);
		user_level = output_user_level(paper_array['level']);
		user_resources = output_user_resources(paper_array['resources']);


		var user_objectscore = '<div class="user_objectscore">客观题：得分<span class="user_score">'+ parseFloat(paper_array['user_score']).toFixed(1)+'</span><span class="sepline"> / </span><span class="total_score"> '+parseFloat(paper_array['objective_points']).toFixed(1)+'(总分)</span></div>';
	//--试卷每一部分summary
		var subques_array=data['result_info'];
		var layout=0;
		if(subques_array!=undefined)	user_objectscore +='<div class="summary_total  clearfix">'+summary_subque_output(layout,subques_array)+'</div>';
		attributes_chart_content = '';
//		attributes_chart_content = '<div id="attributes_chart_content">  <canvas id="attributes_chart" width="500" height="220"></canvas></div>';
//		user_attribute_level_resources = output_attribute_level_resources(data['exam_attribute']);


		//output +='<div class="clearfix">' + paper_name + user_time_used + '</div>' + user_level + user_objectscore + attributes_chart_content + user_resources;
		output += user_level + user_objectscore + attributes_chart_content + user_resources;

//		output +='<div class="user_attribute_level_resources"><b class="rtop"><b class="r1"></b><b class="r2"></b><b class="r3"></b><b class="r4"></b></b> '+user_attribute_level_resources+'<b class="rbottom"><b class="r4"></b><b class="r3"></b><b class="r2"></b><b class="r1"></b></b></div>';

		output +='<div class="user_result_operate clearfix"><a class="btn_05" href="javascript:void(0)" onclick=\'serial_total=0;clearInterval(timer);$(".paper_content").view_userresult('+id+',{operation:"detail",titleContent:"'+settings.titleContent+'"});return false;\'>去看详细记录</a> <a class="btn_05" href="javascript:void(0)" onclick=\'serial_total=0;clearInterval(timer);$("#mainContent").load("'+settings.rebackUrl+'");return false;\'>记录列表</a></div>';

		$(that).html(output);


/*
		var chartSetting1={
			 config:{
				title    : '<i>正确率(%)</i><div class="attribute_resources_label"> 各个考点评价</div>',
				type     : 'bar',
				labelX : attributes_chart['labelX'],
				scaleY : {min: 0,max:100,gap:10},
				bgGradient :{
					from      : '#ffffff',
					to        : '#e3e3e3'
				},
				colorSet : ['#f5e80f'],
				strokeStyle : '#929292'
			},
			data :  attributes_chart['data']
		};
		$('#attributes_chart').jQchart(chartSetting1);

*/		/*
		$(".resources_content_bytags").scrollable({
			size: 5,
			vertical:false,
			clickable:false,
			//navi:'#navi1',
			items:'.resources_content_bytags_items',
			prevPage:'#prev1',//跳转到上一页
			nextPage:'#next1',//跳转到下一页
			hoverClass: 'hover',
			easing:'linear'
		});
		*/
		//alert($(".exam_resources").html());

		}

		catch (err)
			{
				data = undefined;
				$(that).append('服务器交互失败，请重试'+err);
			}

		}
	});	//----end $.ajax()----

}

}//---------end $.fn.view_userresult----
})(jQuery);




function summary_subque_output(layout,subques_array){
	var output='';
	var number=0;
		layout++;
	var data=subques_array;
	$.each(data,function(index,items){
		number++;

		var score = parseFloat(items['score']).toFixed(1);
		var serial_str = items['serial_str'];
		var serial_num_id=items['serial_num'];//数据库中存储的serial_num样式的id号
		if(serial_num_id!=undefined)
			var serial_num=serial[serial_num_id][number];//根据以上id从serial_number.js的serial_number数组中获取对应的样式表，并根据本题所属层级（number）获得具体的样式编号。

		output +='<div class="summary_layout_'+layout+'"><div class="summary_score">'+ serial_str+'&nbsp;'+serial_num+'&nbsp;('+score+')&nbsp;</div>';

		var subques_array=items['subque'];
		if(subques_array!=undefined)
			output +='<div class="summary">'+summary_subque_output(layout,subques_array)+'</div>';

		output +='</div>';
	});

	return output;
}



	function open_basetype(id, output, rebackUrl){

		var title='<div class="title">您的做题结果</div>';
		$.prompt(title+output,
			{buttons: { 去看详细记录: 1, 查看诊断报告: -1, 返回记录列表: 0},
				focus: 0,
				submit: function(v, m, f){
					if(v==1){
						paper_maincontent.view_userresult(id,{operation:"detail",titleContent:".paper_title"});
					}
					else if(v==-1){
						paper_maincontent.view_userresult(id,{operation:"DiagnosticReport",titleContent:".paper_title"});
					}
					else{
						//区分普通用户和管理员
						$('#mainContent').load(rebackUrl);
					}
					$.prompt.close();
					return false;
				}
		 });
	}


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
			title_display : "yes",
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
		//var choice_random=ques_array['choice_random'];//选择题答案顺序是否随机显示

		var points_awarded=parseFloat(ques_array['points_awarded']).toFixed(1);
			if(points_awarded==undefined)	points_awarded=0;
		var sum_score = parseFloat(ques_array['score']).toFixed(1);
		var ques_score = parseFloat(ques_array['score']).toFixed(1);
		//描述类与题干类的题目需要此score来汇总所有小题目的分数总和

		var user_answer_is_correct=ques_array['user_answer_is_correct'];
		var user_answer=ques_array['user_answer'];

		var answers_array=ques_array['answers'];

		var files=ques_array['files'];		//ps whx files
			file_status = files['file_status'];
			if(file_status == 1){
				file_type = files['file_type'];		fileurl = files['fileurl'];
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

		if(files!=undefined&&files!=''&&file_status==1)
			ques_files='<div class="jplayer_wrap"><a id="jplayer-'+item_id+'" class="mp3 jplayer notprint" src='+fileurl+'></a></div> ';


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





		if(basic_type_id==3){
		//为描述类的题设定 本部分试题总分+特殊的编号+名称+body+description 的表现形式
			if(settings.title_display == 'yes' && type_name!=undefined&&type_name!='')	ques_name='<span class="ques_name" > '+type_name+'</span>';


			if(sum_score!=undefined)	ques_score='<span class="ques_score"> （分数/满分：'+sum_score+'/'+points+'）</span>';
			if(ques_serial_name!=''||ques_serial_number!=''||ques_name!=''||ques_body!=''){
				if(ques_files != '')
					ques_point_serial_name_body_answer ='<div class="clearfix">' + ques_files + '<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+ques_score+'</span></div></div><div class="ques_body_description">'+ques_body+'</div>';
				else
					ques_point_serial_name_body_answer='<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+ques_score+'</span></div><div class="ques_body_description">'+ques_body+'</div>';
			}


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
					answer_feedback='<div class="answer_analyse">   <label>【试题材料】</label> <span class="answer_analyse">'+answer["feedback"]+'</span>    </div>  ';
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

				ques_answers +='<table id="ques_answer">  <thead><th class="answer_option">选项</th><th class="answer_analyse">答案解析</th></thead>  <tbody>';
				$.each(answers_array,function(index,answer){
					i++;
					var answer_class='';
					if(answer['is_correct']==1) {
						if(user_answer_is_correct==1)
							answer_class='right';
						else
							answer_class='should';
					}
					else
						if(user_answer_is_correct==0&&user_answer==index)
						//if(user_answer==index)
							answer_class='myanswer';
					ques_answers +='<tr><td class='+answer_class+'>'+serial_number[i]+answer["answer"]+'</td>    <td>'+answer["feedback"]+'</td>  </tr> ';
				});
				ques_answers +='</tbody></table>';

			}
			else 	if(basic_type_id==2){
				//填空题的答案的输出形式

				if(user_answer==undefined)	user_answer='&nbsp;&nbsp;';
				var answer_answer='',answer_feedback='';
				$.each(answers_array,function(index,answer){

					if(answer["answer"]!=undefined)
						answer_answer='<span class="answer_correct">'+answer["answer"]+'</span>';
					if(answer["feedback"]!=undefined && answer["feedback"]!=null && answer["feedback"]!='')
						answer_feedback='<div class="answer_analyse">   <label>【答案解析】</label> <span class="answer_analyse">'+answer["feedback"]+'</span>    </div>  ';
					});
					if(is_objective==1) {
						if(user_answer_is_correct==1)
							ques_answers ='<div class="ques_answer correct"> <div class="answer_correct"><label>【正确答案】</label><span class="answer_correct answer_user">'+user_answer+'</span></div> '+answer_feedback+'</div>';
						else
							ques_answers ='<div class="ques_answer"> <div class="answer_user"><label>【用户答案】</label><span class="answer_user">'+user_answer+'</span></div> <div class="answer_correct"> <label>【正确答案】</label>'+answer_answer+'</div>'+answer_feedback+'</div>';

					}
					else if(is_objective==0){
							ques_answers ='<div class="ques_answer is_subjective">    	<div class="answer_user"> <label>【用户答案】</label><span class="answer_user">'+user_answer+'</span></div>	 <div class="answer_correct"> <label>【参考答案】</label>'+answer_answer+'</div> '+answer_feedback+'</div>';


					}

			}
			/* -------- end 答案 --------*/

			//为选择题与填空题生成对错+编号+名称+分数+body+description+answer的表现形式


			//选择题与填空题在试题前面加"对勾"与"叉号"与分数
			if(basic_type_id==1||basic_type_id==2){

				var answer_img='';
				if(is_objective==0) {
					answer_img='<div class="answer_img is_subjective is_correct"></div>';
				}
				else{

					if(user_answer_is_correct==1) answer_img='<div class="answer_img is_objective is_correct"></div>';
					else answer_img='<div class="answer_img is_objective is_wrong"></div>';

				}

				ques_serial_number=++serial_total_view+'.&nbsp;';

				ques_point_serial_name_body_answer='<div class="ques_point_serial_name_body">'+ques_files+'<span class="serial_total">'+ques_serial_name+ques_serial_number+'</span><div class="ques_content">'+answer_img+ques_body+ques_points+ques_score+ques_answers+'</div></div>';
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

function get_resources(nodes,max_count){
	var resources_array = new Array(), i = 0,resources='';


	$.each(nodes,function(index,node){
		i++;
		if(i<=max_count){
			node_link = node['node']['view_node'];

			resources += '<div class="resources-row">';
			resources += '<div class="fengmian"><a href="'+node_link+'" target="_blank"><img src='+node['node']['field_yuedu_cover_fid']+'/></a></div>';
			resources += '<div class="resources-title">'+node['node']['title']+'</div>';
			resources += '</div>';
		}
	});

	resources_array['count'] = i;
	resources_array['resources'] = resources;

	return resources_array;
}

function output_user_level(level){
	var user_level='';

	//var level = user_results_array['level'];

	if(level!=undefined && level!='' && level!=null){
		var level_name = level['level_name'];
		var level_description = level['description'];

		var level_id = level['level_id'];
		var user_smile_face = '<div class="user_smile_face user_smile_face_'+level_id+'">'+ level_name +'</div>';

		if(level_name != undefined && level_name !='' && level_description != undefined && level_description != ''){//评语
			user_level ='<div class="exam_level user_level">'+ user_smile_face +'<div class="level_evaluate"><span class="level_name">'+level_name+':</span><span class="level_description">'+level_description+'</span></div></div>';
		}
	}


	return user_level;
}

function output_user_resources(resources){
	var user_resources='';

	if(resources!=undefined && resources!='' && resources!=null){

		//var resources = '';
		var resources_tags1 = resources['resources_tags1'];
		var or_resources_tags2 = resources['or_resources_tags2'];
		var resources_class = '';

		var resources_1="",resources_2="";
		if((resources_tags1 != '' && resources_tags1 != undefined) || (or_resources_tags2 != '' && or_resources_tags2 != undefined)){

			var resources_sum = 10; //显示资源个数

			resources_array1 = get_resources(resources_tags1['nodes'],resources_sum);

			resources_count_leave = resources_sum - resources_array1['count'];
			resources_1 = resources_array1['resources'];

			if(resources_count_leave > 0){
				if(or_resources_tags2 != '' && or_resources_tags2 != undefined){
					resources_array2 = get_resources(or_resources_tags2['nodes'],resources_count_leave);
					resources_2 = resources_array2['resources'];
					resources_count_leave -= resources_array2['count'];
				}
			}
			if(resources_count_leave>=5)
				resources_class="no_hidden_resources";
		}

		if(resources_1 !='' || resources_2 !=''){
			//user_resources ='<div class="exam_resources resources"><div class="resources_label">你还可以更优秀哦：</div>	<a id="prev1"></a><!--向前滚动的按钮--> <div class="resources_content_bytags"><div class="resources_content_bytags_items">'+resources_1+resources_2+'</div></div>	<a id="next1"></a><!--向后滚动的按钮-->	</div>';
			user_resources ='<div class="exam_resources resources '+resources_class+'"><div class="resources_label">你还可以更优秀哦：</div>	<div class="resources_content_bytags"><div class="resources_content_bytags_items">'+resources_1+resources_2+'</div></div> <div class="resouces_click"><div id="exam_click_button" class="click_button click_button_show" onclick="javascript:exam_resources_click()">点击展开</div></div> </div>';
		}
		else if(resources['resources']!=undefined&&resources['resources']!=''){//推荐资源
			user_resources ='<div class="resources"><div class="resources_label">你还可以更优秀哦：</div><div class="resources_content">'+resources['resources']+'</div></div>';
		}
	}

	return user_resources;
}

function output_level_resources(user_results_array){
	var user_level='',user_resources='';

	var level = user_results_array['level'];

	if(level!=undefined && level!='' && level!=null){
		var level_name = level['level_name'];
		var level_description = level['description'];

		var level_id = level['level_id'];
		var user_smile_face = '<div class="user_smile_face user_smile_face_'+level_id+'">'+ level_name +'</div>';


		if(level_name != undefined && level_name !='' && level_description != undefined && level_description != ''){//评语
			user_level ='<div class="user_level">'+ user_smile_face +'<div class="level_evaluate"><span class="level_name">'+level_name+':</span><span class="level_description">'+level_description+'</span></div></div>';
		}
	}

	var resources = user_results_array['resources'];



	if(resources!=undefined && resources!='' && resources!=null){

		//var resources = '';
		var resources_tags1 = resources['resources_tags1'];
		var or_resources_tags2 = resources['or_resources_tags2'];


		var resources_1="",resources_2="";
		if((resources_tags1 != '' && resources_tags1 != undefined) || (or_resources_tags2 != '' && or_resources_tags2 != undefined)){

			var resources_sum = 10; //显示资源个数

			resources_array1 = get_resources(resources_tags1['nodes'],resources_sum);

			resources_count_leave = resources_sum - resources_array1['count'];
			resources_1 = resources_array1['resources'];

			if(resources_count_leave > 0){
				if(or_resources_tags2 != '' && or_resources_tags2 != undefined){
					resources_array2 = get_resources(or_resources_tags2['nodes'],resources_count_leave);
					resources_2 = resources_array2['resources'];
				}
			}

		}

		if(resources_1 !='' || resources_2 !=''){
			user_resources ='<div class="resources"><div class="resources_label">你还可以更优秀哦：</div><div class="resources_content_bytags">'+resources_1+resources_2+'</div></div>';
		}
		else if(resources['resources']!=undefined&&resources['resources']!=''){//推荐资源
			user_resources ='<div class="resources"><div class="resources_label">你还可以更优秀哦：</div><div class="resources_content">'+resources['resources']+'</div></div>';
		}
	}

	level_resources = user_level + user_resources;

	return level_resources;
}



function output_attribute_level_resources(user_attributes){

	var attr_level_resources = '';

	//var attr_level_resources = '<div class="attribute_resources_label">各个考点评价</div>';
	//attr_level_resources += '<div id="attributes_chart_content"><canvas id="attributes_chart" width="500" height="220"></canvas></div>';

	attributes_chart['labelX'] = new Array();
	attributes_chart['data'] = new Array(new Array());

	attr_level_resources += '<div class="attribute_resources_content_bytags">';
	$.each(user_attributes,function(index,user_attribute){

		var aid = user_attribute['aid'];
		var attr_name = user_attribute['attr_name'];

		var user_score = parseFloat(user_attribute['user_score']).toFixed(1);
		var total_score = parseFloat(user_attribute['total_score']).toFixed(1);
		var rate= user_attribute['rate'];

		var level = user_attribute['level'];
		var resources = user_attribute['resources'];


		var attr_name_title='', attr_score='', user_level='', top_user_resources = '', user_resources='';


		if(attr_name != undefined && attr_name !='' ){//考点
			attr_name_title = '<div class="attr_name attr_'+aid+'">' + attr_name + '</div>';
		}

		var user_smile_face = '', user_attr_rate = '', user_attr_score = '';

		if(user_score != undefined && total_score != undefined && rate != undefined){
			attributes_chart['labelX'][index] =  attr_name;
			attributes_chart['data'][0][index] = parseFloat(rate*100).toFixed(1);

			if(level!=undefined && level!='' && level!=null){

				var level_id = level['level_id'];
				var level_name = level['level_name'];
				user_smile_face = '<div class="user_smile_face attr_smile_face attr_smile_face_'+level_id+'">'+ level_name +'</div>';
			}
			user_attr_rate = '<div class="user_attr_rate"><span class="user_rate_label">正确率：</span><span class="user_rate">'+parseFloat(rate*100).toFixed(1)+'%</span></div>';
			user_attr_score = '<div class="user_attr_score">（ 得分:' + user_score + ' / 总分:' + total_score + ' ）</div>';
		}

		attr_score ='<div class="attr_score">'+ user_attr_rate + user_attr_score + '</div>';


		if(level!=undefined && level!='' && level!=null){

			var level_id = level['level_id'];
			var level_name = level['level_name'];
			var level_description = level['description'];

			if(level_name != undefined && level_name !='' ){//评语
				var level_description_output = '';
				if(level_description != undefined && level_description != '')
					level_description_output = '<span class="level_description">' + level_description + '</span>';

				user_level ='<div class="user_level"><span class="level_name level_'+level_id+'">'+level_name+':</span>'+level_description_output+'</div>';
			}
		}


		if(resources!=undefined && resources!='' && resources!=null){
			var resources1 = resources['resources1'];
			var resources2 = resources['resources2'];
			var resources3 = resources['resources3'];
			var resources4 = resources['resources4'];


			var top_resources_1="",top_resources_2="",top_resources_3="",top_resources_4="";
			if((resources1 != '' && resources1 != undefined) || (resources2 != '' && resources2 != undefined)||(resources3 != '' && resources3 != undefined) || (resources4 != '' && resources4 != undefined)){

				var top_resources_sum = 2; //显示资源个数
				var top_resources_count_leave = 2, top_resources_count_leave1 = 2, top_resources_count_leave2 = 2;
				if(resources1 != '' && resources1 != undefined){
					top_resources_array1 = get_resources(resources1['nodes'],top_resources_sum);
					top_resources_count_leave = top_resources_sum - top_resources_array1['count'];
					top_resources_1 = top_resources_array1['resources'];
				}

				if(top_resources_count_leave > 0 && resources2 != '' && resources2 != undefined ){
					top_resources_array2 = get_resources(resources2['nodes'],top_resources_count_leave);
					top_resources_count_leave1 = top_resources_count_leave - top_resources_array2['count'];
					top_resources_2 = top_resources_array2['resources'];
				}

				if(top_resources_count_leave1 > 0 && resources3 != '' && resources3 != undefined){
					top_resources_array3 = get_resources(resources3['nodes'],top_resources_count_leave1);
					top_resources_count_leave2 = top_resources_count_leave1 - top_resources_array3['count'];
					top_resources_3 = top_resources_array3['resources'];
				}

				if(top_resources_count_leave2 > 0 && resources4 != '' && resources4 != undefined){
					top_resources_array4 = get_resources(resources4['nodes'],top_resources_count_leave2);
					top_resources_4 = top_resources_array4['resources'];
				}

			}
			if(top_resources_1 !='' || top_resources_2 !='' || top_resources_3 !='' || top_resources_4 !=''){
				top_user_resources ='<div class="top_resources resources"><div class="resources_label">看看他们，会有帮助哦：</div><div class="resources_content_bytags resources_content_bytags_top">'+top_resources_1+top_resources_2+top_resources_3+top_resources_4+'</div></div>';
			}
			else if(resources['resources']!=undefined&&resources['resources']!=''){//推荐资源
				top_user_resources ='<div class="resources"><div class="resources_label">看看他们，会有帮助哦：</div><div class="resources_content">'+resources['resources']+'</div></div>';
			}


			var resources_1="",resources_2="",resources_3="",resources_4="";
			if((resources1 != '' && resources1 != undefined) || (resources2 != '' && resources2 != undefined)||(resources3 != '' && resources3 != undefined) || (resources4 != '' && resources4 != undefined)){

				var resources_sum = 10, total_sum = 0; //显示资源个数
				var resources_count_leave = 10, resources_count_leave1 = 10, resources_count_leave2 = 10;
				if(resources1 != '' && resources1 != undefined){
					resources_array1 = get_resources(resources1['nodes'],resources_sum);
					resources_count_leave = resources_sum - resources_array1['count'];
					resources_1 = resources_array1['resources'];

					total_sum = resources_array1['count'];
				}

				if(resources_count_leave > 0 && resources2 != '' && resources2 != undefined ){
					resources_array2 = get_resources(resources2['nodes'],resources_count_leave);
					resources_count_leave1 = resources_count_leave - resources_array2['count'];
					resources_2 = resources_array2['resources'];

					total_sum += resources_array2['count'];
				}

				if(resources_count_leave1 > 0 && resources3 != '' && resources3 != undefined){
					resources_array3 = get_resources(resources3['nodes'],resources_count_leave1);
					resources_count_leave2 = resources_count_leave1 - resources_array3['count'];
					resources_3 = resources_array3['resources'];

					total_sum += resources_array3['count'];
				}

				if(resources_count_leave2 > 0 && resources4 != '' && resources4 != undefined){
					resources_array4 = get_resources(resources4['nodes'],resources_count_leave2);
					resources_4 = resources_array4['resources'];

					total_sum += resources_array4['count'];
				}

			}

			if((resources_1 !='' || resources_2 !='' || resources_3 !='' || resources_4 !='') && total_sum > 2){
				user_resources ='<div class="bottom_resources resources"><div class="resources_content_bytags resources_content_bytags_bottom">'+resources_1+resources_2+resources_3+resources_4+'</div></div>';
			}

		}
		var resources_class='has_hidden_resources';
		if(top_user_resources == '')
			resources_class='no_resources';
		else if(user_resources == '')
			resources_class='no_hidden_resources';
		else
			resources_class='has_hidden_resources';

		attr_level_resources +='<div class="attr_row '+resources_class+'">' + attr_name_title + '<div class="attr_visible clearfix"> <div class="user_face_score_level fleft"><div class="user_face_score clearfix">' + user_smile_face + attr_score + '</div>' + user_level + '</div> <div class="top_resources_wrap fright">' + top_user_resources + '</div></div><div class="attr_hidden ">'+ user_resources +'</div>		<div class="resouces_click"><div id="click_button_'+aid+'" class="click_button click_button_show" onclick="javascript:resouces_click('+aid+')">点击展开</div></div>	</div>';


	});

	attr_level_resources +='</div>';

	return attr_level_resources;
}

function resouces_click(aid){
	click_id = '.has_hidden_resources #click_button_'+aid;

	if($(click_id).hasClass('click_button_show')){
		$(click_id).removeClass('click_button_show').addClass('click_button_hidden').parent().prev('.attr_hidden ').show();
		$(click_id).parent().prev('.attr_hidden ').find('.bottom_resources .resources_content_bytags .resources-row')
		.each(function(index){
			if(index<2) {
				$(this).css({'display':'none'});
			}
		});
	}
	else if($(click_id).hasClass('click_button_hidden')){
		$(click_id).removeClass('click_button_hidden').addClass('click_button_show').parent().prev('.attr_hidden ').hide();
	}


	return false;

}
function exam_resources_click(){
	var click_id = '#exam_click_button';
	var exam_offest = $('.exam_resources .resources_content_bytags').offset();
	var basetop = exam_offest.top;
	var maxtop = basetop;
	//alert('exam_offest:'+exam_offest.top);
	$('.exam_resources .resources-row').each(function(index){
		var exam_row_offset = $(this).offset();
		 maxtop = exam_row_offset.top;

	});

	var visibleheight = maxtop - basetop;



	if(visibleheight>100){
		//$(click_id).click(function(){
			if($(click_id).hasClass('click_button_show')){
				$(click_id).removeClass('click_button_show').addClass('click_button_hidden').parent().prev('.resources_content_bytags ').css('height',(visibleheight+195)+'px');
			}
			else if($(click_id).hasClass('click_button_hidden')){
				$(click_id).removeClass('click_button_hidden').addClass('click_button_show').parent().prev('.resources_content_bytags ').css('height','195px');
			}
		//});
	}

}
