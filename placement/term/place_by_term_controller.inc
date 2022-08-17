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
 | Filename: place_by_term_controller.inc                                       |
 +------------------------------------------------------------------------------+
 | Description: Control file for placing students by term. It handles control   |
 |              flow logic and form variable handling. Near the bottom of the   |
 |              code is traffic direction instructions. It tells you what order |
 |              the files should be run for this placement process.             |
 +------------------------------------------------------------------------------+

*/

include('placement/placement_header.inc');
include_once('placement/Placement.class');
include_once('history/container_functions.inc');

// Initially we will get a slew of student numbers as we're placing many students at a time.
// We separate these student numbers into objects, and the objects are stored in these various 
// arrays. arr_ready is an array for storing placement objects that are ready to be placed right 
// away because all required information is set in the object. arr_multiple_dept means the student
// has been detected to be current in multiple departments, so we need additional information. 
// arr_container means the student is in a discipline that uses history containeres, we need more
// information.

if ($arr_ready && is_string($arr_ready))
{
    $arr_ready = unpackObject(($arr_ready));
}
if ($arr_multiple_dept && is_string($arr_multiple_dept))
{
    $arr_multiple_dept = unpackObject(($arr_multiple_dept));
}
if ($arr_container && is_string($arr_container))
{
    $arr_container = unpackObject(($arr_container));
}

/* Handy for debugging
print "<pre>";
print_r($placements);
print "</pre>";
*/

// coming from term_results.inc,  we are expecting an array of placements
// with values that look like student_number&job_id
if ($level == "multiple_department_check")
{
    $arr_ready = array();
    $arr_multiple_dept = array();
    $arr_container = array();
    $error_array = array();

    if (sizeof($placements) && is_array($placements))
    {
        // $placements is the incoming variable we're looking for. 
        foreach($placements as $placement_string)
        {
            $arr_placement_values = explode("&",$placement_string);
            $student_number = $arr_placement_values[0];
            $job_id = $arr_placement_values[1];

            $obj_placement = new Placement('');
            $obj_placement->setPlaceByStudentFormValues($student_number, null, $term_id, $year);

            // aggregate all of our error messages to one array. 
            $obj_error_array = $obj_placement->validatePlaceByStudentFormValues();
            $error_array = array_merge((array) $error_array, (array) $obj_error_array);

            // if we have an error, we won't carry the placement through the process. 
            if (sizeof($obj_error_array) == 0)
            {
                $obj_placement->setJobValues($job_id);

                // multiple department check
                $arr_department_ids = $obj_placement->getArrayOfCurrentDepartments();
                if (sizeof($arr_department_ids) == 1)
                {
                    $obj_placement->setStudentDepartmentValues($arr_department_ids[0]);
                    // ready to place student now, store the object in the ready array. 
                    if (!$obj_placement->container_mode)
                    {
                        $arr_ready[] = $obj_placement;
                    }
                    // they use containers
                    else
                    {
                        $arr_container[] = $obj_placement;
                    }
                }
                // multiple department student
                else
                {
                    $arr_multiple_dept[] = $obj_placement;
                }
            }
                
            $obj_error_array = null;
            $arr_department_ids = null;
            $arr_placements_values = null;
            $student_number = null;
            $job_id = null;
        }
        if (sizeof($error_array) && is_array($error_array))
        {
            // don't want duplicate error messages. 
            $error_array = array_unique($error_array);
        }

        // time to direct traffic to proper spot.

        // we have an array of students ready to be placed, and we've got no students that are in multiple departments, and none
        // of them use containers, so lets go ahead and place them. 
        if (sizeof($arr_ready) && !sizeof($arr_container) && !sizeof($arr_multiple_dept))
        {
            $level = "save_placements";
        }
        elseif (sizeof($arr_multiple_dept))
        {
            $level = "multiple_department_check"; 
        }
        elseif (sizeof($arr_container))
        {
            $level = "container_info";
        }
    }
    else
    {
        $level = "term_results";
        error("No students selected for placement. Please select students to place.");
    }
}

// coming from either term results or multiple department check  
if ($level == "container_info")
{
    // if we still have an array of multiple department students, but no accompanying record ids that should have been given from 
    // the multiple_department_check page, something's wrong, so go back to that page. 
    if (is_array($arr_multiple_dept) && sizeof($arr_multiple_dept) && (!sizeof($arr_record_id) || !is_array($arr_record_id)))
    {
        $level = "multiple_deptartment_check";
    }
    // we definitely just came from multiple_department_check as we have an array of record ids, lets use them
    elseif (is_array($arr_record_id) && sizeof($arr_record_id) && is_array($arr_multiple_dept) && sizeof($arr_multiple_dept))
    {
        foreach ($arr_multiple_dept as $key => $placement)
        {
            if (is_a($placement,"Placement"))
            {
                foreach($arr_record_id as $student_number => $record_id)
                {
                    if ($placement->student_number == $student_number)
                    {
                        $placement->setStudentDepartmentValues($record_id);

                        if ($placement->container_mode)
                        {
                            // remove element from multiple department array and place into container array. 
                            unset($arr_multiple_dept[$key]);
                            $arr_container[] = $placement;
                        }
                        else
                        {
                            // remove element from multiple department array and place into the ready array. 
                            unset($arr_multiple_dept[$key]);
                            $arr_ready[] = $placement;
                        }
                        break;
                    }
                    $student_number = null;
                    $record_id = null;
                }
            }
            $key = null;
            $placement = null;
        }
    }

    // We still have placement records where students are in multiple departments. 
    if (sizeof($arr_multiple_dept))
    {
        $error_array[] = "You must select a department for each student.";
        $level = "multiple_department_check";
    }
    // Move on to container information
    elseif (sizeof($arr_container) && is_array($arr_container))
    {
        $level = "container_info";
    }
    // All ready, move to save placements
    else
    {
        $level = "save_placements";
    }
}

// just came from either term results, or multiple department check, or container information
if ($level == "save_placements")
{
    // we still have objects in the container array, but we don't have the associated container information from the container_info page,
    // go back to that page. 
    if (is_array($arr_container) && sizeof($arr_container) && (!sizeof($arr_container_info) || !is_array($arr_container_info)))
    {
        $level = "container_info";
    }
    // we have objects in the container array, and we've got container information, so lets use it. 
    elseif (is_array($arr_container_info) && sizeof($arr_container_info) && is_array($arr_container) && sizeof($arr_container))
    {
        foreach ($arr_container as $key => $placement)
        {
            if (is_a($placement,"Placement"))
            {
                foreach($arr_container_info as $student_number => $value)
                {
                    if ($placement->student_number == $student_number)
                    {
                        $start_date = $arr_container_info[$student_number]['start_date'];
                        $end_date = $arr_container_info[$student_number]['end_date'];
                        $work_term_length = $arr_container_info[$student_number]['work_term_length'];
                        $work_term_hours_per_week = $arr_container_info[$student_number]['work_term_hours_per_week'];
                        $keep_eligible_flag = $arr_container_info[$student_number]['keep_eligible_flag'];

                        if ($placement->container_mode == CONTAINER_TERM)
                        {
                            $placement->setContainerInfoTermValues($start_date, $work_term_length, $work_term_hours_per_week, $keep_eligible_flag);
                            $obj_error_array = $placement->validateContainerInfoTermValues();

                            // great, no errors, remove object from container array and place into ready array. 
                            if (!sizeof($obj_error_array))
                            {
                                unset($arr_container[$key]);
                                $arr_ready[] = $placement;
                            }
                            else
                            {
                                // commit copy of placement back into array of containers
                                $arr_container[$key] = $placement;
                                $error_array = array_merge((array) $error_array,(array) $obj_error_array);
                            }
                        }

                        elseif ($placement->container_mode == CONTAINER_HOURS)
                        {
                            $placement->setContainerInfoHoursValues($start_date, $end_date, $work_term_hours_per_week, $keep_eligible_flag);
                            $obj_error_array = $placement->validateContainerInfoHoursValues();
                            if (!sizeof($obj_error_array))
                            {
                                unset($arr_container[$key]);
                                $arr_ready[] = $placement;
                            }
                            else
                            {
                                // commit copy of placement back into array of containers
                                $arr_container[$key] = $placement;
                                $error_array = array_merge((array) $error_array,(array) $obj_error_array);
                            }
                        }

                        $start_date = null;
                        $end_date = null;
                        $work_term_length = null;
                        $work_term_hours_per_week = null;
                        $obj_error_array = null;

                        break;
                    }
                    $student_number = null;
                }
            }
            $key = null;
            $placement = null;
        }

    }
    
    if (sizeof($error_array) && is_array($error_array))
    {
        $error_array = array_unique($error_array);
    }

    // We still have placement records where students require container information
    if (sizeof($arr_container) && is_array($arr_container))
    {
        $level = "container_info";
    }
    else
    {
        $level = "save_placements";
    }
}

// traffic directions

switch($level) {
    case "place_by_term":
        include('placement/term/place_by_term_form.inc');
        break;
    case "term_results":
        include('placement/term/term_results.inc');
        break;
    case "multiple_department_check":
        include('placement/term/term_multiple_student_dept_form.inc');
        break;
    case "container_info":
        include('placement/term/term_container_info_form.inc'); 
        break;
    case "save_placements":
        include('placement/save_placements.inc');
        break;
    default:
        include('placement/term/place_by_term_form.inc');
}

?>
