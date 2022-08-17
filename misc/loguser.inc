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
 | Filename: loguser.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: this file logs the user time and their request to the log       |
 | file.  Include it when  a student user is attempting to access an admin      |
 | page.  It returns them to  the welcome screen.  The log file is specified    |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/

$result = $GLOBALS['dbh']->Execute("SELECT CONCAT(first_name, ' ', last_name) AS student_name FROM student WHERE login_id='$login_id'");$fullname = ($row = $result->FetchRow()) ? $row["student_name"] : "?";

$outpute = "\n---ATTEMPTED ACCESS OF ADMIN PAGE BY STUDENT---\n";
$outpute .= "Date:    " . date("Y-m-d h:i:sA T") . "\n";
$outpute .= "Login:   " . $login_id . "\n";
$outpute .= "Name:    " . $fullname . "\n";
$outpute .= "IP:      " . $REMOTE_ADDR . "\n";
$outpute .= "Request: " . $REQUEST_URI . "\n\n\n";

$output = date("Y-m-d h:i:sA T") . "\t";
$output .= $login_id . "\t";
$output .= $fullname . "\t";
$output .= $REMOTE_ADDR . "\t";
$output .= $REQUEST_URI . "\t\n";

$file = preg_replace ("/\/[^\/]*$/", "/", $HTTP_SERVER_VARS["PATH_TRANSLATED"]) . LOGFILE;
if ($fp = fopen ($file, "a"))
{
   $dump = flock($fp, LOCK_EX);
   $dump = fwrite($fp, $output);
   $dump = flock($fp, LOCK_UN);
   $dump = fclose($fp);
}

if (SECURITY_EMAIL != "")
{
   mail(SECURITY_EMAIL, "".BRAND_NAME." - Security Alert", $outpute, "From: ".BRAND_NAME_SYSTEM_EMAIL."\n");
}

  include ('welcome.inc');
?>
