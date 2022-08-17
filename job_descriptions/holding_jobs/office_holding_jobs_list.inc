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
 | Filename: office_holding_jobs_list.inc                                       |
 +------------------------------------------------------------------------------+
 | Description: This file displays all of the jobs that are currently in the    |
 | 24 hour hold status, and allows the user to either view a job's specifics,   |
 | or apply to multiple jobs.                                                   |
 +------------------------------------------------------------------------------+

*/

$sql = ("
	SELECT DISTINCT ji.job_id, ji.job_code, ji.position_title, ji.hold_begin_datetime, ec.employer_id, ec.company_name
	FROM job_info AS ji, employer_company AS ec
	WHERE ji.status='" . HOLDING . "'
	AND ji.employer_id=ec.employer_id
	AND ji.hold_end_datetime>NOW()
	");

$sql .= ("
        ORDER BY " . (($order == "company_name") ?  "ec.company_name, ji.hold_begin_datetime, ji.position_title"
	: (($order == "position_title") ? "ji.position_title, ji.hold_begin_datetime, ec.company_name"
	: (($order == "status") ? "ji.admin_status, ji.job_code, ec.company_name"
	: (($order == "job_code") ? "ji.job_code, ji.position_title, ji.hold_begin_datetime"
	: "ji.hold_begin_datetime, ji.position_title, ec.company_name"))))
        );

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0)
{
	// if there are no job in the database, indicate this 

	notify("There are currently no jobs on 24 hour hold.");
}
else      
{
	// There are jobs in the 24 hour holding pattern

	echo("<h4>Jobs in 24 Hour Hold:</h4>");

	$link_href = $PHP_SELF . "&amp;select=holding_jobs&amp;order=";

	echo("<form method='post' action='" . $PHP_SELF . "'>");
	echo("<input type='hidden' name='select' value='holding_jobs' />");
 
        echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
		echo("<td>");
		echo("<table border='0' cellpadding='2'>");

		echo("<tr>");
			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<b class='white'>Department Applied</b>&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "job_code'><b class='white'>Job Code</b></a>&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "position_title'><b class='white'>Position Title</b></a>&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "company_name'><b class='white'>Company Name</b></a>&nbsp;");
			echo("</td>");
	
			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "time_remaining'><b class='white'>Time Remaining on Hold</b></a>&nbsp;");
			echo("</td>");
		echo("</tr>");
        
	$rowclass = 0;
	while ($row = $result->FetchRow())
	{
		$total_job_ids[] = $row["job_id"];
        $position_title = $row["position_title"];
		$job_id = $row["job_id"];
		$company_name = $row["company_name"];
		$employer_id = $row["employer_id"];
		$job_code = $row["job_code"];

		$sql = ("
			SELECT DISTINCT job_id
			FROM department_job_join
			WHERE job_id='" . $row["job_id"] . "'
			AND department_id='" . $auth->department . "'
			");
		$result2 = $GLOBALS['dbh']->Execute($sql);
		$already_applied = $result2->RecordCount();

		// Determine the time remaining

		$time_remaining = getHoldTimeRemaining($row["hold_begin_datetime"]);
		$split_time = split(":", $time_remaining);
		$long_time_remaining = $split_time[0] . " hours, " . $split_time[1] . " minutes";

        $href = $PHP_SELF . "&amp;select=holding_jobs&amp;job_id=" . urlencode($job_id) . "&amp;continue=view_specifics_of_job";
		$company_href = $PHP_SELF . "&amp;select=holding_jobs&amp;employer_id=" . urlencode($employer_id) . "&amp;continue=view_company_specifics";

        echo("<tr>");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			if (!$already_applied)
			{
				echo("&nbsp;<input type='checkbox' name='apply_to_job[]' value='" . $row["job_id"] . "' ");
				if (is_array($apply_to_job) && in_array($row["job_id"], $apply_to_job))
				{
					echo("checked='checked'");
				}
				echo(" />");
			}
			else
			{
				echo("&nbsp;<input type='checkbox' name='unapply_to_job[]' value='" . $row["job_id"] . "' ");
				if (is_array($unapply_to_job) && in_array($row["job_id"], $unapply_to_job))
				{
					echo("checked='checked'");
				}
				echo(" />");
			}
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo($already_applied ? "Yes" : "No");
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "'>$job_code</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "'>$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d"). "'>");
			echo("&nbsp;<a class='blue' href='" . $company_href . "'>$company_name</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d"). "'>");
			echo("&nbsp;$long_time_remaining&nbsp;</td>\n");

		echo("</tr>\n");

        $rowclass++;
	}
	echo("</table>");
	echo("</td>");
	echo("</tr>");
	echo("</table>");

	echo("<input type='hidden' name='total_job_ids' value='" . packObject(($total_job_ids)) . "' />");

	echo("<br />");

	echo("<table border='0' cellpadding='2' cellspacing='0'>");
	echo("<tr align='center'>");
		echo("<td class='row1'>");
		echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");
		echo("<tr>");
			echo("<td>");
			echo("<input type='submit' name='continue' value='Apply/Unapply Your Department to Selected Jobs' />");
			echo("<input type='hidden' name='continue' value='apply_unapply_office_multiple' />");
			echo("</td>");
		echo("</tr>");
		echo("</table>");
		echo("</td>");
	echo("</tr>");
	echo("</table>");

	echo("</form>");
}

?>

