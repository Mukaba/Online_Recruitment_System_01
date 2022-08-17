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
 | Filename: userErrorHandler.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: *  user defined error handling                                  |
 +------------------------------------------------------------------------------+

*/


// we will do our own error handling
// error_reporting(0);

// user defined error handling function
function userErrorHandler ($errno, $errmsg, $filename, $linenum, $vars)
{
	global $QUERY_STRING;
	global $REMOTE_ADDR;
	global $HTTP_USER_AGENT;
	
	// ignore runtime notice
    if( $errno == E_NOTICE ) {
    	return;
    }
    // PHP 5 compatibility
    elseif ($errno == E_STRICT) {
        return;
    }

    // timestamp for the error entry
    $time = date("Y-m-d H:i:s (T)");

    // define an assoc array of error string
    // in reality the only entries we should
    // consider are 2,8,256,512 and 1024
    $errortype = array (
                1   =>  "Error",
                2   =>  "Warning",
                4   =>  "Parsing Error",
                8   =>  "Notice",
                16  =>  "Core Error",
                32  =>  "Core Warning",
                64  =>  "Compile Error",
                128 =>  "Compile Warning",
                256 =>  "User Error",
                512 =>  "User Warning",
                1024=>  "User Notice"
                );
    // set of errors for which a var trace will be saved
    $user_errors = array(E_USER_ERROR, E_USER_WARNING, E_USER_NOTICE);
    
    $err = "<errorentry>\n";
    $err .= "\t<datetime>".$time."</datetime>\n";
    $err .= "\t<errornum>".$errno."</errornum>\n";
    $err .= "\t<errortype>".$errortype[$errno]."</errortype>\n";
    $err .= "\t<errormsg>".$errmsg."</errormsg>\n";
    $err .= "\t<scriptname>".$filename."</scriptname>\n";
    $err .= "\t<scriptlinenum>".$linenum."</scriptlinenum>\n";

    if( trim( $QUERY_STRING ) )
	    $err .= "\t<query>".$QUERY_STRING."</query>\n";
	
    $err .= "\t<ip>" . $REMOTE_ADDR . "</ip>\n";
    $err .= "\t<http_user_agent>" . $HTTP_USER_AGENT . "</http_user_agent>\n";


//    if (in_array($errno, $user_errors))
//    	$err .= "\t<vartrace>".wddx_serialize_value($vars,"Variables")."</vartrace>\n";

    $err .= "</errorentry>\n\n";
    
    //display error message to user
    echo $errmsg;

    // save to the error log, and e-mail me if there is a critical user error
    error_log($err, 3, PATH_LOGS . "error.log");

//    if ($errno == E_USER_ERROR)
//        mail("phpdev@mydomain.com","Critical User Error",$err);

}

set_error_handler("userErrorHandler");

//trigger_error("ROBIN", E_USER_ERROR);

?>
