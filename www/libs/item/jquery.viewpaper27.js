var serial=serial_number;
var paper_maincontent;
var adminid;
var time_limit=0;

(function($){
	$.fn.view_paper=function(id,options){
		var settings=$.extend({
			titleContent:'.title',
			user_id:'0',
			paper_url:'',
			paper_type:''
		},options||{});
	var that=this;
	$(that).html('<div class="loadging"><img width="25px" height="25px" src="images/loading.gif"></div>');
	paper_maincontent=that;


	if(adminid == 1)
		printop = '<a class="floatr notprint" href="javascript:void();" onclick="printPreview();"> 打印试卷预览 </a><a class="doprint" style = "visibility:hidden;" href="javascript:void();" onclick="doPrint();"> 打印 </a>';

	else
		printop = '';

	url = '/itest/egs/index.php?op=get_paper&type=detail&id='+id+'&uid='+user_id;


	//url = '/itest/egs/index.php?op=paper_item&id='+id;

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


		var paper_info=resp['paper_info'][id];

		var paper_name=paper_info['paper_name'];
		var examType=paper_info['exam_type'];
		var sid = paper_info['sid'];

		var description=paper_info['description'];
		var points=parseInt(paper_info['points']);
		var time_limit=paper_info['time_limit'];


		//var is_full=paper_info['is_full'];

		if(examType.indexOf("真题")>=0)
				$(settings.titleContent).html('<span id="title_f">'+paper_name+'</span><a class="btn_05 notprint" href="http://answer.2u4u.com.cn/node/add/best-answer" target="_blank">我要提问</a>'+printop);
		else
			$(settings.titleContent).html('<span id="title_f">'+paper_name+'</span><a class="btn_05 notprint" href="javascript:void(0);" onclick=\'$(".paper_content").create_paper('+sid+',{titleContent:"'+settings.titleContent+'",user_id:"'+settings.user_id+'"});serial_total=0;clearInterval(timer);return false;\'>换一套试卷</a><a class="btn_05 notprint" href="http://answer.2u4u.com.cn/node/add/best-answer" target="_blank">我要提问</a>'+printop);

		var output='<div class="paper_info"><div class="points_time clearfix"><div class="points fleft">试卷总分：'+points+'</div><div class="notprint timer fright"><span class="time_limit">试卷时间：'+time_limit+'</span><a id="stop" href="javascript:stop_t();" style="display:inline;"> 暂停计时 </a><a id="start" href="javascript:start_t();" style="display:none;"> 启动计时 </a><a id="restart" href="javascript:restart_t('+time_limit+');"> 清零计时 </a></div></div><div class="paper_description">'+description+'</div></div>';


		$(that).html(output);
		//试卷基本信息的输出
		$('.time_limit').jQueryTimer(time_limit);//时间倒计时




					var paper_array=resp['paper_items'];
					var output='<div class="paper"><form id="do_ques_form" action="" method="post"><input type="hidden" name="paper_id" value="'+id+'"/><input type="hidden" name="user_id" value="'+settings.user_id+'"/><input type="hidden" name="use_time" class="use_time" value="0"/><div class="allSubques">';
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
						output +='<input type="submit" class="submit notprint" name="submit" value="提交"/>'+printop+'</div></form></div>';

					$(that).append(output);


	/*

	if ( typeof CKEDITOR == 'undefined' )
	{
		alert('<strong><span style="color: #ff0000">Error</span>: CKEditor not found</strong>.This sample assumes that CKEditor (not included with CKFinder) is installed in the "/ckeditor/" path. If you have it installed in a different place, just edit this file, changing the wrong paths in the &lt;head&gt; (line 5) and the "BasePath" value (line 32).' );
	}
	else
	{
		//var editor = CKEDITOR.replace( '.ckeditor_basic');
		//editor.config.toolbar = "Full";
		//CKFinder.setupCKEditor( editor, '/itest/www/libs/ckfinder/');
		//editor.config.width = 880;
		//editor.config.height = 200;
		//alert('abc'+editor);



		CKEDITOR.config.toolbar = [ [ 'Source', '-', 'Bold', 'Italic', 'Underline', 'Strike','-','Link', '-', 'Image' ] ];
		//CKFinder.setupCKEditor( CKEDITOR, '/itest/www/libs/ckfinder/');
		//CKEDITOR.config.width = 400;
		CKEDITOR.config.height = 200;

		$('.ckeditor_basic').each(function(){
			var editor_basic = CKEDITOR.replace(this);
			//editor_basic.config.width = 800;
		});
	}

	*/



		$(".disnext").css("cursor","pointer").click(function(){
				$(this).parent().parent().hide().next().show('slow');
		});
		$(".disprev").css("cursor","pointer").click(function(){
				$(this).parent().parent().hide().prev().show('slow');
		});

	$(".jplayer").myjplayer();

		$(".serial_name_num_name").toggle(
			function(){$(this).nextAll('.allSubques').hide('slow');},
			function(){$(this).nextAll('.allSubques').show('slow');}
		);


		$('<div style="text-align:right" class="sizecontrol notprint">字体：[<span style="cursor:pointer" class="bigsize"> 大 </span><span style="cursor:pointer" class="middlesize"> 中 </span><span style="cursor:pointer" class="smallsize on"> 小 </span>]</div>').prependTo(".paper_info").hide().slideDown('slow');
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



		//add_ques_form 的提交
		var add_ques = {
			url:		'/itest/egs/user_answer_post.php',
			type:		'post',
			target:   	'#footer',// target element to update
			dataType: 	'json',
			beforeSubmit:  clearInterval(timer),  // pre-submit callback
			success:     view_paper_open_by_status //提交后进行返回状态的判断，并确定是否创建子题目
		};
		$('#do_ques_form').ajaxForm(add_ques);

		start_t(time_limit);


			}
			catch (err)
			{
				resp = undefined;
				$(that).append('服务器交互失败，请重试'+err);
			}


		}
	});


   }
})(jQuery);






function showRequest(formData, jqForm) {
   $("#footer").html('About to submit: \n\n' + $.param(formData));
	return true;
}

function view_paper_open_by_status(data){

	var status=data.status;
	var id=data.id;
		//alert('status:'+status+' id:'+id);
	if(status==1) {
		paper_maincontent.view_userresult(id,{operation:"DiagnosticReport",titleContent:".paper_title"});

	}
	else if(status==0) 		$("#footer").html('保存失败，名称重复或缺少必要信息');
	else
		$("#footer").html('服务器交互失败，请重试');
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

	if(files!=undefined&&files!=''&&file_status==1)
		ques_files='<div class="jplayer_wrap"><a id="jplayer-'+item_id+'" class="mp3 jplayer notprint" src='+fileurl+'></a></div> ';


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
			ques_body = '<div class="ques_body_description">'+ques_body+ques_points+'</div>';

		if(ques_serial_name!=''||ques_serial_number!=''||ques_files != ''){
			ques_serial_ques_name = ques_files + '<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+'</span></div>';
		}
		if((ques_serial_name!=''||ques_serial_number!=''||ques_files != '')&&(ques_body==''||ques_body==undefined)){

				ques_serial_ques_name = ques_files + '<div class="serial_name_num_name"><span class="fold_unfold_img">'+ques_serial_name+ques_serial_number+ques_name+ques_time_limit+ques_points+'</span></div>';

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
						ques_answers +='<div><input id="answer_'+index+'"  class="useranswer" type="checkbox" name="'+input_name+'"  value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
					});

				}
				else if(choice_multi==0){
					$.each(answers_array,function(index,answer){
						i++;
						ques_answers +='<div><input id="answer_'+index+'"  class="useranswer" type="radio" name="answer['+item_id+'][useranswer]" value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
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
				question +='<div class="submit_button  notprint"><div class="disnext"><img src="images/next.gif"></div></div></div><div class="allSubques" style="display:none;">';
			else if(layout == sumlay)
				question +='<div class="submit_button notprint"><div class="disprev"><img src="images/previous.gif"></div></div>';
			else	question +='<div class="submit_button notprint"><div class="disprev"><img src="images/previous.gif"></div><div class="disnext"><img src="images/next.gif"></div></div></div><div class="allSubques" style="display:none;">';

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

		if(files!=undefined&&files!=''&&file_status==1) ques_files='<div class="jplayer_wrap"><a id="jplayer-'+item_id+'" class="mp3 jplayer notprint" src='+fileurl+'></a></div> ';


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
				ques_body = '<div class="ques_body_description">'+ques_body+ques_points+'</div>';
			if(ques_serial_name!=''||ques_serial_number!=''||ques_files != ''){
				ques_serial_ques_name='<div class="serial_name_num_name">'+ques_files+ques_serial_name+ques_serial_number+'</div>';
			}
			if((ques_serial_name!=''||ques_serial_number!=''||ques_files != '')&&(ques_body==''||ques_body==undefined))
				ques_serial_ques_name='<div class="serial_name_num_name">'+ques_files+ques_serial_name+ques_serial_number+ques_points+'</div>';


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
						ques_answers +='<div><input id="answer_'+index+'"  class="useranswer" type="checkbox" name="'+input_name+'"  value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
					});

				}
				else if(choice_multi==0){
					$.each(answers_array,function(index,answer){
						i++;
						ques_answers +='<div><input id="answer_'+index+'"  class="useranswer" type="radio" name="answer['+item_id+'][useranswer]" value="'+index+'"/><label for="answer_'+index+'">'+serial_number[i]+answer["answer"]+'</label></div> ';
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

		//alert(question);

	});

	question +='</div>';


	return question;
	//本道试题全部输出完毕（包括其子题目的所有层级）后，返回本试题的所有html
}

