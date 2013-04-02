

function listanimation(){
	//alert('123');
	$('#left_banner .leftrow').click(function(){
		leftid = this.id;
		
		id = leftid.substring(4);
		thatparent = $(this).parent('#left_banner');
		thatparent.animate(
			{
				left:-thatparent.width()
				//,opacity:'hide'
				
			},
			'slow',
			function(){}
		);
		
		$('#listcontent'+id).animate(
			{
				left:0
				//,opacity:'show'
			},
			'slow',
			function(){
				contenth = $('#listcontent'+id).height();
				winh = $(window).height();
				if(contenth > winh)
					$('#main').height(contenth);
				else
					$('#main').height(winh);
				
			}
		);

	});
	
	$('.backbutton').click(function(){
		$('.listcontent').animate(
			{
				left:$('.listcontent').width()
				//,opacity:'show'
			},
			'slow',
			function(){}
		);
		
		$('#left_banner').animate(
			{
				left:0
				//,opacity:'hide'
				
			},
			'slow',
			function(){
				contenth = $('#left_banner').height();
				winh = $(window).height();
				if(contenth > winh)
					$('#main').height(contenth);
				else
					$('#main').height(winh);
			}
		);
	});
	
	$('.detaillist .row').attr('style','cursor:pointer;').click(function(){
		location.href = $(this).find('a').attr('href');
	});
	
	
}