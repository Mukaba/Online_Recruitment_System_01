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
 | Filename: department_choices.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This file is called when a user logs in and is given the        |
 | userlevel "MULTIPLE". This means that the user has multiple departments and  |
 | needs to choose one to log in under. Let the fun begin.                      |
 +------------------------------------------------------------------------------+

*/


?>
</td><td align='left'><img src='<?php print $GLOBALS['colorscheme']['topbar_right']; ?>' alt='' /></td></tr></table>

<table border="0" cellspacing="0" cellpadding="20" width="100%"><tr><td>
<br /><br />
<h2 align="center">Co-op at <?php echo(ORGANIZATION_FULL) ?></h2>
<br />
<br />
<p align='center'>Welcome to <?php echo(BRAND_NAME); ?>, the information system for the Co-op programs at <?php echo(ORGANIZATION_FULL) ?>.</p>

<br />

<p align='center'>
You are registered as belonging to multiple departments in our database.  Please choose the department
you would like to log in under from the drop down menu below.
</p>

<?php

if ($_SESSION['SESSION_multiple_type'] == "office")
{
	$sql = ("
		SELECT DISTINCT a.department_id, a.department_name
		FROM department AS a, contact_internal AS b
		WHERE (b.netlink_id='" . $_SESSION['SESSION_loginID'] . "' OR b.login_id='" . $_SESSION['SESSION_loginID'] . "')
		AND a.department_id=b.department_id
		ORDER BY a.department_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	assert($result->RecordCount() > 1);

	echo("<center>");
	echo("<form method='post' action='mamook.php'>");
	echo("<select name='department_choice'>");

	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["department_id"] . "' ");
		if ($row["department_id"] == $department_choice)
		{
			echo("selected='selected'");
		}
		echo(">" . $row["department_name"] . "</option>");
	}

	echo("</select>");

	echo("<br />");
	echo("<br />");

	echo("<input type='hidden' name='userlevel' value='" . $auth->userlevel . "' />");
	echo("<input type='submit' value='Continue' />");
	echo("</form>");
	echo("</center>");
}
elseif ($_SESSION['SESSION_multiple_type'] == "student")
{
	$sql = ("
		SELECT DISTINCT s.student_number, sd.department_id, d.department_name
		FROM student AS s, student_department AS sd, department AS d
		WHERE (s.netlink_id='" . addslashes(stripslashes($_SESSION['SESSION_loginID'])) . "' || s.login_id='" . addslashes(stripslashes($_SESSION['SESSION_loginID'])) . "')
		AND s.student_number=sd.student_number
		AND d.department_id=sd.department_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	assert($result->RecordCount() > 1);

	echo("<center>");
	echo("<form method='POST' action='mamook.php'>");
	echo("<select name='department_choice'>");

	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["department_id"] . "' ");
		if ($row["department_id"] == $department_choice)
		{
			echo("selected='selected'");
		}
		echo(">" . $row["department_name"] . "</option>");
	}

	echo("</select>");

	echo("<br />");
	echo("<br />");

	echo("<input type='hidden' name='userlevel' value='" . $auth->userlevel . "' />");
	echo("<input type='submit' value='Continue' />");
	echo("</form>");
	echo("</center>");
}

echo("</td>");
echo("</tr>");
echo("</table>");

?>
