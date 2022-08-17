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
 | Filename: sign_up.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: High level code for student sign up sheet.                      |
 +------------------------------------------------------------------------------+

*/

?>
</td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" border="0" alt='' /></td></tr></table>

	<table border="0" cellspacing="0" cellpadding="20" width="100%"><tr><td>
<?php

if (isValidStudentNumber($student_number))
{
	// valid student number
	if (isValidJobID($job_id) && isStudentShortlisted($job_id, $student_number) && isShowable($job_id))
	{
		// show specific job
		if ($userlevel == STUDENT){
			$GLOBALS['dbh']->Execute( "UPDATE students_shortlisted SET last_visited = '" . date("Y-m-d H:i:s") . "'
					  WHERE student_number='$student_number' AND job_id='$job_id'");
		}		
		include('interview/sign_up/student/interview.inc');
	}
	else
	{
		// show default list
		include('interview/sign_up/student/interview_list.inc');
	}
}
else
{
	// not valid student number
	error("<p>Your student number is not in the interview sign-up database.  Please contact
			your Co-op Office.</p>");
}


?>
	</td></tr></table>
