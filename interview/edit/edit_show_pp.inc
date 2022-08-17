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
 | Filename: edit_show_pp.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: Show the phone and presentation radio buttons for the edit      |
 | job_form                                                                     |
 +------------------------------------------------------------------------------+

*/

?>

<tr>
  <td align="right">Is the company giving a presentation?</td>
  <td>
<?php
if ($interview->presentation == 0)
{

               ?>
                 No&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                 	<input type='hidden' name='presentation' value='<?php echo($interview->presentation); ?>' />
                 	<input type="submit" name="submit" value="Add a presentation" />
		<?php  

}
else
{    

               ?>
                 <table border="0" cellpadding="4" cellspacing="0" class="row0">
                    <tr><td rowspan="7" class="row1">Yes&nbsp;&nbsp;</td>
                    <td>Date (yyyy-mm-dd):</td>
<?php

?>
				 
				 <td>
				 	<input type="text" name="pres_date" value="<?php echo($interview->pres_date); ?>" size="12" maxlength="10" /><?php echo popup('pres_date','myform')?>
				 </td></tr>
                 <tr>
                 	<td>Time (24 hr clock):</td>
                 	<td><input type="text" name="pres_time" value="<?php echo($interview->pres_time); ?>" size="12" maxlength="8" /></td>
                 </tr>
                 <tr>
                 	<td>Length (mins):</td>
                 	<td><input type="text" name="pres_len" value="<?php echo($interview->pres_len); ?>" size="12" maxlength="3" /></td>
                 </tr>

                 <tr>
                 	<td>Location:</td>
                 	<td><input type="text" name="pres_loc" value="<?php echo($interview->pres_loc); ?>" size="30" maxlength="60" /></td>
                 </tr>
				 <tr>
            		<td>Notes:</td>
      				<td><textarea name="pres_notes" rows="5" cols="30"><?php echo($interview->pres_notes); ?></textarea></td>
            	 </tr>
		<tr>
			<td>Is this presentation viewable by all students, or only those shortlisted?</td>
			<td>
                        <input class='row2' type="radio" name="pres_viewable_by_all" value="1" <?php 
                        if ( !isset($interview->pres_viewable_by_all) || (($interview) && $interview->pres_viewable_by_all == 1) )
                        {
                                echo(" checked='checked'");
                        }
                        ?> />All&nbsp;&nbsp;&nbsp;&nbsp;
                        <input class='row2' type="radio" name="pres_viewable_by_all" value="0" <?php
                        if ((isset($interview->pres_viewable_by_all)) && $interview->pres_viewable_by_all == 0)
                        {
                                echo(" checked='checked'");
                        }
                        ?> />Shortlisted&nbsp;&nbsp;&nbsp;&nbsp;
                	</td>
            	</tr>

				 <tr>
				 	<td>&nbsp;</td>
				 	<td><center><input type='submit' name='submit' value='Cancel presentation' /></center></td>
				 </tr>
                 </table>
                 
<?php
}      

if ($show2){
	if ($interview2->presentation == 0){
		echo "<br /><b class='black'>(No)</b>";
	}else{
		echo "<br /><b class='black'>(Yes<br />&nbsp;&nbsp;&nbsp;&nbsp;Date: $interview2->pres_date<br />&nbsp;&nbsp;&nbsp;&nbsp;Time: $interview2->pres_time<br />
						&nbsp;&nbsp;&nbsp;&nbsp;Length: $interview2->pres_len<br />&nbsp;&nbsp;&nbsp;&nbsp;Location: $interview2->pres_loc<br />
						&nbsp;&nbsp;&nbsp;&nbsp;Notes: $interview2->pres_notes<br />&nbsp;&nbsp;&nbsp;&nbsp;Viewable By: ";
		echo((($interview2->pres_viewable_by_all) ? "All Students" : "Shortlisted Students Only") . "</b>)&nbsp;&nbsp;&nbsp;&nbsp;");
	}
}	
?>
                 </td></tr>

