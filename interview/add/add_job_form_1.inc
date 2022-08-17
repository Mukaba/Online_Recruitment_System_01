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
 | Filename: add_job_form_1.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This is the first page displayed to the user when adding an     |
 | interview.                                                                   |
 +------------------------------------------------------------------------------+

*/
 
?>


<form method="post" action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="add" />
<input type="hidden" name="interview" value="<?php echo( isset($interview) ? packObject(($interview)) : "");?>" />
   
<center>
<table border="0" cellpadding="5" cellspacing="0" width="100%" class="row1">

<?php
// If this interview is for a job that already has a job code, the user shouldn't be able to change it.

if (!$interview->interview_only)
{
	echo("<tr>");
		echo("<td align='right'>Job code(s):</td>");
		echo("<td>");
		echo($interview->job_code);
		echo("</td>");
	echo("</tr>");
	echo("<input type='hidden' name='job_code' value='" . $interview->job_code . "' />");
}
else
{
	echo("<tr>");
	   	echo("<td align='right'>Job code:</td>");
	   	echo("<td>");
		echo("<input type='text' name='job_code' value='" . (($interview) ? $interview->job_code : urldecode($job_code)) . "' size='16' maxlength='30' />");
		echo("</td>");
	echo("</tr>");
}

// If this interview has an employer id already associated with it, editing company name is not allowed.

if ($interview->employer_id)
{
	echo("<tr>");
		echo("<td align='right'>Company:</td>");
		echo("<td>");
		echo($interview->company_name);
		echo("</td>");
	echo("</tr>");
	echo("<input type='hidden' name='company_name' value='" . $interview->company_name . "' />");
}
else
{
	echo("<tr>");
   		echo("<td align='right'>Company: <b class='red'>*</b></td>");
 		echo("<td>");
		echo("<input type='text' name='company_name' value='" . (($interview) ? $interview->company_name : urldecode($company_name)) . "' size='30' maxlength='75' />");
		echo("</td>");
	echo("</tr>");
}

?>
<tr>
	<td align="right">Employer E-mail Address:</td>
<?php 
     if( $application_email )
     {
         $the_email = $application_email;
     }
     elseif($interview)     
     {
         $the_email = $application_email;
     }
     else
     {
         $the_email = urldecode($email);
     }
 ?>
<td><input type="text" name="employer_email" value="<?php echo($the_email); ?>" size="40" maxlength="100" /></td>
</tr>
<tr>
   	<td align="right">Students short-listed: <b class='red'>*</b></td>
   	<td><textarea name="students" rows="7" cols="40">
<?php 
    if( $arr_selected_students )
    {
        foreach( $arr_selected_students AS $full_name )
        {
            echo($full_name."\n");
        }
    }
    elseif( $interview ) 
    {
        echo($interview->students);
    }
    else
    {
        echo(urldecode($students_shortlisted));    
    } 
?>
</textarea></td>
</tr>
<tr>
   	<td align="right">Number of days:</td>
   	<td>
		<?php
			echo("<select name='intdays'>");
			for($i = 1; $i <= 20; $i++)
			{
				echo("<option value='$i'>". $i. "</option>");
			}
			echo("</select>");
		?>
	</td>
</tr>
<tr>
	<td align="right">Interview Location:</td>
	<td>
		<?php
			$sql = "SELECT interview_place_id, description
				FROM interview_place
				ORDER BY interview_place_id";
			$result = $GLOBALS['dbh']->Execute( $sql );
			?>
			<select name="interview_place" size="1">
			<?php
			while ($row = $result->FetchRow()){
				?>
				<option value="<?php echo $row["interview_place_id"]?>" <?php if($row["interview_place_id"] == $interview->interview_place)  echo "selected='selected'" ?>><?php echo $row["description"]?></option>
				<?php
		}
		?>
		</select>
	</td>
</tr>

<tr>
	<td align="right">Interview Medium:</td>
	<td>
		<?php
			$sql = "SELECT interview_medium_id, description
				FROM interview_medium
				ORDER BY interview_medium_id";
			$result = $GLOBALS['dbh']->Execute( $sql );
			?>
			<select name="interview_medium" size="1">
			<?php
			while ($row = $result->FetchRow()){
				?>
				<option value="<?php echo $row["interview_medium_id"]?>" <?php if($row["interview_medium_id"] == $interview->interview_medium) echo "selected='selected'"?>><?php echo $row["description"]?></option>
				<?php
			}
		?>
		</select>
	</td>
</tr>
<tr>
	<td align="right">Is this company giving a presentation?</td>
	<td>
		<input class='row2' type="radio" name="presentation" value="1" <?php if (($interview) && $interview->presentation == 1) { echo(" checked='checked'"); } ?> />Yes
		&nbsp;&nbsp;&nbsp;&nbsp;
		<input class='row2' type="radio" name="presentation" value="0" <?php if ( !($interview) || (($interview) && $interview->presentation == 0) ) { echo(" checked='checked'"); } ?> />No
	</td>
</tr>
<tr>
	<td align='right' nowrap='nowrap'>
   		Person to Contact:
   	</td>
   	<td>
   		<select name='contact'> 
      	<?php

      		$result = $GLOBALS['dbh']->Execute("
			SELECT a.contact_id, b.first_name, b.last_name
			FROM contact_internal AS a, contact as b
			WHERE a.department_id=" . $auth->department . " 
			AND a.interview_contact_list=1 
			AND a.contact_id=b.contact_id
			");

			$users_id = getContactID($auth->department, $auth->login);

			while( $row =  $result->FetchRow() )      	   			
      			{
      				$name = $row['first_name'] . " " . $row['last_name'];
				$id = $row['contact_id'];
	       			echo("<option value='$id' ");
				if ($row["contact_id"] == $users_id)
				{
					echo("selected='selected'");
				}
				echo(">");
				echo("$name</option>");
      			}
      	?>
		</select>
   	</td>
</tr>
<?php


   $sql = "SELECT interview_type_id, description
   	   FROM interview_type
	   ORDER BY interview_type_id";
   $result = $GLOBALS['dbh']->Execute( $sql );
   
   $simswi = 1;
   if ($simswi == 1) 
      {
   ?>

<tr>
	<td align="right">Interview Type: </td>
	<td>
	<select name="interview_type" size="1">
	<?php
	while ($row = $result->FetchRow()){
		//default
		?>
		<option value="<?php echo $row["interview_type_id"]?>" <?php if ($row["interview_type_id"] == $interview->interview_type) echo "selected='selected'" ?>><?php echo $row["description"]?></option>
		<?php
	}
	?>
	</select>
	</td>
</tr>
<tr>
	<td align="right">Switching Interviews:</td>
	<td align="left">Are there multiple interviewers who want to interview the students separately?<br />eg. Interviewer A sees students 1,2,3,4 and Interviewer B sees students 2,1,4,3.</td>
</tr>
<tr> 
	<td align="right">Simultaneous Interviews:</td>
	<td align="left">Are there multiple interviewers who want to simultaneously interview a portion of the students?<br />eg. Interviewer A sees students 1,2,3 and Interviewer B sees students 4,5,6.</td>
</tr>
<tr>
	<td align="right">Sequential Interviews:</td>
	<td align="left">Do students need to see interviewers in a specific order?<br />
	eg. Student must see Interviewer A first, then Interviewer B, then Interviewer C.</td>
</tr>
	   
<tr>
	<td align='right' nowrap='nowrap'>If this is a sequential or simultaneous<br />interview, how many interviewers are there?</td>
	<td><select name='num_interviewers'>
	<?php
	for ($i = 2; $i < 9; $i++)
	{
		echo("<option value='" . $i . "'");
		if ((!$num_interviewers && $i == 2) || $num_interviewers == $i)
		{
			echo(" selected='selected'");
		}
		echo(">" . $i . "</option>");
	}
	?>
	</select>
	</td>
</tr>
<?php
	}
   ?>
<tr>
	<td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class='red'>*</b>) are required for the default form settings</td>
</tr>
<tr>
<td align="center" colspan="2"><hr />
	<input type="hidden" name="submit" value="Continue" />
	<input type="submit" value="Continue" name="submit" /></td>
</tr>
</table>
</center>
</form>

 
