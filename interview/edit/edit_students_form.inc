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
 | Filename: edit_students_form.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This form is called by edit_students.inc and is used to         |
 | display the current shortlisted students, which students have signed up for  |
 | interviews and allows the user to change the interview shortlist.            |
 +------------------------------------------------------------------------------+

*/



// show the form

?>

<form method="post" action="<?php echo($PHP_SELF); ?>">
<input type='hidden' name='select' value='edit' />
<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
<input type='hidden' name='interview' value='<?php echo ( packObject(($interview)) ); ?>' />
<input type='hidden' name='level1' value='students' />

   <center>
   <table border="0" cellpadding="4" cellspacing="0" class="row1" width="100%">
   




<tr>
	<td align='right'>Students already signed<br />up for this interview:
	<br /><br />
	</td>
	<td>
	<br />
	<?php

	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT a.student_number,a.first_name, a.last_name, c.department_name, c.department_id
		FROM student AS a, interview_time AS b , department AS c,student_department as sd
		WHERE b.job_id='" . addslashes($interview->job_id) . "' AND a.student_number=b.student_number
		AND c.department_id = sd.department_id AND sd.student_number=a.student_number 
		ORDER BY a.last_name, a.first_name, a.student_number
		");

	$intRow=-1;
	$lastStudentNumber=0;
	while ($row=$result->FetchRow()){
		if ($lastStudentNumber<>$row['student_number']){
			$intRow=$intRow+1;
			$arrStudentsShortlisted[$intRow][0]=$row['first_name']." ".$row['last_name'];
			$arrStudentsShortlisted[$intRow][1]=$row['department_name'];
			$lastStudentNumber=$row['student_number'];
		}else{
			$arrStudentsShortlisted[$intRow][1]=$arrStudentsShortlisted[$intRow][1].", ".$row['department_name'];
		}
	}
	
	if ($result->RecordCount() == 0)
	{
		echo("* None *");
	} 
	else
	{
		$counter=0;
		while($counter<=$intRow)
		{
			echo("<br />");
			echo($arrStudentsShortlisted[$counter][0]."<b> (".$arrStudentsShortlisted[$counter][1].")</b>");
			$counter=$counter+1;
		}
	}
	?>
	<br /><br />
	<?php $result->Close(); ?>
	</td></tr>
<tr>
   <td align="right">Students short-listed:
   </td>
   <td>
	<textarea name='students' rows='7' cols='40'><?php
	if (trim($interview->students) != '')
	{
		echo($interview->students);
	} else
	{
		echo("");
	}
	?></textarea>

	<?php
	
	
	//$edit_conflict is set in update_duplicate function if two people are editing jobs at the same time

	if ($edit_conflict)
	{
		echo "<br /><b class='black'>Your Shortlist:";
		for($i=0;$i<count($sid_array);$i++){
			echo "<br />&nbsp;&nbsp;&nbsp;&nbsp;" . getStudentName($sid_array[$i]);
		}
		echo "</b>";
	}
	?>	
    </td>
</tr>


<tr>
   <td align="center" colspan="2"><hr />
		<input type='hidden' name='status' value='Continue' />
   		<input type="submit" value="Save the changes" />&nbsp;&nbsp;&nbsp;&nbsp;
   </td>
</tr>
   </table>
 </center> 
  </form>

