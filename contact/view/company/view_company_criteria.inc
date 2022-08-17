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
| Filename: view_company_criteria.inc                                          |
+------------------------------------------------------------------------------+
| Description: This file is called by view_company.inc and is used to gather   |
| search inputs from the user.                                                 |
+------------------------------------------------------------------------------+

*/


$max_area_id=0;
$area_array = array();

$sql = ("
SELECT a.area_id, country_id, country_name
FROM area_list AS a LEFT JOIN country_list AS c ON a.area_id=c.area_id
ORDER BY area_id,c.order_by
");
$result = $GLOBALS['dbh']->Execute($sql);

$max_sql = ("
SELECT area_id
FROM area_list
ORDER BY area_id DESC
");
$max_result = $GLOBALS['dbh']->Execute($max_sql);
$max_row = $max_result->FetchRow();

for ($i = 1; $i <= $max_row["area_id"]; $i++)
{
$cntry_array[$i] = ("Array(\"NULL\"),");
}
while ($row = $result->FetchRow())
{
if ($row["country_id"] == '')
{
    $str = ("Array(\"NULL\"),");
}
else
{
    $str = ("Array(\"" . $row["country_id"] . "\", \"" . $row["country_name"] . "\"),");
}
$cntry_array[$row["area_id"]] .= $str;
if (!in_array($row["area_id"], $area_array))
{
    $area_array[] = $row["area_id"];
}
if ($row["area_id"] > $max_area_id)
{
    $max_area_id = $row["area_id"];
}
}
sort($area_array);

$str = ("Array(");

for ($i = 0; $i < ($max_area_id + 1); $i++)
{
if ($cntry_array[$i] == '')
{
    $str .= ("Array(\"NULL\"),");
}
else
{
    $str .= ("Array(" . substr($cntry_array[$i], 0, strlen($cntry_array[$i]) - 1) . "),");
}
}
$str = (substr($str, 0, strlen($str) - 1) . ")");
$cntry_array = $str;

$max_country_id=0;
$country_array = array();

/*
We need to put blank values at the start of every pull down array
*/

$max_sql = ("
 SELECT provstate_id
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
while ($row = $result->FetchRow()){
 if ($row["provstate_id"] == ""){
    
    /*
     If there's already a NULL value in this slot, don't put another one in.
    */
    
    if ($prov_array[$row["country_id"]] == "Array(\"NULL\"),")
    {
        $str = "";
    }
    else
    {
            $str = "Array(\"NULL\"),";
            //$str = "Array(),";
    }
 }else{
         $str = "Array(\"" . $row["provstate_id"] . "\", \"" . $row["provstate_name"] . "\"),";
 }
 $prov_array[$row["country_id"]] .= $str;
 if (!in_array($row["country_id"], $country_array)){
         $country_array[] =$row["country_id"];
 }
 if ($row["country_id"] > $max_country_id){
         $max_country_id = $row["country_id"];
 }
}
sort($country_array);

$str = "Array(";
for($i=0;$i<=$max_country_id;$i++){
 if ($prov_array[$i] == ""){
         $str .= "Array(\"NULL\"),";
         //$str .= "Array(),";
 }else{
         $str .= "Array(" . substr($prov_array[$i], 0, strlen($prov_array[$i])-1) . "),";
 }
}
$str = substr($str, 0, strlen($str)-1) . ")";
$prov_state_array = $str;

$max_prov_id = 0;

/*
We need to put blank values at the start of every pull down array
*/

$max_sql = ("
 SELECT region_id
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
 if ($row["region_id"] == ""){
         
         /*
          If there's already a NULL value in this slot, don't put another one in.
         */

         if ($region_array[$row["provstate_id"]] == "Array(\"NULL\"),")
         {
                 $str = "";
         }
         else
         {
                 $str = "Array(\"NULL\"),";
                 //$str = "Array(),";
         }
 }else{
         $str = "Array(\"" . $row["region_id"] . "\", \"" . $row["region_name"] . "\"),";
 }
 $region_array[$row["provstate_id"]] .= $str;

 if ($row["provstate_id"] > $max_prov_id){
         $max_prov_id = $row["provstate_id"];
 }
}
$str = "Array(";
for($i=0;$i<=$max_prov_id;$i++){
 if ($region_array[$i] == ""){
         $str .= "Array(\"NULL\"),";
 }else{
         $str .= "Array(" . substr($region_array[$i], 0, strlen($region_array[$i]) - 1) . "),";
 }
}
$str = substr($str, 0, strlen($str)-1) . ")";
$region_array = $str;
?>

<script type="text/javascript" language="javascript">
<!-- javascript

var region_array = new <?php echo $region_array?>;
var prov_array = new <?php echo $prov_state_array?>;
var cntry_array = new <?php echo $cntry_array?>;

function update_country()
{
    index3 = document.myform.search_area[document.myform.search_area.selectedIndex].value;
    country = cntry_array[index3];
    if (document.myform.search_area[document.myform.search_area.selectedIndex].value == "")
    {
        document.myform.search_country.options.length = 1;
        document.myform.search_country.options[0] = new Option("Choose an Area", "");
        document.myform.search_country.selectedIndex = 0;
        document.myform.search_provstate.options.length = 1;
        document.myform.search_provstate.options[0] = new Option("Choose an Area", "");
        document.myform.search_provstate.selectedIndex = 0;
        document.myform.search_region.options.length = 1;
        document.myform.search_region.options[0] = new Option("Choose an Area", "");
        document.myform.search_region.selectedIndex = 0;
    }
    else
    {
        if (country.length == 1 && country[0] == "NULL")
        {
            document.myform.search_country.options.length = 1;
            document.myform.search_country[0] = new Option("N/A", "");
            document.myform.search_country.selectedIndex = 0;
            document.myform.search_provstate.options.length = 1;
            document.myform.search_provstate.options[0] = new Option("N/A", "");
            document.myform.search_provstate.selectedIndex = 0;
            document.myform.search_region.options.length = 1;
            document.myform.search_region.options[0] = new Option("N/A", "");
            document.myform.search_region.selectedIndex = 0;
        }
        else
        {
            document.myform.search_country.options.length = 0;
            document.myform.search_country.options.length = country.length;
            for ($i = 0; $i < country.length; $i++)
            {
                if (country[$i] == "NULL")
                {
                    document.myform.search_country.options[$i] = new Option(" ", "");
                }
                else
                {
                    document.myform.search_country.options[$i] = new Option(country[$i][1], country[$i][0]);
                }
            }

            if (document.myform.search_area[document.myform.search_area.selectedIndex].value == 1)
            {
                document.myform.search_country.selectedIndex = 1;
            }
            else
            {
                if (document.myform.search_area[document.myform.search_area.selectedIndex].value == 2)
                {
                    document.myform.search_country.selectedIndex = 1;
                }
                else
                { 
                    document.myform.search_country.selectedIndex = 0;
                }
            }

            index4 = document.myform.search_country[document.myform.search_country.selectedIndex].value;
            prov = prov_array[index4];
            if (document.myform.search_country[document.myform.search_country.selectedIndex].value == "")
                    {
                            document.myform.search_provstate.options.length = 1;
                            document.myform.search_provstate.options[0] = new Option("Choose a Country", "");
                            document.myform.search_provstate.selectedIndex = 0;
                            document.myform.search_region.options.length = 1;
                            document.myform.search_region.options[0] = new Option("Choose a Country", "");
                            document.myform.search_region.selectedIndex = 0;
                    }
                    else
                    {
                if (prov.length == 1 && prov[0] == "NULL")
                {
                    document.myform.search_provstate.options.length = 1;
                    document.myform.search_provstate.options[0] = new Option("N/A", "");
                    document.myform.search_provstate.selectedIndex = 0;
                    document.myform.search_region.options.length = 1;
                    document.myform.search_region.options[0] = new Option("N/A", "");
                    document.myform.search_region.selectedIndex = 0;
                }
                else
                {
                    document.myform.search_provstate.options.length = 0;
                    document.myform.search_provstate.length = prov.length;
                    for ($i = 0; $i < prov.length; $i++)
                    {
                        if (prov[$i] == "NULL")
                        {
                            document.myform.search_provstate.options[$i] = new Option(" ", "");
                        }
                        else
                        {
                            document.myform.search_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                        }
                    }
                    document.myform.search_provstate.selectedIndex = 0;
    
                    index5 = document.myform.search_provstate[document.myform.search_provstate.selectedIndex].value;
                    region = region_array[index5];
                    if (document.myform.search_provstate[document.myform.search_provstate.selectedIndex].value == "")
                    {
                        document.myform.search_region.options.length = 1;
                        document.myform.search_region.options[0] = new Option("Choose a Province/State", "");
                        document.myform.search_region.selectedIndex = 0;
                    }
                    else
                    {
                        if (region.length == 1 && region[0] == "NULL")
                        {
                            document.myform.search_region.options.length = 1;
                            document.myform.search_region.options[0] = new Option("N/A", "");
                            document.myform.search_region.selectedIndex = 0;
                        }
                        else
                        {
                            document.myform.search_region.options.length = 0;
                            document.myform.search_region.options.length = region.length;
                            for ($i = 0; $i < region.length; $i++)
                            {
                                if (region[$i] == "NULL")
                                {
                                    document.myform.search_region.options[$i] = new Option(" ", "");
                                }
                                else
                                {
                                    document.myform.search_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                                }
                            }
                            document.myform.search_region.options.selectedIndex = 0;
                        }
                    }	
                }	
            }	
        }	
    }
}	

 function update_prov(){
         index = document.myform.search_country[document.myform.search_country.selectedIndex].value;
         prov = prov_array[index];
    if (document.myform.search_country[document.myform.search_country.selectedIndex].value == "")
    {
        if (document.myform.search_area[document.myform.search_area.selectedIndex].value == "")
        {
            message = "Choose an Area";
        }
        else
        {
            message = "Choose a Country";
        }
        document.myform.search_provstate.options.length = 1;
        document.myform.search_provstate.options[0] = new Option(message, "");
        document.myform.search_provstate.selectedIndex = 0;
        document.myform.search_region.options.length = 1;
        document.myform.search_region.options[0] = new Option(message, "");
        document.myform.search_region.selectedIndex = 0;
    }
    else
    {
             if (prov.length==1 && prov[0] == "NULL"){
                     document.myform.search_provstate.options.length = 1;
                     document.myform.search_provstate.options[0] = new Option("N/A", "");
                     document.myform.search_provstate.selectedIndex = 0;
                     document.myform.search_region.options.length=1;
                     document.myform.search_region.options[0] = new Option("N/A", "");
                     document.myform.search_region.selectedIndex = 0;

             }else{

                     document.myform.search_provstate.options.length =0;
                     document.myform.search_provstate.options.length = prov.length;
                     for($i=0;$i<prov.length;$i++){
                if (prov[$i] == "NULL")
                {
                    document.myform.search_provstate.options[$i] = new Option(" ", "");
                }
                else
                {
                    document.myform.search_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                }
                     }
                     document.myform.search_provstate.selectedIndex = 0;

                     index2=document.myform.search_provstate[document.myform.search_provstate.selectedIndex].value;
                     region = region_array[index2];
            if (document.myform.search_provstate[document.myform.search_provstate.selectedIndex].value == "")
            {
                if (document.myform.search_country[document.myform.search_country.selectedIndex].value == "")
                {
                    if (document.myform.search_area[document.myform.search_area.selectedIndex].value == "")
                    {
                        message = "Choose an Area";
                    }
                    else
                    {
                        message = "Choose a Country";
                    }
                }
                else
                {
                    message = "Choose a Province/State";
                }
                document.myform.search_region.options.length = 1;
                document.myform.search_region.options[0] = new Option(message, "");
                document.myform.search_region.selectedIndex = 0;
            }
            else
            {
                        if (region.length==1 && region[0] == "NULL"){
                                    document.myform.search_region.options.length=1;
                                    document.myform.search_region.options[0] = new Option("N/A", "");
                        }else{
                                document.myform.search_region.options.length=0;
                                document.myform.search_region.options.length=region.length;
                                for($i=0;$i<region.length;$i++){
                                        document.myform.search_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                                }
                        }
            }
        }
    }
}

 function update_region(){

         document.myform.search_region.options.length=0;
         index2=document.myform.search_provstate[document.myform.search_provstate.selectedIndex].value;
         region = region_array[index2];
    if (document.myform.search_provstate[document.myform.search_provstate.selectedIndex].value == "")
    {
        if (document.myform.search_country[document.myform.search_country.selectedIndex].value == "")
        {
            if (document.myform.search_area[document.myform.search_area.selectedIndex].value == "")
            {
                message = "Choose an Area";
            }
            else
            {
                message = "Choose a Country";
            }
        }
        else
        {
            message = "Choose a Province/State";
        }
    
        document.myform.search_region.options.length = 1;
        document.myform.search_region.options[0] = new Option(message, "");
        document.myform.search_region.selectedIndex = 0;
    }
    else
    {    
            if (region.length==1 && region[0] == "NULL"){
                    document.myform.search_region.options.length=1;
                    document.myform.search_region.options[0] = new Option("N/A", "");
            }else{
                    document.myform.search_region.options.length = region.length;
                        for($i=0;$i<region.length;$i++)
            {
                if (region[$i] == "NULL")
                {
                    document.myform.search_region.options[$i] = new Option(" ", "");
                }
                else
                {
                    document.myform.search_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                }
                        }
                }
                document.myform.search_region.selectedIndex = 0;
        }
}

function popUpCompany(slotName, formName)
{
    var str = "document." + formName + ".elements";

    var j = eval(str);
    for (var i = 0; i < j.length; i++)
    {
                var e = j[i];
        if (e.name == slotName)
        {
            var slot = i;
            break;
        }
    }	

    window.open("mamook.php?select=company_department_choose_2&no_headers=1&company_slot="+slot+"&parentFormName="+formName, "CompanyChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes, width=140,height=300");
}

//-->

</script>

<table border="0" cellpadding="5" cellspacing="0" width="96%" class="row1">
<tr>
<td align='center' colspan='2'>
<form method='post' name='myform2' action='<?php echo($PHP_SELF); ?>'>
<input type='hidden' name='select' value='view_contact' />
<input type='hidden' name='level1' value='company' />
<input type='hidden' name='search_comp_or_div' value='company' />
<input type='submit' name='switch_search' value='Go to Simple Search' />
</form>
</td>
</tr>
<tr>
<td align="center" colspan='2'><b class='black'>The list of companies/divisions returned will match the criteria entered below.  
<br />Leave any fields blank that you do not wish to be used in the search.</b>
</td>
</tr>
<tr>
<td colspan='2'><hr width='50%' size='1' /></td>
</tr>

<?php
//if there are any saved searches for student advanced search they are now shown here
//in a drop down box
$s_sql = "  SELECT search_name, search_id
FROM search
WHERE contact_id = '".$auth->contact_id."'
AND search_module_id = '".COMPANY_DIVISION_ADV_SEARCH."'
ORDER BY search_name";
$s_results = $GLOBALS['dbh']->Execute($s_sql);
if($s_results->RecordCount() > 0)
{
    echo("<tr>");
    echo("<td colspan ='2' align='center'>");
    echo("<form method='post' name='myform3' action='$PHP_SELF'>");
    echo("Search:&nbsp;");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='company' />");
    echo("<input type='hidden' name='continue' value='view_company_search_results' />");
    echo("<input type='hidden' name='search_comp_or_div' value='company' />");
    echo("<input type='hidden' name='btnSearch' value='Search' />");
    echo("<select name='search_id'>");
    while ($s_row = $s_results->FetchRow())
    {
        echo("<option value='".$s_row["search_id"]."'>".$s_row["search_name"]."</option>");
    }
    echo("</select>&nbsp;");
    echo("<input type='submit' value='Perform Search' />&nbsp;");
    echo("<input type='submit' name='continue' value='Manage My Saved Searches' />");
    echo("</form>");
    echo("</td>");
    echo("</tr>");
}
echo("<tr>");
echo("<td>");
//echo("<table border='0'>");
echo("<form method='post' name='myform' action='$PHP_SELF'>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='96%' class='row1'>");
echo("<tr>");
echo("<td>");
//echo("<form method='post' name='myform' action='$PHP_SELF'>");
echo("        <input type='hidden' name='select' value='view_contact' />
        <input type='hidden' name='level1' value='company' />
        <input type='hidden' name='search_comp_or_div' value='company' />");

echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
echo("<tr>");
echo("<td>&nbsp;</td><td>&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td align='right'>Company/Division name: ");
echo("<input type='radio' class='row1' name='search_name_as' value='start'");
echo(($search_name_as == 'start' || !$search_name_as) ? "checked='checked'" : "");
echo(" />Starting with&nbsp;");

echo("<input type='radio' class='row1' name='search_name_as' value='containing'");
echo(($search_name_as == 'containing') ? "checked='checked'" : "");
echo(" />Containing&nbsp;");
echo("</td>");

echo("<td align='left'>");
echo("<input type='text' name='search_name_m' size='30'");
if ($search_name_m)
{
    echo(" value=\"" . addslashes(trim($search_name_m)) . "\"");
}
echo(" />");
echo("</td>");
echo("</tr>");

$sql = ("
        SELECT employer_id
        FROM employer_company
        WHERE company_display = '1'
        ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
    ?>

        <tr>    
        <td colspan='2' align='center'>-Or-</td>
        </tr>   
        <tr>    
        <td colspan='2' align='center'>
        <input type='button' name='company_list' value='Choose from a list' onclick='popUpCompany("search_name_m", "myform");' />
        </td>
        </tr>   
        <?php
}

echo("<tr>");
echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2' nowrap='nowrap' align='center'>");
echo("<table border='0'>");

echo("<tr>");
echo("<td>Return companies/divisions with a status of:&nbsp;&nbsp;&nbsp;</td>");

$sql = ("
        SELECT status_id, status_name
        FROM employer_info_statuses
        ORDER BY level_of_activity
        ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<td>");

while ($row = $result->FetchRow())
{
    echo("<input type='checkbox' class='row1' name='search_company_status_status[]' value='" . $row["status_id"] . "' ");
    if ($search_company_status_status && in_array($row["status_id"], $search_company_status_status))
    {
        echo("checked='checked'");
    }
    echo(" />" . $row["status_name"]);
    echo("<br />");
}

echo("</td>");


echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;in&nbsp;&nbsp;</td>");
$sql = ("
        SELECT department_id, department_name
        FROM department
        WHERE using_full_system = '1'
        ORDER BY department_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<td>");
while ($row = $result->FetchRow())
{
    echo("<input type='checkbox' class='row1' name='search_company_status_department[]' value='" . $row["department_id"] . "' ");
    if (is_array($search_company_status_department) && sizeof($search_company_status_department) > 0 && in_array($row["department_id"], $search_company_status_department))
    {
        echo("checked='checked'");
    }
    // If a user loads from an advanced search and no departments were chosen, then don't select any departments.
    elseif((!is_array($search_company_status_department) || sizeof($search_company_status_department) == 0) && strlen($search_id))
    {
    }
    // Otherwise, if a user just came to this search page, check off their default department. 
    elseif (sizeof($search_company_status_department) == 0 && $row["department_id"] == $auth->department)
    {
        echo("checked='checked'");
    }
    echo(" />" . $row["department_name"] . "<br />");
}

echo("</td>");
echo("</tr>");
echo("</table>");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

?>

<tr>
<td align='right'>Website:&nbsp;
<input type='radio' class='row1' name='search_website_as' value='start' <?php
if ($search_website_as == 'start' || !$search_website_as)
{
    echo("checked='checked'");
}
?> />Starting with&nbsp;
<input type='radio' class='row1' name='search_website_as' value='containing' <?php
if ($search_website_as == 'containing')
{
    echo("checked='checked'");
}
?> />Containing&nbsp;
</td>
<td align='left'>
    <input type='text' name='search_website' size='30' <?php
if ($search_website)
{
    echo(" value='" . $search_website . "'");
}
?> />
</td>
</tr>
<tr>
<td align='right'>E-mail address:&nbsp;
<input type='radio' class='row1' name='search_email_as' value='start' <?php
if ($search_email_as == 'start' || !$search_email_as)
{
    echo("checked='checked'");
}
?> />Starting with&nbsp;
<input type='radio' class='row1' name='search_email_as' value='containing' <?php
if ($search_email_as == 'containing')
{
    echo("checked='checked'");
}
?> />Containing&nbsp;
</td>
<td align='left'>
    <input type='text' name='search_email' size='30' <?php
if ($search_email)
{
    echo(" value='" . $search_email . "'");
}
?> />
</td>
</tr>
<tr>
<td align='right'>Phone number:&nbsp;
<input type='radio' class='row1' name='search_phone_as' value='start' <?php
if ($search_phone_as == 'start' || !$search_phone_as)
{
    echo("checked='checked'");
}
?> />Starting with&nbsp;
<input type='radio' class='row1' name='search_phone_as' value='containing' <?php
if ($search_phone_as == 'containing')
{
    echo("checked='checked'");
}
?> />Containing&nbsp;
</td>
<td align='left'>
    <input type='text' name='search_phone' size='20' <?php
if ($search_phone)
{
    echo(" value='" . addslashes(trim($search_phone)) . "'");
}
echo(" />");
?>
</td>
</tr>
<tr>
<td colspan='2' align='center'>&nbsp;</td>
</tr>
<tr>
<td colspan='2' align='center'>Industry:&nbsp;</td>
</tr>
<tr>
<td colspan='2'>
<table align='center' cellpadding='3' cellspacing='3'>
<tr align='center'>
<td align='center' class='row2'>
<table width='100%' class='row2'>
<tr>    
<td colspan='6' align='center'>
Click the <b class='black'>Yes</b> box for any industries that you would like companies/divisions returned to be in.<br />
Click the <b class='black'>No</b> box for any industries that you would NOT like companies/divisions returned to be in.<br />
Leave both boxes blank if you don't want an industry included in your search.
</td>           
</tr>
<tr>
<td colspan='6'>&nbsp;</td>
</tr>
<tr>                    
<td>Yes</td>    
<td>No</td>     
<td>&nbsp;</td>
<td>Yes</td>
<td>No</td>
<td>&nbsp;</td>
</tr>
<?php

$newrow = 0;

$sql = ("
        SELECT industry_id, industry_name
        FROM industries
        ORDER BY industry_name 
        ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    if (!$newrow)
    {
        echo("<tr>");
    }

    echo("<td>");
    echo("<input type='checkbox' class='row2' name='yes_industries[]' value='" . $row["industry_id"] . "' ");
    if (is_array($yes_industries) && in_array($row["industry_id"], $yes_industries))
    {
        echo("checked='checked'");
    }
    echo(" />");
    echo("</td>");
    echo("<td>");
    echo("<input type='checkbox' class='row2' name='no_industries[]' value='" . $row["industry_id"] . "' ");
    if (is_array($no_industries) && in_array($row["industry_id"], $no_industries))
    {
        echo("checked='checked'");
    }
    echo(" />");
    echo("</td>");
    echo("<td>");
    echo(htmlentities($row["industry_name"]));
    echo("</td>");
//    echo("</td>");

    if ($newrow)
    {
        echo("</tr>");
    }

    $newrow = !($newrow);
}

if ($newrow)
{
    echo("<td>");
    echo("&nbsp;");
    echo("</td>");
    echo("</tr>");
}

?>
</table>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td colspan='2' align='center'>Size:&nbsp;</td>
</tr>
<tr>
<td colspan='2'>
<table align='center' cellpadding='3' cellspacing='3'>
<tr align='center'>
<td align='center' class='row2'>
<table width='100%' class='row2'>
<?php

$sql = ("
        SELECT size_id, size_range
        FROM employer_sizes
        ORDER BY size_id
        ");

$result = $GLOBALS['dbh']->Execute($sql);
$rowcount2 = 0;
while ($row = $result->FetchRow())
{
    if ($row["size_range"] != 'Unknown')
    {
        $rowcount2++;
    }
}

$rowcount = 0;
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    if ($row["size_range"] != 'Unknown')
    {
        if (!($rowcount % 2))
        {
            echo("<tr>");
        }

        echo("<td>");
        echo("<input type='checkbox' class='row2' name='search_size[]' value='" . $row["size_id"] . "' ");
        if ($search_size && in_array($row["size_id"], $search_size))
        {
            echo("checked='checked'");
        }
        echo(" />");
        echo($row["size_range"]);
        echo("</td>");

        if ($rowcount % 2)
        {
            echo("</tr>");
        }

        $rowcount++;
        if($rowcount == $rowcount2)
        {
            echo("<td><input type='checkbox' class='row2' name='search_size[]' value='Unknown' />Unknown</td>");
        }
    }
}

if ($rowcount % 2)
{
    echo("<td>");
    echo("&nbsp;");
    echo("</td>");
    echo("</tr>");
}

?>
</table>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td colspan='2' align='center'>Company Type:&nbsp;</td>
</tr>
<tr>
<td colspan='2'>
<table align='center' cellpadding='3' cellspacing='3'>
<tr align='center'>
<td align='center' class='row2'>
<table width='100%' class='row2'>
<?php

$rowcount = 0;

$sql = ("
        SELECT DISTINCT *
        FROM company_type
        ORDER BY type_name
        ");

$result = $GLOBALS['dbh']->Execute($sql);
$rowcount2 = 0;
while ($row = $result->FetchRow())
{
    if ($row["size_range"] != 'Unknown')
    {
        $rowcount2++;
    }
}

$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    if (!($rowcount % 2))
    {
        echo("<tr>");
    }

    echo("<td>");
    echo("<input type='checkbox' class='row2' name='search_type[]' value='" . $row["type_id"] . "' ");
    echo(($search_type && in_array($row["type_id"], $search_type)) ? "checked='checked'" : "");
    echo(" />");
    echo($row["type_name"]);
    echo("</td>");

    if ($rowcount % 2)
    {
        echo("</tr>");
    }
    $rowcount++;
    if($rowcount == $rowcount2)
    {
        echo("<td><input type='checkbox' class='row2' name='search_type[]' value='Unknown' />Unknown</td>");
    }
}

?>
</table>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td align='center' colspan='2'>Company/Division located in:
</td>
</tr>
<tr>
<td colspan='2'>
<table align='center' cellpadding='3' cellspacing='3'>
<tr align='center'>
<td align='center' class='row2'>
<table width='100%' class='row2'>
<tr align='center'>
<td align='right'>Area:
</td>
<td align='left'>
<select name='search_area' onchange='update_country()'>
<?php
echo("<option value='' ");
if (!$search_area || $search_area == '')
{
    echo("selected='selected'");
}
echo(">&nbsp;</option>");
$sql = ("
        SELECT DISTINCT area_id, area_name
        FROM area_list
        ORDER BY area_id
        ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    echo("<option value='" . $row["area_id"] . "' ");
    if ($search_area && $search_area == $row["area_id"])
    {
        echo("selected='selected'");
    }
    echo(">" . $row["area_name"] . "</option>");
}
echo("</select>");
?>
</td>
</tr>
<tr>
<td align='right'>Country:
</td>
<td align='left'>
<select name='search_country' onchange='update_prov()'>
<?php

/*
 Tricky bit of code here.  If for some reason the page has been refreshed, we need to be careful about
 what we put in these location select bars.  If the area has been chosen, then we need to display the
 appropriate countries.  If not, then we need to display a blank and a 'Choose an Area'.
 */

if ($search_area == '')
{
    echo("<option value=''>Choose an Area</option>");
}
else
{
    /*
     An area has been selected.
     */

    $sql = ("
            SELECT DISTINCT country_id, country_name
            FROM country_list
            WHERE area_id='" . $search_area . "'
            ORDER BY order_by 
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    echo("<option value='' ");
    if (!$search_country || $search_country == '')
    {
        echo("selected='selected'");
    }
    echo(">&nbsp;</option>");
    while ($row = $result->FetchRow())
    {
        echo("<option value='" . $row["country_id"] . "' ");
        if ($search_country == $row["country_id"])
        {
            echo("selected='selected'");
        }
        echo(">" . $row["country_name"] . "</option>");
    }
}
echo("</select>");

?>
</td>
</tr>
<tr>
<td align='right'>Province/State:
</td>
<td align='left'>
<select name='search_provstate' onchange='update_region()'>
<?php

/*
 More tricky nonsense here.  The same situation as above, except that now we have to take into account
 the situations of an Area being selected but no Country, or no Area AND no Country.  Ugh.
 */

if ($search_area == '')
{
    echo("<option value='' selected='selected'>&nbsp;</option>");
    echo("<option value=''>Choose an Area</option>");
    echo("<option value='' selected='selected'>&nbsp;</option>");
    echo("<option value='' selected='selected'>&nbsp;</option>");
    echo("<option value='' selected='selected'>&nbsp;</option>");
}
elseif ($search_country == '')
{
    echo("<option value=''>Choose a Country</option>");
}
else
{
    /*
     A country and area have been selected.
     */

    $sql = ("
            SELECT DISTINCT provstate_id, provstate_name
            FROM provstate_list
            WHERE country_id='" . $search_country . "'
            ORDER BY provstate_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    if ($result->RecordCount())
    {
        /*
         If the chosen country has provinces/states associated with it, displays those and a blank
         as choices.  Otherwise display N/A.
         */

        echo("<option value='' ");
        if (!$search_provstate || $search_provstate == '')
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["provstate_id"] . "' ");
            if ($search_provstate == $row["provstate_id"])
            {
                echo("selected='selected'");
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
?>
</td>
</tr>
<tr>
<td align='right'>Region:
</td>
<td align='left'>
<select name='search_region'>
<?php

/*
 Here's the last task.  This is the trickiest.  We have to check if we're dealing with a blank Area, Country
 or Province/State, and display the appropriate choices (this is only really critical when someone updates the
 page somehow).
 */

if ($search_area == '')
{
    echo("<option value=''>Choose an Area</option>");
}
elseif ($search_country == '')
{
    echo("<option value=''>Choose a Country</option>");
}
elseif (!isset($search_provstate))
{
    echo("<option value='' selected='selected'>&nbsp;</option>");
}
else
{
    $sql = ("
            SELECT DISTINCT region_id, region_name
            FROM region_list
            WHERE provstate_id='" . $search_provstate . "'
            ORDER BY region_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    if ($result->RecordCount())
    {
        echo("<option value='' ");
        if (!$search_region || $search_region == '')
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["region_id"] . "' ");
            if ($search_region == $row["region_id"])
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
?>
</td>
</tr>
<tr>
<td align='right'>City starting with:</td>
    <td align='left'><input type='text' size='20' name='search_city' <?php
if ($search_city)
{
    echo("VALUE='" . $search_city . "'");
}
echo(" />");
?>
</td>
</tr>
<tr>
<td align='right'>Postal/Zip Code starting with:</td>
    <td align='left'><input type='text' size='10' name='search_postal_code' <?php
if ($search_postal_code)
{
    echo("VALUE ='" . $search_postal_code . "'");
}
echo(" />");
?>
</td>
</tr>
</table>
</td>
</tr>
</table>
</td>
</tr>

<?php

$sql = ("
        SELECT DISTINCT a.flag_id, a.flag_name
        FROM company_flags AS a, department_company_flags AS b
        WHERE a.flag_id=b.flag_id
        AND b.department_id='" . $auth->department . "'
        ORDER BY a.flag_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
    ?>
        <tr>
        <td colspan='2' align='center'>Flags:</td>
        </tr>
        <tr>
        <td colspan='2'>
        <table align='center' cellpadding='3' cellspacing='3'>
        <tr align='center'>
        <td align='center' class='row2'>
        <table width='100%' class='row2'>
        <tr>
        <td colspan='2' align='center'>Return companies/divisions with&nbsp;
    <input type='radio' class='row2' name='search_flags_boolean' value='and' <?php
        if ($search_flags_boolean == 'and' || !$search_flags_boolean)
        {
            echo("checked='checked'");
        }
    ?> />All&nbsp;
    <input type='radio' class='row2' name='search_flags_boolean' value='or' <?php
        if ($search_flags_boolean == 'or')
        {
            echo("checked='checked'");
        }
    ?> />Any&nbsp;
    of the flags below checked&nbsp;
    </td>
        </tr>
        <tr>
        <td colspan='2'>&nbsp;</td>
        </tr>
        <?php
        $newrow = 0;

    while ($row = $result->FetchRow())
    {
        if (!$newrow)
        {
            echo("<tr>");
        }

        echo("<td>");
        echo("<input type='checkbox' class='row2' name='checked_flags[]' value='" . $row["flag_id"] . "' ");
        if (is_array($checked_flags) && in_array($row["flag_id"], $checked_flags))
        {
            echo("checked='checked'");
        }
        echo(" />");
        echo($row["flag_name"]);
        echo("</td>");

        if ($newrow)
        {
            echo("</tr>");
        }

        $newrow = (($newrow) ? 0 : 1);
    }

    if ($newrow)
    {
        echo("<td>&nbsp;</td>");
        echo("</tr>");
    }
    ?>
        </table>
        </td>
        </tr>
        </table>
        </td>
        </tr>
        <?php
}
?>

<tr>
<td colspan='2' align='center'>Display the following fields in search results:</td>
</tr>
<tr>
<td colspan='2'>
<table align='center' cellpadding='3' cellspacing='3'>
<tr align='center'>
<td align='center' class='row2'>
<table width='100%' class='row2'>
<tr>
<?php

/*
 Set up the display array.  Since we're going to be giving the user
 six select menus to choose from, it makes sense to pull the options
 out of an array each time rather than hard code it all.
 */

$display_array[] = array(
        "value" => "ignore",
        "name" => "&nbsp;"
        );
$display_array[] = array(
        "value" => "address",
        "name" => "Address"
        );
$display_array[] = array(
        "value" => "city",
        "name" => "City"
        );
$display_array[] = array(
        "value" => "country",
        "name" => "Country"
        );
$display_array[] = array(
        "value" => "email",
        "name" => "E-mail"
        );
$display_array[] = array(
        "value" => "fax",
        "name" => "Fax"
        );
$display_array[] = array(
        "value" => "name",
        "name" => "Name"
        );
$display_array[] = array(
        "value" => "phone",
        "name" => "Phone"
        );
$display_array[] = array(
        "value" => "postal_code",
        "name" => "Postal Code"
        );
$display_array[] = array(
        "value" => "provstate",
        "name" => "Province/State"
        );
$display_array[] = array(
        "value" => "region",
        "name" => "Region"
        );
$display_array[] = array(
        "value" => "size",
        "name" => "Size"
        );
$display_array[] = array(
        "value" => "website",
        "name" => "Website"
        );
?>

<td align='right'>First Column:&nbsp;</td>
<td>
Name
<!--Name is no longer optional
<select name='display1'>
<?php
/*
 for ($i = 0; $i < sizeof($display_array); $i++)
 {
 echo("<option value='" . $display_array[$i]["value"] . "' ");
 if ((!$display1 && $display_array[$i]["value"] == "name") || $display1 == $display_array[$i]["value"])
 {
 echo("selected='selected'");
 }
 echo(">" . $display_array[$i]["name"] . "</option>");
 }
 */
?>
</select>
-->
</td>
<td align='right'>Second Column:&nbsp;</td>
<td>
<select name='display2'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$display2 && $display_array[$i]["value"] == 'phone') || $display2 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
    echo(">" . $display_array[$i]["name"] . "</option>");
}
?>
</select>
</td>
</tr>
<tr>
<td align='right'>Third Column:&nbsp;</td>
<td>
<input type='hidden' name='level1' value='company' />
<select name='display3'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$display3 && $display_array[$i]["value"] == 'email') || $display3 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
    echo(">" . $display_array[$i]["name"] . "</option>");
}
?>
</select>
</td>
<td align='right'>Fourth Column:&nbsp;</td>
<td>
<select name='display4'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$display4 && $display_array[$i]["value"] == 'ignore') || $display4 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
    echo(">" . $display_array[$i]["name"] . "</option>");
}
?>
</select>
</td>
</tr>
<tr>
<td align='right'>Fifth Column:&nbsp;</td>
<td>
<select name='display5'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$display5 && $display_array[$i]["value"] == 'ignore') || $display5 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
    echo(">" . $display_array[$i]["name"] . "</option>");
}
?>
</select>
</td>
<td align='right'>Sixth Column:&nbsp;</td>
<td>
<select name='display6'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$display6 && $display_array[$i]["value"] == 'ignore') || $display6 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
    echo(">" . $display_array[$i]["name"] . "</option>");
}
?>
</select>
</td>
</tr>
</table>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td colspan='2' align='center'>Order search results using:</td>
</tr>
<tr>
<td colspan='2'>
<table align='center' cellpadding='3' cellspacing='3'>
<tr align='center'>
<td align='center' class='row2'>
<table width='100%' class='row2'>
<tr>
<td align='right'>First:&nbsp;</td>
<td>
<select name='order1'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$order1 && $display_array[$i]["value"] == 'ignore') || $order1 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
    echo(">" . $display_array[$i]["name"] . "</option>");
}
?>
</select>
</td>
<td align='right'>Second:&nbsp;</td>
<td>
<select name='order2'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$order2 && $display_array[$i]["value"] == 'ignore') || $order2 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
    echo(">" . $display_array[$i]["name"] . "</option>");
}
?>
</select>
</td>
</tr>
<tr>
<td align='right'>Third:&nbsp;</td>
<td>
<select name='order3'>
<?php
for ($i = 0; $i < sizeof($display_array); $i++)
{
    echo("<option value='" . $display_array[$i]["value"] . "' ");
    if ((!$order3 && $display_array[$i]["value"] == 'ignore') || $order3 == $display_array[$i]["value"])
    {
        echo("selected='selected'");
    }
                        echo(">" . $display_array[$i]["name"] . "</option>");
                }
                ?>
                </select>
                </td>
		<td align='right'>Fourth:&nbsp;</td>
                <td>
                <select name='order4'>
                <?php
                for ($i = 0; $i < sizeof($display_array); $i++)
                {
                        echo("<option value='" . $display_array[$i]["value"] . "' ");
                        if ((!$order4 && $display_array[$i]["value"] == 'ignore') || $order4 == $display_array[$i]["value"])
                        {
                                echo("selected='selected'");
                        }
                        echo(">" . $display_array[$i]["name"] . "</option>");
                }
                ?>
                </select>
                </td>
	</tr>
	</table>
	</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td colspan='2' align='center'>&nbsp;</td>
</tr>
<tr>
	<td colspan='2' align='center'><hr />
	<input type='hidden' name='continue' value='first_company_form_done' />
	<input type='submit' value='Continue' />
	&nbsp;&nbsp;&nbsp;
    <input type='button' value='Reset' onclick='javascript:document.reset_page.submit()' />
	&nbsp;&nbsp;&nbsp;
    <?php
	//<input type='reset' value='Reset Form' />
    
    if($btnSearch == "Edit Search Criteria")
    {
        //These variables are only set if you are editing a saved search
        echo("<input type='hidden' name='search_id' value='$search_id' />");
        echo("<input type='hidden' name='edit_page' value='yes' />");
    }
    ?>
    <input type='submit' name='btnSearch' value='Save Search' />
	</td>
</tr>
</table>
	</td>
</tr>
</table></form>
	</td>
</tr>
</table>	</td>
</tr>


<?php
echo("<tr><td>");
echo("<form name='reset_page' method='post' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='continue' value='Go to Advanced Search' />");
if (strlen($search_id))
{
    echo("<input type='hidden' name='search_id' value='".$search_id."' />");
    echo("<input type='hidden' name='edit_page' value='yes' />");
    echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
}
echo("</form>");
//echo("</td></tr>");

?>
