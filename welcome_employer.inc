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
 | Filename: welcome_employer.inc                                               |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: welcome.inc
// show the welcome screen for employers
?>

 </td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>     

<table border="0" cellspacing="0" cellpadding="20" width="100%"><tr><td>
<br /><br />
<h2 align="center">Co-op at <?php echo(ORGANIZATION_FULL) ?></h2>
<h4 align="center" class="bodytext">
<?php
if ($userlevel == ADMINSTUDENT)
{
	$sql = "SELECT CONCAT(a.first_name, ' ', a.last_name) AS name 
		FROM student as a
		WHERE a.student_number='$student_number'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	echo $row["name"] . "<br />";
}
elseif ($userlevel == ADMINEMPLOYER)
{
	$sql = "SELECT CONCAT(a.first_name, ' ', a.last_name) AS name 
		FROM contact as a
		WHERE a.contact_id='$employer_contact_id_login'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	echo $row["name"] . "<br />";
	$sql = "SELECT e.company_name
		FROM employer_company as e, employer_login as l
		WHERE e.employer_id=l.employer_id AND l.contact_id = '" . $employer_contact_id_login . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	echo $row["company_name"] . "<br />";
}
else
{
   if ($userlevel == EMPLOYER){
	$sql = "SELECT e.company_name
		FROM employer_company as e, employer_login as l
		WHERE e.employer_id=l.employer_id AND l.contact_id = '" . $auth->contact_id . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	echo $row["company_name"] . "<br />";
   }
   echo($auth->name); 
   if ($userlevel == OFFICE) {
	$sql = "SELECT department_name FROM department
		WHERE department_id='$auth->department'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	echo("<br />".$row["department_name"]);
   }
}
?>
</h4>
<br />
<p align="center">Welcome to <?php echo(BRAND_NAME); ?>, the information system for the Co-op programs at <?php echo(ORGANIZATION_FULL) ?>.</p>
<br />

<p align='left'>If this is your <b>first time logging in</b>, we recommend that you change your password 
to something you can easily remember. To do so, click "Change Password" from the menu on the left.</p> 

<p>For some <b>quick tips</b> on how to navigate through the system, click "Getting Started".</p>

<p align='left'>To view <b>resumes and cover letters</b>, click "Student Applications".</p>
<br />
<h4 align="center">Please select an option from the menu on the left</h4>
</td></tr></table>
