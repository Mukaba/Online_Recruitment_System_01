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
 | Description: This file is used by both userlevel OFFICE and STUDENT.  In     |
 | the case of the STUDENT there will only ever be one coverletter being saved  |
 | at a time, but in the case of OFFICE there could be multiple coverletters    |
 | being saved at once (in the case where the admin adds multiple               |
 | coverletters).  When multiple coverletters are being added we are copying    |
 | the students generic coverletter and generic resume to made a specific       |
 | coverletter for the selected job.  If there is no generic resume the         |
 | coverletter will not created for that job.  If the student has no generic    |
 | coverletter the coverletter will be left blank for the job.                  |
 +------------------------------------------------------------------------------+

*/

$error = "";

if (($page == "Save Cover Letter" || $page=="Submit File" || $page=="Add Cover Letter")  && $userlevel == OFFICE){
	
	/*In this case only a single coverletter is being added by OFFICE userlevel
	 *Here we are ensureing that the student number is valid, the resume is valid, and the coverletter
	 *type is valid.
	 *
	if ($student_num == ""){
		$error = "Invalid Student Number";
	}else{
		if ($type_id != SPECIFIC){
			$save_error = "You cannot add Generic or Template Resumes for students.";
		}
		$students = array($student_num);
	}*/
}elseif($userlevel == STUDENT){
	
	/*Here a single coverletter is being added by a STUDENT. */

	$students = array($student_number);
}

/*If no errors have occured than we want to save the coverletter(s)*/

if ($save_error == ""){

	if ($use_generic == "on"){
			
		if ( $userlevel == OFFICE ){
						
			/*If we are using generic coverletters then we need to ensure the student number
		  	 *is valid. Don't care about resumes.
			 */
			if (!isValidStudentNumber($student_num, $departments_in_str)){
				error ("Student number - " . $student_number . " - does not exist in the database.");
				continue;
			}
		}
			
		/*
		 *Now we get the the generic coverletter for the student and save it in the $str variable.
		 */
		$sql = "SELECT letter 
			FROM coverletter 
			WHERE job_id = '0' AND type_id = " . GENERIC . " AND 
				student_number = '" . addslashes(stripslashes($student_num)) . "'";
						
		$result = $GLOBALS['dbh']->Execute( $sql );
			
		if ($result->RecordCount() <= 0){
			error (getStudentName($student_num) . " does not have a generic coverletter.  Their 
				coverletter for this job will be blank.");
		}
				
		$row =  $result->FetchRow();
		$str = $row["letter"];
	}else{
			
		/*Only a singe resume is being added so we ensure the student number
		 *is correct and then find the coverletter string and put it into the $str variable.
		*/ 
	//	if (!isValidStudentNumber($student_num, $department_in_str)){
	//		$save_error = "Invalid Student";
	//	}
		if ($page == "Submit File"){
		/*	if (is_uploaded_file($letter_file)){
				$fp = fopen($letter_file, "r");
				$str = fread($fp, filesize( $letter_file ));
				fclose($fp);
				unset($letter_file);
			}else{
				echo "File not uploaded";
			}*/
		}else{
			$str = $letter;
		}
	}

	/*Remove invalid tages - the warning will be printed in the view_html.inc file if there are
	 *not multiple students being added.
	 */
	$str_new = strip_html_tags($str);

	if ($str_new != $str){
		$warning = "<table border=\"0\"><tr><td><font size=\"6\" color=\"red\"><blink>Warning: </blink></font>
			</td><td><b>Cover Letters should not have HTML, HEAD, BODY, SCRIPT, STYLE, TITLE, H1, H2, PRE tags.  
			<br /> HTML, HEAD, BODY, SCRIPT, STYLE, TITLE, PRE tags have been removed; H1 tags coverted to H3 tags 
			and H2 tags coverted to H4 tags.</b></td></tr></table>";
	}

	$str = $str_new;

	if ($coverletter_id <= 0) {
		$sql = "INSERT into coverletter (type_id, student_number, letter, last_modified) values
			('$type_id', '$student_number', '".addslashes(stripslashes($str))."', '".date('Y-m-d, H:i:s', time())."')";
	}
	else { 
		$sql = "Update coverletter set letter='".addslashes(stripslashes($str))."', 
			last_modified = '".date('Y-m-d, H:i:s', time())."' 
                        WHERE coverletter_id = '" . addslashes(stripslashes($coverletter_id)) . "';";
	}

	$result = $GLOBALS['dbh']->Execute($sql);

	if (sizeof($students) > 1){
		$student_num = "";
	}

	if ($coverletter_id <= 0) {
		$sql = "SELECT LAST_INSERT_ID() AS coverletter_id";
      		$result = $GLOBALS['dbh']->Execute($sql);
     		$row = $result->FetchRow();
   		$coverletter_id = $row["coverletter_id"];
	}


}else{
	
	/*An error has occured so we don't want to save the coverletter.  Here we unset the PDF variable so
	 *the coverletter.inc file will be called again and the save error printed out
	 */
	unset($PDF);
}

?>	
