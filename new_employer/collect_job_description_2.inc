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
 | Filename: collect_job_description_2.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file is the second form called to gather job inputs, and   |
 | allows the user to input the second set of values required for a job.        |
 | After this form is filled out, it's inputs are checked, and if all goes      |
 | well, the job's information is displayed on the screen and the user is       |
 | prompted for confirmation.                                                   |
 +------------------------------------------------------------------------------+

*/

echo("<h3>Job Description Form (page 2 of 2)</h3>");
echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='new_employer' />");
echo("<input type='hidden' name='newEmployer' value='" . packObject($newEmployer) . "' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

for ($i = 0; $i < sizeof($newEmployer->job_apply_departments); $i++)
{
	$departments .= ("'" . $newEmployer->job_apply_departments[$i] . "',");
}

$departments = substr($departments, 0, -1);

$sql = ("
	SELECT DISTINCT department_id, department_name
	FROM department
	WHERE department_id IN (" . $departments . ")
	AND job_list
	ORDER BY department_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{

	echo("<tr>");
	echo("<td align='right'>Please choose your preferred host department for this job:</td>");
	echo("<td>");
	echo("<select name='host_department'>");
	
	echo("<option value='' ");
	if (!$newEmployer->job_host_department)
	{
		echo("selected='selected'");
	}
	echo(">&nbsp;</option>");
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["department_id"] . "' ");
		if ($newEmployer->job_host_department == $row["department_id"])
		{
			echo("selected='selected'");
		}
		echo(">" . $row["department_name"] . "</option>");
	}

	echo("</select>");
	echo("</td>");
	echo("</tr>");
}

if (is_array($newEmployer->job_apply_departments) && sizeof($newEmployer->job_apply_departments) < 4)
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
		foreach ($newEmployer->job_apply_departments AS $ad)
		{
			$apply_depts .= ("'" . $ad . "',");
		}
		$apply_depts = substr($apply_depts, 0, -1);

		$sql = ("
			SELECT DISTINCT dd.discipline_id, dd.discipline_name
			FROM department_disciplines AS dd, department_discipline_join AS ddj
			WHERE ddj.department_id IN (" . $apply_depts . ")
			AND ddj.discipline_id=dd.discipline_id
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
			if (is_array($job_disciplines) && in_array($row["discipline_id"], $job_disciplines))
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

echo("<tr>");
	echo("<td align='right'>Job Description: <b class='red'>*</b></td>");
	echo("<td><textarea name='job_description' rows='10' cols='40'>");
	echo($newEmployer->job_description);
	echo("</textarea>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Skills Required: <b class='red'>*</b></td>");
	echo("<td><textarea name='skills_required' rows='10' cols='40'>");
	echo($newEmployer->job_skills_required);
	echo("</textarea>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Special Job Requirements<br />");
	echo("(Driver's License required, must be returning to an<br />");
	echo("academic term, etc.)</td>");
	echo("<td><textarea name='special_requirements' rows='8' cols='40'>");
	echo($newEmployer->job_special_requirements);
	echo("</textarea>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Comments for Co-op Staff:<br />(Not student viewable).</td>");
	echo("<td><textarea name='comments' rows='8' cols='40'>");
	echo($newEmployer->job_comments);
	echo("</textarea>");
	echo("</td>");
echo("</tr>");
if($newEmployer->job_displayname_on_coverltr == '1')
{
        $checked = 'checked';
}
?>

<tr>
<td colspan='2'>
    <table cellpadding='5' cellspacing='5'>
        <tr>
        <td valign='top' align='right'>
            <input type='checkbox' name='displayname_on_coverltr' value = '1' <?php echo($checked) ?> />
        </td>
        <td>
            Check this box if you wish for your name to be visible to students. 
            Knowing your name enables students to address their cover letters to you 
            directly instead of 'Dear Sir/Madam'. For your privacy, you may remain anonymous
            to students if you leave the box unchecked.

        </td>
        </tr>
    </table>
</td>
</tr>
<?php



echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<hr />Fields marked with an asterisk (<b class='red'>*</b>) are required.</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<hr />");
	echo("<input type='hidden' name='continue' value='confirm_job_description' />");
    echo("<input type='hidden' name='date_posted' value='" . $current_date . "' />");
	echo("<input type='submit' value='Continue' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");
echo("</form>");

?>
