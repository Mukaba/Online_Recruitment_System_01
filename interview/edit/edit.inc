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
 | Filename: edit.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This is now the main edit file, which decides whether or not    |
 | to display the interviews for the user to choose which one to edit, or to    |
 | load the editchoose.inc file.                                                |
 +------------------------------------------------------------------------------+

*/

$email_employer = stripSlashes($email_employer);

if ($email_employer == "Don't Send")
{
	$email_employer == '';
	$job_id == '';
}

if (($job_id) || $level1 == "jobinfo" || $level1 == "timedate" || $level1 == "students" || $level1 == "location") 
{
	$URL = ("$PHP_SELF&amp;select=edit&amp;job_id=$job_id");

	?>

	Edit : </td>
            <td width='5'>&nbsp;</td>
            <td valign="middle" nowrap='nowrap'>&nbsp;<a href="<?php echo($URL . "&amp;level1=jobinfo"); ?>"><b class="blue">Interview Info</b></a>&nbsp;</td>
            <td width='5'>&nbsp;</td>
            <td valign="middle" nowrap='nowrap'>&nbsp;<a href="<?php echo($URL . "&amp;level1=timedate"); ?>"><b class="blue">Date/Time</b></a>&nbsp;</td>
            <td width='5'>&nbsp;</td>
            <td valign="middle" nowrap='nowrap'>&nbsp;<a href="<?php echo($URL . "&amp;level1=students"); ?>"><b class="blue">Students shortlisted</b></a>&nbsp;</td>
            <td width='5'>&nbsp;</td>
            <td valign="middle" nowrap='nowrap'>&nbsp;<a href="<?php echo($URL . "&amp;level1=location"); ?>"><b class="blue">Location</b></a>&nbsp;</td>
            <td width='5'>&nbsp;</td>
		<!-- Close the header -->
		<td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>

	<?php
} 
else 
{
	echo $level1;
	?>
	 </td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>
	
	<?php 
}

assert( define("INTERVIEW", 1) );
assert( define("INT_BREAK", 2) );
assert( define("LUNCH",3) );
assert( define("NO_TIME",4) );

include('interview/edit/EditInterview.class');
include('interview/add/dayplan.inc');
include('interview/add/dayplan_input.inc');
if ($department_id == "")
{
	if ($departments_in_group > 1)
	{
		if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) 
		{
			$group_sql = ("
				SELECT b.pulldown_menu_group
				FROM contact_internal AS b
				WHERE b.department_id='$auth->department' 
				AND b.netlink_id='$login_id'
				");
		} 
		else 
		{
			$group_sql = ("
				SELECT b.pulldown_menu_group
				FROM contact_internal AS b
				WHERE b.department_id='$auth->department' 
				AND b.login_id='$login_id'
				");
		}

		$result_group = $GLOBALS['dbh']->Execute($group_sql);
		while($row=$result_group->FetchRow())
		{
			$pulldown_menu_group=$row['pulldown_menu_group'];
		}

		if ($pulldown_menu_group=='1')
		{
			$department_in_str = "'" . implode("', '", $departments_in_group) . "'";
			$department_id='0';
		} 
		else 
		{
			$department_id = $auth->department;
		}
	} 
	else 
	{
		$department_id = $auth->department;
	}
}
if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
	error ("You do not have access to this department.");
	$job_id = "";
	$department_id = $auth->department;
}

if ($department_id == "0")
{
	$department_in_str = "'" . implode("', '", $departments_in_group) . "'";
}
else
{
	$department_in_str = "'" . $department_id . "'";
}


$result = $GLOBALS['dbh']->Execute("select CURRENT_DATE");
$row = $result->FetchRow();
$current_date = $row["CURRENT_DATE"];

if (!isValidJobID($job_id))
{

//	if a job hasn't been selected to edit, show the list of jobs available
	echo("<h3>Interviews - Edit</h3>");   	
	echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>");

	if (($choice != 'Email shortlist' && $choice != 'Email shortlist2') || !$mail[0])
	{
		echo("<h4 align='left'>Please select an interview to edit:</h4>");
	}
	include('interview/edit/list_jobs_admin.inc'); 
}
elseif( $student_num )
{
	 include('interview/edit/edit_header.inc');
	 update_duplicate("edit");
	 include('interview/edit/edit_footer.inc');
}
elseif( isJobInPast($job_id) )
{
	// The user is trying to edit a job that is in past.  This is not
	// allowed.
	echo("<h3>Interviews - Edit</h3>");   	
	
	echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>");
	error("The interview you have chosen is in the past.  Please select a current interview to edit.");
	echo("<h4 align='left'>Please select an interview to edit:</h4>");
	include('interview/edit/list_jobs_admin.inc'); 
}	
else
{
	include('interview/edit/edit_header.inc');
	include('interview/edit/editchoose.inc');
	include('interview/edit/edit_footer.inc');
}

