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

//initializing local variables
$error_msg = NULL;
$selected = NULL;
$checked = NULL;
$sql = NULL;
$result = NULL;
$row = NULL;
$max_work_term_length = NULL;

// Assumptions coming in is that $addHistory->container_mode is already set, and the student 
// they're trying to place is using history containers by term or hours. 

// Display appropriate header: 'own job placement' and 'returning placement' function synonymously; only diff between them is header and page re-routes
if ($select == "add_history")
{
    echo("<h3 align='left'>Job Offers - Own Job Placement</h3>");
}
if ($select == "returning_placement")
{
    echo("<h3 align='left'>Job Offers - Returning Job Placement</h3>");
}

echo("<h4 align='left'>Job Information for ".$addHistory->first_name." ".$addHistory->last_name." (".$addHistory->student_number.") at ".$addHistory->company_name." for ".getTermName($addHistory->term_id)." ".$addHistory->year."</h4>");

if (sizeof($error_array) && is_array($error_array))
{
    foreach($error_array as $error_msg)
    {
        error($error_msg);
    }
    echo("<br />");
}
echo("<table class='row1' cellpadding='4' cellspacing='0' border='0'>");
echo("<form method='post' action='".$PHP_SELF."' name='myform'>");

echo("<tr>");
echo("<td>Start Date (YYYY-MM-DD):</td>");
echo("<td><input type='text' name='start_date' value='".$addHistory->start_date."' maxlength='10' size='10' /> ".popup('start_date', 'myform')."</td>");
echo("</tr>");

if ($addHistory->container_mode == CONTAINER_TERM)
{
    $sql = ("
        SELECT max_num_workterms, history_containers_length_term
        FROM discipline
        WHERE discipline_id = '".$addHistory->discipline_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $max_work_term_length = $row['max_num_workterms'] * $row['history_containers_length_term'];

    echo("<tr>");
    echo("<td>Workterm Length:</td>");
    echo("<td><select name='work_term_length'>");

    for ($i = 1; $i <= $max_work_term_length; $i++) {
        if ((!$addHistory->work_term_length && $i == $row['history_containers_length_term']) || ($addHistory->work_term_length == $i))
        {
            $selected = "selected='selected'";
        }
        echo("<option value='" .$i. "' ".$selected. ">" . $i . ($i == 1 ? " month" : " months") ."</option>");
        $selected = NULL;
    }
    echo("</select></td>");
    echo("</tr>");
}
elseif ($addHistory->container_mode == CONTAINER_HOURS)
{
    echo("<tr>");
    echo("<td>End Date (YYYY-MM-DD):</td>");
    echo("<td><input type='text' name='end_date' value='".$addHistory->end_date."' maxlength='10' size='10' /> ".popup("end_date", "myform")."</td>");
    echo("</tr>");
}

echo("<tr>");
echo("<td>Hours Per Week:</td>");
echo("<td><input type='text' name='work_term_hours_per_week' value='".$addHistory->work_term_hours_per_week."' maxlength='10' size='10' /></td>");
echo("</tr>");

echo("<tr>");
echo("<td>Keep Student Eligible After Placement:</td>");

if ($addHistory->keep_eligible_flag) {
    $checked = "checked='checked'";
}

echo("<td><input type='checkbox' name='keep_eligible_flag' value='1' ".$checked." /></td>");
echo("</tr>");

$checked = NULL;

echo("<tr>");
echo("<input type='hidden' name='addHistory' value='".packObject(($addHistory))."' />");
echo("<input type='hidden' name='level' value='confirm' />");

// re-route to apporiate page, own job or returning job
if($select == "add_history")
{
    echo("<input type='hidden' name='select' value='add_history' />");
}
if($select == "returning_placement")
{
    echo("<input type='hidden' name='select' value='returning_placement' />");
}
echo("<td colspan='2'><hr /><div align='center'><input type='submit' name='submit' value='Add Record' /></div></td>");
echo("</tr>");
echo("</form>");
echo("</table>");
?>
