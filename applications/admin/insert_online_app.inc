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
 | Filename: insert_online_app.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file is called to insert online applications into the      |
 | database.                                                                    |
 +------------------------------------------------------------------------------+

*/
// file called to insert online applications

// check if there are any already sent, if there are, then we send this one, otherwise, just add it
$sql = "SELECT DISTINCT application_id
	FROM applications
	WHERE job_id = '$job_id'
	AND app_status = '".APP_ACTIVATED_ONLINE."'";
$result = $GLOBALS['dbh']->Execute ($sql);
if ($result->RecordCount())
	$app_status = APP_ACTIVATED_ONLINE;
else
	$app_status = APP_APPLIED_ONLINE;

for ($i=0; $i < count($stud_num); $i++) {

	$namesql = $GLOBALS['dbh']->Execute ("SELECT CONCAT(last_name, ', ', first_name) as name FROM student where student_number = '$stud_num[$i]'");
	$namerow = $namesql->FetchRow();
	$name_success = $namerow["name"];

	// first check to see if they're already an application for the student
	$check = $GLOBALS['dbh']->Execute ("SELECT application_id, student_number FROM applications 
				WHERE student_number = '$stud_num[$i]' AND job_id = '$job_id'");

	// need to copy their coverletter to a new one, just in case they make changes to it, won't affect this app
	if ($coverletter_id[$i] != 0) {
		$lettersql = ("SELECT letter FROM coverletter 
					WHERE student_number = '$stud_num[$i]' AND coverletter_id = '$coverletter_id[$i]'");

		// since it was selected from a list, we assume there's only 1
		$findletter = $GLOBALS['dbh']->Execute ($lettersql);
		$lrow = $findletter->FetchRow();
		
		$letter = $lrow ["letter"];

		$newletter = "INSERT INTO coverletter (student_number, letter, last_modified, type_id)
				VALUES ('$stud_num[$i]', '". addslashes (stripslashes ($letter)) ."', '" . date('Y-m-d, H:i:s', time()) . "', '1')";
		$insertletter = $GLOBALS['dbh']->Execute ($newletter);

		// now get its ID
		$newletter_id = $GLOBALS['dbh']->Insert_ID();
	}
	
	// if there isn't, put one in
	if ($check->RecordCount() < 1) {
		$sql = "INSERT INTO applications (student_number, job_id, app_status, applied_date,
			coverletter_id, resume_id, transcript, added_by_admin) 
			VALUES ('$stud_num[$i]', '$job_id', '$app_status', '" . date('Y-m-d, H:i:s', time()) . "',
			'". (($coverletter_id[$i] == 0) ? "" : $newletter_id) ."', 
			'$resume_id[$i]', '". (($inc_trans[$i] == 1) ? "1" :  "") ."', '1')";

		if ($result = $GLOBALS['dbh']->Execute ($sql)) {
			if ($successful == "")
				$successful = " &nbsp; &nbsp; $name_success";
			else
				$successful .= "<br /> &nbsp; &nbsp; $name_success";
		}
		else {
			if ($successful == "")
				$unsuccessful = " &nbsp; &nbsp; $name_success";
			else
				$unsuccessful .= "<br /> &nbsp; &nbsp; $name_success";
		}
	}

}


// now do a quick check if we should set up the job status to "resumes pulled"
$checkpull = $GLOBALS['dbh']->Execute ("SELECT application_id FROM applications
		WHERE added_by_admin = '1' AND job_id = '$job_id'");

if ($checkpull->RecordCount() > 2) {
	$presult = $GLOBALS['dbh']->Execute ("UPDATE job_info SET resumes_pulled = '1' WHERE job_id = '$job_id'");
}
			

?>
