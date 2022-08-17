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
 | Filename: editstudent.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Edit student info's main file.  This seems to do EVERYTHING,    |
 | but hopefully will be dismantled shortly.                                    |
 +------------------------------------------------------------------------------+

*/
include('student_info/edit/EditStudent.class');
include('student_info/functions.inc');

// Get a string of the department ids that belong to the user's group.
if (sizeof($departments_in_group) > 1)
{
    $dept_string = "'" . implode("', '", $departments_in_group) . "'";
}
else
{
    $dept_string = "'" . $auth->department . "'";
}

if ($continue == '')
{
    // The user just got here, and hasn't yet chosen a student to edit.
    include ('student_info/edit/edit_student_header.inc');
    include('student_info/edit/choose_student_to_edit.inc');
}
elseif ($continue == 'Edit Student' || $continue == 'chosen_student')
{
    // They've chosen a student to edit.  Make sure they've we've got either a record_id, student_name or student_id, and then load edit form.
    if (!$student_num && !$student_name && !$record_id)
    {
        // They haven't entered anything to indicate which student to edit.  Reload the page.
        include ('student_info/edit/edit_student_header.inc');
        error("You must either choose a student, or enter a student number or name for the student you wish to edit.");
        include('student_info/edit/choose_student_to_edit.inc');
    }
    else
    {
        // Set the student_number and record_id, and load the form.
        $error = 0;

        if ($record_id)
        {
            $sql = ("
                SELECT DISTINCT sd.student_number
                FROM student_department AS sd
                WHERE sd.record_id='" . $record_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $row = $result->FetchRow();
                $student_num = $row["student_number"];
            }
            else
            {
                // No match for this record id
                $error = 1;
                include ('student_info/edit/edit_student_header.inc');
                error("There is no student registered in the system corresponding to the record that you chose.");
                include('student_info/edit/choose_student_to_edit.inc');
            }
        }
        elseif ($student_num)
        {
            $sql = ("
                SELECT DISTINCT sd.record_id
                FROM student_department AS sd
                WHERE sd.student_number='" . $student_num . "'
                AND sd.department_id IN (" . $dept_string . ")
                ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount() > 1)
            {
                /* 
                 We've got a student with multiple departments.  If more than one of these departments is in the users group, give them a choice
                 of which to edit.
                */

                $error = 1;
                include ('student_info/edit/edit_student_header.inc');
                include('student_info/edit/choose_dept_for_edit.inc');
            }
            elseif (!$result->RecordCount())
            {
                // No student was returned.
                $error = 1;
                include ('student_info/edit/edit_student_header.inc');
                error("There is no student registered in the system corresponding to the student number you entered.");
                include('student_info/edit/choose_student_to_edit.inc');
            }
            else
            {
                $row = $result->FetchRow();
                $record_id = $row["record_id"];
            }
        }
        elseif ($student_name)
        {
            $student_name = trim($student_name);
            if (preg_match("/,.*,/", $student_name))
            {
                $error = 1;
                include ('student_info/edit/edit_student_header.inc');
                error("You may only enter one comma in the student name field.");
                include('student_info/edit/choose_student_to_edit.inc');
            }
            elseif ($student_name == ",")
            {
                $error = 1;
                include ('student_info/edit/edit_student_header.inc');
                error("You must include either a first name or last name to edit a student based on their name.");
                include('student_info/edit/choose_student_to_edit.inc');
            }
            else
            {
                $student_name = str_replace("%", "", $student_name);
                $student_names = explode(",", $student_name);
                $first_name = trim($student_names[1]);
                $last_name = trim($student_names[0]);

                $sql = ("
                    SELECT DISTINCT sd.record_id, s.student_number
                    FROM student AS s
                    INNER JOIN student_department AS sd ON sd.student_number=s.student_number
                    WHERE sd.department_id IN (" . $dept_string . ")
                    ");

                if ($first_name)
                {
                    $sql .= ("
                        AND s.first_name LIKE '" . addslashes($first_name) . "%'
                        ");
                }

                if ($last_name)
                {
                    $sql .= ("
                        AND s.last_name LIKE '" . addslashes($last_name) . "%'
                        ");
                }

                $result = $GLOBALS['dbh']->Execute($sql);

                if ($result->RecordCount() > 1)
                {
                    // We've got a student with multiple departments in this users group, or multiple students matching the information entered.
                    include ('student_info/edit/edit_student_header.inc');
                    $error = 1;
                    include('student_info/edit/choose_student_from_multiple.inc');
                }
                elseif (!$result->RecordCount())
                {
                    // No student was returned.
                    $error = 1;
                    include ('student_info/edit/edit_student_header.inc');
                    error("There is no student registered in the system corresponding to the student name that you entered.");
                    include('student_info/edit/choose_student_to_edit.inc');
                }
                else
                {
                    $row = $result->FetchRow();
                    $student_num = $row["student_number"];
                    $record_id = $row["record_id"];
                }
            }
        }

        if (!$error)
        {
            include ('student_info/edit/edit_student_header.inc');
            // If we haven't had any problems, include the edit form.
            $editStudent = new EditStudent('');
            $editStudent->loadValues($record_id);
            // Load another EditStudent object used to check later if we've got concurrent editing problems.
            $checkChanges = new EditStudent($editStudent);
            $checkChanges = new EditStudent(packObject($checkChanges));

            if ($auth->userlevel == FACULTY)
            {
                include('student_info/edit/edit_student_form_faculty.inc');
            } else {
                include('student_info/edit/edit_student_form.inc');
            }
        }
    }
}
elseif ($continue == 'Save Changes')
{
    // The user has finished editing the student, so check the inputs, and then save the changes.
    include ('student_info/edit/edit_student_header.inc');

    // Restore the student class.
    $editStudent = new EditStudent($editStudent);
    if ($auth->userlevel == FACULTY)
    {
        $editStudent->set_values_faculty($start_year, $semesters, $emergency_name, $emergency_relationship, $emergency_home_phone, 
                $emergency_work_phone, $emergency_home_fax, $emergency_work_fax, $emergency_email, $new_note, 
                $note_ids, $delete_note_ids);
    } else {
        $editStudent->set_values($first_name, $last_name, $middle_name, $preferred_name, $birth, $citizen, $high_school, $gender, $photo, $studentphoto, $deletephoto,
                $discipline_id, $academic_year, $grad_year, $coop_advisor, $advisor, $admit, $withdraw, $convocation_month, $convocation_year, $grad, $grad_gpa, $start_year, $semesters, $general_flags, $eligible_flags,
                $profile_flags, $street_address_current, $street_address_current2, $street_address_current3, $city_current, $province_current, $country_current, $region_current,
                $postal_code_current, $phone_current, $fax_current, $email, $email2, $street_address_permanent, $street_address_permanent2, $street_address_permanent3, 
                $city_permanent, $province_permanent, $country_permanent, $region_permanent, $postal_code_permanent, $phone_permanent, $fax_permanent, $emergency_name, $emergency_relationship,
                $emergency_home_phone, $emergency_work_phone, $emergency_home_fax, $emergency_work_fax, $emergency_email, $new_note, $note_ids, $delete_note_ids);
    }

    // Restore the checkChanges class (used to find concurrent editing problems).
    $checkChanges = new EditStudent($checkChanges);

    // Begin checking inputs
    if ($auth->userlevel == FACULTY)
    {
        $error_msg = $editStudent->checkValuesFaculty();
    } else {
        $error_msg = $editStudent->checkValues();
    }

    if ($error_msg)
    {
        $error_head = "The following problems were encountered while trying to save your inputs:<br /><br />";
        $error_tail = "<br /><br />Please fix the errors mentioned above, and try saving again.";
        $error_msg = $error_head . $error_msg . $error_tail;
        error($error_msg);
        if ($auth->userlevel == FACULTY)
        {
            include('student_info/edit/edit_student_form_faculty.inc');
        } else {
            include('student_info/edit/edit_student_form.inc');
        }
    }
    else
    {
        // Do we have concurrent editing problems?
        $currentStudentValues = new EditStudent('');
        $currentStudentValues->loadValues($checkChanges->record_id);
        if ($auth->userlevel == FACULTY)
        {
            $changes_array = $checkChanges->checkChangesFaculty($currentStudentValues, $editStudent);
        } else {
            $changes_array = $checkChanges->checkChanges($currentStudentValues, $editStudent);
        }

        if ($changes_array["changes_made"])
        {
            $error = 1;

            if ($changes_array["change_by"])
            {
                $change_contact = getContactName($changes_array["change_by"]);
                $change_name = "<b>" . $change_contact["first_name"] . " " . $change_contact["last_name"] . "</b>";
            }
            else
            {
                $change_name = "Someone";
            }

            $msg = ($change_name . " has modified this student since you last loaded the page.");
            $msg .= ("<br /><br />The changes you have made have NOT been saved, but are shown in <b class='black'>bold</b> below the data that can be modified.");
            $msg .= ("  Once you have modified the student as desired, press the 'Save Changes' button below to update the data.<br />");
            $msg .= ("<br />The following fields have been changed while you've been editing: <br /><b>");
            $msg .= ("<br />" . $changes_array["errors_encountered"] . "</b>");

            error($msg);

            //If they entered new notes, save them
            $new_note = $editStudent->new_note;
            $record_id = $editStudent->record_id;

            $editStudent = new EditStudent('');
            $editStudent->loadValues($record_id);

            $editStudent->new_note = $new_note;

            // Create an entity with the current status of the student, so that we may later check to ensure no concurrent editing problems.
            $checkChanges = new EditStudent($editStudent);

            if ($auth->userlevel == FACULTY)
            {
                include('student_info/edit/edit_student_form_faculty.inc');
            } else {
                include('student_info/edit/edit_student_form.inc');
            }
        }
        else
        {
            // No errors.  Fix the formatting on the date fields, remove blank entries in arrays, then save the inputs.

            $editStudent->admit = validDate($editStudent->admit);
            
            /* :EXPIRY: Date to Expire: Undetermined
                        Entered By:     Shaun
                        Date Entered:   May-30-2003

                - This line is no longer needed as we are now using selection boxes for the Convocation Date
                - Not sure when to delete this as we are keeping the old data in the database
                
            $editStudent->grad = validDate($editStudent->grad);

            */
            $editStudent->withdraw = validDate($editStudent->withdraw);
            $editStudent->birth = validDate($editStudent->birth);

            if ($auth->userlevel == FACULTY)
            {
                $error_msg = $editStudent->saveStudentInfoFaculty();
            } else {
                $error_msg = $editStudent->saveStudentInfo();
            }
            // check old and current discipline_id
            // if they're a container discipline
            //   check if they have history records
            //     if yes, add values into history records
            //        syncHistoryRecords
            
            $old_discipline_id = $currentStudentValues->discipline_id;
            $new_discipline_id = $editStudent->discipline_id;

            if ($old_discipline_id != $new_discipline_id)
            {
                include_once('history/container_functions.inc');
                $old_container_mode = useContainers($old_discipline_id);
                $new_container_mode = useContainers($new_discipline_id);

                // Student's discipline uses containers, and they're the same type of containers
                if (($old_container_mode && $new_container_mode) && $old_container_mode == $new_container_mode)
                {
                    syncContainersWithHistory($editStudent->student_number, $editStudent->department_id);
                }
                else
                {
                    $history_sql = ("
                        SELECT h.history_id, h.start_date, h.end_date, h.term_id, h.year
                        , h.work_term_length, h.work_term_hours_per_week
                        FROM history h
                        WHERE h.student_number = '".$editStudent->student_number."' AND h.department_id = '".$editStudent->department_id."'
                        ");
                    $history_result = $GLOBALS['dbh']->Execute($history_sql);
                    if ($history_result->RecordCount())
                    {
                        // no containers to term based containers
                        if (!$old_container_mode && $new_container_mode == CONTAINER_TERM)
                        {
                            while (list($history_id,$start_date,$end_date,$term_id,$year,$work_term_length,$work_term_hours_per_week) = $history_result->FetchRow())
                            {
                                // no start date, grab start date of the term, and format it into a date string
                                if (!$start_date)
                                {
                                    // this start date is really a month
                                    $term_sql = ("
                                        SELECT t.start_date
                                        FROM term t
                                        WHERE t.term_id = '".$term_id."'
                                        ");
                                    $term_result = $GLOBALS['dbh']->Execute($term_sql);
                                    $term_row = $term_result->FetchRow();
                                    $start_date = $year."-".str_pad($term_row['start_date'],2,0,STR_PAD_LEFT)."-01";
                                }

                                // no end date, get end date of the term. 
                                if (!$end_date)
                                {
                                    $end_date = getTermYearEndDate($term_id, $year);
                                }

                                if (!$work_term_length)
                                {
                                    $work_term_length = DEFAULT_WORK_TERM_LENGTH;
                                }

                                if (!$work_term_hours_per_week)
                                {
                                    $work_term_hours_per_week = DEFAULT_WORK_TERM_HOURS_PER_WEEK;
                                }

                                $update_sql = ("
                                    UPDATE history
                                    SET start_date = '".$start_date."', end_date = '".$end_date."'
                                    , work_term_length = '".$work_term_length."', work_term_hours_per_week = '".$work_term_hours_per_week."'
                                    WHERE history_id = '".$history_id."'
                                    ");
                                $GLOBALS['dbh']->Execute($update_sql);
                            }
                        }
                        // no containers to hours based containers
                        elseif (!$old_container_mode && $new_container_mode == CONTAINER_HOURS)
                        {
                            while (list($history_id,$start_date,$end_date,$term_id,$year,$work_term_length,$work_term_hours_per_week) = $history_result->FetchRow())
                            {
                                // get first day of the term 
                                if (!$start_date)
                                {
                                    $term_sql = ("
                                        SELECT t.start_date
                                        FROM term t
                                        WHERE t.term_id = '".$term_id."'
                                        ");
                                    $term_result = $GLOBALS['dbh']->Execute($term_sql);
                                    $term_row = $term_result->FetchRow();
                                    $start_date = $year."-".str_pad($term_row['start_date'],2,0,STR_PAD_LEFT)."-01";
                                }

                                if (!$work_term_hours_per_week)
                                {
                                    $work_term_hours_per_week = DEFAULT_WORK_TERM_HOURS_PER_WEEK;
                                }

                                // calculate end date based on hours worked
                                if (!$end_date)
                                {
                                    $hours_sql = ("
                                        SELECT d.history_containers_length_hours
                                        FROM discipline d
                                        WHERE d.discipline_id = '".$editStudent->discipline_id."'
                                        ");
                                    $hours_result = $GLOBALS['dbh']->Execute($hours_sql);
                                    $hours_row = $hours_result->FetchRow();

                                    $hours_per_day = $work_term_hours_per_week / 7;

                                    $num_days = floor($hours_row['history_containers_length_hours'] / $hours_per_day);

                                    $end_date = date("Y-m-d",strtotime("+".$num_days." days",strtotime($start_date)));
                                }

                                $update_sql = ("
                                    UPDATE history
                                    SET start_date = '".$start_date."', end_date = '".$end_date."'
                                    , work_term_length = ".(($work_term_length) ? "'".$work_term_length."'" : "NULL")."
                                    , work_term_hours_per_week = '".$work_term_hours_per_week."'
                                    WHERE history_id = '".$history_id."'
                                    ");
                                $GLOBALS['dbh']->Execute($update_sql);
                            } // end while
                        }
                        //assumption start date, end date, and work term hours per week are ALWAYS set in this type of container. 
                        elseif ($old_container_mode == CONTAINER_TERM && $new_container_mode == CONTAINER_HOURS)
                        {
                            while (list($history_id,$start_date,$end_date,$term_id,$year,$work_term_length,$work_term_hours_per_week) = $history_result->FetchRow())
                            {
                                $update_sql = ("
                                    UPDATE history
                                    SET work_term_length = NULL
                                    WHERE history_id = '".$history_id."'
                                    ");
                                $GLOBALS['dbh']->Execute($update_sql);
                            }
                        }
                        elseif ($old_container_mode == CONTAINER_HOURS && $new_container_mode == CONTAINER_TERM)
                        {
                            while (list($history_id,$start_date,$end_date,$term_id,$year,$work_term_length,$work_term_hours_per_week) = $history_result->FetchRow())
                            {
                                // calculate how many full-time weeks the student works 
                                $num_days = count_days($start_date, $end_date);
                                $hours_per_day = $work_term_hours_per_week / 7;
                                $total_hours = $hours_per_day * $num_days;

                                $num_full_time_weeks = $total_hours / DEFAULT_WORK_TERM_HOURS_PER_WEEK;
                                $num_full_months = round($num_full_time_weeks / 4);

                                $update_sql = ("
                                    UPDATE history
                                    SET work_term_length = '".$num_full_months."'
                                    WHERE history_id = '".$history_id."'
                                    ");
                                $GLOBALS['dbh']->Execute($update_sql);
                            }
                        }
                        elseif ($old_container_mode && !$new_container_mode)
                        {
                            //:TODO: Need a way to convert containers back into non-container history records
                        }

                        if ($new_container_mode)
                        {
                            syncContainersWithHistory($editStudent->student_number, $editStudent->department_id);
                        }
                    }

                    if ($new_container_mode)
                    {
                        notify("History records have been modified to fit this discipline's history record type.");
                    }
                }
            }

            if ($error_msg)
            {
                // An error occurred while trying to save the student.
                error($error_msg);
                if ($auth->userlevel == FACULTY)
                {
                    include('student_info/edit/edit_student_form_faculty.inc');
                } else {
                    include('student_info/edit/edit_student_form.inc');
                }
            }
            else
            {
                $msg = ("<b>" . $editStudent->first_name . " " . $editStudent->last_name . "'s</b> information has been successfully updated.");
                notify($msg);

                // Reload values from the database, just to be safe.
                $record_id = $editStudent->record_id;
                $editStudent = new EditStudent('');
                $editStudent->loadValues($record_id);

                include('student_info/view/view_student_form.inc');
            }
        }
    }
}

?>
