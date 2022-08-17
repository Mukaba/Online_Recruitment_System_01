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
 | Filename: change_parent_company.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file pulls all of the companies out of the database, and   |
 | then lets the user choose one of them as the new parent company for this     |
 | division.                                                                    |
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
    SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS order_letter
    FROM employer_company
    WHERE company_name IS NOT NULL
    AND company_name!=''
    AND company_display
    ORDER BY company_name
    ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    $order_choices[] = strtoupper($row["order_letter"]);
}

if (!$order)
{
    $order = $order_choices[0];
}

$order_href = ($PHP_SELF . "&amp;select=edit_contact&amp;level1=department&amp;continue=Change+Parent+Company&amp;department_id=" . $department_id);

$sql = ("
    SELECT DISTINCT employer_id, company_name
    FROM employer_company
    WHERE company_name IS NOT NULL
    AND company_name!=''
    AND company_name LIKE '" . $order . "%'
    AND company_display
    ORDER BY company_name
    ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_contact' />");
echo("<input type='hidden' name='level1' value='department' />");
echo("<input type='hidden' name='department_id' value='" . $department_id . "' />");
echo("<input type='hidden' name='order' value='" . $order . "' />");
echo("<input type='hidden' name='continue' value='Change Parent Company' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

echo("<tr>");
echo("<td colspan='2'>");
echo("<br />");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2'>");
echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
echo("<tr>");
echo("<td colspan='2' class='row0'>");
echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
echo("<tr>");
echo("<td colspan='2' align='center' nowrap='nowrap'>");
echo("Choose the new parent company:");
echo("</tr>");
echo("<tr>");
echo("<td colspan='2' align='center' nowrap='nowrap'>");
for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
{
    echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
}
echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");
echo("</td>");
echo("</tr>");
echo("<tr>");
echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td colspan='2'  align='center'>");
$size = 10;
if ($result->RecordCount() < 10)
{
    $size = $result->RecordCount();
}
if ($result->RecordCount() > 0)
{
    echo("<select name='employer_id' size='" . $size . "'>");
    while ($row = $result->FetchRow())
    {
        echo("<option value='" . $row["employer_id"] . "' ");
        if ($employer_id == $row["employer_id"])
        {
            echo("selected='selected'");
        }
        echo(">" . $row["company_name"] . "</option>");
    }
    echo("</select>");
}
else
{
    echo("<br />No companies starting with '" . $order . "'.<br /><br />");
}
echo("</td>");
echo("</tr>");
/*
:NOTE Put this back in if we want to allow the user the choice of what to do with contacts initially in the moved division.
if ($result->RecordCount())
{
    echo("<tr>");
    echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("Would you like contacts that are currently associated with this division to be");
    echo(" moved along with the division, or would you like them to remain associated with");
    echo(" this division's current parent company?");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td align='center'>");
    echo("<input type='radio' class='row0' name='move_contacts' value='department'");
    if ($move_contacts == 'department')
    {
        echo(" checked='checked'");
    }
    echo(" />Move with Division");
    echo("&nbsp;&nbsp;&nbsp;&nbsp;");
    echo("<input type='radio' class='row0' name='move_contacts' value='company'");
    if (!$move_contacts || $move_contacts == 'company')
    {
        echo(" checked='checked'");
    }
    echo(" />Stay with Company");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");
}
*/
echo("</table>");
echo("</td>");
echo("</tr>");
echo("<tr>");
echo("<td colspan='2' align='center'>");
echo("<input type='submit' name='continue' value='Save Parent Company' />");
echo("</tr>");
echo("</form>");
echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</center>");
