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
 | Filename: view_interview.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: Show an interview schedule (as opposed to view all interview    |
 | data) Notes: $job_code or $job_id must  be set in the file that includes     |
 | this file.                                                                   |
 +------------------------------------------------------------------------------+

*/

if ($job_id == "" || !isValidJobID($job_id)){
	error("Invalid Job");
	include ("interview/view/employer/view_main.inc");
}else{

	$sql = ("
		SELECT c.job_code, c.department_id, b.company_name, d.description as medium, a.notes
		FROM interview AS a, 
			employer_company AS b,
			job_info AS c,
			interview_medium AS d,
			contact_job as e
		WHERE a.job_id='$job_id' AND d.interview_medium_id = a.interview_medium_id 
			AND c.job_id = a.job_id AND c.employer_id=b.employer_id AND c.job_id = e.job_id and e.contact_id = '" . $auth->contact_id . "'");
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($row = $result->FetchRow())
	{
	  	$notes = $row["notes"];
		?>
		<h3>Interview Schedule: <?php echo $row["job_code"]?></h3>
		<hr width="100%" />
		<table>
			<tr><td>Company:&nbsp;</td>
				<td><?php echo($row["company_name"]); ?></td></tr>
			<tr><td>Job Code:&nbsp;</td>
				<td><?php echo($row["job_code"]); ?></td></tr>
            <tr><td><b>Host Department:&nbsp;</b></td>
                <td><?php echo(getDepartmentName($row["department_id"])); ?></td></tr>
			<tr><td>Medium:&nbsp;</td>
				<td><?php echo($row["medium"]); ?></td></tr>
	<?php
		echo("<tr><td valign='top'>Notes:</td><td>" . (($notes) ? $notes : "None") . "</td></tr>");
	
		// Presentation Info
		$result = $GLOBALS['dbh']->Execute("SELECT * FROM presentation WHERE job_id='$job_id'");
		if ($row = $result->FetchRow())
		{
			echo("<tr><td>&nbsp;</td></tr>");
			echo("<tr><td valign='top'>Presentation:&nbsp;</td><td>" . formatLongDate($row["pres_date"]) . " at " . formatTime($row["pres_time"]) . "</td></tr>");
			echo("<tr><td valign='top'>Location:&nbsp;</td><td>" . $row["pres_loc"] . "</td></tr>");
			echo("<tr><td valign='top'>Notes:&nbsp;</td><td>" . (($row["pres_notes"]) ? $row["pres_notes"] : "None") . "</td></tr>");
		}
		echo("</table>");
	?>
		<br />
		<h4>Interview Schedule</h4>
	<?php





	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT a.int_date, a.interviewer_id, b.interviewer_name
		FROM interview_time AS a, interview_interviewer AS b
		WHERE a.job_id='" . addslashes($job_id) . "' AND a.interviewer_id=b.interviewer_id
		ORDER BY a.int_date, b.interviewer_name");

	while ($row = $result->FetchRow())
	{

		?>
		<p>
		<table>
			<tr><td>Interviewer:</td><td><?php echo($row["interviewer_name"]); ?></td></tr>
			<tr><td>Date:</td><td><?php echo(formatLongDate($row["int_date"])); ?></td></tr>
		</table>
		<?php
		
		//emily feb 7
		//ELT(a.int_type, 'interview', 'break', 'lunch') does not seem to work in this query therefore
		//I have changed it to "IF (a.int_type =1, '....)"
		//There are other problems with ELT noted in other files
		
		$result_times = $GLOBALS['dbh']->Execute ("
			SELECT 	a.job_id, a.time_id, a.int_time, b.student_number,
				IF(a.int_type=1, 'interview', IF(a.int_type=2, 'break', 'lunch')) AS int_type,
				CONCAT(b.first_name, ' ', b.last_name) AS student_name,
				CONCAT(c2.building_code, ' ', c.room_number) AS int_room,
					CONCAT(d2.building_code, ' ', d.room_number) AS wait_room,
					e.phone AS phone_number
			FROM	interview_time AS a
			LEFT OUTER JOIN	student AS b
			ON		a.student_number=b.student_number
			LEFT OUTER JOIN room AS c
			ON	a.int_room_id=c.room_id
			LEFT OUTER JOIN building AS c2
			ON c.building_id=c2.building_id
			LEFT OUTER JOIN room AS d
			ON	a.int_wait_room_id = d.room_id
			LEFT OUTER JOIN building AS d2
			ON d.building_id=d2.building_id
			LEFT OUTER JOIN interview_phone AS e
			ON	a.time_id=e.time_id
			WHERE	a.int_date='" . $row["int_date"] . "' AND
					a.interviewer_id='" . $row["interviewer_id"] . "' AND
					a.job_id='" . addslashes($job_id) . "'
			ORDER BY a.int_time");
		
?>
			<table cellspacing="0" cellpadding="0" border="1"><tr><td>
				<table border="0" cellpadding="2">
					<tr>
						<td class="rowgrey" align='center'>&nbsp;<b class="white">Interview Time</b>&nbsp;</td>
						<td class="rowgrey" align='center'>&nbsp;<b class="white">Student</b>&nbsp;</td>
						<td class="rowgrey" align='center'>&nbsp;<b class="white">Room or Phone</b>&nbsp;</td>
					</tr>
			<?php
			while ($row_times = $result_times->FetchRow())
			{
				switch($row_times["int_type"])
				{
					case "interview":
						if (!$int_room = $row_times["int_room"]) { $int_room = "TBA"; }
							if (!$wait_room = $row_times["wait_room"]) { $wait_room = "TBA"; }
						echo("<tr>");
						echo("<td class='row0d' align='center'>" . formatTime($row_times["int_time"]) . "</td>");
						echo("<td class='row0d'>&nbsp;" . $row_times["student_name"] . "&nbsp;</td>");
	
						if ($row_times["phone_number"])
						{
							echo("<td class='row0d'>&nbsp;" . $row_times["phone_number"] . "&nbsp;</td>");
						}
						elseif ($int_room == "TBA")
						{
							echo("<td class='row0d'>&nbsp;" . $int_room . "&nbsp;</td>");
						}
						else
						{
							echo("<td class='row0d'>&nbsp;" . $int_room . " / " . $wait_room . "&nbsp;</td>");
						}
						echo("</tr>");
						break;
						
					case "break":
						echo("<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "</td>");
						echo("<td class='row1d'>&nbsp;Break&nbsp;</td>");
						echo("<td class='row1d'>&nbsp;</td></tr>");
						break;
					
					case "lunch":
						echo("<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "</td>");
						echo("<td class='row1d'>&nbsp;Lunch&nbsp;</td>");
						echo("<td class='row1d'>&nbsp;</td></tr>");
						break;
				}
			}
		?>
				</table>
			</td></tr></table>
		</p>
		<?php
	
		}
	}else{
		error ("Invalid Job");
		include ("interview/view/employer/view_main.inc");
	
	}
}
?>

