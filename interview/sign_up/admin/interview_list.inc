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
 | Filename: interview_list.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: list and link to the interviews available for the               |
 | administrators to sign up a student for                                      |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT a.job_id, j.job_code, a.emailed_employer, b.int_date,ec.company_name
	FROM interview AS a, 
		interview_time AS b,
		job_info AS j, employer_company as ec
	WHERE a.job_id=b.job_id AND 
	      a.job_id=j.job_id AND 
	      j.employer_id=ec.employer_id AND
	      b.int_date>=CURRENT_DATE AND 
	      a.cancelled=0
	      AND j.department_id IN (" . $department_in_str . ")
	GROUP BY a.job_id, j.job_code, b.int_date
	ORDER BY " . (($order == "email") ? "a.emailed_employer, ec.company_name, j.job_code, b.int_date"  : (($order == "company") ? "ec.company_name,
		j.job_code, b.int_date, a.emailed_employer" : (($order == "job_code") ? "j.job_code, 
		ec.company_name, b.int_date, a.emailed_employer" : "b.int_date, ec.company_name, j.job_code, a.emailed_employer" ))));
	
	$href = $PHP_SELF . "&amp;select=sign_up&amp;department_id=" . urlencode($department_id) . "&amp;order=";
	echo("<form method='post' action='$PHP_SELF&amp;select=sign_up&amp;selected=1&amp;order=$order&amp;department_id=$department_id'>");

if (sizeof($departments_in_group) > 1){
	?>
	<table class='row1' cellspacing='0' cellpadding='4' border='0'>
	  <tr>
	    <td>Department:</td>
	    <td><select name="department_id">
                <?php
                echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
		foreach($departments_in_group as $dep){
			echo "\t<option value='" . $dep . "'" . (($dep == $department_id) ? " selected='selected'" : "") . ">" . getDepartmentName($dep) . "</option>";
		}
		?>
		</select>
	    </td>
	    <td><input type="submit" value="Go" /></td> 
	  </tr>
	</table>
	<br />
<?php
}
?>
<table cellspacing='0' cellpadding='0' border='1'><tr><td>
	<table border='0' cellpadding='2'>
		<tr>
			<td class='rowgrey' align='center'>&nbsp;<a class="orderable" href="<?php echo($href); ?>date"><b class='white'>Date</b></a>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<a class="orderable" href="<?php echo ($href); ?>company"><b class='white'>Company</b></a></td>
			<td class='rowgrey' align='center'>&nbsp;<a class="orderable" href="<?php echo($href); ?>job_code"><b class='white'>Job Code</b></a>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Final Schedule</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<a class="orderable" href="<?php echo($href); ?>email"><b class='white'>E-mail Employer</b></a>&nbsp;</td>
		</tr>
<?php
		$rowclass = false;
		$signUpComplete = false;

		while ($row = $result->FetchRow())
		{
			$class = ($rowclass) ? "row0d" : "row1d";
			$job_id = $row["job_id"];
      	
			$signed_jobs .= $job_id . ",";
			$href = $PHP_SELF . "&amp;select=sign_up&amp;job_id=" . $row["job_id"] . "&amp;department_id=" . urlencode($department_id);

			echo("<tr><td align='center' class='" . $class . "'>&nbsp;<a class='blue' href='$href'>" . formatStandardDate($row["int_date"]) . "</a>&nbsp;</td>");
			echo("<td align='center' class='" . $class . "'>&nbsp;<a class='blue' href='$href'>" . $row["company_name"] . "</a>&nbsp;</td>");
			echo("<td align='center' class='" . $class . "'>&nbsp;<a class='blue' href='$href'>" . $row["job_code"] . "</a>&nbsp;</td>");

			if (isSignUpComplete($job_id))
			{
				echo("<td class='" . $class . "' align='center'><a class='blue' href='$PHP_SELF&amp;PDF=1&amp;selection=final&amp;job_id=$job_id&amp;department_id=$department_id'><img src='misc/images/calendar.gif' border='0' class='".$class."' alt='' /></a></td>");
			}
			else
			{
				echo("<td class='" . $class . "'>&nbsp;</td>");
			}
				echo("<td class='" . $class . "' align='center'>" . (($row["emailed_employer"]) ? "<img src='misc/images/email.gif' border='0' class='".$class."' alt='' />" : "&nbsp;") . "</td></tr>");

			$rowclass = !$rowclass;
		}
?>
   </table>
</td></tr></table>
</form>
