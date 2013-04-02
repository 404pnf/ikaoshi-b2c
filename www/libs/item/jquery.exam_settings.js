
var comment_name_option="";
url = "/itest/egs/index.php?op=comments_level";
	
	$.ajax({
		url: url,
		dataType: 'json',
		//data: data,
		timeout:200000,// 设置请求超时时间（毫秒）。 
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。  
			$(that).html(" 请求超时! textStatus: " + textStatus + ', errorThrown: ' + errorThrown);
		 }, 
		success: function(resp) 
		{  //请求成功后回调函数。
		
			$.each(resp,function(comment_id,comment_name){
	
			comment_name_option += '<option value="'+comment_id+'">'+comment_name+'</option>';			 
			});
			
			
		}
	});
	
(function($){
	$.fn.exam_default_comments_setting=function(id, title_name){
	
	var that = this;
	
	var postUrl="/itest/egs/add_structure_comments_global.php";
		
	var title='<div class="add_tag_title title">'+title_name+' 评语设置</div>';
	var modules_comments='<div class="modules_comments_content content"><form id="modules_comments_form" action="/itest/egs/add_structure_comments_global.php" method="POST" width="600px">	<input type="hidden" name="exam_type_id" value="'+id+'"/>	<div class="modules_comments_rows"><div class="modules_comments row_1  clearfix"><span class="score_range"><label>正确率范围（小 — 大）：</label><input name="comments[1][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[1][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="comments_name" class="">评语：</label><select name="comments[1][level_id]"  class="level_id"><option value="">-请选择-</option>'+comment_name_option+'</select></span><span class="delete">删除</span></div>			<div class="modules_comments row_2   clearfix"><span class="score_range"><label>正确率范围（小 — 大）：</label><input name="comments[2][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[2][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments[2][level_id]"  class="level_id" ><option value="">-请选择-</option>'+comment_name_option+'</select></span><span class="delete">删除</span></div>			<div class="modules_comments row_3  clearfix"><span class="score_range"><label>正确率范围（小 — 大）：</label><input name="comments[3][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments[3][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments[3][level_id]"  class="level_id"><option value="">-请选择-</option>'+comment_name_option+'</select></span><span class="delete">删除</span></div>     </div><a href="javascript:void(0);" onclick=\"return add_comments_row();\">添加</a>               <button type="submit" class="submit jqidefaultbutton" name="btn_submit" value="提交">提交</button> </form></div>';
								
	
	$(that).html(title+modules_comments);
	
	var modules_commentsUrl="/itest/egs/index.php?op=structure_comment_global&type=detail&id="+id;
	
	$.ajax({
		url: modules_commentsUrl,
		dataType: 'json',
		//data: data,
		timeout:200000,// 设置请求超时时间（毫秒）。 
		error: function (XMLHttpRequest, textStatus, errorThrown) {// 请求失败时调用函数。  
			$(that).html(" 请求超时! textStatus: " + textStatus + ', errorThrown: ' + errorThrown);
		 }, 
		success: function(data) 
		{  //请求成功后回调函数。
			if(data['count'] > 0) {
				
				 comments = data['comment'];
				 $('.modules_comments_rows').html('');
				 row_i = data['count']+1;
				
				i = 0;
				$.each(comments,function(index,comment){
						
					i++;	

					$('.modules_comments_rows').append('<div class="modules_comments row_'+i+'  clearfix"><input type="hidden" name="comments['+i+'][id]" value="'+index+'"/><span class="score_range"><label>正确率范围（小 — 大）：</label><input name="comments['+i+'][min_rate]" type="text" class="min_rate" size="8" value="'+comment["min_rate"]+'"/> — <input name="comments['+i+'][max_rate]" type="text" class="max_rate" size="8" value="'+comment["max_rate"]+'"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments['+i+'][level_id]"  class="level_id" ><option  value="">-请选择-</option>'+comment_name_option+'</select></span><span class="delete">删除</span></div>');
					
						$('.modules_comments_rows .row_'+i+' .level_id').val(comment['level']["level_id"]);
						 
				});		
			}
			
		
			$('.delete').click(function(){
				$(this).parent().remove();
			});
			
			//modules_comments_form 的提交
			var modules_comments_form = {
					url:			postUrl,
					type:			'post',
					dataType: 		'json',
					//beforeSubmit:  showRequest,  // pre-submit callback
					success:       	callbackf  // post-submit callback
					//success:       alert('123');
				};
			
			$('#modules_comments_form').ajaxForm(modules_comments_form);	
						
		}
	});
	
	
	
	
}
})(jQuery);


var	row_i = 4;
	
function add_comments_row(){

	var modules_comments_row = '<div class="modules_comments row_'+row_i+'  clearfix"><span class="score_range"><label>正确率范围（小 — 大）：</label><input name="comments['+row_i+'][min_rate]" type="text" class="min_rate" size="8"/> — <input name="comments['+row_i+'][max_rate]" type="text" class="max_rate" size="8"/></span>	<span class="comments"><label for="level_id" class="">评语：</label><select name="comments['+row_i+'][level_id]"  class="level_id"><option value="">-请选择-</option>'+comment_name_option+'</select></span><span class="delete">删除</span></div>';
	
	$('.modules_comments_rows').append(modules_comments_row);
	
	row_i = row_i + 1;
	
	return false;
	
}

function callbackf(data){
	if(data.status==1) alert('成功');
}

 // pre-submit callback
    function showRequest(formData, jqForm) {
        alert('About to submit: \n\n' + $.param(formData));
        return true;
    }

