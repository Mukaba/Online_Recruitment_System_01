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
 | Filename: delete_date.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This form is called when the user wishes to delete a date. It   |
 | will check to see if any students have signed up for interviews on this      |
 | date, and then confirm the removal.  If there are students signed up for     |
 | interviews on this date, it lets the user know this when prompting for       |
 | confirmation.  Called by edit_datetime.inc                                   |
 +------------------------------------------------------------------------------+

*/


	assert($job_id);

	$tempresult = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT a.student_number, b.first_name, b.last_name
			FROM interview_time AS a, student AS b
			WHERE a.int_date = '$cancel_date'  
			AND a.job_id = '$job_id'
			AND a.student_number IS NOT NULL
			AND a.student_number = b.student_number
			");

	while ($temprow = $tempresult->FetchRow())
	{
		$students_dropped[] = $temprow["student_number"];
		$students_names[] = $temprow["first_name"] . " " . $temprow["last_name"];
	}

	if (sizeof($students_dropped) == 1)
	{
		$msg = ($students_names[0] . " is already signed up for an interview on this day.  Are you sure you want to delete this interview date (" . $cancel_date . ")?");
	} elseif(sizeof($students_dropped) > 1)
	{
		for ($i = 0; $i < (sizeof($students_dropped) - 1); $i++)
		{
			$msg .= ($students_names[$i] . ", ");
		}
		$msg .= ("and " . $students_names[$i]);
		$msg .= (" are already signed up for interviews on this day.  Are you sure you want to delete this interview date (" . $cancel_date . ")?");
	} else
	{
		$msg = ("Are you sure you want to delete this interview date (" . $cancel_date . ")?");
	}
	unset($students_dropped);
	unset($students_names);

	$msg .= ("<table border='0' width='100%' cellspacing='2' cellpadding='2'>
			<tr><td align='right'>
	 		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit' />
			<input type='hidden' name='level1' value='timedate' />
			<input type='hidden' name='submit' value='Time Calculator' />
			<input type='hidden' name='time_mode' value='Save changes' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input type='hidden' name='interview' value='" . packObject(($interview)) . "' />
			<input type='hidden' name='cancel_day[0]' value='" . $cancel_day[0] . "' />");
			for ($i = 0; $i < sizeof($students_dropped); $i++)
			{
				$msg .= ("<input type='hidden' name='students_dropped[$i]' value='" . addslashes($students_dropped[$i]) . "' />");
			}
	$msg .= ("<input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
   			</form>
	 		</td>
 			<td align='left'>
	  		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit' />
			<input type='hidden' name='level1' value='timedate' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
   	  		</form></td></tr>
			</table>");
	error( $msg );   	
?>
