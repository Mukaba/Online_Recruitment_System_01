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
 | Filename: container_info_form.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This file is the container information form which captures data |
 |              required for history containers.                                |
 +------------------------------------------------------------------------------+

*/

// initializing local variables
$error_msg = NULL;
$placement = NULL;
$sql = NULL;
$result = NULL;
$row = NULL;
$max_work_term_length = NULL;
$checked = NULL;
$selected = NULL;

// Assumptions coming in is that $placement->container_mode is already set, and the student 
// they're trying to place is using history containers by term or hours. 
echo("<h3>Job Offers - Place By Term</h3>");

if (sizeof($error_array) && is_array($error_array))
{
    foreach($error_array as $error_msg)
    {
        error($error_msg);
    }
    echo("<br />");
}

if (sizeof($arr_container) && is_array($arr_container))
{
    echo("<form method='post' action='".$PHP_SELF."&select=placement_by_term' name='myform'>");
    if ($arr_ready)
    {                                                                                                                                                
        echo("<input type='hidden' name='arr_ready' value='".packObject(($arr_ready))."' />");
    }
    if ($arr_container)
    {
        echo("<input type='hidden' name='arr_container' value='".packObject(($arr_container))."' />");
    }
    echo("<input type='hidden' name='level' value='save_placements' />");

    foreach($arr_container as $placement)
    {
        $index = $placement->student_number;

        if (is_a($placement,"Placement"))
        {
            echo("<table class='row1' cellpadding='4' cellspacing='0' border='0'>");
            echo("<tr>");
            echo("<td>Student Name:</td>");
            echo("<td>".$placement->first_name." ".$placement->last_name."</td>");
            echo("</tr>");

            echo("<tr>");
            echo("<td>Discipline:</td>");
            echo("<td>".$placement->discipline_name."</td>");
            echo("</tr>");

            echo("<tr>");
            echo("<td>Company Name:</td>");
            echo("<td>".$placement->company_name."</td>");
            echo("</tr>");

            echo("<tr>");
            echo("<td>Job Code:</td>");
            echo("<td>");
            echo("<a href='".$PHP_SELF."&select=view_job&level1=job_id&job_id=".$placement->job_id."&show_quick=true' target='_blank'>".$placement->job_code."</a>");
            echo("</td>");
            echo("</tr>");

            echo("<tr>");
            echo("<td>Start Date (YYYY-MM-DD):</td>");
            echo("<td><input type='text' name='arr_container_info[".$index."][start_date]' value='".$placement->start_date."' maxlength='10' size='10' /> ".popup('arr_container_info['.$index.'][start_date]', 'myform')."</td>");
            echo("</tr>");

            if ($placement->container_mode == CONTAINER_TERM)
            {
                $sql = ("
                    SELECT max_num_workterms, history_containers_length_term
                    FROM discipline
                    WHERE discipline_id = '".$placement->discipline_id."'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $max_work_term_length = $row['max_num_workterms'] * $row['history_containers_length_term'];

                echo("<tr>");
                echo("<td>Workterm Length:</td>");
                echo("<td><select name='arr_container_info[".$index."][work_term_length]'>");

                for ($i = 1; $i <= $max_work_term_length; $i++) {
                    if ((!$placement->work_term_length && $i == $row['history_containers_length_term']) || ($placement->work_term_length == $i))
                    {
                        $selected = "selected='selected'";
                    }
                    echo("<option value='" .$i. "' ".$selected. ">" . $i . ($i == 1 ? " month" : " months") ."</option>");
                    $selected = NULL;
                }
                echo("</select></td>");
                echo("</tr>");
            }
            elseif ($placement->container_mode == CONTAINER_HOURS)
            {
                echo("<tr>");
                echo("<td>End Date (YYYY-MM-DD):</td>");
                echo("<td><input type='text' name='arr_container_info[".$index."][end_date]' value='".$placement->end_date."' maxlength='10' size='10' /> ".popup("arr_container_info[".$index."][end_date]", "myform")."</td>");
                echo("</tr>");
            }

            if ($placement->container_mode)
            {
                echo("<tr>");
                echo("<td>Hours Per Week:</td>");
                echo("<td><input type='text' name='arr_container_info[".$index."][work_term_hours_per_week]' value='".$placement->work_term_hours_per_week."' maxlength='10' size='10' /></td>");
                echo("</tr>");

                echo("<tr>");
                echo("<td>Keep Student Eligible After Placement:</td>");

                if ($placement->keep_eligible_flag) {
                    $checked = "checked='checked'";                                                                                                              
                }

                echo("<td><input type='checkbox' name='arr_container_info[".$index."][keep_eligible_flag]' value='1' ".$checked." /></td>");
                echo("</tr>");

                $checked = NULL;
            }
            echo("<tr><td colspan='2'><hr /></td></tr>");

            echo("</table>");
        }
        echo("<br />");
    }
    echo("<br />");
    echo("<input type='submit' name='submit' value='Place Students' />");
    echo("</form>");
}

?>
