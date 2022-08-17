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
 | Filename: save_history_changes.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: Save the changes made to a history record, into the database    |
 +------------------------------------------------------------------------------+
 | TODO: You'll notice below that there are a bunch of lines that deals with    |
 |       updating the database. All these separate update lines should be       |
 |       combined into one update statement to keep the transaction atomic.     |
 |       It would be really bad if the database went down while a separate      |
 |       update is running. This would leave the database in an inconsistent    |
 |       state.                                                                 |
 +------------------------------------------------------------------------------+

*/

include('history/container_functions.inc');

$term_has_changed_flag = NULL;

// :TODO: Address line 3 needs to be added to edit history. A region for the
//        student's home address is also needed as well as a third address line.
$record_id;
$department_in_str="('".implode("','",$departments_in_group)."')";
$sql = ("
    SELECT h.wt_status,h.department_name,h.year,h.student_number,h.company_name,h.supervisor_name,h.supervisor_fax_number,h.supervisor_position
           ,h.supervisor_phone_number,h.supervisor_cellphone_number,h.supervisor_email,h.work_term_home_phone,h.company_street_address_1
           ,h.company_street_address_2,h.company_city
           ,h.company_province,company_country,h.company_postal_code,h.work_term_street_address_1,h.work_term_street_address_2,h.work_term_city
           ,h.work_term_province,h.work_term_country,h.work_term_postal_code,h.site_visit_by_supervisor,h.site_visit_date_supervisor,h.site_visit_type_id_supervisor
           ,h.work_site_visit_notes,h.site_visit_type_id,h.comment,h.report_title,h.report_subject,h.report_marker,h.report_code,h.site_visit_date,site_visit_by
           ,h.student_work_phone,h.employer_id,h.contact_id,h.job_title,h.term_id,h.work_term_length,h.work_email,h.salary,h.salary_period_id,h.company_region_id
           ,h.work_term_hours_per_week, h.job_id, h.department_id, h.company_department_id, h.start_date, h.end_date, h.temp_supervisor_contact, h.temp_job_contact
           , IF (hc.workterm_code, hc.workterm_code, h.work_term_number) AS work_term_number
           , d.history_containers_min_days_for_wt
           , sd.record_id
	FROM history h 
    INNER JOIN student_department sd
    ON h.student_number = sd.student_number AND h.department_id = sd.department_id
    LEFT JOIN history_container hc
    ON h.history_id = hc.history_id
    LEFT JOIN discipline d
    ON sd.discipline_id = d.discipline_id 
	WHERE h.history_id='".$history_id."'
    ");
$results=$GLOBALS['dbh']->Execute($sql);
$row=$results->FetchRow();

$container_mode = useContainers($row['student_number'],$row['department_id']);

$d_id = $row['department_id'];
$record_id = $row['record_id'];

//get flags for comparision
/*$sql = "SELECT hf.description, hf.history_flags_id 
	FROM history_flags as hf, history_flags_join as hfj, department_history_flags as dhf 
	WHERE hfj.history_flags_id=hf.history_flags_id AND hfj.history_id='".$history_id."'
	      AND dhf.department_id='".$row['department_id']."' AND dhf.history_flags_id=hf.history_flags_id
	ORDER BY hf.description";
$results=$GLOBALS['dbh']->Execute($sql);
while($newrow=$results->FetchRow()){
	$flags[$newrow['history_flags_id']]=true;
}*/

$flags=history_flags($history_id,$row['department_id']);

include ('history/check_input.inc');
include ('history/check_changes.inc');

// An array that holds all the error messages.

$errormsg = array();

if ($container_mode && !$work_term_hours_per_week)
{
    array_push($errormsg,("The work term hours per week is a required field, please enter this information."));
    $errorlevel = 2;
}
elseif ($work_term_hours_per_week && !is_numeric($work_term_hours_per_week))
{
    array_push($errormsg,("The given work term hours per week is not a valid number, please re-enter this value."));
    $errorlevel = 2;
}
elseif ($work_term_hours_per_week && $work_term_hours_per_week > 168)
{
    array_push($errormsg,("The given hours per week exceeds 168 hours which is impossible. Please re-enter this value."));
    $errorlevel = 2;
}

if ($container_mode && !$start_date)
{
    array_push($errormsg,("The start date is a required field, please enter a start date."));
    $errorlevel = 2;
}

if ($container_mode && !$end_date)
{
    array_push($errormsg,("The end date is a required field, please enter an end date."));
    $errorlevel = 2;
}

if ($start_date && !isValidDate2($start_date)) 
{
    array_push($errormsg,("The given start date is invalid. Please check that the date exists, and ensure that the date format is YYYY-MM-DD."));
}

if ($end_date && !isValidDate2($end_date)) 
{
    array_push($errormsg,("The given end date is invalid. Please check that the date exists, and ensure that the date format is YYYY-MM-DD."));
}

if ($start_date && $end_date && $container_mode != CONTAINER_TERM)
{
    if (strtotime($start_date) >= strtotime($end_date)) 
    {
        array_push($errormsg,"The given end date is before the start date. Please verify dates and re-enter them.");
        $errorlevel = 2;
    }
    elseif ($container_mode && count_days($start_date, $end_date) < $row['history_containers_min_days_for_wt']) 
    {
        array_push($errormsg,"A work term must span at least ". $row['history_containers_min_days_for_wt']. " days. Please adjust the start and end dates.");
        $errorlevel = 2;
    }
}

// begin saving values. 
if (!$errorlevel){
	if ($site_visit_type_id<>NULL || $site_visit_type_id<>$row['site_visit_type_id']){
		if ($site_visit_type_id==NULL){
			$site_visit_type_id="NULL";
		}
		$sql = "update history set site_visit_type_id='".$site_visit_type_id."' where history_id='".$history_id."'";
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($site_visit_type_id_supervisor<>NULL || $site_visit_type_id_supervisor<>$row['site_visit_type_id_supervisor']){
		if ($site_visit_type_id_supervisor==NULL){
			$site_visit_type_id="NULL";
		}
		$sql= "update history set site_visit_type_id_supervisor='".$site_visit_type_id_supervisor."' where history_id='".$history_id."'";
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($supervisor_position<>NULL || $supervisor_position<>$row['supervisor_position']){
		$sql= "update history set supervisor_position='".$supervisor_position."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($site_visit_date_supervisor != $row['site_visit_date_supervisor'])
    {
		if (!$site_visit_date_supervisor)
        {
			$sql = ("
                UPDATE history 
                SET site_visit_date_supervisor=NULL 
                WHERE history_id='" . $history_id . "'
                ");
            $GLOBALS['dbh']->Execute($sql);
		}
        else
        {
			$sql = ("
                UPDATE history 
                SET site_visit_date_supervisor='" . $site_visit_date_supervisor . "' 
                WHERE history_id='" . $history_id . "'
                ");
			$GLOBALS['dbh']->Execute($sql);
		}
	}

	if ($site_visit_date != $row['site_visit_date'])
    {
		if (!$site_visit_date)
        {
			$sql = ("
                UPDATE history 
                SET site_visit_date=NULL 
                WHERE history_id='" . $history_id . "'
                ");
			$GLOBALS['dbh']->Execute($sql);	
		}
        else
        {
			$sql = ("
                UPDATE history 
                SET site_visit_date='" . $site_visit_date . "' 
                WHERE history_id='" . $history_id . "'
                ");
			$GLOBALS['dbh']->Execute($sql);
		}
	}

	// If we change the term/year of the student's work term, we will change the student's work term to the new term/year entered. 	
    // Note: If the user is using history containers, this case will not be entered because history containers don't know what term/year
    // they're in until a start/end date is given and calculated. 

    // Here's what the following if statement does
    // onTermYearChange($new_term, $new_year) {
    //    remove eligible flag
    //    insert placed flag
    //    insert wasEverEligible flag
    //    renumber this work term's work term number
    //    disable student's job applications
    //    update work term's year/term/temp_date_entered
    //    update contact status, give supervisor and job's contact credit for this placement
    // }

    // History records that use containers have a different way of calculating what their term/year is.
    // It is located in history.inc, that's why it skips this block when a term and year has changed. 
	if (($editHistory->term_id != $term) || ($editHistory->year != $year) && !$container_mode) {

        $term_has_changed_flag = 1;

        include_once('placement/SkeletonHistory.class');

        // Set history record to new term and year
        $temp_date_entered = getTempDateEntered($term, $year);
        
        $sql = ("
            UPDATE history 
            SET term_id = '".$term."', year = '".$year."', temp_date_entered = '".$temp_date_entered."'  
            WHERE history_id = '".$history_id."'
            ");
        $GLOBALS['dbh']->Execute($sql);

        // get rid of placed flag from previous term/year
        SkeletonHistory::unsetPlacedFlag($record_id, $editHistory->term_id, $editHistory->year);
        // and set a placed flag for the new term/year
        SkeletonHistory::setPlacedFlag($record_id, $term, $year);

        // any one that is placed in a term is assumed to have been eligible for that term, so we set the ever eligible flag 
        if (!wasEverEligible($record_id, $term, $year)) {
            setEverEligible($record_id, $term, $year);
        }

        // If the student has been placed for this term and year, then we will set their flag as PLACED, and remove their ELIGIBLE flag. 
        SkeletonHistory::unsetEligibleFlag($record_id, $term, $year);

        // deactivate their job applications for the new term and year 
        SkeletonHistory::disableStudentApplications($row['student_number'], $term, $year);

        // we don't use containers, and we're changing the term and year, we resync the history record's work term numbers. 
        if (!$container_mode) {
            include_once('placement/SkeletonHistory.class');
            $sh = new SkeletonHistory('');
            $sh->syncNonContainersWithHistory($row['student_number'], $row['department_id']);
        }

        // if the job contact and supervisor contact is the same person, we don't want to give that person 
        // double credit, there's no point in doing so, and updateContactStatus is an expensive operation  
        $supervisor_contact_id = $row['temp_supervisor_contact'];
        $job_contact_id = $row['temp_job_contact'];

        if (($job_contact_id && $supervisor_contact_id) && $job_contact_id == $supervisor_contact_id) {
            updateContactStatus($supervisor_contact_id, $row['department_id'], null, $history_id);
        }
        else {
            if ($supervisor_contact_id) {
                updateContactStatus($supervisor_contact_id, $row['department_id'], null, $history_id);
            }

            if ($job_contact_id)
            {
                updateContactStatus($job_contact_id, $row['department_id'], null, $history_id);
            }
        }
	} // end if term/year changed
	
	if ($company_street_address_1<>NULL || $company_street_address_1<>$row['company_street_address_1'])
    {
		$sql = ("
            UPDATE history SET company_street_address_1 = '".$company_street_address_1."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($company_street_address_2<>NULL || $company_street_address_2<>$row['company_street_address_2'])
    {
		$sql = ("
            UPDATE history SET company_street_address_2 = '".$company_street_address_2."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($company_city<>NULL || $company_city<>$row['company_city'])
    {
		$sql = ("
            UPDATE history SET company_city = '".$company_city."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($company_province<>NULL || $company_province<>$row['company_province'])
    {
		/*Commented out by: Jon
          Date: October 29, 2004
          Reason: This was displaying the word "NULL" under current address when no province was added
          (editing placement history). Also, single quotes (first set) were missing in the update query below and, once added,
          This statement was no longer needed. If no province was chosen, current address will now place a blank
          where the province name is expected.
        
        if ($company_province==NULL)
        {
			$company_province="NULL";
		}*/
        
		$sql = ("
            UPDATE history SET company_province = '".$company_province."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($company_country<>NULL || $company_country<>$row['company_country'])
    {
        /*Commented out by: Jon
          Date: October 29, 2004
          Reason: This was displaying the word "NULL" under current address when no country was added
          (editing placement history). Also, single quotes (first set) were missing in the update query below and, once added,
          This statement was no longer needed. If no province was chosen, current address will now place a blank
          where the country name is expected.

        if ($company_country==NULL)
        {
			$company_country="NULL";
		}*/
        
		$sql = ("
            UPDATE history SET company_country = '".$company_country."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($company_region<>NULL || $company_region<>$row['company_region_id'])
    {
        /*Commented out by: Jon
          Date: October 29, 2004
          Reason: This was displaying the word "NULL" under current address when no region was added
          (editing placement history). Also, single quotes (first set) were missing in the update query below and, once added,
          This statement was no longer needed. If no province was chosen, current address will now place a blank
          where the region name is expected.

        if ($company_region==NULL)
        {
			$company_region="NULL";
		}*/

		$sql = ("
            UPDATE history SET company_region_id = '".$company_region."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($company_postal_code<>NULL || $company_postal_code<>$row['company_postal_code'])
    {
		$sql = ("
            UPDATE history SET company_postal_code = '".$company_postal_code."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($work_term_city<>NULL || $work_term_city<>$row['work_term_city'])
    {
		$sql = ("
            UPDATE history SET work_term_city = '".$work_term_city."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($work_term_postal_code<>NULL || $work_term_postal_code<>$row['work_term_postal_code'])
    {
		$sql = ("
            UPDATE history SET work_term_postal_code = '".$work_term_postal_code."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

    if ($salary XOR $salary_period_id)
    {
        array_push($errormsg,("Both salary and salary pay period must be filled out as they depend on each other to be meaningful."));
    }
    else
    {
        if($salary<>$row['salary'])
        {
            if (is_numeric($salary) && (preg_match("/^(\d)+\.[0-9][0-9]$/",$salary) || preg_match("/^(\d)+$/",$salary)))
            {
                $sql = ("
                    UPDATE history SET salary = '".$salary."' WHERE history_id = '".$history_id."'
                    ");
                $GLOBALS['dbh']->Execute($sql);	
            }
            elseif(!$salary && $row['salary'])
            {
                $sql = ("
                    UPDATE history SET salary = NULL WHERE history_id = '".$history_id."'
                    ");
                $GLOBALS['dbh']->Execute($sql);	
            }
            elseif($salary) 
            {
                array_push($errormsg,('The salary has not saved properly because there were some non-digit characters entered or the number formatting is incorrect. This field can take numbers like: 2200 or 2400.75.'));
            }
        }

        if ($salary_period_id<>$row['salary_period_id'])
        {
            if (preg_match("/^\d+$/",$salary_period_id))
            {
                $sql = ("
                    UPDATE history SET salary_period_id = '".$salary_period_id."' WHERE history_id = '".$history_id."'
                    ");
                $GLOBALS['dbh']->Execute($sql);	
            }
            elseif (!$salary_period_id && $row['salary_period_id'])
            {
                $sql = ("
                    UPDATE history SET salary_period_id = NULL WHERE history_id = '".$history_id."'
                    ");
                $GLOBALS['dbh']->Execute($sql);	
            }
            elseif ($salary_period_id)
            {
                array_push($errormsg,('The salary pay period has not saved properly because there were invalid characters.'));
            }
        }
    }

    if ($salary && $salary_period_id && !$work_term_hours_per_week)
    {
        $sql = ("
            SELECT employer_id
            FROM history
            WHERE history_id  = '".$history_id."'
            ");
        $results = $GLOBALS['dbh']->Execute($sql);
        $employer_row = $results->FetchRow();

        $sql = ("
            SELECT ec.company_type_id, ct.type_name
            FROM employer_company ec
            INNER JOIN company_type ct 
            ON ec.company_type_id = ct.type_id
            WHERE ec.employer_id = '".$employer_row['employer_id']."'
            ");
        $results = $GLOBALS['dbh']->Execute($sql);

        $company_type_row = $results->FetchRow();

        switch ($company_type_row['company_type_id'])
        {
            case FG:
                $work_term_hours_per_week = FG_HOURS_PER_WEEK;
                break;
            case PG:
                $work_term_hours_per_week = PG_HOURS_PER_WEEK;
                break;
            case PB:
                $work_term_hours_per_week = PB_HOURS_PER_WEEK;
                break;
        }

        if ($work_term_hours_per_week)
        {
			array_push($errormsg,("
                You have entered a salary for this record, but have not entered the number of hours per week. 
                For ".$company_type_row['type_name']." jobs, the default hours per week is ".$work_term_hours_per_week." 
                per week. This value has been entered into the hours per week field.
                ")
            );
        }
        else
        {
            array_push($errormsg,("A salary has been entered in for this placement; however, the hours per week field is a required field. Please enter a value in that field."));
        }
    }
    
    if ($work_term_hours_per_week<>$row['work_term_hours_per_week'])
    {
        if (preg_match("/^0+\.0*$/",$work_term_hours_per_week) || preg_match("/^0+$/",$work_term_hours_per_week))
        {
			array_push($errormsg,("Entering 0 hours per week is not valid. Please re-enter the hours per week for this record."));
        }
		elseif (is_numeric($work_term_hours_per_week) && (preg_match("/^(\d)+\.[0-9]{1,2}$/",$work_term_hours_per_week) || preg_match("/^(\d)+$/",$work_term_hours_per_week)))
        {
            $sql = ("   
                UPDATE history SET work_term_hours_per_week = '".$work_term_hours_per_week."' WHERE history_id='".$history_id."'
                ");
            $GLOBALS['dbh']->Execute($sql);	
            if ($container_mode == CONTAINER_HOURS) 
            {
                $container_change_flag = 1; 
            }
        }
        elseif (!$work_term_hours_per_week && $row['work_term_hours_per_week'] && !$container_mode)
        {
            $sql = ("   
                UPDATE history SET work_term_hours_per_week= NULL WHERE history_id = '".$history_id."'
                ");
            $GLOBALS['dbh']->Execute($sql);	
        }
		else 
        {
			array_push($errormsg,("The hours per week field has not saved properly because there were some non-digit characters entered or the number formatting is incorrect. This field can take numbers formatted like: 40 or 37.5 or 36.25."));
        }
    }

    if ($start_date<>$row['start_date'])
    {
        $sql = ("
            UPDATE history SET start_date = ".(($start_date) ? "'".$start_date."'" : "NULL")." WHERE history_id = '".$history_id."'
            ");
        $GLOBALS['dbh']->Execute($sql);
        $container_change_flag = 1; 
    }

    if ($end_date<>$row['end_date'])
    {
        $sql = ("
            UPDATE history SET end_date = ".(($end_date) ? "'".$end_date."'" : "NULL")." WHERE history_id = '".$history_id."'
            ");
        $GLOBALS['dbh']->Execute($sql);
        $container_change_flag = 1; 
    }

    if ($container_mode == CONTAINER_TERM && $work_term_length != $row['work_term_length'])
    {
        $sql = ("
            UPDATE history SET work_term_length = '".$work_term_length."' WHERE history_id = '".$history_id."'
            ");
        $GLOBALS['dbh']->Execute($sql);
        $container_change_flag = 1;
    }

	if ($company_name<>NULL || $company_name<>$row['company_name'])
    {
		$sql = ("
            UPDATE history SET company_name = '".$company_name."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($work_term_street_address_1<>NULL || $work_term_street_address_1<>$row['work_term_street_address_1'])
    {
		$sql = ("
            UPDATE history SET work_term_street_address_1='".$work_term_street_address_1."' where history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($work_term_street_address_2<>NULL || $work_term_street_address_2<>$row['work_term_street_address_2'])
    {
		$sql = ("
            UPDATE history SET work_term_street_address_2 = '".$work_term_street_address_2."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

    if ($work_term_country<>NULL || $work_term_country<>$row['work_term_country'])
    {
        /*Commented out by: Jon
          Date: October 29, 2004
          Reason: This was displaying the word "NULL" under address when no country was added
          (editing placement history). Also, single quotes (first set) were missing in the update query below and, once added,
          This statement was no longer needed. If no country was chosen, address will now place a blank
          where the country name is expected.

          if ($work_term_country==NULL)
          {
                $work_term_country="NULL";
          }
        */
        
        $sql = ("
                UPDATE history SET work_term_country = '".$work_term_country."' WHERE history_id = '".$history_id."'
                ");
        $GLOBALS['dbh']->Execute($sql);	
    }

if ($work_term_province<>NULL || $work_term_province<>$row['work_term_province'])
{
        /*Commented out by: Jon
          Date: October 29, 2004
          Reason: This was displaying the word "NULL" under address when no province was added
          (editing placement history). Also, single quotes (first set) were missing in the update query below and, once added,
          This statement was no longer needed. If no province was chosen, address will now place a blank
          where the province name is expected.

		if ($work_term_province==NULL)
        {
			$work_term_province="NULL";
		}
        */
        
		$sql = ("
            UPDATE history SET work_term_province = '".$work_term_province."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($student_work_phone<>NULL || $student_work_phone<>$row['student_work_phone'])
    {
		$sql = ("
            UPDATE history SET student_work_phone='".$student_work_phone."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($work_email<>NULL || $work_email<>$row['work_email'])
    {
		if ($work_email==NULL)
        {
			$sql = ("
                update history SET work_email = NULL WHERE history_id = '".$history_id."'
                ");
			$GLOBALS['dbh']->Execute($sql);
		}
        else
        {
			$sql = ("
                UPDATE history SET work_email = '".$work_email."' WHERE history_id = '".$history_id."'
                ");
			$GLOBALS['dbh']->Execute($sql);
		}
	}

	if ($supervisor_phone_number<>NULL || $supervisor_phone_number<>$row['supervisor_phone_number'])
    {
		$sql = ("
            UPDATE history SET supervisor_phone_number = '".$supervisor_phone_number."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);
	}
    if ($supervisor_cellphone_number<>NULL || $supervisor_cellphone_number<>$row['supervisor_cellphone_number'])
    {
		$sql = ("
            UPDATE history SET supervisor_cellphone_number = '".$supervisor_cellphone_number."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($supervisor_email<>NULL || $supervisor_email<>$row['supervisor_email'])
    {
		$sql = ("
            UPDATE history SET supervisor_email = '".$supervisor_email."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);
	}

    // This section changed by Adam to allow for updating of contact statuses.
	if ($supervisor_name<>NULL || $supervisor_name<>$row['supervisor_name'])
    {
        $history_department_id = $row["department_id"];

		$sql = ("
            UPDATE history 
            SET supervisor_name = '" . $supervisor_name . "' 
            WHERE history_id = '" . $history_id . "'
            ");
		$GLOBALS['dbh']->Execute($sql);

        // Check if the supervisor has changed
        if (trim($supervisor_name) && (trim($supervisor_name) != trim($row['supervisor_name'])))
        {
            // Let's see if we can get a contact_id for this supervisor.
            $supervisor_contact_id = getSupervisorContactID($history_id);

            if ($supervisor_contact_id)
            {
                $sql = ("
                    UPDATE history
                    SET temp_supervisor_contact = '".$supervisor_contact_id."'
                    WHERE history_id = '".$history_id."'
                    ");
                $GLOBALS['dbh']->Execute($sql);

                updateContactStatus($supervisor_contact_id, $row['department_id'], null, $history_id);
            }
        }
	}

	if ($supervisor_fax_number<>NULL || $supervisor_fax_number<>$row['supervisor_fax_number'])
    {
		$sql = ("
            UPDATE history SET supervisor_fax_number = '".$supervisor_fax_number."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($department_name<>NULL || $department_name<>$row['department_name'])
    {
		$sql = ("
            UPDATE history SET department_name = '".$department_name."' WHERE history_id = '".$history_id."'
            ");
		$GLOBALS['dbh']->Execute($sql);
	}

    /*
	if ($work_term_number<>NULL || $work_term_number<>$row['work_term_number'])
    {
		if (!is_numeric($work_term_number) && $work_term_number<>NULL)
        {
			notify('The work term number may not have saved properly because there were some non-digit characters entered.');
        }
		elseif ($work_term_number<>NULL)
        {
			$sql = ("
                SELECT h.history_id 
				FROM history as h,student_department as sd, student_flags_join as sfj  
				WHERE h.student_number='".$student_num."' AND h.work_term_number='".$work_term_number."' AND sd.student_number=h.student_number 
                AND sd.record_id=sfj.record_id AND sfj.student_flags_id='".CURRENT_FLAG."' AND sd.department_id IN ".$department_in_str." 
                AND h.history_id !='".$history_id."'
                ");
			$results = $GLOBALS['dbh']->Execute($sql);	

			if ($results->RecordCount()==0)
            {
				$sql = ("
                    UPDATE history SET work_term_number ='".$work_term_number."' WHERE history_id = '".$history_id."'
                    ");
				$GLOBALS['dbh']->Execute($sql);
			}
            else
            {
				notify('The work term number did not save because this student already has a work term # '.$work_term_number.' in our records.');
			}
		}
        else
        {
			$sql = ("
                UPDATE history SET work_term_number = NULL WHERE history_id = '".$history_id."'
                ");
			$GLOBALS['dbh']->Execute($sql);
		}
	}
    */
	if ($job_title<>NULL){
		$sql="update history set job_title='".$job_title."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);	
	}

	if ($site_visit_by<>NULL || $site_visit_by<>$row['site_visit_by']){
        if($site_visit_by != -1)
        {
            if ($site_visit_by==NULL){
                $sql = "update history set site_visit_by=NULL where history_id=".$history_id;
                $GLOBALS['dbh']->Execute($sql);
            }else{
                $sql="update history set site_visit_by=".$site_visit_by." where history_id=".$history_id;
                $GLOBALS['dbh']->Execute($sql);
            }
        }
        else
        {
            array_push($errormsg,("You must select a valid selection for Student Work Term Visit"));
        }
            
	}

	if ($site_visit_by_supervisor<>NULL || $site_visit_by_supervisor<>$row['site_visit_by_supervisor']){
        if($site_visit_by_supervisor != -1)
        {
            if ($site_visit_by_supervisor==NULL){
                $sql = "update history set site_visit_by_supervisor=NULL where history_id=".$history_id;
                $GLOBALS['dbh']->Execute($sql);
            }else{
                $sql = "update history set site_visit_by_supervisor=".$site_visit_by_supervisor." where history_id=".$history_id;
                $GLOBALS['dbh']->Execute($sql);
            }
        }
        else
        {
            array_push($errormsg,("You must select a valid selection for Supervisor Work Term Visit"));
        }
            
	} 
    // Set automated contact action for the supervisor of this placement history.
    // Basically, if all three fields (on, by, type) are filled, we set the action, Conducted Worksite Visit,
    // for the supervisor's contact id.

    // convert the site visit types to their respective action methods in the contact_actions_set table 
    switch($site_visit_type_id_supervisor)
    {
        case ON_SITE_VISIT:
        case ON_CAMPUS:
            $converted_site_visit_type_id_supervisor = IN_PERSON;
            break;
        case PHONE_ID:
            $converted_site_visit_type_id_supervisor = PHONE_METHOD;
            break;
        case EMAIL:
            $converted_site_visit_type_id_supervisor = EMAIL_METHOD;
            break;
        case OTHER:
            $converted_site_visit_type_id_supervisor = OTHER_METHOD;
            break;
        case NULL:
        case "0":
            $converted_site_visit_type_id_supervisor = "0";
            break;
        default:
            assert(false);
            break;
    }
        
    $sup_name = $row['supervisor_name']; 

    //get contact id of supervisor for this history record
    $supervisor_contact = getSupervisorContactID($history_id);


    if($site_visit_date_supervisor && $site_visit_by_supervisor && $converted_site_visit_type_id_supervisor && $supervisor_contact)
    {
        $sql = ("
                SELECT contact_actions_id
                FROM contact_actions_set
                WHERE history_id='".$history_id."'
                AND action_id='".CONDUCTED_WORKSITE_VISIT."'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount())
        {
            $update_sql = ("
                    UPDATE contact_actions_set
                    SET contact_id='".$supervisor_contact."',
                     action_on='".$site_visit_date_supervisor."',
                     action_by='".$site_visit_by_supervisor."',
                     action_method_id='".$converted_site_visit_type_id_supervisor."',
                     action_note='Student: ".$student_num."'
                    WHERE history_id='".$history_id."'
                     AND action_id='".CONDUCTED_WORKSITE_VISIT."'
                    ");
            $update_result = $GLOBALS['dbh']->Execute($update_sql);
        } else {
            $insert_sql = ("
                    INSERT INTO contact_actions_set
                    (   
                     contact_id,
                     action_id,
                     action_on,
                     action_by,
                     action_method_id,
                     history_id,
                     action_note
                    )
                    VALUES
                    (
                     '".$supervisor_contact."',
                     '".CONDUCTED_WORKSITE_VISIT."',
                     '".$site_visit_date_supervisor."',
                     '".$site_visit_by_supervisor."',
                     '".$converted_site_visit_type_id_supervisor."',
                     '".$history_id."',
                     'Student: ".$student_num."'
                    )
                    ");
            $insert_result = $GLOBALS['dbh']->Execute($insert_sql);
        }
    }
    else // one of our four things is NULL
    {
        $delete_sql = ("
                DELETE FROM contact_actions_set
                WHERE action_id = '".CONDUCTED_WORKSITE_VISIT."'
                AND history_id='".$history_id."'
                ");
        $result = $GLOBALS['dbh']->Execute($delete_sql);           
    }

	if ($work_term_home_phone<>NULL || $row['work_term_home_phone']<>$work_term_home_phone){
		$sql = "update history set work_term_home_phone='".$work_term_home_phone."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}


	if ($report_title<>NULL || $report_title<>$row['report_title']){
		$sql="update history set report_title='".$report_title."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($report_marker<>NULL || $report_marker<>$row['report_marker']){
		$sql="update history set report_marker='".$report_marker."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}
	if ($report_code<>NULL || $report_code<>$row['report_code']){
		$sql="update history set report_code='".$report_code."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}


	if ($report_subject<>NULL || $report_subject<>$row['report_subject']){
		if ($report_subject==NULL){
			$sql = "update history set report_subject=NULL where history_id=".$history_id;
			$GLOBALS['dbh']->Execute($sql);
		}else{
			$sql="update history set report_subject=".$report_subject." where history_id=".$history_id;
			$GLOBALS['dbh']->Execute($sql);
		}
	}

	if ($wt_status<>NULL || $wt_status<>$row['wt_status']){
		if ($wt_status==NULL){
			$sql = "update history set wt_status=NULL where history_id=".$history_id;
			$GLOBALS['dbh']->Execute($sql);
		}else{
			$sql="update history set wt_status=".$wt_status." where history_id=".$history_id;
			$GLOBALS['dbh']->Execute($sql);
		}
	}

	if ($options<>NULL || $options<>$row['options']){
		$sql="update history set options='".$options."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}

	if ($comment<>NULL || $comment<>$row['comment']){
		$sql="update history set comment='".$comment."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}
	if ($work_site_visit_notes<>NULL || $work_site_visit_notes<>$row['work_site_visit_notes']){
		$sql="update history set work_site_visit_notes='".$work_site_visit_notes."' where history_id=".$history_id;
		$GLOBALS['dbh']->Execute($sql);
	}
	if ($notes<>NULL){
		$contact_row=getContactInfoByLoginID($login_id);
		$contact_id=$contact_row['contact_id'];
		$sql="Insert into history_notes (history_id,notes,contact_id,date_entered) values ('$history_id','$notes','$contact_id','".date('Y-m-d G:h:i:s')."')";
		$GLOBALS['dbh']->Execute($sql);
	}

	/* Handle the Flags Here */
// :BUG: Danger!:  Check to make sure we cant delete any arbitrary history id here....
	$sql = "Delete from history_flags_join where history_id=".$history_id;
	$GLOBALS['dbh']->Execute($sql);

    $arrFlags=history_flags($history_id,$d_id);
    if (is_array($_POST['history_flags']) && sizeof($_POST['history_flags']) > 0){
        foreach($_POST['history_flags'] as $history_flag_id => $value){
            if (!isset($arrFlags[$history_flag_id])){ // Make sure this is actually a valid flag...
                continue; // Its not a valid flag, skip it.
            }
            $GLOBALS['dbh']->Execute("INSERT INTO history_flags_join (history_id,history_flags_id) VALUES (".(int)$history_id.",".(int)$history_flag_id.")");
        }
    }



/*
	$sql="SELECT hf.description,hf.history_flags_id 
	      FROM history_flags as hf, department_history_flags as dhf 
	      WHERE dhf.history_flags_id=hf.history_flags_id AND dhf.department_id='".$d_id."'";
	$results=$GLOBALS['dbh']->Execute($sql);
	while ($row=$results->FetchRow()){
		$flagArray=explode(" ",$row['description']);
		$flag=implode("_",$flagArray);
		if ($$flag=="True"){
			$flag_sql="Insert into history_flags_join (history_id,history_flags_id) values (".$history_id.",".$row['history_flags_id'].")";
			$GLOBALS['dbh']->Execute($flag_sql); 
		}
	}
*/
	if ($oldnotes<>NULL){
		foreach($oldnotes as $x){
			$sql = "Delete from history_notes where history_notes_id=".$x;
			$GLOBALS['dbh']->Execute($sql);
		}
	}
}

elseif ($errorlevel == 1)
{
	array_push($errormsg, ("Your changes have not been saved because someone else is editing the same record. In order to avoid overwriting someone else's changes,  please review the form and re-enter your own changes.  Your changes are inside brackets beside each entry."));
}

if (sizeof($errormsg))
{
    echo("<form name='error_level_form' action='".$PHP_SELF."' method='post'>");
    //echo("<input type='hidden' name='errorlevel' value='".$errorlevel."'>");
	//error("Your Changes have not been saved because someone else is editing the same record. In order to avoid overwriting someone else's changes,  please review the form and re-enter your own changes.  Your changes are inside brackets beside each entry.");
    echo("<input type='hidden' name='errormsg' value='".$errormsg."'>");
    echo("</form>");
}
?>
