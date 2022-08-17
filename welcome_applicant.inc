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
 | Filename: welcome_applicant.inc                                              |
 +------------------------------------------------------------------------------+
 | Description:  show the welcome screen to applicants                          |
 +------------------------------------------------------------------------------+

*/

?>

 </td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>     

<table border="0" cellspacing="0" cellpadding="20" width="100%"><tr><td>
<br /><br />
<h2 align="center">Co-op at <?php echo(ORGANIZATION_FULL) ?></h2>
<br />
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
<p align="center">Welcome to <?php echo(BRAND_NAME); ?>, the information system for the Co-op programs at <?php echo (ORGANIZATION_FULL) ?>.</p>
<br />
<?php

?>
<p><i><b>Hints to get you started:</b></i></p>
<p>To apply for a job, first view the job description.  If you are eligible to apply, an "Apply to Job" button will appear at the bottom of the screen 
below the "View in PDF button".  Clicking this "Apply to Job" button will allow you to create a specific cover letter for that job (or select a generic one). Please 
note that the button will not appear if the job requires paper applications.  If you think you should be able to apply to an online job but do not see the "Apply to Job" 
button, please contact your Co-op Office.</p>
<p>Resumes are shown to employers in PDF format.  Your resume is entered in HTML, but it is converted to PDF within our system.  Please check that your resume 
converts to PDF without any major problems.  Some resumes with non-standard table definitions appear a bit odd, but the majority of the resumes look normal. We are 
currently developing a new resume system based on input we have received from students and employers.  The new system will not require any HTML.  It will be available 
later this Fall.</p>

<?php
echo("<h5>All information contained in and accessed through the ".BRAND_NAME." Information 
        System is privileged and confidential. Further unauthorized distribution is prohibited and is contrary to 
        ".ORGANIZATION_CODE." Computing And Telecommunications Policy 6030 
        <a href='http://web.uvic.ca/uvic-policies/pol-6000/6030CTUR.html' target='_blank'> http://web.uvic.ca/uvic-policies/pol-6000/6030CTUR.html</a>
        </h5>");
?>

<h4 align="center"><b>Please select an option from the menu bar</b></h4>
</td></tr></table>
