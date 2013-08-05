(function($){

	$.fn.userresult_list=function(uid,options){
	
//	alert(uid);
	
		var settings=$.extend({
			type : "detail",
			tid	: 0,
			num : 0,
			admin : 0
		},options||{});

		
var that=this;
if(uid < 0 && settings.admin != 1)	 	
	$(that).html('请重新<a href="index.html">登录</a>');
else{
	if(settings.admin == 1 && uid < 0 )  uid = -1; 

	var url = '/itest/egs/index.php?op=user_results_list&type='+settings.type+'&id='+uid;
	
	if(settings.tid != 0 && settings.tid != null)  url +='&type_id='+settings.tid;
	
	if(settings.num >0 )  url +='&num='+settings.num;
	
	$(that).html('<tr><td colspan="5"><div class="loadging"><img width="25px" height="25px" src="images/loading.gif"> </div></td></tr>');
	
	var user_result_data =new Array(new Array()) ;
	var user_result_data_labelX = new Array();
	
	
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
	
	if(data==null || data=='' || data==[]) {
		$(that).html('<tr><td colspan="5">没有你的历史记录，请选择左侧的模考或专项测试，开始你的测试旅程。</td></tr>');
		$('.jQchart').css({'display':'none'});
		$('canvas').css({'display':'none'});
	}
	else{
		$(that).html('');
		$.each(data,function(index,user_result){
			
			var user_result_id = index;
			var user_result_array = user_result;
			
			var paper_id=user_result_array['paper_id'];		
			var paper_name=user_result_array['paper_name'];
			
			var created=user_result_array['time_end'];
			var user_score=user_result_array['user_score'];	
			var objective_points=user_result_array['objective_points'];
			var rate = user_result_array['rate'];
				if(rate !== false){
					user_result_data[0].push( parseFloat(rate*100).toFixed(2));
					user_result_data_labelX.push(paper_name);
				}
			var exam_points=user_result_array['exam_points'];	
			
			var operate='<a href="javascript:void(0);" onclick=\'$(".paper_content").view_userresult('+user_result_id+',{operation:"summary",titleContent:".paper_title",rebackUrl:"user_exam.html"});return false;\'> 各部分得分   </a> <a href="javascript:void(0)" onclick=\'$(".paper_content").view_userresult('+user_result_id+',{operation:"DiagnosticReport",titleContent:".paper_title",rebackUrl:"user_exam.html"});return false;\'>诊断报告</a> <a href="javascript:void(0);" onclick=\'$(".paper_content").view_userresult('+user_result_id+',{operation:"detail",titleContent:".paper_title"});return false;\'>   完整记录 </a>';
			
			
			var paper_row_pre='<tr class=row_'+user_result_id+'>';
			var paper_row_suf='</tr>';
			
			var paper_name='<td class="paper_name">'+paper_name+'<span class="score">（<span class="user_total_score">'+user_score+'</span>/'+objective_points+'）</span></td>';
			var paper_operate='<td class="operate">'+operate+'</td>';
			
			var paper_exam_points='<td class="exam_points">'+exam_points+'</td>';
			var paper_created='<td class="created">'+created+'</td>';
			//var paper_user_score='';
			//var paper_type='<td class="type">'+type_name+'</td>';

			$(that).append(paper_row_pre+paper_name+paper_operate+paper_exam_points+paper_created+paper_row_suf);
			
		});//--end $.each()
	}//--end else

user_result_data[0] = user_result_data[0].reverse();
user_result_data_labelX = user_result_data_labelX.reverse();
dataLength = user_result_data[0].length;

if(dataLength>0){
	$('canvas').css({'display':'block'});
	$('.jQchart').css({'display':'block'});
	if(dataLength > 30){
		user_result_data[0] = user_result_data[0].slice(dataLength-30,dataLength); 
		user_result_data_labelX = user_result_data_labelX.slice(dataLength-30,dataLength); 
	}
	var chartSetting1={
		 config:{
			title    : '<i>正确率(%)</i>',
			//type     : 'line',
			labelX : user_result_data_labelX,
			scaleY : {min: 0,max:100,gap:10},
			labelYunit:'(単位 %)' ,
			bgGradient :{
				from      : '#fdf7f1',
				to        : '#dddbda'
			},
			labelDataOffsetY	:-20,
			labelDataOffsetX	:0,
			//strokeStyle	:'red',
			width 	: 800,
			height 	: 200,
			colorSet : ['#c79102'],
			strokeStyle : '#bfbcbc' 
		},
		data :  user_result_data
	};
	$('#user_result_chart').jQchart(chartSetting1);
}
else{
	$('canvas').css({'display':'none'});
	$('.jQchart').css({'display':'none'});
}	

$('.jQchart-labelData').hover(function(){
	id = this.id.slice(18);
	$(this).append('<div class="labelX_hover" style="position:absolute;">'+$('#jQchart-labelX_'+id).text()+' ('+$(this).text()+'%)</div>');
},function(){
	$('.labelX_hover').remove();
});

$('.jQchart-labelX').hover(function(){
	id = this.id.slice(15);
	//alert($(this).offset().left);
	$(this).after('<div class="labelX_hover" style="position:absolute;">'+$(this).text()+' ('+$('#jQchart-labelData_'+id).text()+'%)</div>');
	$('.labelX_hover').offset({'left':$(this).offset().left+40});
},function(){
	$('.labelX_hover').remove();
});

		}
			catch (err)
			{
				resp = undefined;
				$(that).append('服务器交互失败，请重试'+err);
			}
			
		}	
	});//--end $.ajax()
}

	return false;  
}
})(jQuery);
