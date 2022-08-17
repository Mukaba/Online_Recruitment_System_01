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
 | Filename: history.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: This file is one of the first called when moving through the    |
 | history module, and operates mostly as a redirector, determining which       |
 | form the user should be directed to.                                         |
 +------------------------------------------------------------------------------+

*/


    if ($history_id == NULL) {
        echo "</td><td align='left'><img src=\"".$GLOBALS['colorscheme']['topbar_right']."\" alt=\"\" /></td></tr></table>";
        echo "<h3>Student Information - Placement History</h3>";
        notify("Please select a placement history record to edit");
    }
    else 
    {
        include('history/EditHistory.class');
        $sql = "SELECT student_number
            FROM history
            WHERE history_id='".addslashes($history_id)."'";
        $results = $GLOBALS['dbh']->Execute($sql);
        $row = $results->FetchRow();
        $student_num=$row['student_number'];
?>
&nbsp;</td>
        <td width='5'>&nbsp;</td>
        <td valign="middle">
        <form name="edithistory" action="mamook.php" method="post">
        <input type="hidden" name="select" value="history" />
        <input type="hidden" name="per_page_max" value="<?php echo $per_page_max?>" />
        <input type="hidden" name="history_id" value="<?php echo $history_id?>" />
        <input type="hidden" name="history_list" value="<?php echo $history_list?>" />
        <input type='hidden' name='student_num' value='<?php echo $student_num?>' />
        <input type='hidden' name='student_list' value='<?php echo $student_list?>' />
        <input type='hidden' name='student_department' value='<?php echo $student_department?>' />
        <input type='hidden' name='searchStudents' value='<?php echo $searchStudents?>' />
        <input type="hidden" name="menu_select" value="<?php echo $menu_select?>" />
        <input type='hidden' name='searchHistory' value='<?php echo $searchHistory?>' />
        <b class='black'>View:</b>&nbsp;<a href="javascript:document.edithistory.submit()"><b class="blue">History Record&nbsp;</b></a> 
        </form>
        </td>
        <?php
        if ($searchStudents<>NULL){
            ?>
                             <td width='5'>&nbsp;</td>
                <td valign="middle">
                <form name="student_info" action="<?php echo $PHP_SELF?>" method="post">
                <input type="hidden" name="select" value="<?php echo urlencode('View Student')?>" />
                <input type="hidden" name="student_num" value="<?php echo $student_num?>" />
                <input type="hidden" name="student_list" value="<?php echo $student_list?>" />
                <input type="hidden" name="student_department" value="<?php echo $student_department?>" />
                <input type="hidden" name="menu_select" value="<?php echo $menu_select?>" />
                <input type='hidden' name='searchStudents' value='<?php echo $searchStudents?>' />
                <a href="javascript:document.student_info.submit()"><b class="blue">Student Info&nbsp;</b></a> 
                </form>
                </td>
                               <?php
        }
?>
<td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>
<?php

    if($btnsubmit=="Save Changes")
    {
        if ($history_id == NULL)
        {
            echo "</td><td align='left'><img src=\"".$GLOBALS['colorscheme']['topbar_right']."\" alt=\"\" /></td></tr></table>";
            echo "<h3>Student Information - Placement History</h3>";
            notify("Please select a placement history record to edit");
        }
        else
        {
            include_once('history/EditHistory.class');
            $sql = "SELECT student_number
                FROM history
                WHERE history_id=".addslashes($history_id);
            $results = $GLOBALS['dbh']->Execute($sql);
            $row = $results->FetchRow();
            $student_num=$row['student_number'];
            if ($userlevel == OFFICE){
                if ($department_id == ""){
                    $department_id = $auth->department;
                }
                $department_in_str = "('" . implode("','", $departments_in_group) . "')";
            }
            include_once('history/functions.inc');
            //if "save changes" button is pressed then call the file that saves the changes
            if ($btnsubmit=="Save Changes"){
                if ($editHistory<>NULL && is_string($editHistory)){
                    $editHistory = unpackObject($editHistory);
                    $saved = true;
                }
                include ('history/save_history_changes.inc');
                $from_edit_page = 1;
                echo("<table width='90%'>");
                    echo("<tr>");
                    echo("<td align='left'>");
                        echo "<h3>Student Information - Placement History</h3>";
                    echo("</td>");
                include('history/history_quickmenu.inc');
                if (is_array($errormsg) && sizeof($errormsg) > 0)
                {
                    // this variable comes from save_history_changes.inc
                    if ($term_has_changed_flag) {
                        notify('You have modified the term/year of this history record. '.getStudentName($student_num). 
                            ' has been made ineligible for '.getTermName($term).' '.$year.'.');
                    }
                    foreach ($errormsg as $msg)
                    {
                        error($msg);
                    }
                    include('history/edithistory.inc');
                }
                else
                {
                    // Coming from edit history, and no errors were detected, and we have changed the term, this implies
                    // that the student wasn't using history containers. 
                    if ($term_has_changed_flag) {
                        $term_has_changed_msg = ('You have modified the term/year of this history record. '.getStudentName($student_num). 
                            ' has been made ineligible for '.getTermName($term).' '.$year.'.');
                    }
                    // container recalculation here
                    if ($container_change_flag)
                    {
                        include_once('history/container_functions.inc');
                        include_once('placement/SkeletonHistory.class');

                        $container_sql = ("
                            SELECT sd.record_id, sd.student_number, sd.department_id, sd.discipline_id
                            , h.term_id, h.year, h.start_date, h.end_date
                            , h.work_term_length, h.work_term_hours_per_week
                            , h.job_id, h.temp_supervisor_contact
                            FROM history h
                            INNER JOIN student_department sd
                            ON h.student_number = sd.student_number AND h.department_id = sd.department_id 
                            WHERE h.history_id = '".addslashes($history_id)."'
                            ");
                        $container_result = $GLOBALS['dbh']->Execute($container_sql);
                        $container_row = $container_result->FetchRow();
                        
                        $container_mode = useContainers($container_row['student_number'], $container_row['department_id']);

                        if ($container_mode)
                        {
                            // We key in the type of container the history record is using, and the start and end dates
                            // for the history record, or the length of the work term. Based on these values, we run the
                            // various genereateHistoryRecords[Term|Hours] which will split up this history record into 
                            // pieces. For example, if I put in an 8 month work term, and a work term is supposed to be 
                            // 4 months long, then the generate functions will give me back 2 work terms that are 4 months
                            // each. It will automatically figure out the various start/end dates for the broken up work
                            // term. 
                            $sh = new SkeletonHistory('');
                            $sh->setRecordID($container_row['record_id']);
                            $sh->setStudentNumber($container_row['student_number']);
                            $sh->setStudentDepartmentID($container_row['department_id']);
                            $sh->setDisciplineID($container_row['discipline_id']);
                            $sh->setStartDate($container_row['start_date']);
                            $sh->setContainerMode($container_mode);

                            if ($container_mode == CONTAINER_TERM)
                            {
                                $sh->setWorkTermLength($container_row['work_term_length']);
                                $arrResults = $sh->generateHistoryRecordsTerm();
                            }
                            elseif ($container_mode == CONTAINER_HOURS)
                            {
                                $sh->setEndDate($container_row['end_date']);
                                $sh->setWorkTermHoursPerWeek($container_row['work_term_hours_per_week']);
                                $arrResults = $sh->generateHistoryRecordsHours();
                            }

                            // if we are infact breaking up a work term into pieces, we want to copy the following information, and dump it into the 
                            // new work term pieces  
                            if (sizeof($arrResults) > 1)
                            {
                                $history_sql = ("
                                    SELECT h.employer_id, h.student_work_phone, h.job_title, h.work_email, h.salary, h.company_department_id, h.company_city
                                    , h.company_name, h.department_name, h.company_street_address_1, h.company_street_address_2, h.company_postal_code
                                    , h.company_position, h.supervisor_phone_number, h.supervisor_cellphone_number, h.supervisor_name, h.supervisor_fax_number
                                    , h.supervisor_email, h.work_term_street_address_1, h.work_term_street_address_2, h.work_term_city, h.work_term_postal_code
                                    , h.work_term_province, h.work_term_country, h.job_id, h.company_province, h.work_term_home_phone, h.company_country
                                    , h.supervisor_position, h.company_region_id, h.work_term_region_id, h.salary_period_id, h.work_term_hours_per_week
                                    , h.temp_supervisor_contact, h.temp_job_contact
                                    FROM history h
                                    WHERE h.history_id = '".addslashes($history_id)."'
                                    ");
                                $history_result = $GLOBALS['dbh']->Execute($history_sql);
                                $history_row = $history_result->FetchRow();

                                // addslashes to all our values that are going into the database
                                if (is_array($history_row) && sizeof($history_row)) {
                                    foreach ($history_row as $key => $value) {
                                        $history_row[$key] = addslashes($value);
                                    }
                                }
                            }

                            $new_container = 0;

                            // grab the job's contact id for contact status purposes
                            $job_contact_id = $container_row['temp_job_contact'];
                            $supervisor_contact_id = $container_row['temp_supervisor_contact'];

                            $arr_term_changed_msg = array();

                            $temp_history_id = $history_id;

                            for ($i = 0; $i < sizeof($arrResults); $i++)
                            {
                                if ($container_row['term_id'] != $arrResults[$i]['term_id'] || $container_row['year'] != $arrResults[$i]['year']) {
                                    $term_has_changed_flag = 1;
                                }

                                $temp_date_entered = getTempDateEntered($arrResults[$i]['term_id'],$arrResults[$i]['year']);

                                // The first work term record piece already exists as a history record in the system. If we're changing the term of this existing
                                // record, then we run the following to adjust the dates of the record. 
                                if ($i == 0) {
                                    if ($container_mode == CONTAINER_TERM)
                                    {
                                        $update_sql = ("
                                            UPDATE history
                                            SET term_id = '".$arrResults[$i]['term_id']."', year = '".$arrResults[$i]['year']."'
                                            , start_date = '".$arrResults[$i]['start_date']."', end_date = '".$arrResults[$i]['end_date']."'
                                            , work_term_length = '".$arrResults[$i]['work_term_length']."'
                                            , temp_date_entered = '".$temp_date_entered."'
                                            WHERE history_id = '".addslashes($history_id)."'
                                            ");
                                        $GLOBALS['dbh']->Execute($update_sql);
                                    }
                                    elseif ($container_mode == CONTAINER_HOURS)
                                    {
                                        $update_sql = ("
                                            UPDATE history
                                            SET term_id = '".$arrResults[$i]['term_id']."', year = '".$arrResults[$i]['year']."'
                                            , start_date = '".$arrResults[$i]['start_date']."', end_date = '".$arrResults[$i]['end_date']."'
                                            , temp_date_entered = '".$temp_date_entered."'
                                            WHERE history_id = '".addslashes($history_id)."'
                                            ");
                                        $GLOBALS['dbh']->Execute($update_sql);
                                    }

                                    if ($term_has_changed_flag) {
                                        // Delete placed flag from original history record 
                                        SkeletonHistory::unsetPlacedFlag($sh->record_id, $container_row['term_id'], $container_row['year']);
                                    }
                                }
                                else {
                                    // this triggers a notify message in view_history.inc
                                    $new_container = 1;

                                    $insert_sql = ("
                                        INSERT INTO history
                                        (employer_id, student_number, student_work_phone, job_title, term_id, year, work_term_length
                                        , work_email, salary, company_department_id, company_city, company_name, department_name
                                        , company_street_address_1, company_street_address_2, company_postal_code, company_position
                                        , supervisor_phone_number, supervisor_cellphone_number, supervisor_name, supervisor_fax_number, supervisor_email
                                        , work_term_street_address_1, work_term_street_address_2, work_term_city, work_term_postal_code, work_term_province
                                        , work_term_country, job_id, company_province, work_term_home_phone, company_country, supervisor_position
                                        , department_id, company_region_id, work_term_region_id, salary_period_id, work_term_hours_per_week
                                        , temp_supervisor_contact, temp_job_contact, temp_date_entered, start_date, end_date, entered_on, entered_by
                                        )
                                        VALUES
                                        (".(($history_row['employer_id']) ? "'".$history_row['employer_id']."'" : "NULL")."
                                        ,'".$sh->student_number."'
                                        ,".(($history_row['student_work_phone']) ? "'".$history_row['student_work_phone']."'" : "NULL")."
                                        ,".(($history_row['job_title']) ? "'".$history_row['job_title']."'" : "NULL")."
                                        ,'".$arrResults[$i]['term_id']."'
                                        ,'".$arrResults[$i]['year']."'
                                        ,".(($arrResults[$i]['work_term_length']) ? "'".$arrResults[$i]['work_term_length']."'" : "NULL")."
                                        ,".(($history_row['work_email']) ? "'".$history_row['work_email']."'" : "NULL")."
                                        ,".(($history_row['salary']) ? "'".$history_row['salary']."'" : "NULL")."
                                        ,".(($history_row['company_department_id']) ? "'".$history_row['company_department_id']."'" : "NULL")."
                                        ,".(($history_row['company_city']) ? "'".$history_row['company_city']."'" : "NULL")."
                                        ,".(($history_row['company_name']) ? "'".$history_row['company_name']."'" : "NULL")."
                                        ,".(($history_row['department_name']) ? "'".$history_row['department_name']."'" : "NULL")."
                                        ,".(($history_row['company_street_address_1']) ? "'".$history_row['company_street_address_1']."'" : "NULL")."
                                        ,".(($history_row['company_street_address_2']) ? "'".$history_row['company_street_address_2']."'" : "NULL")."
                                        ,".(($history_row['company_postal_code']) ? "'".$history_row['company_postal_code']."'" : "NULL")."
                                        ,".(($history_row['company_position']) ? "'".$history_row['company_position']."'" : "NULL")."
                                        ,".(($history_row['supervisor_phone_number']) ? "'".$history_row['supervisor_phone_number']."'" : "NULL")."
                                        ,".(($history_row['supervisor_cellphone_number']) ? "'".$history_row['supervisor_cellphone_number']."'" : "NULL")."
                                        ,".(($history_row['supervisor_name']) ? "'".$history_row['supervisor_name']."'" : "NULL")."
                                        ,".(($history_row['supervisor_fax_number']) ? "'".$history_row['supervisor_fax_number']."'" : "NULL")."
                                        ,".(($history_row['supervisor_email']) ? "'".$history_row['supervisor_email']."'" : "NULL")."
                                        ,".(($history_row['work_term_street_address_1']) ? "'".$history_row['work_term_street_address_1']."'" : "NULL")."
                                        ,".(($history_row['work_term_street_address_2']) ? "'".$history_row['work_term_street_address_2']."'" : "NULL")."
                                        ,".(($history_row['work_term_city']) ? "'".$history_row['work_term_city']."'" : "NULL")."
                                        ,".(($history_row['work_term_postal_code']) ? "'".$history_row['work_term_postal_code']."'" : "NULL")."
                                        ,".(($history_row['work_term_province']) ? "'".$history_row['work_term_province']."'" : "NULL")."
                                        ,".(($history_row['work_term_country']) ? "'".$history_row['work_term_country']."'" : "NULL")."
                                        ,".(($history_row['job_id']) ? "'".$history_row['job_id']."'" : "NULL")."
                                        ,".(($history_row['company_province']) ? "'".$history_row['company_province']."'" : "NULL")."
                                        ,".(($history_row['work_term_home_phone']) ? "'".$history_row['work_term_home_phone']."'" : "NULL")."
                                        ,".(($history_row['company_country']) ? "'".$history_row['company_country']."'" : "NULL")."
                                        ,".(($history_row['supervisor_position']) ? "'".$history_row['supervisor_position']."'" : "NULL")."
                                        ,'".$sh->student_department_id."'
                                        ,".(($history_row['company_region_id']) ? "'".$history_row['company_region_id']."'" : "NULL")."
                                        ,".(($history_row['work_term_region_id']) ? "'".$history_row['work_term_region_id']."'" : "NULL")."
                                        ,".(($history_row['salary_period_id']) ? "'".$history_row['salary_period_id']."'" : "NULL")."
                                        ,".(($history_row['work_term_hours_per_week']) ? "'".$history_row['work_term_hours_per_week']."'" : "NULL")."
                                        ,".(($history_row['temp_supervisor_contact']) ? "'".$history_row['temp_supervisor_contact']."'" : "NULL")."
                                        ,".(($history_row['temp_job_contact']) ? "'".$history_row['temp_job_contact']."'" : "NULL")."
                                        ,'".$temp_date_entered."'
                                        ,'".$arrResults[$i]['start_date']."'
                                        ,'".$arrResults[$i]['end_date']."'
                                        , NOW()
                                        ,'".$auth->contact_id."'
                                        )
                                        ");
                                    $GLOBALS['dbh']->Execute($insert_sql);
                                    $temp_history_id = $GLOBALS['dbh']->Insert_ID();

                                    $sh->setPlacedFlag($arrResults[$i]['term_id'],$arrResults[$i]['year']);
                                }

                                if ($term_has_changed_flag)
                                {
                                    // this triggers a notify message in view_history.inc
                                    $arr_term_changed_msg[] = getTermName($arrResults[$i]['term_id']).' '.$arrResults[$i]['year'];

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

                                    // If the student has been placed for this term and year, then we will set their flag as PLACED, and remove their ELIGIBLE flag. 
                                    // set placed flag for the given term/year
                                    SkeletonHistory::setPlacedFlag($sh->record_id, $arrResults[$i]['term_id'], $arrResults[$i]['year']);
                                    SkeletonHistory::unsetEligibleFlag($sh->record_id, $arrResults[$i]['term_id'], $arrResults[$i]['year']);

                                    if (!wasEverEligible($sh->record_id, $arrResults[$i]['term_id'], $arrResults[$i]['year'])) {
                                        setEverEligible($sh->record_id, $arrRestuls[$i]['term_id'], $arrResults[$i]['year']);
                                    }

                                    SkeletonHistory::disableStudentApplications($sh->student_number, $arrResults[$i]['term_id'],$arrResults[$i]['year']);

                                    // update the supervisor and job contact's contact status

                                    if (($job_contact_id && $supervisor_contact_id) && $job_contact_id == $supervisor_contact_id) {
                                        updateContactStatus($supervisor_contact_id, $row['department_id'], null, $temp_history_id);
                                    }
                                    else {
                                        if ($supervisor_contact_id) {
                                            updateContactStatus($supervisor_contact_id, $row['department_id'], null, $temp_history_id);
                                        }

                                        if ($job_contact_id) {
                                            updateContactStatus($job_contact_id, $row['department_id'], null, $temp_history_id);
                                        }
                                    }
                                }
                                /*
                                // Otherwise, these are new history pieces that came from a larger (length) history record
                                elseif ($i != 0)
                                {
                                    $arr_term_changed_msg[] = getTermName($arrResults[$i]['term_id']).' '.$arrResults[$i]['year'];

                                    SkeletonHistory::setPlacedFlag($sh->record_id, $arrResults[$i]['term_id'], $arrResults[$i]['year']);
                                    SkeletonHistory::unsetEligibleFlag($sh->record_id, $arrResults[$i]['term_id'], $arrResults[$i]['year']);

                                    if (!wasEverEligible($sh->record_id, $arrResults[$i]['term_id'], $arrResults[$i]['year'])) {
                                        setEverEligible($sh->record_id, $arrResults[$i]['term_id'], $arrResults[$i]['year']);
                                    }

                                    SkeletonHistory::disableStudentApplications($sh->student_number, $arrResults[$i]['term_id'],$arrResults[$i]['year']);

                                    // update the supervisor and job contact's contact status
                                    if (($job_contact_id && $supervisor_contact_id) && $job_contact_id == $supervisor_contact_id) {
                                        updateContactStatus($supervisor_contact_id, $row['department_id'], null, $new_history_id);
                                    }
                                    else {
                                        if ($supervisor_contact_id) {
                                            updateContactStatus($supervisor_contact_id, $row['department_id'], null, $new_history_id);
                                        }

                                        if ($job_contact_id) {
                                            updateContactStatus($job_contact_id, $row['department_id'], null, $new_history_id);
                                        }
                                    }
                                }
                                */
                            } // end for
                            syncContainersWithHistory($sh->student_number,$sh->student_department_id);

                            if ($term_has_changed_flag) {
                                $arr_term_changed_msg = array_unique($arr_term_changed_msg);
                                $term_changed_substring = implode(', ', $arr_term_changed_msg);
                                $term_has_changed_msg = ('You have modified the term/year of this history record. '.getStudentName($student_num). 
                                    ' has been made ineligible for '.$term_changed_substring.'.'); 
                            }
                        } // end if($container_mode)
                    }

                    include('history/view_history.inc');
                }
            }
        }
        unset($continue);
    }

    if ($continue == 'View History')
    {
        echo("<table width='90%'>");
        echo("<tr>");
        echo("<td align='left'>");
            echo "<h3>Student Information - Placement History</h3>";
        echo("</td>");
        include_once('history/history_quickmenu.inc');
        include('history/view_history.inc');
    }
    elseif ($continue == 'Edit History')
    {
        echo("<table width='90%'>");
        echo("<tr>");
        echo("<td align='left'>");
            echo "<h3>Student Information - Placement History</h3>";
        echo("</td>");
        include_once('history/history_quickmenu.inc');
        include('history/edithistory.inc');
    }
}
?>
