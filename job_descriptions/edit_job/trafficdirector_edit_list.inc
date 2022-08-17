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
 | Filename: trafficdirector_edit_list.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file loads up all of the jobs, and allows the              |
 | trafficdirector to select one of them to edit.                               |
 +------------------------------------------------------------------------------+

*/

$nextTermYear = getNextTermYear();

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_job' />");

echo("<table border='0' cellpadding='2' cellspacing='0'>");

echo("<tr align='center'>");
echo("<td class='row1'>");
echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");

echo("<tr align='center'>");
	echo("<td align='right'>");
	echo("Show Future Jobs: ");
	echo("</td>");

	echo("<td align='left'>");
	echo("<select name='show_future'>");
	echo("<option value='show_future_jobs' ");
	if ($show_future == 'show_future_jobs')
	{
		echo("selected='selected'");
	}
	echo(">Yes</option>");
	
	echo("<option value='hide_future_jobs' ");
	if (!$show_future || $show_future == 'hide_future_jobs')
	{
		echo("selected='selected'");
	}
	echo(">No</option>");

	echo("</select>");
	echo("</td>");
echo("</tr>");

$sql = ("
	SELECT DISTINCT department_id, department_name
	FROM department
	WHERE job_list
	ORDER BY department_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<tr align='center'>");
	echo("<td align='right'>");
	echo("Show jobs for: ");
	echo("</td>");
	echo("<td align='left'>");
	echo("<select name='show_jobs'>");
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["department_id"] . "' ");
		echo($show_jobs == $row["department_id"] ? "selected='selected'" : "");
		echo(">" . $row["department_name"] . "</option>");
	}
	echo("<option value='uvic' ");
	echo(!$show_jobs || $show_jobs == 'uvic' ? "selected='selected'" : "");
	echo(">All of " . ORGANIZATION_CODE . "</option>");
	echo("</select>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<input type='submit' value='Go' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");

echo("</table>");

echo("</form>");

if (!$show_jobs)
{
	$show_jobs = 'uvic';
}
elseif ($show_jobs != 'uvic')
{
	$extra_sql .= ("
		AND ji.department_id='" . $show_jobs . "'
		");
}

$sql = ("
	SELECT DISTINCT ji.position_title, ji.closing_date, ji.job_id, ji.job_code
	FROM job_info AS ji
	WHERE ji.term_id='" . $nextTermYear["term"] . "'
	AND ji.year='" . $nextTermYear["year"] . "'
	AND ji.status!=" . INT_ONLY . "
	");
$sql .= $extra_sql;

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0)
{
	// if there are no jobs in the database, indicate this 

	if ($show_jobs != 'uvic')
	{
		$sql = ("
			SELECT DISTINCT department_name
			FROM department
			WHERE department_id='" . $show_jobs . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		
		notify("There are no current jobs for <b>" . $row["department_name"] . "</b> registered in the system.");
	}
	else
	{
		notify("There are no current jobs registered in the system.");
	}
} 
else      
{
	// there are jobs in the system

	$sql = ("
		SELECT DISTINCT ji.closing_date, ji.job_code, ji.position_title, ji.job_id, ji.admin_status, ec.company_name
		FROM job_info AS ji, employer_company AS ec
		WHERE ji.status=" . NOT_CHECKED . "
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
		");
	$sql .= $extra_sql;
	
	$sql .= ("
		ORDER BY ji.closing_date, ji.job_code, ji.position_title
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() != 0)
	{
	echo("<h4>Unchecked Jobs:</h4>");

        echo("
        <table cellspacing='0' cellpadding='0' border='1'><tr><td>
        <table border='0' cellpadding='2'>
        <tr>  
	<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
	<td class='rowgrey' align='center'>&nbsp;<b class='white'>Closing Date</b>&nbsp;</td>
	<td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
	<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
	<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
	</tr>
	");
        
	$rowclass = 0;
	while ($row = $result->FetchRow())
	{
        	$position_title = $row["position_title"];
	        if (validDate($row["closing_date"]) != -1)
		{
			$closing_date = $row["closing_date"];
		}
		else
		{
			$closing_date = "Not yet set";
		}
		if ($row["job_code"])
		{
			$job_code = $row["job_code"];
		}
		else
		{
			$job_code = "Not yet set";
		}
		$job_id = $row["job_id"];
		$company_name = $row["company_name"];
		
	        $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

        	echo("<tr>");

		echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
		echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

       		echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
		echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

	       	echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
		echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

		echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		$admin_status_gif = getAdminStatusGif($job_id);
		echo($admin_status_gif);
		echo("</td>\n");

		echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

		echo("</tr>\n");

	        $rowclass++;
	}
	echo("</table>");
	echo("</td></tr></table>");
	} 

	$sql = ("
		SELECT DISTINCT ji.closing_date, ji.job_code, ji.job_id, ji.position_title, ji.admin_status, ec.company_name
		FROM job_info AS ji, employer_company AS ec
		WHERE ji.status=" . CHECKED . "
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
		");
	$sql .= $extra_sql;

	$sql .= ("
		ORDER BY ji.closing_date, ji.job_code, ji.position_title
		");

	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() != 0)
	{
		echo("<h4>Checked Jobs (not yet posted):</h4>");
		
		echo("
		<table cellspacing='0' cellpadding='0' border='1'><tr><td>
		<table border='0' cellpadding='2'>
		<tr>  
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Closing Date</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
		</tr>
		");

		$rowclass = 0;
		while ($row = $result->FetchRow())
		{
			$position_title = $row["position_title"];
			if (validDate($row["closing_date"]) != -1)
			{
				$closing_date = $row["closing_date"];
			}
			else
			{
				$closing_date = "Not yet set";
			}
			if ($row["job_code"])
			{
				$job_code = $row["job_code"];
			}
			else
			{
				$job_code = "Not yet set";
			}
			$job_id = $row["job_id"];
			$company_name = $row["company_name"];
				
			$href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

			echo("<tr>");
			
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			$admin_status_gif = getAdminStatusGif($job_id);
			echo($admin_status_gif);
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

			echo("</tr>\n");
			$rowclass++;
		}
		echo("</table>");
		echo("</td></tr></table>");
	} 
	
	// Display the holding jobs.

	$sql = ("
		SELECT DISTINCT ji.closing_date, ji.job_code, ji.position_title, ji.job_id, ji.admin_status, ec.company_name
		FROM job_info AS ji, employer_company AS ec
                WHERE ji.status=" . HOLDING . "
                AND ji.term_id='" . $nextTermYear["term"] . "'
                AND ji.year='" . $nextTermYear["year"] . "'
                AND ec.employer_id=ji.employer_id
                ");
        $sql .= $extra_sql;

        $sql .= ("
                ORDER BY ji.closing_date, ji.job_code, ji.position_title
                ");

	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount() != 0)
	{
		echo("<h4>Holding Jobs:</h4>");

		echo("
		<table cellspacing='0' cellpadding='0' border='1'><tr><td>
		<table border='0' cellpadding='2'>
		<tr>  
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Closing Date</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
		</tr>
		");
		
		$rowclass = 0;
		while ($row = $result->FetchRow())
		{
			$position_title = $row["position_title"];
			if (validDate($row["closing_date"]) != -1)
			{
				$closing_date = $row["closing_date"];
			}
			else
			{
				$closing_date = "Not yet set";
			}
			if ($row["job_code"])
			{
				$job_code = $row["job_code"];
			}
			else
			{
				$job_code = "Not yet set";
			}
			$job_id = $row["job_id"];
			$company_name = $row["company_name"];
			
			$href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

			echo("<tr>");
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			$admin_status_gif = getAdminStatusGif($job_id);
			echo($admin_status_gif);
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

			echo("</tr>\n");
			$rowclass++;
		}
			
		echo("</table>");
		echo("</td></tr></table>");
	}
	// Display posted jobs

	$sql = ("
		SELECT DISTINCT ji.position_title, ji.closing_date, ji.job_id, ji.job_code, ji.admin_status, ec.company_name
		FROM job_info AS ji, employer_company AS ec
		WHERE ji.status=" . POSTED . "
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
		");

	$sql .= $extra_sql;
	
	$sql .= ("
		ORDER BY ji.closing_date, ji.job_code, ji.position_title
		");
	
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount() != 0)
	{
		echo("<h4>Open Jobs:</h4>");

		echo("
		<table cellspacing='0' cellpadding='0' border='1'><tr><td>
		<table border='0' cellpadding='2'>
		<tr>  
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Closing Date</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
		</tr>
		");
		
		$rowclass = 0;
		while ($row = $result->FetchRow())
		{
			$position_title = $row["position_title"];
			if (validDate($row["closing_date"]) != -1)
			{
				$closing_date = $row["closing_date"];
			}
			else
			{
				$closing_date = "Not yet set";
			}
			if ($row["job_code"])
			{
				$job_code = $row["job_code"];
			}
			else
			{
				$job_code = "Not yet set";
			}
			$job_id = $row["job_id"];
			$company_name = $row["company_name"];
			
			$href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

			echo("<tr>");
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			$admin_status_gif = getAdminStatusGif($job_id);
			echo($admin_status_gif);
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

			echo("</tr>\n");
			$rowclass++;
		}
			
		echo("</table>");
		echo("</td></tr></table>");
	}


	$sql = ("
		SELECT DISTINCT ji.closing_date, ji.job_code, ji.position_title, ji.job_id, ji.admin_status, ec.company_name
		FROM job_info AS ji, employer_company AS ec
		WHERE ji.status=" . CLOSED . "
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
		");

	$sql .= $extra_sql;

	$sql .= ("
		ORDER BY ji.closing_date, ji.job_code, ji.position_title
		");
	
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() != 0)
	{
		echo("<h4>Closed Jobs:</h4>");
		
		echo("
		<table cellspacing='0' cellpadding='0' border='1'><tr><td>
		<table border='0' cellpadding='2'>
		<tr>  
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Closing Date</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
		</tr>
		");

		$rowclass = 0;
		while ($row = $result->FetchRow())
		{
			$position_title = $row["position_title"];
			if (validDate($row["closing_date"]) != -1)
			{
				$closing_date = $row["closing_date"];
			}
			else
			{
				$closing_date = "Not yet set";
			}
			if ($row["job_code"])
			{
				$job_code = $row["job_code"];
			}
			else
			{
				$job_code = "Not yet set";
			}
			$job_id = $row["job_id"];
			$company_name = $row["company_name"];
			
			$href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

			echo("<tr>");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			$admin_status_gif = getAdminStatusGif($job_id);
			echo($admin_status_gif);
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

			echo("</tr>\n");
			$rowclass++;
		}
		echo("</table>");
		echo("</td></tr></table>");
	} 

	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT ji.position_title, ji.job_code, ji.closing_date, ji.job_id, ji.admin_status, ec.company_name
		FROM job_info AS ji, employer_company AS ec
		WHERE ji.status=" . CANCELLED . "
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
		ORDER BY ji.closing_date, ji.job_code, ji.position_title
		");

	if ($result->RecordCount() != 0)
	{
		echo("<h4>Cancelled Jobs:</h4>");

		echo("
		<table cellspacing='0' cellpadding='0' border='1'><tr><td>
		<table border='0' cellpadding='2'>
		<tr>  
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Closing Date</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
		</tr>
		");
		
		$rowclass = 0;
		while ($row = $result->FetchRow())
		{
			$position_title = $row["position_title"];
			if (validDate($row["closing_date"]) != -1)
			{
				$closing_date = $row["closing_date"];
			}
			else
			{
				$closing_date = "Not yet set";
			}
			if ($row["job_code"])
			{
				$job_code = $row["job_code"];
			}
			else
			{
				$job_code = "Not yet set";
			}
			$job_id = $row["job_id"];
			$company_name = $row["company_name"];
		
			$href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);
			echo("<tr>");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			$admin_status_gif = getAdminStatusGif($job_id);
			echo($admin_status_gif);
			echo("</td>\n");
		
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

			echo("</tr>\n");
			$rowclass++;
		}
		echo("</table>");
		echo("</td></tr></table>");
	} 
}

// If they would like to see future jobs, then display this for them.

if ($show_future == 'show_future_jobs')
{
	echo("<br />");

	$sql = ("
		SELECT DISTINCT start_date, end_date
		FROM term
		WHERE term_id='" . $nextTermYear["term"] . "'
		");

	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$start_end = $row;
	
	$sql = ("
	        SELECT DISTINCT ji.position_title, ji.closing_date, ji.job_id, ji.job_code, ji.term_id, ji.year, ji.admin_status, ec.company_name
	        FROM job_info AS ji, term AS b, employer_company AS ec
		WHERE ji.status!=" . INT_ONLY . "
		AND ( (ji.term_id=b.term_id AND b.start_date>'" . $start_end["end_date"] . "' AND ji.year='" . $nextTermYear["year"] . "')
		OR ji.year>'" . $nextTermYear["year"] . "' )
		AND ec.employer_id=ji.employer_id
	        ");

	$sql .= $extra_sql;

	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount() == 0)
	{
	        // if there are no job in the database, indicate this
	
		if ($show_jobs != 'uvic')
		{
			$sql = ("
				SELECT DISTINCT department_name
				FROM department
				WHERE department_id='" . $show_jobs . "'
				");
			$result2 = $GLOBALS['dbh']->Execute($sql);
			$row2 = $result2->FetchRow();
		
			notify("There are no future jobs for <b>" . $row2["department_name"] . "</b> registered in the system.");
		}
		else
		{
	        	notify("There are no future jobs registered in the system.");
		}
	} 
	else
	{
	        // there are jobs in the system
	
	        echo("<h4>Future Jobs:</h4>");
	
		echo("
	        <table cellspacing='0' cellpadding='0' border='1'><tr><td>
	        <table border='0' cellpadding='2'>
	        <tr>
	        <td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
	        <td class='rowgrey' align='center'>&nbsp;<b class='white'>Closing Date</b>&nbsp;</td>
	        <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Term</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
	        </tr>
	        ");

	        $rowclass = 0;
	        while ($row = $result->FetchRow())
	        {
	                $position_title = $row["position_title"];
	                if (validDate($row["closing_date"]) != -1)
	                {
	                        $closing_date = $row["closing_date"];
	                }
	                else
	                {
	                        $closing_date = "Not yet set";
	                }
	                if ($row["job_code"])
	                {
	                        $job_code = $row["job_code"];
	                }
	                else
	                {
	                        $job_code = "Not yet set";
	                }
	                $job_id = $row["job_id"];
			$term_id = $row["term_id"];
			$year = $row["year"];
			$company_name = $row["company_name"];
	
	                $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);
	
	                echo("<tr>");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
	                echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

	                echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
	                echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

	                echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
	                echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo(getTermName($term_id) . ", " . $year . "</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			$admin_status_gif = getAdminStatusGif($job_id);
			echo($admin_status_gif);
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

	                echo("</tr>\n");
	                $rowclass++;
		        }
	
	                echo("</table>");
	                echo("</td></tr></table>");
	}
}

// Status icon key

echo("<h4>Status Icon Key:</h4>");

echo("<table cellspacing='0' cellpadding='0' border='1'>");
echo("<tr>");
echo("<td>");
echo("<table border='0' cellpadding='2'>");

echo("<tr>");
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
	
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
echo("</tr>");

$admin_status_array = getAdminStatusGifArray();
$newrow = 0;
$rowclass = 0;

foreach ($admin_status_array AS $asa)
{
	if (!($newrow % 2))
	{
		echo("<tr>");
	}
	echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
	echo($asa["gif"]);
	echo("</td>\n");
	
	echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
	echo($asa["name"]);
	echo("</td>\n");

	if ($newrow % 2)
	{
		echo("</tr>");
		$rowclass++;
	}

	$newrow++;
}

if ($newrow % 2)
{
	// Fill out any extra columns
	echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;</td>");
	echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;</td>");
	echo("</tr>");
}

echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");

?>
