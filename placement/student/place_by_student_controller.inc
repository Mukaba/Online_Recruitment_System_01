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
 | Filename: place_by_student_controller.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: Small file used to determine what page to load.                 |
 +------------------------------------------------------------------------------+

*/

include("placement/Placement.class");
include_once("history/container_functions.inc");
include('placement/placement_header.inc');

if ($placement && is_string($placement)) {
    $placement = unpackObject(($placement));
    $placement = new Placement($placement);
}
else {
    $placement= new Placement('');
}

//They've come to this page, and want to get to a certain level, but no placement object exists. Move them back to the first page (place_by_student.inc)
//as all the other pages depend on the object existing. 

if ($level && !is_object($placement))
{
    $level = "place_by_student";
}

// The user has entered a student name or student number, so we have to check if the given student is in
// we validate that information to make sure the student name/number is valid. If it is invalid, we bounce
// them back to the initial place_by_student screen. If the student is in multiple departments, we load 
// up the multiple_department_check screen which will let the user tell us what department the student's
// placement should be a part of. If the student is not in multiple departments and the number/name is valid
// then we go on to the jobs that the student can be placed in. 
if ($level == "multiple_department_check") 
{
    // we're coming from the place_by_student file, and we're setting the values
    if ($student_num || $student_num_select)
    {
        if (($student_num && $student_num_select) || (!$student_num && $student_num_select))
        {
            $student_num = $student_num_select;
        }

        $placement->setPlaceByStudentFormValues($student_num, NULL, $term_id, $year);
        $error_array = $placement->validatePlaceByStudentFormValues();
    }
    elseif ($student_name)
    {
        $placement->setPlaceByStudentFormValues(NULL, $student_name, $term_id, $year);
        $error_array = $placement->validatePlaceByStudentFormValues();
    }
    else
    {
        $error_array[] = "Please enter a student number or a student name.";
    }

    if (sizeof($error_array))
    {
        $level = "place_by_student";
    }
    elseif($placement->student_number)
    {
        if (!$placement->hasAppliedToJobs()) 
        {
            $level = "place_by_student";

            // Use these functions to grab a first name and last name
            $arr_record_ids = $placement->getArrayOfCurrentDepartments();

            if (sizeof($arr_record_ids)) {
                $placement->setStudentDepartmentValues($arr_record_ids[0]);
                
                $error_array[] = $placement->first_name." ".$placement->last_name." (".$placement->student_number.") has not applied for any jobs for 
                                 ".getTermName($placement->term_id). " ".$placement->year;
            }
            // otherwise, student is not current
            else {
                $error_array[] = getStudentName($placement->student_number)." (".$placement->student_number.") is not current, and therefore 
                                 cannot be placed for ".getTermName($placement->term_id)." ".$placement->year;
            }
        }
        else
        {
            $arr_record_ids = $placement->getArrayOfCurrentDepartments();
            if (sizeof($arr_record_ids) == 1) 
            {
                $placement->setStudentDepartmentValues($arr_record_ids[0]);
                $level = "show_jobs";
            }
            // else continue onto multiple_department_check
        }
    }
}

// We check to see if the student has actually applied to any jobs. If not, we bounce the user
// back to the initial place_by_student screen. 
if ($level == "show_jobs")
{
    if (!$placement->record_id && $record_id)
    {
        $placement->setStudentDepartmentValues($record_id);

        if (!$placement->hasAppliedToJobs())
        {
            $level = "place_by_student";
            $error_array[] = $placement->first_name." ".$placement->last_name." (".$placement->student_number.") has not applied for any jobs for 
                             ".getTermName($placement->term_id). " ".$placement->year;
        }
        // else continue on to show jobs 
    }
}

if ($level == "container_info_form")
{
    $placement->setJobValues($job_id);
    $error_array = $placement->validateJobValues();
    if (sizeof($error_array))
    {
        $level = "show_jobs";    
    }

    elseif (!$placement->container_mode)
    {
        $level = "save_placements";
    }
    // else continue onto container_info_form
}
if ($level == "save_placements")
{
    // user coming from container info form to the final confirmation page. 
    if ($placement->container_mode == CONTAINER_HOURS)
    {
        $placement->setContainerInfoHoursValues($start_date, $end_date, $work_term_hours_per_week, $keep_eligible_flag);
        $error_array = $placement->validateContainerInfoHoursValues();
        if (sizeof($error_array))
        {
            $level = "container_info_form";
        }

        $arr_history_records = $placement->generateHistoryRecordsHours();
    }
    elseif ($placement->container_mode == CONTAINER_TERM)
    {
        $placement->setContainerInfoTermValues($start_date, $work_term_length, $work_term_hours_per_week, $keep_eligible_flag);
        $error_array = $placement->validateContainerInfoTermValues();
        if (sizeof($error_array))
        {
            $level = "container_info_form";
        }
    }
}

/* Handy for debugging.
print "<pre>";
print_r($placement);
print "</pre>";
*/

switch ($level) {
    case "place_by_student":
        include('placement/student/place_by_student_form.inc');
        break;
    case "multiple_department_check":
        include('placement/student/multiple_student_dept_form.inc');
        break;
    case "show_jobs":
        include('placement/student/show_jobs.inc');
        break;
    case "container_info_form":
        include('placement/student/container_info_form.inc');
        break;
    case "save_placements":
        include('placement/save_placements.inc');
        break;
    default:
        include('placement/student/place_by_student_form.inc');
}

?>
