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
 | Filename: edit_students.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This form is called by editchoose.inc when the user wishes to   |
 | edit the shortlisted students for this interview.                            |
 +------------------------------------------------------------------------------+

*/


assert($job_id);
$jobresult = $GLOBALS['dbh']->Execute("
		SELECT contact_id, employer_department_id, department_id, employer_id, job_code
		FROM job_info 
		WHERE job_id='" . $job_id . "'
		");
$jobrow = $jobresult->FetchRow();
$job_code = $jobrow["job_code"];
		

// if a job has been selected...

if( isset($interview) )
{
   	$interview = unpackObject($interview);
   	if( $submit != "Close Time Calculator" && $submit != "Modify Schedule" && 
			$submit != "Time Calculator" && empty($subselection) )
   	{

		/*
		 Set the shortlisted students variable here.
		*/
		$interview->students = stripSlashes(stripslashes($students));
			
	}

	/*
	 If the student part has been filled out, we need to check for
	 conflicts, and if there are any, confirm removal.
	*/

	$noconflict = true;
	unset($conflict_studentname);
	unset($conflict_studentnum);
	unset($conflict_studentemail);

	/*
	 Check and see if any students that have already signed up for
	 interviews have been removed from the shortlist.  The variable
	 confirm is changed only if we have students signed up for 
	 interviews and shortlisted.
	*/
	
	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT a.first_name, a.last_name, a.student_number 
		FROM student AS a, interview_time AS b
		WHERE b.job_id='$job_id' AND a.student_number=b.student_number
		");

	/*
	 We now have a list of all the students that were signed up
	 for interviews with this job.  Now we need to check and see
	 if those students are still on the shortlist, or if they've
	 been removed.
	*/


	while($row = $result->FetchRow())
	{
		$name = ($row["first_name"] . " " . $row["last_name"]);
		$conflict = true;
		$student_array = explode( "\n", stripslashes($students));
		for ($i = 0; $i < sizeof($student_array); $i++)
		{
			if ( trim(stripslashes($student_array[$i])) == trim(stripslashes($name)) )
			{	
				$conflict = false;
				$break;
			}
			
		}
		if ($conflict)
		{
			$cur_studentnum = $row["student_number"];
			$conflict_studentnum[] = $row["student_number"];
			$conflict_studentname[] = $name;

					
			$sql = ("
				SELECT DISTINCT student_number, first_name, last_name
				FROM student
				WHERE student_number='" . trim( $cur_studentnum) . "'
				");
			$result2 = $GLOBALS['dbh']->Execute($sql);
			$row2 = $result2->FetchRow();

			// Now we need to formulate the students email address.
		
			if(getStudentEmail($cur_studentnum))
                	{
                	        $conflict_studentemail[] = getStudentEmail($cur_studentnum);
                	}
			else
			{
				$noemail_studentnum[] = $cur_studentnum;
				$noemail_studentname[] = $row2["first_name"] . " " . $row2["last_name"];
			}
		}

		/*
		 Now we have a list of all the students that were signed up for
		 an interview, but have also been removed from the shortlist.
		 If this list contains any elements, we need to ask for confirmation
		 that the students removed from the shortlisted should really be
		 removed.
		*/

		if (($conflict_studentnum) && ($status != 'Conflict resolved') && ($status != 'Send Email'))
		{
			$status = 'Conflict found';
		}
	}
}

	?>
	<table width='100%'>
	<tr>
	<td>
	<h3>Interview - Edit  
	<?php 
	
	echo($job_code); 
	echo("</h3>");
	echo("</td><td align='right'>");
	echo("
	<form method='post' action='$PHP_SELF&amp;PDF=1&amp;job_id=$job_id'>

        <input type='hidden' name='employer_department_id' value='".$jobrow['employer_department_id']."' />          
        <input type='hidden' name='department_id' value='".$jobrow['department_id']."' />          
        <input type='hidden' name='employer_id' value='".$jobrow['employer_id']."' />
        <input type='hidden' name='job_id' value='".$job_id."' />
        <input type='hidden' name='contact_id' value='".$jobrow['contact_id']."' />
        <input type='hidden' name='job_code' value='".$jobrow['job_code']."' />
    
    <select name='selection'>
        <option value='view_company'>View this Company</option>
        ".(($jobrow['employer_department_id']) ? "<option value='view_department'>View this Job's Division</option>" : "")."
        ".(($jobrow['contact_id']) ? "<option value='view_contact'>View this Job's Contact</option>" : "")."
        <option value='view_applications'>View this Job's Applications</option>
        
        <option value='sign_up'>Sign up Students for this Interview</option> 
	    <option value='single'>Preview Schedule</option>
	    <option value='final'>Final Interview Schedule</option>
<option value='final_student'>Final Interview Schedule (Student Numbers)</option>
	    <option value='email_schedule'>E-mail Schedule to Employer</option>
	");
	
	$sql = ("
		SELECT DISTINCT emailed_newsgroup
		FROM interview
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	if (trim($row["emailed_newsgroup"]))
	{
		echo("<option value='email_students'>E-mail Shortlisted Students</option>");
	}

	echo("
	</select>
	&nbsp;
	<input type='image' src='misc/images/arrowg.gif' alt='go' />
	</form>
	</td></tr></table>
	");

$status = urldecode( $status );
switch ($status)
{
case "Conflict found":

	// A conflict has been found between students signed up for 
	// interviews already, and those removed from the shortlist.

        include('interview/edit/promptConflict.inc');
        break;

case "Send Email":
	
	// The email is ready to be sent.

	include('interview/edit/mail/send_students_email.inc');
	break;


case "Conflict resolved":

     	$sendEmail = true;


case "Continue":
	
	// The changes have been entered, and there was no conflict 
	// between students removed from shortlist and those already
	// signed up for an interview.

	if ( trim($interview->students) == '')
	{
		error("You must have at least one student shortlisted.");
		include('interview/edit/edit_students_form.inc');
	} else
	{
		include('interview/edit/update_students.inc');
	}	
	break;

case "Conflict edit":
default:
	$interview = new EditInterview('');
	$interview->loadValues($job_id, 'Students');
	if ($userlevel == OFFICE)
	{
		include('interview/edit/edit_students_form.inc');
	} else
	{
		include('misc/loguser.inc');
	}
	break;

} //switch
		






/***		ALL functions are located below this point		***/







  	
function confirmDialog( $URL, $message, $param=array() )
{
	echo("<form method='post' action='$URL'>");
	for( $i = 0; $i < count( $param ); $i++ )
	{
		echo("<input type='hidden' name='{$param[$i][0]}' value='{$param[$i][1]}' />");
	}
	echo("
		<table width='100%' cellpadding='2' cellspacing='0' class='row1' border='1'><tr><td>
		<table width='100%' cellpadding='4' cellspacing='0'>
		<tr>
			<td align='left'>$message</td></tr>
		<tr>
			<td>
				<input type='submit' name='subselection' value='Confirm' />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type='submit' name='subselection' value='Cancel' /></td></tr>
		</table>
		</td></tr></table>
		</form>");
}



?>
