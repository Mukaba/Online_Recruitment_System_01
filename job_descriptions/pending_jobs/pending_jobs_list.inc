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
 | Filename: pending_jobs_list.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file loads up all of the pending jobs that match the       |
 | users department, and allows the admin to select one of them to view.        |
 +------------------------------------------------------------------------------+

*/

$departments_in_group = department_groups($auth->department);
foreach ($departments_in_group AS $dep)
{
	$department_str .= ("'" . $dep . "',");
}
$department_str = substr($department_str, 0, -1);

$sql = ("
	SELECT DISTINCT pjd.job_id, pjd.employer_id, pjd.contact_id, pjd.position_title, pjd.date_added, pcom.company_name,
	CONCAT(pcon.last_name, ', ', pcon.first_name) AS contact_name
	FROM pending_job_description AS pjd
    INNER JOIN pending_employer_company AS pcom
        ON pcom.employer_id=pjd.employer_id
    INNER JOIN pending_employer_contact AS pcon
        ON pcon.contact_id=pjd.contact_id
    WHERE pjd.job_id IS NOT NULL
	");

if ($show_jobs_for == 'group')
{
	$sql .= ("
		AND pjd.host_department IN (" . $department_str . ")
		");
}
else
{
	$sql .= ("
		AND pjd.host_department='" . $auth->department . "'
		");
}

$sql .= ("
	ORDER BY " . 
	(($order_by == 'contact_name') ? "pcon.last_name, pcon.first_name, pjd.date_added, pjd.position_title" :
	($order_by == 'company_name' ? "pcom.company_name, pjd.date_added, pjd.position_title, pcon.last_name" :
	($order_by == 'position_title' ? "pjd.position_title, pjd.date_added, pcom.company_name, pcon.last_name" :
	"pjd.date_added, pjd.position_title, pcom.company_name, pcon.last_name")))
	);
$result = $GLOBALS['dbh']->Execute($sql);

// Make sure there actually are jobs in the system for this department's group

$test_sql = ("
	SELECT *
	FROM pending_job_description
	WHERE host_department IN (" . $department_str . ")
	");
$test_result = $GLOBALS['dbh']->Execute($test_sql);

if ($test_result->RecordCount() == 0)
{
	// If there are no job in the database, indicate this 

	notify("There are currently no jobs in the system pending approval.");
}
elseif ($result->RecordCount() == 0)
{
	// They have asked to see pending jobs for only their department, and their are none.  Indicate this fact.

	echo("<table border='0' cellpadding='2' cellspacing='0'>");
        echo("<tr align='center'>");
                echo("<td class='row1'>");
                echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");

                echo("<form method='post' action='" . $PHP_SELF . "&amp;select=pending_jobs'>");

                echo("<tr align='center'>");
                	echo("<td>");
                	echo("Show jobs that are pending the approval of your:&nbsp;");
			echo("</td>");
		echo("</tr>");

		echo("<tr align='center'>");
			echo("<td>");
			echo("<select name='show_jobs_for'>");
			echo("<option value='department' ");
			if ($show_jobs_for == 'department')
			{
				echo("selected='selected'");
			}
			echo(">Department</option>");
			echo("<option value='group' ");
			if ($show_jobs_for == 'group')
			{
				echo("selected='selected'");
			}
			echo(">Group</option>");
			echo("</select>");
			echo("</td>");
		echo("</tr>");

		echo("<tr align='center'>");
			echo("<td>");
			echo("<input type='submit' value='Go' />");
			echo("</td>");
		echo("</tr>");

                echo("</table>");

                echo("</td>");
        echo("</tr>");
        echo("</table>");

	echo("<br />");

	notify("There are currently no jobs in the system pending the approval of your department.");
}
else
{
	// There are jobs in the system

	$href = ($URL . "&amp;order_by=");

	echo("<table border='0' cellpadding='2' cellspacing='0'>");
        echo("<tr align='center'>");
                echo("<td class='row1'>");
                echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");

                echo("<form method='post' action='" . $PHP_SELF . "&amp;select=pending_jobs'>");

                echo("<tr align='center'>");
                        echo("<td>");
                        echo("Show jobs that are pending the approval of your:&nbsp;");
                        echo("</td>");
                echo("</tr>");

                echo("<tr align='center'>");
                        echo("<td>");
                        echo("<select name='show_jobs_for'>");
                        echo("<option value='department' ");
                        if ($show_jobs_for == 'department')
                        {
                                echo("selected='selected'");
                        }
                        echo(">Department</option>");
                        echo("<option value='group' ");
                        if ($show_jobs_for == 'group')
                        {
                                echo("selected='selected'");
                        }
                        echo(">Group</option>");
                        echo("</select>");
                        echo("</td>");
                echo("</tr>");

                echo("<tr align='center'>");
                        echo("<td>");
                        echo("<input type='submit' value='Go' />");
                        echo("</td>");
                echo("</tr>");

                echo("</table>");

                echo("</td>");
        echo("</tr>");
        echo("</table>");

	echo("<br />");

        echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
	echo("<td>");
	
	echo("<table border='0' cellpadding='2'>");

	echo("<tr>");
		echo("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='" . $href . "position_title'>");
		echo("<b class='white'>Position Title&nbsp;</b></a></td>");

		echo("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='" . $href . "company_name'>");
		echo("<b class='white'>Company Name&nbsp;</b></a></td>");

		echo("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='" . $href . "contact_name'>");
		echo("<b class='white'>Contact Name&nbsp;</b></a></td>");

		echo("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='" . $href . "date_added'>");
		echo("<b class='white'>Date Received&nbsp;</b></a></td>");
	echo("</tr>");

	$rowclass = 0;
	while ($row = $result->FetchRow())
	{
	        $href = ($URL . "&amp;job_id=" . $row["job_id"] . "&amp;continue=");

		echo("<tr>");
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "view_job_info'>" . $row["position_title"] . "</a>&nbsp;");
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "view_contact_company_info'>" . $row["company_name"] . "</a>&nbsp;");
			echo("</td>\n");
		
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "view_contact_company_info'>" . $row["contact_name"] . "</a>&nbsp;");
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;" . $row["date_added"] . "&nbsp;");
			echo("</td>\n");
		echo("</tr>");

	        $rowclass++;
	}

	echo("</table>");
	echo("</td>");
	echo("</tr>");
	echo("</table>");
} 

?>
