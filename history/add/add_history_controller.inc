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
 | Filename: add_history_controller.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This is the controller which directs traffic and sends users to |
 |              the correct destination. Near the bottom of the code are        |
 |              include commands. They show the order of the process of placing |
 |              a student in an own job.                                        |
 +------------------------------------------------------------------------------+

*/

// when we go to the edit history page, it already has the line below, so don't out put that. 
if ($level != "save_and_edit")
{
?>
   </td><td><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>
<?php
}
?>
<?php

include("history/add/AddHistory.class");
include_once("history/container_functions.inc");

if ($addHistory && is_string($addHistory)) {
    $addHistory = unpackObject(($addHistory));
    $addHistory = new AddHistory($addHistory);
}
else {
    $addHistory = new AddHistory('');
}

//user is trying to get to a level, but no addHistory object exists, so send them off to the main form as all other pages
//require the addHistory object. 
if ($level && !is_object($addHistory))
{
    $level = "main_form";
}

// validation steps

// we've just come from the main form, and we're headed for multiple department checking. 
if ($level == "check_multiple_departments")
{
    $addHistory->setMainFormValues($company_name, $student_num, $term_id, $year);
    $error_array = $addHistory->validateMainFormValues();
    // Error in form values, send back to main form for re-entry. 
    if (sizeof($error_array))
    {
        $level = "main_form";
    }
    else
    {
        $arr_department_list = $addHistory->getArrayOfCurrentDepartments();
        // Student not in multiple departments (ie, not current in multiple departments)
        if(sizeof($arr_department_list) == 1)
        {
            $addHistory->setStudentDepartmentValues($arr_department_list[0]);
            //student only in one department, bypass check_multiple_departments form
            $level = "select_company";
        }
    }
}
// user has just come the main form or from the multiple departments student page. 
if ($level == "select_company")
{
    // User has just selected a student that is in multiple departments. Save the unique record id.
    if ($record_id)
    {
        $addHistory->setStudentDepartmentValues($record_id);
    }
    $addHistory->setContainerMode(useContainers($addHistory->student_number, $addHistory->student_department_id));

    if (!$addHistory->container_mode)
    {
        if ($addHistory->isStudentPlaced())
        {
            $level = "main_form";
            $error_array[] = "This student has already been placed for ".getTermName($addHistory->term_id)." ".$addHistory->year.". Please select a different term and year.";
        }
    }

    if (!$addHistory->isStudentEligible())
    {
        $level = "main_form";
        $error_array[] = "This student is not eligible to be placed for ".getTermName($addHistory->term_id)." ".$addHistory->year.". If you wish to place this student, make the student eligible first.";
    }
}
if ($level == "container_info_form")
{
    $addHistory->setCompanyDivisionValues($division_id);
    if (!$addHistory->container_mode)
    {
        $level = "confirm";
    }
    elseif ($addHistory->container_mode == CONTAINER_HOURS || $addHistory->container_mode == CONTAINER_TERM)
    {
        $level = "container_info_form";
    }
}
if ($level == "confirm")
{
    // user coming from container info form to the final confirmation page. 
    if ($addHistory->container_mode == CONTAINER_HOURS)
    {
        $addHistory->setContainerInfoHoursValues($start_date, $end_date, $work_term_hours_per_week, $keep_eligible_flag);
        $error_array = $addHistory->validateContainerInfoHoursValues();
        if (sizeof($error_array))
        {
            $level = "container_info_form";
        }

        $arr_history_records = $addHistory->generateHistoryRecordsHours();
    }
    elseif ($addHistory->container_mode == CONTAINER_TERM)
    {
        $addHistory->setContainerInfoTermValues($start_date, $work_term_length, $work_term_hours_per_week, $keep_eligible_flag);
        $error_array = $addHistory->validateContainerInfoTermValues();
        if (sizeof($error_array))
        {
            $level = "container_info_form";
        }
    }
}
if ($level == "dont_save_and_new") 
{
    $addHistory = NULL;
    $level = "main_form";
}

/* Handy for debugging purposes. 
print "<pre>";
print_r($addHistory);
print "</pre>";
*/

// controller
    
if ($level=="main_form" || $level==NULL){
	include('history/add/main_form.inc');
}
if ($level=="check_multiple_departments"){
    include("history/add/student_multiple_department_form.inc");
}
if ($level=="select_company"){
	include('history/add/company_department.inc');
}
if ($level=="container_info_form"){
    include("history/add/container_info_form.inc");
}
if ($level=="confirm"){
	include('history/add/confirm.inc');
}
if ($level=="save_and_edit"){
	include('history/add/save_add_changes.inc');
    // Going to edit history (relies on $history_id being set in save_add_changes.inc)
    include_once('history/EditHistory.class');
    include('history/history_header.inc');
    include('history/edithistory.inc');
}
if ($level=="save_and_new") {
    include('history/add/save_add_changes.inc');
    notify("The student has been successfully placed.");
    include('history/add/main_form.inc');
}
?>
