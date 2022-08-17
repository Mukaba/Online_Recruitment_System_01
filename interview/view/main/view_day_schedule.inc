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
 | Filename: view_day_schedule.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: show start time, location, employer and interviewer in table    |
 | form for all interviews on a given date  $date, $department must be sent by  |
 | the script that includes this file.                                          |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
$int_date = $date;

?>

<h4 align="left">Interviews on <?php echo($int_date); ?>:</h4>

<table cellspacing="0" cellpadding="0" border="1"><tr><td>
	<table border="0" cellpadding="2">
		<tr>
			<td class="rowgrey" align="center">&nbsp;<b class="white">Start Time</b>&nbsp;</td>
			<td class="rowgrey" align="center">&nbsp;<b class="white">Location</b>&nbsp;</td>
			<td class="rowgrey" align="center">&nbsp;<b class="white">Employer</b>&nbsp;</td>
			<td class="rowgrey" align="center">&nbsp;<b class="white">Interviewer</b>&nbsp;</td>
			<td class="rowgrey" align="center">&nbsp;<b class="white">Other</b>&nbsp;</td>
		</tr>

<?php

// interview query

$rst_interview = $GLOBALS['dbh']->Execute("
	SELECT a.job_id AS job_id, a.int_time AS time, j.job_code AS job_code, b.interview_medium_id AS interview_medium_id, 
	0 AS presentation, c.company_name AS company, d.interviewer_name AS interviewer, IF(NOT ISNULL(e.room_number), 
	CONCAT(e2.building_code, ' ', e.room_number), 'TBA') AS location
	FROM interview_time AS a, interview AS b, job_info AS j, interview_medium AS m, interview_place as p, employer_company AS c,
	interview_interviewer AS d
	LEFT OUTER JOIN	room AS e ON a.int_room_id=e.room_id
	LEFT OUTER JOIN	building AS e2 ON e.building_id=e2.building_id
	LEFT OUTER JOIN	interview_phone AS f ON a.time_id=f.time_id
	WHERE a.int_date='$int_date'
	AND j.job_id = b.job_id
	AND a.job_id=b.job_id
	AND j.employer_id=c.employer_id
	AND a.interviewer_id=d.interviewer_id
	AND j.department_id IN (" . $department_in_str . ")
	AND b.cancelled=0
	AND p.interview_place_id = b.interview_place_id
	AND b.interview_medium_id = m.interview_medium_id
	AND (p.description = '" . ONCAMPUS . "' OR (m.description= '" . PHONE . "'  AND ISNULL(f.time_id)))
	AND a.int_type = '1'
	AND a.student_number IS NOT NULL
	ORDER BY a.int_time");
/*
$rst_interview = $GLOBALS['dbh']->Execute("
	SELECT	a.job_id AS job_id, a.int_time AS time,
		j.job_code AS job_code, b.phone AS phone, 0 AS presentation,
		d.company_name AS company,
		e.interviewer_name AS interviewer,
		IF(NOT ISNULL(f.room_number), CONCAT(f2.building_code, ' ', f.room_number), 'TBA') AS location
	FROM	interview_time AS a,
		interview AS b,
		job_code AS j,
		employer_company AS d, 
		interview_interviewer AS e
	LEFT OUTER JOIN	room AS f
	ON 	a.int_room_id=f.room_id
	LEFT OUTER JOIN	building AS f2
	ON		f.building_id=f2.building_id
	WHERE	j.job_id = b.job_id
		a.int_date='$int_date' AND a.job_id=b.job_id
			" . (($department == "all") ? "" : "AND j.department_id=" . $department) . "
			AND b.cancelled=0
			AND d.employer_id=j.employer_id AND e.interviewer_id=a.interviewer_id
	ORDER BY a.int_time");
*/


// presentation query
$rst_presentation = $GLOBALS['dbh']->Execute("
	SELECT	a.job_id AS job_id, a.pres_time AS time, a.pres_loc AS location, j.job_code AS job_code,
		0 AS interview_medium_id, 1 AS presentation, d.company_name AS company, '' AS interviewer
	FROM presentation AS a, 
		interview AS b, 
		employer_company AS d, 
		job_info AS j
	WHERE	a.pres_date='$int_date' AND a.job_id=b.job_id AND j.job_id=b.job_id
			AND j.department_id IN (" . $department_in_str . ")
			AND j.employer_id=d.employer_id
	ORDER BY a.pres_time, a.job_id");

// first interview time query
$rst_first_interview = $GLOBALS['dbh']->Execute("
	SELECT	MIN(TIME_FORMAT(a.int_time, '%k%i%S') + 0) AS time,
				a.job_id AS job_id, a.time_id AS time_id, p.description AS description
	FROM		interview_time AS a, interview_medium AS m, interview_place AS p, interview AS b
	LEFT OUTER JOIN	interview_phone AS c
	ON			a.time_id=c.time_id
	WHERE a.int_date='" . addslashes($int_date) . "'
	AND a.job_id=b.job_id
	AND b.interview_medium_id = m.interview_medium_id
	AND m.interview_medium_id = b.interview_medium_id
	AND b.interview_place_id = p.interview_place_id
	AND (p.description = '" . ONCAMPUS . "' OR (m.description= '" . PHONE . "'))
	AND a.int_type='1'
	AND a.student_number IS NOT NULL
	GROUP BY a.job_id");
/*
$rst_first_interview = $GLOBALS['dbh']->Execute("
	SELECT MIN(TIME_FORMAT(int_time, '%k%i%S') + 0) AS time, job_id AS job_id
	FROM interview_time
	WHERE int_date='$int_date'
	GROUP BY job_id");
*/

$i = 0;
while ($row = $rst_interview->FetchRow())
{
	$interview[$i++] = $row;
}
$num_interviews = $i;

$i = 0;
while ($row = $rst_presentation->FetchRow())
{
	$presentation[$i++] = $row;
}
$num_presentations = $i;

$i = 0;
while ($row = $rst_first_interview->FetchRow())
{
	$temp_result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT phone
		FROM interview_phone
		WHERE time_id='" . $row["time_id"] . "'
		AND phone IS NOT NULL
		");
	if ($temp_row = $temp_result->FetchRow() || ($row["description"] == ONCAMPUS))
	{
		$time = $row["time"];
		if (strlen($time) == 5) { $time = "0" . $time; }
		$first_interview[$i]["time"] = substr($time, 0, 2) . ":" . substr($time, 2, 2) . ":" . substr($time, 4, 2);
		$first_interview[$i]["job_id"] = $row["job_id"];
		$i++;
	}
	$temp_result->Close();
}
$num_first_interview = count($first_interview);

$i = 0;
$j = 0;
$rowclass = 1;
$cur_time_display = (date("Y-m-d") == $int_date) ? true : false;
while ($i < $num_interviews || $j < $num_presentations)
{
	if ($i == $num_interviews) { $case = 1; }
	elseif ($j == $num_presentations) { $case = 0; }
	elseif (strcmp($interview[$i]["time"], $presentation[$j]["time"]) < 0) { $case = 0; }
	else { $case = 1; }

	switch ($case)
	{
		case 0:
			// interview
			$row = $interview[$i++];
			break;

		case 1:
			// presentation
			$row = $presentation[$j++];
			break;
	}

	$new_time = $last_time != $row["time"];
	
	if ($new_time) { $rowclass = ($rowclass) ? 0 : 1; }
	$class = ($rowclass) ? "row0d" : "row1d";
	
	echo("<tr><td class='" . $class . "'>&nbsp;");
	if (date("G:i") <= $row["time"] && $cur_time_display) { echo("<b>"); }
	echo((($new_time) ? formatTime($row["time"]) : ""));
	if (date("G:i") <= $row["time"] && $cur_time_display) { echo("</b>");  $cur_time_display = false; }
	echo("&nbsp;</td>");
	echo("<td class='" . $class . "'>&nbsp;" . $row["location"] . "&nbsp;</td>");
	echo("<td class='" . $class . "'>&nbsp;" . $row["company"] . "&nbsp;</td>");
	echo("<td class='" . $class . "'>&nbsp;" . $row["interviewer"] . "&nbsp;</td>");
	echo("<td class='" . $class . "' align='center'>&nbsp;");

	if (!$case)
	{
		for ($k = 0; $k < $num_first_interview; $k++)
		{
			if (($first_interview[$k]["job_id"] == $row["job_id"]) && ($first_interview[$k]["time"] == $row["time"]))
			{
				echo("<img src='misc/images/green_flag.gif' width='10' height='12' border='0' alt='' />");
				break;
			}
		}
	}
	if (isPhone($row["interview_medium_id"])) { echo("<img src='misc/images/phone.gif' width='18' height='12' border='0' alt='' />"); }
	if ($row["presentation"]) { echo("<img src='misc/images/presentation.gif' width='16' height='15' border='0' alt='' />"); }
	echo("&nbsp;</td></tr>\n");

	$last_time = $row["time"];
}

?>

	</table>
</td></tr></table>
