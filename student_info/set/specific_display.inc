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
 | specific student.                                                            |
 +------------------------------------------------------------------------------+

 :TODO: At some point we need to allow staff to set eligible flags for certain 
        term/years, just like general_display.inc
*/
echo("<h3>Set Student Flags</h3>");

//Use record id as tsudent number is obsolete

$sql = ("
        SELECT DISTINCT record_id
        FROM student_department
        WHERE student_number='" . $student_num . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($record_row = $result->FetchRow())
{
    $record_id = $record_row["record_id"];
}

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='set_student_flags' />");
echo("<input type='hidden' name='checked_flags' value='".$checked_flags." />");
echo("<input type='hidden' name='level1' value='flags' />");
echo("<input type='hidden' name='student_num' value='" . $student_num . "' />");
echo("<input type='hidden' name='record_id' value='" . $record_id . "' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

$student_name = getStudentName($student_num);

//Use record id as tsudent number is obsolete

$sql = ("
        SELECT DISTINCT record_id
        FROM student_department
        WHERE student_number='" . $student_num . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($record_row = $result->FetchRow())
{
    $record_id = $record_row["record_id"];
}


$sql = ("
	SELECT DISTINCT sfj.student_flags_id
	FROM student_flags_join AS sfj, department_student_flags AS dsf
	WHERE dsf.department_id='" . $auth->department . "'
	AND sfj.record_id='" . $record_id . "'
	AND sfj.student_flags_id=dsf.student_flags_id
	ORDER BY dsf.student_flags_id
	");

$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
	$dep_flags_set[] = $row["student_flags_id"];
}

$sql = ("
	SELECT DISTINCT sf.student_flags_id, sf.description
	FROM department_student_flags AS dsf, student_flags AS sf
	WHERE dsf.department_id='" . $auth->department . "'
	AND dsf.student_flags_id=sf.student_flags_id
    AND sf.student_flags_id != '".ELIGIBLE_FLAG."' AND sf.student_flags_id != '".PLACED_FLAG."'
	ORDER BY sf.description
	");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");   
	echo("<td colspan='2' align='center'>Student Flags for: " . $student_name . "</td>");
echo("</tr>\n");
        
echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<table cellpadding='5' cellspacing='0' border='0' class='row0' align='center'>\n");
	$newrow = 0;
	while ($row = $result->FetchRow())
	{
		$department_contact_flags[] = $row["student_flags_id"];

		if (!($newrow % 2))
		{
			echo("<tr>");
		}
		echo("<td nowrap='nowrap'><input type='checkbox' class='row2' name='contact_flags[]' value='" . $row["student_flags_id"] . "' ");
		if (is_array($dep_flags_set) && in_array($row["student_flags_id"], $dep_flags_set))
		{
			echo("checked='checked'");
		}
		echo(" />" . $row["description"] . "</td>");
		if ($newrow % 2)
		{
			echo("</tr>\n");
		}
		$newrow++;
	}

	/*
	 Fill in the remaining columns (<td>'s).
	*/

	while ($newrow % 2)
	{
		echo("<td>&nbsp;</td>");
		$newrow++;
	}
	echo("</tr>");

	echo("</table>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<input type='hidden' name='department_contact_flags' value='" . packObject($department_contact_flags) . "' />");
	echo("<input type='hidden' name='continue' value='save_specific' />");
	echo("<input type='hidden' name='student_ids' value='$student_ids' />");
	echo("<input type='hidden' name='student_id_list' value='$student_id_list' />");
	echo("<input type='hidden' name='form_submitted' value='1' />");
	echo("<input type='submit' value='Save Changes' />");
	echo("&nbsp;&nbsp;<input type='reset' value='Reset' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</form>");

?>

	
