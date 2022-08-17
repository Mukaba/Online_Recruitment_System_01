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
 | Filename: assign_host_department.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file is called when a traffic director level user would    |
 | like to assign a new host department for a job.                              |
 +------------------------------------------------------------------------------+

*/

echo("<form method='post' name='form1' action='" . $URL . "'>");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

echo("<tr>");
	echo("<td>&nbsp;</td>");
echo("</tr>");

$sql = ("
	SELECT DISTINCT pec.company_name, pjd.position_title
	FROM pending_employer_company AS pec, pending_job_description AS pjd
	WHERE pjd.job_id='" . addslashes($job_id) . "'
	AND pec.employer_id=pjd.employer_id
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

echo("<tr>");
	echo("<td align='center'>You are assigning a host department for the job: <b class='black'>" . $row["position_title"] . "</b>");
	echo(" that has been posted by the company: <b class='black'>" . $row["company_name"] . "</b>.");
	echo("</td>");
echo("</tr>");

echo("<tr align='center'>");
echo("<td align='center'>");
echo("<table border='0' class='row2'>");
echo("<tr align='center'>");
	echo("<td>");
	echo("<table border='0' cellpadding='0' cellspacing='4'>");
	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	echo("<tr>");
	        echo("<td colspan='2' align='center'>");
		echo("Host Department:&nbsp;&nbsp;");
	        echo("<select name='assigned_host_department'>");

		$sql = ("
			SELECT DISTINCT host_department
			FROM pending_job_description
			WHERE job_id='" . addslashes($job_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$default_department = $row["host_department"];
	                        
		$sql = ("
			SELECT DISTINCT department_id, department_name
			FROM department
			WHERE job_list
			ORDER BY department_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		echo("<option value='' ");
		if (!$assigned_host_department && !$default_department)
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</OPTION>");
		while ($row = $result->FetchRow())
		{
			echo("<option value='" . $row["department_id"] . "' ");
			if (($assigned_host_department == $row["department_id"]) || (!$assigned_host_department && $default_department == $row["department_id"]))
			{
				echo("selected='selected'");
			}       
			echo(">" . $row["department_name"] . "</option>");
		}
	
		echo("</select>");
		echo("</td>");
	echo("</tr>");
	
	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>"); 
		echo("Below are listed the departments that the employer has specified are applicable to the job.");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	$sql = ("
		SELECT DISTINCT apply_departments
		FROM pending_job_description
		WHERE job_id='" . addslashes($job_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
    $apply_departments = explode(',', $row["apply_departments"]);
	$newrow = 1;

	for ($i = 0; $i < sizeof($apply_departments); $i++)
	{
		echo($newrow % 2 ? "<tr>" : "");
		
		echo("<td>");
		echo(getDepartmentName($apply_departments[$i]));
		echo("</td>");

		echo($newrow % 2 ? "" : "</tr>");

		$newrow++;
	}

	if ($newrow % 2)
	{
		//Fill out any remaining empty column, and end the row.
	
		echo("<td>&nbsp;</td>");
		echo("</tr>");
	}
			
		
	echo("</table>");
	echo("</td>");
echo("</tr>");
echo("</table>");
echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center'>");
	echo("<input type='submit' value='Save Changes' />");
	echo("<input type='hidden' name='continue' value='save_host_department' />");
	echo("&nbsp;&nbsp;");
	echo("<input type='submit' name='continue' value='Cancel' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");

echo("</form>");

?>
