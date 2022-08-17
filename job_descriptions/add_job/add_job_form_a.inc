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
 | Filename: add_job_form_a.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file displays all of the companies to the user, allowing   |
 | them to choose one.  Once they have chosen a company, the page will reload,  |
 | and the user will have the choice of selecting to add the contact to the     |
 | company, or of choosing one of the divisions with this company to add the    |
 | contact to.   After having chosen a division or none at all, the user may    |
 | finally select a contact for that division.                                  |
 +------------------------------------------------------------------------------+

*/

?>

<script type="text/javascript" language="javascript">
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

function add_new_company()
{
    document.form1.add_this_object = 'employer';
    document.form1.submit();
}

//-->

</script>

<?php

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

if ($order) {
    $order = stripslashes($order);
}
elseif (!$order)
{
    if ($employer_id)
    {
        $sql = ("
                SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS company_letter
                FROM employer_company
                WHERE employer_id='" . $employer_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $order = $row["company_letter"];
    }
    else
    {
        $order = $order_choices[0];
    }
}

$order_href = $PHP_SELF . "&amp;select=add_job";

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
echo("<table border='0' cellpadding='0' cellspacing='0' width='97%' class='row1'>");

echo("<tr>");
echo("<td colspan='2'>");
echo("<input type='hidden' name='select' value='add_job' />");
echo("<input type='hidden' name='order' value='" . $order . "' />");
echo("<input type='hidden' name='add_this_object' value='' />");

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


echo("<br />");
echo("</td>");
echo("</tr>");

/*
 First things first.  If they have not yet chosen a company, then give them a list of companies,
 and the option to add a new company.  Once they select a company, reload the page, and this time,
 let them choose either 1) a division to add the contact to 2) to add the contact directly to the
 company or 3) to add a new division.
 */

if (!$employer_id)
{
    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
    echo("<tr>");
    echo("<td align='center' colspan='2' class='row0'>");
    echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
    echo("<tr>");
    echo("<td align='center' colspan='2' nowrap='nowrap'><b class='black'>Choose a company to add the new job to:</b></td>");
    echo("</tr>");

    if ($order_choices)
    {
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

        /*
         Inputs below are used to keep javascript from complaining.
         */

        echo("<input type='hidden' name='company_website' value='' />");
        echo("<input type='hidden' name='company_description' value='' />");

        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td colspan='2' align='center'>-Or-<br /><br />");
        echo("</td>");
        echo("</tr>");
    }
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<input type='hidden' name='continue' value='add_new_entity' />");
    echo("<input type='hidden' name='add_this_object' value='employer' />");
    echo("<input type='submit' value='Add a New Company' />");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
   // echo("</form>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
}
elseif ($employer_id && !$department_id)
{
    /*
     Pull out the company website and description from the database, so we can display that
     to the user.
     */

    $sql = ("
            SELECT DISTINCT website, company_description
            FROM employer_company
            WHERE employer_id='" . $employer_id . "'
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
        if (!$employer_id)
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
    echo("<tr>");
    echo("<td align='right'>Company Website:</td>");
    echo("<td>");
    echo("<input type='text' name='company_website' size='35' maxlength='60' value='");
    echo((isset($company_website)) ? $company_website : $company_row["website"]);
    echo("' /></td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td valign='top' nowrap='nowrap' align='right'>Company Description:</td>");
    echo("<td>");
    echo("<textarea rows='8' cols='33' name='company_description'>");
    echo((isset($company_description)) ? $company_description : $company_row["company_description"]);
    echo("</textarea>");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'><b class=black>Choose a division to add the job to:</b>");
    echo("<br /><br />");
    echo("</td>");
    echo("</tr>");

    $sql = ("
            SELECT DISTINCT department_name, department_id
            FROM employer_department
            WHERE employer_id='" . $employer_id . "'
            ORDER BY department_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<select name='department_id' onchange='renewDepartment()'>");
    echo("<option value='' ");
    if (!$department_id)
    {
        echo("selected='selected'");
    }
    echo(">&nbsp;</option>");
    /*echo("<option value='none' ");
      if ($department_id == 'none')
      {
      echo("selected='selected'");
      }
      echo(">No Division</option>");*/
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

    /*
     Below inputs are to keep javascript from complaining.
     */

    echo("<input type='hidden' name='department_website' value='' />");
    echo("<input type='hidden' name='department_description' value='' />");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>-Or-<br /><br />");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<input type='hidden' name='continue' value='add_new_entity' />");
    echo("<input type='hidden' name='add_this_object' value='department' />");
    echo("<input type='hidden' name='employer_id=' value='" . $employer_id . "' />");
    echo("<input type='submit' value='Add a New Division' />");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
  //  echo("</form>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
}
elseif ($employer_id && $department_id)
{

    /*
     Pull out the company and division website and description from the database, so we can display that
     to the user.
     */

    $sql = ("
            SELECT DISTINCT website, company_description
            FROM employer_company
            WHERE employer_id='" . $employer_id . "'
            ");
    $company_result = $GLOBALS['dbh']->Execute($sql);
    $company_row = $company_result->FetchRow();

    if ($department_id != 'none')
    {
        $sql = ("
                SELECT DISTINCT department_website, department_description
                FROM employer_department
                WHERE department_id='" . $department_id . "'
                ");
        $department_result = $GLOBALS['dbh']->Execute($sql);
        $department_row = $department_result->FetchRow();
    }

    if ($contact_id)
    {
        $sql = ("
                SELECT DISTINCT cellphone, phone, fax, email
                FROM contact
                WHERE contact_id='" . $contact_id . "'
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
        if (!$employer_id)
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
    echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td align='right'>Company Website:</td>");
    echo("<td>");
    echo("<input type='text' name='company_website' size='35' maxlength='60' value='");
    echo((isset($company_website)) ? $company_website : $company_row["website"]);
    echo("' /></td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td valign='top' nowrap='nowrap' align='right'>Company Description:</td>");
    echo("<td>");
    echo("<textarea rows='8' cols='33' name='company_description'>");
    echo((isset($company_description)) ? $company_description : $company_row["company_description"]);
    echo("</textarea>");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");

    $sql = ("
            SELECT DISTINCT department_name, department_id
            FROM employer_department
            WHERE employer_id='" . $employer_id . "'
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
    if (!$department_id)
    {
        echo("selected='selected'");
    }
    echo(">&nbsp;</option>");
    /*echo("<option value='none' ");
      if ($department_id == 'none')
      {
      echo("selected='selected'");
      }
      echo(">No Division</option>");*/
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
    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");
    if ($department_id && $department_id != 'none')
    {
        echo("<tr>");
        echo("<td align='right'>Division Website:</td>");
        echo("<td>");
        echo("<input type='text' name='department_website' size='35' maxlength='60' value='");
        echo((isset($department_website)) ? $department_website : $department_row["department_website"]);
        echo("' /></td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td valign='top' nowrap='nowrap' align='right'>Division Description:</td>");
        echo("<td>");
        echo("<textarea rows='8' cols='33' name='department_description'>");
        echo((isset($department_description)) ? $department_description : $department_row["department_description"]);
        echo("</textarea>");
        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td colspan='2' align='center'>&nbsp;</td>");
        echo("</tr>");
    }
    else
    {
        /*
         The two below inputs are so that the javascript doesn't complain.
         */
        echo("<input type='hidden' name='department_website' value='' />");
        echo("<input type='hidden' name='department_description' value='' />");
    }

    if ($department_id == 'none')
    {
        $sql = ("
                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.contact_id
                FROM contact AS a, contact_employer AS b
                WHERE b.employer_id='" . $employer_id . "'
                AND NOT (b.department_id)
                AND a.contact_id=b.contact_id
                ORDER BY a.last_name,a.first_name
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
    }
    elseif ($department_id != 'none')
    {
        $sql = ("
                SELECT DISTINCT CONCAT(a.first_name, ' ',  a.last_name) AS name, a.contact_id
                FROM contact AS  a, contact_employer AS b
                WHERE b.employer_id='" . $employer_id . "'	
                AND b.department_id='" . $department_id . "'
                AND a.contact_id=b.contact_id
                ORDER BY a.last_name,a.first_name
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
    }
    if ($result->RecordCount())
    {
        echo("<tr>");
        if (!$enable_save)
        {
            echo("<td colspan='2' align='center'><b class=black>Choose a contact for this job:</b>");	
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
        if (!$contact_id)
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while($row = $result->FetchRow())
        {
            echo("<option value='" . $row["contact_id"] . "' ");
            if ($contact_id == $row["contact_id"])
            {
                echo("selected='selected'");
            }
            echo(">" . $row["name"] . "</option>");
        }
        echo("</select>");
    }
    else
    {
        echo("<b class=black>No existing contacts with this ");
        echo(($department_id == 'none') ? "company<br />(that aren't associated with a division)" : "division");
        echo(".</b><br /><br />");
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
        // Only show this button if the user has chosen a division.
        if ($department_id && $department_id != 'none')
        {
            echo("<tr>");
            echo("<td colspan='2' align='center'>");
            echo("<input type='hidden' name='continue' value='add_new_entity' />");
            echo("<input type='hidden' name='add_this_object' value='contact' />");
            echo("<input type='submit' value='Add a New Contact' />");
            echo("</td>");
            echo("</tr>");
        }
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
        echo(($contact_email) ? $contact_email : $contact_row["email"]);
        echo("' /></td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td align='right'>Contact's Phone:</td>");
        echo("<td align='left'>");
        if ($contact_row["phone"] == USE_COMPANY)
        {
            $sql = ("
                    SELECT DISTINCT ec.phone
                    FROM employer_company AS ec, contact_employer AS ce
                    WHERE ce.contact_id='" . $contact_id . "'
                    AND ec.employer_id=ce.employer_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $company_phone = $row["phone"];

            echo($company_phone . "&nbsp;<b class='black'>(Using Company Phone)</b>");
            echo("<input type='hidden' name='contact_phone' value='" . USE_COMPANY . "' />");
        }
        elseif ($contact_row["phone"] == USE_DEPARTMENT)
        {
            $sql = ("
                    SELECT DISTINCT ed.phone
                    FROM employer_department AS ed, contact_employer AS ce
                    WHERE ce.contact_id='" . $contact_id . "'
                    AND ed.department_id=ce.department_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $department_phone = $row["phone"];

            if ($department_phone == USE_COMPANY)
            {
                $sql = ("
                        SELECT DISTINCT ec.phone
                        FROM employer_company AS ec, contact_employer AS ce
                        WHERE ce.contact_id='" . $contact_id . "'
                        AND ec.employer_id=ce.employer_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                $department_phone = $row["phone"];
            }

            echo($department_phone . "&nbsp;<b class='black'>(Using Division Phone)</b>");
            echo("<input type='hidden' name='contact_phone' value='" . USE_DEPARTMENT . "' />");
        }
        else
        {
            echo("<input type='text' name='contact_phone' size='22' maxlength='25' value='");
            echo(($contact_phone) ? $contact_phone : $contact_row["phone"]);
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
                    SELECT DISTINCT ec.fax
                    FROM employer_company AS ec, contact_employer AS ce
                    WHERE ce.contact_id='" . $contact_id . "'
                    AND ec.employer_id=ce.employer_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $company_fax = $row["fax"];

            echo($company_fax . "&nbsp;<b class='black'>(Using Company Fax)</b>");
            echo("<input type='hidden' name='contact_fax' value='" . USE_COMPANY . "' />");
        }
        elseif ($contact_row["fax"] == USE_DEPARTMENT)
        {
            $sql = ("
                    SELECT DISTINCT ed.fax
                    FROM employer_department AS ed, contact_employer AS ce
                    WHERE ce.contact_id='" . $contact_id . "'
                    AND ed.department_id=ce.department_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $department_fax = $row["fax"];

            if ($department_fax == USE_COMPANY)
            {
                $sql = ("
                        SELECT DISTINCT ec.fax
                        FROM employer_company AS ec, contact_employer AS ce
                        WHERE ce.contact_id='" . $contact_id . "'
                        AND ec.employer_id=ce.employer_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                $department_fax = $row["fax"];
            }

            echo($department_fax . "&nbsp;<b class='black'>(Using Division Fax)</b>");
            echo("<input type='hidden' name='contact_fax' value='" . USE_DEPARTMENT . "' />");
        }
        else
        {
            echo("<input type='text' name='contact_fax' size='22' maxlength='25' value='");
            echo(($contact_fax) ? $contact_fax : $contact_row["fax"]);
            echo("' />");
        }
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td align='right'>Contact's Cellphone:</td>");
        echo("<td align='left'>");
        echo("<input type='text' name='contact_cellphone' size='40' maxlength='25' value='");
        echo( ($contact_cellphone) ? $contact_cellphone : $contact_row["cellphone"] ); 
        echo("' />");
        echo("</td>");
        echo("</tr>");
    }
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    //this is a fix to prevent "Add job for contact" (QM) from erasing but allowing contact info to still be updated 
    echo("<input type='hidden' name='visited_job_form_a' value='1' />");
    echo("</td>");
    echo("</tr>");
        if ($enable_save)
    {
        echo("<tr>");
        echo("<td colspan='2' align='center'>");

        echo("<form name='form3' method='post' action='".$PHP_SELF."'>");
        echo("<input type='hidden' name='continue' value='' />\n");

        // We need to check and see if the employer chosen actually has jobs in the system.
        $sql =  ("
            SELECT DISTINCT ji.job_id
            FROM job_info AS ji
            LEFT JOIN contact_employer AS ce 
                ON ji.employer_department_id = ce.department_id
            WHERE ji.employer_id = '" . $employer_id . "'
            AND ce.contact_id = '".$contact_id."'
            AND ji.year >= '".(date("Y") - 2)."'
            AND ji.position_title != ''
            AND ji.position_title IS NOT NULL
            AND ji.status IN ('". CLOSED ."', '". CANCELLED ."')
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount()) {
            unset($order);
            echo("<input type='button' value='Repost Old Job' onclick = 'javascript:cont_submit(\"show_admin_templates\")' />\n");
            echo("&nbsp;\n");	
        }

        echo("<input type='button' value='Start From Scratch' onclick = 'javacript:cont_submit(\"level1\")' />\n");
     //   echo("</form>\n");
        echo("</td>\n");
        echo("</tr>");
    }
}	

echo("</table>");
echo("</form>");

?>

<script type="text/javascript" language="javascript">
function cont_submit(cont){
    var j;
    for(var i = 0; i < document.form1.elements.length; i++)
    {
        var e = document.form1.elements[i]
            if (e.name == "continue")
            {
                j = i;
            }
    }

    document.form1[j].value = cont;
    document.form1.submit();
}

</script>
