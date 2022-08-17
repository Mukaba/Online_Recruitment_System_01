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
 | Filename: merge_company_list.inc                                             |
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
    SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS order_letter
    FROM employer_company
    WHERE company_name IS NOT NULL
    AND company_name!=''
    AND company_display
    AND employer_id!='" . $employer_id . "'
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

$order_href = $PHP_SELF . "&amp;select=edit_contact&amp;level1=company&amp;employer_id=" . $employer_id . "&amp;continue=merge_company_with_another";

$sql = ("
    SELECT DISTINCT employer_id, company_name
    FROM employer_company
    WHERE company_name IS NOT NULL
    AND company_name!=''
    AND company_name LIKE '" . $order . "%'
    AND company_display
    AND employer_id!='" . $employer_id . "'
    ORDER BY company_name
    ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($order_choices)
{

    echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
    echo("<input type='hidden' name='select' value='edit_contact' />");
    echo("<input type='hidden' name='level1' value='company' />");
    echo("<input type='hidden' name='order' value='" . $order . "' />");
    echo("<input type='hidden' name='first_employer_id' value='" . $employer_id . "' />");
    echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
    echo("<input type='hidden' name='continue' value='merge_companies_chosen' />");

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

    echo("<tr>");
    echo("<td align='center' colspan='2' nowrap='nowrap'>");
    echo("Please choose the company that you would like to merge with <b>" . getCompanyName($employer_id) . "</b>.");
    echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>\n");

    echo("<tr>");
    echo("<td colspan='2' align='center' nowrap='nowrap'>");
    for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
    {
        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
    }
    echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");
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
        echo("<select name='second_employer_id' size='" . $size . "'>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["employer_id"] . "' ");
            if ($second_employer_id == $row["employer_id"])
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

}
else
{
    // There are no companies in the database, so let the user know this.
    notify("There are currently no companies in the database (and thus, none that you can merge with this one).");
}

?>
