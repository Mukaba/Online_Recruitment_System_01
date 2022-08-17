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
 | Filename: log_form_variables.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This file is used to save all of the form variables that we     |
 | just got into a file in the logs directory.                                  |
 +------------------------------------------------------------------------------+

*/


$fp = fopen(PATH_LOGS . "logs_form_vars.log", "a");

$str = ("\n*********************************************************\n");
$sql="SELECT CURRENT_DATE as date, CURRENT_TIME as time";
$result=$GLOBALS['dbh']->Execute($sql);
while ($row=$result->FetchRow()){$date=$row['date'];$time=$row['time'];}
$str .= ("$date : $time : $auth->name : $REMOTE_ADDR\n");
    if( trim( $QUERY_STRING ) ){
    		$query = stripslashes($QUERY_STRING);
                $str .= "\tURL = $query\n";
		}
$str .= "\tBROWSER/PLATFORM = $HTTP_USER_AGENT\n";
		
foreach( $HTTP_POST_VARS as $var=>$value )
{
	$str .= ("\t$var = $value\n");
}

$str .= ("*********************************************************\n");
$str .= ("\n");

fputs($fp, $str);

?>
