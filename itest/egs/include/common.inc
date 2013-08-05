<?php
class COMMON
{

		
   static function my_json_encode($arr){
	if (function_exists('json_encode')){
			
		  return json_encode($arr);
		  
		}else{
		 include("JSON.php");
		 $json = new Services_JSON();		 
		 return $json->encode($arr);
		 
	  }	
	  
}	

  static function my_json_decode($arr){
	if (function_exists('json_encode')){
			
		  return json_decode($arr);
		  
		}else{
		 include("JSON.php");
		 $json = new Services_JSON();		 
		 return $json->decode($arr);
		 
	  }	
	  
	  
   }	
}


?>