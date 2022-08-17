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
 | Filename: dayplan_input.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: get the initial inputs for time calucaltor                      |
 +------------------------------------------------------------------------------+

*/

function getDayPlan( $URL, $intdays, $day, $default_int_date, $default_int_num = 1, $default_int_length = '' )
{
	global $interview;
	global $start_time;

 	assert( isset($interview) && is_object($interview) );
	echo("<form name='myform' method='post' action='$URL'>");

	echo("<center><table border='0' cellpadding='4' cellspacing='0' width='100%'>");

   	if ($intdays > 1) 
   	{
	  $color_scheme = "row1d";
	  echo("<tr><td colspan='2' class='row0' align='center'><b class='white'>Day $day</b></td></tr>");
   	}
   	else { $color_scheme = "row1"; }
  
    echo("<tr>");
    echo("<td>");
    echo("<input type='hidden' name='day' value='$day' />");
    echo("<input type='hidden' name='intdays' value='$intdays' />");
    echo("<input type='hidden' name='time_mode' value='dayplan_input_complete' />");
    echo("<input type='hidden' name='coming_from_start' value='1' />");
    echo("<input type='hidden' name='default_int_date' value='$default_int_date' />");
	echo("<input type='hidden' name='interview' value='" . packObject(($interview)) . "' />");
    echo("</td>");
    echo("</tr>");
?>

<tr class='<?php echo( $color_scheme ); ?>'>
	<td align="right">Interview date (yyyy-mm-dd): <b class='red'>*</b></td>
	<td><input type="text" name="int_date" value="<?php echo($default_int_date); ?>" size="12" maxlength="10" />&nbsp;<?php echo popup('int_date','myform')?></td>
	</tr>
	<tr class='<?php echo( $color_scheme ); ?>'>
		<td align="right" nowrap='nowrap'><!--  width=60% -->First interview time (24 hour clock): <b class='red'>*</b></td>
		<td><input type="text" name="start_time" 
				value="<?php if( (isset($start_time)) && ($start_time!=NULL) ) 
									echo(formatTime($start_time, false)); ?>" size="12" maxlength="8" /></td>
	</tr>
	<tr class='<?php echo( $color_scheme ); ?>'>
		<td align="right" width=60%>
		<?php
		echo(($interview->interview_type == 2) ? "Total time each student is interviewed (minutes): <b class='red'>*</b>" : "Interview length (minutes): <b class='red'>*</b>");
		?>
		</td>
		<td><input type="text" name="int_length" value="<?php echo($default_int_length); ?>" size="12" maxlength="3" /></td>
	</tr>
	<tr class='<?php echo( $color_scheme ); ?>'>
	<td align="right">
	<?php
	echo(($interview->interview_type == 3 || $interview->interview_type == 2) ? "Number of students seeing each interviewer:" : "Number of interview times:");
	?>
		</td>
		<td>
		<select name="int_num">
		<?php

			for( $i = 1; $i < 26; $i++ )
			{
				echo("<option value='$i'" .(( $i == $default_int_num )?"selected='selected'":"") . ">" . "$i" . "</option>");
			}
		echo("</select></td></tr>");
		?>
		<tr class='<?php echo( $color_scheme ); ?>'>
			<td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class='red'>*</b>) are required for the default form settings</td>
		</tr>
<?php
		echo("<tr class=".$color_scheme."><td colspan='2' align='center'><hr /><input type='submit' value='&nbsp;Continue&nbsp;' name='submit' /></td></tr></table></center></form>");
}
?>
