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
 | Filename: edit_host_department_form.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file is called when a member of the co-op staff would      |
 | like to edit the host department for a job.  Employers are not given access  |
 | to this part.                                                                |
 +------------------------------------------------------------------------------+

*/

echo("<form method='post' name='form1' action='" . $URL . "'>");
echo("<input type='hidden' name='select' value='edit_job' />");
echo("<input type='hidden' name='level1' value='host_department' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
echo("<tr>");
	echo("<td>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center'>");
	echo("<b class='instruct'>Warning:</b><b class=black> If you save the host department to a department other than yours, you will
		no longer be able to edit this job (which includes changing the host department back).  Do not change
		the host department unless you are absolutely sure you need to.</b>");
	echo("</td>");
echo("</tr>");

echo("<tr align='center'>");
echo("<td align='center'>");
echo("<table border='0' class='row2'>");
echo("<tr align='center'>");
	echo("<td>");
	echo("<table border='0' cellpadding='10' cellspacing='4'>");
	echo("<tr>");
	        echo("<td align='right'>Host Department:</td>");
	        echo("<td>");
	        echo("<select name='host_department'>");
	                        
		$sql = ("
			SELECT DISTINCT department_id, department_name
			FROM department
			WHERE job_list
			AND using_full_system
			ORDER BY department_name
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		echo("<option value='' ");
		if (($job->host_department) && $job->host_department == '')
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
	echo("</table>");
	echo("</td>");
echo("</tr>");
echo("</table>");
echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center'><hr />");
	echo("<input type='submit' name='continue' value='Save Changes' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");

echo("</form>");

?>
