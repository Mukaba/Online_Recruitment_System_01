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
 | Filename: verify.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: Shows the user what tables this student exists in. Author:      |
 | -CHaN Note: The check mark image is hardcoded to the following path:         |
 | 'misc/images/smchk_bl.gif'                                                   |
 +------------------------------------------------------------------------------+

*/
?>
</td><td align="left"><img src="misc/images<?php echo $_SESSION['SESSION_colorscheme']; ?>/topbar_right.jpg" alt='' /></td></tr></table>
<h3>Student Information - Verify Delete Student</h3>
<br />
<?php
    $back_link = "<p align='center'><a href='".$PHP_SELF."&amp;select=delete_student'>Back to delete student</a></p>";
    $multiple_department_flag = false;

    if ($student_num == NULL) 
    {
        error("Please supply a student number.");
        print $back_link;
    }
    
    elseif ($dept_id == NULL) 
    {
        error("Please supply a department.");
        print $back_link;
    }
    
    // Reject if department is not an integer or a script kiddy is trying to hack us by throwing in a wild character. 
    elseif (!preg_match("/^(\d*)$/",$dept_id) || $dept_id == "%") 
    {
        error("Please supply a valid department.");
        print $back_link;
    }
    
    // Reject if student number is not an integer or a script kiddy is trying to hack us by throwing in a wild character. 
    elseif (!preg_match("/^(\d*)$/",$student_num) || $student_num == "%") 
    {
        error("Please supply a valid student number.");
        print $back_link;
    }
    
    else 
    {
        $sql = "SELECT record_id
                FROM student_department
                WHERE student_number = '".$student_num."' AND department_id = '".$dept_id."'";
        $results = $GLOBALS['dbh']->Execute($sql);
        if ($results->RecordCount() == 0) 
        {
            error("This student number does not exist in this department");
            print $back_link;
        }
        else 
        {
            $row=$results->FetchRow();
            $record_id = $row['record_id'];
            

            $sql = "SELECT login_id
                    FROM student
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            $login_id = $row['login_id'];

            //Multiple Department
            $sql = "SELECT department_id 
                    FROM student_department
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            if ($results->RecordCount() > 1) 
            {
                $dept_string = "";
                while($row=$results->FetchRow()) 
                {
                    if ($row['department_id'] == 0)
                    {
                        $dept_string .= " 0,";
                    }
                    // We don't want to display the current department we have chosen. So do nothing.
                    elseif($row['department_id'] == $dept_id)
                    {
                    }
                    else 
                    {
                        $sql = "SELECT department_name
                                FROM department
                                WHERE department_id = '".$row['department_id']."'";
                        $dept_results = $GLOBALS['dbh']->Execute($sql);
                        $dept_row = $dept_results->FetchRow();
                        $dept_string .= " ".$dept_row['department_name'].",";
                    }
                }
                $dept_string = substr("$dept_string", 0, -1);               
                // Output occurs below
                $multiple_department_flag = true;
            }
            //
            
            // each query must follow this array's order.
            $table = array('Applications','Cover Letters','Eligible/Placed','History','Interview Time','Preferences','Resumes','Semesters'
                          ,'Student','Student Department','Student Flags','Student Jobs Flagged','Student Notes','Student Shortlisted','Transcripts'
                          );

            // The checked array keeps track of which tables a student exists in. In addition, blue checkmarks are assigned to tables that are student specific. 
            // Meaning, tables that depend on student_number only. Otherwise, green checkmarks are assigned to tables that are department specific. Meaning
            // tables that depend on (student_number AND department_id) OR (record_id).
            $checked = array();
            $count=0;

            // Applications
            $sql = "SELECT COUNT(application_id) as num_records
                    FROM applications
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            // 

            //Cover Letters
            $sql = "SELECT count(coverletter_id) as num_records
                    FROM coverletter
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //
            
            //Eligible/Placed
            $sql = "SELECT count(record_id) as num_records
                    FROM eligible_placed
                    WHERE record_id = '".$record_id."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "green";
            }
            $count++;
            //

            //History
            $sql = "SELECT count(history_id) as num_records
                    FROM history
                    WHERE student_number = '".$student_num."' AND department_id = '".$dept_id."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "green";
            }
            $count++;
            //

            //Interview Time
            $sql = "SELECT count(time_id) as num_records
                    FROM interview_time
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //

            //Preferences
            $sql = "SELECT count(time_id) as num_records
                    FROM interview_time
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //

            //Resumes
            $sql = "SELECT count(resume_id) as num_records
                    FROM resume
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //

            //Semesters Table
            $sql = "SELECT count(record_id) as num_records
                    FROM semesters_table
                    WHERE record_id = '".$record_id."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "green";
            }
            $count++;
            //

            //Student
            $sql = "SELECT count(student_number) as num_records
                    FROM student
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //

            //Student Department
            $sql = "SELECT count(student_number) as num_records
                    FROM student_department
                    WHERE student_number = '".$student_num."' AND department_id = '".$dept_id."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "green";
            }
            $count++;
            //

            //Student Flags
            $sql = "SELECT count(record_id) as num_records
                    FROM student_flags_join
                    WHERE record_id = '".$record_id."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "green";
            }
            $count++;
            //

            //Student Jobs Flagged
            $sql = "SELECT count(student_number) as num_records
                    FROM student_jobs_flagged
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //

            //Student Notes
            $sql = "SELECT count(student_notes_id) as num_records
                    FROM student_notes
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //

            //Student Shortlisted
            $sql = "SELECT count(student_number) as num_records
                    FROM students_shortlisted
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //

            //Transcripts
            $sql = "SELECT count(student_number) as num_records
                    FROM transcript
                    WHERE student_number = '".$student_num."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();
            if ($row['num_records'] > 0) 
            {
                $checked[$count] = "blue";
            }
            $count++;
            //
            
            //Displays what tables that this student exists in.
            print "<p>";
            print "<b>Student Name:</b> ".getStudentName($student_num)."<br />";
            print "<b>Student Number:</b> ".$student_num."<br />";
            print "<b>Department:</b> ".getDepartmentName($dept_id)."<br />";
            if ($multiple_department_flag == true)
            {
                print "<b>Multiple Department(s):</b> ".$dept_string."<br />";
                print "<b>Note:</b> Only department specific records will be deleted because this student is in multiple departments.<br />";
            }
            print "</p>";
            print "<table cellspacing=0 cellpadding=0 border=1>";
                print "<tr align='center'><td>";
                    print "<table cellpadding=2 border=0>";
                        print "<tr class='rowgrey'>";
                            print "<td align='center'>&nbsp;<b class='white'>Table</b>&nbsp;</td>";
                            print "<td align='center'>&nbsp;<b class='white'>Record Exists</b>&nbsp;</td>";
                        print "</tr>";
                    for($i=0;$i<sizeof($table);$i++) 
                    {
                        $color = $i % 2;
                        print "<tr class='row".$color."d'>";
                            print "<td align='center'>".$table[$i]."</td>";
                            print "<td align='center'>";
                            if ($checked[$i] == "blue") 
                            {
                                print "<img src= 'misc/images/smchk_bl.gif' alt='' />";
                            }
                            elseif ($checked[$i] == "green") 
                            {
                                print "<img src= 'misc/images/smchk_gr.gif' alt='' />";
                            }
                            else 
                            {
                                print "&nbsp;";
                            }
                            print "</td>";
                        print "</tr>";
                    }
                    print "</table>";
                print "</td></tr>";
            print "</table>";
            print "<form name='myform' action='".$PHP_SELF."&amp;select=deleted_student' method='post'>";
                notify("***WARNING, YOU ARE ABOUT TO DELETE A STUDENT***");
                print "<br /><input type='submit' value='Delete Student' />";
                print "<input type='hidden' name='student_num' value='".$student_num."' />";
                print "<input type='hidden' name='dept_id' value='".$dept_id."' />";
                print "<input type='hidden' name='multiple_department_flag' value='".$multiple_department_flag."' />";
            print "</form>";

            // Legend
            print "<h4>Status Icon Key</h4>";
            print "<table cellspacing=0 cellpadding=0 border=1>";
                print "<tr align='center'><td>";
                    print "<table cellpadding=2 border=0>";
                        print "<tr class='rowgrey'>";
                            print "<td align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>";
                            print "<td align='center'>&nbsp;<b class='white'>Description</b>&nbsp;</td>";
                        print "</tr>";
                        print "<tr class='row0d'>";
                            print "<td align='center'><img src= 'misc/images/smchk_bl.gif' alt='' /></td>";
                            print "<td align='center'>Student Specific Records</td>";
                        print "</tr>";
                        print "<tr class='row1d'>";
                            print "<td align='center'><img src= 'misc/images/smchk_gr.gif' alt='' /></td>";
                            print "<td align='center'>Department Specific Records</td>";
                        print "</tr>";
                    print "</table>";
                print "</tr>";
            print "</table>";
        }// End else
    }// End Else
?>
