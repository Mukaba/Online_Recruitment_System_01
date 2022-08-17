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
 | Filename: view_by_date_main.inc                                              |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_by_date_main.inc
if ($department_id == "") { $department_id = $auth->department; }

if ($cal_date == "") { $cal_date = Date("Y-m-d"); } // default to today
?>
<form name="myform" action="<?php echo($PHP_SELF . "&amp;select=view") ?>" method="post">
   <input type="hidden" name="level1" value="date" />

   <table class='row1' cellspacing='0' cellpadding='4' border="0" width="60%">
	<?php
	if (sizeof($departments_in_group) > 1){
	?>
	<tr align='center'>
	<td align='right'>Department:</td>
	<td align='left'><select name="department_id">
                <?php
                echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
		foreach($departments_in_group as $department){
			echo("<option value='" . $department . "'" . (($department == $department_id) ? " selected='selected'" : "") . ">" . getDepartmentName($department) . "</option>\n");
		}
		?>
	    </select>
	    <input type="submit" value="Go" /></td>
	</tr>
	<tr><td colspan='2'><hr /></td></tr>
	<?php
	}
	?>
      <tr>
         <td colspan="2" align="center">
<?php
   include("misc/calendar.inc");
   if ($themonthyear<>NULL && $cal-submit==NULL){
	$anArray=explode(",",$themonthyear);
 	$cal_month=$anArray[0];
	$cal_year=$anArray[1];
	calendar($cal_year, $cal_month,"", $cal_submit, $department_id);	
   }else{
	   calendar($cal_year, $cal_month, $cal_date, $cal_submit, $department_id); 
   }
?>
         </td>
      </tr>
      <tr>
         <td colspan="2" align="center">Show:&nbsp;&nbsp;
            <select name="level2">
               <option value="all_interviews"<?php if ($level2 == "all_interviews") { echo(" selected='selected'"); } ?>>All Interviews</option>
               <option value="all_interviews_by_student"<?php if ($level2 == "all_interviews_by_student") { echo (" selected='selected'"); }?>>All Interviews by Student</option>
               <option value="day_schedule"<?php if ($level2 == "day_schedule") { echo(" selected='selected'"); } ?>>Day Schedule</option>
               <option value="lunch_schedule"<?php if ($level2 == "lunch_schedule") { echo(" selected='selected'"); } ?>>Lunch Schedule</option>
            </select>
         </td>
      </tr>
      <tr>
         <td colspan="2" align="center"><hr />
            <input type="submit" name="btnsubmit" value="View Results" />
         </td>
      </tr>
   </table>
</form>
