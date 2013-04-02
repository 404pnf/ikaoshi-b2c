<?php
require 'config.php';

$api = new API();
$arr = $api->query_exam_type('','','');
echo COMMON::my_json_encode($arr);

?>
