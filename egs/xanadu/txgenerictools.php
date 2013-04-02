<?php

class TX
{
    public function getResultText($text, $type = "info")
    {
        $result = null;

        if ($type == "error")
        {
            $result = "<font style='font-size: 11px' color='red' face='Arial'>" . $text .
                "</font>";
        }
        else
            if ($type == "success")
            {
                $result = "<font style='font-size: 11px' color='green' face='Arial'>" . $text .
                    "</font>";
            }
            else
                if ($type == "info")
                {
                    $result = "<font style='font-size: 11px' color='black' face='Arial'>" . $text .
                        "</font>";
                }
                else
                {
                    $result = "<font style='font-size: 11px' color='gray' face='Arial'>" . $text .
                        "</font>";
                }

                return $result;
    }

    public function isNullOrEmptyString($str)
    {
        if (empty($str))
        {
            return true;
        }
        if (!is_string($str))
        {
            return true;
        }
        if ($str === "")
        {
            return true;
        }
        if ($str == "")
        {
            return true;
        }

        return false;
    }

    public function recursiveChmod($path, $filePerm = 0644, $dirPerm = 0755)
    {
        // Check if the path exists
        if (!file_exists($path))
        {
            return (false);
        }
        // See whether this is a file
        if (is_file($path))
        {
            // Chmod the file with our given filepermissions
            chmod($path, $filePerm);
            // If this is a directory...
        } elseif (is_dir($path))
        {
            // Then get an array of the contents
            $foldersAndFiles = scandir($path);
            // Remove "." and ".." from the list
            $entries = array_slice($foldersAndFiles, 2);
            // Parse every result...
            foreach ($entries as $entry)
            {
                // And call this function again recursively, with the same permissions
                TX::recursiveChmod($path . "/" . $entry, $filePerm, $dirPerm);
            }
            // When we are done with the contents of the directory, we chmod the directory itself
            chmod($path, $dirPerm);
        }
        // Everything seemed to work out well, return TRUE
        return (true);
    }

    public function endWith($FullStr, $EndStr)
    {
        // Get the length of the end string
        $StrLen = strlen($EndStr);
        // Look at the end of FullStr for the substring the size of EndStr
        $FullStrEnd = substr($FullStr, strlen($FullStr) - $StrLen);
        // If it matches, it does end with EndStr
        return $FullStrEnd == $EndStr;
    }

    public function startWith($Haystack, $Needle)
    {
        // Recommended version, using strpos
        return strpos($Haystack, $Needle) === 0;
    }

    // Another way, using substr
    public function startWithOld($Haystack, $Needle)
    {
        return substr($Haystack, 0, strlen($Needle)) == $Needle;
    }

    public function removeIllegalDictChars($strA)
    {
        $strR = str_replace('&ldquo;', '“', $strA);
        $strR = str_replace('&rdquo;', '”', $strR);
        $strR = str_replace('&hellip;', '…', $strR);
        //  $strR = str_replace('up', 'ddd', $strR);
        return $strR;
    }

    public static function initMySQLDB($userName, $password, $dbName, $host =
        'localhost', $port = 3306)
    {
        $db = new mysqli($host, $userName, $password, $dbName, $port);

        if (mysqli_connect_errno())
        {
            return ("数据库连接失败：" . mysqli_connect_error());
        }
        else
        {
            $db->set_charset('utf8');
            return $db;
        }
    }

    public static function closeMySQLDB($db)
    {
        if (isset($db))
        {
            $db->close();
        }
    }

    public static function getMySQLDBCountFull($sql, $userName, $password, $dbName,
        $host = 'localhost', $port = 3306)
    {
        $db = TX::initMySQLDB($userName, $password, $dbName, $host, $port);
        if (isset($db))
        {
            if (gettype($db) != "string")
            {
                if ($result = $db->query($sql))
                {
                    if ($row = $result->fetch_row())
                    {
                        $r = $row[0];
                    }
                    $result->close();
                    if (!isset($r))
                    {
                        $r = -1;
                    }
                    return $r;
                }
                else
                {
                    return "数据库查询结果错误";
                }
            }
            else
            {
                return "数据库对象不存在";
            }
        }
        else
        {
            return "数据库对象不存在";
        }
    }

    public static function getMySQLDBCount($db, $sql)
    {
        if (isset($db))
        {
            if ($result = $db->query($sql))
            {
                if ($row = $result->fetch_row())
                {
                    $r = $row[0];
                }

                $result->close();
                if (!isset($r))
                {
                    $r = -1;
                }
                return $r;
            }
            else
            {
                return "数据库查询结果错误";
            }
        }
        else
        {
            return "数据库对象不存在";
        }
    }

    public static function insertMySQLDBFull($sql, $userName, $password, $dbName, $host =
        'localhost', $port = 3306)
    {
        $db = TX::initMySQLDB($userName, $password, $dbName, $host, $port);
        if (isset($db))
        {
            if (gettype($db) == "string")
            {
                return $db;
            }
            else
            {
                $stmt = $db->prepare($sql);
                if ($stmt->execute())
                {
                    return $db->insert_id;
                }
                else
                {
                    return ('数据库插入记录失败：' . $stmt->error);
                }

                $stmt->close();

                $db->close();
            }

        }
        else
        {
            return ('数据库初始化失败，原因不明');
        }
    }

    public function assemble_request($method, $args = array(), $dump_request = false)
    {
        print_r($method);
        try
        {
            print phpversion();
            $request = xmlrpc_encode_request($method, array());
            print 'cc';
            $context = stream_context_create(array('http' => array('method' => "POST",
                'header' => "Content-Type: text/xml", 'content' => $request)));
            print_r($context);
            if ($dump_request)
            {
                echo '<pre>';
                echo htmlentities($request);
                echo '</pre>';
            }
        }
        catch (exception $eee)
        {
            die($eee->message);
        }
        return $context;
    }

    public function dump_response($response)
    {
        echo '<pre>';
        if (xmlrpc_is_fault($response))
        {
            trigger_error("xmlrpc: $response[faultString] ($response[faultCode])");
        }
        else
        {
            print_r($response);
        }
        echo '</pre>';
    }

    public function make_args_array($method, $session, $args = array(), $dump_args = false)
    {
        $domain = '2u4u.com.cn';
        $timestamp = (string )time();
        $nonce = user_password();
        $key = '4a13889853ed635941670fe6054a9759';
        $hash = hash_hmac('sha256', $timestamp . ';' . $domain . ';' . $nonce . ';' . $method,
            $key);
        $arrHash = array();
        array_push($arrHash, $hash, $domain, $timestamp, $nonce, $session);
        if ($dump_args)
        {
            echo '<pre>';
            var_dump(array_merge($arrHash, $args));
            echo '</pre>';
        }
        return array_merge($arrHash, $args);
    }

    public function user_password($length = 10)
    {
        $allowable_characters =
            'abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ23456789';
        $len = strlen($allowable_characters) - 1;
        $pass = '';
        for ($i = 0; $i < $length; $i++)
        {
            $pass .= $allowable_characters[mt_rand(0, $len)];
        }
        return $pass;
    }

    public function getDrupalUniqueCode($length = "")
    {
        $code = md5(uniqid(rand(), true));
        if ($length != "") return substr($code, 0, $length);
        else  return $code;
    }
}

class TXCalendarDayAction
{
    public $mainText;
    public $actionType;
    public $actionPath;

    public function __construct()
    {
        $mainText = "";
        $actionType = "Open Link";
        $actionPath = "about:blank";

    }

}

class TXCalendarDayArray
{
    public $date;
    public $actionList;

    public function __construct()
    {
        $date = "";
        $actionList = "";
    }

}

class DrupalXmlrpc
{

    function __construct($domain = '', $apiKey = '', $endPoint = '', $verbose = false)
    {
        // set local domain or IP address
        // this needs to match the domain set when you created the API key
        $this->domain = $domain;

        // set API key
        $this->kid = $apiKey;

        // set target web service endpoint
        $this->endpoint = $endPoint;

        // extended debugging
        $this->verbose = $verbose;

        // call system.connect to get our required anonymous sessionId:
        $retVal = $this->send('system.connect', array());
        var_dump($retVal);
        $this->session_id = $retVal['sessid'];

        if ($this->verbose)
        {
            $func = 'DrupalXmlrpc->__construct:';
            if ($this->session_id) error_log($func . ' got anonymous session id fine');
            else  error_log($func . ' failed to get anonymous session id!');
        }
    }

    /***********************************************************************
    * Function for sending xmlrpc requests
    */
    public function send($methodName, $functionArgs = array())
    {
        $protocolArgs = array();
        // only the system.connect method does not require a sessionId:
        if ($methodName == 'system.connect')
        {
            $protocolArgs = array($this->endpoint, $methodName);
        }
        else
        {
            $timestamp = (string )time();
            $nonce = $this->getUniqueCode("10");

            // prepare a hash
            $hash_parameters = array($timestamp, $this->domain, $nonce, $methodName);
            $hash = hash_hmac("sha256", implode(';', $hash_parameters), $this->kid);

            // prepared the arguments for this service:
            // note, the sessid needs to be the one returned by user.login
            $protocolArgs = array($this->endpoint, $methodName, $hash, $this->domain, $timestamp,
                $nonce, $this->session_id);
        }

        $params = array_merge($protocolArgs, $functionArgs);
        return call_user_func_array('xmlrpc', $params);
    }

    /***************************************************
    * login and return user object
    */
    public function userLogin($userName = '', $userPass = '')
    {
        if ($this->verbose) error_log('DrupalXmlrpc->userLogin() called with userName "' .
                $userName . '" and pass "' . $userPass . '"');

        // clear out any lingering xmlrpc errors:
        xmlrpc_error(null, null, true);

        $retVal = $this->send('user.login', array($userName, $userPass));
        if (!$retVal && xmlrpc_errno())
        {
            if ($this->verbose) error_log('userLogin() failed! errno "' . xmlrpc_errno() .
                    '" msg "' . xmlrpc_error_msg() . '"');
            return false;
        }
        else
        {
            // remember our logged in session id:
            $this->session_id = $retVal['sessid'];

            // we might need the user object later, so save it:
            $user = new stdClass();
            $user = (object)$response['user'];
            $this->authenticated_user = $user;
            return $user;
        }
    }

    /***************************************************
    * logout, returns 0 for okay, or -1 for error.
    */
    public function userLogout()
    {
        $retVal = $this->send('user.logout', array());
        if (!$retVal)
        {
            if ($this->verbose) error_log('userLogout() failed! errno "' . xmlrpc_errno() .
                    '" msg "' . xmlrpc_error_msg() . '"');
            return - 1;
        }

        return 0; // meaning okay
    }

    /***************************************************
    * Function for generating a random string, used for
    * generating a token for the XML-RPC session
    */
    public function getUniqueCode($length = "")
    {
        $code = md5(uniqid(rand(), true));
        if ($length != "") return substr($code, 0, $length);
        else  return $code;
    }
}

?>