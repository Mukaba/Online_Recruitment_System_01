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
 | Filename: existing_job_choices.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called to display the existing job choices to      |
 | the user, or give them the option to enter a job code and create a new       |
 | interview from scratch.                                                      |
 +------------------------------------------------------------------------------+

 //:TODO: This page's pagination needs fixing, it's inefficient.
*/

// These variables will help reduce the problem of too many job's on a screen

if ($per_page_max == "") { $per_page_max = 50; }
if ($per_page_max < 5) { $per_page_max = 5; }
if ($start_row == '') { $start_row = 0; }

// Get the current term and year.

$sql = ("
 	SELECT CURRENT_DATE() AS c_date
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$current_date = explode('-', $row["c_date"]);

$current_year = $current_date[0];
$current_term = getCurrentTermID();

// Get the next term and year

$nextTermYear = getNextTermYear();

// These variables determine which term and year we are looking at.

if ($show_term_year)
{
	$job_term_year = explode(" ", $show_term_year);
	$job_term_id = $job_term_year[0];
	$job_year = $job_term_year[1];
}
else
{
	$show_term_year = $nextTermYear["term"] . " " . $nextTermYear["year"];
	if ($job_term_id == '') { $job_term_id = $nextTermYear["term"]; }
	if ($job_year == '') { $job_year = $nextTermYear["year"]; }
}
/* 
Commented out by: Jon
Date: 2004-09-22
Reason: These have been commented out and moved to line 121 to reverse ordering of dates in pulldown

$jobTermYear[] = array(
	"option_name" => getTermName($current_term) . ", " . $current_year,
	"option_value" => $current_term . " " . $current_year
	);*/
/*$jobTermYear[] = array(
	"option_name" => getTermName($nextTermYear["term"]) . ", " . $nextTermYear["year"],
	"option_value" => $nextTermYear["term"] . " " . $nextTermYear["year"]
	);*/
/*$jobTermYear[] = array(
    "option_name" => getTermName($current_term) . ", " . $current_year,
        "option_value" => $current_term . " " . $current_year
            );*/
if ($nextTermYear["term"] == 1)
{
	$jobTermYear[] = array(
		"option_name" => getTermName(2) . ", " . ($nextTermYear["year"] + 1),
		"option_value" => "2 " . ($nextTermYear["year"] + 1)
		);
}
elseif ($nextTermYear["term"] == 3)
{
	$jobTermYear[] = array(
		"option_name" => getTermName(1) . ", " . $nextTermYear["year"],
		"option_value" => "1 " . $nextTermYear["year"]
		);
}
else
{
	$jobTermYear[] = array(
		"option_name" => getTermName(3) . ", " . $nextTermYear["year"],
		"option_value" => "3 " . $nextTermYear["year"]
		);
}
$jobTermYear[] = array(
    "option_name" => getTermName($nextTermYear["term"]) . ", " . $nextTermYear["year"],
        "option_value" => $nextTermYear["term"] . " " . $nextTermYear["year"]
            );

$jobTermYear[] = array(
    "option_name" => getTermName($current_term) . ", " . $current_year,
            "option_value" => $current_term . " " . $current_year
                        );

// Get Job id's with interviews so that we don't include them !

$sql = ("
	SELECT job_id from interview
	");
$result = $GLOBALS['dbh']->Execute($sql);

$job_ids_interview = "(";
while ($row = $result->FetchRow()) {
	$job_ids_interview .= "'". $row["job_id"] ."',";
}

// Now get job ids that have an interview, but are part of a multiple job interview (won't show up in previous query).
$sql = ("
	SELECT DISTINCT job_id
	FROM interview_job_join
	");
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
	$job_ids_interview .= "'" . $row["job_id"] . "',";
}

if ($job_ids_interview != "(") { 
	$job_ids_interview = substr($job_ids_interview, 0, -1) . ")";
} else {
	$job_ids_interview = "('')";
}

// Now get department ids for this department and its groups.
$department_ids = department_groups($auth->department);
foreach ($department_ids AS $di)
{
	$dept_ids .= ("'" . $di . "',");
}

if ($dept_ids)
{
	$dept_ids = substr($dept_ids, 0, -1);
}
else
{
	$dept_ids = ("''");
}

/*
 The code below will look pretty ugly.  We need to first determine whether or not we actually
 have shortlisted and current jobs in the system that interviews can be created for.  Then,
 we need to show ONLY those jobs that match the chosen term and year.
*/

// Get Shortlisted Jobs

$sql = "SELECT DISTINCT COUNT(s.job_id) as count, j.job_code, e.email, e.company_name, j.job_id 
	FROM job_info as j, students_shortlisted AS s
	LEFT JOIN employer_company AS e ON j.employer_id = e.employer_id
	WHERE j.job_id = s.job_id 
	AND s.job_id NOT IN $job_ids_interview
	AND j.department_id IN (" . $dept_ids . ")
	";
$extra_sql_1 = ("
	AND j.term_id='" . $job_term_id . "'
	AND j.year='" . $job_year . "'
	");

$extra_sql_2 = ("
	GROUP BY s.job_id
	");

$jobs_shortlisted_result = $GLOBALS['dbh']->Execute($sql . $extra_sql_2);
$jobs_are_shortlisted = $jobs_shortlisted_result->RecordCount();

$sql .= $extra_sql_1;
$sql .= $extra_sql_2;

$get_shortlisted = $GLOBALS['dbh']->Execute($sql);

$shortlist_result = $GLOBALS['dbh']->Execute($sql);
while ($shortlist_row = $shortlist_result->FetchRow())
{
	$shortlist_ids .= ("'" . $shortlist_row["job_id"] . "',");
}
if ($shortlist_ids)
{
	$shortlist_ids = substr($shortlist_ids, 0, -1);
}
else
{
	$shortlist_ids = ("''");
}

// Get jobs that aren't shortlisted, but have closed.

$sql = ("
	SELECT DISTINCT ji.job_id, ji.job_code, ji.position_title, ec.company_name, ed.department_name
	FROM job_info AS ji
	LEFT JOIN employer_department AS ed ON ed.department_id=ji.employer_department_id
	LEFT JOIN employer_company AS ec ON ec.employer_id=ji.employer_id
	WHERE ji.status='" . CLOSED . "'
	AND ji.hiring_status='" . PENDING . "'
	AND ji.job_id NOT IN $job_ids_interview
	AND ji.job_id NOT IN (" . $shortlist_ids . ")
	AND ji.position_title!=''
	AND ji.position_title IS NOT NULL
	AND ji.department_id IN (" . $dept_ids . ")
	");

$jobs_closed_result = $GLOBALS['dbh']->Execute($sql);

$extra_sql = ("
	AND ji.term_id='" . $job_term_id . "'
	AND ji.year='" . $job_year . "'
	ORDER BY job_code
	");
$sql .= $extra_sql;

$closed_copy = $GLOBALS['dbh']->Execute($sql);

$sql .= ("
	LIMIT " . $start_row . ", " . $per_page_max
	);

$closed_jobs = $GLOBALS['dbh']->Execute($sql);

$jobs_are_closed = $jobs_closed_result->RecordCount();

$num_shortlisted = $get_shortlisted->RecordCount();
$num_closed = $closed_copy->RecordCount();
	
if ($jobs_are_shortlisted || $jobs_are_closed)
{
	instruct("Please enter a job code to create a new interview, or choose from the jobs below to add an interview for an existing job(s):");
					
	// Option to Enter own job code
	
	echo("<h4>New Interview (no existing job):</h4>");
	
	echo("<table cellpadding='4' border='0'>");
	
	echo("<tr class='row1'>");
	echo("<td colspan='3' align='center'>");
	
	echo("<form method='post' action='" . $PHP_SELF . "'>");
	echo("<input type='hidden' name='select' value='add' />");
	echo("<input type='hidden' name='job_select' value='yes' />");
	echo("<input type='hidden' name='provided_job_code' value='true' />");
	
	echo("Job code:&nbsp;&nbsp;");
	echo("<input type='text' name='job_code' value='" . $job_code . "' maxlength='120' />");
	echo("&nbsp;&nbsp;<input type='submit' value='Go' />");
	echo("</form>");
	echo("</td>");
	echo("</tr>");
	
	echo("</table>");
	
	echo("<form method='post' action='" . $PHP_SELF . "'>");
	echo("<input type='hidden' name='select' value='add' />");
	echo("<input type='hidden' name='job_select' value='yes' />");

	echo("<h4>-- Or --</h4>");
	echo("<table class='row1' cellpadding='4' cellspacing='0'><tr><td>");
	echo("Show jobs for:&nbsp;");
	echo("<select name='show_term_year'>");
	foreach ($jobTermYear AS $jty)
	{
		echo("<option value='" . $jty["option_value"] . "' ");
		if ($show_term_year == $jty["option_value"])
		{
			echo("selected='selected'");
		}
		echo(">" . $jty["option_name"] . "</option>");
	}
	echo("</select>");
	echo("&nbsp;");
	echo("<input type='submit' name='change_limit' value='Go' />");
	echo("</td></tr></table>");

	// Display shortlisted jobs

	if ($num_shortlisted) {
		echo("<h4>Jobs already shortlisted:</h4>");


		echo("<table cellspacing='0' cellpadding='0' border='1'>");
                        echo("<tr>");
                        echo("<td>\n");

                        echo("<table border='0' cellpadding='2' width='100%'>");

		echo("<tr>");
			echo("<td class='rowgrey'><b class='white'>&nbsp;</b></td>");
			echo("<td class='rowgrey'><b class='white'>Job Code</b></td>");
			echo("<td class='rowgrey'><b class='white'>Company Name</b></td>");
			echo("<td class='rowgrey'><b class='white'>Students Shortlisted</b></td>");
		echo("</tr>");


		while ($row = $get_shortlisted->FetchRow()) { 
		
			// Get Students that are Shortlisted
			$students_shortlisted = "";
			$sql = "SELECT s.first_name, s.last_name from students_shortlisted as t left join student as s ON s.student_number = t.student_number where job_id = '". $row["job_id"]."'";
			$get_students = $GLOBALS['dbh']->Execute($sql);
			while ($row_student = $get_students->FetchRow()) {
				$students_shortlisted .= $row_student["first_name"] . " " . $row_student["last_name"] . "\n";
			}
			$students_shortlisted = substr($students_shortlisted, 0, -1);


		(($i%2 == 0) ? $class = "row0d" : $class = "row1d");
			echo("<tr class='" . $class . "'>");
				echo("<td>");
				echo("<input type='checkbox' name='jobs_for_interview[]' value='" . $row["job_id"] . "'");
				if ($jobs_for_interview && in_array($row["job_id"], $jobs_for_interview))
				{
					echo(" checked='checked'");
				}
				echo(" /></td>");
				echo("<td>" . $row["job_code"] . "</td>");
				echo("<td>" . $row["company_name"] . "</td>");
				echo("<td>" . $row["count"] . "</td>");
			echo("</tr>");

			$i++;
		}

		echo("<tr class='" . (($i % 2) ? "row1d" : "row0d") . "'>");
			echo("<td colspan='4' align='center'>");
			echo("<input type='submit' name='existing_job_interview' value='Create interview for selected jobs' />");
			echo("</td>");
		echo("</tr>");

		echo("</table>");

		echo("</td>");
		echo("</tr>");
		echo("</table>");
	}

	// Display closed jobs
	
	$pages = ceil($num_closed / $per_page_max);

	if ($num_closed)
	{
		echo("<h4>Jobs that have closed:</h4>");

		echo("<table cellspacing='0' cellpadding='4' border='0'>");

		echo("<tr class='row1'>");
	        echo("<td colspan='2'>");
	        echo($num_closed . " job" . (($count != 1) ? "s" : "") . " on " . $pages . " page" . (($pages > 1) ? "s" : ""));
	        echo("&nbsp;<input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
	        echo(" jobs per page.");
	        echo("</td>");
	        echo("</tr>");

	        echo("<tr class='row1'>");

	        echo("<td colspan='2'>");
		echo("<select name='start_row'>");

		for ($i = 0; $i < $pages; $i++)
		{
			$row_num_start = $i * $per_page_max;
			$row_num_end = $row_num_start + $per_page_max - 1;
			if ($row_num_end >= $num_closed) $row_num_end = $num_closed - 1;
	
            $closed_copy->Move($row_num_start);
            $job_code_row = $closed_copy->FetchRow();
            $job_code_start = getJobCode($job_code_row[0]);

			if (strlen($job_code_start) > 15) { $job_code_start = substr($job_code_start, 0, 13) . "..."; }
	
            $closed_copy->Move($row_num_end);
            $job_code_row = $closed_copy->FetchRow();
            $job_code_end = getJobCode($job_code_row[0]);

			if (strlen($job_code_end) > 15) { $job_code_end = substr($job_code_end, 0, 13) . "..."; }
	
			echo("<option value='" . $row_num_start . "' ");
			if ($start_row == $row_num_start)
			{
				echo("selected='selected'");
			}
			echo(">" . $job_code_start . " to " . $job_code_end . "</option>\n");
		}
	
		echo("</select>");

		echo("&nbsp;&nbsp;");
		echo("<input type='submit' name='change_limit' value='Go' />");

		echo("</td>");
		echo("</tr>");
		echo("</table>");
			
        		echo("<table cellspacing='0' cellpadding='0' border='1'>");
		echo("<tr>");
		echo("<td>\n");

        		echo("<table border='0' cellpadding='2' width='100%'>");

		echo("<tr>");
			echo("<td class='rowgrey'>&nbsp;</td>");
			echo("<td class='rowgrey'><b class='white'>Job Code</b></td>");
			echo("<td class='rowgrey'><b class='white'>Position Title</b></td>");
			echo("<td class='rowgrey'><b class='white'>Company Name</b></td>");
		echo("</tr>");

		$i = 0;

		while ($closed_row = $closed_jobs->FetchRow())
		{
			echo("<tr class='" . (($i % 2) ? "row1d" : "row0d") . "'>");
				echo("<td>");
				echo("<input type='checkbox' name='jobs_for_interview[]' value='" . $closed_row["job_id"] . "'");
				if ($jobs_for_interview && in_array($closed_row["job_id"], $jobs_for_interview))
				{
					echo(" checked='checked'");
				}
				echo(" />");
				echo("</td>");
				
				echo("<td>");
				echo($closed_row["job_code"]);
				echo("</td>");

				echo("<td>");
				echo($closed_row["position_title"]);
				echo("</td>");

				echo("<td>");
				echo($closed_row["company_name"]);
				echo("</td>");
			echo("</tr>");

			$i++;
		}

		echo("<tr class='" . (($i % 2) ? "row1d" : "row0d") . "'>");
			echo("<td colspan='4' align='center'>");
			echo("<input type='submit' name='existing_job_interview' value='Create interview for selected jobs' />");
			echo("</td>");
		echo("</tr>");

		echo("</table>");

		echo("</td>");
		echo("</tr>");
		echo("</table>");
	}

	echo("</form>");
} 
else 
{
	$interview = new AddInterview('');
	$interview->interview_only = true;
	
	include('interview/add/add_job_form.inc');
}

?>
