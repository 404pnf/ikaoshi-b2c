(function($){
	$.fn.tags_list=function(op,options){
		var settings=$.extend({
			type : "detail",
			id : "tags",
			attribution : ""//专为考点评语设置
		},options||{});
	
	var that=this;
	
	
	if(op == "attr_comments")
	{
		$(that).html('');
		
		var url = '/itest/egs/index.php?op='+op;
		
		
		if(settings.attribution!=''){
			url += "&attr_id="+settings.attribution;
		}
		
		$.getJSON(url,function(data){		
			
		$.each(data,function(index,attr){
			//"attribution":[{"aid":"46","name":"\u9605\u8bfb\u80fd\u529b"}],
			//"comment":[{"level_name":"\u521d\u7ea7","description":"ceshi ","weight":"10"}]
			
			var attributions = attr['attribution'][0];
			var tags = attr['comment'];
			
			var attr_id = attributions['aid'];
			var attr_name = attributions['name'];
			
			//alert(attr_id);
			$(that).append('<div class="attr_'+attr_id+'"><div class="attribution">'+attr_name+'</div><table id="'+settings.id+'" class="defaultTable"><thead>	<th class="weight">权重</th><th  class="name">名称</th><th class="description">描述</th><th class="operate">操作</th></thead><tbody class="attr_'+attr_id+'_list"></tbody> </table></div>');
	
			var list_content='tbody.attr_'+attr_id+'_list';
		if(tags==null){
			//$('.attr_'+attr_id).remove();
			var row_pre='<tr class="content-none">';
			var row_suf='</tr>';
			
			var row_content_none='<td colspan = 4 >此考点没有添加任何评语等级，则无法给用户评价以及推荐资源</td>';
			$(list_content).append(row_pre+row_content_none+row_suf);
		}
		else
			$.each(tags,function(index,tag){
				//alert($(that).html());
				var temp_array=	tag;
				
				var title=temp_array['level_name'];
				var description=temp_array['description'];
				var weight=temp_array['weight'];		
				
				var operate='<a href="javascript:void(0);return false;" onclick=\'\'>修改</a>'
				
				
				var row_pre='<tr class='+index+'>';
				var row_suf='</tr>';
				
				var row_weight='<td class="weight">'+weight+'</td>';
				var row_title='<td class="name">'+title+'</td>';
				var row_description='<td class="description">'+description+'</td>';
				
				var row_operate='<td class="operate">'+operate+'</td>';
				
				
				$(list_content).append(row_pre+row_weight+row_title+row_description+row_operate+row_suf);
				
			});//--end $.each()
			
		});//--end $.each()		
		});//--end $.getJson()
	}
	else{
	
	$(that).html('<table id="'+settings.id+'" class="defaultTable"><thead>	<th class="weight">权重</th><th  class="name">名称</th><th class="description">描述</th><th class="operate">操作</th></thead><tbody class="'+settings.id+'_list"></tbody> </table>');
	
	var list_content='tbody.'+settings.id+'_list';
	$(list_content).html('<tr><td colspan="4"><div class="loadging"><img width="25px" height="25px" src="images/loading.gif"></div></td></tr>');
	$(list_content).html('');
	$.getJSON('/itest/egs/index.php?type='+settings.type+'&op='+op,function(data){		
	
	$(list_content).html('');
	$.each(data,function(index,tags){
		//alert($(that).html());
		var temp_array=	tags;
		
		var title=temp_array['name'];
		var description=temp_array['description'];
		var weight=temp_array['weight'];		
		
		var operate='<a href="javascript:void(0);return false;" onclick=\'\'>修改</a>'
		
		
		var row_pre='<tr class='+index+'>';
		var row_suf='</tr>';
		
		var row_weight='<td class="weight">'+weight+'</td>';
		var row_title='<td class="name">'+title+'</td>';
		var row_description='<td class="description">'+description+'</td>';
		
		var row_operate='<td class="operate">'+operate+'</td>';
		
		
		$(list_content).append(row_pre+row_weight+row_title+row_description+row_operate+row_suf);
		
	});//--end $.each()
			
	});//--end $.getJson()
	} 
	return false;  
}
})(jQuery);