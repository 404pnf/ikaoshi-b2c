var comment_name_option="",attributions_name_radio_input="",attribution_comments_level_option="";// comments_level 下拉菜单的内容
var row_i = 4;//评语设置的行数，通过全局变量监测行数的增加

$(function() {
   
   
   //组卷模版的提交
    var options1 = {
		url:			'/itest/egs/exam_structure.php',
		type:			'post',
        target:        '#bottom-message',   // target element to update
		dataType: 	'json',
        //beforeSubmit:  showRequest,  // pre-submit callback
        success:       open_by_status  // post-submit callback
    };
    $('#zujuan_form').ajaxForm(options1);
	
	
	
	
	
	// comments_level 下拉菜单的内容
	$.getJSON("/itest/egs/index.php?op=comments_level",function(data){	
	
		$.each(data,function(comment_id,comment_name){
			
			comment_name_option += '<option value="'+comment_id+'">'+comment_name+'</option>';			 
		});
		
		//alert(comment_name_option);
	});
	
	
	// attribution_comments_level 下拉菜单的内容
	$.getJSON("/itest/egs/index.php?op=attribution_comments_level",function(data){	
	
		$.each(data,function(level_id,level_name){
			
			attribution_comments_level_option += '<option value="'+level_id+'">'+level_name+'</option>';			 
		});		
		//alert(comment_name_option);modules_comments(18,"whx测试");
	});
	
	
	
	// comments_attr 的 radio input
	$.getJSON("/itest/egs/index.php?op=attribution_info&type=simple",function(data){	
		
		$.each(data,function(index,attributions){
			aid = attributions['aid'];
			attribution_name = attributions['name'];

			
			pre_div ='';
			
			if(index==0) 
				pre_div = '<div class="group">';
			else if(index%4.0==0)
				pre_div = '</div><div class="group">';
			
			
			attributions_name_radio_input += pre_div+'<div class="field"><input type="radio" name="attribution" id="attribution_'+aid+'" value="'+aid+'" class="radioinput" />	<label for="attribution_'+aid+'">'+attribution_name+'</label> </div>';
			
			
		});		
		
		attributions_name_radio_input += '</div>';
		
		//alert(attributions_name_radio_input);
	});
	
	
});

function open_by_status(data){
	var status=data.status;
	var id=data.id;
	var modules_name=data.name;
	var comments_name=data.comments_name;
	
	if(status==1) {
		 $("#globalmessage").html('保存成功！');
		 alert('保存成功！');
		// location.href = "manage_modules.html";
	}else if(status==2) {
		modules_comments(id,modules_name);
	}
	else if(status==0)  $("#globalmessage").html('保存失败，名称重复或缺少必要信息');
	else
		$("#globalmessage").html('服务器交互失败，请重试');

}

function open_by_status2(data){
	
	var status=data.status;
	var sid=data.sid;
	var modules_name=data.title;
	//alert(status+sid+modules_name);
	if(status==1) {
		var title='<div class="add_tag_title title">'+modules_name+'</div>';
		var output='<div class="content">保存成功！</div>';
		rebackUrl = "manage_modules.html";
		$.prompt.close();
		
		$.prompt(title+output,
			{buttons: { 测试生成试卷: 2, 返回模板列表: 1, 取消: 0},
				focus: 0,
				submit: function(v, m, f){ 					
					if(v == 2){
						$(".zujuan_content").addClass('paper_content');
						$(".zujuan_title").addClass('paper_title');
						$(".paper_content").create_paper(sid,{titleContent:".paper_title",user_id:user_id});
					}
					else if(v == 1){
						$('#mainContent').load(rebackUrl);
					}
					$.prompt.close();
					return false; 					
				}
		 });
		 
		 return true;
		 
	}
	else if(status == 0) { alert('您没有填写任何数据，如果不想设置评语，请点击“取消”！'); return false;}
	else if(status == 2) { alert('您提交的数据不完整，请检查后再提交！'); return false;}
	else
		$("#globalmessage").html('服务器交互失败，请重试');

}



function modules_comments(id, modules_name){
	
	var modules_commentsUrl="/itest/egs/index.php?op=structure_comment&type=detail&id="+id;
	
	$.getJSON(modules_commentsUrl,function(data){	
		//alert(modules_commentsUrl+data['count']);
		if(data['count'] > 0) {
			
			 comments = data['comment'];
			 $('.modules_comments_rows').html('');
			 row_i = data['count'];
			
			i = 0;
			$.each(comments,function(scid,comment){
				var resources='',resources_tags1='',or_resources_tags2='';
				
				resources = comment["resources"]["resources"];
				resources_tags1 = comment["resources"]["resources_tags1"];
				or_resources_tags2 = comment["resources"]["or_resources_tags2"];
				
				if(resources==null || resources=='null' || resources==undefined) resources='';
				if(resources_tags1==null || resources_tags1=='null' || resources_tags1==undefined) resources_tags1='';
				if(or_resources_tags2==null || or_resources_tags2=='null' || or_resources_tags2==undefined) or_resources_tags2='';
				
				$('.modules_comments_rows').append('<div class="modules_comments row_'+i+'  clearfix"><input type="hidden" name="comments['+i+'][scid]" value="'+scid+'"/><span class="score_range"><label>正确率范围：</label><input name="comments['+i+'][min_rate]" type="text" class="min_rate" size="8" value="'+comment["min_rate"]+'"/> — <input name="comments['+i+'][max_rate]" type="text" class="max_rate" size="8" value="'+comment["max_rate"]+'"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments['+i+'][level_id]"  class="level_id" ><option  value="">-请选择-</option>'+comment_name_option+'</select></span><div class="resources-content"><label>资源tags：</label>  <input value="'+resources_tags1+'" name="comments['+i+'][resources_tags1]" class="resources_tags1"> OR <input value="'+or_resources_tags2+'" name="comments['+i+'][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments['+i+'][resources]" class="resources" rows="3" cols="45" > '+resources+'</textarea></div></div>');
				
					$('.modules_comments_rows .row_'+i+' .level_id').val(comment["level"]["level_id"]);
					
				i++;		 
			});		
		}
		//alert(comment_name_option);modules_comments(18,"whx测试");
	});
	
	var postUrl="/itest/egs/add_structure_comment.php";
//alert(id+modules_name+comment_name_option);
	
	var title='<div class="add_tag_title title">'+modules_name+' 评语设置</div>';
	var modules_comments='<div class="modules_comments_content content"><form id="modules_comments_form" action="" method="POST" width="600px">	<input type="hidden" name="sid" value="'+id+'"/>	<div class="modules_comments_rows"><div class="modules_comments row_1  clearfix"><span class="score_range"><label>正确率范围：</label><input name="comments[1][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[1][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="comments_name" class="">评语：</label><select name="comments[1][level_id]"  class="level_id"><option value="">-请选择-</option>'+comment_name_option+'</select></span><div class="resources-content"><label>资源tags：</label>  <input value="" name="comments[1][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments[1][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments[1][resources]" class="resources" rows="3" cols="45" ></textarea></div></div>			<div class="modules_comments row_2   clearfix"><span class="score_range"><label>正确率范围：</label><input name="comments[2][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[2][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments[2][level_id]"  class="level_id" ><option value="">-请选择-</option>'+comment_name_option+'</select></span><div class="resources-content"><label>资源tags：</label>  <input value="" name="comments[2][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments[2][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments[2][resources]" class="resources" rows="3" cols="45" ></textarea></div></div>			<div class="modules_comments row_3  clearfix"><span class="score_range"><label>正确率范围：</label><input name="comments[3][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[3][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments[3][level_id]"  class="level_id"><option value="">-请选择-</option>'+comment_name_option+'</select></span><div class="resources-content"><label>资源tags：</label>  <input value="" name="comments[3][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments[3][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments[3][resources]" class="resources" rows="3" cols="45" ></textarea></div></div> </div><a href="javascript:void(0);" onclick=\"return add_modules_comments_row();\">添加</a>               <button type="submit" class="submit jqidefaultbutton" name="btn_submit" value="提交">提交</button> </form></div>';
								
		$.prompt(title+modules_comments,
				{buttons: { 取消: 0 },
				focus: 1,
				loaded:function(){
					$(".jqibuttons").appendTo("#modules_comments_form");
					$(".jqidefaultbutton").prependTo('.jqibuttons');
					
					//modules_comments_form 的提交
					var modules_comments_form = {
							url:			postUrl,
							type:			'post',
							dataType: 	'json',
        					//beforeSubmit:  showRequest,  // pre-submit callback
        					success:       open_by_status2  // post-submit callback
							//success:       ocation.href = "manage_modules.html";}//提交后返回模板列表
						};
					
					$('#modules_comments_form').ajaxForm(modules_comments_form);	
				},
				submit:function(v){ 
					if(v==0){
						$.prompt.close();
						return true; //we're done
					}
					return false; 
				}
				
			});
							
	
}
	
function add_modules_comments_row(){
	//alert(row_i);
	var modules_comments_row = '<div class="modules_comments row_'+row_i+'  clearfix"><span class="score_range"><label>正确率范围：</label><input name="comments['+row_i+'][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments['+row_i+'][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments['+row_i+'][level_id]"  class="level_id"><option value="">-请选择-</option>'+comment_name_option+'</select></span><div class="resources-content"><label>资源tags：</label>  <input value="" name="comments['+row_i+'][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments['+row_i+'][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments['+row_i+'][resources]" class="resources" rows="3" cols="45" ></textarea></div></div>';
	
	$('.modules_comments_rows').append(modules_comments_row);
	
	row_i = row_i + 1;
	
	return false;
	
}


	
function 	comment_attributions(id, modules_name){
	
	var title='<div class="add_tag_title title">'+modules_name+' 考点评语设置</div>';
	
	var html = '<div><input type="hidden" name="sid" id="sid" value="'+id+'"/><input type="hidden" name="modules_name" id="modules_name"  value="'+modules_name+'"/><label class="attr_label">请选择考点:</label></div>'+attributions_name_radio_input;
//alert(title);
	$.prompt(title+html,{
		buttons: { 取消: false, 下一步: true },
		focus: 1,
		submit:function(v,m,f){ 
			if(!v)
				$.prompt.close()
			else{
				attr_comments(v,m,f);
			}
			//return false; 
		}
	});
}




function attr_comments(v,m,f){
	id = f.sid;
	modules_name = f.modules_name;
	aid = f.attribution;

	var postUrl="/itest/egs/add_structure_attr_comment.php";
	
	var title='<div class="add_tag_title title">'+modules_name+' 考点评语设置</div>';
	var modules_comments='<div class="modules_comments_content content"><form id="attr_modules_comments_form" action="/itest/egs/add_structure_attr_comment.php" method="POST" >	<input type="hidden" name="sid" value="'+id+'"/>	<input type="hidden" name="aid" value="'+aid+'"/>	 <div class="modules_comments_rows"><div class="modules_comments row_1  clearfix">	<span class="score_range"><label>正确率范围：</label> <input name="comments[1][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[1][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="comments_name" class="">评语：</label><select name="comments[1][level_id]"  class="level_id"><option value="">-请选择-</option>'+attribution_comments_level_option+'</select></span>	<div class="resources-content"><label>资源tags：</label>  <input value="" name="comments[1][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments[1][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments[1][resources]" class="resources" rows="3" cols="45" ></textarea></div></div>			<div class="modules_comments row_2   clearfix"><span class="score_range"><label>正确率范围：</label> <input name="comments[2][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[2][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments[2][level_id]"  class="level_id" ><option value="">-请选择-</option>'+attribution_comments_level_option+'</select></span> <div class="resources-content"><label>资源tags：</label>  <input value="" name="comments[2][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments[2][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments[2][resources]" class="resources" rows="3" cols="45" ></textarea></div></div>			<div class="modules_comments row_3  clearfix">	<span class="score_range"><label>正确率范围：</label> <input name="comments[3][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[3][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments[3][level_id]"  class="level_id"><option value="">-请选择-</option>'+attribution_comments_level_option+'</select></span> <div class="resources-content"><label>资源tags：</label>  <input value="" name="comments[3][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments[3][or_resources_tags2]" class="or_resources_tags2"></div><div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments[3][resources]" class="resources" rows="3" cols="45" ></textarea></div></div> </div><a href="javascript:void(0);" onclick=\"return add_attr_modules_comments_row();\">添加</a>                </form></div>';
		
		if(f.attribution != undefined && f.attribution !='')
			$.prompt(title+modules_comments,{
				buttons: { 返回上一步: -1, 提交并设置下一考点: 0,完成: 1 },
				focus: 1,
				loaded:function(){
					//modules_comments_form 的提交
					var modules_comments_form = {
						url:			postUrl,
						type:			'post',
						dataType: 	'json',
						//beforeSubmit:  showRequest,  // pre-submit callback
						success:       open_by_status2  // post-submit callback
						//success:       ocation.href = "manage_modules.html";}//提交后返回模板列表
					};
					$('#attr_modules_comments_form').ajaxForm(modules_comments_form);	
				},
				submit:function(v,m,f){ 
					if(v==-1){
						comment_attributions(id,modules_name);
					}
					else if(v==0){
						//alert(v);
						
						$('#attr_modules_comments_form').ajaxSubmit();
						comment_attributions(id,modules_name);
						//$.prompt.close();$.prompt.goToState('state1');//go forward
					}
					else{
						$('#attr_modules_comments_form').ajaxSubmit();	
						//$.prompt.close();
					}
					//return false; 
				}
			});
		else
			$.prompt('请“返回上一步”，先设置考点。 ',{
				buttons: { 返回上一步: -1, 取消: 1 },
				focus: 0,
				submit:function(v,m,f){ 
					if(v==-1)
						comment_attributions(id,modules_name);
					else{
						$.prompt.close();
					}
					//return false; 
				}
			});
		
		
}


function add_attr_modules_comments_row(){
	//alert(row_i);
	var modules_comments_row = '<div class="modules_comments row_'+row_i+'  clearfix"><span class="score_range"><label>正确率范围：</label> <input name="comments['+row_i+'][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments['+row_i+'][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments['+row_i+'][level_id]"  class="level_id"><option value="">-请选择-</option>'+attribution_comments_level_option+'</select></span> <div class="resources-content"><label>资源tags：</label>  <input value="" name="comments['+row_i+'][resources_tags1]" class="resources_tags1"> OR <input value="" name="comments['+row_i+'][or_resources_tags2]" class="or_resources_tags2"></div> <div class="resources" ><label class="floatl" for="resources">推荐资源：</label><textarea name = "comments['+row_i+'][resources]" class="resources" rows="3" cols="45" ></textarea></div></div>';
	
	$('.modules_comments_rows').append(modules_comments_row);
	
	row_i = row_i + 1;
	
	return false;
	
}




    // pre-submit callback
    function showRequest(formData, jqForm) {
        alert('About to submit: \n\n' + $.param(formData));
        return true;
    }

    // post-submit callback
    function showResponse(responseText, statusText)  {
      if(statusText==true) $('#bottom-message').html(responseText);
	  // alert('this: ' + this.tagName +'\nstatus: ' + statusText + '\n\nresponseText: \n' + responseText + '\n\nThe output div should have already been updated with the responseText.');
    }
	