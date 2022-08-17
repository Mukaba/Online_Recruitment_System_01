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
 | Description: This page is the first one loaded when someone chooses to add   |
 | a job.  It decides which form to load, or whether or not it is time to save  |
 | the inputs into the database.  It also runs the validation checking on the   |
 | inputs given.                                                                | 
 +------------------------------------------------------------------------------+

*/

include('job_descriptions/add_job/AddJob.class');

$current_date = date('Y-m-d');

$error = 0;

if ($userlevel == ADMINEMPLOYER)
{
	$sql = "SELECT employer_id 
            FROM contact_employer
            WHERE contact_id='".$employer_contact_id_login."'";
	$result = $GLOBALS['dbh']->Execute($sql);

    $row = $result->FetchRow();
    $employer_id = $row['employer_id'];
}

if ($userlevel == EMPLOYER || $userlevel == ADMINEMPLOYER)
{
	/*
	 If an employer has logged in, we need to make sure that they are associated with a company
	 before we allow them to add jobs.
	*/
	$sql = "SELECT company_name
            FROM employer_company
            WHERE employer_id='".$employer_id."'";
    $result = $GLOBALS['dbh']->Execute($sql);

    /*
	if (!$result->RecordCount() && $userlevel != ADMINEMPLOYER)
	{
        $continue = 'Invalid Employer';
	}
	else
	{
		$row = $result->FetchRow();
	
		if (!trim($row["company_name"]))
		{
			$continue = 'Invalid Employer';
		}
	}
    */
    if (!($row = $result->FetchRow()) && $userlevel != ADMINEMPLOYER)
    {
        $continue = 'Invalid Employer';
    }
}

if (($continue == '' || $java_reload) && (($userlevel != EMPLOYER) && ($userlevel != ADMINEMPLOYER)))
{
	/*
	 No form has been submited yet, or the page is being refreshed because a company was chosen, 
	 so show first screen.  This only needs to be done if the user is an ADMINISTRATOR or OFFICE
	 member.  Otherwise, it's an employer we're dealing with, and we already have the information
	 that we need.
	*/

	if ($unset_company)
	{
		unset($company_website);
		unset($company_description);
	}
	if ($unset_department)
	{
		unset($department_website);
		unset($department_description);
	}
	if ($unset_contact)
	{
		unset($contact_phone);
		unset($contact_cellphone);
        unset($contact_email);
		unset($contact_fax);
	}

	$company_website = removeSlashes($company_website);
	$company_description = removeSlashes($company_description);
	$department_website = removeSlashes($department_website);
	$department_description = removeSlashes($department_description);

	include('job_descriptions/add_job/add_job_form_a.inc' );
}


elseif ($continue == 'show_admin_templates')
{
	// They've chosen the company, now give them the option of choosing a template job.

	include('job_descriptions/add_job/staff_job_choice.inc');
}


else if ((!$java_reload && $continue == 'level1') || ((($userlevel == EMPLOYER ) || ($userlevel == ADMINEMPLOYER)) && !$job && !$continue))
{
	/*
	 We've received confirmation from the first input screen.  Now we want to begin checking the inputs passed.
	 First thing to do is pull out the appropriate information pertaining to the contact, employer, and (maybe)
	 division that were chosen, and save them in the job object.  
	*/
	
	if ($userlevel == EMPLOYER || $userlevel == ADMINEMPLOYER)
	{
		if (!$employer_start_scratch)
		{
			/*
			 Check and see if they have jobs in the database from the past.  If they do give them the option of using one
			 of those jobs as a template.
			*/
			$nextTermYear = getNextTermYear();
		
			$sql = "SELECT job_id
				    FROM job_info
				    WHERE (status = ".CLOSED." OR status = ".CANCELLED.")
				    AND employer_id ='".$employer_id."'";
			$result = $GLOBALS['dbh']->Execute($sql);
			
			/*
			 They've got jobs in the past, and they haven't yet filled anything out.  Set the variable that will later
			 give them the option of using one of their old jobs as a template.
			*/
			if ($result->RecordCount())
			{
				$load_special_employer_page = true;
			}
		}

	}

	if ($load_special_employer_page)
	{
		/*
		 Load up the page that will allow the employer to choose whether or not they would like to choose a job as a template.
		*/

		include('job_descriptions/add_job/employer_job_choice.inc');
	}
	else
	{
		// Save the updated values into the database

		$sql = ("
			UPDATE employer_company
			SET website='" . $company_website . "',
			company_description='" . $company_description . "'
			WHERE employer_id='" . $employer_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		if ($department_id != 'none')
		{
			$sql = ("
				UPDATE employer_department
				SET department_website='" . $department_website . "',
				department_description='" . $department_description . "'
				WHERE department_id='" . $department_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}

// this was causing problems so I took it out (it would blank out anything that was there) - Brenton		
    if($visited_job_form_a){
        $sql = ("
                UPDATE contact
			    SET email='" . $contact_email . "',
                phone='" . $contact_phone . "',
                cellphone='" . $contact_cellphone . "',
                fax='" . $contact_fax . "'
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
    }
		// Pull values out and assign them.

		$sql = ("
			SELECT company_name, website, company_description
			FROM employer_company
			WHERE employer_id='" . $employer_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	
		$row = $result->FetchRow();
		
		$company_name = $row["company_name"];
		$company_website = $row["website"];
		$company_description = $row["company_description"];
	
		if ($department_id && $department_id != 'none')
		{
			$sql = ("
				SELECT department_name, department_website, department_description
				FROM employer_department
				WHERE department_id='" . $department_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			
			$row = $result->FetchRow();
			
			$department_name = $row["department_name"];
			$department_website = $row["department_website"];
			$department_description = $row["department_description"];
		}
		$sql = ("
			SELECT CONCAT(first_name, ' ', last_name) AS name, cellphone,  email, phone, fax
			FROM contact
			WHERE contact_id='" . $contact_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	
		$row = $result->FetchRow();
		
		$contact_name = $row["name"];
		$contact_email = $row["email"];
		$contact_phone = $row["phone"];
        $contact_cellphone = $row["cellphone"];
		$contact_fax = $row["fax"];
	
		$job = new AddJob('');
		$job->set_value_3($company_name, $company_description, $company_website, $employer_id, $department_name,
		$department_description, $department_website, $department_id, $contact_name, $contact_phone, $contact_cellphone, $contact_email, 
		$contact_fax, $contact_id);
	
		// Now we want to include the second screen, and allow the user to start entering inputs for the job.
	
		if ($error == '0')
		{
			include('job_descriptions/add_job/add_job_form_b.inc');
		}
	}
}
elseif ($continue == 'template_job_chosen')
{
	/*
	 They've chosen a template job.  We need to use EditJob to draw the template job's values out of the database,
	 insert the new contact and employer information, set all of this information into an AddJob class, and then load
	 up the second job screen.  Everything else is the same from there on in.
	*/
    if($job_id)
    {

        include('job_descriptions/edit_job/EditJob.class');

        $job = new EditJob('');
        $job->loadValues($job_id);

        // Put these values into an AddJob class.

        $job = new AddJob($job);

        $sql = ("
                SELECT company_name, website, company_description
                FROM employer_company
                WHERE employer_id='" . $job->employer_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $row = $result->FetchRow();

        $company_name = $row["company_name"];
        $company_website = $row["website"];
        $company_description = $row["company_description"];

        if ($job->department_id && $job->department_id != 'none')
        {
            $sql = ("
                    SELECT department_name, department_website, department_description
                    FROM employer_department
                    WHERE department_id='" . $job->department_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $row = $result->FetchRow();

            $department_name = $row["department_name"];
            $department_website = $row["department_website"];
            $department_description = $row["department_description"];
        }
        $sql = ("
                SELECT CONCAT(first_name, ' ', last_name) AS name, email, phone, cellphone, fax
                FROM contact
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $row = $result->FetchRow();
        if(!$contact_id)
        {
            $contact_id = $job->contact_id;
        }
        $department_id = $job->department_id;
        $employer_id = $job->employer_id;
        $contact_name = $row["name"];
        $contact_email = $row["email"];
        $contact_phone = $row["phone"];
        $contact_cellphone = $row["cellphone"];
        $contact_fax = $row["fax"];

        // Put the contact information into the AddJob class.

        $job->set_value_3($company_name, $company_description, $company_website, $employer_id, $department_name,
                $department_description, $department_website, $department_id, $contact_name, $contact_phone, $contact_cellphone, $contact_email,
                $contact_fax, $contact_id);
        include('job_descriptions/add_job/add_job_form_b.inc');
    }
    else
    {
        error("You must select a job to repost");
	    include('job_descriptions/add_job/staff_job_choice.inc');
    }

}
elseif ($continue == 'level2')
{
	// Eliminate fields like salary_period here before validation check if necessary.

	$error = 0;

	$job = unpackObject($job);

	$curterm = getCurrentTermID();
	$tempdate = explode("-", $current_date);
	
	// Remove all of the blank elements in $apply_departments and $workterm_lengths.

	$workterm_lengths = trim_array($workterm_lengths);
	$apply_departments = trim_array($apply_departments);

	if (trim($apply_method == JOB_APP_METHOD_PAPER))
	{
		$receive_address = '';
	}

	if ((trim($salary_amount1) == '') && (trim($salary_amount2) == ''))
	{
		$salary_period = '';
	}
	else
	{
		if (trim($salary_amount1) == '' && isValidSalary(trim($salary_amount2)))
		{
			$salary_amount1 = $salary_amount2;
		}
		if (trim($salary_amount2) == '' && isValidSalary(trim($salary_amount1)))
		{
			$salary_amount2 = $salary_amount1;
		}
		$salary_amount1 = normalizeSalary($salary_amount1);
		$salary_amount2 = normalizeSalary($salary_amount2);
	}

	if ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
	{
		// If an employer is adding the job, set the status to NOT CHECKED
		$admin_status = NOT_CHECKED_EMPLOYER;
		$status = NOT_CHECKED;
	}
	else
	{
		$sql = ("
			SELECT job_status_id
			FROM admin_status
			WHERE status_id='" . $admin_status . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$status = $row["job_status_id"];
	}

	$job->set_value_1($position_title, $job_code, $employer_code, $num_positions, $apply_method, $receive_address, $country, $provstate,
	$region, $city, $salary_amount1, $salary_amount2, $salary_period, $start_term, $start_year, $start_date, $start_date_date, $start_date_month,
    $start_date_year, $end_date, $end_date_date, $end_date_month, $end_date_year, $closing_date, $closing_time, 
	$workterm_lengths, $workterm_hours, $other_postings, $industry, $status, $admin_status, $min_academic, $min_workterms, $apply_departments);

	// Begin validation of inputs here.

	if (($status != NOT_CHECKED) && (validDate($job->closing_date) == ''))
	{
		$error = 1;
		error("You must enter a closing date to add a job with status other than <b>NOT CHECKED</b>.");
		include('job_descriptions/add_job/add_job_form_b.inc');
	}

    elseif ($job->num_positions == '' || !ereg("^([0-9])+$", $job->num_positions))
    {
        $error = 1;
        error("You must enter a valid number of positions.");
        include('job_descriptions/add_job/add_job_form_b.inc');
    }
    elseif (trim($job->position_title) == '')
	{
		$error = 1;
		error("You must enter a position title to add a job.");
		include('job_descriptions/add_job/add_job_form_b.inc');
	}
    elseif (!isValidEmail($job->receive_address) && (($job->apply_method == JOB_APP_METHOD_ONLINE)))
	{
        $error = 1;
        error("If you wish to receive online aplications, you must include a valid e-mail address.");
        include('job_descriptions/add_job/add_job_form_b.inc');
    }
	elseif ($job->city == '')
	{
		$error = 1;
		error("You must include a city name if you wish to add a job.");
		include('job_descriptions/add_job/add_job_form_b.inc');
	}
	elseif ((!isValidSalary($job->salary_amount1) && trim($job->salary_amount1 != '')) || (!isValidSalary($job->salary_amount2) 
	&& trim($job->salary_amount2 != '')))
	{
		$error = 1;
		error("You must either include a valid salary amount/range, or none at all.");
		include('job_descriptions/add_job/add_job_form_b.inc');
	}
	elseif (!$workterm_lengths)
	{
		$error = 1;
		error("You must specify at least one workterm length in order to add a job.");
		include('job_descriptions/add_job/add_job_form_b.inc');
	}
	elseif ($workterm_hours != '' && !ereg("(.*[0-9]+.*)+", $workterm_hours))
	{
		$error = 1;
		error("You must specify a valid number of workterm hours, or none at all.");
		include('job_descriptions/add_job/add_job_form_b.inc');
	}
    elseif ((check_date($job->closing_date) == 0) && ($job->status != NOT_CHECKED && $job->admin_status != CHECKED_WAITING_DATE))
    {
        $error = 1;                                                                                                                              
        error("To enter a job with status other than <b>NOT CHECKED</b>, you must provide                                                        
                a valid closing date in the form YYYY-MM-DD no earlier than today's date $current_date.");                                               
        include('job_descriptions/add_job/add_job_form_b.inc');                                                                                  
    }
	elseif ((validTime($job->closing_time) == -1) && ($userlevel == OFFICE))
    {
        $error = 1;
        error("To enter a job, you must provide a valid closing time in 24-hour format.");
        include('job_descriptions/add_job/add_job_form_b.inc');
    }

    $error_msg = NULL;
    $job->start_date = NULL;
    $job->end_date = NULL;

    if ($job->start_date_date || $job->start_date_month || $job->start_date_year)
    {
        if (!$job->start_date_month)
        {
            $error_msg .= "When entering a start date for your job, the month field is required. ";
        }
        elseif (!$job->start_date_year)
        {
            $error_msg .= "When entering a start date for your job, the year field is required. ";
        }
        else
        {
            if (!$job->start_date_date)
            {
                $job->start_date_date = "00";
            }
            $job->start_date = $job->start_date_year."-".str_pad($job->start_date_month,2,0,STR_PAD_LEFT)."-".str_pad($job->start_date_date,2,0,STR_PAD_LEFT);

            if(!isValidDate2($job->start_date))
            {
                $error_msg .= "The given start date is not a valid date, please re-enter. ";
            }
        }
    }
    if ($job->end_date_date || $job->end_date_month || $job->end_date_year)
    {
        if (!$job->end_date_month)
        {
            $error_msg .= "When entering an end date for your job, the month field is required. ";
        }
        elseif (!$job->end_date_year)
        {
            $error_msg .= "When entering an end date for your job, the year field is required. ";
        }
        else
        {
            if (!$job->end_date_date)
            {
                $job->end_date_date = "00";
            }

            // The valid date function requires dates with the format YYYY-MM-DD, so it doesn't like dates that are like 2004-4-4, so we have to pad
            // the date to look like 2004-04-04

            $job->end_date = $job->end_date_year."-".str_pad($job->end_date_month,2,0,STR_PAD_LEFT)."-".str_pad($job->end_date_date,2,0,STR_PAD_LEFT);

            if(!isValidDate2($job->end_date))
            {
                $error_msg .= "The given end date is not a valid date; please re-enter. ";
            }
        }
    }

    if ($job->end_date && !$job->start_date)
    {
        $error_msg .= "A start date for the job is required because you have provided an end date.";
    }

    if (!$error_msg)
    {
        if ($job->start_date)
        {
            // If a start date is given, then we will override the start term and year that the user has given, as the start date is more accurate. 
            $arr_term_year = getTermYearWithGracePeriod($job->start_date);
            $job->start_term = $arr_term_year["term"];
            $job->start_year = $arr_term_year["year"];
        }

        // If we have a start date and no end date
        // check what term we're in using start_date + grace period
        // use that term's last day as the end date
        if ($job->start_date && !$job->end_date)
        {
            $job->end_date = getTermYearEndDate($job->start_term,$job->start_year);
            notify("No end date was provided for this job, so the last day of the term (".formatSpecialZeroDate($job->end_date).") will be used as the end date.");
        }

        if (isValidDate2($job->start_date) && isValidDate2($job->end_date) && count_days($job->start_date, $job->end_date) < HISTORY_MIN_DAYS_FOR_WT)
        {
            notify("The start date and end date are less than ".HISTORY_MIN_DAYS_FOR_WT." days apart. Should the workterm be longer?");
        }
    }

    //:TODO: This should not be hard coded like this. 
	if ( ($tempdate[0] == $job->start_year) && (($curterm == 1 && $job->start_term == 2) || 
	($curterm == 1 && $job->start_term == 3) || ($curterm == 3 && $job->start_term == 2)))
	{
		$error = 1;
		error("You must enter a start of workterm that is in the future or present.");
		include('job_descriptions/add_job/add_job_form_b.inc');
	}

    if ($error_msg)
    {
        $error = 1;
        error($error_msg);
        include('job_descriptions/add_job/add_job_form_b.inc');
    }

	// End of validation for first form.

	if ($error == 0)
	{
		include('job_descriptions/add_job/add_job_form_c.inc');
	}
}
elseif ($continue == 'level3')
{
	// Remove all of the blank elements from $experience_levels.
	
	$job = unpackObject($job);

	$date_posted = $current_date;

	$job_disciplines = trim_array($job_disciplines);
	if (!$job_disciplines)
	{
		$job_disciplines[] = ALL_DISCIPLINES;
	}
	
	$job->set_value_2($host_department, $job_disciplines, $job_description, $skills_required, $special_requirements
            , $comments, $admin_comments, $for_student_comments, $date_posted, $entered_by, $special_instructions
            , $special_instr_button, $show_special_instr_button, $displayname_on_coverltr);

	// Begin checking second set of inputs from here.
	
	if ($job->job_description == '')
	{
		$error = 1;
		error("You must write a description for this job.");
		include('job_descriptions/add_job/add_job_form_c.inc');
	}
	elseif ($job->skills_required == '')
	{
		$error = 1;
		error("You must specify some skills required for this job.");
		include('job_descriptions/add_job/add_job_form_c.inc');
	}
	elseif ( ($job->apply_method == JOB_APP_METHOD_SPECIAL) && ($job->show_special_instr_button == 1) && ( trim($job->special_instr_button) == ''))
	{
		$error=1;
		error("The Special Instruction button may not be left blank if you wish to display it.");
		include('job_descriptions/add_job/add_job_form_c.inc');
	}
	
	if ($job->status != POSTED)
	{
		$job->date_posted = '';
	}

    /*
       If they've set no applicable departments, set all departments as applicable.  We want to do this
       here, because if we did it on the previous screen, and they got an error, the page would be reloaded
       with all of the checkboxes filled, and would potentially confuse the user.
     */              

    if (!($job->apply_departments))
    {
        $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            $job->apply_departments[] = $row["department_id"];
        }
    }

	foreach ($job->apply_departments AS $ad)
	{
		$disc_depts .= ("'" . $ad . "',");
	}
	$disc_depts = substr($disc_depts, 0, -1);
	if (!$disc_depts)
	{
		$disc_depts = ("''");
	}

	$sql = ("
		SELECT discipline_id
		FROM department_discipline_join
		WHERE department_id IN (" . $disc_depts . ")
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (sizeof($job->job_disciplines) == $result->RecordCount())
	{
		$job->job_disciplines = '';
		$job->job_disciplines[] = ALL_DISCIPLINES;
	}

	if ($error == 0)
	{
		include('job_descriptions/add_job/confirm_inputs.inc');
	}
}	

elseif ($continue == 'add_job_confirmed')
{
	// They've confirmed that they'd like to add this job regardless of similarities
	// between another one at the same company

	$job = unpackObject($job);

	include('job_descriptions/add_job/add_job.inc');
}

elseif ($continue == 'level4')
{
    /*
     We've just received some company, division, or contact information.
     Insert this into the appropriate database, retrieve the id number assigned 
     to this information, and then reload add_job_form_a.inc
     */

    $error = 0;

    if ($add_this_object == 'employer')
    {

        // Replace "s with 's
        $company_name = preg_replace("/\"/", "'", $company_name);

        // Make sure we have a company name.

        if (trim($company_name) == '')
        {
            $error = 1;
            error("You must specify a name for the new company.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif (!$add_confirmed)
        {
            // Check for similar company names in the database.

            $check_name = $company_name[0] . $company_name[1] . $company_name[2];

            $sql = ("
                    SELECT DISTINCT company_name
                    FROM employer_company
                    WHERE company_name LIKE '" . addslashes($check_name) . "%'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $error = 1;

                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ("<b class=instruct>".$row["company_name"]."</b>");
                    $msg .= (" is an exisiting company in the database with a name similar to");
                    $msg .= (" the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this company to the database?<br /><br />");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $company_names_array[] = $row["company_name"];
                    }
                    for ($i = 0; $i < (sizeof($company_names_array) - 1); $i++)
                    {
                        $msg .= ("<b class=instruct>".$company_names_array[$i] . "</b>, ");
                    }
                    $msg .= ("and <b class=instruct>" . $company_names_array[$i] . "</b> ");
                    $msg .= ("are companies with similar names to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this company to the database?<br /><br />");
                }

                promptAddCompany($msg, $company_name, $company_website, $company_description);
            }
        }

        if ($error == 0)
        {
            $sql = ("
                INSERT INTO employer_company
                (company_name, website, company_description, entered_by, entered_on, company_display)
                VALUES
                (
                 '" . addslashes(removeSlashes(trim($company_name))) . "',
                 '" . addslashes(removeSlashes(trim(normalizeWebsite($company_website)))) . "',
                 '" . addslashes(removeSlashes(trim($company_description))) . "',
                 '" . addslashes($auth->login) . "',
                 CURRENT_DATE,
                 '1'
                )
                ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                    SELECT LAST_INSERT_ID() AS employer_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $employer_id = $row["employer_id"];

            // Now we need to update the status for this new company, for every department at our organization.
            $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                updateCompanyStatus($employer_id, $row["department_id"]);
            }

            $order = $company_name[0];
            include('job_descriptions/add_job/add_job_form_a.inc');
        }
    }
    if ($add_this_object == 'department')
    {

        // Replace "s with 's
        $department_name = preg_replace("/\"/", "'", $department_name);

        // Make sure we have a division name.

        if (trim($department_name) == '')
        {
            $error = 1;
            error("You must specify a name for the new division.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif (!$add_confirmed)
        {

            /*
             Check in the database for any divisions at this company with similar names, and ask the user if they are
             sure they want to add THIS division.
             */

            $check_name = $department_name[0] . $department_name[1] . $department_name[2];

            $sql = ("
                    SELECT a.department_name, b.company_name
                    FROM employer_department AS a, employer_company AS b
                    WHERE a.department_name LIKE '" . addslashes($check_name) . "%'
                    AND a.employer_id='" . $department_company_id . "'
                    AND b.employer_id='" . $department_company_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $error = 1;
                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ("<b class=instruct>".$row["department_name"]."</b>");
                    $msg .= (" is an exisiting division at <b class=instruct>" . $row["company_name"] . "</b> already in the database ");
                    $msg .= (" with a name similar to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this division to the database?<br /><br />");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $department_names_array[] = $row["department_name"];
                    }
                    for ($i = 0; $i < (sizeof($department_names_array) - 1); $i++)
                    {
                        $msg .= ("<b class=instruct>".$department_names_array[$i] . "</b>, ");
                    }
                    $msg .= ("and <b class=instruct>" . $department_names_array[$i] . "</b> ");
                    $msg .= ("are divisions at <b class=instruct>" . $row["company_name"] . "</b> already in the database ");
                    $msg .= ("with similar names to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this division to the database?<br /><br />");
                }

                promptAddDepartment($msg, $department_name, $department_website, $department_description, $employer_id);
            }
        }

        if ($error == 0)
        {
            /*
             Save into the database.  When done, update the employer_company table for the
             parent company, so that in the future we know this company has divisions.
             */

            $sql = ("
                    INSERT INTO employer_department
                    (department_name, department_website, department_description, 
                     entered_by, entered_on, employer_id)
                    VALUES
                    (
                     '" . addslashes(removeSlashes(trim($department_name))) . "',
                     '" . addslashes(removeSlashes(trim(normalizeWebsite($department_website)))) . "',
                     '" . addslashes(removeSlashes(trim($department_description))) . "',
                     '" . addslashes($auth->login) . "',
                     CURRENT_DATE,
                     '" . addslashes(removeSlashes($employer_id)) . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                    SELECT LAST_INSERT_ID() AS department_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $department_id = $row["department_id"];

            // Now we need to update the status for this new division, and its company, for every department at our organization.
            $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                updateDivisionStatus($department_id, $row["department_id"]);
                updateCompanyStatus($employer_id, $row["department_id"]);
            }

            $order = $company_name[0];
            include('job_descriptions/add_job/add_job_form_a.inc');
        }
    }
    if ($add_this_object == 'contact')
    {
        // Replace "s with 's
        $contact_first_name = preg_replace("/\"/", "'", $contact_first_name);
        $contact_last_name = preg_replace("/\"/", "'", $contact_last_name);

        // Validate the user inputs here.

        if (trim($contact_first_name) == '')
        {
            $error = 1;
            error("You must supply a first name for the new contact.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif (trim($contact_last_name) == '')
        {
            $error = 1;
            error("You must supply a last name for the new contact.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif ((trim($contact_email) != '') && !(isValidEmail($contact_email)))
        {
            $error = 1;
            error("You must include a valid set of e-mail addresses, or none at all.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif ((trim($contact_phone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact_phone)))))
        {
            $error = 1;
            error("You must include a valid phone number with an area code, or none at all.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif ((trim($contact_cellphone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact_cellphone)))))
        {
            $error = 1;
            error("You must include a valid cellphone number with an area code, or none at all.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif ((trim($contact_fax) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact_fax)))))
        {
            $error = 1;
            error("You must include a valid fax number with an area code, or none at all.");
            include('job_descriptions/add_job/add_job_form_d.inc');
        }
        elseif (!$add_confirmed)
        {

            /*
             Check in the database for any contacts at this company/division with similar names, and ask the
             user if they are sure they want to add THIS contact.  First check if we have a matching phone number.
             If we do, then display all contacts that share the last initial as this one, and check to make sure
             the user wishes to add this contact.  If no matching phone number, then show the user all contacts
             with similar last names to this contact and prompt for confirmation.
             */

            $contact_last_name = removeSlashes(trim($contact_last_name));

            $check_name = $contact_last_name[0] . $contact_last_name[1] . $contact_last_name[2];
            $check_initial = $contact_last_name[0];



            if (trim($contact_phone) != '')
            {
                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.company_name
                        FROM contact AS a, employer_company AS b, contact_employer AS c
                        WHERE a.last_name LIKE '" . $check_initial . "%'
                        AND a.phone='" . addslashes(removeSlashes(trim($contact_phone))) . "'
                        AND c.employer_id='" . $employer_id . "'
                        AND a.contact_id=c.contact_id
                        AND b.employer_id=c.employer_id
                        ");
                $result  = $GLOBALS['dbh']->Execute($sql);
            }

            if (!($result->RecordCount()) || (trim($contact_phone) == ''))
            {
                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.company_name
                        FROM contact AS a, employer_company AS b, contact_employer AS c
                        WHERE a.last_name LIKE '" . addslashes($check_name) . "%'
                        AND c.employer_id='" . $employer_id . "'
                        AND a.contact_id=c.contact_id
                        AND b.employer_id=c.employer_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }

            if ($result->RecordCount())
            {
                $error = 1;
                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ("<b class=instruct>".$row["name"]."</b>");
                    $msg .= (" is an exisiting contact at <b class=instruct>" . $row["company_name"] . "</b> already in the database ");
                    $msg .= (" with a name similar to the one you are entering.");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $contact_names_array[] = $row["name"];
                    }
                    for ($i = 0; $i < (sizeof($contact_names_array) - 1); $i++)
                    {
                        $msg .= ("<b class=instruct>".$contact_names_array[$i] . "</b><br />");
                    }
                    $msg .= ("<b class=instruct>".$contact_names_array[$i] . "</b><br /><br />");
                    $msg .= ("are contacts at <b class=instruct>" . $row["company_name"] . "</b> already in the database ");
                    $msg .= ("with similar names to the one you are entering.");
                }
                $msg .= ("<br /><br />Are you sure you want to add this contact to the database?<br /><br />");
                promptAddContact($msg, $contact_first_name, $contact_last_name, $contact_phone, $contact_cellphone, $contact_fax, $contact_email, $employer_id, $department_id);

            }

        }

        if ($error == 0)
        {
            // If there is no division, then the department_id will == 'none'.  Set it to nothing.

            if ($department_id == 'none')
            {
                $department_id = '';
            }
            $sql = ("
                    INSERT INTO contact
                    (type, first_name, last_name, email, phone, cellphone, fax)
                    VALUES
                    (
                     'employer',
                     '" . addSlashes(removeSlashes(trim($contact_first_name))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_last_name))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_email))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_phone))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_cellphone))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_fax))) . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                    SELECT LAST_INSERT_ID() AS contact_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $contact_id = $row["contact_id"];

            $sql = ("
                    INSERT INTO contact_employer
                    (contact_id, department_id, employer_id, entered_on, entered_by)
                    VALUES
                    (
                     '" . $contact_id . "',
                     '" . addSlashes(removeSlashes(trim($department_id))) . "',
                     '" . addSlashes(removeSlashes(trim($employer_id))) . "',
                     CURRENT_DATE,
                     '" . addSlashes($auth->login) . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            // Now we need to update the status for this new contact, and its division and company, for every department at our organization.
            $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                // Save the contact with an initial status of POTENTIAL_LEAD.  This will change when this job is posted.
                updateContactStatus($contact_id,$row['department_id'],null,null);
            }

            $sql = ("
                    SELECT DISTINCT company_name
                    FROM employer_company
                    WHERE employer_id='" . $employer_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $company_name = $row["company_name"];

            // If the department_id is blank, set it back to none.

            if (!$department_id)
            {
                $department_id = 'none';
            }

            $order = $company_name[0];
            $enable_save = 'true';
            include('job_descriptions/add_job/add_job_form_a.inc');
        }
    }
}

elseif ($continue == 'add_new_entity')
{
	/*
	 The user has chosen to add a new company, division, or contact.  Load
	 the appropriate form.
	*/
	
    if ($add_this_object == 'contact' && (!$department_id || $department_id == 'none'))
    {
        error("If you wish to add a new contact, you must first choose a division.");
        include('job_descriptions/add_job/add_job_form_a.inc');
    }
    else
    {
        include('job_descriptions/add_job/add_job_form_d.inc');
    }
}

elseif ($continue == 'Add job now') 
{
	/* 
	 We are now ready to add the job to the database.  If an
	 employer account is being used to add the job, it will
	 be added with the NEW flag set, and will need to be updated
	 with a status of posted before it can be seen by students.
	*/

	$job = unpackObject($job);
	include('job_descriptions/add_job/add_job.inc');
}

elseif ($continue == 'Invalid Employer')
{
	/*
	 The employer has a login_id, but they do not have a company
	 associated with them, therefore we cannot allow them to add
	 a job.
	*/

	$msg = ("Sorry, you do not have a company associated with you in our database.");
	$msg .= ("  Please contact the ".ORGANIZATION_CODE." Co-op department to have this problem resolved.");
	error($msg);
}


// End of the add_job_form.inc code.

/*
 function below is used to determine whether or not a user wants to edit a job, and is called
 when they attempt to add a job to the database whose job code is already in use.
*/

function promptEdit( $msg, $job_id )
{
        global $PHP_SELF;
        instruct( $msg .  "
		<table cellspacing=2 cellpadding=2><tr><td>
                <form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit_job' />
			<input type='hidden' name='job_id' value='$job_id' />
			<input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
		</form>
		</td>
		<td>
		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='' />
			<input type='hidden' name='menu_select' value='' />
			<input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
		</form></td></tr>
		</table>");
}

// Function below prompts user for confirmation to add a company that has a similar name in the database.

function promptAddCompany($msg, $company_name, $company_website, $company_description)
{
        global $PHP_SELF;
        instruct($msg .  "
                <table cellspacing=2 cellpadding=2>
                <tr>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_job' />
                        <input type='hidden' name='add_this_object' value='employer' />
                        <input type='hidden' name='add_confirmed' value='true' />
                        <input type='hidden' name='continue' value='level4' />
                        <input type='hidden' name='company_name' value='" . $company_name . "' />
                        <input type='hidden' name='company_website' value='" . $company_website . "' />
                        <input type='hidden' name='company_description' value='" . $company_description . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_job' />
                        <input type='hidden' name='add_this_object' value='employer' />
                        <input type='hidden' name='continue' value='add_new_entity' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form>
                </td>
                </tr>
                </table>");
}

function promptAddDepartment($msg, $department_name, $department_website, $department_description, $employer_id)
{
        global $PHP_SELF;
        instruct($msg .  "
                <table cellspacing=2 cellpadding=2>
                <tr>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_job' />
                        <input type='hidden' name='add_this_object' value='department' />
                        <input type='hidden' name='add_confirmed' value='true' />
                        <input type='hidden' name='continue' value='level4' />
                        <input type='hidden' name='department_name' value='" . $department_name . "' />
                        <input type='hidden' name='department_website' value='" . $department_website . "' />
                        <input type='hidden' name='department_description' value='" . $department_description . "' />
                        <input type='hidden' name='employer_id' value='" . $employer_id . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_job' />
                        <input type='hidden' name='add_this_object' value='employer' />
                        <input type='hidden' name='continue' value='add_this_entity' />
                        <input type='hidden' name='employer_id' value='" . $employer_id . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form>
                </td>
                </tr>
                </table>");
}
function promptAddContact($msg, $contact_first_name, $contact_last_name, $contact_phone, $contact_cellphone, $contact_fax, $contact_email, $employer_id, $department_id)
{
        global $PHP_SELF;
        instruct($msg .  "
                <table cellspacing=2 cellpadding=2>
                <tr>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_job' />
                        <input type='hidden' name='add_this_object' value='contact' />
                        <input type='hidden' name='add_confirmed' value='true' />
                        <input type='hidden' name='continue' value='level4' />
                        <input type='hidden' name='contact_first_name' value='" . $contact_first_name . "' />
                        <input type='hidden' name='contact_last_name' value='" . $contact_last_name . "' />
                        <input type='hidden' name='contact_phone' value='" . $contact_phone . "' />
                        <input type='hidden' name='contact_cellphone' value='" . $contact_cellphone . "' />
                        <input type='hidden' name='contact_fax' value='" . $contact_fax . "' />
                        <input type='hidden' name='contact_email' value='" . $contact_email . "' />
                        <input type='hidden' name='employer_id' value='" . $employer_id . "' />
                        <input type='hidden' name='department_id' value='" . $department_id . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_job' />
                        <input type='hidden' name='add_this_object' value='contact' />
                        <input type='hidden' name='continue' value='add_this_entity' />
                        <input type='hidden' name='employer_id' value='" . $employer_id . "' />
                        <input type='hidden' name='department_id' value='" . $department_id . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form>
                </td>
                </tr>
                </table>");
}

?>
