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
 | Filename: add_job_form_c.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is the third form called by add_job_form.inc and      |
 | allows the user to input the second set of values required for a job.        |
 | After this form is filled out, it's inputs are checked, and if all goes      |
 | well, the job's information is displayed on the screen and the user is       |
 | prompted for confirmation.                                                   |
 +------------------------------------------------------------------------------+

*/

?>

<form method="post" action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="add_job" />
<input type='hidden' name='job' value='<?php echo(($job) ? packObject($job) : ""); ?>' />
<input type='hidden' name='entered_by' value='<?php echo($auth->contact_id); ?>' />
<input type='hidden' name='date_posted' value='<?php echo($current_date); ?>' />

<center>
<table border="0" cellpadding="5" cellspacing="0" width="100%" class="row1">
<tr>
<td colspan='2'>&nbsp;
</td>
</tr>
<?php
if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{
    echo("<tr>");
    echo("<td align='right'>Please choose the preferred host department for this job:</td>");
    echo("<td>");
    echo("<select name='host_department'>");

    $sql = ("
            SELECT DISTINCT department_id, department_name
            FROM department
            WHERE (job_list OR department_id='" . $auth->department . "')
            AND using_full_system
            ORDER BY department_name
            ");

    $result = $GLOBALS['dbh']->Execute($sql);

    if ($userlevel != OFFICE && $userlevel != ADMINISTRATOR && $userlevel != TRAFFICDIRECTOR)
    {
        // This should never happen

        echo("<option value='' ");
        if (isset($job->host_department) && $job->host_department == '')
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
    }
    while ($row = $result->FetchRow())
    {
        echo("<option value='" . $row["department_id"] . "' ");
        if ((!$job->host_department && $row["department_id"] == $auth->department) || ($job->host_department == $row["department_id"]))
        {
            echo("selected='selected'");
        }
        echo(">" . $row["department_name"] . "</option>");
    }

    echo("</select>");
    echo("</td>");
    echo("</tr>");
}
elseif ($userlevel == EMPLOYER || $userlevel == ADMINEMPLOYER)
{
    if ($job->apply_departments)
    {
        for ($i = 0; $i < sizeof($job->apply_departments); $i++)
        {
            $departments .= ("'" . $job->apply_departments[$i] . "',");
        }
    }

    /*
     Get all of the department_id's that have hosted jobs they have posted for already.
     Include these in their list of choices.
     */

    $nextTermYear = getNextTermYear();

    $sql = ("
            SELECT DISTINCT department_id
            FROM job_info
            WHERE employer_id='" . $employer_id . "'
            AND term_id='" . $nextTermYear["term"] . "'
            AND year='" . $nextTermYear["year"] . "'
            AND status!=''
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $departments .= ("'" . $row["department_id"] . "',");
    }

    $departments = substr($departments, 0, -1);

    $sql = ("
            SELECT department_id, department_name
            FROM department
            WHERE using_full_system = '1'
            ");
    if ($job->apply_departments)
    {
        // If they've chosen applicable departments, give them those as choices.

        $sql .= ("
                AND department_id IN (" . $departments . ")
                ");
    }

    $sql .= ("
            ORDER BY department_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {

        echo("<tr>");
        echo("<td align='right'>Please choose the preferred host department for this job:</td>");
        echo("<td>");
        echo("<select name='host_department'>");

        echo("<option value='' ");
        if (!$job->host_department)
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["department_id"] . "' ");
            if ($job->host_department == $row["department_id"])
            {
                echo("selected='selected'");
            }
            echo(">" . $row["department_name"] . "</option>");
        }

        echo("</select>");
        echo("</td>");
        echo("</tr>");
    }
}

if (is_array($job->apply_departments) && sizeof($job->apply_departments) < 4)
{
    // Once everyone is using this, picking no departments means you shouldn't enter this section of code.

    echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("<table cellpadding='3' cellspacing='3'>");
    echo("<tr>");
    echo("<td class='row2'>");
    echo("<table width='100%' class='row2'>");

    echo("<tr>");
    echo("<td align='center' colspan='2'>Please choose the disciplines applicable to this job:");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td align='center' colspan='2'>&nbsp;</td>");
    echo("</tr>");

    $newrow = 0;
    foreach ($job->apply_departments AS $ad)
    {
        $apply_depts .= ("'" . $ad . "',");
    }
    $apply_depts = substr($apply_depts, 0, -1);

    $sql = ("
            SELECT DISTINCT dd.discipline_id, dd.discipline_name
            FROM department_disciplines AS dd, department_discipline_join AS ddj
            WHERE ddj.department_id IN (" . $apply_depts . ")
            AND ddj.discipline_id=dd.discipline_id
            ORDER BY dd.discipline_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        if (!($newrow % 2))
        {
            echo("<tr>");
        }

        echo("<td>");
        echo("<input type='checkbox' name='job_disciplines[]' value='" . $row["discipline_id"] . "' ");
        if (is_array($job->job_disciplines) && in_array($row["discipline_id"], $job->job_disciplines))
        {
            echo("checked='checked'");
        }
        echo(" />" . htmlentities($row["discipline_name"]));
        echo("</td>");

        if ($newrow % 2)
        {
            echo("</tr>");
        }
        $newrow++;
    }

    if ($newrow % 2)
    {
        //Finish off the last column
        echo("<td>&nbsp;</td>");
        echo("</tr>");
    }

    echo("<tr><td colspan='2'>&nbsp;</td></tr>");

    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
}

?>
<tr>
<td align="right">Job description: <b class='red'>*</b></td>
<td><textarea name="job_description" rows="10" cols="40"><?php 
echo(($job) ? stripSlashes($job->job_description) : ""); ?></textarea>
</td>
</tr>
<tr>
<td align="right">Skills required: <b class='red'>*</b></td>
<td><textarea name="skills_required" rows="10" cols="40"><?php 
echo(($job) ? stripSlashes($job->skills_required) : ""); ?></textarea>
</td>
</tr>
<tr>
<td align="right">Special job requirements:<br />(Drivers License required,
        <br />must be returning to an<br />academic term, etc.)</td>
<td><textarea name="special_requirements" rows="8" cols="40"><?php 
echo(($job) ? stripSlashes($job->special_requirements) : ""); ?></textarea>
</td>
</tr>
<tr>
<td align="right">Employer comments for Co-op staff:<br />(Only viewable by Co-op staff and employer, not students).</td>
<td><textarea name="comments" rows="8" cols="40"><?php 
echo(($job) ? stripSlashes($job->comments) : ""); ?></textarea>
</td>
</tr>
    <?php
if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{
    ?>
        <tr>
        <td align='right'>Co-op staff comments:<br />(Only viewable by Co-op staff, not employer or students).</td>
        <td><textarea name='admin_comments' rows='8' cols='40'><?php
        echo(($job) ? stripSlashes($job->admin_comments) : ""); ?></textarea>
        </td>
        </tr>
        <tr>
        <td align='right'>Job status comments for students:<br />(Only viewable by Co-op staff and students, not employer).</td>
        <td><textarea name='for_student_comments' rows='8' cols='40'><?php
        echo(($job) ? stripSlashes($job->for_student_comments) : ""); ?></textarea>
        </td>
        </tr>
        <?php
        if ($job->apply_method == JOB_APP_METHOD_SPECIAL) {
            ?>
                <tr>
                <td align="right">Special instructions for applying: <br />(apply directly to employers site, etc.)</td>
                <td><textarea name="special_instructions" rows="8" cols="40"><?php 
                echo (($job) ? stripSlashes($job->special_instructions) : ""); ?></textarea>
                </td>
                </tr>
                <tr>
                <td align="right">Special instructions button:<br /> When student clicks, their name will be <br />
                listed with this job in the Applications module. <br />(Only displayed if checked and <br />
                        receiving applications is "Special", <br />eg: "Sign up for Presentation", "Applied to Employer's Site")</td>
                <td><input type='checkbox' name='show_special_instr_button' class='row1' value='1' /> Display Special Instruction Button <br />
                <input type='text' name='special_instr_button' size="40" value="<?php echo (($job) ? stripSlashes($job->special_instr_button) : ""); ?>" />
                </td>
                </tr>
                <?php
        }
}

$checked = '';
if($job->displayname_on_coverltr == '1')
{
    $checked = 'checked';
}
?>

<tr>
<td colspan='2'>
    <table cellpadding='5' cellspacing='5'>
        <tr>
        <td valign='top' align='right'>
            <input type='checkbox' name='displayname_on_coverltr' value='1' <?php echo($checked) ?> />
        </td>
        <td>
            Check this box if you wish for your name to be visible to students.  
            Knowing your name enables students to address their cover letters to you directly instead of 'Dear Sir/Madam'.  
            For your privacy, you may remain anonymous to students if you leave the box unchecked.
        </td>
        </tr>
    </table>
</td>
</tr>
<tr>
<td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class='red'>*</b>) are required for the default form settings</td>
</tr>
<tr>
<td colspan='2' align="center"><hr />
<input type="hidden" name="continue" value="level3" />
<input type="submit" value="Continue" />
</td>
</tr>
</table>
</center>
</form>
