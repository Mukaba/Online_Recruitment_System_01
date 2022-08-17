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
 | Filename: merge_contact_list.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This file pulls all of the companies out of the database, and   |
 | then lets the user choose one of them to merge with the first one they have  |
 | chosen.                                                                      |
 +------------------------------------------------------------------------------+

*/


/*
 First off, we need to find which letters to allow the user to click on across the
 screen.  If there's no companies in the database for the letter Q, there's no
 point in showing that as an option to click on.
*/

$sql = ("
    SELECT DISTINCT SUBSTRING(last_name, 1, 1) AS order_letter
    FROM contact
    WHERE last_name IS NOT NULL
    AND last_name!=''
    AND type='employer'
    AND contact_id!='" . $contact_id . "'
    ORDER BY last_name
    ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
        $order_choices[] = strtoupper($row["order_letter"]);
}

if (!$contact_order)
{
        $contact_name = getContactName($contact_id);
        $contact_order = substr($contact_name["last_name"], 0, 1);
}

$order_href = ($PHP_SELF . "&amp;select=edit_contact&amp;level1=contact&amp;contact_id=" . $contact_id . "&amp;continue=merge_contact_with_another");

$sql = ("
        SELECT DISTINCT c.contact_id, ce.employer_id, CONCAT(c.last_name, ', ', c.first_name) AS name
        FROM contact AS c, contact_employer AS ce
        WHERE c.type='employer'
        AND c.contact_id=ce.contact_id
        AND c.last_name LIKE '" . addslashes($contact_order) . "%'
        AND c.contact_id!='" . $contact_id . "'
        ORDER BY last_name, first_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_contact' />");
echo("<input type='hidden' name='level1' value='contact' />");
echo("<input type='hidden' name='contact_order' value='" . htmlspecialchars($contact_order, ENT_QUOTES) . "' />");
echo("<input type='hidden' name='first_contact_id' value='" . $contact_id . "' />");
echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
echo("<input type='hidden' name='continue' value='merge_contacts_chosen' />");

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

		$contact_name = getContactName($contact_id);
        $sql2 = ("
            SELECT DISTINCT employer_id
            FROM contact_employer
            WHERE contact_id='" . $contact_id . "'
            ");
        $result2 =$GLOBALS['dbh']->Execute($sql2);
        $row = $result2->FetchRow();

		echo("<tr>");
			echo("<td align='center' colspan='2' nowrap='nowrap'>");
			echo("Please choose the contact that you would like to merge with <b>" . $contact_name["first_name"] . " " . $contact_name["last_name"] . "</b>");
            echo(" working at <b>" . getCompanyName($row["employer_id"]) . "</b>.");
			echo("</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2' align='center' nowrap='nowrap'>");
			for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
			{
				echo("<a class='blue' href='" . $order_href . "&amp;contact_order=" . htmlspecialchars($order_choices[$i], ENT_QUOTES) . "'>" . $order_choices[$i] . "</a> | ");
			}
			echo("<a class='blue' href='" . $order_href . "&amp;contact_order=" . htmlspecialchars($order_choices[$i], ENT_QUOTES) . "'>" . $order_choices[$i] . "</a>");
			echo("</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'  align='center'>");
			$size = 10;
			if ($result->RecordCount() < 10)
			{
				$size = $result->RecordCount() + 1;
			}
			if ($result->RecordCount() > 0)
			{
				echo("<select name='second_contact_id' size='" . $size . "'>");
				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["contact_id"] . "' ");
					if ($second_contact_id == $row["contact_id"])
					{
						echo("selected='selected'");
					}
					echo(">" . $row["name"] . " - (" . getCompanyName($row["employer_id"]) . ")</option>");
				}
				echo("</select>");
			}
			else
			{
				echo("<br />No contacts starting with '" . $contact_order . "'.<br /><br />");
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

//	echo("</form>");

	echo("</table>");
	echo("</td>");
echo("</tr>\n");
echo("</table>");
echo("</center>");
	echo("</form>");

?>
