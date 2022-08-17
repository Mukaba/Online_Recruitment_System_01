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
 | Description: This file is used both by administrators and students to save   |
 | their resumes to the database. The resume string is found, tested to ensure  |
 | it doesn't contain certain tages, and saved to the database. If an error     |
 | occurs the variable $error will contain the error string                     |
 +------------------------------------------------------------------------------+

*/

//find the string to be saved
if ($htmlstring) {
	
	$str = addslashes(stripslashes(unpackObject(($htmlstring))));
}
else {

	//text area was updated
	$str = $resume;
}


//remove HTML, BODY, and H[1-9] tags
$str_new = strip_html_tags($str);

//if some tags were removed than need to warn the user
//note this warning is printed in the view_html.inc (OFFICE) or view.inc(STUDENT) file
if ($str_new != $str){

	$warning = "<table border=\"0\"><tr><td><font size=\"6\" COLOR=\"RED\">Warning:</font></td><td><b>
			Resumes should not have HTML, HEAD, BODY, SCRIPT, TITLE, STYLE, H1, H2, PRE tags.  <br />HTML, HEAD, BODY, 
			SCRIPT, TITLE, STYLE, PRE tags have been removed; H1 tags coverted to H3 tags and H2 tags coverted to H4 
			tags.</b></td></tr></table>";
}
$str = $str_new;

$student = "";
if ($userlevel == STUDENT) {

	$student = $student_number;
}

elseif ($userlevel == OFFICE) {

	$student = $student_num;
}

if ($type_id == ""){

	$save_error = "Invalid type.";
}

elseif ($type_id != SPECIFIC && $resume_id== "") {

	//this check is for people hacking.  If one not specific resume exists we don't want to allow them to add another
	$sql = "SELECT resume_id FROM resume WHERE student_number = '" . addslashes($student) . "' AND type_id = '" . addslashes($type_id) . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount() >=1) {

		$save_error = "You already have a " . (($type_id == GENERIC) ? "generic" : (($type_id == TEMPLATE) ? "template" : "error")) . " resume.  You can not add another. ";
	}
}

if (trim ($name) == "") {

	switch ($type_id) {

		case GENERIC:
			$name = "Generic";
			break;

		case TEMPLATE:
			$name = "Template";
			break;

		case SPECIFIC:
			$name = "Specific";
			break;
	}
}

if ($userlevel == ADMINSTUDENT) {

	$save_error = "You cannot make changes to resumes while logged in as an administrator.";
}

if (!isValidStudentNumber($student, $department_in_str)) {

	$save_error = "You do not have access to students in this department";
}

if ($resume_id != "" && !isValidResume($student, $resume_id)) {

	$save_error = "Invalid resume.";
}

if ($save_error == "") {

	/*if we are adding a resume then the resume_id will not yet be set and we need to insert the new information
	 *otherwise we need to update the information corresponding to the given resume id.
	 */
	if ($resume_id == "") {

			$sql = "INSERT INTO resume (resume_id, student_number, resume, last_modified, type_id, name) 
				VALUES (NULL, '$student', '" . addslashes(stripslashes($str)) . 
				"', '" . date('Y-m-d, H:i:s', time()) . "', '" . addslashes($type_id) . 
				"', '" . addslashes(stripslashes($name)) . "')";
	}
	
	else {
	
		
			$sql = "UPDATE resume SET
					resume = '" . addslashes(stripslashes($str)) . "',
					last_modified = '" . date('Y-m-d, H:i:s', time()) . "',
					type_id = '" . addslashes($type_id) . "',
					name = '" . addslashes(stripslashes($name)) . "',
				WHERE 
					resume_id = '" . addslashes($resume_id) . "' 
					AND student_number = '" . addslashes($student) . "'";
	}
	
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($resume_id == "") {
	
		$result2 = $GLOBALS['dbh']->Execute ( "SELECT LAST_INSERT_ID()" );
		$row = $result2->FetchRow();
		$resume_id = $row[0];
	}
}

?>	
