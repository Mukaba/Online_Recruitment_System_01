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
 | Filename: confirmChange.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This form is called if an interview is changed from an          |
 | off-campus phone interview to an on-campus phone interview and one or more   |
 | students have already signed up phone numbers.                               |
 +------------------------------------------------------------------------------+

*/


	assert($job_id);

	if (sizeof($phone_student) == 1)
	{
		$msg = ($phone_student[0]["name"] . " has already entered a phone number for this interview.  Are you sure you want to change it to an on-campus phone interview?");
	} else
	{
		for ($i = 0; $i < (sizeof($phone_student) - 1); $i++)
		{
			$msg .= ($phone_student[$i]["name"] . ", ");
		}
		$msg .= ("and " . $phone_student[$i]["name"]);
		$msg .= (" have already entered phone numbers for this interview.  Are you sure you want to change it to an on-campus phone interview?");
	}
	
	$msg .= ("<table border='0' width='100%' cellspacing='2' cellpadding='2'>
			<tr><td align='right'>
	 		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit' />
			<input type='hidden' name='level1' value='jobinfo' />
			<input type='hidden' name='submit' value='Save the changes' />
			<input type='hidden' name='changeDetected' value='true' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input type='hidden' name='interview' value='" . packObject(($interview)) . "' />
		");
	$msg .= ("<input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
   			</form>
	 		</td>
 			<td align='left'>
	  		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit' />
			<input type='hidden' name='level1' value='jobinfo' />
			<input type='hidden' name='submit' value='' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
   	  		</form></td></tr>
			</table>");
	echo("<br />");
	error( $msg );   	

?>
