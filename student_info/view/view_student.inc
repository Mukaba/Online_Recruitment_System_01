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
 | Filename: view_student.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file acts as a single point of contact with the view       |
 | student module. This file directs traffic to other parts of this module.     |
 +------------------------------------------------------------------------------+

*/

if (sizeof($departments_in_group) > 1)
{
    $dept_string = "'" . implode("', '", $departments_in_group) . "'";
}
else
{
    $dept_string = "'" . $auth->department . "'";
}

// User has just arrived to view/search page. Continue has not been set yet.  
if ($continue == '')
{
    // Check to see if the user prefers to use advanced search or simple search. 
    $sql = ("
        SELECT ci.search_use_advanced
        FROM contact_internal AS ci
        WHERE ci.department_id = '".$auth->department."' AND (ci.login_id = '".$login_id."' OR ci.netlink_id = '".$login_id."')
        ");
    $results = $GLOBALS['dbh']->Execute($sql);
    $row = $results->FetchRow();

    if ($results->RecordCount() == 1 && $row['search_use_advanced'] == 1)
    {
        $continue = 'Advanced Search';
        include_once('student_info/view/view_student_header.inc');
        include('student_info/view/advanced_search.inc');
    }

    else
    {
        include_once('student_info/view/view_student_header.inc');
        include('student_info/view/choose_student_to_view.inc');
    }
}

elseif ($continue == 'Simple Search' || $continue == 'Go')
{
    include_once('student_info/view/view_student_header.inc');
    include('student_info/view/choose_student_to_view.inc');
}

elseif ($continue == 'Advanced Search')
{
    if($btnSearch != "Manage My Saved Searches")  //added to allow the Manage Searches button to work with out the advanced search page coming up 
    {    
        include_once('student_info/view/view_student_header.inc');
        include('student_info/view/advanced_search.inc');
    }
    else                                          // If the user wants to edit a saved search
    {
        include("search/manage_searches.inc");
    }
}

elseif ($continue == 'Advanced Search Results')
{
    include_once('student_info/view/view_student_header.inc');
    include('student_info/view/advanced_search_results.inc');
}

elseif ($continue == 'View Student' || $continue == 'Chosen Student')
{
    // They've chosen a student to edit.  Make sure they've we've got either a record_id, student_name or student_id, and then load edit form.
    if (!$student_num && !$student_name && !$record_id)
    {
        // They haven't entered anything to indicate which student to edit.  Reload the page.
        include_once('student_info/view/view_student_header.inc');
        error("You must either choose a student, or enter a student number or name for the student you wish to view.");
        include('student_info/view/choose_student_to_view.inc');
    }

    else
    {
        // Set the student_number and record_id, and load the form.
        $error = 0;

        if ($record_id)
        {
            $sql = ("
                SELECT DISTINCT sd.student_number, sd.department_id
                FROM student_department AS sd
                WHERE sd.record_id='" . $record_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $student_num = $row["student_number"];
            $department_id = $row['department_id'];
            $continue = 'Chosen Student';
        }
        elseif ($student_num)
        {
            $sql = ("
                SELECT DISTINCT sd.record_id, sd.department_id
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
                include_once('student_info/view/view_student_header.inc');
                include('student_info/view/choose_dept_for_view.inc');
            }
            else
            {
                $row = $result->FetchRow();
                $record_id = $row["record_id"];
                $department_id = $row['department_id'];
                $continue = 'Chosen Student';
            }
        }
        elseif ($student_name)
        {
            $student_name = trim($student_name);
            if (preg_match("/,.*,/", $student_name))
            {
                $error = 1;
                include_once('student_info/view/view_student_header.inc');
                error("You may only enter one comma in the student name field.");
                include('student_info/view/choose_student_to_view.inc');
            }
            elseif ($student_name == ",")
            {
                $error = 1;
                include_once('student_info/view/view_student_header.inc');
                error("You must include either a first name or last name to edit a student based on their name.");
                include('student_info/view/choose_student_to_view.inc');
            }
            else
            {
                // Strip wild card character from student name. 
                $student_name = str_replace("%", "", $student_name);
                $student_names = explode(",", $student_name);
                $first_name = trim($student_names[1]);
                $last_name = trim($student_names[0]);

                $sql = ("
                    SELECT DISTINCT sd.record_id, s.student_number, sd.department_id
                    FROM student AS s
                    INNER JOIN student_department AS sd ON sd.student_number=s.student_number
                    WHERE sd.department_id IN (" . $dept_string . ")
                    ");

                if ($first_name)
                {
                    $sql .= ("
                        AND s.first_name LIKE '" . $first_name . "%'
                        ");
                }

                if ($last_name)
                {
                    $sql .= ("
                        AND s.last_name LIKE '" . $last_name . "%'
                        ");
                }

                $result = $GLOBALS['dbh']->Execute($sql);

                if ($result->RecordCount() > 1)
                {
                    // We've got a student with multiple departments in this users group, or multiple students matching the information entered.
                    $error = 1;
                    include_once('student_info/view/view_student_header.inc');
                    include('student_info/view/choose_student_from_multiple_view.inc');
                }
                else
                {
                    $row = $result->FetchRow();
                    $student_num = $row["student_number"];
                    $record_id = $row["record_id"];
                    $department_id = $row['department_id'];
                    $continue = 'Chosen Student';
                }
            }
        }

        if ($department_id != "0" && !in_array($department_id, $departments_in_group))
        {
            // :TODO: No idea what $page and $PDF does.... it's related to resume, but we're probably going to tear it down!
            $page = "";
            unset( $PDF );

            include_once('student_info/view/view_student_header.inc');
            error("You cannot access this student because the student does not belong in your group or department, or the student was not found.");
            include('student_info/view/choose_student_to_view.inc');
            $error = 1;
        }

    }//End Else
}//End elseif
elseif($continue == "save search")
{
    include_once('student_info/view/view_student_header.inc');
}
else
{
    include_once('student_info/view/view_student_header.inc');
    include('student_info/view/choose_student_to_view.inc');
}

#if ($error == 0 && $record_id <> NULL)
if ($continue == "Chosen Student" && $error == 0 && $record_id <> NULL)
{
    if (!$student_num)
    {
        $sql = "SELECT student_number
                FROM student_department
                WHERE record_id = '".$record_id."'";
        $results = $GLOBALS['dbh']->Execute($sql);
        $row = $results->FetchRow();
        $student_num = $row['student_number'];
    }

    // If we haven't had any problems finding the student, load up all the information about this student, and display it. 
    include('student_info/edit/EditStudent.class');
    $editStudent = new EditStudent('');
    $editStudent->loadValues($record_id);

    //  Build Code for previous and next button... $left_number being the student number to go to if the user clicks on previous,
    //  and $right_number .. next. 

    $student_number_array = explode(",", $student_list);
    //get student numbers of the students the next and previous buttons will take us to
    for ($i = 0; $i < sizeof($student_number_array); $i++) 
    {
        if ($student_number_array[$i] == $record_id) 
        {
            $position = $i;
            $left_pos = $i - 1;
            $right_pos = $i + 1;
            break;
        }
    }
    
    if ($left_pos < 0) 
    { 
        $left_pos = sizeof($student_number_array) - 1; 
    }
    
    if ($right_pos > sizeof($student_number_array) - 1) 
    { 
        $right_pos = 0; 
    }

    $left_number = $student_number_array[$left_pos];
    $right_number = $student_number_array[$right_pos];

    include_once('student_info/view/view_student_header.inc');
    include('student_info/view/view_student_form.inc');
} 

?>

