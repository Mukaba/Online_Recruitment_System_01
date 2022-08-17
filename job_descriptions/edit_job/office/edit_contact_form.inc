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
 | Filename: edit_contact_form.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file displays the current choices of company, division     |
 | and contact to the user, as well as any pertinent information for these      |
 | inputs. The user may change any of these options, but may only save the      |
 | inputs once a company and contact id have been chosen.                       |
 +------------------------------------------------------------------------------+

*/

?>

<script type='text/javascript' language='javascript'>

<!--javascript

function renewCompany()
{
    document.form1.java_reload.value = 'true';
    document.form1.unset_company.value = 'true';
    document.form1.submit();
}

function renewDepartment()
{
    document.form1.java_reload.value = 'true';
    document.form1.unset_department.value = 'true';
    document.form1.submit();
}

function renewContact()
{
    document.form1.java_reload.value = 'true';
    if (document.form1.contact_id.selectedIndex != 0)
    {
        document.form1.enable_save.value = 'true';
    }
    document.form1.unset_contact.value = 'true';
    document.form1.submit();
}

//-->

</script>

<?php

$order_href = ($PHP_SELF . "&amp;select=edit_job&amp;level1=contact&amp;job_id=" . $job_id . "&amp;unset_company=true&amp;unset_department=true&amp;unset_contact=true");

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
        ORDER BY company_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    $order_choices[] = strtoupper($row["order_letter"]);
}

if (!(isset($order)))
{
    if ($job->company_name)
    {
        $order = $job->company_name[0];
    }
    else
    {
        $order = $order_choices[0];
    }
}

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
echo("<input type='hidden' name='select' value='edit_job' />");
echo("<input type='hidden' name='order' value='" . $order . "' />");
echo("<input type='hidden' name='level1' value='contact' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");

/*
 The three inputs below are used by javascript to tell the server side PHP
 that it should unset the company, division, or contact variables before
 reloading the page.
 */

echo("<input type='hidden' name='unset_company' value='' />");
echo("<input type='hidden' name='unset_department' value='' />");
echo("<input type='hidden' name='unset_contact' value='' />");

/*
 The two inputs below are used to let the server side PHP
 know whether or not it should be showing a save button,
 and whether or not the page has been reloaded by java.
 */

echo("<input type='hidden' name='java_reload' value='' />");
echo("<input type='hidden' name='enable_save' value='' />");

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
 let them choose either 1) a division to add the contact to 2) to add the contact directly to the
 company or 3) to add a new division.
 */


if (!$job->employer_id)
{
    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
    echo("<tr>");
    echo("<td align='center' colspan='2' class='row0'>");
    echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
    echo("<tr>");
    echo("<td align='center' colspan='2' nowrap='nowrap'>Choose a company:</td>");
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
        echo("<select name='employer_id' onchange='renewCompany()' size='" . $size . "'>");
        echo("<option value='' ");
        if (!$job->employer_id || $job->employer_id == '')
        {
            echo("selected='selected'");
        }
        echo(">&amp;nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["employer_id"] . "' ");
            if ($job->employer_id == $row["employer_id"])
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

    /*
     Inputs below are used to keep javascript from complaining.
     */

    echo("<input type='hidden' name='company_website' value='' />");
    echo("<input type='hidden' name='company_description' value='' />");

  //  echo("</form>");
   // echo("</td>");
   // echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>-Or-<br /><br />");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<a href='" . $PHP_SELF . "&amp;select=edit_job&amp;add_this_object=employer&amp;continue=add_new_entity&amp;level1=contact&amp;job_id=" . $job_id . "'>");
    echo("<img src='misc/images/AddCompany.gif' border='0' alt='' />");
    echo("</a>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
}
elseif ($job->employer_id && !$job->department_id)
{
    /*
     Pull out the company website and description from the database, so we can display that
     to the user.
     */

    $sql = ("
            SELECT DISTINCT website, company_description
            FROM employer_company
            WHERE employer_id='" . $job->employer_id . "'
            ");
    $company_result = $GLOBALS['dbh']->Execute($sql);
    $company_row = $company_result->FetchRow();

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
    echo("<tr>");
    echo("<td align='center' colspan='2' class='row0'>");
    echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
    echo("<tr>");
    echo("<td align='center' colspan='2' nowrap='nowrap'>Company:</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center' nowrap='nowrap'>");
    for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
    {
        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
    }
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
        echo("<select name='employer_id' onchange='renewCompany()' size='" . $size . "'>");
        echo("<option value='' ");
        if (!$job->employer_id)
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["employer_id"] . "' ");
            if ($job->employer_id == $row["employer_id"])
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
    echo("<tr>");
    echo("<td align='right'>Company Website:</td>");
    echo("<td>");
    echo("<input type='text' name='company_website' size='35' maxlength='60' value='");
    echo((isset($job->company_website)) ? $job->company_website : $company_row["website"]);
    echo("' /></td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td valign='top' nowrap='nowrap' align='right'>Company Description:</td>");
    echo("<td>");
    echo("<textarea rows='8' cols='33' name='company_description'>");
    echo((isset($job->company_description)) ? $job->company_description : $company_row["company_description"]);
    echo("</textarea>");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>Choose a division:");
    echo("<br /><br />");
    echo("</td>");
    echo("</tr>");

    $sql = ("
            SELECT DISTINCT department_name, department_id
            FROM employer_department
            WHERE employer_id='" . $job->employer_id . "'
            ORDER BY department_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<select name='department_id' onchange='renewDepartment()'>");
    echo("<option value='' ");
    if (!$job->department_id)
    {
        echo("selected='selected'");
    }
    echo(">&nbsp;</option>");
    /*echo("<option value='none' ");
    if ($job->department_id == 'none')
    {
        echo("selected='selected'");
    }
    echo(">No Division</option>");*/
    while ($row = $result->FetchRow())
    {
        echo("<option value='" . $row["department_id"] . "' ");
        if ($job->department_id == $row["department_id"])
        {
            echo("selected='selected'");
        }
        echo(">" . $row["department_name"] . "</option>");
    }
    echo("</select>");

    /*
     Below inputs are to keep javascript from complaining.
     */

    echo("<input type='hidden' name='department_website' value='' />");
    echo("<input type='hidden' name='department_description' value='' />");
  //  echo("</form>");
  //  echo("</td>");
  //  echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>-Or-<br /><br />");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<a href='" . $PHP_SELF . "&amp;select=edit_job&amp;level1=contact&amp;add_this_object=department");
    echo("&amp;continue=add_new_entity&amp;employer_id=" . $job->employer_id . "&amp;job_id=" . $job_id . "'>");
    echo("<img src='misc/images/AddDepartment.gif' border='0' alt='' />");
    echo("</a>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
}
elseif ($job->employer_id && $job->department_id)
{

    /*
     Pull out the company and division website and description from the database, so we can display that
     to the user.
     */

    $sql = ("
            SELECT DISTINCT website, company_description
            FROM employer_company
            WHERE employer_id='" . $job->employer_id . "'
            ");
    $company_result = $GLOBALS['dbh']->Execute($sql);
    $company_row = $company_result->FetchRow();

    if ($job->department_id != 'none')
    {
        $sql = ("
                SELECT DISTINCT department_website, department_description
                FROM employer_department
                WHERE department_id='" . $job->department_id . "'
                ");
        $department_result = $GLOBALS['dbh']->Execute($sql);
        $department_row = $department_result->FetchRow();
    }

    if ($job->contact_id)
    {
        $sql = ("
                SELECT DISTINCT phone, cellphone, fax, email
                FROM contact
                WHERE contact_id='" . $job->contact_id . "'
                ");
        $contact_result = $GLOBALS['dbh']->Execute($sql);
        $contact_row = $contact_result->FetchRow();
    }

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
    echo("<tr>");
    echo("<td colspan='2' align='center' class='row0'>");
    echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
    echo("<tr>");
    echo("<td colspan='2' align='center' nowrap='nowrap'>Company:</td>");
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
        echo("<select name='employer_id' onchange='renewCompany()' size='" . $size . "'>");
        echo("<option value='' ");
        if (!$job->employer_id)
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["employer_id"] . "' ");
            if ($job->employer_id == $row["employer_id"])
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
    echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td align='right'>Company Website:</td>");
    echo("<td>");
    echo("<input type='text' name='company_website' size='35' maxlength='60' value='");
    echo((isset($job->company_website)) ? $job->company_website : $company_row["website"]);
    echo("' /></td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td valign='top' nowrap='nowrap' align='right'>Company Description:</td>");
    echo("<td>");
    echo("<textarea rows='8' cols='33' name='company_description'>");
    echo((isset($job->company_description)) ? $job->company_description : $company_row["company_description"]);
    echo("</textarea>");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");

    $sql = ("
            SELECT DISTINCT department_name, department_id
            FROM employer_department
            WHERE employer_id='" . $job->employer_id . "'
            ORDER BY department_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("Division:");
    echo("<br /><br />");
    echo("</td>"); 
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<select name='department_id' onchange='renewDepartment()'>");
    echo("<option value='' ");
    if (!$job->department_id)
    {
        echo("selected='selected'");
    }
    echo(">&nbsp;</option>");
    /*echo("<option value='none' ");
    if ($job->department_id == 'none')
    {
        echo("selected='selected'");
    }
    echo(">No Division</option>");*/
    while ($row = $result->FetchRow())
    {
        echo("<option value='" . $row["department_id"] . "' ");
        if ($job->department_id == $row["department_id"])
        {
            echo("selected='selected'");
        }
        echo(">" . $row["department_name"] . "</option>");
    }
    echo("</select>");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");
    if ($job->department_id && $job->department_id != 'none')
    {
        echo("<tr>");
        echo("<td align='right'>Division Website:</td>");
        echo("<td>");
        echo("<input type='text' name='department_website' size='35' maxlength='60' value='");
        echo((isset($job->department_website)) ? $job->department_website : $department_row["department_website"]);
        echo("' /></td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td valign='top' nowrap='nowrap' align='right'>Division Description:</td>");
        echo("<td>");
        echo("<textarea rows='8' cols='33' name='department_description'>");
        echo((isset($job->department_description)) ? $job->department_description : $department_row["department_description"]);
        echo("</textarea>");
        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td colspan='2' align='center'>&nbsp;</td>");
        echo("</tr>");
    }
    else
    {
        // The two below inputs are so that the javascript doesn't complain.
        echo("<input type='hidden' name='department_website' value='' />");
        echo("<input type='hidden' name='department_description' value='' />");
    }

    if ($job->department_id == 'none')
    {
        $sql = ("
                SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS name, c.contact_id
                FROM contact AS c
                INNER JOIN contact_employer AS ce
                ON c.contact_id=ce.contact_id
                LEFT JOIN employer_info_status_flags_join eisfj ON eisfj.contact_id = c.contact_id
                WHERE ce.employer_id='" . $job->employer_id . "'
                AND (eisfj.status_flag_id!='" . CONTACT_DO_NOT_CONTACT_FLAG . "' OR eisfj.status_flag_id IS NULL)
                AND ce.deleted_flag = '0'
                AND NOT (ce.department_id)
                ORDER BY c.last_name,c.first_name
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
    }
    else
    {
        $sql = ("
                SELECT DISTINCT CONCAT(c.first_name, ' ',  c.last_name) AS name, c.contact_id
                FROM contact AS c
                INNER JOIN contact_employer AS ce
                ON c.contact_id=ce.contact_id
                LEFT JOIN employer_info_status_flags_join eisfj ON eisfj.contact_id = c.contact_id
                WHERE ce.employer_id='" . $job->employer_id . "'	
                AND (eisfj.status_flag_id!='" . CONTACT_DO_NOT_CONTACT_FLAG . "' OR eisfj.status_flag_id IS NULL)
                AND ce.deleted_flag = '0'
                AND ce.department_id='" . $job->department_id . "'
                ORDER BY c.last_name,c.first_name
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
    }
    if ($result->RecordCount())
    {
        echo("<tr>");
        if (!$enable_save)
        {
            echo("<td colspan='2' align='center'>Choose a contact:");	
        }
        else
        {
            echo("<td colspan='2' align='center'>Contact:");
        }
        echo("<br /><br />");
        echo("</td>");
        echo("</tr>");
    }

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    if ($result->RecordCount())
    {
        echo("<select name='contact_id' onchange='renewContact()'>");
        echo("<option value='' ");
        if (!$job->contact_id)
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while($row = $result->FetchRow())
        {
            echo("<option value='" . $row["contact_id"] . "' ");
            if ($job->contact_id == $row["contact_id"])
            {
                echo("selected='selected'");
            }
            echo(">" . $row["name"] . "</option>");
        }
        echo("</select>");
    }
    else
    {
        echo("No existing contacts with this ");
        echo(($job->department_id == 'none') ? "company<br />(that aren't associated with a division)" : "division");
        echo(".<br /><br />");
    }
    echo("</td>");
    echo("</tr>");
    if (!$enable_save)
    {
        if ($result->RecordCount())
        {
            echo("<tr>");
            echo("<td colspan='2' align='center'>-Or-<br /><br />");
            echo("</td>");
            echo("</tr>");
        }
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<a href='" . $PHP_SELF . "&amp;select=edit_job&amp;level1=contact&amp;add_this_object=contact&amp;continue=add_new_entity&amp;level1=contact");
        echo("&amp;department_id=" . $job->department_id . "&amp;employer_id=" . $job->employer_id . "&amp;job_id=" . $job_id . "'>");
        echo("<img border='0' src='misc/images/AddContact.gif' alt='' />");
        echo("</a>");
        echo("</td>");
        echo("</tr>");
    }
    else
    {
        echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td align='right'>Contact's E-mail:</td>");
        echo("<td align='left'>");
        echo("<input type='text' name='contact_email' size='40' maxlength='100' value='");
        echo(($job->contact_email) ? $job->contact_email : $contact_row["email"]);
        echo("' /></td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td align='right'>Contact's Phone:</td>");
        echo("<td align='left'>");
        if ($contact_row["phone"] == USE_COMPANY)
        {
            $sql = ("
                    SELECT DISTINCT phone
                    FROM employer_company
                    WHERE employer_id='" . $job->employer_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $company_phone = $row["phone"];

            echo($company_phone . "&nbsp;&nbsp;<b class='black'>(Using Company Phone)</b>");
            echo("<input type='hidden' name='contact_phone' value='" . USE_COMPANY . "' />");
        }
        elseif ($contact_row["phone"] == USE_DEPARTMENT)
        {
            $sql = ("
                    SELECT DISTINCT ed.phone
                    FROM employer_department AS ed, contact_employer AS ce
                    WHERE ce.contact_id='" . $job->contact_id . "'
                    AND ed.department_id=ce.department_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $department_phone = $row["phone"];

            if ($department_phone == USE_COMPANY)
            {
                $sql = ("
                        SELECT DISTINCT phone
                        FROM employer_company
                        WHERE employer_id='" . $job->employer_id . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $department_phone = $row["phone"];
            }

            echo($department_phone . "&nbsp;&nbsp;<b class='black'>(Using Division Phone)</b>");
            echo("<input type='hidden' name='contact_phone' value='" . USE_DEPARTMENT . "' />");
        }
        else
        { 
            echo("<input type='text' name='contact_phone' size='22' maxlength='25' value='");
            echo(($job->contact_phone) ? $job->contact_phone : $contact_row["phone"]);
            echo("' />");
        }
        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td align='right'>Contact's Fax:</td>");
        echo("<td align='left'>");
        if ($contact_row["fax"] == USE_COMPANY)
        {
            $sql = ("
                    SELECT DISTINCT fax
                    FROM employer_company
                    WHERE employer_id='" . $job->employer_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $company_fax = $row["fax"];

            echo($company_fax . "&nbsp;&nbsp;<b class='black'>(Using Company Fax)</b>");
            echo("<input type='hidden' name='contact_fax' value='" . USE_COMPANY . "' />");
        }
        elseif ($contact_row["fax"] == USE_DEPARTMENT)
        {
            $sql = ("
                    SELECT DISTINCT ed.fax
                    FROM employer_department AS ed, contact_employer AS ce
                    WHERE ce.contact_id='" . $job->contact_id . "'
                    AND ed.department_id=ce.department_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $department_fax = $row["fax"];

            if ($department_fax == USE_COMPANY)
            {
                $sql = ("
                        SELECT DISTINCT fax
                        FROM employer_company 
                        WHERE employer_id='" . $job->employer_id . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $department_fax = $row["fax"];
            }

            echo($department_fax . "&nbsp;&nbsp;<b class='black'>(Using Division Fax)</b>");
            echo("<input type='hidden' name='contact_fax' value='" . USE_DEPARTMENT . "' />");
        }
        else
        {
            echo("<input type='text' name='contact_fax' size='22' maxlength='25' value='");
            echo(($job->contact_fax) ? $job->contact_fax : $contact_row["fax"]);
            echo("' />");
        }
        echo("</td>");
        echo("</tr>");
            echo("<tr>");
            echo("<td align='right'>Contact's Cellphone:</td>");
            echo("<td align='left'>");
            echo("<input type='text' name='contact_cellphone' size='22' maxlength='25' value='");
            echo(($job->contact_cellphone) ? $job->contact_cellphone : $contact_row["cellphone"]);
            echo("' />");
            echo("</td>");
            echo("</tr>"); 
    }
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    if ($enable_save)
    {
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<input type='submit' name='continue' value='Save Changes' />");
      //  echo("</form>");
      //  echo("</td>");
      //  echo("</tr>");
    }
}	

echo("</table>");
echo("</center>");
echo("</form>");

?>
