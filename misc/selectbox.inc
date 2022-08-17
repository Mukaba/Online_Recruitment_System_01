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
 | Filename: selectbox.php                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file is called when a user wishes to assign a lunch        |
 | host. This file is a popup that allows them to set up the lunch and then     |
 | save changes, or to cancel and exit.  Comment added by Adam.                 |
 +------------------------------------------------------------------------------+

*/

if ($userlevel == OFFICE){
    if ($department_id == ""){
        $department_id = $auth->department;
    }

    if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
                        $error = ("You do not have access to this department");
                        $department_id = $auth->department;
                        $page = "";
                        unset( $PDF );
                }
                $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
        }

	if ($btnSubmit=="Save Changes"){
		if ($host=="Unassigned"){
			$tmpNumber="NULL";
		}else{
			$tmpNumber=$host;
		}
		$sql="Update interview_time set student_number=".$tmpNumber." WHERE time_id=".$time_id;
		$GLOBALS['dbh']->Execute($sql);
	}
	
	if ($userlevel == OFFICE) {
		$sql="Select c.first_name,c.last_name,c.contact_id from contact as c, contact_internal as ci WHERE ci.contact_id=c.contact_id AND ci.interview_lunch_list=1 AND ci.department_id IN (".$department_in_str.") Order by c.last_name, c.first_name";
		$results=$GLOBALS['dbh']->Execute($sql);
	} elseif ($userlevel == TRAFFICDIRECTOR) {
		$sql="Select c.first_name,c.last_name,c.contact_id,d.department_code from department as d, contact as c, contact_internal as ci 
			WHERE ci.contact_id=c.contact_id AND ci.interview_lunch_list=1 and d.department_id=ci.department_id Order by c.last_name, c.first_name";
		$results=$GLOBALS['dbh']->Execute($sql);
	}

	$info_sql="Select ii.interviewer_name,ji.job_id,ji.job_code,it.int_time,it.int_date,ec.company_name FROM interview_interviewer as ii, interview_time as it, job_info as ji,employer_company as ec WHERE ii.interviewer_id=it.interviewer_id AND it.job_id=ji.job_id AND ec.employer_id=ji.employer_id AND int_type=3 AND it.time_id=".$time_id;
	$info_results=$GLOBALS['dbh']->Execute($info_sql);
	$info_row=$info_results->FetchRow();

	$user_sql="Select contact_id from contact_internal where (login_id='".$login_id."' OR netlink_id='".$login_id."')";
	$user_results=$GLOBALS['dbh']->Execute($user_sql);
	$user_row=$user_results->FetchRow();

?>
<script type='text/javascript' language="javascript">
function initialize(){
	window.focus();
}
function initialize2(){
	window.opener.document.location.reload();
	self.close();
}
function initialize3(){
	window.opener.document.location="mamook.php?select=redirect&no_html=1&cid="+document.myform.cid.value+"&time_id="+document.myform.time_id.value;
	self.close();
}
function closewindow(){
	self.close();
}
function changevalues(){
	document.myform.cid.value=document.myform.host.options[document.myform.host.selectedIndex].value;
}
</script>
<link rel="stylesheet" href="css/color<?php echo $_SESSION['SESSION_colorscheme']; ?>.css" type="text/css">
</head>
<?php
	if ($btnSubmit<>"Save Changes"){
		echo "<body bgcolor='#FFFFFF' onLoad='initialize()'>";
	}else{
		if ($tmpNumber=="NULL" || $user_row['contact_id']==$tmpNumber){
			echo "<body bgcolor='#FFFFFF' onLoad='initialize2()'>";
		}else{
			echo "<body bgcolor='#FFFFFF' onLoad='initialize3()'>";
		}
	}
	
	$lunch_sql="SELECT CONCAT(c.first_name,' ',c.last_name) as name from contact as c, interview_time as it where it.student_number=c.contact_id and it.time_id=".$time_id." order by c.last_name, c.first_name";
	$lunch_results=$GLOBALS['dbh']->Execute($lunch_sql);
	$lunch_row=$lunch_results->FetchRow();
?>
<form name="myform" action="mamook.php?select=selectbox&amp;no_headers=1" method="POST">
<table cellpadding=0 cellspacing=0 align=center height=100%>
<tr>
	<td valign='middle'>
		<table border=1 cellpadding=3 cellspacing=0 align=center>
			<tr>
				<td class='rowgrey'><b class='white'>Time</b></td>
				<td class='rowgrey'><b class='white'>Interviewer</b></td>
				<td class='rowgrey'><b class='white'>Company Name</b></td>
			</tr>
			<tr>
				<td class='row0d'><?php echo $info_row['int_time']?></td>
				<td class='row0d'><?php echo $info_row['interviewer_name']?></td>
				<td class='row0d'><?php echo $info_row['company_name']?></td>
			</tr>
		</table>
		<p>
		<table align=center>
			<tr>
				<td class='rowgrey' align=center>
					<select name="host" onChange="changevalues()">
<?php
						if ($lunch_results->RecordCount()>0){
							echo "<option value='Unassigned'>Unassigned</option>";
						}else{
							echo "<option selected value='Unassigned'>Unassigned</option>";
						}
?>							
<?php
						if ($userlevel == OFFICE) {
							while($row=$results->FetchRow()){
								if ($lunch_row['name']<>$row['first_name']." ".$row['last_name']){
	?>
									<option value="<?php echo $row['contact_id']?>"><?php echo $row['first_name']." ".$row['last_name']?></option>						
	<?php
								}else{
	?>
									<option selected value="<?php echo $row['contact_id']?>"><?php echo $row['first_name']." ".$row['last_name']?></option>
	<?php
									$cid=$row['contact_id'];
	?>
	<?php
								}
							}
						} elseif ($userlevel == TRAFFICDIRECTOR) {
							while($row=$results->FetchRow()){
								if ($lunch_row['name']<>$row['first_name']." ".$row['last_name']){
	?>
									<option value="<?php echo $row['contact_id']?>"><?php echo $row['first_name']." ".$row['last_name']." (".$row['department_code'].")"?></option>						
	<?php
								}else{
	?>
									<option selected value="<?php echo $row['contact_id']?>"><?php echo $row['first_name']." ".$row['last_name']." (".$row['department_code'].")"?></option>
	<?php
									$cid=$row['contact_id'];
	?>
	<?php
								}
							}
						}
?>
				
					</select>
				</td>
			</tr>
		</table>
		<p>
		<table align=center>
			<tr>
				<td align=center><input type="submit" name="btnSubmit" value="Save Changes"></td>
				<td align=center><input type="button" name="cancel" value="Cancel and Close Window" onClick="closewindow()"></td>
			</tr>
		</table>
	</td>
</tr>
</table>
<input type="hidden" name="time_id" value="<?php echo $time_id?>">
<input type="hidden" name="cid" value="<?php echo $cid?>">
</form>
