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
 | Filename: new_employer.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file is the main routing file for a first time employer.   |
 |  It determines which form they have filled out (or none at all) and loads    |
 | the appropriate pages.                                                       |
 +------------------------------------------------------------------------------+

*/

$error = 0;

include('new_employer/new_employer_functions.inc'); 

if (!$continue)
{
	/*
	 They just got here, so load up the welcome screen for them.
	*/

	include('new_employer/starting_screen.inc');
}

elseif ($continue == 'collect_contact_information')
{
	/*
	 They are done with the first screen and we can collect company
	 information from them.
	*/

	include('new_employer/collect_contact_info.inc');
}

elseif ($continue == 'Edit Inputs')
{
	// Restore the newEmployer class and reload the first form, or second form if inputsFor == 'contact'.

	$newEmployer = unpackObject($newEmployer);

	if ($inputsFor == 'contact')
	{
		include('new_employer/collect_contact_info.inc');
	}
	elseif ($inputsFor == 'job')
	{
		include('new_employer/collect_job_description.inc');
	}
	else
	{
		include('new_employer/collect_company_info.inc');
	}
}

elseif ($continue == 'confirm_company_information')
{
    /*
       They have filled out the second form.  Check it for validity and then
       show it to them and prompt for confirmation.
     */

    // If they have only provided one address line and it's in the second slot, move it to the first.

    if (trim($company_street_address_1) == '' && trim($company_street_address_2) != '')
    {
        $company_street_address_1 = trim($company_street_address_2);
        $company_street_address_2 = '';
    }

	$newEmployer = unpackObject($newEmployer);

    $newEmployer->set_company_values($employer_id, $company_name, $company_website, $company_description, $company_street_address_1,
            $company_street_address_2, $company_street_address_3, $company_city, $company_region_id, $company_provstate_id, $company_country_id, 
            $company_postal_code, $company_phone, $company_fax, $company_email, $company_industry_id, $company_size_id, $company_department_name,
            $company_type_id);

    if (trim($newEmployer->company_name) == '')
    {
        $error = 1;
        error("You must include a company name.");
        include('new_employer/collect_company_info.inc');
    }
    elseif ((trim($newEmployer->company_email) != '') && !(isValidEmail($newEmployer->company_email)))
    {
        $error = 1;
        error("You must include a valid set of e-mail addresses, or none at all.");
        include('new_employer/collect_company_info.inc');
    }
    elseif ((trim($newEmployer->company_phone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($newEmployer->company_phone)))))
    {
        $error = 1;
        error("You must include a valid phone number with an area code, or none at all.");
        include('new_employer/collect_company_info.inc');
    }
    elseif ((trim($newEmployer->company_fax) != '') && !(ereg('(.*[0-9].*){8,}',(trim($newEmployer->company_fax)))))
    {
        $error = 1;
        error("You must include a valid fax number with an area code, or none at all.");
        include('new_employer/collect_company_info.inc');
    }
	elseif (!trim($newEmployer->company_type_id))
	{
		$error = 1;
		error("You must choose a company type for your company.");
		include('new_employer/collect_company_info.inc');
    }
    elseif (!trim($newEmployer->company_industry_id))
    {
        $error = 1;
        error("You must include an industry for your company.");
        include('new_employer/collect_company_info.inc');
    }

	if ($error == 0)
	{
		include('new_employer/confirm_company_info.inc');
	}
}

elseif ($continue == 'save_company_information')
{
	// They have confirmed the company information.  Save it, and then load up the next form.

	$newEmployer = unpackObject($newEmployer);

	include('new_employer/save_company_info.inc');
	
	include('new_employer/collect_job_description.inc');
}

elseif ($continue == 'confirm_contact_information')
{
	/*
	 They have filled out the first form.  Check it for validity and then
	 show it to them and prompt for confirmation.
	*/

	$error = 0;

	$newEmployer = new NewEmployer('');

    $newEmployer->set_contact_values($contact_id, $contact_title, $contact_first_name, $contact_middle_name, $contact_last_name,
            $contact_called_name, $contact_email, $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_street_address_1, 
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, 
            $contact_postal_code);

	/*
	 Insert input checking here.  Be SURE that we have an e-mail address (so that we can send
	 the account login and password somewhere).
	*/

	if (trim($newEmployer->contact_first_name) == '' || trim($newEmployer->contact_last_name) == '')
	{
		$error = 1;
		error("You must include both a first and last name.");
		include('new_employer/collect_contact_info.inc');
	}
	elseif ((trim($newEmployer->contact_phone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($newEmployer->contact_phone)))))
        {
                $error = 1;
                error("You must include a valid phone number with an area code, or none at all.");
                include('new_employer/collect_contact_info.inc');
        }
        elseif ((trim($newEmployer->contact_fax) != '') && !(ereg('(.*[0-9].*){8,}',(trim($newEmployer->contact_fax)))))
        {
                $error = 1;
                error("You must include a valid fax number with an area code, or none at all.");
                include('new_employer/collect_contact_info.inc');
        }
	elseif (trim($newEmployer->contact_email) == '' || !isValidEmail($newEmployer->contact_email))
	{
		$error = 1;
		error("You must include a valid e-mail address for yourself.");
		include('new_employer/collect_contact_info.inc');
	}

	if ($error == 0)
	{
		include('new_employer/confirm_contact_info.inc');
	}
}	

elseif ($continue == 'save_contact_information')
{
	/*
	 They have confirmed the contact information.  Save it, and then load up the
	 job description form.
	*/

	$newEmployer = unpackObject($newEmployer);

	include('new_employer/save_contact_info.inc');
	
	include('new_employer/collect_company_info.inc');
}

elseif ($continue == 'collect_more_job_info')
{
	/*
	 They've filled out the first job info page.  Check the inputs to make sure they are valid,
	 and then load up the second page for their viewing pleasure.
	*/

	$newEmployer = unpackObject($newEmployer);

	// Eliminate fields like salary_period here before validation check if necessary.

    $error = 0;

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

    $newEmployer->set_job_values_1($position_title, $employer_code, $num_positions, $apply_method, $receive_address, $region
            , $city, $provstate, $country, $start_term, $start_year, $start_date, $start_date_date, $start_date_month, $start_date_year
            , $end_date, $end_date_date, $end_date_month, $end_date_year, $salary_amount1, $salary_amount2, $salary_period, $workterm_lengths
            , $workterm_hours, $other_postings, $min_academic, $min_workterms, $apply_departments);

    if (trim($newEmployer->job_position_title) == '')
    {
        $error = 1;
        error("You must enter a position title to add a job.");
        include('new_employer/collect_job_description.inc');
    }
    elseif (!isValidEmail($newEmployer->job_receive_address) && (($newEmployer->job_apply_method == JOB_APP_METHOD_ONLINE)))
    {
        $error = 1;
        error("If you wish to receive online aplications, you must include a valid e-mail address.");
        include('new_employer/collect_job_description.inc');
    }
    elseif ($newEmployer->job_city == '')
    {
        $error = 1;
        error("You must include a city name if you wish to add a job.");
        include('new_employer/collect_job_description.inc');
    }
    elseif ((!isValidSalary($newEmployer->job_salary_amount1) && trim($newEmployer->job_salary_amount1 != '')) 
            || (!isValidSalary($newEmployer->job_salary_amount2) && trim($newEmployer->job_salary_amount2 != '')))
    {
        $error = 1;
        error("You must either include a valid salary amount/range, or none at all.");
        include('new_employer/collect_job_description.inc');
    }
    //:TODO: This should not be hard coded 
    elseif (($tempdate[0] == $newEmployer->job_start_year) && (($curterm == 1 && $newEmployer->job_start_term == 2) ||
                ($curterm == 1 && $newEmployer->job_start_term == 3) || ($curterm == 3 && $newEmployer->job_start_term == 2)))
    {
        $error = 1;
        error("You must enter a start date that is in the future or present to add a job.");
        include('new_employer/collect_job_description.inc');
    }
    elseif (!$workterm_lengths)
    {
        $error = 1;
        error("You must specify at least one workterm length in order to add a job.");
        include('new_employer/collect_job_description.inc');
    }
    elseif ($workterm_hours != '' && !ereg("(.*[0-9]+.*)+", $workterm_hours))
    {
        $error = 1;
        error("You must specify a valid number of workterm hours, or none at all.");
        include('new_employer/collect_job_description.inc');
    }

    $error_msg = NULL;
    $newEmployer->job_start_date = NULL;
    $newEmployer->job_end_date = NULL;

    if ($newEmployer->job_start_date_date || $newEmployer->job_start_date_month || $newEmployer->job_start_date_year)
    {
        if (!$newEmployer->job_start_date_month)
        {
            $error_msg .= "When entering a start date for your job, the month field is required. ";
        }
        elseif (!$newEmployer->job_start_date_year)
        {
            $error_msg .= "When entering a start date for your job, the year field is required. ";
        }
        else
        {
            if (!$newEmployer->job_start_date_date)
            {
                $newEmployer->job_start_date_date = "00";
            }
            // The required date format is YYYY-MM-DD, so if a user enters in 2004-1-1, we have to pad the date so it becomes 2004-01-01
            $newEmployer->job_start_date = $newEmployer->job_start_date_year."-".str_pad($newEmployer->job_start_date_month,2,0,STR_PAD_LEFT)."-".str_pad($newEmployer->job_start_date_date,2,0,STR_PAD_LEFT);

            if(!isValidDate2($newEmployer->job_start_date))
            {
                $error_msg .= "The given start date is not a valid date, please re-enter. ";
            }
        }
    }
    if ($newEmployer->job_end_date_date || $newEmployer->job_end_date_month || $newEmployer->job_end_date_year)
    {
        if (!$newEmployer->job_end_date_month)
        {
            $error_msg .= "When entering an end date for your job, the month field is required. ";
        }
        elseif (!$newEmployer->job_end_date_year)
        {
            $error_msg .= "When entering an end date for your job, the year field is required. ";
        }
        else
        {
            if (!$newEmployer->job_end_date_date)
            {
                $newEmployer->job_end_date_date = "00";
            }

            // The valid date function requires dates with the format YYYY-MM-DD, so it doesn't like dates that are like 2004-4-4, so we have to pad
            // the date to look like 2004-04-04

            $newEmployer->job_end_date = $newEmployer->job_end_date_year."-".str_pad($newEmployer->job_end_date_month,2,0,STR_PAD_LEFT)."-".str_pad($newEmployer->job_end_date_date,2,0,STR_PAD_LEFT);

            if(!isValidDate2($newEmployer->job_end_date))
            {
                $error_msg .= "The given end date is not a valid date, please re-enter. ";
            }
        }
    }

    if ($newEmployer->job_end_date && !$newEmployer->job_start_date)
    {
        $error_msg .= "A start date for the job is required because you have provided an end date.";
    }

    if (!$error_msg)
    {
        if ($newEmployer->job_start_date)
        {
            // If a start date is given, then we will override the start term and year that the user has given, as the start date is more accurate. 
            $arr_term_year = getTermYearWithGracePeriod($newEmployer->job_start_date);
            $newEmployer->job_start_term = $arr_term_year["term"];
            $newEmployer->job_start_year = $arr_term_year["year"];
        }

        // If we have a start date and no end date
        // check what term we're in using start_date + grace period
        // use that term's last day as the end date
        if ($newEmployer->job_start_date && !$newEmployer->job_end_date)
        {
            $newEmployer->job_end_date = getTermYearEndDate($newEmployer->job_start_term,$newEmployer->job_start_year);
            notify("No end date was provided for this job, so the last day of the term (".formatSpecialZeroDate($newEmployer->job_end_date).") will be used as the end date.");
        }

        if (isValidDate2($newEmployer->job_start_date) && isValidDate2($newEmployer->job_end_date) 
            && count_days($newEmployer->job_start_date, $newEmployer->job_end_date) < HISTORY_MIN_DAYS_FOR_WT)
        {
            notify("The start date and end date is less than ".HISTORY_MIN_DAYS_FOR_WT." days apart. The workterm has to be longer.");
        }                                                                                                                                            
    }

    if ($error_msg)
    {
        $error = 1;
        error($error_msg);
        include('new_employer/collect_job_description.inc');
    }
	
	if ($error == 0)
    {
        /*
           If they've set no applicable departments, set all departments as applicable.  We want to do this
           here, because if we did it on the previous screen, and they got an error, the page would be reloaded
           with all of the checkboxes filled, and would potentially confuse the user.
         */

        if (!($newEmployer->job_apply_departments))
        {
            // Add in the below line to make this query function only for ENGR and CSC
            // AND department_id IN ('1','2')
            $sql = ("
                    SELECT department_id
                    FROM department
                    WHERE job_list = '1'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $newEmployer->job_apply_departments[] = $row["department_id"];
            }
        }

        include('new_employer/collect_job_description_2.inc');
    }
}

elseif ($continue == 'confirm_job_description')
{
	/*
	 They have filled out the last job form.  Check it for validity and then
	 show it to them and prompt for confirmation.
	*/

	$newEmployer = unpackObject($newEmployer);

    $job_disciplines = trim_array($job_disciplines);
    if (!$job_disciplines)
    {
        $job_disciplines[] = ALL_DISCIPLINES;
    }

    $newEmployer->set_job_values_2($host_department, $job_disciplines, $job_description, $skills_required, $special_requirements, $comments, $displayname_on_coverltr);

    if ($newEmployer->job_description == '')
    {
        $error = 1;
        error("You must write a description for this job.");
        include('new_employer/collect_job_description_2.inc');
    }
    elseif ($newEmployer->job_skills_required == '')
    {
        $error = 1;
        error("You must specify some skills required for this job.");
        include('new_employer/collect_job_description_2.inc');
    }

    foreach ($newEmployer->job_apply_departments AS $ad)
    {
        $disc_depts .= ("'" . $ad . "',");
    }
    $disc_depts = substr($disc_depts, 0, -1);
    if (!$disc_depts)
    {
        $disc_depts = ("''");
    }

    $sql = ("
            SELECT DISTINCT discipline_id
            FROM department_discipline_join
            WHERE department_id IN (" . $disc_depts . ")
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    if (sizeof($newEmployer->job_disciplines) == $result->RecordCount())
    {
        $newEmployer->job_disciplines = '';
        $newEmployer->job_disciplines[] = ALL_DISCIPLINES;
    }

    // Insert input checking here.

    if ($error == 0)
    {
        include('new_employer/confirm_job_description.inc');
    }
}

elseif ($continue == 'save_job_description')
{
	/*
	 They have confirmed the job description.  Save it, then let them know that
	 its been saved, thank them for choosing us, blah blah, and let them know
	 they can now go and that we will send their login_id and password to the
	 entered e-mail (and then show the e-mail address).
	*/

	$newEmployer = unpackObject($newEmployer);

	include('new_employer/save_job_description.inc');

	$company_name = $newEmployer->company_name;
	$contact_name = $newEmployer->contact_first_name . " " . $newEmployer->contact_last_name;
	$position_title = $newEmployer->job_position_title;

	if ($newEmployer->job_host_department)
	{
		// Send an e-mail to the host department's primary contact.

		$host_department = $newEmployer->job_host_department;
		// We don't want to send department's an email right now.
		//include('new_employer/send_department_email.inc');
	}
	else
	{
		$no_host_department = TRUE;
	}

	// Send the traffic director an email regardless.
	include('new_employer/send_td_email.inc');
}

?>
