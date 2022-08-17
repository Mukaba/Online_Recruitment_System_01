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
 | Filename: save_add_changes.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: When a user adds a student to his/her own job placement. This   |
 | file saves that information about which company he/she was placed in.        |
 +------------------------------------------------------------------------------+

*/

/*
 :CAUTION: Currently, this file does NOT add a contact_id to a freshly created history record.
 HOWEVER, if this is changed, This file MUST be changed to account for updating contact statuses
 (an entry should be inserted into department_contact_status, similar to the technique used in
 job_descriptions/add_job/add_job.inc).
*/

// Typically, a division's name come in the form: My Division.inc (Victoria)
// The city name appended to the division name is stripped out using the following lines of code.
$addHistory->setDivisionName = addslashes(stripslashes($addHistory->division_name));
$addHistory->setDivisionName = preg_replace("/(.+)\(.+\)/","\\1",$addHistory->division_name);
$addHistory->setDivisionName = trim($addHistory->division_name);

// Get the student's department_id.
/// determine the workterm number
if (!$addHistory->container_mode)
{
    $work_term_number = $addHistory->generateWorkTermNumber();
    $temp_date_entered = getTempDateEntered($addHistory->term_id, $addHistory->year);

    $sql = ("
        INSERT INTO history 
        (work_term_number,company_department_id,department_id,employer_id,term_id,year,student_number,department_name,company_name
        ,temp_date_entered, entered_by, entered_on) 
        VALUES 
        (".$work_term_number."
        ,'".$addHistory->division_id."'
        ,'".$addHistory->student_department_id."'
        ,'".$addHistory->employer_id."'
        ,'".$addHistory->term_id."'
        ,'".$addHistory->year."'
        ,'".$addHistory->student_number."'
        ,'".addslashes(stripslashes($addHistory->division_name))."'
        ,'".addslashes(stripslashes($addHistory->company_name))."'
        ,'".$temp_date_entered."'
        ,'".$auth->contact_id."'
        , NOW()
        )
        ");
    $GLOBALS['dbh']->Execute($sql); 	

    $sql = ("
        SELECT LAST_INSERT_ID() as history_id
        ");
    $results=$GLOBALS['dbh']->Execute($sql);
    $row=$results->FetchRow();	 
    $history_id=$row['history_id'];

    $addHistory->setPlacedFlag();
    $addHistory->unsetEligibleFlag();
    $addHistory->disableStudentApplications();

    if (!wasEverEligible($addHistory->record_id, $addHistory->term_id, $addHistory->year)) {
        setEverEligible($addHistory->record_id, $addHistory->term_id, $addHistory->year);
    }
                            
}

if ($addHistory->container_mode)
{
    if ($addHistory->container_mode == CONTAINER_TERM)
    {
        $arr_history_records = $addHistory->generateHistoryRecordsTerm();
    }
    elseif ($addHistory->container_mode == CONTAINER_HOURS)
    {
        $arr_history_records = $addHistory->generateHistoryRecordsHours();
    }

    for($i  = 0; $i < sizeof($arr_history_records); $i++)
    {
        $temp_date_entered = getTempDateEntered($arr_history_records[$i]['term_id'], $arr_history_records[$i]['year']);

        if ($addHistory->container_mode == CONTAINER_TERM)
        {
            $sql = ("
                INSERT INTO history 
                (company_department_id,department_id,employer_id,term_id,year,student_number,department_name,company_name,temp_date_entered, work_term_length
                , work_term_hours_per_week, start_date, end_date, entered_by, entered_on) 
                VALUES 
                ('".$addHistory->division_id."'
                 ,'".$addHistory->student_department_id."'
                 ,'".$addHistory->employer_id."'
                 ,'".$arr_history_records[$i]['term_id']."'
                 ,'".$arr_history_records[$i]['year']."'
                 ,'".$addHistory->student_number."'
                 ,'".addslashes(stripslashes($addHistory->division_name))."'
                 ,'".addslashes(stripslashes($addHistory->company_name))."'
                 ,'".$temp_date_entered."'
                 ,'".addslashes(stripslashes($arr_history_records[$i]['work_term_length']))."'
                 ,'".$addHistory->work_term_hours_per_week."'
                 ,'".$arr_history_records[$i]['start_date']."'
                 ,'".$arr_history_records[$i]['end_date']."'
                 ,'".$auth->contact_id."'
                 , NOW()
                )
                ");
        }
        elseif ($addHistory->container_mode == CONTAINER_HOURS)
        {
            $sql = ("
                INSERT INTO history 
                (company_department_id,department_id,employer_id,term_id,year,student_number,department_name,company_name,temp_date_entered
                , work_term_hours_per_week, start_date, end_date, entered_by, entered_on) 
                VALUES 
                ('".$addHistory->division_id."'
                 ,'".$addHistory->student_department_id."'
                 ,'".$addHistory->employer_id."'
                 ,'".$arr_history_records[$i]['term_id']."'
                 ,'".$arr_history_records[$i]['year']."'
                 ,'".$addHistory->student_number."'
                 ,'".addslashes(stripslashes($addHistory->division_name))."'
                 ,'".addslashes(stripslashes($addHistory->company_name))."'
                 ,'".$temp_date_entered."'
                 ,'".$addHistory->work_term_hours_per_week."'
                 ,'".$arr_history_records[$i]['start_date']."'
                 ,'".$arr_history_records[$i]['end_date']."'
                 ,'".$auth->contact_id."'
                 , NOW()
                )
                ");
        }
        $GLOBALS['dbh']->Execute($sql); 	

        if ($i == 0)
        {
            $sql = ("
                SELECT LAST_INSERT_ID() as history_id
                ");
            $results=$GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();	 
            $history_id=$row['history_id'];
        }

        $addHistory->setPlacedFlag($arr_history_records[$i]['term_id'],$arr_history_records[$i]['year']);

        if (!$addHistory->keep_eligible_flag) {
            $addHistory->unsetEligibleFlag($arr_history_records[$i]['term_id'],$arr_history_records[$i]['year']);
            $addHistory->disableStudentApplications($arr_history_records[$i]['term_id'],$arr_history_records[$i]['year']);
        }

        if (!wasEverEligible($addHistory->record_id, $arr_history_records[$i]['term_id'], $arr_history_records[$i]['year'])) {
            setEverEligible($addHistory->record_id, $arr_history_records[$i]['term_id'], $arr_history_records[$i]['year']);
        }
    }

    // if we're using containers, insert/update the containers to match the additional wt in history
    if (useContainers($addHistory->student_number, $addHistory->student_department_id)) {
        syncContainersWithHistory($addHistory->student_number, $addHistory->student_department_id);
    }
}

// clean up after object. 
$addHistory = null;

?>
