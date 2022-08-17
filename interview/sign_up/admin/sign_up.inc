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
 | Filename: sign_up.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

?>
<!-- Close the header  -->
</td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" border="0" alt='' /></td></tr></table>

<?php

// filename: sign_up.inc
// high level code for admin sign up sheet.

echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>\n");
if ($department_id == ""){
        if ($departments_in_group > 1){
                if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) {
                        $group_sql = "SELECT b.pulldown_menu_group
                        FROM contact_internal AS b
                        WHERE b.department_id='$auth->department' AND b.netlink_id='$login_id'";
                } else {
                        $group_sql = "SELECT b.pulldown_menu_group
                        FROM contact_internal AS b
                        WHERE b.department_id='$auth->department' AND b.login_id='$login_id'";
                }
                $result_group=$GLOBALS['dbh']->Execute($group_sql);
                while($row=$result_group->FetchRow()){
                        $pulldown_menu_group=$row['pulldown_menu_group'];
                }
                if ($pulldown_menu_group=='1'){
                        $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
                        $department_id='0';
                } else {
                        $department_id = $auth->department;
                }
        } else {
                $department_id = $auth->department;
        }
}
if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
    
    error ("You do not have access to this department.");
	$job_id = "";
	$department_id = $auth->department;
}
if ($department_id == "0"){
	$department_in_str = "'" . implode("', '", $departments_in_group) . "'";
}else{
	$department_in_str = "'" . $department_id . "'";
}
	

if (isValidJobID($job_id))
{
	// show specific job
	if (($email_employer == "create") || ($email_employer == "Send"))
	{
		include('interview/sign_up/admin/email_employer.inc');
	}
	else
	{
		include('interview/sign_up/admin/interview.inc');
	}
}
else
{
	// show default list
	?>
	<h3>Interviews - Modify Sign-up</h3>
	<?php
	include('interview/sign_up/admin/interview_list.inc');
}

echo ("</td></tr></table>\n");


?>
