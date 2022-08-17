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
 | Filename: update_students.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is called by edit_students.inc after the user opts    |
 | to save his changes.  Changes are saved, and if a conflict is encountered,   |
 | the user is prompted for confirmation.  If the user confirms the conflict,   |
 | then a flag is set so that the e-mail form will be displayed after.          |
 +------------------------------------------------------------------------------+

*/

/*** 	update short listed students info    ***/
	unset($students);
	$tempstudents = explode(chr(10), trim( $interview->students));
	for ($i = 0; $i < sizeof($tempstudents); $i++)
	{
		if (trim($tempstudents[$i]) != '')
		{
			$students[] = $tempstudents[$i];
		}
	}
	for ($i = 0; $i < sizeof($students); $i++)
    	{
		$studentNum = findStudentNumbers($students[$i]);
		if( count($studentNum) > 1 )
		{
			$duplicate_students[] = $studentNum;	
		}
		elseif( count($studentNum) == 1 )
		{
			$sid_array[] = $studentNum[0];
		}
		else
		{
			error("<b>Warning: $students[$i]</b> doesn't exist in the database.  The student must be added to the database before being added to an interview shortlist.<br />" );
		}
		unset($studentNum);
	}
	
	/*** 		check if there are enough slots  		***/
	
	$result =isSlotsEnough(	$interview->schedules, $interview->students, ((isSwitching($interview->interview_type) || isSimultaneous($interview->interview_type) )? true : false ));
	if( $result < 0 )
	{
		error("Warning: More short-listed students than scheduled time slots.<br />
		       ");		
	}
	
	$job_code = $interview->job_code;
	$last_modified = $interview->last_modified;
	update_duplicate("edit");
	
?>
