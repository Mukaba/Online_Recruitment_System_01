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
 | Filename: save.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used only by the uselevel OFFICE as they are the   |
 | only userlevel allowed to edit transcripts at the moment. It grabs the       |
 | string, removes bad tags, and saves the transcript string to the database.   |
 +------------------------------------------------------------------------------+

*/


$str = $transcript;

$str_new = strip_html_tags($str);

if ($str_new != $str){
	$warning = "<table border=\"0\"><tr><td><font size=\"6\" color=\"red\"><blink>Warning: </blink></font></td><td><b>Resumes should not have HTML, HEAD, BODY, SCRIPT, H1, H2 tags.  HTML, HEAD, BODY, SCRIPT tags have been removed; H1 tags coverted to H3 tags and H2 tags coverted to H4 tags.</b></td></tr></table>";
}
$str = $str_new;

//save
$sql = "REPLACE INTO transcript(student_number, transcript, last_modified) VALUES ('$student_num', '" . addslashes(stripslashes($str)) . "', '" . date('Y-m-d, H:i:s', time()) . "')";
$result = $GLOBALS['dbh']->Execute($sql);


?>	
