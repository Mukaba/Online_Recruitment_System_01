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
 | Filename: apply.inc                                                          |
 +------------------------------------------------------------------------------+
 | Description: This is the main file that does all of the work for job         |
 | applications.                                                                |
 +------------------------------------------------------------------------------+

*/


// there are 2 spots to hit a button to view the app in pdf (only employer & admin side)
// so here, we find out which one it was, and shrink it down to a common one (students will just use view_apps1)

// make sure that things in admin, employer, and student are labelled properly, they're pretty much all the same
// with respect to the part of the form, (ex: selected_app_id)  - just make sure the names are the same

if ($PDF) {

	// setup the right one, these are for employers
	if ($view_apps1) {
		$view_who = $view_who1;
		$view_part = $part_of_app1;
	}
	elseif ($view_apps2) {
		$view_who = $view_who2;
		$view_part = $part_of_app2;
	}
	// for admin
	elseif ($view_all_apps) {
		$view_who = 'all';
		$view_part = 'whole';
	}
	elseif ($view_selected_apps) {
		$view_who = 'selected';
		$view_part = 'whole';
	}


	// main stuff - if the $output_pdf isn't set, then it doesn't do anything
	if ($view_who == 'all') {
		// set the array to something to use later on
		$selected_app_id = unpackObject(($all_apps));
		$output_pdf = true;
	}

	// only employers can flag jobs,
	elseif (($view_who == 'flagged') && (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER) || ($userlevel == ADMINEMPLOYER))) {

		$sql = ("
            SELECT DISTINCT a.application_id
			FROM applications AS a, employer_apps_flagged AS eaf
			WHERE a.application_id = eaf.application_id
			AND eaf.contact_id = '". ($userlevel == ADMINEMPLOYER ? $employer_contact_id_login : $contact_id) ."'
			AND a.job_id = '$job_id'
            AND a.app_status != '".APP_UNAVAILABLE."'
            ");

		$result = $GLOBALS['dbh']->Execute ($sql);

		$selected_app_id = array ();
		while ($row = $result->FetchRow()){
			$selected_app_id[] = $row["application_id"];
		}
		
		$output_pdf = true;
	}

	// find those that are shorltisted, shouldn't be too hard
	elseif (($view_who == 'shortlisted') && (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER) || ($userlevel == ADMINEMPLOYER))){

		$sql = ("
            SELECT DISTINCT a.application_id
			FROM applications AS a, students_shortlisted AS ss
			WHERE a.student_number = ss.student_number
			AND a.job_id = '$job_id'
			AND ss.job_id = '$job_id'
            AND a.app_status != '".APP_UNAVAILABLE."'
            ");

		$result = $GLOBALS['dbh']->Execute ($sql);

		$selected_app_id = array ();
		while ($row = $result->FetchRow())
			$selected_app_id[] = $row["application_id"];

		$output_pdf = true;
	}

	elseif ($view_who == 'selected') {
		$output_pdf = true;
	}
	
    // note: $output_pdf only applies to the applications themselves NOTHING else (ie: listing of applicants)
	if ($output_pdf) {
        if (!is_array ($selected_app_id)) 
        {
            if ($selected_app_id != '') 
            {
                $selected_app_id = array($selected_app_id);
            } 
            else 
            {
                $selected_app_id = array();
            }
        }

		// make sure there's something in the array
        if (sizeof ($selected_app_id) <= 0) 
        {
            unset ($PDF);
            $none_selected = true;
            if (($continue == 'jump_to_send' && $option == 'view') || ($continue == 'jump_to_view' && $option == 'send')) 
            {
                $continue = substr($continue, 0, 8) . $option;
            }
            if ($view_apps1 || $view_apps2) 
            {
                $continue = 'view_applicants';
            }
        } else {
			include("PDF/PDFconversion/ConvertToPDF.class");
			$application = new ConvertToPDF ();
			$need_toc = (sizeof ($selected_app_id) > 1 ? 'yes': 'no');

			switch ($view_part) {
				case 'letter':
					$application->convert_coverletter_applications ($selected_app_id, $need_toc );
					break;
				case 'resume':
					$application->convert_resume_applications ($selected_app_id, $need_toc );
					break;
				case 'whole':
				default:
					$application->convert_whole_applications ($selected_app_id, $need_toc );
			}
		}
	}

	else {

        // this will create the PDF printable list of the applications
        if ($continue == 'create_pdf_list') {

            include("PDF/PDFconversion/ConvertToPDF.class");
            $list = new ConvertToPDF ();
            $list->convert_application_list ($job_id);
        }

		else {
            unset ($PDF);
	    }
    }

} // if pdf 


// otherwise
else {
	include('applications/apply_header.inc');
    
	if ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
		include('applications/student/apply_student.inc');

	elseif ($userlevel == OFFICE || $userlevel == ADMINISTRATOR)
		include('applications/admin/apply_admin.inc');

	elseif ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
		include('applications/employer/apply_employer.inc');

    else error("You do not have access to this area.");

} // else

?>
