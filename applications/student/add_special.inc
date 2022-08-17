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
 | Filename: add_special.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This is to add special jobs, such as ones in which the          |
 | student must apply at a company's site. It essentially creates an 'empty'    |
 | application.                                                                 |
 +------------------------------------------------------------------------------+

*/

$sql = "SELECT job_code, position_title FROM job_info where job_id = '$job_id'";
$result = $GLOBALS['dbh']->Execute($sql);
if ($job_row = $result->FetchRow()) {} else { error("Error with applying: Job id lost"); }

// let's actually insert applications
$inserting = "Insert into applications (student_number, job_id, applied_date, app_status)
	values ('".addslashes(stripslashes($student_number))."', '".addslashes(stripslashes($job_id))."', 
	'".date('Y-m-d, H:i:s', time())."', '7')";

//check, just to make sure
if ($GLOBALS['dbh']->Execute ($inserting)) {
	notify("You have been added to the list for:<b> ".$job_row["job_code"].", ". $job_row["position_title"]."</b>");

	echo ("<form method='post' action='$PHP_SELF'>");
	echo ("<input type='submit' value='Return to Job Descriptions' />");
	echo ("</form>");
}

// we should never come here.
else {
	error ("Error inserting application.");
}


?>
