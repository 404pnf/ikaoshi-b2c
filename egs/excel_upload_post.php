<?php
header("Content-type:text/html;charset=utf-8");
set_time_limit(0);

if($_POST['leadExcel'] == "true")
{
	require_once 'config.php';
	require_once 'excel_import.inc';

	//获取上传的文件名
  $filename = $_FILES['inputExcel']['name'];

  $filetype = $_FILES['inputExcel']["type"];
  $is_xls =  check_filetype($filename,$filetype);

  if($is_xls){
		//上传到服务器上的临时文件名
	  $tmp_name = $_FILES['inputExcel']['tmp_name'];
	  $upload_file = check_file_exists($filename);
	 //echo '1:'.var_export($upload_file,true);

	  $result = move_uploaded_file($tmp_name,$upload_file);


	  if($result){
	  	excel_read_and_insert($upload_file);
	  	//echo '2:'.var_export($result,true);
	  }else{
	  	echo "文件移动出错";
	  }
  }else{
  	echo "不支持该上传文件类型";
  }

}else{
	echo "上传提交出错";
}





?>