function impromptu_content(type, title){
	
	var title='<div class="'+type+'_title title">'+title+'</div>';	
	var content='<div class="content">抱歉，没有找到！</div>';
	
	if(type == 'bangzhu'){
	//帮助中心	
		content = '<div class="bangzhu_content"><div>    大家好！欢迎大家使用外研社EGS考试训练系统，希望能够为备考试的您提供帮助！ </div><p>    EGS系统最大的特点是<font color="red">随机组卷</font>功能，即用户所做的不是固定的试卷，而是实时生成的，每个人的试卷都不同！每个人都可以去体验一键生成试卷的快感！ </p><div style="background:none repeat scroll 0 0 #F8EFBC;">专项测试                                                                       </div>  <p>    EGS系统为用户提供英语词汇、语法、听力、快速阅读、阅读理解、完形填空、改错、翻译、写作等所有考试题型的专项训练，用户可以根据自己的弱点有针对性的进行强化训练。</p>  <div style="background:none repeat scroll 0 0 #F8EFBC;">模考测试 </div>	<p>    EGS系统为用户提供了大学英语四级、大学英语六级、英语专业四级、英语专业八级、考研英语、PETS、高等学校英语应用能力考试、商务英语、雅思、托福、托业等大量外语考试的在线模拟。 </p>			<div style="background:none repeat scroll 0 0 #F8EFBC;">我的历史记录</div>	<p>    用户可以查看自己的所有做题记录，为复习提供更多的参考。 </p></div>';
	}
	else if(type == 'user_active'){
	//活跃用户
		content='<div class="user_active_content"><table class="table1"><tr><th>序号</th><th>用户</th><th>生成试卷数量</th> </tr></table><table class="table2"><tr><th>序号</th><th>用户</th><th>生成试卷数量</th></tr></table></div>';
		
		$.getJSON('/itest/egs/index.php?op=active_users',function(data){	
			
			$.each(data,function(index,item){
				//alert(index%2);
				var  num = 0, name = '', id = 0;				
				num = item['num'];
				
				$.each(item['info'],function(info_id,info_name){
					id = info_id;
					name = info_name;
				});
				//content += id+name+num;
				if(index%2==0)
					$('.user_active_content .table1').append('<tr><td>'+(index+1)+'</td><td>'+name+'</td><td>'+num+'</td></tr>');
				if(index%2==1)
					$('.user_active_content .table2').append('<tr><td>'+(index+1)+'</td><td>'+name+'</td><td>'+num+'</td></tr>');
			});
		
		});	
		
		
	}
	else if(type == 'user_score'){	
		//用户排行榜	{"1":"\u56db\u7ea7","4":"\u516d\u7ea7","5":"GMAT\u8003\u8bd5","6":"GRE\u8003\u8bd5","7":"LSAT\u8003\u8bd5","8":"\u96c5\u601d","9":"TOEFL","10":"\u4e13\u4e1a\u82f1\u8bed\u56db\u7ea7","11":"\u6d4b\u8bd5\u7c7b\u8bd5\u5377","14":"\u97e9\u8bed\u6c34\u5e73\u8003\u8bd5"}
		
		content='<div class="user_score_content"></div>';
		
		$.getJSON('/itest/egs/index.php?type=simple&op=exam',function(data){	
			
			$.each(data,function(id,title){
				$('.user_score_content').append('<table class="tid_'+id+'"><caption>'+title+'</caption><tr><th>序号</th><th>用户</th><th>分数</th> <th>客观题正确率</th></tr></table>');
				//alert(id+title);
				$.getJSON('/itest/egs/index.php?op=user_rate_rank&id='+id,function(data){	
					tid = id;
					if(data['status'] == 1){
						$.each(data['list'],function(index,item){
							var  rate = 0, name = '', id = 0, score = 0, time = '';
							rate = item['rate']*100+'%';
							score = item['score'];
							time = item['time_end'];
							$.each(item['info'],function(info_id,info_name){
								id = info_id;
								name = info_name;
							});	
							
							$('.user_score_content .tid_'+tid).append('<tr><td>'+(index+1)+'</td><td>'+name+'</td><td>'+score+'</td><td>'+rate+'</td></tr>');
							
						});
					}
					else if(data['status'] == 0){
						$('.user_score_content .tid_'+tid).css({'display':'none'});
					}
			});	
		});
			
	});
		
		//content='<div class="user_score_content"><table class="table1"><tr><th>用户</th><th>生成试卷数量</th> </tr></table><table class="table2"><tr><th>用户</th><th>生成试卷数量</th></tr></table></div>';
		
	
	}
	else {
		alert('抱歉，没有找到！');
		return false;
	}
		
		$.prompt(title+content,{ buttons:{'关闭':true} });
							
	
}       

