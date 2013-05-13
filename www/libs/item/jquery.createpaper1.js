(function($){
	$.fn.create_paper=function(id,options){
		var settings=$.extend({
			titleContent:'.title',
			user_id:'0'
		},options||{});
		var that=this;

		url = '/itest/egs/create_exam_paper_by_structure.php?id='+id+'&uid='+settings.user_id;

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


				try
				{

					if(resp.status==1){
						$(that).html('');
						that.view_paper(resp.id,{titleContent:settings.titleContent,user_id:settings.user_id});
					}
					else if(resp.status==0)  {
						$(that).html('试卷生成失败，请重试。message::'+resp.message);

					}
					else
						$(that).html('错误信息：'+resp.status);

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