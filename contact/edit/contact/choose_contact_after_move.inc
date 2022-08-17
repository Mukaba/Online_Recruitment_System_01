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
 | Filename: choose_contact_after_move.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file pulls all of the contacts out of the database for     |
 | the given employer, and then lets the user choose one of them to be the new  |
 | contact for the moved contact's associated jobs.                             |
 +------------------------------------------------------------------------------+

*/


$original_contact_name = getContactName($contact_id);

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_contact' />");
echo("<input type='hidden' name='level1' value='contact' />");
echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
echo("<input type='hidden' name='department_id' value='" . $department_id . "' />");
echo("<input type='hidden' name='continue' value='choose_contact_after_move' />");
echo("<input type='hidden' name='old_employer_id' value='" . $old_employer_id . "' />");
echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

echo("<tr>");
	echo("<td colspan='2'>");
	echo("<br />");
	echo("</td>");
echo("</tr>\n");

echo("<tr>");
	echo("<td colspan='2'>");
	echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
	echo("<tr>");
		echo("<td colspan='2' class='row0'>");
		echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");

        $sql = ("
            SELECT DISTINCT c.contact_id, CONCAT(c.last_name, ', ', c.first_name) AS name
            FROM contact AS c
            INNER JOIN contact_employer AS ce 
            ON ce.contact_id=c.contact_id
            WHERE c.type='employer'
            AND ce.employer_id='" . $old_employer_id . "'
            AND ce.deleted_flag = '0'
            AND c.contact_id!='" . $contact_id . "'
            ORDER BY c.last_name
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

		echo("<tr>");
			echo("<td align='center' colspan='2' nowrap='nowrap'>");
			echo("Please choose the new contact to associate with <b>" . $original_contact_name["first_name"]);
			echo(" " . $original_contact_name["last_name"] . "'s</b> old jobs.");
			echo("</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			$size = 10;
			if ($result->RecordCount() < 10)
			{
				$size = $result->RecordCount() + 1;
			}
			if ($result->RecordCount() > 0)
			{
				echo("<select name='job_contact_id' size='" . $size . "'>");
				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["contact_id"] . "' ");
					if ($job_contact_id == $row["contact_id"])
					{
						echo("selected='selected'");
					}
					echo(">" . $row["name"] . "</option>");
				}
				echo("</select>");
			}
			echo("</td>");
		echo("</tr>\n");
		echo("</table>");
		echo("</td>");
	echo("</tr>\n");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("<input type='submit' value='Continue' />");
		echo("</td>");
	echo("</tr>\n");

	echo("</form>");

	echo("</table>");
	echo("</td>");
echo("</tr>\n");
echo("</table>");
echo("</center>");
?>
