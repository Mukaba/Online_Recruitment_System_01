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
 | Filename: confirm_removal.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This page prompts the user to make sure that they would like    |
 | to remove the selected student's applications from this job.                 |
 +------------------------------------------------------------------------------+

*/

$sql = ("
	SELECT status, job_code, position_title
	FROM job_info
	WHERE job_id='" . addslashes($job_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

echo("<form method='post' action='" . $PHP_SELF . "' name='form'>");
echo("<input type='hidden' name='select' value='applications' />");
echo("<input type='hidden' name='selected_app_id' value='" . packObject(($selected_app_id)) . "' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo("<input type='hidden' name='option' value='" . $option . "' />");
echo("<input type='hidden' name='remove_confirmed' value='true' />");
echo("<input type='hidden' name='continue' value='Remove Applications' />");
echo("<h3>Job Applications</h3>");
echo("<table border='1' cellspacing='0' cellpadding='0' class='error'>");
echo("<tr class='error'>");
	echo("<td>");
	echo("<table width='100%' cellpadding='5' cellspacing='0'>");
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("You have specified that you would like to ");
		
		if ($row['status'] == CLOSED)
		{
			echo("make unavailable ");
		}
		else
		{
			echo("flag as removed ");
		}
	
		echo("the application(s) for: <br /><b> ");
		
		for ($i = 0; $i <= sizeof($selected_app_id) - 1; $i++) {
			$sql = "SELECT student_number FROM applications 
				WHERE application_id = '". $selected_app_id[$i] ."'";
            $result = $GLOBALS['dbh']->Execute($sql);
            $stud_num = $result->FetchRow();
			echo(getStudentName($stud_num["student_number"]) . "<br /> ");
		}

		echo("</b>to this job: ");
		
		echo($row["job_code"] . " " . $row["position_title"] . ".<br /><br />Are you sure that you want to do this?");

		echo("</td>");
	echo("</tr>");

	echo("<tr align='center'>");
		echo("<td align='right'>");
            echo("<input type='submit' value='Confirm' />");
        echo("</td>");
        echo("<td align='left'>");
            echo("<input type='button' value='Cancel' onclick='javascript:document.cancelform.submit();' />");
        echo("</td>");
    echo("</tr>");
    echo("</table>");
	echo("</td>");
echo("</tr>");
echo("</table>");
echo("</form>");
echo("<form method='post' action='" . $PHP_SELF . "' name='cancelform'>");
    echo("<input type='hidden' name='select' value='applications' />");
    echo("<input type='hidden' name='continue' value='view_applicants' />");
    echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo("</form>");

?>
