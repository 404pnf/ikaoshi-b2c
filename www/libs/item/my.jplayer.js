// $Id: jplayer.js,v 1.3 2010/11/04 12:50:18 blazey Exp $

/**
 * @file
 * Drupal behaviors for the jPlayer audio player.
 */
//$(".jplayer").myjplayer();

(function($){
 $.fn.myjplayer=function(){

  var that=this;
  $(that).each(function(index) {
    var wrapper = $(this).parent().get(0);
    var player = this;
    var indexid = index+1;
  	var playerurl = $(this).attr('src');


	var playerhtml='<div id="jquery_jplayer_'+indexid+'" class="jp-jplayer"></div>		<div class="jp-audio">			<div class="jp-type-single">				<div id="jp_interface_'+indexid+'" class="jp-interface">					<ul class="jp-controls">						<li><a id="jquery_jplayer_'+indexid+'-play" href="#" class="jp-play"  title="播放">play</a></li>						<li><a id="jquery_jplayer_'+indexid+'-pause" href="#" class="jp-pause" title="暂停">pause</a></li>					</ul>			</div>			</div>		</div>';

	$(wrapper).append(playerhtml);

	$(player).jPlayer({
		ready: function (event) {
			$(event.target).jPlayer("setMedia", {
				mp3: playerurl
			}).jPlayer("load");
		},
		play: function (event){
				  $(this).jPlayer("pauseOthers"); // pause all players except this one.
		},
		ended: function (event) {
			$(event.target).jPlayer("pause");
		},
		swfPath: "libs/jquery",
		supplied: "mp3",
     	volume: 100,
		cssSelectorAncestor: "#jp_interface_"+indexid
	});

  });

};

})(jQuery);