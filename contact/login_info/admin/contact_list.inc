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
 | Filename: contact_list.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file offers allows the user to choose from one of the      |
 | contacts in the database, and with which they can create/recreate a login    |
 | ID and password for them (and e-mail it out).                                |
 +------------------------------------------------------------------------------+

*/


/*
 First off, we need to find which letters to allow the user to click on across the
 screen.  If there's no companies in the database for the letter Q, there's no
 point in showing that as an option to click on.  Also, we want to make sure that
 we only pull out companies that have contacts associated with them, or with divisions
 of theirs.
*/

$sql = ("
    SELECT DISTINCT SUBSTRING(c.last_name, 1, 1) AS order_letter
    FROM contact AS c
    INNER JOIN contact_employer ce
    ON c.contact_id = ce.contact_id
    WHERE c.last_name IS NOT NULL
    AND c.last_name!=''
	AND c.last_name NOT LIKE '*%'
	AND c.type='employer'
    AND ce.deleted_flag = '0'
    ORDER BY c.last_name
    ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    if (!is_array($order_choices) || !in_array(strtoupper($row["order_letter"]), $order_choices))
    {
        $order_choices[] = strtoupper($row["order_letter"]);
    }
}

if ($order_choices)
{
	
	if (!$contact_order)
	{
	        $contact_order = $order_choices[0];
	}
	
	$order_href = ($PHP_SELF . "&amp;select=make_login_info");
	
	$sql = ("
        SELECT DISTINCT c.contact_id, CONCAT(c.last_name, ', ', c.first_name) AS name, ec.company_name
        FROM contact as c
        INNER JOIN contact_employer as ce
        ON c.contact_id = ce.contact_id
        INNER JOIN employer_company as ec
        ON ce.employer_id = ec.employer_id
        WHERE c.type='employer'
        AND c.last_name LIKE '" . addslashes($contact_order) . "%'
        AND ce.deleted_flag = '0'
        ORDER BY c.last_name, c.first_name
        ");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	
	echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
	echo("<input type='hidden' name='select' value='make_login_info' />");
	echo("<input type='hidden' name='contact_order' value='" . htmlspecialchars($contact_order,ENT_QUOTES) . "' />");
	
	echo("<center>");
	echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
	
	echo("<tr>");
	        echo("<td colspan='2'>");
	        echo("<br />");
	        echo("</td>");
	echo("</tr>");
	
	/*
	 Show them the list of contacts, and let them choose one.  Wee!
	*/
	
	echo("<tr>");
		echo("<td colspan='2'>");
		echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
		echo("<tr>");
			echo("<td align='center' colspan='2' class='row0'>");
			echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
			echo("<tr>");
				echo("<td align='center' colspan='2'>");
				echo("Below are listed all of the contacts in the database.  Please choose the one that you would like to 
					create/recreate a login ID and password for, and click 'Continue'.");
				echo("</td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td colspan='2'>&nbsp;</td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td colspan='2' align='center' nowrap='nowrap'>");
				for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
	            {
                    //part of process for converting from html to xhtml
                    if($order_choices[$i]=="'")
                    {
    				echo("<a class='blue' href='" . $order_href . "&amp;contact_order=" . $order_choices[$i] . ">" . $order_choices[$i] . "</a> | ");
                    }
                    else
                    {     
					echo("<a class='blue' href='" . $order_href . "&amp;contact_order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
				    }
                }
				echo("<a class='blue' href='" . $order_href . "&amp;contact_order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");
				echo("</td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td colspan='2'  align='center'>");
				$size = 10;
				if ($result->RecordCount() < 10)
				{
					$size = $result->RecordCount() + 1;
				}
				if ($result->RecordCount() > 0)
				{
					echo("<select name='contact_id' size='" . $size . "'>");
					echo("<option value='' ");
					if (!$contact_id || $contact_id == '')
					{
						echo("selected='selected'");
					}
					echo(">&nbsp;</option>");
					while ($row = $result->FetchRow())
					{
						echo("<option value='" . $row["contact_id"] . "' ");
						if ($contact_id == $row["contact_id"])
						{
							echo("selected='selected'");
						}
						echo(">" . $row["name"] . " -- (".$row['company_name'].")</option>");
					}
					echo("</select>");
				}
				else
				{
					echo("<br />No contacts starting with '" . $contact_order . "'.<br /><br />");
				}
				echo("</td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td colspan='2'>&nbsp;</td>");
			echo("</tr>");
			echo("<tr>");
				echo("<td colspan='2' align='center'>");
				echo("<input type='hidden' name='continue' value='contact_chosen' />");
				echo("<input type='submit' value='Continue' />");
				echo("</td>");
			echo("</tr>");
			echo("</table>");
			echo("</td>");
		echo("</tr>");
		echo("</table>");
		echo("</td>");
	echo("</tr>");
	echo("</table>");
	echo("</center>");
echo("</form>");

}
else
{
	notify("There are currently no contacts in the database.");
}

?>
