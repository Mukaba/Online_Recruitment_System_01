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
 | Filename: show_data.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: Get all interview data for job $job_id from the database and    |
 | display it used after a interview is added or edited.                        |
 +------------------------------------------------------------------------------+

*/
$result0 = $GLOBALS['dbh']->Execute("SELECT * FROM job_info WHERE job_id='$job_id'");

$row = $result0->FetchRow();
$job_code = $row["job_code"];
$employer_id = $row["employer_id"];

$result1 = $GLOBALS['dbh']->Execute("select * from interview where job_id='$job_id'");
$row = $result1->FetchRow();

$notes = $row["notes"];
$staff_notes = $row["staff_notes"];
$interview_medium = $row["interview_medium_id"];
$interview_place = $row["interview_place_id"];

$presentation = $row["presentation"];

$interview_type = $row["interview_type_id"];

//retrieve presentation info
if( $presentation == 1 )
{
	$result4 = $GLOBALS['dbh']->Execute("SELECT * FROM presentation WHERE job_id='$job_id'");
	$row4 = $result4->FetchRow();
	$pres_date = $row4["pres_date"];
	$pres_time = $row4["pres_time"];
	$pres_end_time = $row4["end_time"];
	$pres_loc = $row4["pres_loc"];
	$pres_notes = $row4["pres_notes"];
	$pres_viewable_by_all = $row4["viewable_by_all"];
}

$result3 = $GLOBALS['dbh']->Execute("SELECT company_name FROM employer_company WHERE employer_id='$employer_id'");
$row = $result3->FetchRow();
$company_name 	= $row['company_name'];

if ($select != 'edit')
{

    $sql = "SELECT contact_id, department_id employer_department_id, employer_id, job_code
        FROM job_info
        WHERE job_id='" . $job_id . "'";
    $result = $GLOBALS['dbh']->Execute($sql);
    $row_quick = $result->FetchRow();

echo("
	<table width=85% style='margin-top:-1em;'>
	<tr>
	<td><hr /></td></tr>
	<tr>			
	<td align='right'>
    <form method='post' action='$PHP_SELF&amp;PDF=1&amp;job_id=$job_id'>

        <input type='hidden' name='employer_department_id' value='".$jobrow['employer_department_id']."' />          
        <input type='hidden' name='department_id' value='".$row_quick['department_id']."' />          
        <input type='hidden' name='employer_id' value='".$row_quick['employer_id']."' />
        <input type='hidden' name='job_id' value='".$job_id."' />
        <input type='hidden' name='contact_id' value='".$row_quick['contact_id']."' />
        <input type='hidden' name='job_code' value='".$row_quick['job_code']."' />

    <select name='selection'>
        <option value='view_company'>View this Company</option>
        ".(($row_quick['employer_department_id']) ? "<option value='view_department'>View this Job's Division</option>" : "")."
        ".(($row_quick['contact_id']) ? "<option value='view_contact'>View this Job's Contact</option>" : "")."
        <option value='view_applications'>View this Job's Applications</option>
        <option value='single'>Preview Schedule</option>
	    <option value='final'>Final Interview Schedule</option>
        <option value='final_student'>Final Interview Schedule (Student Numbers)</option>
	    <option value='edit'>Edit this Interview</option>
	    <option value='sign_up'>Sign up Students for this Interview</option>
	    <option value='email_schedule'>E-mail Schedule to Employer</option>
	");

	$sql = ("
		SELECT DISTINCT emailed_newsgroup
		FROM interview
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	if (trim($row["emailed_newsgroup"]))
	{
		echo("<option value='email_students'>E-mail Shortlisted Students</option>");
	}
echo("
	</select>
	<input type='image' src='misc/images/arrowg.gif' alt='go' />
	</form>
	</td>
	</tr>
	</table>
	");
} else
{
	echo("<br /><br />");
}
echo("<table>
		<tr>
			<td><b>Job Code:</b></td>
			<td>&nbsp;&nbsp;&nbsp;$job_code</td></tr>
		<tr>
			<td><b>Employer:</b></td>
			<td>&nbsp;&nbsp;&nbsp;$company_name</td></tr>");

$result_email = $GLOBALS['dbh']->Execute("SELECT c.email as email FROM contact as c, contact_job as e WHERE c.contact_id = e.contact_id AND e.job_id = '" . $job_id . "'");
$row_email = $result_email->FetchRow();

echo("		<tr>
			<td><b>Employer E-mail:</b></td>
			<td>&nbsp;&nbsp;&nbsp;" . $row_email["email"] . "</td>
		</tr>
		</table>
		<br />");
			


assert( empty($int_date) );

$result4a = $GLOBALS['dbh']->Execute("select distinct int_date from interview_time where job_id='$job_id' ORDER BY int_date");
while($row4a = $result4a->FetchRow())
{
   $int_date[] = $row4a[0];
}

$location = array();
$i=0;
if( isOnCampus($interview_place) || isPhone($interview_medium) )
{
	$result4 = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT interviewer_id 
		FROM interview_time 
		WHERE job_id = '$job_id'
		ORDER BY interviewer_id
		");
	$num_interviewers = $result4->RecordCount();
	while($row = $result4->FetchRow()){
		$result6=$GLOBALS['dbh']->Execute("SELECT distinct int_room_id, int_wait_room_id
				   FROM interview_time
				   WHERE interviewer_id = '" . $row["interviewer_id"] . "'");
		if ($result6->RecordCount() > 1){
			$location[$i] = "Many";
			$wait_room[$i] = "Many";
		}else{
			$row6 = $result6->FetchRow();
	   		$result4a = $GLOBALS['dbh']->Execute( "SELECT CONCAT(b.building_code, ' ', a.room_number) AS room_code, a.wait_room_id 
	   					   FROM room as a, building as b 
	   					   WHERE a.building_id=b.building_id AND a.room_id='" . $row6["int_room_id"] . "'" );
	   
	   		$row4a = $result4a->FetchRow();
	 		if( $row4a["room_code"] != '' )
	 		{
		   		$location[$i] = $row4a["room_code"];
			}else{
		   		$location[$i] = "T . B . A";
			}
	   
	   		$result4b = $GLOBALS['dbh']->Execute( "SELECT CONCAT(b.building_code, ' ', a.room_number) AS room_code, a.wait_room_id 
	   				   FROM room as a, building as b 
	   				   WHERE a.building_id=b.building_id AND a.room_id='" . $row6["int_wait_room_id"] . "'" );
	   		
			$row4b = $result4b->FetchRow();
	   		if( $row4b["room_code"] != '' ){
				$wait_room[$i] = $row4b["room_code"];
			}else{
				$wait_room[$i] = "T . B . A";
	   		}
		}
		$result5 = $GLOBALS['dbh']->Execute("select distinct interviewer_name from interview_interviewer where interviewer_id = '" . $row["interviewer_id"] . "'");
		$row5 = $result5->FetchRow();
		$interviewer_name[$i] = $row5[0];
		$i++;
	}

   echo("<table cellspacing='1' cellpadding='1'>");
   if( !(isSimultaneous($interview_type)) && !(isSwitching($interview_type)) && !(isSequential($interview_type)))
   {
      echo("<tr><td><b>Interviewer:</b></td><td>$interviewer_name[0]</td></tr>");
      echo("<tr><td><b>Waiting room:</b></td><td>$wait_room[0]</td></tr>");
      echo("<tr><td><b>Interview location:</b></td><td>$location[0]</td></tr></table><br />");
   }
   else
   {
      for( $i = 0; $i < $num_interviewers; $i++)
      {
         $j = $i+1;
	 	 echo("<tr><td><b>Interviewer $j:</b></td><td>$interviewer_name[$i]</td></tr>");
         echo("<tr><td><b>Waiting room $j:</b></td><td>$wait_room[$i]</td></tr>");
         echo("<tr><td><b>Interview location $j:</b></td><td>$location[$i]</td></tr>");
	 	 echo("<tr><td>&nbsp;</td><td>&nbsp;</td></tr>");
      }
      echo("</table><br />");
   }
}
else
{
	$result4 = $GLOBALS['dbh']->Execute("select distinct interviewer_id from interview_time where job_id = '$job_id' ORDER BY interviewer_id");
	$num_interviewers = $result4->RecordCount();
	while($row4 = $result4->FetchRow())
	{
	   $result5 = $GLOBALS['dbh']->Execute("select distinct interviewer_name from interview_interviewer where interviewer_id = '" . $row4["interviewer_id"] . "'");
	   $row5 = $result5->FetchRow();
	   $interviewer_name[$i] = $row5[0];
	   $i++;
	}

   echo("<table cellspacing='1' cellpadding='1'>");
   if( !(isSimultaneous($interview_type)) && !(isSwitching($interview_type)) && !(isSequential($interview_type)))
   {
      echo("<tr><td><b>Interviewer:</b></td><td>$interviewer_name[0]</td></tr>");
   }
   else
   {
      for( $i = 0; $i < $num_interviewers; $i++)
      {
         $j = $i+1;
	 	 echo("<tr><td><b>Interviewer $j:</b></td><td>$interviewer_name[$i]</td></tr>");
      }
   }

   $result5a = $GLOBALS['dbh']->Execute("select * from interview_location where job_id='$job_id'");
   $row5a =  $result5a->FetchRow();
   echo("<tr><td><b>Interview location:</b></td><td>" . $row5a['street_address_1'] . "</td></tr>");
   if( $row5a['street_address_2'] != '' ) echo("<tr><td>&nbsp;</td><td>" . $row5a['street_address_2'] . "</td></tr>");

   echo("<tr><td>&nbsp;</td><td>" . $row5a['city'] . ",&nbsp;" . $row5a['province'] . "</td></tr>
		 <tr><td>&nbsp;</td><td>" . $row5a['country'] . "</td></tr>");
   if ( $row5a['directions'] != '' )  echo("<tr><td><b>Directions :</b></td><td>". $row5a['directions'] ."</td></tr>");

   echo ("</table>");

}

      
/***		retrieve daily schedule ( interview, break, lunch )	***/  

for($i = 0; $i < count( $int_date ); $i++)
{
   echo("<table cellspacing='0' cellpadding='0' border='1'><tr><td><table border='0' cellpadding='2' cellspacing='2'>");

   $resulta = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT int_time, int_type 
	FROM interview_time 
	WHERE job_id='$job_id' 
	AND int_date='$int_date[$i]'
	AND sequential='0'
	ORDER BY int_time");

   $interviews = array ();
   $breaks = array ();
   $lunch = NULL;

   if (!defined (INTERVIEW))
     define (INTERVIEW, 1);
   if (!defined (NO_TIME))
     define (NO_TIME, 4);
   if (!defined (INT_BREAK))
     define (INT_BREAK, 2);
 
   while($row = $resulta->FetchRow())
   {
   		
   		if( $row["int_type"] == INTERVIEW || $row["int_type"] == NO_TIME )
   			$interviews[] = $row["int_time"];
   		
   		elseif( $row["int_type"] == INT_BREAK )
   			$breaks[] = $row["int_time"];
   		
   		else 					
   			$lunch = $row["int_time"];
   }
   
   echo("<tr><td class='rowgrey' align='center'><b class='white'>Date</b></td><td class='rowgrey' align='center'>&nbsp;<b class='white'>$int_date[$i]</b>&nbsp;</td></tr>");
   echo("<tr class='row1d' align='center'><td>&nbsp;<b>Time Slots</b>&nbsp;</td><td>");
   if( count($interviews) >= 1 )
   {	
	   foreach($interviews as $time)
	   {
	      echo(formatTime($time) . "<br />");
	   }
   }
   else
   {
	echo("&nbsp;");
   }
   echo("</td></tr>");
   
   $row_color = 0;
   
   if( $lunch )   
   {
      $lunch = formatTime($lunch);
      echo("<tr class='row0d' align='center'><td><b>Lunch</b></td><td>$lunch</td></tr>");
      $row_color=1;
   }

   if( $breaks )
   {
      if($row_color == 1) { echo("<tr class='row1d' align='center'><td><b>Breaks</b></td><td>"); }
      else { echo("<tr class='row0d' align='center'><td><b>Breaks</b></td><td>");  }
		
	  foreach( $breaks as $break )	
      {
         echo(formatTime($break) . "<br />");
      }
      echo("</td></tr>");
   }
   
   echo("</table></td></tr></table><br />");
   
   unset($interviews);
   unset($breaks);
   unset($lunch);

}//end for

echo("<table cellspacing='0' cellpadding='0' border='1'><tr><td><table border='0' cellpadding='2' cellspacing='2'>");

echo("<tr><td class='rowgrey' align='center'>&nbsp;<b class='white'>Students Shortlisted</b>&nbsp;</td><td class='rowgrey' align='center'><b class='white'>Department</b></td></tr>");

$result2 = $GLOBALS['dbh']->Execute("
   SELECT a.student_number,CONCAT(a.first_name, ' ', a.last_name) AS student_name, c.department_name as department_name
   FROM student AS a, students_shortlisted AS b, department as c, student_department as sd
   WHERE b.job_id='" . addslashes($job_id) . "' AND a.student_number=b.student_number AND sd.department_id = c.department_id AND sd.student_number=a.student_number
   ORDER BY a.last_name, a.first_name, department_name");

$intRow=-1;
$lastStudentNumber=-1;
while ($row=$result2->FetchRow()){
	if ($lastStudentNumber<>$row['student_number']){
		$intRow=$intRow+1;
		$arrStudentsShortlisted[$intRow][0]=$row['student_name'];
		$arrStudentsShortlisted[$intRow][1]=$row['department_name'];
	}else{
		$arrStudentsShortlisted[$intRow][1]=$arrStudentsShortlisted[$intRow][1].", ".$row['department_name'];
	}
	$lastStudentNumber=$row['student_number'];
}

$counter=0;
$rowcount=5;
while ($counter<=$intRow){
	echo ("<tr><td class='".(($rowcount % 2) ? "row0d" : "row1d")."'>&nbsp;".$arrStudentsShortlisted[$counter][0] . "&nbsp;</td><td class='" . ((($rowcount) % 2) ? "row0d" : "row1d") . "'>&nbsp;".$arrStudentsShortlisted[$counter][1]."</td></tr>");
	$counter=$counter+1;
	$rowcount++;
}



echo("</table></td></tr></table>");


$row9  = $GLOBALS['dbh']->GetRow("select description from interview_medium where interview_medium_id = '$interview_medium'");

echo("<p>Interview medium: " . $row9["description"] . "<br />");
echo("Is this company giving a presentation?  ");
if($presentation==0){
   echo("No</p>");
}else{
   echo("Yes<br /><br />");
   echo("<table border='0' cellpadding='2' cellspacing='2'>");
   echo("<tr class='row1d' align='left'><td>&nbsp;<b>Presentation date</b>&nbsp;</td><td>$pres_date</td></tr>");
   echo("<tr class='row1d' align='left'><td>&nbsp;<b>Presentation time</b>&nbsp;</td><td> " . formatTime($pres_time, false). " - " . formatTime( $pres_end_time, false ) . "</td></tr>");
   echo("<tr class='row1d' align='left'><td>&nbsp;<b>Presentation location</b>&nbsp;</td><td>$pres_loc</td></tr>");
   echo("<tr class='row1d' align='left'><td>&nbsp;<b>Presentation viewable by</b>&nbsp;</td><td>" . (($pres_viewable_by_all) ? "All students" : "Shortlisted Students Only") . "</td></tr>");
   if( $pres_notes )
   {
   		echo("<tr class = 'row1d' align='left'><td>&nbsp;<b>Presentation Notes</b></td><td>$pres_notes</td></tr>");
   }   
   echo("</table><br />");
}

if($notes) echo("<p>Notes:<br />$notes<br /><br />");

if ($staff_notes)
{
	echo("<p>Notes viewable only by Co-op staff:<br />" . $staff_notes . "<br /><br />");
}

echo("<table cellpadding='0' cellspacing='2'>
		<tr><td><a class='blue' href='$PHP_SELF&amp;select=edit&amp;job_id=$job_id'>Edit this Interview&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</a></td>
			<td><a class='blue' href='$PHP_SELF&amp;PDF=1&amp;selection=single&amp;job_id=$job_id'>Print Preview</a></td></tr></table>");
?>
