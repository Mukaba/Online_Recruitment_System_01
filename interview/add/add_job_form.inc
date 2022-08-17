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
 | Filename: add_job_form.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: *  the add job process takes a number of screens.  this code    |
 | determines which of these screens to display * if there were any critical    |
 | fields missing on one page, an error message will be displayed and the user  |
 | will be * required to fill them out before he/she can proceed. * * Function  |
 | References: AddInterview.class                                               |
 +------------------------------------------------------------------------------+

*/

$result = $GLOBALS['dbh']->Execute("select CURRENT_DATE");
$row = $result->FetchRow();
$current_date = $row["CURRENT_DATE"];
$error = 0;

$submit = stripSlashes($submit);

if( $student_num )
{   // short-listed student has conflicted name
	
	 update_duplicate("add");
}
elseif( $timetest == 1)
{
	//the user has started using the time calculator
	
	//restore the AddInterview object which containing previous forms' inputs
	
	$interview = unpackObject($interview);
	check_job_existence($interview->job_code);

	if( $error == 0 )
	{
		include('interview/add/timetest.inc');
	}
}   
elseif( $submit == '')
{
   //no form has been submited yet, so show first screen
   include('interview/add/add_job_form_1.inc');
}

if( $submit == 'Continue')
{
	/*
	 Split the e-mail employer field so we can check if it contains valid e-mail address.
	*/

	$address_list = preg_split("/\s*,\s*/", $employer_email);
	for ($i = 0; $i < sizeof($address_list); $i++)
	{
		if (!ereg('^[-!#$%&\'*+\\./0-9=?A-Z^_`a-z{|}~]+'.'@'.'[-!#$%&\'*+\\/0-9=?A-Z^_`a-z{|}~]+\.'.'[-!#$%&\'*+\\./0-9=?A-Z^_`a-z{|}~]+$',( trim($address_list[$i]))))
		{
			$bad_address_list = true;
		}
	}

	// We should have an interview already existant by the time we get here.
	$interview = unpackObject($interview);

	//information has been entered on the first screen
   	//check to make sure info from 1st screen was entered correctly, if so continue, otherwise, go back.
	
   	//save inputs by set corresponding attributes of the AddInterview object
   	$interview->set_value_1($job_code, $company_name, $employer_email, $students, $intdays, $interview_medium, $presentation, 
	$interview_type, $num_interviewers, $contact, $interview_place );
	$interview->no_time = $no_time;

	if( trim($job_code) == '' )
	{
		$error=1;
		error("You must enter a job code to add an interview");
   		include('interview/add/add_job_form_1.inc' );
	}

   	if (trim($company_name) == "" && $error != 1 ){
		$error = 1;
		error("You must enter a company name to add an interview");
		include('interview/add/add_job_form_1.inc');
	} 
	
	if((trim($employer_email) != '') && ($bad_address_list))
	{
		$error = 1;
		error("You must either include a valid employer e-mail address, or none at all.");
		include('interview/add/add_job_form_1.inc');
	}
	
	//ensure that if job_code already exists the interview is past, and in same group..
	$next_date = interviewDate($job_code);
	if ($next_date != "" && $next_date >= $current_date && $error != 1){
		
		$department_arr = department_groups($auth->department);

		for ($i = 0; $i < sizeof($department_arr); $i++)
                {
                        $dept_string .= ("'" . $department_arr[$i] . "',");
                }

                $dept_string = substr($dept_string, 0, -1);

		if (strlen($dept_string) < 2) { $dept_string = "''"; }

		$sql = "SELECT j.job_id, a.cancelled
			FROM interview_time as i, job_info as j, interview as a
			WHERE j.department_id IN (".$dept_string.") 
			AND int_date = '" . addslashes($next_date) . "' 
			AND a.job_id = i.job_id 
			AND i.job_id = j.job_id 
			AND j.job_id!='" . $interview->job_id . "'
			AND j.job_code = '" . addslashes(stripslashes($job_code)) . "'";
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount()) {
		$error = 1;

			$row =  $result->FetchRow();
			$job_id = $row["job_id"];
			$cancelled = $row["cancelled"];
       	 			
			$msg = "Interview <b class=instruct>$interview->job_code</b> has already been scheduled for ";
			$result= $GLOBALS['dbh']->Execute("select distinct int_date from interview_time where job_id=$job_id order by int_date");
			while( ($row = $result->FetchRow() ) )
	       	 	{
				$msg = $msg . "<b class=instruct>$row[0]</b>&nbsp;";
	         	}
			if ($cancelled)
			{
				$msg .= ("and cancelled.  <br /><br />Would you like to uncancel this interview?<br /><br />");
			}
			else
			{
   		   		$msg = $msg . ("<br /><br />Would you like to edit it?<br /><br />");
			}
	    	  	promptEdit( $msg, $job_id );
		
		}
	
	}
	if( $error == 0 )
   	{
		$result = $GLOBALS['dbh']->Execute("select cancelled from interview where job_id='$job_id'");
		$row = $result->FetchRow();
		$cancelled = $row[0];
	
		if( $cancelled == '1' )
		{
   			promptEdit("Interview <b>$interview->job_code</b> has been <b>cancelled</b>. Would you like to restore it?<br /><br />", $job_id);
	 	}
	}
   
	if( ($error == 0) && ( $error_message = check_student_name($interview->students) ) ){ $error = 1; error($error_message); include('interview/add/add_job_form_1.inc' ); }
	if( $error == 0 )
	{
		//if no error, continue to next form
		include('interview/add/add_job_form_2.inc' );
	}
}

if ( $submit == 'Send off the e-mail')
{
	include('interview/add/send_newsgroup_email.inc');
}

if ( $submit == 'Send off second e-mail')
{
	$email_employer = "pass_through";
	include('interview/add/send_newsgroup_email.inc');
}

if ( $submit == "Don't Send")
{
	notify("The interview has successfully been added, and the shortlisted students have been e-mailed.");
	include('interview/add/show_data.inc');
}

if( $submit == 'Calculate Times For Me')
{
	//restore the AddInterview object which containing previous forms' inputs
	$interview = unpackObject($interview);

	$wait_locations = array();
	for($i=0;$i<$interview->num_interviewers;$i++){
		$varName = "wait_room" . $i;
		$wait_locations[] = $$varName;

		/*
		 Code below add to keep track of the actual buildings that were chosen.
		*/

		$roomName = "wait_building" . $i;
		$waitbuildings[] = $$roomName;
	}

	$locations = array();
	for ($i=0;$i<$interview->num_interviewers;$i++){
		$varName = "room" . $i;
		$locations[] = $$varName;
		
		/*
		 Code below added to keep track of the actual buildings that were chosen.
		*/

		$buildName = "locations" . $i;
		$buildings[] = $$buildName;
	}
        
	check_job_existence($interview->job_code);
	
	$use_room_numbers = ${"use_room_number"} == "on" ? 1 : 0;

	if( $error == 0 )
 	{
			// save inputs of form 2
      			$interview->set_value_2($interviewers, $locations, $wait_locations, $notes, $staff_notes, $pres_date, $pres_time, $pres_len, $pres_loc, $pres_notes, $pres_viewable_by_all, $use_room_numbers );
			$interview->set_value_3($address1, $address2, $city,  $province, $country, $direction );

			// check interviewer's name
			for( $i = 0; $i < count( $interviewers ); $i++ )
			{
				if( trim( $interviewers[$i] ) == '' )
				{
					$error = 1;
					error( "The interviewer's name can not be empty" );
					break;
				}
			}

			/*** check presentation info ***/			
			if( $error == 0 && $interview->presentation == 1 )
			{
	      		if( (check_date($interview->pres_date) == 0) ) { $error =1; error("You must enter a valid presentation date, which must be in the format 'YYYY-MM-DD', and no earlier than today $current_date."); }
    	  		if( ($error == 0) && ( ($interview->pres_time = validTime($interview->pres_time)) == -1) ) { $interview->pres_time = ''; $error = 1; error("You must enter a valid presentation time"); }
	      		if( ($error == 0) && ($interview->pres_len = intval($interview->pres_len) ) <= 0  ) { $error = 1; error("You must enter a valid presentation length"); }
			}
			
			
			if( ($error == 0) && ( isOnCampus($interview->interview_place) || isPhone($interview->interview_medium) ) )
			{
				//check duplicated room ( not including T.B.A )
				if( isDuplicate( $interview->locations, $duplicate ) && $duplicate != '')				
				{		   			
		  			$error = 1;
		  			error("Duplicate interview rooms.");
				}				
			}	   			
			/*** off campus but not phone interview ***/			
			elseif( $error == 0 )
			{
	      		if( empty($interview->address1) ) { $error = 1; error("You must enter a street address"); }
	      		if( ($error == 0) && empty($interview->city)  ) { $error = 1; error("You must enter a city name"); }
	      		
	      		if( empty($interview->province)  ) { $interview->province = "B.C."; }
	      		if( empty($interview->country)  ) { $interview->country = "Canada"; }
			}						

      		if( $error == 1 )
	   			include('interview/add/add_job_form_2.inc');
			else 
   				include('interview/add/timetest.inc');       	//show the time calculator entry screen
			
   }
}

function check_job_existence($job_code)
{
/*
	global $error;
	
	assert( isset( $job_code ) );
	
   	if( db_check_existence( "job_info", "job_code", $job_code ) )
   	{
		$error = 1;
		$row = $GLOBALS['dbh']->GetRow("select job_id from job_info where job_code='$job_code'");
		$job_id = $row["job_id"];
   		
		promptEdit("Interview <b class=instruct>$job_code</b> has already been added.  Would you like to edit it?<br /><br />", $job_id);
   	}
*/   	
}

function promptEdit( $msg, $job_id )
{
	global $PHP_SELF;
	global $cancelled;

	instruct( $msg .  "<table cellspacing='2' cellpadding='2'><tr><td>
			  		<form method='post' action='$PHP_SELF'>
	    			  	<input type='hidden' name='select' value='edit' />
   	  			  		<input type='hidden' name='job_id' value='$job_id' />
						<input type='hidden' name='uncancel' value='true' />
   	  		  			<input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
   	  				</form>
 					</td>
 					<td>
		   	  		<form method='post' action='$PHP_SELF'>
		   	  		  <input type='hidden' name='select' value='' />
					  <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
   	  				</form></td></tr>
					</table>");   	
}
?>
