var ques_type='<option value="">-请选择-</option>';
var ques_basetype='';

var subques_questype='';
var subques_questags='';

var ques_content_div='';
var add_ques_link_div='';

$(function(){
	
	//试卷题型
	$.getJSON('/itest/egs/index.php?op=item_type',function(data){		
		$.each(data,function(index,term){
			type_id = term['type_id'];
			tname = term['name'];
			ques_type +='<option value="'+ type_id +'">'+ tname+'</option>';
		});
		
	});
	
	//基本题型
	$.getJSON('/itest/egs/index.php?op=basic&type=detail',function(data){
		ques_basetype ='<div class="basetype_subtitle subtitle">选择试题基本题型</div><div class="basetype_list">';	
		$.each(data,function(index,term){	
			termindex = term['tid'];
			if(termindex==1)	ques_basetype += '<div class="basetype_name"><input type="radio" class="basetype" value="'+termindex+'" name="basetype" checked><label class="basetype_label">'+term['name']+'</label></div><div class="basetype_description">'+term['description']+'</div>';
			else			ques_basetype += '<div class="basetype_name"><input type="radio" class="basetype" value="'+termindex+'" name="basetype"><label class="basetype_label">'+term['name']+'</label></div><div class="basetype_description">'+term['description']+'</div>';
		});
		ques_basetype +='</div>';
	});
	
	return false;   
	 
});  



var count=4;//选择题选项添加的count全局控制变量，在新建一个题目是重新设定count=4，open_basetype（）的allrun（）函数中设定。
function add_question(target,resouce){
	
	//var html=$(resouce).clone(true);
	var html = '<tr><td><input  class="answer_correct" type="checkbox" name="answer['+count+'][checkbox]"></td>   <td><textarea id="answer_'+count+'" name="answer['+count+'][answer]" class="answer_answer" type="text" cols="40" rows="5" ></textarea></td>    <td><textarea id="explain_'+count+'" name="answer['+count+'][explain]" type="text" cols="40" rows="5"></textarea></td></tr>';
	$(target).append(html); 	
	
	add_shareCKEditor(count);
}

function add_shareCKEditor(i){
	CKEDITOR.replace( 'answer_'+i,
	{
		sharedSpaces :
		{
			top : 'topSpace',
			bottom : 'bottomSpace'
		},
		removePlugins : 'maximize,resize'
	} );
	
	CKEDITOR.replace( 'explain_'+i,
	{
		sharedSpaces :
		{
			top : 'topSpace',
			bottom : 'bottomSpace'
		},
		removePlugins : 'maximize,resize'
	} );
}

 
 	function BrowseServer( startupPath, functionData )
{
	//alert('whx');
	// You can use the "CKFinder" class to render CKFinder in a page:
	var finder = new CKFinder();
	
	// The path for the installation of CKFinder (default = "/ckfinder/").
	finder.basePath = '/itest/www/libs/ckfinder/';

	//Startup path in a form: "Type:/path/to/directory/"
	finder.startupPath = startupPath;

	// Name of a function which is called when a file is selected in CKFinder.
	finder.selectActionFunction = SetFileField;

	// Additional data to be passed to the selectActionFunction in a second argument.
	// We'll use this feature to pass the Id of a field that will be updated.
	finder.selectActionData = functionData;

	// Launch CKFinder
	finder.popup();
}

// This is a sample function which is called when a file is selected in CKFinder.
function SetFileField( fileUrl, data )
{
	document.getElementById( data["selectActionData"] ).value = fileUrl;
	
	var sFileName = this.getSelectedFile().name;
	var sFileDate = this.getSelectedFile().date;
	var sFileType = data["fileType"];
	document.getElementById( 'thumbnails' ).innerHTML +=
			'<div class="thumb">' +
				'<img src="' + fileUrl + '" />' +
				'<div class="caption">' +
					'<a href="' + data["fileUrl"] + '" target="_blank">' + sFileName + '</a> (' + data["fileSize"] + 'KB、'+sFileDate+'、'+sFileType+')' +
				'</div>' +
			'</div>';

	document.getElementById( 'preview' ).style.display = "block";
}
	

function is_multichoice_f(){

	$('#globalmessage').empty();
	var j=0;
	$('input.answer_correct').each(function(){				
		if($(this).attr("checked")) j++;
		
	});
	
	if(j<=0) {
		$('#globalmessage').html("请至少选择一个正确答案");
		return false;
	}
	else{
		var that=$('#ques_is_multichoice');
		if(!that.attr("checked")&&j>1){
			$('#globalmessage').html('您没有勾选“是多选题”，但是选择了两个正确答案，请做修正');
			return false;
		}
		else if(that.attr("checked")&&j<=1){
			$('#globalmessage').html('您勾选了“是多选题”，但是选择的正确答案少于两个，请做修正');
			return false;
		}
		
	}
	subques_questype=$('#ques_type_form').val();
	subques_questags=$('.ques_tags_input').val();
	return true;
}

function subques_inherit(){
	subques_questype=$('#ques_type_form').val();
	subques_questags=$('.ques_tags_input').val();	
	return true;
}

var basetype4='<div class="add_ques_subtitle subtitle">创建试题材料<span class="parent_ques_title"></span></div><form id="add_ques_form" action="/itest/egs/add_item_post.php" method="POST" >	<div class="post_id"><input id="user_id" type="hidden" name="uid" value="'+user_id+'" /><input id="basetypeid" type="hidden" name="id" value="" /><input type="hidden" name="op" value="material" /></div><div class="is_subques"> <input type="hidden" name="pid" value="" id="pid"/><div class="only_pid"><input id="only_pid" name="only_pid" type="checkbox" checked><label for="only_pid">唯一依赖于父题目</label> <span class="note">默认为不唯一依赖，即可以在父级题目不被调用的情况下，调用此题目。</span></div></div><div class="ques_name_score clearfix">	<div class="ques_name fleft">        <label for="ques_name">标题：</label>  <input id="ques_name" name="name" type="text" />    <div class="note"></div>	</div>	<div id="ques_type" class="ques_type">    <label for="ques_type_form">所属题型：</label>    <select  id="ques_type_form" name="type" class="required" ></select>	</div>  </div><div class="ques_name_score clearfix">	<div class="ques_time fleft">   <label for="ques_time">时间：</label>   <input id="ques_time" name="time" type="text" class="time" size="16" value="0"/>   <span class="unit">分钟</span>    </div>	<div class="ques_score">   <label class="ques_score_label">分值：</label>  <input name="point" type="text" maxlength="5" class="required" size="3" value="1"/>    <span class="unit">分</span>    </div></div>	<div class="ques_body">  <label for="ques_body">阅读材料：</label>  <textarea id="ques_body" name="body" type="text" cols="80" rows="10" ></textarea><div class="note"></div></div>	<div class="ques_explain"><label for="ques_explain">原文或翻译：</label><textarea id="ques_explain" name="answer[explain]" type="text" cols="80" class="ckeditor_basic"></textarea></div>	<div class="subques_random">        <input id="subques_random" name="subques_random" type="checkbox" >		<label for="subques_random">子题目随机排列</label>    <span class="note">子题目的默认显示顺序为编号顺序</span>	</div>	<div class="ques_files"><label for="ques_has_files">附件</label><input name="filepath" type="text" class="files"/></div>		<div class="ques_tags">	<label class="ques_tags_label">标签：</label><input name="tags" type="text"  class="ques_tags_input required" size="80"/></div>		<div class="ques_attributes"><label class="ques_attributes_label">考点：</label>  <input name="attributes" type="text"  class="ques_attributes_input" size="80"/></div><input type="submit" value="保存" name="btn_submit" class="submit" onclick="return subques_inherit();"><input type="submit" value="保存并创建其子题目" name="btn_submit_subques" class="submit" onclick="return subques_inherit();"><input type="reset"  value="重设" class="submit"></form> ';

var basetype3='<div class="add_ques_subtitle subtitle">创建试题说明<span class="parent_ques_title"></span></div><form id="add_ques_form" action="/itest/egs/add_item_post.php" method="POST" >	<div class="post_id"><input id="user_id" type="hidden" name="uid" value="'+user_id+'" /><input id="basetypeid" type="hidden" name="id" value="" /><input type="hidden" name="op" value="description" /></div><div class="is_subques"><input type="hidden" name="pid" value="" id="pid"/> <div class="only_pid"><input id="only_pid" name="only_pid" type="checkbox" checked><label for="only_pid">唯一依赖于父题目</label> <span class="note">默认为不唯一依赖，即可以在父级题目不被调用的情况下，调用此题目。</span></div></div><div class="ques_name_score clearfix">	<div class="ques_name fleft">        <label for="ques_name">标题：</label>        <input id="ques_name" name="name" type="text" />        <div class="note"></div>	</div>	<div id="ques_type" class="ques_type">        <label for="ques_type_form">所属题型：</label>        <select  id="ques_type_form" name="type" class="required" ></select>	</div>    </div><div class="ques_name_score clearfix">	<div class="ques_time fleft">        <label for="ques_time">时间：</label>        <input id="ques_time" name="time" type="text" class="time" size="16" value="0"/>        <span class="unit">分钟</span>    </div>	<div class="ques_score">        <label class="ques_score_label">分值：</label>        <input name="point" type="text" maxlength="5" class="required" size="3" value="1"/>        <span class="unit">分</span>    </div></div>	<div class="ques_body">        <label for="ques_body">Direction：</label>        <textarea id="ques_body" name="body" type="text" cols="80" rows="10"></textarea>        <div class="note"></div>	</div>		<div class="subques_random">        <input id="subques_random" name="subques_random" type="checkbox" >		<label for="subques_random">子题目随机排列</label>        <span class="note">子题目的默认显示顺序为编号顺序</span>	</div>	<div class="ques_files"><label for="ques_has_files">附件</label><input name="files" type="file" class="files"/></div>	<div class="ques_tags">		<label class="ques_tags_label">标签：</label>        <input name="tags" type="text"  class="ques_tags_input required" size="80"/>	</div><div class="ques_attributes"><label class="ques_attributes_label">考点：</label>  <input name="attributes" type="text"  class="ques_attributes_input" size="80"/>	</div><input type="submit" value="保存" name="btn_submit" class="submit" onclick="return subques_inherit();"><input type="submit" value="保存并创建其子题目" name="btn_submit_subques" class="submit" onclick="return subques_inherit();"><input type="reset"  value="重设" class="submit"></form> ';

var basetype2='<div class="add_ques_subtitle subtitle">创建填空题<span class="parent_ques_title"></span></div><form id="add_ques_form" action="/itest/egs/add_item_post.php" method="POST" >	<div class="post_id"><input id="user_id" type="hidden" name="uid" value="'+user_id+'" /><input id="basetypeid" type="hidden" name="id" value="" /><input type="hidden" name="op" value="filling" /></div><div class="is_subques"> <input type="hidden" name="pid" value="" id="pid"/><div class="only_pid"><input id="only_pid" name="only_pid" type="checkbox" checked><label for="only_pid">唯一依赖于父题目</label> <span class="note">默认为不唯一依赖，即可以在父级题目不被调用的情况下，调用此题目。</span></div></div><div class="ques_name_score clearfix"><div class="ques_name fleft">   <label for="ques_name">名称：</label>   <input id="ques_name" name="name" type="text" />   <div class="note">可不填，默认为题目id+部分题干</div></div><div id="ques_type" class="ques_type">   <label for="ques_type_form">所属题型：</label>   <select  id="ques_type_form" name="type" class="required" ></select></div>    </div><div class="ques_name_score clearfix"><div class="ques_time fleft">   <label for="ques_time">时间：</label>   <input id="ques_time" name="time" type="text" class="time" size="16" value="0"/>   <span class="unit">分钟</span>    </div><div class="ques_score">   <label class="ques_score_label">分值：</label>   <input name="point" type="text" maxlength="5" class="required" size="3" value="1"/>   <span class="unit">分</span>    </div></div><div class="ques_body">   <label for="ques_body">题干：</label>   <textarea id="ques_body" name="body" type="text" cols="80" rows="10"></textarea>   <div class="note"></div><div class="ques_is_objective">   <input id="ques_is_objective" name="is_objective" type="checkbox" >	<label for="ques_is_objective">是客观填空题</label>   <span class="note">默认为主观填空题</span></div>	<div class="ques_answer_blank">  <label for="ques_answer_blank">blank大小：</label>    <select id="ques_answer_blank" name="answer_blank"><option value="0">-请选择-</option><option value="1">单个单词</option><option value="2">一句话</option><option value="3">一段文字</option></select>	<span class="note">默认为2至3个单词大小</span></div>	<div class="ques_answer"><div class="answer_correct">	<label for="answer_correct" class="fleft">正确答案：</label>   <textarea id="answer_answer" name="answer[answer]" type="text" cols="80" rows="5"></textarea>   </div>      <div class="answer_analyse">   <label for="answer_analyse">答案解析：</label>   <textarea id="answer_explain" name="answer[explain]" class="ckeditor_basic" type="text" cols="80" rows="5"></textarea>   </div></div>     	<div class="ques_files"><label for="ques_has_files">附件</label><input name="filepath" type="text" class="files"/></div>	<div class="ques_tags">	<label class="ques_tags_label">标签：</label>   <input name="tags" type="text"  class="ques_tags_input required" size="80"/></div><div class="ques_attributes"><label class="ques_attributes_label">考点：</label>  <input name="attributes" type="text"  class="ques_attributes_input required" size="80"/>	</div><input type="submit" value="保存" name="btn_submit" class="submit" onclick="return subques_inherit();"><input type="reset"  value="重设" class="submit"></form> ';

var basetype1='<div class="add_ques_subtitle subtitle">创建选择题<span class="parent_ques_title"></span></div><form id="add_ques_form" action="/itest/egs/add_item_post.php" method="POST" >	<div class="post_id"><input id="user_id" type="hidden" name="uid" value="'+user_id+'" /><input id="basetypeid" type="hidden" name="id" value="" /><input type="hidden" name="op" value="multichoice" /></div><div class="is_subques"> <input type="hidden" name="pid" value="" id="pid"/><div class="only_pid"><input id="only_pid" name="only_pid" type="checkbox" checked><label for="only_pid">唯一依赖于父题目</label> <span class="note">默认为不唯一依赖，即可以在父级题目不被调用的情况下，调用此题目。</span></div></div><div class="ques_name_score clearfix"> <div class="ques_name fleft">   <label for="ques_name">名称：</label>   <input id="ques_name" name="name" type="text" />   <div class="note">可不填，默认为题目id+部分题干</div> </div> <div id="ques_type" class="ques_type">   <label for="ques_type_form">所属题型：</label>   <select  id="ques_type_form" name="type" class="required" ></select> </div>    </div><div class="ques_name_score clearfix"> <div class="ques_time fleft">   <label for="ques_time">时间：</label>   <input id="ques_time" name="time" type="text" class="time" size="16" value="0"/>   <span class="unit">分钟</span>    </div> <div class="ques_score">   <label class="ques_score_label">分值：</label>   <input name="point" type="text" maxlength="5" class="required" size="3" value="1"/>   <span class="unit">分</span>    </div></div><div class="ques_body">   <label for="ques_body">题干：</label><textarea id="ques_body" name="body" type="text" cols="80" rows="10"></textarea> <div class="note"></div><div class="ques_is_multichoice">   <input id="ques_is_multichoice" name="is_multichoice" type="checkbox"  >  <label for="ques_is_multichoice">是多选题</label>   <span class="note">默认为单选</span> </div>  <div class="answer_is_random">   <input id="answer_is_random" name="answer_is_random" type="checkbox" checked>  <label for="answer_is_random">答案顺序随机</label>   <span class="note">默认为按顺序显示</span> </div>    <div class="ques_answer">   <table id="ques_answer">  <caption >答案</caption>  <thead><th class="answer_correct">正确</th><th class="answer_option">选项</th><th class="answer_analyse">答案解析</th></thead> <tr><td id="topSpace" colspan="3"></td></tr> <tbody class="answer_body">   <tr><td><input  class="answer_correct" type="checkbox" name="answer[0][checkbox]"></td>   <td><textarea id="answer_0" name="answer[0][answer]" class="answer_answer" type="text" cols="40" rows="5" ></textarea></td>    <td><textarea id="explain_0" name="answer[0][explain]" type="text" cols="40" rows="5"></textarea></td></tr>   <tr><td><input class="answer_correct" type="checkbox" name="answer[1][checkbox]"></td>   <td><textarea id="answer_1"  name="answer[1][answer]" class="answer_answer" type="text" cols="40" rows="5"  ></textarea></td>   <td><textarea id="explain_1" name="answer[1][explain]" type="text" cols="40" rows="5"></textarea></td>  </tr>   <tr><td><input class="answer_correct" type="checkbox" name="answer[2][checkbox]"></td>   <td><textarea id="answer_2" name="answer[2][answer]"  class="answer_answer" type="text" cols="40" rows="5"></textarea></td>      <td><textarea id="explain_2" name="answer[2][explain]" type="text" cols="40" rows="5"></textarea></td>  </tr> <tr><td><input  class="answer_correct" type="checkbox" name="answer[3][checkbox]"></td>   <td><textarea id="answer_3" name="answer[3][answer]"  class="answer_answer" type="text" cols="40" rows="5"></textarea></td>      <td><textarea id="explain_3" name="answer[3][explain]" type="text" cols="40" rows="5"></textarea></td>  </tr> </tbody><tr><td id="bottomSpace" colspan="3"></td></tr></table>  <a href="javascript:void(0);" onclick=\'add_question("#ques_answer tbody.answer_body","#ques_answer tbody.answer_body >  tr:nth-child(1)")\' >增加选项</a> </div>		<!--	<div class="ques_files"><label>附件</label <input id="ques_image" name="imagePath" type="text" size="60" /><input type="button" value="上传图片" onclick=\'BrowseServer( "Images:/", "imagePath" );\' /> <input id="ques_audio" name="audioPath" type="text"/><input type="button" value="上传音频" onclick=\'BrowseServer( "Audio:/", "audioPath" );\' /></div> --> <div class="ques_files"><label for="ques_has_files">附件</label><input name="filepath" type="text" class="files"/></div>  <div class="ques_tags">  <label class="ques_tags_label">标签：</label>   <input name="tags" type="text"  class="ques_tags_input required" size="80"/> </div><div class="ques_attributes"><label class="ques_attributes_label">考点：</label>  <input name="attributes" type="text"  class="ques_attributes_input required" size="80"/>	</div><input type="submit" value="保存" name="btn_submit" class="submit" onclick="return is_multichoice_f();"><input type="reset"  value="重设" class="submit"></form>';

function open_basetype(pid,options){
	
	var settings=$.extend({
			ques_content_div : '',
			add_ques_link_div: ''
	},options||{});
	
	if(settings.ques_content_div!='')	ques_content_div=settings.ques_content_div;
	if(settings.add_ques_link_div!='')	add_ques_link_div=settings.add_ques_link_div;
	
	var title='';
	if(pid==-1) title='<div class="title">创建无父级题目的内容</div>'; 
	else  {
		parent_title(pid);
		title = '<div class="parent_ques_title title"></div>';
	}
	
	$.prompt(title+ques_basetype,
		{buttons: { 确定: true, 取消: false},
			focus: 0,
			submit: function(v, m, f){
				if(v){
					var e=f.basetype;
					
					if(e==4){
						$(ques_content_div).html(basetype4);							 
					}
					if(e==3){
						$(ques_content_div).html(basetype3);	
					}
					if(e==2){							
						$(ques_content_div).html(basetype2);
					}
					if(e==1){
						$(ques_content_div).html(basetype1);
					}	
					if(e==1 || e==2 || e==3 || e==4){
						allrun(e,pid);	
					}				
				}	
				$.prompt.close();
				return false;
			}
	});
}

function parent_title(pid){
	$.getJSON('/itest/egs/index.php?op=item_info&id='+pid,function(data){
		quuesname=data[pid];
		$('.parent_ques_title').append('创建<span class="pques">'+quuesname+'</span>的子题目');
	});
}
function allrun(basetypeid,pid){
	
	count=4;
	$(add_ques_link_div).empty();//清空添加题目的链接
	//$('#basetype_button').attr({style:"display:none"});	
	//add_ques_form 中需要ajax显示的内容
	$('#basetypeid').val(basetypeid);
	
	if(pid!=-1) parent_title(pid);
	$('#pid').val(pid);
	if(pid==-1){
		 $('.only_pid').attr({style:"display:none"});
		 $('#only_pid').attr("checked", false);
	}
	$('#ques_type_form').empty();
	$('#ques_type_form').append(ques_type);	
	$('#ques_type_form').val(subques_questype);
	$('.ques_tags_input').val(subques_questags);
	
	if ( typeof CKEDITOR == 'undefined' )
	{
		alert('<strong><span style="color: #ff0000">Error</span>: CKEditor not found</strong>.This sample assumes that CKEditor (not included with CKFinder) is installed in the "/ckeditor/" path. If you have it installed in a different place, just edit this file, changing the wrong paths in the &lt;head&gt; (line 5) and the "BasePath" value (line 32).' );
	}
	else
	{
		var editor = CKEDITOR.replace( 'ques_body');
		editor.config.toolbar = "Full";
		CKFinder.setupCKEditor( editor, '/itest/www/libs/ckfinder/');
		//editor.config.width = 880;
		editor.config.height = 200;
		//alert('abc'+editor);
		
		$('.answer_answer').each(function(i){
			add_shareCKEditor(i);	
		});	
				
		$('.ckeditor_basic').each(function(){
			var editor_basic = CKEDITOR.replace(this);
			//editor_basic.config.width = 800;
		});
		
		CKEDITOR.config.toolbar = [ [ 'Source', '-', 'Bold', 'Italic', 'Underline', 'Strike','-','Link', '-', 'Image' ] ];
		CKFinder.setupCKEditor( CKEDITOR, '/itest/www/libs/ckfinder/');
		//CKEDITOR.config.width = 400;
		CKEDITOR.config.height = 100;		
		
	}
	
	//add_ques_form 的验证
	$('#add_ques_form').validate();
	$('.required').prev().append('<span style="color:red;">*</span>');	
	
	//add_ques_form 的提交
	var add_ques = {
		url:		'/itest/egs/add_item_post.php',
		type:		'post',
		target:   	'#bottom-message',// target element to update
		dataType: 	'json',
		//beforeSubmit:  showRequest,  // pre-submit callback
		success:     open_by_status //提交后进行返回状态的判断，并确定是否创建子题目
	};
	$('#add_ques_form').ajaxForm(add_ques);
	
	return true;
}


function open_by_status(data){
	var status=data.status;
	var id=data.id;
	var pid=data.pid;
	
	if(status==1||status==2) {
		ques_display(id,pid,status);
	}
	else if(status==0)  $("#globalmessage").html('保存失败，名称重复或缺少必要信息');
	else
		$("#globalmessage").html('服务器交互失败，请重试');

}



function ques_display(id,pid,status){
	
	$('#globalmessage').empty();
	$('#bottom-message').empty();
	$(add_ques_link_div).empty();
	
	$(ques_content_div).view_ques(id,{add_ques : false});//显示新建的题目内容
			
			
	if(status==1){
		$(add_ques_link_div).append('<div id="basetype_button" style="display:block;">	<a href="javascript:void(0);" onclick="open_basetype('+id+')">请选择子题目试题类型</a><span class="error"></span></div>');
	}
	else if(status==2)
	{
		 if(pid!=-1){
			$(add_ques_link_div).append('<div id="basetype_button" style="display:block;">	<a href="javascript:void(0);" onclick="open_basetype(-1)">新建无父级题目的内容</a><span class="error"></span></div>');
			$(add_ques_link_div).append('<div id="basetype_button" style="display:block;">	<a href="javascript:void(0);" onclick="open_basetype('+pid+')">继续创建同级内容</a><span class="error"></span></div>');
		}
		else
			$(add_ques_link_div).append('<div id="basetype_button" style="display:block;">	<a href="javascript:void(0);" onclick="open_basetype(-1)">新建无父级题目的内容</a><span class="error"></span></div>');
	}

}