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
 | Filename: edit_location.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This part of the form allows the user to edit any location      |
 | information pertaining to the job.                                           |
 +------------------------------------------------------------------------------+

*/
include('misc/isEmailSent.inc');

assert($job_id);
$jobresult = $GLOBALS['dbh']->Execute("
			SELECT contact_id, employer_department_id, department_id, employer_id, job_code
			FROM job_info 
			WHERE job_id='" . $job_id . "'");

$jobrow = $jobresult->FetchRow();
$job_code = $jobrow["job_code"];
		

// if a job has been selected...

if( isset($interview) )
{
   	$interview = unpackObject($interview);
}
?>

<table width='100%'>
  <tr>
    <td><h3>Interview - Edit <?php echo($job_code . "</h3>");?></td>
    <td align='right'>
       <form method='post' action="<?php echo $PHP_SELF . "&amp;PDF=1&amp;job_id=$job_id"?>">

        <input type='hidden' name='employer_department_id' value='<?php echo $jobrow['employer_department_id']; ?>' />          
       <input type='hidden' name='department_id' value='<?php echo $jobrow['department_id']; ?>' />          
       <input type='hidden' name='employer_id' value='<?php echo $jobrow['employer_id'];?>' />
       <input type='hidden' name='job_id' value='<?php echo $job_id;?>' />
       <input type='hidden' name='contact_id' value='<?php echo $jobrow['contact_id'];?>' />
       <input type='hidden' name='job_code' value='<?php echo $jobrow['job_code'];?>' />

	<select name='selection'>
        <option value='view_company'>View this Company</option>
       <?php echo (($jobrow['employer_department_id']) ? "<option value='view_department'>View this Job's Division</option>" :"" ); ?>
       <?php echo (($jobrow['contact_id']) ? "<option value='view_contact'>View this Job's Contact</option>" :"" ); ?>
        <option value='view_applications'>View this Job's Applications</option>

        <option value='sign_up'>Sign up Students for this Interview</option> 
		<option value='single'>Preview Schedule</option>
		<option value='final'>Final Interview Schedule</option>
        <option value='final_student'>Final Interview Schedule (Student Numbers)</option>
		<option value='email_schedule'>E-mail Schedule to Employer</option>

<?php

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

?>
	</select>
	&nbsp;
	<input type='image' src='misc/images/arrowg.gif' alt='go' />
       </form>
    </td>
  </tr>
</table>
<?php
$submit = urldecode( $submit );
switch ($submit)
{
case "Save Room Changes":
	if ($userlevel == OFFICE){
		include ( 'interview/edit/update_location_per_time.inc');
	}else{
		include('misc/loguser.inc');
	}
	break;

case "Send Waitroom Email":
	include('interview/edit/mail/send_waitroom_email.inc');
	break;

case "Save the changes":

	if ($userlevel == OFFICE){
		$locations = array();
		$wait_locations = array();
		for($i=0;$i<count($interview->interviewer_ids);$i++){
			$varName = "room" . $i;
			$locations[] = $$varName;
			$varName = "wait_room" . $i;
			$wait_locations[] = $$varName;
		}

		$old_interview_place = $interview->interview_place;
		$interview->updateLocation($interview_place, $use_room_number);

		if (isOnCampus($interview->interview_place) || isPhone($interview->interview_medium)){
			$interview->updateRoomLocation($locations, $wait_locations);
		}else{
			$interview->updateOffCampusLoc($address1, $address2, $city, $province, $country, $direction);
		}
		include( 'interview/edit/update_location.inc' );
        }else{
		include('misc/loguser.inc');
	}
	break;
case  "Multiple Rooms":
	$interview = new EditInterview('');
	$interview->loadValues($job_id, 'Location');
	if ($userlevel == OFFICE){
		include('interview/edit/edit_location_per_time_form.inc');
	}else{
		include('misc/loguser.inc');
	}
	break;
default:

         // ...show the job form
         // get the data from the database

	$interview = new EditInterview('');
	$interview->loadValues($job_id, 'Location');
	if ($userlevel == OFFICE)
	{
		include('interview/edit/edit_location_form.inc');
	} else
	{
		include('misc/loguser.inc');
	}
	break;
}

//   $interview->debug();

/***		ALL functions are located below this point		***/


?>
