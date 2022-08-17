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
 | Description: This file displays the form that shows the user the contact's   |
 | current inputs and allows them to make any changes needed.  The forms        |
 | inputs are then passed back to edit_contact.inc                              |
 +------------------------------------------------------------------------------+

*/


$max_country_id=0;
$country_array = array();

// We need to put blank values at the start of every pull down array
$max_sql = ("
    SELECT DISTINCT provstate_id
    FROM provstate_list
    ORDER BY provstate_id DESC
    ");
$max_result = $GLOBALS['dbh']->Execute($max_sql);
$max_row = $max_result->FetchRow();

for ($i = 1; $i <= $max_row["provstate_id"]; $i++)
{
    $prov_array[$i] = ("Array(\"NULL\"),");
}

$result = $GLOBALS['dbh']->Execute("SELECT c.country_id, provstate_id, provstate_name
        FROM country_list as c LEFT JOIN provstate_list as p ON c.country_id = p.country_id
        ORDER BY c.order_by");
while ($row = $result->FetchRow())
{
    if ($row["provstate_id"] == "")
    {
        // If there's already a NULL value in this slot, don't put another one in.
        if ($prov_array[$row["country_id"]] == "Array(\"NULL\"),")
        {
            $str = "";
        }
        else
        {
            $str = "Array(\"NULL\"),";
        }
    }
    else
    {
        $str = "Array(\"" . $row["provstate_id"] . "\", \"" . $row["provstate_name"] . "\"),";
    }
    $prov_array[$row["country_id"]] .= $str;
    if (!in_array($row["country_id"], $country_array))
    {
        $country_array[] =$row["country_id"];
    }
    if ($row["country_id"] > $max_country_id)
    {
        $max_country_id = $row["country_id"];
    }
}
sort($country_array);

$str = "Array(";
for($i=0;$i<=$max_country_id;$i++)
{
    if ($prov_array[$i] == "")
    {
        $str .= "Array(\"NULL\"),";
    }
    else
    {
        $str .= "Array(" . substr($prov_array[$i], 0, strlen($prov_array[$i])-1) . "),";
    }
}
$str = substr($str, 0, strlen($str)-1) . ")";
$prov_state_array = $str;

$max_prov_id = 0;

// We need to put blank values at the start of every pull down array
$max_sql = ("
        SELECT DISTINCT region_id
        FROM region_list
        ORDER BY region_id DESC
        ");
$max_result = $GLOBALS['dbh']->Execute($max_sql);
$max_row = $max_result->FetchRow();

for ($i = 1; $i <= $max_row["region_id"]; $i++)
{
    $region_array[$i] = ("Array(\"NULL\"),");
}

$result = $GLOBALS['dbh']->Execute("SELECT p.provstate_id, r.region_id, r.region_name
        FROM provstate_list as p LEFT JOIN region_list as r ON p.provstate_id = r.provstate_id
        ORDER BY p.provstate_id");
while ($row = $result->FetchRow()){
    if ($row["region_id"] == "")
    {
        // If there's already a NULL value in this slot, don't put another one in.
        if ($region_array[$row["provstate_id"]] == "Array(\"NULL\"),")
        {
            $str = "";
        }
        else
        {
            $str = "Array(\"NULL\"),";
        }
    }
    else
    {
        $str = "Array(\"" . $row["region_id"] . "\", \"" . $row["region_name"] . "\"),";
    }
    $region_array[$row["provstate_id"]] .= $str;

    if ($row["provstate_id"] > $max_prov_id)
    {
        $max_prov_id = $row["provstate_id"];
    }
}
$str = "Array(";
for($i=0;$i<=$max_prov_id;$i++)
{
    if ($region_array[$i] == "")
    {
        $str .= "Array(\"NULL\"),";
    }
    else
    {
        $str .= "Array(" . substr($region_array[$i], 0, strlen($region_array[$i]) - 1) . "),";
    }
}
$str = substr($str, 0, strlen($str)-1) . ")";
$region_array = $str;

$max_region_id = 0;
$sql = ("
        SELECT region_id, city_name
        FROM region_list
        ORDER BY region_id
        ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    if ($row["city_name"] == "")
    {
        $str = "";
    }
    else
    {
        $str = $row["city_name"];
    }

    $city_array[$row["region_id"]] = $str;

    if ($row["region_id"] > $max_region_id)
    {
        $max_region_id = $row["region_id"];
    }
}

$str = "Array(";
for ($i = 0; $i <= $max_region_id; $i++)
{
    if ($city_array[$i] == "")
    {
        $str .= "\"\",";
    }
    else
    {
        $str .= "\"" . $city_array[$i] . "\",";
    }
}

$str = substr($str, 0, (strlen($str) - 1)) . ")";
$city_array = $str;

?>
<script type='text/javascript' language='javascript'>
var city_array = new <?php echo($city_array); ?>;
var region_array = new <?php echo $region_array?>;
var prov_array = new <?php echo $prov_state_array?>;
var cntry_array = new <?php echo $country_array?>;

function update_prov()
{
    index = document.form1.contact_country_id[document.form1.contact_country_id.selectedIndex].value;
    prov = prov_array[index];
    if (document.form1.contact_country_id[document.form1.contact_country_id.selectedIndex].value == "")
    {
        message = "Choose a Country";
        document.form1.contact_provstate_id.options.length = 1;
        document.form1.contact_provstate_id.options[0] = new Option(message, "");
        document.form1.contact_provstate_id.selectedIndex = 0;
        document.form1.contact_region_id.options.length = 1;
        document.form1.contact_region_id.options[0] = new Option(message, "");
        document.form1.contact_region_id.selectedIndex = 0;
    }
    else
    {
        if (prov.length==1 && prov[0] == "NULL")
        {
            document.form1.contact_provstate_id.options.length = 1;
            document.form1.contact_provstate_id.options[0] = new Option("N/A", "");
            document.form1.contact_provstate_id.selectedIndex = 0;
            document.form1.contact_region_id.options.length=1;
            document.form1.contact_region_id.options[0] = new Option("N/A", "");
            document.form1.contact_region_id.selectedIndex = 0;

        }
        else
        {
            document.form1.contact_provstate_id.options.length =0;
            document.form1.contact_provstate_id.options.length = prov.length;
            for($i=0;$i<prov.length;$i++)
            {
                if (prov[$i] == "NULL")
                {
                    document.form1.contact_provstate_id.options[$i] = new Option(" ", "");
                }
                else
                {
                    document.form1.contact_provstate_id.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                }
            }
            document.form1.contact_provstate_id.selectedIndex = 0;

            index2=document.form1.contact_provstate_id[document.form1.contact_provstate_id.selectedIndex].value;
            region = region_array[index2];
            if (document.form1.contact_provstate_id[document.form1.contact_provstate_id.selectedIndex].value == "")
            {
                if (document.form1.contact_country_id[document.form1.contact_country_id.selectedIndex].value == "")
                {
                    message = "Choose a Country";
                }
                else
                {
                    message = "Choose a Province/State";
                }
                document.form1.contact_region_id.options.length = 1;
                document.form1.contact_region_id.options[0] = new Option(message, "");
                document.form1.contact_region_id.selectedIndex = 0;
            }
            else
            {
                if (region.length==1 && region[0] == "NULL"){
                    document.form1.contact_region_id.options.length=1;
                    document.form1.contact_region_id.options[0] = new Option("N/A", "");
                }else{
                    document.form1.contact_region_id.options.length=0;
                    document.form1.contact_region_id.options.length=region.length;
                    for($i=0;$i<region.length;$i++){
                        document.form1.contact_region_id.options[$i] = new Option(region[$i][1], region[$i][0]);
                    }
                }
            }
        }
    }
}

function update_region()
{
    document.form1.contact_region_id.options.length=0;
    index2=document.form1.contact_provstate_id[document.form1.contact_provstate_id.selectedIndex].value;
    region = region_array[index2];
    if (document.form1.contact_provstate_id[document.form1.contact_provstate_id.selectedIndex].value == "")
    {
        if (document.form1.contact_country_id[document.form1.contact_country_id.selectedIndex].value == "")
        {
            message = "Choose a Country";
        }
        else
        {
            message = "Choose a Province/State";
        }

        document.form1.contact_region_id.options.length = 1;
        document.form1.contact_region_id.options[0] = new Option(message, "");
        document.form1.contact_region_id.selectedIndex = 0;
    }
    else
    {
        if (region.length==1 && region[0] == "NULL")
        {
            document.form1.contact_region_id.options.length=1;
            document.form1.contact_region_id.options[0] = new Option("N/A", "");
        }
        else
        {
            document.form1.contact_region_id.options.length = region.length;
            for($i=0;$i<region.length;$i++)
            {
                if (region[$i] == "NULL")
                {
                    document.form1.contact_region_id.options[$i] = new Option(" ", "");
                }
                else
                {
                    document.form1.contact_region_id.options[$i] = new Option(region[$i][1], region[$i][0]);
                }
            }
        }
        document.form1.contact_region_id.selectedIndex = 0;
    }
}

function update_city()
{
    index3 = document.form1.contact_region_id[document.form1.contact_region_id.selectedIndex].value;

    if (index3)
    {
        city = city_array[index3];
        document.form1.contact_city.value = city;
    }
}

</script>

<?php

if (!$employer_id)
{
    $employer_id = $contact->contact_employer_id;
}
if (!$department_id)
{
    $department_id = $contact->contact_department_id;
}

if ($userlevel == TRAFFICDIRECTOR)
{
    echo("<form method='post' action='" . $PHP_SELF . "' name='delete_form'>");
    echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    echo("<input type='hidden' name='select' value='verify_delete_contact' />");
    echo("</form>");
}

echo("<form method='post' action='" . $URL . "' name='form1'>");
echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
echo("<tr>");
echo("<td align='center' colspan='2'>&nbsp;");
echo("<input type='hidden' name='level1' value='contact' />");
echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
echo("<input type='hidden' name='department_id' value='" . $department_id . "' />");
echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
echo("<input type='hidden' name='check_changes' value='" . packObject($check_changes) . "' />");
echo("<input type='hidden' name='changes_array' value='" . packObject(($changes_array)) . "' />");
echo("</td>");
$sql = ("
        SELECT DISTINCT company_name
        FROM employer_company
        WHERE employer_id='" . $employer_id . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$company_name = $row["company_name"];

if ($department_id && $department_id != 'none')
{
    $sql = ("
            SELECT DISTINCT department_name
            FROM employer_department
            WHERE department_id='" . $department_id . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $department_name = $row["department_name"];
}

?>


</tr>

<tr>
<td align='center' colspan='2'><b class='black'>Parent Company:</b>&nbsp;
<b class='instruct'><?php echo($company_name); ?></b>
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . getCompanyName($changes_array["contact_employer_id"]) . ")</b>" : ""); ?>
</td>
</tr>

<?php

if ($department_name)
{
    echo("<tr>");
    echo("<td align='center' colspan='2'><b class='black'>Parent Division:</b>&nbsp;");
    echo("<b class='instruct'>".$department_name."</b>");
    echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . getDivisionName($changes_array["contact_department_id"]) . ")</b>" : "");
    echo("</td>");
    echo("</tr>");
}

// Let them merge or move this contact only if there's no current or future jobs associated with the contact, or the contact is not the only one under the company.
// get rid of deleted flag
$sql = ("
        SELECT DISTINCT ce.contact_id
        FROM contact_employer AS ce, contact_employer AS ce2
        LEFT JOIN employer_info_status_flags_join eisfj ON eisfj.contact_id = ce.contact_id
        WHERE ce2.contact_id='" . $contact_id . "'
        AND ce.contact_id!='" . $contact_id . "'
        AND ce.deleted_flag = '0'
        AND ce2.employer_id=ce.employer_id
        AND (eisfj.status_flag_id NOT IN ('".CONTACT_DO_NOT_CONTACT_FLAG."') OR eisfj.status_flag_id IS NULL)
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$other_contacts = $result->RecordCount();

echo("<tr>");
echo("<td align='center' colspan='2'>");
if ($other_contacts || !getJobsWithContact($contact_id))
{
    echo("<input type='submit' name='continue' value='Change Parent Company/Division' />");
    echo("&nbsp;&nbsp;");
    echo("<input type='submit' name='continue' value='Merge Contact with Another' />");

    if ($userlevel == TRAFFICDIRECTOR)
    {
        echo("&nbsp;&nbsp;");
        echo("<input type='button' name='delete_button' value='Delete Contact' onclick='javascript:document.delete_form.submit()' />");
    }
}
else
{
    echo("<b class='black'>This contact has at least one present or future job associated with it, and no other contacts at this company to");
    echo(" which the job(s) can be assigned to.  Therefore, this contact cannot be moved or merged until there is at least one more contact at the company");
    echo(", or until all of the jobs associated with it have closed and passed.");
}
echo("</td>");
echo("</tr>");

?>

<tr>
    <td colspan='2'>&nbsp;</td>
</tr>

<tr>
    <td align='right'>Contact Status:</td>
    <td align='left'>
    <?php
    echo(getEmployerStatusName($contact->contact_status_id));
    ?>
    </td>
</tr>

<tr>
<td align='right'>Contact's Title:</td>
<td align='left'>
<?php

$sql = ("
        SELECT DISTINCT title_id, title_name
        FROM title
        ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<select name='contact_title'>");
while ($row = $result->FetchRow())
{
    echo("<option value='" . $row["title_id"] . "' ");
    if ($contact->contact_title == $row["title_id"])
    {
        echo("selected='selected'");
    }
    echo(">" . $row["title_name"] . "</option>");
}
echo("</select>");
echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . getTitleName($changes_array["contact_title"]) . ")</b>" : "");
?>
</td>
</tr>

<tr>
<td align='right'>Contact's first name: <b class='red'>*</b></td>
<td align='left'><input type='text' name='contact_first_name' maxlength='40' size='22' value="<?php echo($contact->contact_first_name); ?>" />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_first_name"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's last name: <b class='red'>*</b></td>
<td align='left'><input type='text' name='contact_last_name' maxlength='40' size='22' value="<?php echo($contact->contact_last_name); ?>" />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_last_name"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's middle name:</td>
<td align='left'><input type='text' name='contact_middle_name' maxlength='40' size='22' value="<?php echo($contact->contact_middle_name); ?>" />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_middle_name"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's preferred name:</td>
<td align='left'><input type='text' name='contact_called_name' maxlength='40' size='22' value="<?php echo($contact->contact_called_name); ?>" />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_called_name"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Dear...:</td>
<td align='left'><input type='text' name='contact_greeting' maxlength='75' size='40' value="<?php echo($contact->contact_greeting); ?>" />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_greeting"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's e-mail address:</td>
<td align='left'><input type='text' name='contact_email' maxlength='100' size='40' value='<?php echo($contact->contact_email); ?>' />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_email"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's phone number:</td>
<td align='left' nowrap='nowrap'>

<?php

/*
 Get the division and company phone numbers from the database.
 */

$sql = ("
        SELECT DISTINCT phone
        FROM employer_company
        WHERE employer_id='" . $employer_id . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$company_phone = $row["phone"];

$sql = ("
        SELECT DISTINCT phone
        FROM employer_department
        WHERE department_id='" . $department_id . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$department_phone = $row["phone"];

if ($department_phone == USE_COMPANY)
{
    $department_phone = $company_phone;
}

if (trim($contact->contact_phone == USE_COMPANY))
{
    echo($company_phone);
    echo("&nbsp;&nbsp;(Company phone)&nbsp;&nbsp;");
    echo("<input type='submit' name='continue' value='Specify contact phone' />");
    if ($department_phone)
    {
        echo("<input type='submit' name='continue' value='Use Division Phone' />");
    }
    echo("<input type='hidden' name='contact_phone' value='" . USE_COMPANY . "' />");
}
elseif (trim($contact->contact_phone == USE_DEPARTMENT))
{
    echo($department_phone);
    echo("&nbsp;&nbsp;(Division phone)&nbsp;&nbsp;");
    echo("<input type='submit' name='continue' value='Specify contact phone' />");
    if ($company_phone)
    {
        echo("<input type='submit' name='continue' value='Use Company Phone' />");
    }
    echo("<input type='hidden' name='contact_phone' value='" . USE_DEPARTMENT . "' />");
}
else
{
    echo("<input type='text' name='contact_phone' size='22' maxlength='25' value='" . $contact->contact_phone . "' />");
    if ($company_phone || $department_phone)
    {
        echo("&nbsp;&nbsp;Or&nbsp;&nbsp;");
    }
    if ($company_phone)
    {
        echo("<input type='submit' name='continue' value='Use Company Phone' />");
    }
    if ($department_phone)
    {
        echo("<input type='submit' name='continue' value='Use Division Phone' />");
    }
}

echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_phone"] . ")</b>" : "");
?>
</td>
</tr>

<tr>
<td align='right'>Contact's fax number:</td>
<td align='left'>

<?php

// Get the division and company fax numbers from the database.

$sql = ("
        SELECT DISTINCT fax
        FROM employer_company
        WHERE employer_id='" . $employer_id . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$company_fax = $row["fax"];

$sql = ("
        SELECT DISTINCT fax
        FROM employer_department
        WHERE department_id='" . $department_id . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$department_fax = $row["fax"];

if ($department_fax == USE_COMPANY)
{
    $department_fax = $company_fax;
}

if (trim($contact->contact_fax == USE_COMPANY))
{
    echo($company_fax);
    echo("&nbsp;&nbsp;(Company fax)&nbsp;&nbsp;");
    echo("<input type='submit' name='continue' value='Specify contact fax' />");
    if ($department_fax)
    {
        echo("<input type='submit' name='continue' value='Use Division Fax' />");
    }
    echo("<input type='hidden' name='contact_fax' value='" . USE_COMPANY . "' />");
}
elseif (trim($contact->contact_fax == USE_DEPARTMENT))
{
    echo($department_fax);
    echo("&nbsp;&nbsp;(Division fax)&nbsp;&nbsp;");
    echo("<input type='submit' name='continue' value='Specify contact fax' />");
    if ($company_fax)
    {
        echo("<input type='submit' name='continue' value='Use Company Fax' />");
    }
    echo("<input type='hidden' name='contact_fax' value='" . USE_DEPARTMENT . "' />");
}
else
{
    echo("<input type='text' name='contact_fax' size='22' maxlength='25' value='" . $contact->contact_fax . "' />");
    if ($company_fax || $department_fax)
    {
        echo("&nbsp;&nbsp;Or&nbsp;&nbsp;");
    }
    if ($company_fax)
    {
        echo("<input type='submit' name='continue' value='Use Company Fax' />");
    }
    if ($department_fax)
    {
        echo("<input type='submit' name='continue' value='Use Division Fax' />");
    }
}

echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_fax"] . ")</b>" : "");
?>

</td>
</tr>

<tr>
<td align='right'>Contact's pager number:</td>
<td align='left'><input type='text' name='contact_pager' size='22' maxlength='25' value='<?php echo($contact->contact_pager); ?>' />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_pager"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's cellphone number:</td>
<td align='left'><input type='text' name='contact_cellphone' size='22' maxlength='25' value='<?php echo($contact->contact_cellphone); ?>' />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_cellphone"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's position title:</td>
<td align='left'><input type='text' name='contact_position_title' size='40' maxlength='100' value='<?php echo($contact->contact_position_title); ?>' />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_position_title"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact's department (if applicable):</td>
<td align='left'><input type='text' name='contact_department_name' size='40' maxlength='150' value='<?php echo($contact->contact_department_name); ?>' />
<?php echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . $changes_array["contact_department_name"] . ")</b>" : ""); ?>
</td>
</tr>

<tr>
<td align='right'>Contact Has Login ID?:</td>
<?php
$sql1 = ("
        SELECT employer_id
        FROM employer_login
        WHERE contact_id='" . $contact_id . "'
        ");
$result1 = $GLOBALS['dbh']->Execute($sql1);
if($result1->RecordCount() != 0)
{
    echo("<td width='50%' align='left'>Yes</td>");
}else{
    echo("<td width='50%' align='left'>No</td>");
}
echo("</tr>");


if($result1->RecordCount() != 0){
    $sql = ("
            SELECT last_datetime_in
            FROM login_info
            WHERE contact_id = '".$contact_id."'");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        echo("<tr align='center'>");
        echo("<td align='right' width='50%' nowrap='nowrap'>Last Login:</td>");
        echo("<td width='50%' align='left'>" . date("F d, Y",strtotime($row["last_datetime_in"])) . "</td>");
        echo("</tr>");
    }
}
?>

<tr>
<td colspan='2'>&nbsp;</td>
</tr>

<tr>
<td align='center' colspan='2'>Contact Location:

<?php

$sql = ("
        SELECT DISTINCT street_address_1, street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code
        FROM employer_company 
        WHERE employer_id='" . $employer_id . "'
        ");
$company_result = $GLOBALS['dbh']->Execute($sql);
$company_row = $company_result->FetchRow();

$sql = ("
        SELECT DISTINCT street_address_1, street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code, location_info
        FROM employer_department
        WHERE department_id='" . $department_id . "'
        ");
$department_result = $GLOBALS['dbh']->Execute($sql);
$department_row = $department_result->FetchRow();

if ($department_row["location_info"] == USE_COMPANY)
{
    $department_row = $company_result->FetchRow();
}       

/* 
   If there is at least one of the location fields filled out, then set the variable
   to let us know that this is the case.
 */

$department_info_valid = ($department_row["street_address_1"] || $department_row["city"] || $department_row["region_id"]
        || $department_row["provstate_id"] || $department_row["country_id"] || $department_row["postal_code"]);

$company_info_valid = ($company_row["street_address_1"] || $company_row["city"] || $company_row["region_id"]
        || $company_row["provstate_id"] || $company_row["country_id"] || $company_row["postal_code"]);

if ($contact->contact_location_info)
{
    echo("&nbsp;<input type='submit' name='continue' value='Specify contact location' />");
}
if ($contact->contact_location_info != USE_COMPANY && $company_info_valid)
{
    echo("&nbsp;<input type='submit' name='continue' value='Use Company Location' />");
}
if ($contact->contact_location_info != USE_DEPARTMENT && $department_info_valid)
{
    echo("&nbsp;<input type='submit' name='continue' value='Use Division Location' />");
}

echo("<input type='hidden' name='contact_location_info' value='" . $contact->contact_location_info . "' />");

?>

</td>
</tr>

<tr>
<td colspan='2' align='center'>
<table cellpadding='5' cellspacing='0' border='0' class='row0'>

<?php

if ($contact->contact_location_info == USE_COMPANY)
{
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<b class='black'>Using Company's Location</b>");
    echo("</td>");
    echo("</tr>");
}
elseif ($contact->contact_location_info == USE_DEPARTMENT)
{
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<b class='black'>Using Division's Location</b>");
    echo("</td>");
    echo("</tr>");
}

if (($contact->contact_location_info == USE_COMPANY && $company_row["street_address_1"]) 
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["street_address_1"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>Street Address 1:</td>");
    echo("<td align='left'>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["street_address_1"]) ? $company_row["street_address_1"] : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["street_address_1"]) ? $department_row["street_address_1"] : "&nbsp;");
    }
    else
    {
        echo("<input type='text' name='contact_street_address_1' size='45' maxlength='75' value=\"" . $contact->contact_street_address_1 . "\" />");
    }

    echo("</td>");
    echo("</tr>");

}

if (($contact->contact_location_info == USE_COMPANY && $company_row["street_address_2"])
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["street_address_2"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>Street Address 2 (optional):</td>");
    echo("<td align='left'>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["street_address_2"]) ? $company_row["street_address_2"] : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["street_address_2"]) ? $department_row["street_address_2"] : "&nbsp;");
    }
    else
    {
        echo("<input type='text' name='contact_street_address_2' size='45' maxlength='75' value=\"" . $contact->contact_street_address_2 . "\" />");
    }

    echo("</td>");
    echo("</tr>");

}

if (($contact->contact_location_info == USE_COMPANY && $company_row["street_address_3"])
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["street_address_3"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>Street Address 3 (optional):</td>");
    echo("<td align='left'>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["street_address_3"]) ? $company_row["street_address_3"] : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["street_address_3"]) ? $department_row["street_address_3"] : "&nbsp;");
    }
    else
    {
        echo("<input type='text' name='contact_street_address_3' size='45' maxlength='75' value=\"" . $contact->contact_street_address_3 . "\" />");
    }

    echo("</td>");
    echo("</tr>");

}

if (($contact->contact_location_info == USE_COMPANY && $company_row["country_id"])
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["country_id"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>Country:</td>");
    echo("<td>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["country_id"]) ? getCountryName($company_row["country_id"]) : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["country_id"]) ? getCountryName($department_row["country_id"]) : "&nbsp;");
    }
    else
    {

        echo("<select name='contact_country_id' onchange='update_prov()'>");

        echo("<option value='' ");
        if (!$contact->contact_country_id || $contact->contact_country_id == '')
        {       
            echo("selected='selected'");
        }       
        echo(">&nbsp;</option>");
        $sql = ("
                SELECT DISTINCT country_id, country_name
                FROM country_list
                ORDER BY order_by
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["country_id"] . "'");
            if (($contact) && ($contact->contact_country_id == $row["country_id"]))
            {
                echo(" selected='selected'");
            }
            echo(">" . $row["country_name"] . "</option>");
        }
        echo("</select>");

    }

    echo("</td>");
    echo("</tr>");

}

if (($contact->contact_location_info == USE_COMPANY && $company_row["provstate_id"])
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["provstate_id"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>Province/State:</td>");
    echo("<td>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["provstate_id"]) ? getProvstateName($company_row["provstate_id"]) : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["provstate_id"]) ? getProvstateName($department_row["provstate_id"]) : "&nbsp;");
    }
    else
    {

        echo("<select name='contact_provstate_id' onchange='update_region()'>");

        if ($contact->contact_country_id == '' || $contact->contact_country_id == '0')
        {
            echo("<option value=''>Choose a Country</option>");
        }
        else
        {
            /*
             A country has been selected if we're at this point, so pull that countries provinces
             or states out of the database and display them.
            */

            $sql = ("
                    SELECT DISTINCT provstate_id, provstate_name
                    FROM provstate_list
                    WHERE country_id='" . addslashes($contact->contact_country_id) . "'
                    ORDER BY provstate_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                /*
                 If the chosen country has provinces or states, display them,
                 otherwise show N/A.
                */

                echo("<option value='' ");
                if (!$contact->contact_country_id || $contact->contact_country_id == '')
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["provstate_id"] . "'");
                    if ((!($contact->contact_provstate_id) && ($row["provstate_id"] == 1)) || (($contact) && ($contact->contact_provstate_id == $row["provstate_id"])))
                    {
                        echo(" selected='selected'");
                    }
                    echo(">" . $row["provstate_name"] . "</option>");
                }
            }
            else
            {
                echo("<option value='' selected='selected'>N/A</option>");
            }
        }
        echo("</select>");
    }

    echo("</td>");
    echo("</tr>");

}

if (($contact->contact_location_info == USE_COMPANY && $company_row["region_id"])
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["region_id"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>Region<br />(If in Canada):</td>");
    echo("<td>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["region_id"]) ? getRegionName($company_row["region_id"]) : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["region_id"]) ? getRegionName($department_row["region_id"]) : "&nbsp;");
    }
    else
    {
        echo("<select name='contact_region_id' onchange='update_city()'>");

        if ($contact->contact_country_id == '' || $contact->contact_country_id == '0')
        {
            echo("<option value=''>Choose a Country</option>");
        }
        elseif (!isset($contact->contact_provstate_id))
        {
            echo("<option value=''>Choose a Province/State</option>");
        }
        else
        {
            $sql = ("
                    SELECT DISTINCT region_id, region_name
                    FROM region_list
                    WHERE provstate_id='" . $contact->contact_provstate_id . "'
                    ORDER BY region_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                echo("<option value='' ");
                if (!$contact->contact_region_id || $contact->contact_region_id == '')
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["region_id"] . "' ");
                    if ($contact->contact_region_id == $row["region_id"])
                    {
                        echo("selected='selected'");
                    }
                    echo(">" . $row["region_name"] . "</option>");
                }
            }
            else
            {
                echo("<option value='' selected='selected'>N/A</option>");
            }
        }

        echo("</select>");

    }

    echo("</td>");
    echo("</tr>");

}

if (($contact->contact_location_info == USE_COMPANY && $company_row["city"])
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["city"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>City:</td>");
    echo("<td align='left'>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["city"]) ? $company_row["city"] : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["city"]) ? $department_row["city"] : "&nbsp;");
    }
    else
    {
        echo("<input type='text' name='contact_city' size='40' maxlength='40' value=\"" . $contact->contact_city . "\" />");
    }

    echo("</td>");
    echo("</tr>");

}

if (($contact->contact_location_info == USE_COMPANY && $company_row["postal_code"])
        || ($contact->contact_location_info == USE_DEPARTMENT && $department_row["postal_code"])
        || !$contact->contact_location_info)
{

    echo("<tr>");
    echo("<td align='right'>Postal/Zip Code:</td>");
    echo("<td align='left'>");

    if ($contact->contact_location_info == USE_COMPANY)
    {
        echo(($company_row["postal_code"]) ? $company_row["postal_code"] : "&nbsp;");
    }
    elseif ($contact->contact_location_info == USE_DEPARTMENT)
    {
        echo(($department_row["postal_code"]) ? $department_row["postal_code"] : "&nbsp;");
    }
    else
    {
        echo("<input type='text' name='contact_postal_code' size='10' maxlength='10' value=\"" . $contact->contact_postal_code . "\" />");
    }

    echo("</td>");
    echo("</tr>");

}

?>

</table>
</td>
</tr>

<?php
if ($changes_array["contact_location_changed"] || $changes_array["contact_location_info"])
{
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<table cellpadding='5' cellspacing='0' border='0' class='row0'>");

    if ($changes_array["contact_location_info"] == USE_COMPANY)
    {
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<b class='black'>You had specified that you would like to use the parent company's location information.</b>");
        echo("</td>");
        echo("</tr>");
    }
    elseif ($changes_array["contact_location_info"] == USE_DEPARTMENT)
    {
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<b class='black'>You had specified that you would like to use the parent division's location information.</b>");
        echo("</td>");
        echo("</tr>");
    }
    elseif ($changes_array["contact_location_changed"])
    {
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<b class='black'>You had specified the following location information:</b>");
        echo("</td>");
        echo("</tr>");

        if (trim($changes_array["contact_street_address_1"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("Street Address 1:");
            echo("</td>");
            echo("<td align='left'>");
            echo($changes_array["contact_street_address_1"]);
            echo("</td>");
            echo("</tr>");
        }

        if (trim($changes_array["contact_street_address_2"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("Street Address 2:");
            echo("</td>");
            echo("<td align='left'>");
            echo($changes_array["contact_street_address_2"]);
            echo("</td>");
            echo("</tr>");
        }

        if (trim($changes_array["contact_street_address_3"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("Street Address 3:");
            echo("</td>");
            echo("<td align='left'>");
            echo($changes_array["contact_street_address_3"]);
            echo("</td>");
            echo("</tr>");
        }

        if (trim($changes_array["contact_country_id"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("Country:");
            echo("</td>");
            echo("<td align='left'>");
            echo(getCountryName($changes_array["contact_country_id"]));
            echo("</td>");
            echo("</tr>");
        }

        if (trim($changes_array["contact_provstate_id"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("Province/State:");
            echo("</td>");
            echo("<td align='left'>");
            echo(getProvstateName($changes_array["contact_provstate_id"]));
            echo("</td>");
            echo("</tr>");
        }

        if (trim($changes_array["contact_region_id"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("Region:");
            echo("</td>");
            echo("<td align='left'>");
            echo(getRegionName($changes_array["contact_region_id"]));
            echo("</td>");
            echo("</tr>");
        }

        if (trim($changes_array["contact_city"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("City:");
            echo("</td>");
            echo("<td align='left'>");
            echo($changes_array["contact_city"]);
            echo("</td>");
            echo("</tr>");
        }

        if (trim($changes_array["contact_postal_code"]))
        {
            echo("<tr>");
            echo("<td align='right'>");
            echo("Postal Code:");
            echo("</td>");
            echo("<td align='left'>");
            echo($changes_array["contact_postal_code"]);
            echo("</td>");
            echo("</tr>");
        }

    }

    echo("</table>");
    echo("</td>");
    echo("</tr>");
}
?>

<tr>
<td colspan='2'>&nbsp;</td>
</tr>


<?php

$sql = ("
        SELECT DISTINCT a.flag_name, a.flag_id
        FROM contact_flags AS a
        INNER JOIN department_contact_flags AS b ON b.flag_id=a.flag_id
        WHERE b.department_id='" . $auth->department . "'
        ORDER BY a.flag_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{	
    // If there are contact flags for this co-op department, then display them and let them check them.

    echo("<tr>");
    echo("<td colspan='2' align='center'>Contact Flags</td>");
    echo("</tr>\n");

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<table cellpadding='5' cellspacing='0' border='0' class='row0' align='center'>\n");
    $newrow = 0;
    while ($row = $result->FetchRow())
    {
        if (!($newrow % 2))
        {
            echo("<tr>");
        }
        echo("<td nowrap='nowrap'><input type='checkbox' class='row2' name='contact_flags[]' value='" . $row["flag_id"] . "' ");
        if (is_array($contact->contact_flags) && in_array($row["flag_id"], $contact->contact_flags))
        {
            echo("checked='checked'");
        }
        echo(" />" . $row["flag_name"]);
        echo($changes_array["contact_changes_made"] ? "<br /><b class='black'>(" . (is_array($changes_array["contact_flags"]) && in_array($row["flag_id"], $changes_array["contact_flags"]) ? "On" : "Off") . ")</b>" : "");
        echo("</td>\n");
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

    echo("</table>");
    echo("</td>");
    echo("</tr>");
}

if ($contact->contact_note_ids)
{
    echo("<tr>");
    echo("<td>");
    echo("<input type='hidden' name='contact_note_ids' value='" . packObject(($contact->contact_note_ids)) . "' />");
    echo("</td>");
    echo("</tr>");
    foreach ($contact->contact_note_ids AS $cni)
    {
        echo("<tr><td colspan='2'><hr /></td></tr>");
        $sql = ("
                SELECT DISTINCT contact_actions_id
                , action_by
                , action_on
                , action_note
                FROM contact_actions_set
                WHERE contact_actions_id='" . $cni . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $contact_name = getContactName($row["action_by"]);
        $contact_name = $contact_name["first_name"] . " " . $contact_name["last_name"];

        echo("<tr>");
        echo("<td align='right'>Note entered by: </td>");
        if($contact_name!=" "){
            echo("<td align='left'><b>" . $contact_name . "</b></td>");
        }
        if($contact_name==" "){
            echo("<td align='left'><b>&nbsp;</b></td>");
        }    
        echo("</tr>");
        echo("<tr>");
        echo("<td align='right'>Note entered on: </td>");
        echo("<td align='left'><b>" . $row["action_on"] . "</b></td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td align='right'>Note: </td>");
        echo("<td align='left'>". $row["action_note"] ."</td>");
        echo("</tr>");
    }
}

/*

 This part will not be released to UVic.

 <tr>
 <td align='right'>Comments specific<br />to your co-op department:</td>
 <td align='left'><textarea wrap name='contact_department_comments' rows='10' cols='40'><?php echo($contact->contact_department_comments); ?></textarea></td>
 </tr>

 */
?>




<tr>
<td colspan='2'>&nbsp;</td>
</tr>

<tr>
<td colspan='2'><hr /></td>
</tr>

<?php

if ($contact->contact_changes_recorded_1)
{
    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table border='0' width='100%'>");

    echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("<b class='black'>Recent history of changes for this contact:</b>");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td width='50%'>&nbsp;</td>");
    echo("<td width='50%'>&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td align='right'>");
    echo("<b class='black'>Fields changed:</b>");
    echo("</td>");
    echo("<td align='left'>");
    echo($contact->contact_changes_recorded_1);
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td align='right'>");
    echo("<b class='black'>Changes above made on:</b>");
    echo("</td>");
    echo("<td align='left'>");
    echo(formatLongDate($contact->contact_change_date_1));
    echo("</td>");
    echo("</tr>");

    $sql = ("
            SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
            FROM contact AS a, department AS b, contact_internal AS c
            WHERE a.contact_id='" . $contact->contact_change_by_1 . "'
            AND c.contact_id=a.contact_id
            AND b.department_id=c.department_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    echo("<tr>");
    echo("<td align='right'>");
    echo("<b class='black'>Changes above made by:</b>");
    echo("</td>");
    echo("<td align='left'>");
    echo($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
    echo("</td>");
    echo("</tr>");

    if ($contact->contact_changes_recorded_2)
    {
        echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td align='right'>");
        echo("<b class='black'>Fields changed:</b>");
        echo("</td>");
        echo("<td align='left'>");
        echo($contact->contact_changes_recorded_2);
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td align='right'>");
        echo("<b class='black'>Changes above made on:</b>");
        echo("</td>");
        echo("<td align='left'>");
        echo(formatLongDate($contact->contact_change_date_2));
        echo("</td>");
        echo("</tr>");

        $sql = ("
                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                FROM contact AS a, department AS b, contact_internal AS c
                WHERE a .contact_id='" . $contact->contact_change_by_2 . "'
                AND c.contact_id=a.contact_id
                AND b.department_id=c.department_id
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        echo("<tr>");
        echo("<td align='right'>");
        echo("<b class='black'>Changes above made by:</b>");
        echo("</td>");
        echo("<td align='left'>");
        echo($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
        echo("</td>");
        echo("</tr>");
    }
    if ($contact->contact_changes_recorded_3)
    {
        echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td align='right'>");
        echo("<b class='black'>Fields changed:</b>");
        echo("</td>");
        echo("<td align='left'>");
        echo($contact->contact_changes_recorded_3);
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td align='right'>");
        echo("<b class='black'>Changes above made on:</b>");
        echo("</td>");
        echo("<td align='left'>");
        echo(formatLongDate($contact->contact_change_date_3));
        echo("</td>");
        echo("</tr>");

        $sql = ("
                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                FROM contact AS a, department AS b, contact_internal AS c
                WHERE a .contact_id='" . $contact->contact_change_by_3 . "'
                AND c.contact_id=a.contact_id
                AND b.department_id=c.department_id
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        echo("<tr>");
        echo("<td align='right'>");
        echo("<b class='black'>Changes above made by:</b>");
        echo("</td>");
        echo("<td align='left'>");
        echo($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
        echo("</td>");
        echo("</tr>");
    }

    echo("</table>");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'><hr /></td>");
    echo("</tr>");
}

?>

<tr>
<td colspan='2' align='center'>Fields marked with an asterisk (<b class='red'>*</b>) are required<hr /></td>
</tr>
<tr>
<td align='center' colspan='2'>
<input type='submit' name='continue' value='Save Changes' />
&nbsp;&nbsp;&nbsp;&nbsp;
<input type='reset' value='Reset Changes' />
</td>
</tr>

</table>
</center>

</form>
