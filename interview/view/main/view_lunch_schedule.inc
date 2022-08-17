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
 | Filename: view_lunch_schedule.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: show break_time, interviewer_name, company_name in table form   |
 | for all interviews on a given date  $date, $department must be sent by the   |
 | script that includes this file.                                              |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
$int_date = $date;

$result3 = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT b.int_time, b.end_time, c.interviewer_name, f.company_name, b.student_number,b.time_id
	FROM interview_time AS b, interview_interviewer AS c, interview AS d, employer_company AS f, 
	job_info as j, interview_medium as m, interview_place as z
	WHERE b.int_date='" . addslashes($int_date) . "' 
	AND b.int_type='3'
	AND j.department_id IN (" . $department_in_str . ")
	AND m.interview_medium_id = d.interview_medium_id 
	AND b.interviewer_id=c.interviewer_id 
	AND j.job_id = d.job_id 
	AND b.job_id=d.job_id 
	AND j.employer_id=f.employer_id 
	AND m.description = '" . INPERSON . "'
	AND z.interview_place_id = d.interview_place_id
	AND z.description = '" . ONCAMPUS . "'
	AND NOT(d.cancelled)
	ORDER BY b.int_time
	");

if ($result3->RecordCount())
{
   echo("<h4 align='left'>Lunch breaks on " . $int_date . ":</h4>");

   ?>
   <table cellspacing="0" cellpadding="0" border="1"><tr><td>
      <table border="0" cellpadding="2">
         <tr>
            <td class="rowgrey" align='center'>&nbsp;<b class="white">Time</b>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<b class="white">Interviewer</b>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<b class="white">Company</b>&nbsp;</td>
	    <td class="rowgrey" align='center'>&nbsp;<b class="white">Lunch Host</b>&nbsp;</td>
         </tr>
   <?php

   $rowclass = 0;
   while($row3 = $result3->FetchRow())
   {
	$internal_sql="Select Concat(c.first_name,' ',c.last_name) as name from contact as c,interview_time as it WHERE it.student_number=c.contact_id AND it.time_id=".$row3['time_id'];
        $internal_results=$GLOBALS['dbh']->Execute($internal_sql);
        $internal_row=$internal_results->FetchRow();
	if ($internal_results->RecordCount()>0){
		$host=$internal_row['name'];
	}else{
		$host="<b>Unassigned</b>";
	}  
      echo("<tr><td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;" . formatTime($row3["int_time"])." - ". formatTime($row3["end_time"]) . "&nbsp;</td>");
      echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;" . $row3["interviewer_name"] . "&nbsp;</td>");
      echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;" . $row3["company_name"] . "&nbsp;</td>");
      echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;" . $host . "&nbsp;</td></tr>");
      $rowclass++;
   }

   ?>
      </table>
   </td></tr></table>
   <?php
}
else
{
   echo("No interviewer lunch breaks are scheduled on $int_date.");
}

?>
