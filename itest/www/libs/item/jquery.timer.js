var totaltime,maxtime,timer,timecontent;
clearInterval(timer);

(function($){
	$.fn.jQueryTimer=function(t){


		timecontent=this;
		totaltime = t*60;
		maxtime = totaltime;

		//alert(t+'分钟，开始计时！');
		timer = setInterval("CountDown()",1000);
	}

})(jQuery)


	function CountDown(){
			var negetive='';
				temtime=Math.abs(maxtime);
				hours = Math.floor(temtime/3600);
				minutes = Math.floor(temtime%3600/60);
				seconds = Math.floor(temtime%60);

				if(maxtime<0) negetive='超时';
				if(hours<10) hours='0'+hours;
				if(minutes<10) minutes='0'+minutes;
				if(seconds<10) seconds='0'+seconds;

				var leftTime =negetive+hours+":"+minutes+":"+seconds;
				$(timecontent).html(leftTime);
				$('.use_time').val(totaltime-maxtime);

				if(maxtime==0&&alertPop)
					alertPop("时间到!");
				--maxtime;
				//alert(settings.use_time_content);



				//window.name = maxtime;

	}
	function restart_t(){
			clearInterval(timer);
			maxtime = totaltime;
			timer = setInterval("CountDown()",1000);
			//$('#restart').css("display","none");
			$('#stop').css("display","inline");
			$('#start').css("display","none");
	}
	function stop_t(){
			clearInterval(timer);
			//$('#restart').css("display","none");
			$('#stop').css("display","none");
			$('#start').css("display","inline");
	}
	function start_t(){
			//CountDown();
			timer = setInterval("CountDown()",1000);
			//$('#restart').css("display","none");
			$('#stop').css("display","inline");
			$('#start').css("display","none");
	}
//var timer = new jQueryTimer();
