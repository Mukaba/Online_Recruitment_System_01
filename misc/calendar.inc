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
 | Filename: calendar.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// Call the calendar function as follows...
// calendar($cal_year, $cal_month, $cal_date, $cal_submit);
//
// $cal_date will contain the currently selected date.
function calendar($year, $month, $select_date, $cal_submit="",$department_id, $out_dept=true, $locale_here=false)
{
   global $departments_in_group;
   echo("\n<!-- *** BEGIN CALENDAR *** -->\n");
   $themonths[1]="January";
   $themonths[2]="February";
   $themonths[3]="March";
   $themonths[4]="April";
   $themonths[5]="May";
   $themonths[6]="June";
   $themonths[7]="July";
   $themonths[8]="August";
   $themonths[9]="September";
   $themonths[10]="October";
   $themonths[11]="November";
   $themonths[12]="December";

   $this_year = date("Y");
   $this_month = date("n");
   $this_day = date("j");
   if ($year == "") { $year = $this_year; }
   if ($month == "") { $month = $this_month; }
   if ($cal_submit == "Today") {
      $year = $this_year;
      $month = $this_month;
      $select_date = "";
   }
   list ($year, $month) = cal_month_change($year, $month, $cal_submit);
   $days_in_month = cal_days_in_month($year, $month);

   if ($select_date == "") {
      $select_date = $year . "-" . $month . "-" . $this_day;
   }
   else
   {
      list(,,$sday) = explode("-", $select_date);
      if ($sday == "") { $sday = $this_day; }
      $select_date = $year . "-" . $month . "-" . (($days_in_month < $sday) ? $days_in_month : $sday) ;
   }
   $time_stamp = mktime(0, 0, 0, $month, 1, $year);
   $week_day = date("w", $time_stamp); 

   $qyear = $year;
   if (($qmonth = $month + 1) > 12) {
      $qyear++;
      $qmonth = 1;
   }
   if ($qmonth < 10) { $qmonth = "0" . $qmonth; }
global $auth;
$in_dept = false;
$out_dept = false;
if ($department_id == "0" || $department_id == $auth->department){
	$in_dept = true;
}
if (($department_id != $auth->department && $department_id != "0") || ($department_id == "0" && sizeof($departments_in_group) > 1)){
	$out_dept = true;
}

$department_in_str = "";
if ($department_id == "0"){
	foreach($departments_in_group as $dep){
		if ($dep != $auth->department){
			$department_in_str .= "'" . $dep . "', ";
		}
	}
	$department_in_str = substr($department_in_str, 0, strlen($department_in_str) - 2);
}else{
	$department_in_str = "'" . $department_id . "'";
}

if ($out_dept)
{
  $sql_out_dept = ("
	SELECT a.int_date
	FROM interview_time AS a, job_info AS b, interview AS c
	WHERE a.job_id=b.job_id
	AND c.job_id=b.job_id
	AND NOT(c.cancelled)
	AND b.department_id IN (" . $department_in_str . ")
	AND a.int_date>='$year-" . (($month < 10) ? "0" : "") . $month . "-01'
	AND a.int_date<'$qyear-$qmonth-01'
	GROUP BY a.int_date
	");

   $result_out_dept = $GLOBALS['dbh']->Execute($sql_out_dept);
   while (list($rows_out_dept[]) = $result_out_dept->FetchRow());
   $int_dates_out_dept = count($rows_out_dept);

}

if ($in_dept){
	// locale part in query needs fixing
   $sql_in_dept = ("
	SELECT a.int_date
	FROM interview_time AS a, interview AS b, job_info AS j, interview_place as p, interview_medium as m
	WHERE a.job_id=b.job_id
	AND j.job_id = b.job_id
	AND j.department_id= '" . $auth->department . "' 
	AND a.int_date>='$year-" . (($month < 10) ? "0" : "") . $month . "-01'
	AND a.int_date<'$qyear-$qmonth-01'
	AND a.int_type=1
	AND m.interview_medium_id = b.interview_medium_id
	AND p.interview_place_id = p.interview_place_id
	" . (($locale_here) ? " AND (p.description = '" . ONCAMPUS . "'  OR (m.description = '" . PHONE . "'))" : "") . "
	AND NOT(b.cancelled)
	GROUP BY a.int_date
	");

   $result_in_dept = $GLOBALS['dbh']->Execute($sql_in_dept);
   while (list($rows_in_dept[]) = $result_in_dept->FetchRow());
   $int_dates_in_dept = count($rows_in_dept);
}
?>
      <input type="hidden" name="cal_year" value="<?php echo($year); ?>" />
      <input type="hidden" name="cal_month" value="<?php echo($month); ?>" />
      <table class="cal" border="1" cellspacing="0" cellpadding="2">
         <tr class="cal-titlebar">
            <td align="center"><input class="cal-submit" type="submit" name="cal_submit" value="&lt;" /></td>
            <td colspan="5" align="left" class="cal-title">&nbsp;
		<select name="themonthyear" onchange="document.myform.submit()">
<?php
			for ($x=13;$x>-12;$x--){
				$reference=$x+$month;
				if ($reference<=0){
					$xmonth=$reference+12;
					$xyear=$year-1;
				}elseif ($reference>12){
					$xmonth=$reference-12;
					$xyear=$year+1;
				}else{
					$xmonth=$reference;
					$xyear=$year;
				}
				if ($x==0){
					echo "<option value=".chr(34).$xmonth.",".$xyear.chr(34)."selected='selected'>".$themonths[$xmonth]." ".$xyear."</option>";
				}else{
					echo "<option value=".chr(34).$xmonth.",".$xyear.chr(34).">".$themonths[$xmonth]." ".$xyear."</option>";
				}
			}
 ?>
		</select>
	    <input class="cal-submit" type="submit" name="cal_submit" value="Today" /></td>
            <td align="center"><input class="cal-submit" type="submit" name="cal_submit" value="&gt;" /></td>
         </tr>
         <tr class="cal-day-names-area">
            <td align="center" class="cal-day-names">Su</td><td align="center" class="cal-day-names">Mo</td><td align="center" class="cal-day-names">Tu</td>
            <td align="center" class="cal-day-names">We</td><td align="center" class="cal-day-names">Th</td><td align="center" class="cal-day-names">Fr</td>
            <td align="center" class="cal-day-names">Sa</td>
         </tr>
<?php

   echo("<tr class='cal-date-area'>\n");

   $day = 1;
   for ($i = 0; $i < $week_day; $i++) { echo("<td>&nbsp;</td>\n"); }
   while ($day <= $days_in_month)
   {
      if (!($i % 7) && ($i != 0)) { echo("</tr>\n<tr class='cal-date-area'>\n"); }
      $i++;

      $cdate1 = $year . "-" . (($month < 10) ? "0" : "") . $month . "-" . (($day < 10) ? "0" : "") . $day;
      $cdate2 = $year . "-" . $month . "-" . (($day<10) ? "0" : "") . $day;
	
	if ($out_dept)
	{
	      $interviews_out_dept = false;
	      for ($k = 0; $k < $int_dates_out_dept; $k++) { if ($rows_out_dept[$k] == $cdate1) { $interviews_out_dept = true; break; } }
	}
	if ($in_dept)
	{
	      $interviews_in_dept = false;
	      for ($k = 0; $k < $int_dates_in_dept; $k++) { if ($rows_in_dept[$k] == $cdate1) { $interviews_in_dept = true; break; } }
	}
	//      echo("<td align='right'");
	      echo("<td align='left'");
	      if ($day == $this_day && $month == $this_month && $year == $this_year) { echo(" class='cal-today'"); }
	      echo(">");

	if ($out_dept)
	{
	      if ($interviews_out_dept) { echo("<span style='text-decoration:underline;'>"); }
	}
	if ($in_dept)
	{
		if ($interviews_in_dept) { echo("<b>"); }
	}
	//      echo("<sup class='cal-dates'>" . $day . "</sup>");
	      echo("<sup class='cal-dates'>" . ((strlen($day) == 1) ? $day . " &nbsp;" : $day) . "</sup>");
	
	if ($in_dept)
	{
		if ($interviews_in_dept) { echo("</b>"); }
	}
	if ($out_dept)
	{
	      if ($interviews_out_dept) { echo("</span>"); }
	}
	
	if (($in_dept && $interviews_in_dept) || ($out_dept && $interviews_out_dept) )
	{
	         echo("<input name='cal_date' type='radio' value='$cdate1'");
		if ($cdate2 == $select_date) { echo(" class='cal-today' checked='checked'"); }
	         echo(" />");
	}
	
	echo("</td>\n");
	$day++;
}
	   
   for (; $i % 7; $i++) { echo("<td>&nbsp;</td>\n"); }
   echo("</tr>\n");

?>
      </table>
<?php
echo("<!-- *** END CALENDAR *** -->\n\n");
}





function cal_month_change($year, $month, $month_change)
{
   if ($month_change == ">")
   {
      if ($month++ > 12) 
      {
         $month = 1;
         $year++;
      }
   }
   elseif ($month_change == "<")
   {
      if (--$month < 1)
      {
         $month = 12;
         $year--;
      }
   }
   return Array($year, $month);
}





function cal_days_in_month($year, $month)
{
   if ($month == 2) { return ((($year % 4) || (!($year % 100) && ($year % 400))) ? 28 : 29); }
   elseif ($month == 4 || $month == 6 || $month == 9 || $month == 11) { return 30; }
   else { return 31; }
}

?>
