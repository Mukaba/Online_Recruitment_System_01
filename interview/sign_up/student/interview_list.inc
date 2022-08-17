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
 | Filename: interview_list.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: show the interviews the student has and link to the sign up     |
 | schedules.  if the student has no future interviews, tell them               |
 |                                                                              |
 |                                                                              |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
$interview_list = new interview_list($student_number);
$interview_list->display();


class interview_list
{
	var $error_output;
	var $notify_output;
	var $output = "";

	var $student_number;
		
	function interview_list($student_number)
	{
		$this->student_number = $student_number;
	}

	function display()	
	{
		echo("<h3>Sign Up for an Interview</h3>");
		$this->generate();
		
		if ($this->error_output) { echo(error("<h1 class='black'>Error</h1>" . $this->error_output)); }
		if ($this->notify_output) { echo(notify($this->notify_output)); }
		//echo($this->output);
	}
		
	function generate()
	{
		$signed_up = $this->signed_up($this->student_number);
		$not_signed_up = $this->not_signed_up($this->student_number);
		$shortlisted = $this->shortlisted($this->student_number);
		if (!$shortlisted && !$not_signed_up && !$signed_up) { echo("<h4>Current Interviews:</h4>");notify("You do not have any interviews at this time."); }
		
		if ($shortlisted) { $this->display_today($shortlisted); }
		if ($not_signed_up) { $this->display_please_sign_up($not_signed_up); }
		if ($signed_up) { $this->display_already_signed_up($signed_up); }
		$this->display_presentations();
		if ($shortlisted) { $this->display_past($shortlisted); }
	}

	function get_last_visited( $job_id )
	{	
		$result = $GLOBALS['dbh']->Execute(" SELECT 	last_visited
					FROM   	students_shortlisted
					WHERE 	student_number = '$this->student_number' AND
						job_id = '$job_id'");
		$row = $result->FetchRow();		
		return ereg_replace( "[: -]", "", $row['last_visited']);										
	}	
	
	function get_status( $job_id, $date_updated )
	{
		$last_visited = $this->get_last_visited( $job_id );
		if( !$last_visited )
			return  "New";
		
		elseif( $date_updated < $last_visited )
			return "--";
			
		else
			return "<font style='color:red;'>Updated</font>";
	}

		
	function display_today($shortlisted)
	{
		global $PHP_SELF;
		
		if (!$shortlisted) { return false; }

		$sql = "
			SELECT DISTINCT ec.company_name, j.job_id, j.job_code, a.int_date
			FROM	employer_company as ec, job_info AS j, interview_time as a
			WHERE j.job_id IN (" . $shortlisted . ") AND j.job_id = a.job_id and a.int_date = CURRENT_DATE
                        and ec.employer_id=j.employer_id
			ORDER BY job_code";
	
		$result = $GLOBALS['dbh']->Execute($sql);
		$i = 0;
		while ($row = $result->FetchRow())
		{
			
			$result2 = $GLOBALS['dbh']->Execute("
					SELECT MAX(date_edited) as date_updated
					FROM interview_edit
					WHERE job_id = '" . $row["job_id"] . "'");
			$edit_row = $result2->FetchRow();

			$href = $PHP_SELF . "&amp;select=student_sign_up&amp;job_id=" . $row["job_id"];
		
			$rows[$i]["date"] = "<a class='blue' href='$href'>" . formatStandardDate($row["int_date"]) . "</a>";
			$rows[$i]["job_code"] = "<a class='blue' href='$href'>" . $row["job_code"] . (($row["cancelled"]) ? "<span style='color: #FF0000;'> (Cancelled)</span>" : "") . "</a>";
			$rows[$i]["status"] = $this->get_status( $row['job_id'], $edit_row["date_updated"]);
			$rows[$i]["company_name"] = "<a class=\"blue\" href=\"$href\">".$row["company_name"]."</a>";
			$i++;
		}
		
		if ($rows) { $this->display_link_table("Today:", $rows); }
	}

	
	function display_please_sign_up($not_signed_up)
	{
		global $PHP_SELF;
		
		if (!$not_signed_up) { return false; }

		$sql = "
			SELECT DISTINCT ec.company_name, j.job_id, j.job_code, min(b.int_date) as int_date
			FROM	employer_company as ec, job_info AS j, interview_time AS b
			WHERE j.job_id=b.job_id  
				AND (j.job_id IN (" . $not_signed_up . "))
				and ec.employer_id=j.employer_id
			GROUP BY j.job_id
			HAVING MAX(b.int_date) > CURRENT_DATE()
			ORDER BY int_date";
		$result = $GLOBALS['dbh']->Execute($sql);
		$i = 0;
		while ($row = $result->FetchRow())
		{
			$result2 = $GLOBALS['dbh']->Execute("
					SELECT MAX(date_edited) as date_updated
					FROM interview_edit
					WHERE job_id = '" . $row["job_id"] . "'");
			$edit_row =  $result2->FetchRow();
			
			$result3 = $GLOBALS['dbh']->Execute("
					SELECT DISTINCT int_date
					FROM interview_time
					WHERE job_id = '" . $row["job_id"] . "'
					ORDER BY int_date");
			$href = $PHP_SELF . "&amp;select=student_sign_up&amp;job_id=" . $row["job_id"];
			
			$date_str = "";
			while($time_row =  $result3->FetchRow()){
				$date_str .= "<a class='blue' href='$href'>" . formatStandardDate($time_row["int_date"]) . "</a><br />&nbsp;";
			}

			$rows[$i]["date"] = substr($date_str, 0, strlen($date_str) - 12);
			//"<a class='blue' href='$href'>" . formatStandardDate($row["int_date"]) . "</a>";
			$rows[$i]["job_code"] = "<a class='blue' href='$href'>" . $row["job_code"] . (($row["cancelled"]) ? "<span style='color: #FF0000;'> (Cancelled)</span>" : "") . "</a>";
			$rows[$i]["status"] = $this->get_status( $row['job_id'], $edit_row['date_updated'] );
			$rows[$i]["company_name"] = "<a class=\"blue\" href=\"$href\">".$row["company_name"]."</a>";
			$i++;
		}

		if ($rows) { $this->display_link_table("Please Sign Up:", $rows); }
	}

	
	function display_already_signed_up($signed_up)
	{
		global $PHP_SELF;
		
		if (!$signed_up) { return false; }

		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT a.job_id, a.job_code, b.int_date, ec.company_name
			FROM	job_info AS a, interview_time AS b, employer_company as ec
			WHERE a.job_id=b.job_id AND b.student_number = '" . $this->student_number . "' AND  
					(a.job_id IN (" . $signed_up . ")) and ec.employer_id=a.employer_id
			ORDER BY	b.int_date");
		$i = 0;
		while ($row = $result->FetchRow())
		{
			$result2 = $GLOBALS['dbh']->Execute("
					SELECT MAX(date_edited) AS date_updated
					FROM interview_edit
					WHERE job_id = '" . $row["job_id"] . "'");
			$edit_row = $result2->FetchRow();
			
			$href = $PHP_SELF . "&amp;select=student_sign_up&amp;job_id=" . $row["job_id"];
			
			$rows[$i]["date"] = "<a class='blue' href='$href'>" . formatStandardDate($row["int_date"]) . "</a>";
			$rows[$i]["job_code"] = "<a class='blue' href='$href'>" . $row["job_code"] . (($row["cancelled"]) ? "<span style='color: #FF0000;'> (Cancelled)</span>" : "") . "</a>";
			$rows[$i]["status"] = $this->get_status( $row['job_id'], $edit_row['date_updated'] );
			$rows[$i]["company_name"] = "<a class=\"blue\" href=\"$href\">".$row["company_name"]."</a>";
			
			$i++;
		}

		if ($rows) { $this->display_link_table("Signed Up:", $rows); }
	}

	
	function display_past($shortlisted)
	{
		global $PHP_SELF;
		
		if (!$shortlisted) { return false; }

		$date_sort = array();
		$date_show = array();
		$job_codes = array();
		$company_names = array ();
		
		$sql = "SELECT DISTINCT a.job_id, a.job_code,  b.int_date, c.cancelled, ec.company_name
			FROM job_info AS a, interview_time AS b, interview AS c, employer_company as ec
			WHERE a.job_id=b.job_id AND a.job_id=c.job_id and ec.employer_id=a.employer_id
				AND ((b.int_date < CURRENT_DATE) OR c.cancelled) AND
				(a.job_id IN (" . $shortlisted . ")) 
				AND b.student_number = '" . $this->student_number . "'";
		$result = $GLOBALS['dbh']->Execute($sql);
		$signed_up = "";
		$i = 0;
		while( $row =  $result->FetchRow()){
			$href = $PHP_SELF . "&amp;select=student_sign_up&amp;job_id=" . $row["job_id"];
			$signed_up .= $row["job_id"] . ",";
			$date_sort[$i] = $row["int_date"];
			$date_str = "<a class='blue' href='$href'>" . formatStandardDate($row["int_date"]) . "</a><br />&nbsp;";
			$date_show[$i] = substr($date_str, 0, strlen($date_str) - 12);
			$job_codes[$i] = "<a class='blue' href='$href'>" . $row["job_code"] . (($row["cancelled"]) ? "<span style='color: #FF0000;'> (Cancelled)</span>" : "") . "</a>";
			$company_names[$i] = "<a class=\"blue\" href=\"$href\">".$row["company_name"]."</a>";
			$i++;
		}
		$signed_up = substr($signed_up, 0, strlen($signed_up) -1);
		
		$sql = "SELECT DISTINCT ec.company_name, a.job_id, a.job_code, c.cancelled, MAX(b.int_date) as MAX_INT_DATE
			FROM job_info AS a, interview_time AS b, interview AS c, employer_company as ec
			WHERE a.job_id=b.job_id AND a.job_id=c.job_id and ec.employer_id=a.employer_id
				AND (a.job_id IN (" . $shortlisted . ")) ";
		if (strlen($signed_up) > 0){
			$sql .= "AND (a.job_id NOT IN (" . $signed_up . "))";
		}
		$sql .= "GROUP BY a.job_id
			HAVING (MAX_INT_DATE < CURRENT_DATE OR c.cancelled)";
		$result = $GLOBALS['dbh']->Execute($sql);	
		while ($row = $result->FetchRow())
		{
			$href = $PHP_SELF . "&amp;select=student_sign_up&amp;job_id=" . $row["job_id"];
			
			$result3 = $GLOBALS['dbh']->Execute("
					SELECT DISTINCT int_date
					FROM interview_time
					WHERE job_id = '" . $row["job_id"] . "'
					ORDER BY int_date DESC");
			$date_str = "";
			$max = 0;
			while($date_row  = $result3->FetchRow()){
				if($date_row["int_date"] > $max){
					$max = $date_row["int_date"];
				}
				$date_str .= "<a class='blue' href='$href'>" . formatStandardDate($date_row["int_date"]) . "</a><br />&nbsp;";
			}
			$date_sort[$i] = $max;
			
			$date_show[$i] = substr($date_str, 0, strlen($date_str) - 12);
			$job_codes[$i] = "<a class='blue' href='$href'>" . $row["job_code"] . (($row["cancelled"]) ? "<span style='color: #FF0000;'> (Cancelled)</span>" : "") . "</a>";
			$company_names[$i] = "<a class=\"blue\" href=\"$href\">".$row["company_name"]."</a>";
			$i++;
		}
		
		array_multisort($date_sort, SORT_DESC, $job_codes, SORT_ASC, SORT_STRING, $date_show, $company_names);
		for ($i = 0; $i < count($date_sort); $i++){
			$rows[$i]["date"] = $date_show[$i];
			$rows[$i]["job_code"] = $job_codes[$i];
			$rows[$i]["company_name"] = $company_names[$i];
		}
		if ($rows) { $this->display_link_table("Past &amp; Cancelled:", $rows); }
	}
	
	
	function display_link_table($heading, $rows)
	{		
		echo("<h4>" . $heading . "</h4>");
		echo("<table cellspacing='0' cellpadding='0' border='1'><tr><td>");
		echo("	<table border='0' cellpadding='2'>");
		echo("		<tr>");
		echo("			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>");
		echo("			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>");
		echo("			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Date</b>&nbsp;</td>");
		echo("			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
		echo("		</tr>");

		$num_rows = sizeof($rows);
		$rowclass = 0;
		for ($i = 0; $i < $num_rows; $i++)
		{
			echo("		<tr>");
			echo("			<td class='" . (($rowclass) ? "row0d" : "row1d") . "'>&nbsp;" . $rows[$i]["job_code"] . "&nbsp;</td>");
			echo("			<td class='" . (($rowclass) ? "row0d" : "row1d") . "'>&nbsp;" . $rows[$i]["company_name"] . "&nbsp;</td>");
			echo("			<td class='" . (($rowclass) ? "row0d" : "row1d") . "'>&nbsp;" . $rows[$i]["date"] . "&nbsp;</td>");
			echo("			<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;" . $rows[$i]["status"] . "&nbsp;</td>");
			echo("		</tr>");
			$rowclass = ($rowclass) ? 0 : 1;
		}
		
		echo("	</table>");
		echo("</td></tr></table>");
	}


	function shortlisted($student_number)
	{
		// not signed up && within the last two terms
			
		//  The old sql statement to do this.  Didn't work properly.  This doesn't give last two terms, it gives the last term and the currrent term. 
		//  New sql written by Chris

		//$sql = ("
		//	SELECT	a.job_id
		//	FROM	students_shortlisted AS a, interview AS b, term as t, job_info as j
		//	WHERE	a.student_number='" . addslashes($student_number) . "' AND a.job_id=b.job_id AND b.student_visible
		//	AND a.job_id = j.job_id 
		//	AND t.term_id = j.interview_term_id 
		//	AND (DATE_FORMAT(DATE_ADD(CURRENT_DATE(), INTERVAL -4 MONTH), '%m') BETWEEN t.start_date AND t.end_date 
		//	OR 
		//	MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date ) AND (j.interview_year = YEAR(CURRENT_DATE()) 
		//	OR j.interview_year = DATE_FORMAT(DATE_ADD(CURRENT_DATE(), INTERVAL -4 MONTH), '%Y')) 
		//	");

		$sql = ("
			SELECT	a.job_id
			FROM	students_shortlisted AS a, interview AS b, term as t, job_info as j
			WHERE	a.student_number='" . addslashes($student_number) . "' AND a.job_id=b.job_id AND b.student_visible
			AND a.job_id = j.job_id 
			AND t.term_id = j.interview_term_id 
			AND ((((DATE_FORMAT(DATE_ADD(CURRENT_DATE(), INTERVAL -4 MONTH), '%m')  >= t.start_date AND DATE_FORMAT(DATE_ADD(CURRENT_DATE(), INTERVAL -4 MONTH), '%m') <= t.end_date)) 
			AND j.interview_year = DATE_FORMAT(DATE_ADD(CURRENT_DATE(), INTERVAL -4 MONTH), '%Y')) OR  
			(((DATE_FORMAT(DATE_ADD(CURRENT_DATE(), INTERVAL -8 MONTH),'%m') >= t.start_date AND DATE_FORMAT(DATE_ADD(CURRENT_DATE(),INTERVAL -8 MONTH),'%m') <= t.end_date)   AND (j.interview_year = DATE_FORMAT(DATE_ADD(CURRENT_DATE(), INTERVAL -8 MONTH),'%Y'))))
			OR (MONTH(CURRENT_DATE()) >= t.start_date AND MONTH(CURRENT_DATE()) <= t.end_date and YEAR(CURRENT_DATE())=j.interview_year)) 
			");

		$result = $GLOBALS['dbh']->Execute($sql);

		if (!$result) { return false; }
		$shortlisted = false;
		while ($row = $result->FetchRow())
		{
			$shortlisted .= $row["job_id"] . ",";
		}
		$result->Close();
		
		if ($shortlisted) { $shortlisted = substr($shortlisted, 0, (strlen($shortlisted) - 1)); }
		return $shortlisted;
	}

	function signed_up($student_number)
	{
		// not signed up
		$result = $GLOBALS['dbh']->Execute("
			SELECT	a.job_id
			FROM		students_shortlisted AS a, interview AS c
			LEFT OUTER JOIN interview_time AS b
			ON			a.job_id=b.job_id AND b.student_number='" . addslashes($student_number) . "' AND NOT b.switch
			WHERE	a.student_number='" . addslashes($student_number) . "' AND a.job_id=c.job_id  AND b.int_date > CURRENT_DATE() 
				AND NOT c.cancelled AND c.student_visible AND NOT ISNULL(b.int_date)");
		if (!$result) { return false; }
		$signed_up = false;
		while ($row = $result->FetchRow())
		{
			$signed_up .= $row["job_id"] . ",";
		}
		$result->Close();
		
		if ($signed_up) { $signed_up = substr($signed_up, 0, (strlen($signed_up) - 1)); }
		return $signed_up;
	}

	function not_signed_up($student_number)
	{
		// not signed up
		$sql = ("
			SELECT a.job_id, MAX(z.int_date)
			FROM students_shortlisted AS a, interview AS c, interview_time AS z
			LEFT OUTER JOIN interview_time AS b ON a.job_id=b.job_id AND b.student_number='" . addslashes($student_number) . "' AND NOT b.switch
			WHERE	a.student_number='" . addslashes($student_number) . "' 
			AND a.job_id=c.job_id 
			AND z.job_id=a.job_id
			AND (b.int_date > CURRENT_DATE() OR IF(ISNULL(b.int_date), (z.int_date>=CURRENT_DATE()), 0))
			AND NOT c.cancelled 
			AND c.student_visible 
			AND IF(ISNULL(b.int_date), 1, 0)
			GROUP BY a.job_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if (!$result) { return false; }
		$not_signed_up = false;
		while ($row = $result->FetchRow())
		{
			$not_signed_up .= $row["job_id"] . ",";
		}
		$result->Close();
		
		if ($not_signed_up) { $not_signed_up = substr($not_signed_up, 0, (strlen($not_signed_up) - 1)); }
		return $not_signed_up;
	}
	
	function error($error_text)
	{
		$this->error_output .= "<p>" . $error_text . "</p>";
	}

	function notify($notify_text)
	{
		$this->notify_output .= "<p>" . $notify_text . "</p>";
	}

	function display_presentations()
	{
		global $student_number;
		global $auth;

		$sql = ("
			SELECT DISTINCT a.department_id
			FROM department_group AS a, department_group AS b
			WHERE a.group_id=b.group_id
			AND b.department_id='" . $auth->department . "'
			");
		$group_result = $GLOBALS['dbh']->Execute($sql);

		if ($group_result->RecordCount())
		{
			while($group_row = $group_result->FetchRow())
                        {
                                $department_ids .= ("'" . $group_row["department_id"] . "',");
                        }       
                                
                        $department_ids = substr($department_ids, 0, (strlen($department_ids) - 1));
                }               
                else    
                {       
                        $department_ids = ("'" . $auth->department . "'");
                }
			
			
		$sql = "SELECT DISTINCT e.company_name, p.job_id, p.pres_date, j.job_code,
			p.pres_time, p.pres_loc, p.pres_notes, p.end_time
			FROM presentation as p, job_info as j, employer_company as e, students_shortlisted AS s, interview as i
			WHERE j.job_id = p.job_id 
			AND j.employer_id = e.employer_id 
			AND pres_date >= CURRENT_DATE()
			AND i.job_id = p.job_id
			AND i.student_visible
			AND (
			(p.viewable_by_all AND j.department_id IN (" . $department_ids . "))
			OR
			(s.student_number='" . addslashes($student_number) . "' AND p.job_id=s.job_id)
			)
			";

		$result = $GLOBALS['dbh']->Execute( $sql );

			echo("<h4>Upcoming Company Presentations:</h4>");
		if ($result->RecordCount() > 0){	
			
			echo("<table border=\"1\">
					  <tr>
					    <td align=\"center\" class=\"rowgrey\"><b class=\"white\">Company</b></td>
					    <td align=\"center\" class=\"rowgrey\"><b class=\"white\">Job Code</b></td>
					    <td align=\"center\" class=\"rowgrey\"><b class=\"white\">Date</b></td>
					    <td align=\"center\" class=\"rowgrey\"><b class=\"white\">Time</b></td>
					    <td align=\"center\" class=\"rowgrey\"><b class=\"white\">Location</b></td>
					  </tr>");
			$i = 0;
			while($row =  $result->FetchRow()){
				$shortlisted = isStudentShortlisted($row["job_id"], $this->student_number);
				(($i % 2 == 0) ? $class="row0d" : $class="row1d");
				$i++;
				if( $shortlisted )
				{
					$notes = "<font color=\"red\">You have an interview with this company.  </font>";
					$notes .= "<br />" . $row["pres_notes"];
				}else{
					$notes = $row["pres_notes"] . " ";
				}
				echo("<tr>
						    <td rowspan=\"2\" class=\"$class\">" . $row["company_name"] . " </td>
						    <td class=\"$class\">" . $row["job_code"] . "</td>
						    <td class=\"$class\">" . formatStandardDate($row["pres_date"]) . " </td>
							    <td class=\"$class\">" . formatTime($row["pres_time"]) . " - " . formatTime($row["end_time"]) . " </td>
						    <td class=\"$class\">" . $row["pres_loc"] . " </td>
						  </tr>
						  <tr>
						    <td class=\"$class\" colspan=\"4\">" . $notes . "&nbsp;</td>
						  </tr>");
			}
			echo("</table>");
		}else{
			notify("There are no upcoming company presentations.");
		}
	}
}


?>
