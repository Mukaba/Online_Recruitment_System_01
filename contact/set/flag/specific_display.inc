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
 | Filename: specific_display.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This form is used to let the user edit multiple flags for a     |
 | specific contact.                                                            |
 +------------------------------------------------------------------------------+

*/


echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='set_contact_flag' />");
echo("<input type='hidden' name='level1' value='flags' />");
echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
echo("<input type='hidden' name='flag_changes_array' value='" . packObject(($flag_changes_array)) . "' />");
echo("<input type='hidden' name='initial_flags_set' value='" . packObject(($initial_flags_set)) . "' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

$sql = ("
	SELECT DISTINCT CONCAT(first_name, ' ', last_name) AS name
	FROM contact
	WHERE contact_id='" . $contact_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$contact_name = $row["name"];

$sql = ("
	SELECT DISTINCT a.flag_id
	FROM contact_flags_set AS a, department_contact_flags AS b
	WHERE b.department_id='" . $auth->department . "'
	AND a.contact_id='" . $contact_id . "'
	AND a.flag_id=b.flag_id
	ORDER BY a.flag_id
	");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
	$dep_flags_set[] = $row["flag_id"];
}

$sql = ("
	SELECT DISTINCT b.flag_id, b.flag_name
	FROM department_contact_flags AS a, contact_flags AS b
	WHERE a.department_id='" . $auth->department . "'
	AND b.flag_id=a.flag_id
	ORDER BY b.flag_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");   
	echo("<td colspan='2' align='center'>Contact Flags for: " . $contact_name . "</td>");
echo("</tr>\n");
        
echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<table cellpadding='5' cellspacing='0' border='0' class='row0' align='center'>\n");
	$newrow = 0;
	while ($row = $result->FetchRow())
	{
		$department_contact_flags[] = $row["flag_id"];

		if (!($newrow % 2))
		{
			echo("<tr>");
		}
		echo("<td nowrap='nowrap'><input type='checkbox' class='row2' name='contact_flags[]' value='" . $row["flag_id"] . "' ");
		if (is_array($dep_flags_set) && in_array($row["flag_id"], $dep_flags_set))
		{
			echo("checked='checked'");
		}
		echo(" />" . $row["flag_name"]);
		echo($flags_changed["flags_changed"] ? "<br /><b class='black'>(" . (in_array($row["flag_id"], $flags_changed["flags_set"]) ? "On" : "Off") . ")</b>" : "");
		echo("</td>");
		if ($newrow % 2)
		{
			echo("</tr>\n");
		}
		$newrow++;
	}

	// Fill in the remaining columns (<td>'s).

	while ($newrow % 2)
	{
		echo("<td>&nbsp;</td>");
		$newrow++;
	}
	//echo("</tr>");

	echo("</table>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'><hr />");
	echo("<input type='hidden' name='department_contact_flags' value='" . packObject(($department_contact_flags)) . "' />");
	echo("<input type='hidden' name='continue' value='save_specific' />");
    //:TODO: We need a back to list button. This is annoying because a user cannot return to his/her search results. 
	echo("<input type='submit' value='Save Changes' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");
echo("</form>");

?>

	
