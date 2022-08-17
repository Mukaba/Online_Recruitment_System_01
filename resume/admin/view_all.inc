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
 | Filename: view_all.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: Searches for matches and displays all resumes that qualify..    |
 | variables sent to by view_main.inc include : student_name, student_num,      |
 | s_view                                                                       |
 +------------------------------------------------------------------------------+

*/

?>


<script type='text/javascript' language='javascript'>
<!--javascript

function load_order(order)
{
    document.form_order.order_by.value=order;
    document.form_order.submit();
}
//-->
</script>


<?php

// Fixed issues with spaces (like when people cut and paste them in, sometimes they get a space too and it complains)
$student_num = trim($student_num);
$student_num2 = trim($student_num2);
$student_name = trim($student_name);

$errorset = 0;
$error=0;

// User is coming from student/history quickmenu, and a record_id is available, derive department_id from it. 
if ($record_id)
{
    $sql = ("
        SELECT department_id
        FROM student_department
        WHERE record_id = '".$record_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $department_id = $row['department_id'];
}

//if ($department_id == '0' || $department_id == '' || !$department_id) {
    $department_in_str = "('" . implode("', '", $departments_in_group) . "')";
//} else { 
 //   $department_in_str = "('$department_id')";
//}
if ($student_num2 != "") {
    $student_num = $student_num2;
    // Get record_id for use in quickmenu
    $sql_get_recordid = "Select distinct record_id from student_department
        WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
    $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
    if ($result_get_recordid->RecordCount()==1)
    {
        $row_get_recordid = $result_get_recordid->FetchRow();
        $record_id = $row_get_recordid['record_id'];
    } elseif ($result_get_recordid->RecordCount()==0) {
        $sql_get_recordid2 = "Select distinct record_id from student_department
            WHERE student_number='".$student_num."' and department_id IN ".$department_in_str."";
        $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
        if ($result_get_recordid2->RecordCount()==1)
        {
            $row_get_recordid2 = $result_get_recordid2->FetchRow();
            $record_id = $row_get_recordid2['record_id'];
        }
    }
}
else
{
    if ($student_num != "") {
        // Get record_id for use in quickmenu
        $sql_get_recordid = "Select distinct record_id from student_department
            WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
        $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
        if ($result_get_recordid->RecordCount()==1)
        {
            $row_get_recordid = $result_get_recordid->FetchRow();
            $record_id = $row_get_recordid['record_id'];
        } 
        elseif ($result_get_recordid->RecordCount()==0) 
        {
            $sql_get_recordid2 = "Select distinct record_id from student_department
                WHERE student_number='".$student_num."' and department_id IN ".$department_in_str."";
            $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
            if ($result_get_recordid2->RecordCount()==1)
            {
                $row_get_recordid2 = $result_get_recordid2->FetchRow();
                $record_id = $row_get_recordid2['record_id'];
            }
        }
    }
    else
    {
        if ($student_name != "")
        {
            $student_name = trim($student_name);
            if (preg_match("/,.*,/", $student_name))
            {
                $errorset = 1;
                error("You may only enter one comma in the student name field.");
            }
            elseif ($student_name == ",")
            {
                $errorset = 1;
                error("You must include a partial last name to view resumes based on the name field.");
            }
            else
            {
                // Strip wild card character from student name.
                $student_name = str_replace("%", "", $student_name);
                $student_names = explode(",", $student_name);
                $first_name = trim($student_names[1]);
                $last_name = trim($student_names[0]);

                // Get record_id for use in quickmenu
                $sql_get_students = ("
                    SELECT DISTINCT s.student_number 
                    FROM student as s
                    INNER JOIN student_department as sd
                    ON s.student_number = sd.student_number
                    WHERE sd.department_id IN ".$department_in_str."
                    ");
                if ($first_name) {
                    $sql_get_students .= (" AND s.first_name LIKE '".$first_name."'"); 
                }
                if ($last_name) {
                    $sql_get_students .= (" AND s.last_name LIKE '".$last_name."'");
                }

                $result_get_students=$GLOBALS['dbh']->Execute($sql_get_students);
                if ($result_get_students->RecordCount()==1)
                {
                    $row_get_students = $result_get_students->FetchRow();
                    $student_number = $row_get_students['student_number'];
                    $sql_get_recordid = "Select distinct record_id from student_department
                        WHERE student_number='".$student_number."' and department_id='".$auth->department."'";
                    $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
                    if ($result_get_recordid->RecordCount()==1)
                    {
                        $row_get_recordid = $result_get_recordid->FetchRow();
                        $record_id = $row_get_recordid['record_id'];
                    } elseif ($result_get_recordid->RecordCount()==0) {
                        $sql_get_recordid2 = "Select distinct record_id from student_department
                            WHERE student_number='".$student_number."' and department_id IN ".$department_in_str."";
                        $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
                        if ($result_get_recordid2->RecordCount()==1)
                        {
                            $row_get_recordid2 = $result_get_recordid2->FetchRow();
                            $record_id = $row_get_recordid2['record_id'];
                        }
                    }
                }
            }
        } else {
            $first_name = "";
            $last_name = "";
        }
    }
}

if (empty($student_num) && empty($student_num2) && empty($student_name)) {
    error('Please enter a student number or student name.');
    include("view_main.inc"); 
    $errorset = 1;
}
 
if ($student_num != "" && !$errorset) {
 
    $sql="Select student_number from student_department where student_number='".$student_num."' AND department_id IN ".$department_in_str;
    $results=$GLOBALS['dbh']->Execute($sql);

    if ($results->RecordCount()==0){
        error("Invalid student number, or student not in your department(s).");
        include("view_main.inc"); 
        $student_num=0;
        $errorset=1;
    }
}

if ($term_year_submit != 'Go')
{
    $nextTermYear = getNextTermYear();
    $term = $nextTermYear["term"];
    $year = $nextTermYear["year"];
}

if ($errorset === 0)
{

    if($order_by)
    {
        if($order_by == "student_name")
        {
            $select_sql = "SELECT CONCAT(s.first_name, ' ', s.last_name) as student_name, r.resume_id, s.student_number,
            ct.description as type, DATE_FORMAT(r.last_modified, '%b %d, %Y') as last_modified, r.name "; 
        }
        elseif($order_by == "last_modified")
        {
            $select_sql = "SELECT  DATE_FORMAT(r.last_modified, '%b %d, %Y') as last_modified, ct.description as type, 
                CONCAT(s.first_name, ' ', s.last_name) as student_name, r.resume_id, s.student_number, r.name ";
        }
        else
        {
            $select_sql = "SELECT $order_by, DATE_FORMAT(r.last_modified, '%b %d, %Y') as last_modified, ct.description as type,
            CONCAT(s.first_name, ' ', s.last_name) as student_name, r.resume_id, s.student_number, r.name ";
        }
    }
    else
    {
        $select_sql = "SELECT CONCAT(s.first_name, ' ', s.last_name) as student_name, r.resume_id, s.student_number,
        ct.description as type, DATE_FORMAT(r.last_modified, '%b %d, %Y') as last_modified, r.name ";
    }



    $from_sql = " FROM student as s 
        LEFT JOIN resume as r ON r.student_number = s.student_number 
        LEFT JOIN coverletter_type as ct ON ct.type_id = r.type_id
        LEFT JOIN student_department as sd ON sd.student_number = s.student_number
        WHERE
        s.first_name like '".$first_name."%' AND
        s.last_name like '".$last_name."%' AND
        s.student_number like '".$student_num."%' AND
        sd.department_id IN ".$department_in_str." AND
        (ct.type_id IN ('".SPECIFIC_COVERLETTER."', '".TEMPLATE_COVERLETTER."', '".GENERIC_COVERLETTER."')) ";

    if($order_by)
    {
        if($order_by == "last_modified")
        {
            $order_by = "r.last_modified";
        }

        if(($lastorder == $order_by) && ($data_flow == "ASC"))
        {
            $data_flow = "DESC";
        }
        elseif(($lastorder == $order_by) && ($data_flow == "DESC"))
        {
            $data_flow = "ASC";
        }
        elseif(!$data_flow)
        {
            $data_flow = "DESC";
        }

        $order_sql = "ORDER BY $order_by $data_flow ";

        if($order_by == "last_modified")
        {
            $order_by = "r.last_modified";
        }
    }
    else
    {
        $order_sql = "ORDER BY  s.last_name ASC, s.first_name ASC, ct.type_id DESC, r.last_modified DESC  ";
        $order_by = "student_name";
        $data_flow = "ASC";
    }

    $sql = $select_sql." ".$from_sql." ".$order_sql;                                                                                                                                                                                                    $sql = $select_sql." ".$from_sql." ".$order_sql; 



    // Display Resumes !
    if ($error != '1')
    {
        echo("<table width='100%'>");
        echo("<tr>");
        if ($auth->userlevel == OFFICE)
        {
            echo("<td><h3 align='left'>Student Information - View/Edit Resumes</h3></td>\n");
        } else {
            echo("<td><h3 align='left'>Student Information - View Resumes</h3></td>\n");
        }
        // Display the quickmenu
        if ($auth->userlevel == OFFICE)
        {
            echo("<td align='right'>");
            if ($record_id)
            {
                include('student_info/student_quickmenu.inc');
            }
            echo("</td>\n");
        }
        echo("</tr>");
        echo("</table>");

        //paging form
        $results = $GLOBALS['dbh']->Execute($sql);
        $num_jobs = $results->RecordCount();
        if ($num_jobs == 0) {
            notify("No Resumes found.\n");
        }
        echo("<h4>Choose a Resume($num_jobs):</h4>");
        if ($jobs_per_page == "") { $jobs_per_page = 20; }
        if ($jobs_per_page < 5) { $jobs_per_page = 5; }
        if ($job_page == '') { $job_page = 0; }


        // display the thingy if necessary
        if ($num_jobs > 20) {

            $num_pages = ceil($num_jobs / $jobs_per_page);

            $job_pos_start = $job_page * $jobs_per_page;
            $job_pos_end = $job_pos_start + $jobs_per_page;

            $sql .= " LIMIT $job_pos_start, $jobs_per_page";

            unset($PDF);
            //echo ("\n<table border='0' class='row1' cellspacing='0' cellpadding='4'>");
            echo("<tr>");
            echo("<td>");
            echo("\n<form method='post' action='" . $PHP_SELF . "'>");
            if ($last_name != '')
            {
                ?>
                    <input type='hidden' name='student_name' value="<?php echo($last_name.",".$first_name)?>" />
                    <?php
            }
           //echo("</td>");
           //echo("</tr>");
           echo ("\n<table border='0' class='row1' cellspacing='0' cellpadding='4'>");
          
            echo ("\n<tr>");
            echo ("\n<td valign='bottom'>");
            echo("\n<input type='hidden' name='select' value='resume' />");
            echo("\n<input type='hidden' name='page' value='View Resumes' />");
            echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
            echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
            echo ("\n<input type='hidden' name='student_num' value='".$student_num."' />");
            echo ("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
            echo ("\n<input type='hidden' name='record_id' value='".$record_id."' />");
            echo ("\n<input type='hidden' name='department_id' value='".$department_id."' />");
            echo ("\n<input type='hidden' name='term' value='".$term."' />");
            echo ("\n<input type='hidden' name='year' value='".$year."' />");

            $page_number = $job_page + 1;
            echo ($num_jobs . " resume" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "") .
                    ": ".$page_number);
            echo ("\n</td>");
            echo ("\n<td valign='bottom'>");
            echo ("<input type='text' name='jobs_per_page' size='3' value='$jobs_per_page' /> resumes per page");
            echo ("\n</td>");
            echo ("\n</tr>");

            echo ("\n<tr>");
            echo ("\n<td>");
            echo ("\n<select name='job_page'>");

            for ($j = 0; $j < $num_pages; $j ++)
            {

                $from_pos = $j * $jobs_per_page;
                $to_pos = $from_pos + $jobs_per_page - 1;
                if ($to_pos >= $num_jobs) $to_pos = $num_jobs - 1;

                $results->Move($from_pos);
                $results_row = $results->FetchRow();
                $from = $results_row[0];

                $results->Move($to_pos);
                $results_row = $results->FetchRow();
                $to = $results_row[0];

                // special cases to concatenate the strings
                if ($order == 'term_year')
                {
                    $from = substr($from, 0, 2) . ". " . substr($from, -4, 4);
                    $to = substr($to, 0, 2) . ". " . substr($to, -4, 4);
                }
                elseif
                    ($order == 'h.department_id')
                    {
                        $from = getDepartmentName($from);
                        $to = getDepartmentName($to);
                    }
                else
                {
                    if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
                    if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
                }

                echo ("\n<option value='$j'". (($j == $job_page) ? "selected='selected'" : "") .">");
                echo ((($from == '') ? "n/a" : $from) ." to  ". (($to == '') ? "n/a" : $to));
                echo ("\n</option>");
            }
            echo ("\n</select>");
            echo ("\n</td>");

            echo ("\n<td align='left'>");
            echo ("\n<input type='submit' value='View' />");
            echo ("\n</td>");
            echo ("\n</tr>");
            echo ("\n</table>");
            echo ("\n</td>");
            echo ("\n</tr>");
            echo ("\n</form>");
        }


        $result = $GLOBALS['dbh']->Execute($sql);
        if ($result->RecordCount() != 0) {

            // arrows
            if ($num_jobs > 20) {
                echo ("<table cellspacing='0' cellpadding='0' border='0'>");
                echo ("<tr>");
                echo ("<td align='left'>");

                if ($job_page > 0) {
                    unset($PDF);
                    echo ("\n<form method='post' action='$PHP_SELF'>");
                    echo ("\n<input type='hidden' name='select' value='resume' />");
                    echo ("\n<input type='hidden' name='page' value='View Resumes' />");
                    echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
                    echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
                    echo ("\n<input type='hidden' name='student_num' value='".$student_num."' />");
                    echo ("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
                    echo ("\n<input type='hidden' name='record_id' value='".$record_id."' />");
                    echo ("\n<input type='hidden' name='department_id' value='".$department_id."' />");
                    echo ("\n<input type='hidden' name='term' value='".$term."' />");
                    echo ("\n<input type='hidden' name='year' value='".$year."' />");
                    if ($last_name != '')
                    {
                        ?>
                            <input type='hidden' name='student_name' value="<?php echo($last_name.",".$first_name)?>" />
                            <?php
                    }
                    echo ("\n<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
                    echo ("\n<input type='hidden' name='job_page' value='". $job_page ."' />");
                    echo ("\n<input type='submit' value='<' />");
                    echo ("\n</form>");
                }

                else {
                    echo ("&nbsp;");
                }

                echo ("\n</td>");
                echo ("\n<td align='right'>");

                if ($job_page < ($num_pages - 1)) {
                    echo ("\n<form method='post' action='$PHP_SELF'>");
                    echo ("\n<input type='hidden' name='select' value='resume' />");
                    echo ("\n<input type='hidden' name='page' value='View Resumes' />");
                    echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
                    echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
                    echo ("\n<input type='hidden' name='student_num' value='".$student_num."' />");
                    echo ("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
                    echo ("\n<input type='hidden' name='department_id' value='".$department_id."' />");
                    echo ("\n<input type='hidden' name='record_id' value='".$record_id."' />");
                    echo ("\n<input type='hidden' name='term' value='".$term."' />");
                    echo ("\n<input type='hidden' name='year' value='".$year."' />");
                    if ($last_name != '')
                    {
                        ?>
                            <input type='hidden' name='student_name' value="<?php echo($last_name.",".$first_name)?>" />
                            <?php
                    }
                    echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
                    echo ("<input type='hidden' name='job_page' value='". ($job_page + 1) ."' />");
                    echo ("<input type='submit' value='>' />");
                    echo ("</form>");
                }

                else {
                    echo ("&nbsp;");
                }

                echo ("</td>");
                echo ("</tr>");

                echo ("<td colspan='2'>");
            }
        }

        echo ("
                <form method='post' action='$PHP_SELF'>
                <table border='1' cellpadding='0' cellspacing='0'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>
                <td class='rowgrey' align='center'>&nbsp;</td>
                <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"student_name\")'><b class='white'>&nbsp;Student Name&nbsp;</b></a></td>
                <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"r.name\")'><b class='white'>&nbsp;Resume Name&nbsp;</b></a></td>
                <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"ct.description\")'><b class='white'>&nbsp;Type&nbsp;</b></a></td>
                <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"last_modified\")'><b class='white'>&nbsp;Last Modified&nbsp;</b></a></td>
                <td class='rowgrey' align='center'><b class='white'>&nbsp;Jobs&nbsp;</b></td>
                </tr>
                ");

        $get_resumes = $GLOBALS['dbh']->Execute($sql);
        $i= 0;
        while ($row = $get_resumes->FetchRow())
        {
            (($i % 2 == 0) ? $class="row0d" : $class="row1d");
            $i++;
            $sql = "SELECT ji.job_code
                FROM job_info as ji, applications as a
                WHERE a.student_number = '" . addslashes($row["student_number"]) . "' 
                AND a.job_id = ji.job_id AND a.resume_id = '" . $row["resume_id"] . "'
                AND ji.term_id = '".$term."'
                AND ji.year = '".$year."'
                ORDER BY ji.job_code";
            $result_jobs = $GLOBALS['dbh']->Execute($sql);
            $jobs = "";
            while ($row_jobs = $result_jobs->FetchRow()){
                $jobs .= $row_jobs["job_code"] . "<br />";
            }
            $jobs = substr($jobs, 0, strlen($jobs) - 6);
            ?>
                <tr valign='middle' class='<?php echo $class?>'>
                <td class="<?php echo $class?>" align='center' nowrap='nowrap'><input type='Radio' name='resume_id' value='<?php echo $row["resume_id"]?>' /></td>
                <td class="<?php echo $class?>" nowrap='nowrap'>&nbsp;<?php echo (($row["student_name"] == "") ? "&nbsp;" : $row["student_name"])?></td>
                <td class="<?php echo $class?>" nowrap='nowrap' align='center'>&nbsp;<?php echo (($row["name"] == "") ? "&nbsp;" : $row["name"])?></td>
                <td class="<?php echo $class?>" nowrap='nowrap'>&nbsp;<?php echo (($row["type"] == "") ? "&nbsp;" : $row["type"])?></td>
                <td class="<?php echo $class?>" nowrap='nowrap'>&nbsp;<?php echo (($row["last_modified"] == "") ? "&nbsp;" : $row["last_modified"])?></td>
                <td class="<?php echo $class?>" nowrap='nowrap'><?php echo (($jobs == "") ? "&nbsp;" : $jobs)?></td>
                </tr>
                <?php
        }


        if ($i == 0) {
            echo    "<tr>
                <td class='row0d' colspan=6 align='center' valign='middle'><br />
                No Matches For Your Search Criteria, please <a href='$PHP_SELF&amp;select=resume&amp;page=view'>try again</a>.<br />&nbsp;
            </td>
                </tr>";         
        }
        ?>
            </table>
            </td></tr></table>
            <?php //echo("</td></tr></table>"); ?>
            <br />
            <input type="hidden" value="<?php echo $department_id?>" name="department_id" />
            <input type="hidden" value="<?php echo $last?>" name="last" />
            <input type="hidden" value="<?php echo $student_num?>" name="student_num" />
            <input type="hidden" value="<?php echo $s_view?>" name="s_view" />
            <input type="hidden" value="1" name="PDF" />
            <input type="hidden" value="resume" name="select" />
            <input type="hidden" value="<?php echo($record_id)?>" name="record_id" />
            <input type="hidden" value="<?php echo($department_in_str)?>" name="department_in_str" />
            <input type="hidden" value="<?php echo($term)?>" name="term" />
            <input type="hidden" value="<?php echo($year)?>" name="year" />
            <input type="hidden" value="<?php echo($term_year_submit)?>" name="term_year_submit" />

            <?php
            if ($i != 0)
            {
                echo("<table class='row1' cellspacing='0' cellpadding='5' border='0'><tr><td>");
                echo("<input type='submit' value='View HTML' name='page' />&nbsp;&nbsp;");
                echo("<input type='submit' value='View PDF' name='page' />");
                if ($auth->userlevel == OFFICE)
                {
                    echo("&nbsp;&nbsp;<input type='submit' value='Edit' name='page' />");
                }
                echo("</td></tr></table>");
            } 
            else{
                echo("</td>");
                echo("</tr>");
                echo("</table>");
           }
        echo("</form>");

        echo("<form name='form_order' action='".$PHP_SELF."' method='post'>");
        echo("\n<input type='hidden' name='select' value='resume' />");
        echo("\n<input type='hidden' name='page' value='View Resumes' />");
        echo("\n<input type='hidden' name='order_by' value='' />");
        echo("\n<input type='hidden' name='lastorder' value='$order_by' />");
        echo("\n<input type='hidden' name='data_flow' value='$data_flow' />");
        echo("\n<input type='hidden' name='student_num' value='".$student_num."' />");
        echo("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
        echo("\n<input type='hidden' name='record_id' value='".$record_id."' />");
        echo("\n<input type='hidden' name='term' value='".$term."' />");
        echo("\n<input type='hidden' name='year' value='".$year."' />");
        if ($last_name != '')
        {
            echo("<input type='hidden' name='student_name' value='".$last_name.",".$first_name."' />");
        }
        echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
        echo ("<input type='hidden' name='job_page' value='". $job_page ."' />");
    }
}
?>
