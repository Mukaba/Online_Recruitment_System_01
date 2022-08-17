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
 | Filename: change_company_department.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file displays all of the companies to the user, allowing   |
 | them to choose one.  Once they have chosen a company, the page will reload,  |
 | and the user will have the choice of choosing one of the divisions with      |
 | this company or to set the contact to be associated directly with the        |
 | company.                                                                     |
 +------------------------------------------------------------------------------+

*/


?>

<script type='text/javascript' language='javascript'>
<!--javascript

function renew()
{
    document.form1.department_id.selectedIndex = 0;
    document.form1.submit();
}

function renew2()
{
    if (document.form1.department_id.selectedIndex != 0)
    {
        document.form1.enable_save.value='true';
    }
    document.form1.submit();
}

//-->
</script>

<?php

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

if (!(isset($enable_save)))
{
    $enable_save = 'true';
}

$order_href = ($PHP_SELF . "&amp;select=edit_contact&amp;level1=contact&amp;enable_save=&amp;continue=Change+Parent+Company/Division&amp;contact_id=" . $contact_id . "&amp;old_employer_id=" . $old_employer_id);

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
echo("<input type='hidden' name='level1' value='contact' />");
echo("<input type='hidden' name='order' value='" . $order . "' />");
echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
echo("<input type='hidden' name='department_id' value='" . $department_id . "' />");
echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
echo("<input type='hidden' name='enable_save' value='' />");
echo("<input type='hidden' name='continue' value='Change Parent Company/Division' />");
echo("<input type='hidden' name='old_employer_id' value='" . $old_employer_id . "' />");
echo("<input type='hidden' name='old_contact_id' value='" . $old_contact_id . "' />");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

echo("<tr>");
echo("<td colspan='2'>");
echo("<br />");
echo("</td>");
echo("</tr>");

/*
 First things first.  If they have not yet chosen a company, then give them a list of companies,
 and the option to add a new company.  Once they select a company, reload the page, and this time,
 let them choose a division.
 */

if (!$employer_id)
{
    echo("<tr>");
    echo("<td align='center' colspan='2' nowrap='nowrap'>Choose a company:</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
    echo("<tr>");
    echo("<td colspan='2' class='row0'>");
    echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
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
    echo("<td colspan='2'  align='center'>");
    $size = 10;
    if ($result->RecordCount() < 10)
    {
        $size = $result->RecordCount() + 1;
    }
    if ($result->RecordCount() > 0)
    {
        echo("<select name='employer_id' onchange='renew()' size='" . $size . "'>");
        echo("<option value='' ");
        if (!$employer_id || $employer_id == '')
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
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

    // Hidden input below is to keep javascript from complaining.
    echo("<input type='hidden' name='department_id' value='' />");

   //   echo("</form>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
}
else
{
    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
    echo("<tr>");
    echo("<td colspan='2' class='row0'>");
    echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>Company:</td>");
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
    echo("<td colspan='2'  align='center'>");
    $size = 10;
    if ($result->RecordCount() < 10)
    {
        $size = $result->RecordCount() + 1;
    }
    if ($result->RecordCount() > 0)
    {
        echo("<select name='employer_id' onchange='renew()' size='" . $size . "'>");
        echo("<option value='' ");
        if (!$employer_id || $employer_id == '')
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
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
    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");

    $sql = ("
            SELECT DISTINCT department_name, department_id
            FROM employer_department
            WHERE employer_id='" . $employer_id . "'
            ORDER BY department_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    echo("<tr>");
    echo("<td colspan='2' align='center'>Choose a division:");
    echo("<br /><br />");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<select name='department_id' onchange='renew2()'>");
    echo("<option value='' ");
    if (!$department_id)
    {
        echo("selected='selected'");
    }
    echo(">&nbsp;</option>");
    while ($row = $result->FetchRow())
    {
        echo("<option value='" . $row["department_id"] . "' ");
        if ($department_id == $row["department_id"])
        {
            echo("selected='selected'");
        }
        echo(">" . $row["department_name"] . "</option>");
    }
    echo("</select>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    if ($enable_save)
    {
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<input type='submit' name='continue' value='Save Parent Company/Division' />");
        echo("</tr>");
    }
   // echo("</form>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
}

echo("</table>");
echo("</center>");
echo("</form>");
?>
