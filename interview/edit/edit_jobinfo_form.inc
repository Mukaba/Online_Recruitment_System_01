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
 | Filename: edit_jobinfo_form.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This form comes up with all data previously entered showing,    |
 | the user has the opportunity to change the data.                             |
 +------------------------------------------------------------------------------+

*/


// show the form

$show2=0;
if (isset($interview2))
{
	$show2 = 1;
}

?>

<form name='myform' method="post" action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="edit" />
<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
<input type='hidden' name='interview' value='<?php echo ( packObject(($interview)) ); ?>' />
<?php 
echo (($show2) ? "<input type='hidden' name='interview2' value='" . packObject(($interview2)) . "' />" : "" );

echo("<center>");
echo("<table border='0' cellpadding='4' cellspacing='0' class='row1' width='100%'>");

// If this interview is for a job that already has a job code, the user shouldn't be able to change it.
$sql = ("
	SELECT DISTINCT job_code
	FROM job_info
	WHERE job_id='" . $job_id . "'
	AND status='" . INT_ONLY . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

$sql = ("
	SELECT DISTINCT job_id
	FROM interview_job_join
	WHERE interview_job_id='" . $job_id . "'
	");
$result2 = $GLOBALS['dbh']->Execute($sql);

if (!$result->RecordCount() || $result2->RecordCount())
{
	echo("<tr>");
		echo("<td align='right'>Job code:</td>");
		echo("<td>");
		echo($interview->job_code);
		echo("<input type='hidden' name='job_code' value=\"" . $interview->job_code . "\" />");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Employer:</td>");
		echo("<td>");
		echo($interview->company_name);
		echo("<input type='hidden' name='company_name' value=\"" . $interview->company_name . "\" />");
		echo("</td>");
	echo("</tr>");
}
else
{
	echo("<tr>");
		echo("<td align='right'>Job code:</td>");
		echo("<td>");
		echo("<input type='text' name='job_code' value=\"" . $interview->job_code . "\" size='24' maxlength='30' />");
		echo(($show2) ? "<br /><b class='black'>(" . $interview2->job_code . ")</b>" : "");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Employer:</td>");
		echo("<td><input type='text' name='company_name' value=\"" . $interview->company_name . "\" size='30' maxlength='75' />");
		echo(($show2) ? "<br /><b class='black'>(" . $interview2->company_name . ")</b>" : "");
	   	echo("</td>");
	echo("</tr>");
}

echo("<tr>");
	echo("<td align='right'>Employer E-mail Address:</td>");
	echo("<td><input type='text' name='employer_email' value='" . $interview->employer_email . "' size='30' maxlength='100' />");
	echo(($show2) ? "<br /><b class='black'>(" . $interview2->employer_email . ")</b>" : "");
   	echo("</td>");
echo("</tr>");

$sql = ("
	SELECT description 
	FROM interview_place
	WHERE interview_place_id = '" . addslashes($interview->interview_place) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

?>
    
    <tr class="row1">
    	<td align="right">Interview Place:</td>
    	<td><?php echo (($row["description"]=="") ? "Error" : $row["description"]) ?>
        <input type="hidden" name="interview_place" value="<?php echo $interview->interview_place?>" />
        </td>
    </tr>
<?php
   if (!isSwitching($interview->interview_type) && !isSimultaneous($interview->interview_type) && !isSequential($interview->interview_type))
   {
?>
      <tr>
         <td align="right">Interviewer:</td>
         <td>
         <input type="text" name="interviewers[0]" value="<?php echo($interview->interviewers[0]); ?>" size="30" maxlength="100" />
	 	<?php echo (($show2) ? "<br /><b class='black'>(" . $interview2->interviewers[0] . "</b>)" : "" )?></td>

      </tr>
<?php
      if( isOnCampus($interview->interview_place) || isPhone($interview->interview_medium) )
      {
		$int_location = $interview->location_names[0];

		echo("
		      <tr>
        	 	<td align='right'>Interview Location:</td>
        	 	<td>" . (($int_location=="") ? "T . B . A" : $int_location) . "</td>
		      </tr>");
       }

   }
   else
   {
      for ($i = 0; $i < count($interview->interviewers); $i++)
      {
         $j = $i + 1;
         echo("<tr>");
         echo("<td align='right'>Interviewer $j:</td>");
         echo("<td><input type='text' name='interviewers[$i]' value=\"{$interview->interviewers[$i]}\" size='30' maxlength='100' />" . (($show2) ? "<br /><b class='black'>(" . $interview2->interviewers[$i] . ")</b>" : "") . "</td>");
         echo("<input type='hidden' name='interviewer_ids[$i]' value='{$interview->interviewer_ids[$i]}' />");
         echo("</tr>");
         if( isOnCampus($interview->interview_place) || isPhone($interview->interview_medium) )
         {

         	echo("<tr><td align='right'>Interview Location $j:</td>");
		
		$int_location = $interview->location_names[$i];
	
		echo("
        	 	<td>" . (($int_location=="") ? "T . B . A" : $int_location) . "</td>
		      </tr>");
	}

      }
   }
if( !isOnCampus($interview->interview_place) && !isPhone($interview->interview_medium) )
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT *
		FROM interview_location
		WHERE job_id = '" . $interview->job_id . "'
		");
	$row = $result->FetchRow();
	echo ("<tr><td align='right'>Location:</td><td>");
	$loc_str = $row["street_address_1"] . "<br />" . (($row["street_address_2"]=="") ? "" : $row["street_address_2"] .
			"<br />") . $row["city"] . ", " . $row["province"] . "<br />" . $row["country"];
	echo $loc_str;
	echo ("</td></tr>");


}

echo("<tr>
		<td align='right'>Person to Contact:</td>
		<td><select name='contact'>");


$result = $GLOBALS['dbh']->Execute("SELECT a.contact_id, b.first_name, b.last_name
					   FROM contact_internal AS a, contact as b
					   WHERE a.department_id=" . $auth->department . "
					   AND a.interview_contact_list=1 AND a.contact_id=b.contact_id");
$match = false;
$match2 = false;
while( $row =  $result->FetchRow() )      	   			
{
		$name = $row['first_name'] . " " . $row['last_name'];
		$id = $row['contact_id'];
		if( $id == $interview->contact )
		{
			$match = true;
			echo("<option value='$id' selected='selected'>$name</option>");
		}
		else {  echo("<option value='$id'>$name</option>"); }
		if ($id == $interview2->contact){
			$match2 = true;
			$contact_name = $name;
		}
}

if( !$match )
{
	$row = $GLOBALS['dbh']->GetRow( "SELECT CONCAT( first_name, ' ',  last_name) AS name from contact where contact_id = '" . $interview->contact . "'" );
	echo("<option value='" . $interview->contact . "' selected='selected'>" . $row['name'] . "</option>");
}
if ( !$match2 ){
	$row = $GLOBALS['dbh']->GetRow("SELECT CONCAT( first_name, ' ', last_name) AS name from contact where contact_id = '" . $interview->contact . "'" );
	$contact_name = $row["name"];
}
echo("</select>" . (($show2) ? "<br /><b class='black'>(" . $contact_name . ")</b>" : "") . "</td></tr>");


$sql = "SELECT *
	FROM interview_medium";
$result = $GLOBALS['dbh']->Execute($sql);

?>	<tr class="row1">
        <td align="right">Interview Medium:</td>
        <td><select name="interview_medium">
	<?php
	while($row = $result->FetchRow()){
		if ($row["interview_medium_id"] == $interview->interview_medium){
			?>
			<option value="<?php echo $row["interview_medium_id"]?>" selected='selected'><?php echo $row["description"]?></option>
			<?php
		}else{
			?>
			<option value="<?php echo $row["interview_medium_id"]?>"><?php echo $row["description"]?></option>
			<?php	
		}
		if ($row["interview_medium_id"] == $interview2->interview_medium){
			$int_medium = $row["description"];
		}
	}
?>
	</select>
	<?php echo (($show2) ? "<br /><b class='black'>(" . $int_medium . ")</b>" : "" ) ?>	
	</td></tr>

<?php

   include('interview/edit/edit_show_pp.inc');
   
?>

<tr>
   <td align="right">Students short-listed:</td>
   <td>
   <?php
		$result = $GLOBALS['dbh']->Execute("SELECT student_number FROM students_shortlisted WHERE job_id='$job_id'");
                while($row = $result->FetchRow())
                {
                   $result2 = $GLOBALS['dbh']->Execute("SELECT first_name, last_name FROM student WHERE student_number='$row[0]'");
                   $row2 = $result2->FetchRow();
		   $first_names[] = $row2['first_name'];
		   $last_names[] = $row2['last_name'];
		   $student_numbers[] = $row['student_number'];
                }
                if(sizeof($first_names))
                {
			array_multisort($last_names, SORT_ASC,  $first_names, $student_numbers);
                        for( $i = 0; $i < sizeof($first_names); $i++ )
                        {
                
				$get_deptcode = $GLOBALS['dbh']->Execute("SELECT DISTINCT a.department_name 
								FROM department as a, student as b, student_department as sd 
								WHERE b.student_number = '". $student_numbers[$i] . "' AND sd.student_number = b.student_number AND sd.department_id = a.department_id");
				
				if($row_deptcode = $get_deptcode->FetchRow()) {
					$dep = " (". $row_deptcode["department_name"] . ")";
						while ($row_deptcode=$get_deptcode->FetchRow()){
							$dep = substr($dep,0,strlen($dep)-1).",".$row_deptcode['department_name'].")"; 
						}
				} else { $dep = ""; }

		        echo $first_names[$i] . " " . $last_names[$i] . " <b class='black'><font size='-1'>$dep</font></b>";
			echo "<br />";
			}
                }
   ?>

   <input type="hidden" name="students" value="<?php echo($interview->students) ?>" />&nbsp;
   </td>
   </tr>

   <tr>
   <td align="right">Notes:</td>
   <td><textarea name="notes" rows="7" cols="40"><?php echo($interview->notes); ?></textarea>
   	<?php echo (($show2) ? "<br /><b class='black'>(" . $interview2->notes . ")</b>" : "") ?></td>
   </tr>
   <tr>
	<td align='right'>Notes viewable only by Co-op staff:</td>
	<td><textarea name='staff_notes' rows='7' cols='40'><?php echo($interview->staff_notes); ?></textarea>
	<?php echo (($show2) ? "<br /><b class='black'>(" . $interview2->staff_notes . ")</b>" : "") ?></td>
   </tr>

<?php

   /*** display daily schedule ***/
   for ($i = 0; $i < count($interview->int_dates); $i++)
   {
      $day = $i + 1;
      echo("<tr>");

      if ( count($interview->int_dates) == 1)
      {
         echo("<td align='right'>Interview Date:</td>");
      }
      else
      {
         echo("<td align='right'>Day $day:</td>");
      }

      foreach( $interview->schedules[$i] as $s )
      {
      	if( $s['TYPE'] == INTERVIEW || $s['TYPE'] == NO_TIME )
      		$displayTime[]  = formatTime( $s['START'], false);
      		
      	elseif ( $s['TYPE'] == INT_BREAK )
      		$displayBreak[] = formatTime( $s['START'], false);
      		
      	else
      		$displayLunch = formatTime( $s['START'], false );
      }
	  if( $displayTime )    $displayTime = implode( $displayTime, "<br />" );
      if( $displayBreak )   $displayBreak = implode ( $displayBreak, "<br />" );

      echo("<td>
      <table border='0' cellpadding='2' cellspacing='0' class='row0' width='90%' ><tr><td>
      <table border='0' cellpadding='4' cellspacing='1' class='row0' width='100%' >
     	<tr class='row1'>
     		<td align='right' width='40%' nowrap='nowrap'>
     			Date:&nbsp;&nbsp;</td>
	      	<td align='center'>
		      	" . $interview->int_dates[$i] . "
		<input type='hidden' name='int_dates[$i]' value='" . addslashes($interview->int_dates[$i]) ."' />
		</td>
	</tr>
	<tr class='row1'>
      		<td valign='middle' align='right' width='40%' nowrap='nowrap'>Time slots:&nbsp;&nbsp;</td>
      		<td>$displayTime</td></tr>");
	   if( $displayLunch )
		   echo("<tr class='row1'>
	      			<td align='right' width='40%' nowrap='nowrap'>Lunch break:&nbsp;&nbsp;</td>
		      		<td>$displayLunch</td></tr>");
	    if(  $displayBreak )
		    echo("<tr class='row1'>
		      		<td valign='middle' align='right' width='40%' nowrap='nowrap'>Other breaks:&nbsp;&nbsp;</td>
    		  		<td>$displayBreak</td></tr>");
	    echo("<tr class='row1'><td></td>
		  </tr>");
    		  		
		echo("</table></td></tr></table></td></tr>");
		
     unset( $displayTime );
     unset( $displayBreak );
	 unset( $displayLunch );
   }
?>

<tr><td colspan='2'><hr /></td></tr>
<tr>
   <td align="center" colspan="2">
		<input type="submit" name="submit" value="Save the changes" />
   </td>
</tr>
<?php
if (!$show2)
{
?>
<tr><td colspan='2'><hr /><br /></td></tr>
<tr>
   <td align='center' colspan='2'>
		
		<?php
		if( $interview->student_visible == 0 ) 
		{ 
			echo("<input type='submit' name='submit' value='Post Interview' />&nbsp;&nbsp;&nbsp;&nbsp;"); 
		}
		else 
		{ 
			echo("<input type='submit' name='submit' value='Remove from posting' />&nbsp;&nbsp;&nbsp;&nbsp;"); 
			if (!isNewsGroupEmailSent($job_id) && !$int_cancelled)
			{
				echo("<input type='submit' name='submit' value='E-mail Shortlist to Students' />&nbsp;&nbsp;&nbsp;&nbsp;");
			}
			else
			{
				echo("<input type='submit' name='submit' value='Send Shortlisted Students an E-mail' />&nbsp;&nbsp;&nbsp;&nbsp;");
			}
		}
		if (!$int_cancelled)
		{
			echo("<input type='submit' name='submit' value='Cancel Interview' />");
		}
		?>
	</td>
   </tr>
<tr><td colspan='2'><br /></td></tr>
<?php
}
?>
	</table>
    </center>
   </form>

