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
 | Filename: all.inc                                                            |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user wishes to view all of the     |
 | lunches, and have the ability to assign lunch hosts.                         |
 +------------------------------------------------------------------------------+

*/

?>

   Set : </td>
            <td width='5'>&nbsp;</td>
            <td valign="middle">&nbsp;<a href="<?php echo("$PHP_SELF&amp;select=lunchall"); ?>"><b class="blue">All Lunches</b></a>&nbsp;</td>
            <td width='5'>&nbsp;</td>
            <td valign="middle">&nbsp;<a href="<?php echo("$PHP_SELF&amp;select=lunchsome"); ?>"><b class="blue">Unassigned Lunches</b></a>&nbsp;</td>
            <td width='5'>&nbsp;</td>
<td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>

<script type="text/javascript" language="javascript">
//<![CDATA[
<!--javascript
function pop(tid){
	window.open("mamook.php?select=selectbox&no_headers=1&time_id="+tid,"Host","toolbar=no,menubar=no,fullscreen=0,top=0,left=0,height=175,width=400");
}	
//-->
//]]>
</script>

<?php

if ($userlevel == OFFICE){
        if ($department_id == ""){
                $department_id = $auth->department;
        }

        if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
		error ("You do not have access to this department");
                $department_id = $auth->department;
                $page = "";
                unset( $PDF );
        }
        $department_in_str = "'" . implode("', '", $departments_in_group) . "'";

$sql = ("
	SELECT ji.job_id, ji.job_code, it.int_time, it.end_time, ii.interviewer_name, it.int_date, ec.company_name, it.time_id 
	FROM interview_interviewer as ii, interview as i, interview_time as it, job_info as ji, employer_company as ec 
	WHERE it.job_id=ji.job_id 
	AND ec.employer_id=ji.employer_id 
	AND int_type=3 
	AND int_date>='".date(Y)."-".date(m)."-".date(d)."' 
	AND i.job_id=it.job_id 
	AND i.interview_place_id=1 
	AND i.interview_medium_id=1 
	AND ii.interviewer_id=it.interviewer_id 
	AND ji.department_id IN (".$department_in_str.") 
	AND NOT(i.cancelled)
	ORDER BY int_date, int_time
	");

} elseif ($userlevel == TRAFFICDIRECTOR) {
        if ($department_id == ""){
                $department_id = $auth->department;
        }

        if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
//		error ("You do not have access to this department.");
                $department_id = $auth->department;
                $page = "";
                unset( $PDF );
        }
        $department_in_str = "'" . implode("', '", $departments_in_group) . "'";

$sql = ("
	SELECT ji.job_id, ji.job_code, d.department_code, it.int_time, it.end_time, ii.interviewer_name, it.int_date, ec.company_name, it.time_id 
	FROM interview_interviewer as ii, interview as i, interview_time as it, job_info as ji, employer_company as ec, department as d
	WHERE it.job_id=ji.job_id 
	AND ec.employer_id=ji.employer_id 
	AND int_type=3 
	AND int_date>='".date(Y)."-".date(m)."-".date(d)."' 
	AND i.job_id=it.job_id 
	AND i.interview_place_id=1 
	AND i.interview_medium_id=1 
	AND ii.interviewer_id=it.interviewer_id 
	AND ji.department_id=d.department_id
	AND NOT(i.cancelled)
	ORDER BY int_date, int_time
	");

}

$results = $GLOBALS['dbh']->Execute($sql);  
$row = $results->FetchRow();
echo "<form name='myform' action='$PHP_SELF&amp;select=lunchall' method='post'>";

if ($message_sent=="yes"){
	notify("The e-mail has been sent successfully.");
}

echo ("<h3 align='left'>Interviews - All Lunch Schedules</h3>");
if ($results->RecordCount()==0){
        notify("There are no upcoming lunches.");
}
while ($row<>FALSE){

//	echo "<p>";
	$thedate=$row['int_date'];
        $anArray=explode("-",$row['int_date']);
        $adate=date("l F d, Y",mktime(0,0,0,$anArray[1],$anArray[2],$anArray[0]));
	echo "<h4 align='left'>".$adate.":</h4>";
	$count=0;
	echo "<table border='1' cellpadding='0' cellspacing='0'><tr><td>";
	echo "<table border='0' cellpadding='2'>";
	if ($userlevel == OFFICE) {
		echo "<tr><td class='rowgrey' align='center'>&nbsp;<b class='white'>Time</b>&nbsp;</td><td class='rowgrey' align='center'>&nbsp;<b class='white'>Interviewer</b>&nbsp;</td><td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td><td class='rowgrey' align='center'>&nbsp;<b class='white'>Lunch Host</b>&nbsp;</td><td class='rowgrey' align='center'>&nbsp;<b class='white'>Edit Host</b>&nbsp;</td>";
	} elseif ($userlevel == TRAFFICDIRECTOR) {
		echo "<tr><td class='rowgrey' align='center'>&nbsp;<b class='white'>Time</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Interviewer</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Host Department</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Lunch Host</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Edit Host</b>&nbsp;</td>";
	}
	while ($row['int_date']==$thedate && $row<>FALSE){
		if ($count%2 == 0){
			$class="row1d";
		}else{
			$class="row0d";
		}
	
		$lunch_sql="SELECT c.first_name,c.last_name FROM contact as c, interview_time as it WHERE it.int_type=3 AND c.contact_id = it.student_number AND it.job_id=".$row['job_id']." AND it.int_time='".$row['int_time']."'"; 
		$lunch_results=$GLOBALS['dbh']->Execute($lunch_sql);
		$lunch_row=$lunch_results->FetchRow();
		if ($lunch_results->RecordCount()>0){
			$assigned=$lunch_row['first_name']." ".$lunch_row['last_name'];
		}else{
			$assigned="<b>Unassigned</b>";	
		}
?>
		<tr>
			<td align='center' class="<?php echo $class?>">&nbsp;<?php echo formatTime(substr($row['int_time'],0,strlen($row['int_time'])-3))?> - <?php echo formatTime(substr($row['end_time'],0,strlen($row['end_time'])-3))?>&nbsp;</td>
			<td align='center' class='<?php echo $class?>'>&nbsp;<?php echo $row['interviewer_name']?>&nbsp;</td>
			<td align='center' class="<?php echo $class?>">&nbsp;<a href="<?php echo $PHP_SELF?>?&amp;select=view&amp;department_id=0&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;job_id=<?php echo $row['job_id']?>"><?php echo $row['company_name']?></a>&nbsp;</td>
			<?php
			if ($userlevel == TRAFFICDIRECTOR){
			?>
			<td align='center' class='<?php echo $class?>'>&nbsp;<?php echo $row['department_code']?>&nbsp;</td>
			<?php
			}
			?>
			<td align='center' class="<?php echo $class?>">&nbsp;<?php echo $assigned?>&nbsp;</td>
			<td align='center' class="<?php echo $class?>">&nbsp;<img width='15' height='15' src="misc/images/clipboard.gif" onmousedown="pop(<?php echo $row['time_id']?>)" alt="" />&nbsp;</td>
		</tr>		
<?php
		$row=$results->FetchRow();
		$count=$count+1;
	}
	echo "</table></td></tr></table>";
}
?>
<input type="hidden" name="submitted" value="yes" />
<input type="hidden" name="changethis" value="" />
</form>
