var listType='';//为搜索设定搜索类型与方式，比如是试卷还是模版paper_info 、 exam_structure_list

(function($){
	$.fn.paper_list=function(op,options){
		var settings=$.extend({
			add_way : "append",
			type : "detail",
			id : "0",
			num : 0,
			admin : 0
		},options||{});
	
	var that=this;
	
	listType=op;
	
	if(settings.add_way == 'append')	{
		 if(op == 'exam_structure_list'){
			$(that).append('<table id="paper_search_result">        <thead>        	<th  class="name">模拟题</th><th class="type">分类</th><th class="operate">操作</th>        </thead>         <tbody class="module_list"><tr><td colspan="3">loading......</td></tr>        </tbody>    </table>');
		}
		else if(op == 'paper_info' || op == 'paper_info_by_term_id'){
			$(that).append('<table id="paper_search_result">        <thead>        	<th  class="name">真题</th><th class="type">分类</th><th class="operate">操作</th>        </thead>         <tbody class="paper_list"><tr><td colspan="3">loading......</td></tr>        </tbody>    </table>');
		}
	}
	else{
		 if(op == 'exam_structure_list'){
			$(that).html('<table id="paper_search_result">        <thead>        	<th  class="name">模拟题</th><th class="type">分类</th><th class="operate">操作</th>        </thead>         <tbody class="module_list"><tr><td colspan="3">loading......</td></tr>        </tbody>    </table>');
		}
		else if(op == 'paper_info' || op == 'paper_info_by_term_id'){
			$(that).html('<table id="paper_search_result">        <thead>        	<th  class="name">真题</th><th class="type">分类</th><th class="operate">操作</th>        </thead>         <tbody class="paper_list"><tr><td colspan="3">loading......</td></tr>        </tbody>    </table>');
		}	
	}
	
	var url='/itest/egs/index.php?op='+op+'&type='+settings.type;
	
	if(settings.id != 0)  url='/itest/egs/index.php?op='+op+'&type='+settings.type+'&id='+settings.id;
	
	if(settings.num >0 )  url +='&num='+settings.num;
	

	$.getJSON(url,function(data){		
		//alert(data);
		if(data==null||data=='') $('table').html('<tr><td colspan="3">没有此类型的试卷</td></tr>');
		else{
			
			if(op=='paper_info' || op=='paper_info_by_term_id'){
				
				$('tbody.paper_list').html('');
				$.each(data,function(index,papers){
					
					var paper_array = papers;
					
					var paper_id = paper_array['paper_id'];
					var title = paper_array['paper_name'];
					var type_name = paper_array['exam_type'];			
					
					
					
					var operate = '<a href="javascript:void(0);" onclick=\'$(".paper_content").view_paper('+paper_id+',{titleContent:".paper_title",user_id:"'+user_id+'"});return false;\'> 做题 </a>';	
					
					if(settings.admin == 1)
						operate = '<a href="javascript:void(0);" onclick=\'$(".paper_content").view_paper('+paper_id+',{titleContent:".paper_title",user_id:"'+user_id+'"});return false;\'> 做题 </a> <a href="javascript:void(0);" onclick=\'delete_paper('+paper_id+');return false;\'> 删除 </a>';	
						
					var paper_row_pre = '<tr class=tr_'+paper_id+'>';
					var paper_row_suf = '</tr>';
					
					var paper_title = '<td class="name">	<a href="javascript:void(0);" onclick=\'$(".paper_content").view_paper('+paper_id+',{titleContent:".paper_title",user_id:"'+user_id+'"});return false;\'>'+title+'</a>	</td>';	
					
					var paper_type = '<td class="type">'+type_name+'</td>';
					var paper_operate = '<td class="operate">'+operate+'</td>';
					
					
					$('tbody.paper_list').append(paper_row_pre+paper_title+paper_type+paper_operate+paper_row_suf);
					
				});//--end $.each()
			}
			else if(op=='exam_structure_list' ){
				
				$('tbody.module_list').html('');
				$.each(data,function(index,papers){
					
					var paper_array = papers;
					
					var sid = paper_array['sid'];
					var title = paper_array['title'];
					var type_name = paper_array['type_name'];			
					
					var operate = '<a href="javascript:void(0);" onclick=\'$(".paper_content").create_paper('+sid+',{titleContent:".paper_title",user_id:"'+user_id+'"});return false;\'> 做题 </a>';			
					
					var paper_row_pre = '<tr class=tr_'+sid+'>';
					var paper_row_suf = '</tr>';
					
					var paper_title = '<td class="name"><a href="javascript:void(0);" onclick=\'$(".paper_content").create_paper('+sid+',{titleContent:".paper_title",user_id:"'+user_id+'"});return false;\'>'+title+'</a></td>';	
					var paper_type = '<td class="type">'+type_name+'</td>';
					var paper_operate = '<td class="operate">'+operate+'</td>';
					
					
					$('tbody.module_list').append(paper_row_pre+paper_title+paper_type+paper_operate+paper_row_suf);
					
				});//--end $.each()
			}
		}
			
	});//--end $.getJson()
	
	return false;  
}
})(jQuery);



(function($){
	$.fn.specialModuleList=function(op,options){
		var settings=$.extend({
			type : "detail",
			id : "0"
		},options||{});
	
	
		var that=this;
		$(that).html('');
		//$(that).html('<div id="mainContentInner"><div class="paper_title title">专项训练</div><div class="messages" id="globalmessage"></div><div class="paper_content content"></div></div>');
		
		listType=op;
			
		var url='/itest/www/data/moduleList.json';
	
		/*
			{
			"term" : "语法",
			"content" : [{
					"term" : "四级",
					"content" : {
							"0":{"title":"2010年12月四级真题及解析","id":"33"},
							"1":{"title":"四级模板 Model Test 1","id":"18"},
							"2":{"title":"2010年12月四级真题及解析","id":"33"}
					}
			},
		*/
		
		$.getJSON(url,function(data){		
			//alert(data);
			$.each(data,function(index,item){
				term1 = item['term'];
				content1 = item['content'];
				
				if(term1 == listType){
					$('.paper_title').html(term1);
					groupclassid = 0;
					$.each(content1,function(index,item){
						termclassid = index;
						term2 = item['term'];
						content2 = item['content'];
						
						if(termclassid%2==0){
							groupclassid++;
							$('.paper_content').append('<div class="specialgroup group_'+groupclassid+' clearfix"></div>');
						}
						$('.group_'+groupclassid).append('<table class="floatl term_'+termclassid+'"><caption>'+term2+'</caption></table>');
						var  title = '',  id = 0, operate =  '';
						
						var i = 0;
						$.each(content2,function(index,item){
							i = i+1;
							title = item['title'];
							id = item['id'];
							
							title_td = '<a href="javascript:void(0);" onclick=\'$(".paper_content").create_paper('+id+',{titleContent:".paper_title",user_id:"'+user_id+'"});return false;\'>'+title+'</a>';
							
							//operate = '<a href="javascript:void(0);" onclick=\'$(".paper_content").create_paper('+id+',{titleContent:".paper_title",user_id:"'+user_id+'"});return false;\'> 做题 </a>';		
							
							
							$('.paper_content .term_'+termclassid ).append('<tr class="tr_'+ id +'"><td>'+ title_td +'</td></tr>');
						});	
						
							
						
					});
					//$(".paper_content").create_paper(12,{titleContent:".paper_title",user_id:"5"});return false;
					return true;
				}
				
			});	
		});	
		
	}
	
})(jQuery);


function delete_paper(paper_id){
	
	url = "/itest/egs/index.php?op=paper_delete&id="+paper_id;
	$.getJSON(url,function(data){	
	
		if(data['count']!=0)	
			confirm_message = "已有用户做过此试卷，确定要删除此试卷的所有内容吗？";
		else
			confirm_message = "确定删除此试卷？";
			
		if(confirm(confirm_message)){
			url_delete = "/itest/egs/index.php?op=paper_delete&type=confirm&id="+paper_id;
			
			$.getJSON(url_delete,function(data){
				
				if(data['status'] == 1){
					$('.tr_'+paper_id).remove();
					alert('删除成功');
				}
				
			});
		}
	
			
	});	
}