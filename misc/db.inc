<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: db.inc                                                             |
 +------------------------------------------------------------------------------+
 | Description: Database configuration file                                     |
 +------------------------------------------------------------------------------+

*/
$db_dsn = defined("DB_DSN") ? trim(constant("DB_DSN")) : null;
if (!empty($db_dsn)){
    $dbh = &ADONewConnection(DB_DSN);
} else { // Old style mysql only connect    
    if (!defined("DB_TYPE")) die("<b>DB_TYPE</b> not defined.  You must define this or use DB_DSN.");
    if (!defined("DB_USER")) die("<b>DB_USER</b> not defined.  You must define this or use DB_DSN.");
    if (!defined("DB_PASS")) die("<b>DB_PASS</b> not defined.  You must define this or use DB_DSN.");
    if (!defined("DATABASE")) die("<b>DATABASE</b> not defined.  You must define this or use DB_DSN.");
    $dbh = &ADONewConnection(DB_TYPE);
    $dbh->Connect('localhost', DB_USER, DB_PASS, DATABASE);
}

// This function alerts us of broken queries. This is called from the ADOdb error
// function. Check adodb.inc.php's $raiseErrorFn for more information
function alert_broken_query() {
	global $IPS_FOR_DEBUG;
	global $REMOTE_ADDR;
    global $auth;

    if (func_get_arg(1) != 'EXECUTE' && func_get_arg(1) != 'CacheExecute') {
        return;
    }

    if (func_get_arg(1) == 'EXECUTE') {
        $sql = func_get_arg(4);
        $error_desc = func_get_arg(3);
    }    

    if (func_get_arg(1) == 'CacheExecute') {
        $sql = func_get_arg(5);
        $error_desc = func_get_arg(3);
    }

    //Applying regular expression and retrieving the skeletal query
    $date = date('Y-m-d H:i:s', time());     
    $patterns = array("/\'(\w+)\'/", "/\"(\w+)\"/", "/=\s(\d+)/", "/\'/", "/\"/", "/\n/", "/\t/" );
    $replace = array("", "", "=", "", "", " ", " ");
    $brokenquery = preg_replace ($patterns, $replace,$sql);
    
    $arrDebug = debug_backtrace();

    $file_name = $arrDebug[2]["file"];
    $line_number = $arrDebug[2]["line"];

    //Generating the insert query according to the login
    if($auth->userlevel == STUDENT)  
    {
        $insert_sql = ("
            INSERT INTO broken_queries 
            SET query = '".$brokenquery."', date_time = '".$date."', student_number ='". $auth->student_number."', IP_address = '".$REMOTE_ADDR."', counter = 1
            , file_name = '".$file_name."', line_number = '".$line_number."'
            ");
    }
    else if((strcmp($auth->contact_id,"") == 0) || $auth->contact_id == NULL)
    {
        $insert_sql = ("
            INSERT INTO broken_queries 
            SET query = '".$brokenquery."', date_time = '".$date."', IP_address = '".$REMOTE_ADDR."', counter = 1
            , file_name = '".$file_name."', line_number = '".$line_number."'
            ");
    }
    else if($auth->userlevel == OFFICE || $auth->userlevel == FACULTY)
    {
        $insert_sql = ("
            INSERT INTO broken_queries 
            SET query = '".$brokenquery."', date_time = '".$date."', contact_id =". $auth->contact_id.", IP_address = '".$REMOTE_ADDR."', counter = 1
            , file_name = '".$file_name."', line_number = '".$line_number."'
            ");
    }
    else if($auth->userlevel == TRAFFICDIRECTOR)
    {
        $insert_sql = ("
            INSERT INTO broken_queries 
            SET query = '".$brokenquery."', date_time = '".$date."', contact_id = ".$auth->contact_id.", IP_address = '".$REMOTE_ADDR."', counter = 1
            , file_name = '".$file_name."', line_number = '".$line_number."'
            ");  
    }
    
    $dbname = DATABASE;
    $email_to = DB_EMAIL_TO;
    $body = "This e-mail has been sent to notify you that there was an error while processing a query\n".
        "Here is the info on the break up of this query \n\n".
        "\tThe Error: ".$error_desc."\n\n".
        "\tThe Query: ".$sql."\n\n".
        "\tThe File: ".$file_name."\n".
        "\tThe Line: ".$line_number."\n\n".
        "\tThe Address: ".$REMOTE_ADDR."\n".
        "\n\n\tThis error occured at ".$date;        
    //Calling the perl script.
    exec(SCRIPT_PATH."/email_broken_queries.pl \"$body\" \"$insert_sql\" \"$brokenquery\" \"$email_to\" \"$dbname\" ");

    // $IPS_FOR_DEBUG are the IP addresses of the developers. We only want to display debugging information to the developers, not the general public. 
    if($error_desc && (is_array($IPS_FOR_DEBUG) && in_array($REMOTE_ADDR, $IPS_FOR_DEBUG)))
    {
        error("Debug Error Information: " . $error_desc . "<br /><br />Broken Query: " . $sql."<br /><br />File Name: ".$file_name."<br />Line Number: ".$line_number);
    }
}

?>
