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
 | Filename: job_search.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description: Display a title, and box to select term and other options.      |
 +------------------------------------------------------------------------------+

*/

/*
 They've done nothing yet, so display the list of jobs that they may review applications for.
*/

$nextTermYear = getNextTermYear ();

echo("<h3>View Applicants</h3>");

echo ("<table border='0' cellpadding='2' cellspacing='0'>");

echo ("<tr>");
echo ("<td class='row1'>");
echo ("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>"); 

echo ("<tr align='center'>");
echo ("<td>");

$show_jobs = urldecode($show_jobs);

echo ("<form method='post' action='" . $PHP_SELF . "&amp;select=applications'>");
echo ("<input type='hidden' name='level1' value='" . $level1 . "' />");

// Get the term and year that we are showing jobs for.


echo ("Show jobs for:&nbsp;");
$sql = ("
	SELECT DISTINCT term_id, term_name, end_date, start_date
	FROM term
	ORDER BY start_date
	");
$term_result = $GLOBALS['dbh']->Execute($sql);

if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
	$sql = "SELECT MAX(year) as end_year, MIN(year) as start_year
		FROM job_info
		WHERE employer_id = '$employer_id'";
}
elseif ($userlevel == ADMINEMPLOYER) {
	$sql = "SELECT MAX(ji.year) as end_year, MIN(ji.year) as start_year
		FROM job_info as ji, contact_employer as ce
		WHERE ce.contact_id = '$employer_contact_id_login'
		AND ce.employer_id = ji.employer_id";
}

$year_result = $GLOBALS['dbh']->Execute($sql);
$year_row = $year_result->FetchRow();

// !!!!!!!!!!! might wanna change, doing lastest of 2000
$start_year = (($year_row["start_year"] < 2000) ? 2000 : $year_row["start_year"]);

echo ("<select name='show_term'>");
echo ("<option value='ALL' ");
if ($show_term == 'ALL')
{
	echo ("selected='selected'");
}

echo (">ALL</option>");
while ($term_row = $term_result->FetchRow())
{
	echo ("<option value='" . $term_row["term_id"] . "' ");
	if ($show_term == $term_row["term_id"] || (!$show_term && $term_row["term_id"] == $nextTermYear["term"]))
	{
		echo ("selected='selected'");
	}
	echo (">"  . getMonth($term_row["start_date"])."</option>");
}
echo ("</select>");
echo ("&nbsp;");
echo ("<select name='show_year'>");
echo ("<option value='ALL' ");
if ($show_year == 'ALL')
{
	echo ("selected='selected'");
}
echo (">ALL</option>");
for ($i = $start_year; $i <= $year_row["end_year"]; $i++)
{
	echo ("<option value='" . $i . "' ");
	if ($show_year == $i || (!$show_year && $i == $nextTermYear["year"]))
	{
		echo ("selected='selected'");
	}
	echo (">" . $i . "</option>");
}
echo ("</select>");
echo ("</td>");
echo ("</tr>");

echo ("<tr align='center'>");
echo ("<td>");
echo ("<hr width='100%'>");
echo ("<input type='submit' value='Go' />");
echo ("</td>");
echo ("</tr>");

echo ("</table>");
echo ("</td>");
echo ("</tr>");
echo ("</table>");
echo ("</form><br />");


include('applications/employer/job_list.inc');

?>
