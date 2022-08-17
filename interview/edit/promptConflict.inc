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
 | Filename: promptConflict.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This form is called when a student already signed up for an     |
 | interview is removed from the shortlist.  This form prompts the user for     |
 | confirmation of removal.                                                     |
 +------------------------------------------------------------------------------+

*/


	assert($job_id);

	if (sizeof($conflict_studentnum) == 1)
	{
		$msg = ($conflict_studentname[0] . " is already signed up for an interview.  Are you sure you want to remove them from the shortlist?");
	} else
	{
		for ($i = 0; $i < (sizeof($conflict_studentname) - 1); $i++)
		{
			$msg .= ($conflict_studentname[$i] . ", ");
		}
		$msg .= ("and " . $conflict_studentname[$i]);
		$msg .= (" are already signed up for an interview.  Are you sure you want to remove them from the shortlist?");
	}
	
	$noconflict = false;
	$msg .= ("<table border='0' width='100%' cellspacing='2' cellpadding='2'>
			<tr><td align='right'>
	 		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit' />
			<input type='hidden' name='level1' value='students' />
			<input type='hidden' name='status' value='Continue' />
			<input type='hidden' name='status' value='Conflict resolved' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input type='hidden' name='interview' value='" . packObject(($interview)) . "' />
			<input type='hidden' name='students' value=\"" . addslashes($interview->students) . "\" />");
			for ($i = 0; $i < sizeof($conflict_studentname); $i++)
			{
				$msg .= ("<input type='hidden' name='conflict_studentname[$i]' value='" . addslashes($conflict_studentname[$i]) . "' />");
			}
	$msg .= ("<input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
   			</form>
	 		</td>
 			<td align='left'>
	  		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit' />
			<input type='hidden' name='level1' value='students' />
			<input type='hidden' name='status' value='Conflict edit' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
   	  		</form></td></tr>
			</table>");
	error( $msg );   	

?>
