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
 | Filename: apply_admin.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This file displays all of the current applications for          |
 | students in the users department and gives them the option of applying and   |
 | removing their students (up until the applications have been activated).     |
 +------------------------------------------------------------------------------+

*/


/* something to keep in mind
if the $PDF variable is unset above, it continues through to the else statement below,
and since $continue is/was set to E-mail All by the drop down quickmenu it evaluates that.
So that's why above where unset($PDF) occures there's $continue = "view_applicants" right after
Then I also pass a flag so a notify() message appears tell the user to select someone.
the exception is 'default' above, since the time it hits this is when 'Add', 'Remove', etc. are selected.
*/

switch ($continue) {

	case 'jump_to_view':
	case 'jump_to_send':
		$option = 'send';
		assert($job_id);
		include('applications/admin/applicant_list.inc');
		break;
/*  :EXPIRY: June 31 2005 - Patrick - Removal of view pages
	case 'jump_to_view':
		$option = 'view';
		assert($job_id);
		include('applications/admin/applicant_list.inc');
		break;
*/
	case 'send_email':
		include ('applications/email/email.inc');
		break;

	case 'E-mail All':
		$esql = "SELECT DISTINCT application_id, student_number
			FROM applications
			WHERE job_id='" . $job_id . "'";
		$eresult = $GLOBALS['dbh']->Execute ($esql);

		while ($erow = $eresult->FetchRow())
			$which_students[] = $erow["student_number"];


	// drop through & check
	case 'E-mail Selected':
		// yeah, I know this may be redundant, but it's neccessary cause of the drop through
		if ($continue == 'E-mail Selected') {

			if (sizeof ($selected_app_id) > 0) {
				$ssql = "SELECT distinct application_id, student_number
					FROM applications
					WHERE application_id IN ('". implode ($selected_app_id, "','") ."')";
				$sresult = $GLOBALS['dbh']->Execute ($ssql);

				while ($srow = $sresult->FetchRow())
					$which_students[] = $srow["student_number"];
			}

			else {
				error ("No students have been selected to e-mail.");
				include ('applications/admin/applicant_list.inc');
				break;
			}
		}

		// now play with it to get it ready
		// make sure there's e-mail for everybody
		for ($i=0; $i < sizeof($which_students); $i++) {

			$check_email[] = array(
				"student_number" => $which_students[$i],
				"email" => getStudentEmail($which_students[$i]),
				"name" => getStudentName($which_students[$i])
				);

			if (!$check_email[$i]["email"]) 
				$noemail_students[] = $check_email[$i]["name"]; 
			else
				$email_students[] = $check_email[$i]["email"];
		}

		// create proper error message
		if (sizeof($noemail_students) > 1) {
			for ($i = 0; $i < (sizeof($noemail_students) - 1); $i++) 
				$email_warn .= $noemail_students[$i] . ", ";
			$email_warn .= "and " . $noemail_students[$i] . " do not have Netlink or login IDs. You 
				will need to enter an e-mail address for these students in the BCC field. Separate 
				e-mail addresses with commas if necessary.";
		}
		
		elseif (sizeof($noemail_students) == 1) {
			$email_warn = $noemail_students[0] . " does not have a Netlink or login ID. You will need 
				to enter an e-mail address for this student in the BCC field.  Separate e-mail 
				addresses with commas if necessary.";
		}

		if (is_array($email_students)) {
			$send_students = true;
			$sending_email_to = $email_students;
			include ('applications/email/email.inc');
		}
		else {
			error ("No students have been selected to e-mail.");
			include ('applications/admin/applicant_list.inc');
		}
		break;


	case 'E-mail Employer':
		$sql = "SELECT ji.receive_address as job_email, c.email as contact_email
			FROM job_info as ji
			LEFT JOIN contact as c ON c.contact_id = ji.contact_id
			WHERE ji.job_id='" . $job_id . "'";
		$result = $GLOBALS['dbh']->Execute ($sql);
		if ($row = $result->FetchRow()) {
			if ($row["job_email"] || ($row["job_email"] != "") || ($row["job_email"] != NULL))
				$sending_email_to[] = $row["job_email"];
			else
				$sending_email_to[] = $row["contact_email"];
		}
		$send_students = false;
		include ('applications/email/email.inc');
		break;

	
	case 'Employer Login':
		$result = $GLOBALS['dbh']->Execute("SELECT DISTINCT contact_id FROM job_info where job_id = '$job_id'");
		
		if ($row = $result->FetchRow())
			$contact_id = $row["contact_id"];
		
		$continue = 'contact_chosen';
		include ('contact/login_info/admin/make_login_info.inc');
		break;

	case 'view_applicants':
		/*
		 They've chosen a job, display the list of applicants in their department for this job.
		*/
		assert($job_id);
		include('applications/admin/applicant_list.inc');
		break;
		

	case 'Add an Application':
	case 'add_applicant':
		/*
		 They've chosen to add an application to a given job for a given student.
		*/

		assert($job_id);
		if ((trim($student_list) == "") && ($next_step == 'select_from_list')) {
			$next_step = 'enter_names';
			error ("You must enter student names");
		}
		include ('applications/admin/add_application.inc');
		break;
		

	case 'add_applicant_now':
		/*
		 They've chosen to add a new applicant for this job.  Save the applicant in the database,
		 and then display the list of applicants for the job again.
		*/

		assert($job_id);

		if ($apply_method == JOB_APP_METHOD_PAPER) {
			include ('applications/admin/insert_paper_app.inc');
        }

		elseif ($apply_method == JOB_APP_METHOD_ONLINE) {
			include ('applications/admin/insert_online_app.inc');
        }
		
		else {
			include ('applications/admin/insert_special_app.inc');
        }
	
		// this is incase they click the add more, it inserts them, then keeps going
		if ($add_more) {

			$unknown_array = unpackObject(($unknown_students));
			for ($z=0; $z < count($unknown_array); $z ++) {
				$unknown_notice .= ((count($unknown_array) < 1) ? "" : "<br /> &nbsp; &nbsp; ") .$unknown_array[$z];
				$unknown_text .= ((count($unknown_array) < 1) ? "" : "\n") .$unknown_array[$z];
			}
	
			include ('applications/admin/add_application.inc');
		}

		else
			include ('applications/admin/applicant_list.inc');
		break;


	case 'Remove Applications':
		/*
		 They've chosen to remove the selected student's applications from a given job.
		*/

		assert($job_id);

		if (!trim($selected_app_id)) {
			error("You must specify the application that you would like to remove.");
			include('applications/admin/applicant_list.inc');
		}
		else {
			if (is_string($selected_app_id))
				$selected_app_id = unpackObject(($selected_app_id));
			
			$selected_app_id = trim_array($selected_app_id);

			include('applications/admin/remove_applicant.inc');
		}
		break;
		

	case 'Activate Selected Applications':
		/*
		 They've chosen to make chosen students available again.
		*/

		assert($job_id);

		if (!trim($selected_app_id)) {
			error("You must specify the student that you would like to make available.");
			include('applications/admin/applicant_list.inc');
		}
		else {
			$selected_app_id = trim_array ($selected_app_id);

			include('applications/admin/activate_applications.inc');
			include('applications/admin/applicant_list.inc');
		}
		break;


	case 'Activate All Applications':
	        /*
	         They've chosen to make chosen students available again.
	         */

	        assert($job_id);

	   	$selected_app_id = unpackObject(($all_apps));

	    	include('applications/admin/activate_applications.inc');
		include('applications/admin/applicant_list.inc');
		break;


	case 'Deactivate Applications':

		assert($job_id);

	        if (!trim($selected_app_id)) {
			error("You must specify the student that you would like to make unavailable.");
			include('applications/admin/applicant_list.inc');
		}
		else {
			$selected_app_id = trim_array ($selected_app_id);

			include('applications/admin/deactivate_applications.inc');
			include('applications/admin/applicant_list.inc');
		}
		break;


	case 'Replace Applications':
		assert($job_id);

		if (!trim($selected_app_id)) {

			error("You must specify the application that you would like to replace.");
			include('applications/admin/applicant_list.inc');
		}

		else {
			$selected_app_id = trim_array ($selected_app_id);

			include('applications/admin/replace_applications.inc');
			include('applications/admin/applicant_list.inc');
		}
		break;
        
    case 'Shortlist All Students':
        $selected_app_id = unpackObject(($all_apps));  //use list of all applications, but first transfer data back into array format
    case 'Shortlist Selected Students':
        assert($job_id);                                         //make sure we have a job_id
        
        $existing_job_interview = true;                          //set to bypass 'interview/add/add.inc' state machine to the state we need
        $job_select = 'yes';                                     // <---same as above---->
        $jobs_for_interview = array($job_id);                    //create an array with the current job code
        
        if (!trim($selected_app_id))                             //If there are no student applications selected
        {
            error("You must specify the student(s) that you would like to shortlist.");
            include('applications/admin/applicant_list.inc');    //Redisplay this same page
        }
        else          
        {
            $selected_app_id = trim_array ($selected_app_id);    
            $arr_selected_students = array();                    //create a new array for list of student names
            foreach($selected_app_id AS $app_id)                 //for each application number
            {                                                    //find the last name and first name of the student applying
                $sql = "SELECT s.first_name, s.last_name
                        FROM student AS s, applications AS a
                        WHERE a.student_number = s.student_number
                        AND a.application_id = '$app_id'";
            
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $full_name = $row["first_name"]." ".$row["last_name"];  //store the name in this format: 'first_name last_name'
                array_push($arr_selected_students, $full_name);         //push the name onto the array  
            }
            
            $sql = "SELECT c.email
                    FROM contact AS c, job_info AS j
                    WHERE c.contact_id = j.contact_id
                    AND j.job_id = '$job_id'";
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $application_email = $row["email"];  
             
            include('interview/add/add.inc');                           //skip to the add interview page
        }
        break;

        
	default:
		/*
		 They've done nothing yet, so display the list of jobs that they may review applications for.
		*/
		include('applications/admin/jobs_search.inc');
		break;
	

}// end switch
?>
