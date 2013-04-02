<?php

include ("libs/xmlrpc/xmlrpc.inc");
include ("libs/xmlrpc/xmlrpc_wrappers.inc");
require "txgenerictools.php";

//header('content-type:text/html; charset=utf-8');

class resultObject
{
    public $message = '参数错误，请检查';

    function __construct()
    {
    }

    public function getText()
    {
        return $this->message;
    }
}

$resultV = new resultObject();
$reqV = null;
if (isset($_POST['req']))
{
    $reqV = $_POST['req'];
}
else
{
    if (isset($_GET['req']))
    {
        $reqV = $_GET['req'];
    }
    else
    {
        print $resultV->getText();
        exit(0);
    }
}

$formatV = null;
if (isset($_POST['format']))
{
    $formatV = $_POST['format'];
}
else
{
    if (isset($_GET['format']))
    {
        $formatV = $_GET['format'];
    }
    else
    {
        $formatV = 'text';
    }
}

$modeV = null;
if (isset($_POST['mode']))
{
    $modeV = $_POST['mode'];
}
else
{
    if (isset($_GET['mode']))
    {
        $modeV = $_GET['mode'];
    }
    else
    {
        $modeV = 'default';
    }
}

$valueV = null;
if (isset($_POST['value']))
{
    $valueV = $_POST['value'];
}
else
{
    if (isset($_GET['value']))
    {
        $valueV = $_GET['value'];
    }
    else
    {
        $valueV = '';
    }
}

if ($reqV == 'showip')
{
    switch ($modeV)
    {
        case '2':
            {

            }
            break;
        default:
            {
                $resultV->message = $_SERVER["REMOTE_ADDR"];
            }
    }

    switch ($formatV)
    {
        default:
            {
                print $resultV->getText();
            }
    }

}
else
    if ($reqV == 'showipdetail')
    {
        $tmpIP = null;
        switch ($modeV)
        {
            case '2':
                {

                }
                break;
            default:
                {
                    $tmpIP = $_SERVER["REMOTE_ADDR"];
                }
        }

        $url = 'http://www.youdao.com/smartresult-xml/search.s?type=ip&q=' . $tmpIP;
        $contents = file_get_contents($url);
        $resultV->message = htmlspecialchars(iconv('gbk', 'utf-8', $contents));

        switch ($formatV)
        {
            default:
                {
                    print $resultV->getText();
                }
        }
    }
    else
        if ($reqV == 'showipdetailother')
        {
            $tmpIP = null;
            switch ($modeV)
            {
                case '2':
                    {

                    }
                    break;
                default:
                    {
                        $tmpIP = $valueV;
                    }
            }

            $url = 'http://www.youdao.com/smartresult-xml/search.s?type=ip&q=' . $tmpIP;
            $contents = file_get_contents($url);
            $xml = new SimpleXMLElement($contents);
            try
            {
                // echo $xml->product->location;
                $resultV->message = $xml->product->ip . ': ' . $xml->product->location;
            }
            catch (exception $eee)
            {
                $resultV->message = '查询时发生错误';
            }
            switch ($formatV)
            {
                default:
                    {
                        print $resultV->getText();
                    }
            }
        }
        else
            if ($reqV == 'showiddetail')
            {

                $url = 'http://www.youdao.com/smartresult-xml/search.s?type=id&q=' . $valueV;
                $contents = file_get_contents($url);
                $xml = new SimpleXMLElement($contents);
                try
                {
                    // echo $xml->product->location;
                    $resultV->message = $xml->product->code . ': ' . $xml->product->location .
                        ', 生日: ' . $xml->product->birthday . ', 性别: ' . (($xml->product->gender == 'm') ?
                        '男性' : '女性');
                }
                catch (exception $eee)
                {
                    $resultV->message = '查询时发生错误';
                }

                switch ($formatV)
                {
                    default:
                        {
                            print $resultV->getText();
                        }
                }
            }
            else
                if ($reqV == 'showmobiledetail')
                {

                    $url = 'http://www.youdao.com/smartresult-xml/search.s?type=mobile&q=' . $valueV;
                    $contents = file_get_contents($url);
                    $xml = new SimpleXMLElement($contents);
                    try
                    {
                        // echo $xml->product->location;
                        $resultV->message = $xml->product->phonenum . ': ' . $xml->product->location;
                    }
                    catch (exception $eee)
                    {
                        $resultV->message = '查询时发生错误';
                    }

                    switch ($formatV)
                    {
                        default:
                            {
                                print $resultV->getText();
                            }
                    }
                }
                else
                    if ($reqV == 'getxanaduvo')
                    {

                        header('Pragma: public');
                        header('Last-Modified: ' . gmdate('D, d M Y H:i:s') . ' GMT');
                        header('Cache-Control: no-store, no-cache, must-revalidate');
                        header('Cache-Control: pre-check=0, post-check=0, max-age=0');
                        header('Content-Transfer-Encoding: binary');
                        header('Content-Encoding: none');
                        header('Content-type: application/force-download');
                        header('Content-Disposition: attachment; filename="xanaduvosetup.exe"');

                        try
                        {
                            $filesize = filesize('./xanaduvosetup.exe'); //获得文件大小
                            header('Content-length: ' . $filesize);

                            readfile('./xanaduvosetup.exe');
                        }
                        catch (exception $e)
                        {
                            die(json_encode($e->getMessage()));

                        }
                    }
                    else
                        if ($reqV == 'redirect')
                        {

                            $urlV = null;
                            if (isset($_POST['url']))
                            {
                                $urlV = $_POST['url'];
                            }
                            else
                            {
                                if (isset($_GET['url']))
                                {
                                    $urlV = $_GET['url'];
                                }
                                else
                                {
                                    $urlV = 'http://2u4u.com.cn';
                                }
                            }
                            header('Location: ' . $urlV);
                        }
                        else
                            if ($reqV == 'getbaiduennews')
                            {

                                print <<< END
<style type=text/css> div{font-size:12px;font-family:arial}.baidu{font-size:14px;line-height:24px;font-family:arial} a,a:link{color:#0000cc;}
.baidu span{color:#6f6f6f;font-size:12px} a.more{color:#008000;}a.blk{color:#000;font-weight:bold;}</style>
<script language="JavaScript" type="text/JavaScript" src="http://news.baidu.com/ns?word=title%3A%D3%A2%D3%EF&tn=newsfcu&from=news&cl=2&rn=50&ct=0"></script>
END;

                            }
                            else
                                if ($reqV == 'getbaiduennewsnonewwindow')
                                {

                                    $pageStrV = file_get_contents('http://2u4u.com.cn/xieq/xanadu/xanaduapi.php?req=getbaiduennews');
                                    //  file_put_contents('./logX.txt', $pageStrV);
                                    $pageStrV = str_replace(' target=\"_blank\"', ' target=\"_self\"', $pageStrV);
                                    die($pageStrV);

                                }
                                else
                                    if ($reqV == 'connectdrupal')
                                    {
                                        $c = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                        $c->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                        $r = &$c->send(new xmlrpcmsg('system.connect'));

                                        try
                                        {
                                            $c2 = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                            $c2->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                            $timestamp = (string )time();

                                            $nonce = TX::getDrupalUniqueCode("10");

                                            // prepare a hash
                                            $hash_parameters = array($timestamp, '2u4u.com.cn', $nonce, 'system.connect');
                                            $hash = hash_hmac("sha256", implode(';', $hash_parameters), 'your_api_key');

                                            $sessid = $r->val['sessid'];
                                            $username = $_REQUEST['uname'];
                                            $pass = $_REQUEST['upass'];
                                            $domain = '.2u4u.com.cn';

                                            $r2 = &$c2->send(new xmlrpcmsg('user.login', array(new xmlrpcval($username), new
                                                xmlrpcval($pass))));
                                            if ($r2->faultCode())
                                            {
                                                die($_REQUEST['callback'] . '("' . $username . $pass . $r2->faultString() .
                                                    '");');
                                            }
                                            $outputObject['sessid'] = $r2->val['sessid'];
                                            $outputObject['userid'] = $r2->val['user']['uid'];
                                            $outputObject['username'] = $r2->val['user']['name'];
                                            if (!empty($_REQUEST['callback']))
                                            {
                                                die($_REQUEST['callback'] . '(' . json_encode($outputObject) . ');');
                                            }
                                            else
                                            {
                                                print (json_encode($outputObject));
                                            }

                                        }
                                        catch (exception $eee)
                                        {
                                            var_dump($eee->message);
                                        }

                                        if ($r->faultCode())
                                        {
                                            echo "<p>Server error： '" . htmlspecialchars($r->faultString());
                                        }

                                        //die($session);

                                    }
                                    else
                                        if ($reqV == 'connectdrupalindirectly')
                                        {
                                            $c = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                            //$c->setDebug(2);
                                            $c->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                            $r = &$c->send(new xmlrpcmsg('system.connect'));

                                            try
                                            {
                                                $c2 = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                                //$c2->setDebug(2);
                                                $c2->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                                $timestamp = (string )time();

                                                $nonce = TX::getDrupalUniqueCode("10");

                                                // prepare a hash
                                                $hash_parameters = array($timestamp, '2u4u.com.cn', $nonce, 'system.connect');
                                                $hash = hash_hmac("sha256", implode(';', $hash_parameters), 'your_api_key');

                                                $sessid = $r->val['sessid'];
                                                $username = $_REQUEST['uname'];
                                                $pass = $_REQUEST['upass'];
                                                $domain = '.2u4u.com.cn';

                                                $r2 = &$c2->send(new xmlrpcmsg('user.login', array(new xmlrpcval($username), new
                                                    xmlrpcval($pass))));
                                                if ($r2->faultCode())
                                                {
                                                    $outputObject['status'] = 'Error';
                                                    $outputObject['errorMessage'] = $r2->faultString();
                                                    if (!empty($_REQUEST['callback']))
                                                    {

                                                        die($_REQUEST['callback'] . '("' . json_encode($outputObject) . '");');
                                                    }
                                                    else
                                                    {
                                                        die(json_encode($outputObject));
                                                    }
                                                }
                                                $outputObject['sessid'] = $r2->val['sessid'];
                                                $outputObject['userid'] = $r2->val['user']['uid'];
						$outputObject['username'] = $r2->val['user']['name'];
						$outputObject['email'] = $r2->val['user']['init'];
						$outputObject['roles'] = $r2->val['user']['roles'];
                                                $outputObject['status'] = 'Success';
                                                $outputObject['errorMessage'] = '';
                                                if (!empty($_REQUEST['callback']))
                                                {
                                                    die($_REQUEST['callback'] . '(' . json_encode($outputObject) . ');');
                                                }
                                                else
                                                {
                                                    print (json_encode($outputObject));
                                                }

                                            }
                                            catch (exception $eee)
                                            {
                                                var_dump($eee->message);
                                            }

                                            if ($r->faultCode())
                                            {
                                                echo "<p>Server error： '" . htmlspecialchars($r->faultString());
                                            }

                                            //die($session);

                                        }
                                    else
                                        if ($reqV == 'connect2u4uqh')
					{
						$appkeyV = $_REQUEST['appkey'];
						if ($appkeyV != '8881')
						{
							$outputObject['status'] = 'Error';
                                                    	$outputObject['errorMessage'] = 'App key错误';
                                                    if (!empty($_REQUEST['callback']))
                                                    {

                                                        die($_REQUEST['callback'] . '("' . json_encode($outputObject) . '");');
                                                    }
                                                    else
                                                    {
                                                        die(json_encode($outputObject));
                                                    }
						}

                                            $c = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                            //$c->setDebug(2);
                                            $c->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                            $r = &$c->send(new xmlrpcmsg('system.connect'));

                                            try
                                            {
                                                $c2 = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                                //$c2->setDebug(2);
                                                $c2->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                                $timestamp = (string )time();

                                                $nonce = TX::getDrupalUniqueCode("10");

                                                // prepare a hash
                                                $hash_parameters = array($timestamp, '2u4u.com.cn', $nonce, 'system.connect');
                                                $hash = hash_hmac("sha256", implode(';', $hash_parameters), 'your_api_key');

                                                $sessid = $r->val['sessid'];
                                                $username = $_REQUEST['uname'];
                                                $pass = $_REQUEST['upass'];
                                                $domain = '.2u4u.com.cn';

                                                $r2 = &$c2->send(new xmlrpcmsg('user.login', array(new xmlrpcval($username), new
                                                    xmlrpcval($pass))));
                                                if ($r2->faultCode())
                                                {
                                                    $outputObject['status'] = 'Error';
                                                    $outputObject['errorMessage'] = $r2->faultString();
                                                    if (!empty($_REQUEST['callback']))
                                                    {

                                                        die($_REQUEST['callback'] . '("' . json_encode($outputObject) . '");');
                                                    }
                                                    else
                                                    {
                                                        die(json_encode($outputObject));
                                                    }
                                                }
						
						header('content-type:text/html; charset=utf-8');

                                                $outputObject['sessid'] = $r2->val['sessid'];
                                                $outputObject['userid'] = $r2->val['user']['uid'];
						$outputObject['username'] = $r2->val['user']['name'];
						$outputObject['email'] = $r2->val['user']['init'];
						$outputObject['roles'] = $r2->val['user']['roles'];
                                                $outputObject['status'] = 'Success';
                                                $outputObject['errorMessage'] = '';
                                                if (!empty($_REQUEST['callback']))
                                                {
                                                    die($_REQUEST['callback'] . '(' . json_encode($outputObject) . ');');
                                                }
                                                else
                                                {
                                                    print (json_encode($outputObject));
                                                }

                                            }
                                            catch (exception $eee)
                                            {
                                                var_dump($eee->message);
                                            }

                                            if ($r->faultCode())
                                            {
                                                echo "<p>Server error： '" . htmlspecialchars($r->faultString());
                                            }

                                            //die($session);

                                        }
                                    else
                                        if ($reqV == 'connect2u4uint')
					{
						$appkeyV = $_REQUEST['appkey'];
						if ($appkeyV != '8881')
						{
							$outputObject['status'] = 'Error';
                                                    	$outputObject['errorMessage'] = 'App key错误';
                                                    if (!empty($_REQUEST['callback']))
                                                    {

                                                        die($_REQUEST['callback'] . '("' . json_encode($outputObject) . '");');
                                                    }
                                                    else
                                                    {
                                                        die(json_encode($outputObject));
                                                    }
						}

                                            $c = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                            //$c->setDebug(2);
                                            $c->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                            $r = &$c->send(new xmlrpcmsg('system.connect'));

                                            try
                                            {
                                                $c2 = new xmlrpc_client("/services/xmlrpc", "2u4u.com.cn", 80);
                                                //$c2->setDebug(2);
                                                $c2->return_type = 'phpvals'; // let client give us back php values instead of xmlrpcvals
                                                $timestamp = (string )time();

                                                $nonce = TX::getDrupalUniqueCode("10");

                                                // prepare a hash
                                                $hash_parameters = array($timestamp, '2u4u.com.cn', $nonce, 'system.connect');
                                                $hash = hash_hmac("sha256", implode(';', $hash_parameters), 'your_api_key');

                                                $sessid = $r->val['sessid'];
                                                $username = $_REQUEST['uname'];
                                                $pass = $_REQUEST['upass'];
                                                $domain = '.2u4u.com.cn';

                                                $r2 = &$c2->send(new xmlrpcmsg('user.login', array(new xmlrpcval($username), new
							xmlrpcval($pass))));
						var_dump($r2);
                                                if ($r2->faultCode())
                                                {
                                                    $outputObject['status'] = 'Error';
                                                    $outputObject['errorMessage'] = $r2->faultString();
                                                    if (!empty($_REQUEST['callback']))
                                                    {

                                                        die($_REQUEST['callback'] . '("' . json_encode($outputObject) . '");');
                                                    }
                                                    else
                                                    {
                                                        die(json_encode($outputObject));
                                                    }
						}
					//	var_dump($r2->val['user']['data']['roles']['19']);
                                                $outputObject['sessid'] = $r2->val['sessid'];
                                                $outputObject['userid'] = $r2->val['user']['uid'];
						$outputObject['username'] = $r2->val['user']['name'];
						$outputObject['email'] = $r2->val['user']['mail'];
						$outputObject['roles'] = $r2->val['user']['roles'];
                                                $outputObject['status'] = 'Success';
                                                $outputObject['errorMessage'] = '';
                                                if (!empty($_REQUEST['callback']))
                                                {
                                                    die($_REQUEST['callback'] . '(' . json_encode($outputObject) . ');');
                                                }
                                                else
                                                {
                                                    print (json_encode($outputObject));
                                                }

                                            }
                                            catch (exception $eee)
                                            {
                                                var_dump($eee->message);
                                            }

                                            if ($r->faultCode())
                                            {
                                                echo "<p>Server error： '" . htmlspecialchars($r->faultString());
                                            }

                                            //die($session);

                                        }
                                        else
                                            if ($reqV == 'printValue')
                                            {
                                                die($valueV);
                                            }
                                            else
                                            {

                                            }

?>


