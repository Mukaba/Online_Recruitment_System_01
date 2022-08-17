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
 | Filename: save_placements.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file takes the student number and job id from              |
 | term_results.inc and places a student by updating the appropriate tables.    |
 | :TODO: Get rid of table/column spanning through file.                        |
 +------------------------------------------------------------------------------+

*/

?>

<h3>Job Offers - Confirmation</h3>
<?php

if ($select == "placement_by_term" && is_array($arr_ready) && sizeof($arr_ready))
{
    if (sizeof($error_array) && is_array($error_array))
    {
        foreach($error_array as $error_msg)
        {
            error($error_msg);
        }
    }

	foreach ($arr_ready as $placement)
    {
        if (is_a($placement,"Placement"))
        {
            place_student($placement);
        }
	}
}
elseif($select == "placement_by_student")
{
    place_student($placement);
}

// place_student now has transactional abilities. Every update/insert/delete can be reversed if any part of the algorithm fails to perform as expected.

function place_student($placement)
{
    if ($placement->isJobFilled())
    {
        error("Job ".$placement->job_code." has already been filled, we couldn't place ".$placement->first_name." ".$placement->last_name.".");
    }
    elseif ($placement->isStudentPlaced() && !$placement->container_mode)
    {
        error($placement->first_name." ".$placement->last_name." has already been placed for ".getTermName($placement->term_id)." ".$placement->year.".");
    }
    elseif (!$placement->isStudentEligible())
    {
        error($placement->first_name." ".$placement->last_name." is not eligible to be placed for ".getTermName($placement->term_id)." ".$placement->year.".");
    }
    else
    {
        // We've passed through all the error checking and validation, and now we're ready to place. There are three different types of placement records
        // that require different information. 

        // This first case is a vanilla flavoured placement record. When you generate one of these records, it assumes the placement record is the length
        // of a term defined by your settings. In this IF statement is everything that has to be filled out for one of these types of placement records. 
        if (!$placement->container_mode)
        {
            $work_term_number = $placement->generateWorkTermNumber();
            $temp_date_entered = getTempDateEntered($placement->term_id, $placement->year);

            // get job's start and end date which we'll use for generating the history record
            $sql = ("
                SELECT start_date, end_date
                FROM job_info
                WHERE job_id = '".$placement->job_id."'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $sql = ("
                INSERT INTO history 
                (work_term_number,job_id,department_id,student_number,employer_id,company_name,company_department_id,department_name,year,term_id
                , temp_date_entered, temp_job_contact, entered_by, entered_on, start_date, end_date) 
                VALUES 
                ('".$work_term_number."'
                ,'".$placement->job_id."'
                ,'".$placement->student_department_id."'
                ,'".$placement->student_number."'
                ,'".$placement->employer_id."'
                ,'".addslashes(stripslashes($placement->company_name))."'
                ,'".$placement->division_id."'
                ,'".addslashes(stripslashes($placement->division_name))."'
                ,'".$placement->year."'
                ,'".$placement->term_id."'
                ,'".$temp_date_entered."'
                ,'".$placement->contact_id."'
                ,'".$auth->contact_id."'
                , NOW()
                , ".(($row['start_date']) ? "'".$row['start_date']."'" : "NULL")."
                , ".(($row['end_date']) ? "'".$row['end_date']."'" : "NULL")."
                )
                ");
            $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                SELECT LAST_INSERT_ID() as history_id
                ");
            $results=$GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();	 
            $history_id=$row['history_id'];

            updateContactStatus($placement->contact_id, $placement->student_department_id, null, $history_id);

            $placement->unsetEligibleFlag();
            $placement->setPlacedFlag();
            $placement->setJobFilled();
            $placement->setJobsStudentStatus();
            $placement->disableStudentApplications();

            if (!wasEverEligible($placement->record_id, $placement->term_id, $placement->year)) {
                setEverEligible($placement->record_id, $placement->term_id, $placement->year);
            }
        }

        // Otherwise, there are history containers that are used in the system. Containers assume that the placement record being entered does not automatically
        // span a term. These records can be less than a term, and are measured in different time units. The two modes we have right now can be based on hours
        // a student works, or by months. 
        if ($placement->container_mode)
        {
            if ($placement->container_mode == CONTAINER_TERM)
            {
                $arr_history_records = $placement->generateHistoryRecordsTerm();
            }
            elseif ($placement->container_mode == CONTAINER_HOURS)
            {
                $arr_history_records = $placement->generateHistoryRecordsHours();
            }

            for($i  = 0; $i < sizeof($arr_history_records); $i++)
            {
                $temp_date_entered = getTempDateEntered($arr_history_records[$i]['term_id'], $arr_history_records[$i]['year']);

                // all the required information required for students place in a term container. 
                if ($placement->container_mode == CONTAINER_TERM)
                {
                    $sql = ("
                        INSERT INTO history 
                        (job_id,company_department_id,department_id,employer_id,term_id,year,student_number,department_name,company_name,temp_date_entered, work_term_length
                        , work_term_hours_per_week, start_date, end_date, temp_job_contact, entered_by, entered_on) 
                        VALUES 
                        ('".$placement->job_id."'
                         ,'".$placement->division_id."'
                         ,'".$placement->student_department_id."'
                         ,'".$placement->employer_id."'
                         ,'".$arr_history_records[$i]['term_id']."'
                         ,'".$arr_history_records[$i]['year']."'
                         ,'".$placement->student_number."'
                         ,'".addslashes(stripslashes($placement->division_name))."'
                         ,'".addslashes(stripslashes($placement->company_name))."'
                         ,'".$temp_date_entered."'
                         ,'".addslashes(stripslashes($arr_history_records[$i]['work_term_length']))."'
                         ,'".$placement->work_term_hours_per_week."'
                         ,'".$arr_history_records[$i]['start_date']."'
                         ,'".$arr_history_records[$i]['end_date']."'
                         ,'".$placement->contact_id."'
                         ,'".$auth->contact_id."'
                         , NOW()
                        )
                        ");
                }
                // all the required information required for students place in a hours based container. 
                elseif ($placement->container_mode == CONTAINER_HOURS)
                {
                    $sql = ("
                        INSERT INTO history 
                        (job_id,company_department_id,department_id,employer_id,term_id,year,student_number,department_name,company_name,temp_date_entered
                        , work_term_hours_per_week, start_date, end_date, temp_job_contact, entered_by, entered_on) 
                        VALUES 
                        ('".$placement->job_id."'
                         ,'".$placement->division_id."'
                         ,'".$placement->student_department_id."'
                         ,'".$placement->employer_id."'
                         ,'".$arr_history_records[$i]['term_id']."'
                         ,'".$arr_history_records[$i]['year']."'
                         ,'".$placement->student_number."'
                         ,'".addslashes(stripslashes($placement->division_name))."'
                         ,'".addslashes(stripslashes($placement->company_name))."'
                         ,'".$temp_date_entered."'
                         ,'".$placement->work_term_hours_per_week."'
                         ,'".$arr_history_records[$i]['start_date']."'
                         ,'".$arr_history_records[$i]['end_date']."'
                         ,'".$placement->contact_id."'
                         ,'".$auth->contact_id."'
                         , NOW()
                        )
                        ");
                }
                $GLOBALS['dbh']->Execute($sql); 	

                $sql = ("
                    SELECT LAST_INSERT_ID() as history_id
                    ");
                $results = $GLOBALS['dbh']->Execute($sql);
                $row = $results->FetchRow();
                $history_id = $row['history_id'];
                // Update the status for the job contact, division and company.

                updateContactStatus($placement->contact_id, $placement->student_department_id, null, $history_id);

                if (!$placement->keep_eligible_flag) {
                    $placement->unsetEligibleFlag($arr_history_records[$i]['term_id'],$arr_history_records[$i]['year']);
                }

                $placement->setPlacedFlag($arr_history_records[$i]['term_id'],$arr_history_records[$i]['year']);
                $placement->setJobFilled();
                $placement->setJobsStudentStatus();

                if (!$placement->keep_eligible_flag) {
                    $placement->disableStudentApplications($arr_history_records[$i]['term_id'],$arr_history_records[$i]['year']);
                }

                if (!wasEverEligible($placement->record_id, $arr_history_records[$i]['term_id'], $arr_history_records[$i]['year'])) {
                    setEverEligible($placement->record_id, $arr_history_records[$i]['term_id'], $arr_history_records[$i]['year']);
                }
            }

            // if we're using containers, insert/update the containers to match the additional wt in history
            if (useContainers($placement->student_number, $placement->student_department_id)) {
                syncContainersWithHistory($placement->student_number, $placement->student_department_id);
            }
        }

        notify($placement->first_name." ".$placement->last_name." has been successfully placed in job ".$placement->job_code.".");
    }
}

?>
