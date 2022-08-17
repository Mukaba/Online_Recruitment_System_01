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
 | Filename: deleted.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: This file actually deletes the student from the database.       |
 +------------------------------------------------------------------------------+

*/
?>
</td><td align="left"><img src="misc/images<?php echo $_SESSION['SESSION_colorscheme']; ?>/topbar_right.jpg" alt='' /></td></tr></table>
<h3>Student Information - Deleted Student</h3>
<br />
<?php
    $back_link = "<p align='center'><a href='".$PHP_SELF."&amp;select=delete_student'>Back to delete student</a></p>";
    if ($student_num == NULL) 
    {
        error("Please supply a student number.");
        print $back_link;
    }

    // Reject if student number is not an integer or a script kiddy is trying to hack us by throwing in a wild character. 
    elseif (!preg_match("/^(\d*)$/",$student_num) || $student_num == "%") 
    {
        error("Please supply a valid student number.");
        print $back_link;
    }

    elseif ($dept_id == NULL) 
    {
        error("Please supply a department");
        print $back_link;
    }

    // Reject if department is not an integer or a script kiddy is trying to hack us by throwing in a wild character. 
    elseif (!preg_match("/^(\d*)$/",$dept_id) || $dept_id == "%") 
    {
        error("Please supply a valid department.");
        print $back_link;
    }
    
    else 
    {
        $student_name = getStudentName($student_num);

        // We get the student's record ID again from the database. We don't pass the record ID from verify.inc for security reasons.
        $sql = "SELECT record_id
                FROM student_department
                WHERE student_number = '".$student_num."' AND department_id = '".$dept_id."'";
        $results = $GLOBALS['dbh']->Execute($sql);
        if ($results->RecordCount() == 1) 
        {
            $row = $results->FetchRow();
            $record_id = $row['record_id'];

            // Delete all the department specific student records.
            $sql = "DELETE FROM eligible_placed WHERE record_id = '".$record_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "DELETE FROM history WHERE student_number = '".$student_num."' AND department_id = '".$dept_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "DELETE FROM history_container WHERE student_number = '".$student_num."' AND department_id = '".$dept_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "DELETE FROM semesters_table WHERE record_id = '".$record_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "DELETE FROM student_department WHERE record_id = '".$record_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "DELETE FROM student_flags_join WHERE record_id = '".$record_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "DELETE FROM student_notes WHERE record_id = '".$record_id."'";
            $GLOBALS['dbh']->Execute($sql);

            // If the student is not in multiple departments, we delete everything related to the student.
            if ($multiple_department_flag == false) 
            {
                $sql = "DELETE FROM applications WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM coverletter WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM interview_time WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM preferences WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM resume WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM student WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM student_jobs_flagged WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM students_shortlisted WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
                $sql = "DELETE FROM transcript WHERE student_number = '".$student_num."'";
                $GLOBALS['dbh']->Execute($sql);
            }

            notify($student_name." (".$student_num.") has been deleted from the database.");
            print $back_link;
        }

        else 
        {
            notify("An error has occured while deleting ".$student_name." from the database. A valid record ID was not found");
            print $back_link;
        }
    }//End else
?>
