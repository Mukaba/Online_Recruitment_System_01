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
 | Filename: traffic_director_job_list.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file loads up all of the pending jobs for the traffic      |
 | director, and allows the user to select one of them to view.                 |
 +------------------------------------------------------------------------------+

*/

$sql = ("
	SELECT DISTINCT pjd.*
	FROM pending_job_description AS pjd
    INNER JOIN pending_employer_company AS pcom
        ON pcom.employer_id=pjd.employer_id
    INNER JOIN pending_employer_contact AS pcon
        ON pcon.contact_id=pjd.contact_id
	");
$result = $GLOBALS['dbh']->Execute($sql);

$sql2 = ("
	SELECT DISTINCT *
	FROM job_info
	WHERE (department_id=''
	OR department_id IS NULL)
    AND status != " . CANCELLED . "
	");
$result2 = $GLOBALS['dbh']->Execute($sql2);

if ($result->RecordCount() == 0 && $result2->RecordCount() == 0)
{
	// If there are no job in the database, indicate this 
	notify("There are currently no jobs in the system pending approval, and no approved jobs without host departments.");
}
else
{
	// There are jobs in the system, display jobs with host departments first.

	$sql = ("
		SELECT DISTINCT pjd.job_id, pjd.employer_id, pjd.contact_id, pjd.position_title, pjd.date_added, pcom.company_name,
		CONCAT(pcon.last_name, ', ', pcon.first_name) AS contact_name
		FROM pending_job_description AS pjd
        INNER JOIN pending_employer_company AS pcom
            ON pcom.employer_id=pjd.employer_id
        INNER JOIN pending_employer_contact AS pcon
            ON pcon.contact_id=pjd.contact_id
		WHERE pjd.host_department!='0'
		");
	
	$sql .= ("
		ORDER BY " . 
		(($order_by == 'contact_name') ? "pcon.last_name, pcon.first_name, pjd.date_added, pjd.position_title" :
		($order_by == 'company_name' ? "pcom.company_name, pjd.date_added, pjd.position_title, pcon.last_name" :
		($order_by == 'position_title' ? "pjd.position_title, pjd.date_added, pcom.company_name, pcon.last_name" :
		"pjd.date_added, pjd.position_title, pcom.company_name, pcon.last_name")))
		);
	
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount())
	{
		$href = ($URL . "&amp;order_by=");

		echo("<h4>Jobs with host departments:</h4>");
	
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
			$ignore_contact_ids .= ("'" . $row["contact_id"] . "',");
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

	echo("<br />");

	$sql = ("
                SELECT DISTINCT pjd.job_id, pjd.employer_id, pjd.contact_id, pjd.position_title, pjd.date_added, pcom.company_name,
                CONCAT(pcon.last_name, ', ', pcon.first_name) AS contact_name
                FROM pending_job_description AS pjd, pending_employer_company AS pcom, pending_employer_contact AS pcon
                WHERE pcom.employer_id=pjd.employer_id
                AND pcon.contact_id=pjd.contact_id
                AND pjd.host_department='0'
                ");

        $sql .= ("
                ORDER BY " .
                (($order_by == 'contact_name') ? "pcon.last_name, pcon.first_name, pjd.date_added, pjd.position_title" :
                ($order_by == 'company_name' ? "pcom.company_name, pjd.date_added, pjd.position_title, pcon.last_name" :
                ($order_by == 'position_title' ? "pjd.position_title, pjd.date_added, pcom.company_name, pcon.last_name" :
                "pjd.date_added, pjd.position_title, pcom.company_name, pcon.last_name")))
                );

        $result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount())
	{
		$href = ($URL . "&amp;order_by=");

		echo("<h4>Unassigned Jobs:</h4>");

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
			$ignore_contact_ids .= ("'" . $row["contact_id"] . "',");
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

	echo("<br />");

	$sql = ("
                SELECT DISTINCT ji.job_id, ji.employer_id, ji.contact_id, ji.position_title, ji.date_posted, ec.company_name,
                CONCAT(c.last_name, ', ', c.first_name) AS contact_name
                FROM job_info AS ji, employer_company AS ec, contact AS c
                WHERE ec.employer_id=ji.employer_id
                AND c.contact_id=ji.contact_id
                AND ji.department_id='0'
                ");

        $sql .= ("
                ORDER BY " .
                (($order_by_2 == 'contact_name') ? "c.last_name, c.first_name, ji.date_posted, ji.position_title" :
                ($order_by_2 == 'company_name' ? "ec.company_name, ji.date_posted, ji.position_title, c.last_name" :
                "ji.position_title, ji.date_posted, ec.company_name, c.last_name" ))
                );

	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount())
	{
		$href = ($URL . "&amp;order_by_2=");

		echo("<h4>Jobs that are no longer pending, but do not have assigned host departments:</h4>");

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
                echo("</tr>");

                $rowclass = 0;
                while ($row = $result->FetchRow())
                {
                        $href = ($PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . $row["job_id"]);

                        echo("<tr>");
                                echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
                                echo("&nbsp;<a class='blue' href='" . $href . "'>" . $row["position_title"] . "</a>&nbsp;");
                                echo("</td>\n");

                                echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
                                echo("&nbsp;<a class='blue' href='" . $href . "'>" . $row["company_name"] . "</a>&nbsp;");
                                echo("</td>\n");

                                echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
                                echo("&nbsp;<a class='blue' href='" . $href . "'>" . $row["contact_name"] . "</a>&nbsp;");
                                echo("</td>\n");
                        echo("</tr>");

                        $rowclass++;
                }

                echo("</table>");
                echo("</td>");
                echo("</tr>");
                echo("</table>");
        }
}

if ($ignore_contact_ids)
{
	$ignore_contact_ids = substr($ignore_contact_ids, 0, -1);
}
else
{
	$ignore_contact_ids = ("''");
}

$sql = ("
	SELECT DISTINCT pcom.company_name, CONCAT(pcon.last_name, ', ', pcon.first_name) AS contact_name, pcom.employer_id, pcon.contact_id, pcon.entered_on
	FROM pending_employer_contact AS pcon
	LEFT JOIN pending_employer_company AS pcom ON pcon.employer_id=pcom.employer_id
	WHERE pcon.contact_id NOT IN (" . $ignore_contact_ids . ")
    AND pcon.module = 'job'
	");

$sql .= ("
	ORDER BY " . (($order_by_3 == 'contact_name') ? "pcon.last_name, pcon.first_name, pcom.company_name" : 
	($order_by_3 == 'entered_on') ? "pcon.entered_on, pcom.company_name, pcon.last_name" : 
	"pcom.company_name, pcon.last_name")
	);

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	echo("<h4>Incomplete entries</h4>");

	echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
	echo("<td>");

	echo("<table border='0' cellpadding='2'>");

	$href = ($URL . "&amp;order_by_3=");

	echo("<tr>");
		echo("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='" . $href . "company_name'>");
		echo("<b class='white'>Contact Name&nbsp;</b></a></td>");

		echo("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='" . $href . "contact_name'>");
		echo("<b class='white'>Company Name&nbsp;</b></a></td>");

		echo("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='" . $href . "entered_on'>");
		echo("<b class='white'>Entered On&nbsp;</b></a></td>");
	echo("</tr>");

	$rowclass = 0;
	while ($row = $result->FetchRow())
	{
		$href = ($URL . "&amp;contact_id=" . $row["contact_id"] . "&amp;continue=");

		echo("<tr>");
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "view_contact_company_info_incomplete'>" . $row["contact_name"] . "</a>&nbsp;");
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			if ($row["company_name"])
			{
				echo("&nbsp;<a class='blue' href='" . $href . "view_contact_company_info_incomplete'>" . $row["company_name"] . "</a>&nbsp;");
			}
			else
			{
				echo("&nbsp;None Entered&nbsp;");
			}
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo($row["entered_on"]);
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
