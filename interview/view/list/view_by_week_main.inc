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
 | Filename: view_by_week_main.inc                                              |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/
$sql = "SELECT DATE_FORMAT('" . $start_date . "', '%W %M %d, %Y') as start,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 6 DAY), '%W %M %d, %Y') as end,
		DATE_FORMAT('" . $start_date . "', '%M %d, %Y') as start2,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 6 DAY), '%M %d, %Y') as end2,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 7 DAY), '%M %d, %Y') as back1start,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 1 DAY), '%M %d, %Y') as back1end,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 14 DAY), '%M %d, %Y') as back2start,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 8 DAY), '%M %d, %Y') as back2end,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 21 DAY), '%M %d, %Y') as back3start,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 15 DAY), '%M %d, %Y') as back3end,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 28 DAY), '%M %d, %Y') as back4start,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 22 DAY), '%M %d, %Y') as back4end,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 35 DAY), '%M %d, %Y') as back5start,
		DATE_FORMAT(DATE_SUB('" . $start_date . "', INTERVAL 29 DAY), '%M %d, %Y') as back5end,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 7 DAY), '%M %d, %Y') as for1start,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 13 DAY), '%M %d, %Y') as for1end,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 14 DAY), '%M %d, %Y') as for2start,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 20 DAY), '%M %d, %Y') as for2end,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 21 DAY), '%M %d, %Y') as for3start,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 27 DAY), '%M %d, %Y') as for3end,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 28 DAY), '%M %d, %Y') as for4start,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 34 DAY), '%M %d, %Y') as for4end,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 35 DAY), '%M %d, %Y') as for5start,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 41 DAY), '%M %d, %Y') as for5end
	";
$row = $GLOBALS['dbh']->GetRow($sql);
$start_date_format = $row["start"];
$end_date_format = $row["end"];
$start_date_format2 = $row["start2"];
$end_date_format2 = $row["end2"];
$day=array(array(), array(), array(), array(), array(), array(), array());

// Get Weeks Used in the Quick Jump Select
$backweek[] = $row["back1start"] . " - " . $row["back1end"];
$backweek[] = $row["back2start"] . " - " . $row["back2end"]; 
$backweek[] = $row["back3start"] . " - " . $row["back3end"]; 
$backweek[] = $row["back4start"] . " - " . $row["back4end"]; 
$backweek[] = $row["back5start"] . " - " . $row["back5end"]; 
$forweek[] = $row["for1start"] . " - " . $row["for1end"];
$forweek[] = $row["for2start"] . " - " . $row["for2end"]; 
$forweek[] = $row["for3start"] . " - " . $row["for3end"]; 
$forweek[] = $row["for4start"] . " - " . $row["for4end"]; 
$forweek[] = $row["for5start"] . " - " . $row["for5end"]; 


/*
 Set up each day of the given week with the job_id's of the interview's that occur on that day.
*/


$sql = ("
	SELECT DISTINCT(i.job_id) as job_id, MIN(i.int_time) as int_time
	FROM interview_time as i, interview AS z, students_shortlisted as ss, student_department as sd
	LEFT JOIN job_info as j ON j.job_id = i.job_id
	WHERE i.int_date = '" . addslashes($start_date) . "' 
	AND z.job_id=i.job_id
	AND NOT(z.cancelled)
	AND ss.student_number=sd.student_number AND i.job_id=ss.job_id
	AND (j.department_id IN ('" . implode("', '", $departments_in_group) . "') OR sd.department_id='".$auth->department."')
	GROUP BY job_id 
	ORDER BY int_time
	");

$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow()){$day[0][] = $row["job_id"];}

$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT(i.job_id) as job_id, MIN(i.int_time) as int_time 
	FROM interview_time as i, interview AS z, students_shortlisted as ss, student_department as sd
	LEFT JOIN job_info as j ON j.job_id = i.job_id 
	WHERE i.int_date = DATE_ADD('" . $start_date . "', INTERVAL 1 DAY) 
	AND z.job_id=i.job_id
	AND NOT(z.cancelled)
	AND ss.student_number=sd.student_number AND i.job_id=ss.job_id
	AND (j.department_id IN ('" . implode("', '", $departments_in_group) . "') OR sd.department_id='".$auth->department."')
	GROUP BY job_id 
	ORDER BY int_time");
while ($row = $result->FetchRow()){ $day[1][] = $row["job_id"]; }

$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT(i.job_id) as job_id, MIN(i.int_time) as int_time 
	FROM interview_time as i, interview AS z, students_shortlisted as ss, student_department as sd
	LEFT JOIN job_info as j ON j.job_id = i.job_id 
	WHERE i.int_date = DATE_ADD('" . $start_date . "', INTERVAL 2 DAY) 
	AND z.job_id=i.job_id
	AND NOT(z.cancelled)
	AND ss.student_number=sd.student_number AND i.job_id=ss.job_id
	AND (j.department_id IN ('" . implode("', '", $departments_in_group) . "') OR sd.department_id='".$auth->department."')
	GROUP BY job_id 
	ORDER BY int_time");
while ($row = $result->FetchRow()){ $day[2][] = $row["job_id"]; }

$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT(i.job_id) as job_id, MIN(i.int_time) as int_time 
	FROM interview_time as i, interview AS z, students_shortlisted as ss, student_department as sd
	LEFT JOIN job_info as j ON j.job_id = i.job_id 
	WHERE i.int_date = DATE_ADD('" . $start_date . "', INTERVAL 3 DAY) 
	AND z.job_id=i.job_id
	AND NOT(z.cancelled)
	AND ss.student_number=sd.student_number AND i.job_id=ss.job_id
	AND (j.department_id IN ('" . implode("', '", $departments_in_group) . "') OR sd.department_id='".$auth->department."')
	GROUP BY job_id 
	ORDER BY int_time");
while ($row = $result->FetchRow()){ $day[3][] = $row["job_id"]; }

$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT(i.job_id) as job_id, MIN(i.int_time) as int_time 
	FROM interview_time as i, interview AS z, students_shortlisted as ss, student_department as sd
	LEFT JOIN job_info as j ON j.job_id = i.job_id 
	WHERE i.int_date = DATE_ADD('" . $start_date . "', INTERVAL 4 DAY) 
	AND z.job_id=i.job_id
	AND NOT(z.cancelled)
	AND ss.student_number=sd.student_number AND i.job_id=ss.job_id
	AND (j.department_id IN ('" . implode("', '", $departments_in_group) . "') OR sd.department_id='".$auth->department."')
	GROUP BY job_id 
	ORDER BY int_time");
while ($row = $result->FetchRow()){ $day[4][] = $row["job_id"]; }

$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT(i.job_id) as job_id, MIN(i.int_time) as int_time 
	FROM interview_time as i, interview AS z, students_shortlisted as ss, student_department as sd
	LEFT JOIN job_info as j ON j.job_id = i.job_id 
	WHERE i.int_date = DATE_ADD('" . $start_date . "', INTERVAL 5 DAY) 
	AND z.job_id=i.job_id
	AND NOT(z.cancelled)
	AND ss.student_number=sd.student_number AND i.job_id=ss.job_id
	AND (j.department_id IN ('" . implode("', '", $departments_in_group) . "') OR sd.department_id='".$auth->department."')
	GROUP BY job_id 
	ORDER BY int_time");
while ($row = $result->FetchRow()){ $day[5][] = $row["job_id"]; }

$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT(i.job_id) as job_id, MIN(i.int_time) as int_time 
	FROM interview_time as i, interview AS z, students_shortlisted as ss, student_department as sd
	LEFT JOIN job_info as j ON j.job_id = i.job_id 
	WHERE i.int_date = DATE_ADD('" . $start_date . "', INTERVAL 6 DAY) 
	AND z.job_id=i.job_id
	AND NOT(z.cancelled)
	AND ss.student_number=sd.student_number AND i.job_id=ss.job_id
	AND (j.department_id IN ('" . implode("', '", $departments_in_group) . "') OR sd.department_id='".$auth->department."')
	GROUP BY job_id 
	ORDER BY int_time");
while ($row = $result->FetchRow()){ $day[6][] = $row["job_id"]; }


$sql = "SELECT DATE_FORMAT('" . $start_date . "', '<b class=\'white\'>%W</b><hr /><b class=\'white\'>%b %d</b>') as day1,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 1 DAY), '<b class=\'white\'>%W</b><hr /><b class=\'white\'>%b %d</b>') as day2,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 2 DAY), '<b class=\'white\'>%W</b><hr /><b class=\'white\'>%b %d</b>') as day3,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 3 DAY), '<b class=\'white\'>%W</b><hr /><b class=\'white\'>%b %d</b>') as day4,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 4 DAY), '<b class=\'white\'>%W</b><hr /><b class=\'white\'>%b %d</b>') as day5,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 5 DAY), '<b class=\'white\'>%W</b><hr /><b class=\'white\'>%b %d</b>') as day6,
		DATE_FORMAT(DATE_ADD('" . $start_date . "', INTERVAL 6 DAY), '<b class=\'white\'>%W</b><hr /><b class=\'white\'>%b %d</b>') as day7";
$week_day_row= $GLOBALS['dbh']->GetRow($sql);

$day_count = array(count($day[0]), count($day[1]), count($day[2]), count($day[3]), count($day[4]), count($day[5]), count($day[6]));
?>
<form action='<?php echo $PHP_SELF?>' method='post' name='viewbyweek'>
<input type='hidden' name='select' value='view' />
<input type='hidden' name='level1' value='week' />
<input type='hidden' name='start_date' value='<?php echo $start_date?>' />
<table border='0' cellpadding='0' cellspacing='0'>
  <tr>
    <td>
	<table class='row1' border='0' cellpadding='4' cellspacing='0' width='100%'>
	  <tr>
	    <td align='left'><input type='submit' value='&nbsp;&lt;&lt;&nbsp;' name='BackWeek' /><input type='submit' value='&nbsp;&lt;&nbsp;' name='Back' /></td>
	    <td rowspan='2' align='center'>
		<?php   // Select Case that allows users to jump to specific weeks.. 
			echo "<select name='JumpWeeks' onchange='document.viewbyweek.submit()'>";
				for ($i = sizeof($forweek) - 1; $i>= 0; $i--) {
					echo "<option value='".(($i+1) * 7)."'>".$forweek[$i]."</option>\n";
				}
            echo "<option selected='selected'>$start_date_format2 - $end_date_format2</option>"; 
                for ($i = 0; $i <= sizeof($backweek) - 1; $i++) {
                    echo "<option value='".(($i+1) * -7)."'>".$backweek[$i]."</option>\n";
                }
			echo "</select>";
		?>
		<input type='submit' value='&nbsp;Today&nbsp;' name='Today' /></td>
	    <td align='right'><input type='submit' value='&nbsp;&gt;&nbsp;' name='Forward' /><input type='submit' value='&nbsp;&gt;&gt;&nbsp;' name='ForwardWeek' /></td>
	  </tr>
	</table>
    </td>
  </tr>
  <tr>
    <td><br />
	<table border='1' cellspacing='0' cellpadding='2' width='100%'>
	  <tr>
	    <td class='rowgrey' width='14%' align='center'><?php echo $week_day_row["day1"]?></td>
	    <td class='rowgrey' width='14%' align='center'><?php echo $week_day_row["day2"]?></td>
	    <td class='rowgrey' width='14%' align='center'><?php echo $week_day_row["day3"]?></td>
	    <td class='rowgrey' width='14%' align='center'><?php echo $week_day_row["day4"]?></td>
	    <td class='rowgrey' width='14%' align='center'><?php echo $week_day_row["day5"]?></td>
	    <td class='rowgrey' width='14%' align='center'><?php echo $week_day_row["day6"]?></td>
	    <td class='rowgrey' width='14%' align='center'><?php echo $week_day_row["day7"]?></td>
	  </tr>
<?php
$href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;department_id=0&amp;job_id=";
while ($day_count[0] > 0 || $day_count[1] > 0 || $day_count[2] > 0 ||$day_count[3] > 0 ||$day_count[4] > 0 ||$day_count[5] > 0 ||$day_count[6] > 0){
	$job = "";
	echo "<tr>\n";
	$str = "";
	$col_span=1;
	
	for ($i=0;$i<7;$i++){
		if ($job && $day_count[$i]>0 && in_array($job, $day[$i])){
			$day[$i] = remove_element2($day[$i], $job);
			$day_count[$i]--;
			$col_span++;
		}else{
			if ($i != 0){
				if ($job){
					list($job_code, $interview_medium_id, $interview_place_id, $company, $department, $interviewers, $start_time) = get_info($job);
					if ($department == $auth->department) {
                        $depstring = "";
						$class="row0d";
					} else if (in_array($department,$departments_in_group)) {
                        $depstring = "";
						$class="row1d";
					} else {
                        $depstring = "<b>[".getDepartmentCode($department)."]</b><br />";
						$class="row2d";
					}
					if (isPhone($interview_medium_id)){
						if (isOnCampus($interview_place_id)){
							echo "<td valign='top' colspan='$col_span' class='".$class."'><a class='blue' href='" . $href . $job . "'>" . $job_code . " </a><img src='misc/images/phone.gif' width='18' height='12' border='0' alt='' /><br />" . $depstring . $company . "<br />" . $start_time . "</td>"; 
						} else {
							echo "<td valign='top' colspan='$col_span' class='".$class."'><a class='blue' href='" . $href . $job . "'>" . $job_code . " </a><img src='misc/images/phonehome.gif' width='26' height='16' border='0' alt='' /><br />" . $depstring . $company . "<br />" . $start_time . "</td>"; 
						}
					} elseif (!isOnCampus($interview_place_id)){
						echo "<td valign='top' colspan='$col_span' class='".$class."'><a class='blue' href='" . $href . $job . "'>" . $job_code . " </a><img src='misc/images/bus.gif' width='20' height='12' border='0' alt='' /><br />" . $depstring . $company . "<br />" . $start_time . "</td>"; 
					} else {
						echo "<td valign='top' colspan='$col_span' class='".$class."'><a class='blue' href='" . $href . $job . "'>" . $job_code . "</a><br />" . $depstring . $company . "<br />" . $start_time . "</td>"; 
					}
				}else{
					echo "<td valign='top' colspan='$col_span' class='row1d'>$str</td>\n";
				}
			}
			$found = 0;
			if ($i!=0){
				for($k=0;$k<$day_count[$i];$k++){
					if (!in_array($day[$i][$k], $day[$i-1])){
						$found = 1;
						break;
					}
				}
			}else{
				$k=0;
			}
			$col_span = 1;
			$str = "";
			if ($day_count[$i] > 0 && ($i==0 || $found)){
				$job = $day[$i][$k];
				$day[$i] = remove_element2($day[$i], $job);
				$day_count[$i] --;
				$str .= $job;
			}else{
				$job ="";
				$str .= "&nbsp;";
			}
		}
	}
	if ($job != ""){
		list($job_code, $interview_medium_id, $interview_place_id, $company, $department, $interviewers, $start_time) = get_info($job);
		if (isPhone($interview_medium_id)){
			if (isOnCampus($interview_place_id)){
		echo "<td colspan='$col_span' class='" . (($department == $auth->department) ? "row0d" : "row1d") . "'><a class='blue' href='" . $href . $job . "'>" . $job_code . " </a><img src='misc/images/phone.gif' width='18' height='12' border='0' alt='' /><br />" . $company . "<br />" . $start_time . "</td>"; 
			} else {
				echo "<td colspan='$col_span' class='" . (($department == $auth->department) ? "row0d" : "row1d") . "'><a class='blue' href='" . $href . $job . "'>" . $job_code . " </a><img src='misc/images/phonehome.gif' width='26' height='16' border='0' alt='' /><br />" . $company . "<br />" . $start_time . "</td>"; 
			}
		} elseif (!isOnCampus($interview_place_id)){
			echo "<td colspan='$col_span' class='" . (($department == $auth->department) ? "row0d" : "row1d") . "'><a class='blue' href='" . $href . $job . "'>" . $job_code . " </a><img src='misc/images/bus.gif' width='20' height='12' border='0' alt='' /><br />" . $company . "<br />" . $start_time . "</td>"; 
		} else {
			echo "<td colspan='$col_span' class='" . (($department == $auth->department) ? "row0d" : "row1d") . "'><a class='blue' href='" . $href . $job . "'>" . $job_code . "</a><br />" . $company . "<br />" . $start_time . "</td>"; 
		}
	}else{		
		echo "<td colspan='$col_span' class='row1d'>$str</td>\n";
	}
	echo "</tr>\n";
}
?>

	
	</table>
    </td>
  </tr>
</table>
<br />
<table class="row1d" cellpadding="10" cellspacing="0" border='1'>
<tr><td nowrap='nowrap'>Legend: &nbsp;&nbsp;</td><td nowrap='nowrap' colspan="2"><img src='misc/images/phone.gif' width='18' height='12' border='0' alt='' /> Phone Interview, &nbsp;&nbsp;<img src='misc/images/phonehome.gif' width='26' height='16' border='0' alt='' /> Off-Campus Phone Interview, &nbsp;&nbsp;<img src='misc/images/bus.gif' width='20' height='12' border='0' alt='' /> Off-Campus In-Person Interview</td> 
</tr>
<?php
if (sizeof($departments_in_group) > 1){
?>
	<tr><td nowrap='nowrap' class="row1d">&nbsp;</td><td nowrap='nowrap' class="row0d">Your Department's Interviews</td><td nowrap='nowrap' class="row1d">Your Group Departments' Interviews</td></tr>
	<tr><td nowrap='nowrap' class="row1d">&nbsp;</td><td colspan='2' nowrap='nowrap' class="row2d">Other Departments' Interviews for Your Students</td></tr>
<?php
} else {
?>
	<tr><td nowrap='nowrap' class="row1d">&nbsp;</td><td nowrap='nowrap' class="row0d">Your Department's Interviews</td><td nowrap='nowrap' class="row2d">Other Departments' Interviews for Your Students</td></tr>
<?php
}
?>
</table>
</form>



<?php

function remove_element2($array, $element){
	$temp_array = array();
	while ($ele = array_shift($array)){
		if ($ele != $element){
			array_push($temp_array, $ele);
		}
	}
	return $temp_array;
}

function get_info($job_id){
	$sql = "SELECT j.job_code, i.interview_medium_id, i.interview_place_id, e.company_name, j.department_id
		FROM interview as i, job_info as j LEFT JOIN employer_company as e ON j.employer_id = e.employer_id
		WHERE i.job_id=j.job_id AND j.job_id = '" . addslashes($job_id) . "'";
	$info_row_1 = $GLOBALS['dbh']->GetRow($sql);

	$sql = "SELECT DISTINCT t.interviewer_id, i.interviewer_name
		FROM interview_interviewer as i, interview_time as t
		WHERE i.interviewer_id = t.interviewer_id AND t.job_id = '" . addslashes($job_id) . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow()){
		$int .= $row["interviewer_name"] . ", ";
	}
	$int = substr($int, 0, strlen($int) - 2);

	$sql = "SELECT MIN(int_time) as start
		FROM interview_time
		WHERE job_id = '" . addslashes($job_id) . "'";
	$info_row_2 = $GLOBALS['dbh']->GetRow($sql);
	$result_array = array($info_row_1["job_code"] , $info_row_1["interview_medium_id"], $info_row_1["interview_place_id"], $info_row_1["company_name"], $info_row_1["department_id"], $int, formatTime($info_row_2["start"]));
	return $result_array;
}
