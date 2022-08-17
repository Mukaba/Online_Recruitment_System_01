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
 | Filename: show_old_new_department_inputs_then_contact.inc                    |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the new and old inputs      |
 | for a division, which will be used to update an existing division.  The      |
 | user can then make any changes they wish and then save them to the           |
 | database.  After completion, the changes will be saved and the user will be  |
 | able to update the contact information.                                      |
 +------------------------------------------------------------------------------+

*/

$max_country_id=0;
$country_array = array();

/*
 We need to put blank values at the start of every pull down array
*/

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
                $str = "Array('" . $row["provstate_id"] . "', '" . $row["provstate_name"] . "'),";
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
                $str = "Array('" . $row["region_id"] . "', '" . $row["region_name"] . "'),";
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

        function update_prov(){
                index = document.form1.department_country_id[document.form1.department_country_id.selectedIndex].value;
                prov = prov_array[index];
                if (document.form1.department_country_id[document.form1.department_country_id.selectedIndex].value == "")
                {
			message = "Choose a Country";
                        document.form1.department_provstate_id.options.length = 1;
                        document.form1.department_provstate_id.options[0] = new Option(message, "");
                        document.form1.department_provstate_id.selectedIndex = 0;
                        document.form1.department_region_id.options.length = 1;
			            document.form1.department_region_id.options[0] = new Option(message, "");
                        document.form1.department_region_id.selectedIndex = 0;
                }
                else
                {
                        if (prov.length==1 && prov[0] == "NULL"){
                                document.form1.department_provstate_id.options.length = 1;
                                document.form1.department_provstate_id.options[0] = new Option("N/A", "");
                                document.form1.department_provstate_id.selectedIndex = 0;
                                document.form1.department_region_id.options.length=1;
                                document.form1.department_region_id.options[0] = new Option("N/A", "");
                                document.form1.department_region_id.selectedIndex = 0;

                        }else{

                                document.form1.department_provstate_id.options.length =0;
                                document.form1.department_provstate_id.options.length = prov.length;
                                for($i=0;$i<prov.length;$i++){
                                        if (prov[$i] == "NULL")
                                        {
                                                document.form1.department_provstate_id.options[$i] = new Option(" ", "");
                                        }
                                        else
                                        {
                                                document.form1.department_provstate_id.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                                        }
                                }
                                document.form1.department_provstate_id.selectedIndex = 0;

                                index2=document.form1.department_provstate_id[document.form1.department_provstate_id.selectedIndex].value;
                                region = region_array[index2];
                                if (document.form1.department_provstate_id[document.form1.department_provstate_id.selectedIndex].value == "")
                                {
                                        if (document.form1.department_country_id[document.form1.department_country_id.selectedIndex].value == "")
                                        {
						message = "Choose a Country";
                                        }
                                        else
                                        {
						message = "Choose a Province/State";
                                        }
                                        document.form1.department_region_id.options.length = 1;
                                        document.form1.department_region_id.options[0] = new Option(message, "");
                                        document.form1.department_region_id.selectedIndex = 0;
                                }
                                else
                                {
                                        if (region.length==1 && region[0] == "NULL"){
                                                document.form1.department_region_id.options.length=1;
                                                document.form1.department_region_id.options[0] = new Option("N/A", "");
                                        }else{
                                                document.form1.department_region_id.options.length=0;
                                                document.form1.department_region_id.options.length=region.length;
                                                for($i=0;$i<region.length;$i++){
                                                        document.form1.department_region_id.options[$i] = new Option(region[$i][1], region[$i][0]);
                                                }
                                        }
                                }
                        }
                }
        }

        function update_region(){

                document.form1.department_region_id.options.length=0;
                index2=document.form1.department_provstate_id[document.form1.department_provstate_id.selectedIndex].value;
                region = region_array[index2];
                if (document.form1.department_provstate_id[document.form1.department_provstate_id.selectedIndex].value == "")
                {
                        if (document.form1.department_country_id[document.form1.department_country_id.selectedIndex].value == "")
                        {
				message = "Choose a Country";
                        }
                        else
                        {
                                message = "Choose a Province/State";
                        }
			
                        document.form1.department_region_id.options.length = 1;
                        document.form1.department_region_id.options[0] = new Option(message, "");
                        document.form1.department_region_id.selectedIndex = 0;
                }
                else
                {
                        if (region.length==1 && region[0] == "NULL"){
                                document.form1.department_region_id.options.length=1;
                                document.form1.department_region_id.options[0] = new Option("N/A", "");
                        }else{
                                document.form1.department_region_id.options.length = region.length;
                                for($i=0;$i<region.length;$i++){
                                        if (region[$i] == "NULL")
                                        {
                                                document.form1.department_region_id.options[$i] = new Option(" ", "");
                                        }
                                        else
                                        {
                                                document.form1.department_region_id.options[$i] = new Option(region[$i][1], region[$i][0]);
                                        }
                                }
                        }
                        document.form1.department_region_id.selectedIndex = 0;
                }
        }

	function update_city()
        {
                index3 = document.form1.department_region_id[document.form1.department_region_id.selectedIndex].value;

                if (index3)
                {
                        city = city_array[index3];
                        document.form1.department_city.value = city;
                }
        }

</script>

<?php

if (!$oldValues->department_phone) { $oldValues->department_phone = $pendingEmployer->company_phone; }

if (!$oldValues->department_fax) { $oldValues->department_fax = $pendingEmployer->company_fax; }

if (!$oldValues->department_email) { $oldValues->department_email = $pendingEmployer->company_email; }

if (!$oldValues->department_website) { $oldValues->department_website = $pendingEmployer->company_website; }

if (!$oldValues->department_size_id) { $oldValues->department_size_id = $pendingEmployer->company_size_id; }

if (!$oldValues->department_industry_id) { $oldValues->department_industry_id = $pendingEmployer->company_industry_id; }

if (!$oldValues->department_description) { $oldValues->department_description = $pendingEmployer->company_description; }

echo("<form method='post' action='" . $PHP_SELF . "' name='form1'>");
echo("<input type='hidden' name='action' value='do_contact_next' />");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='matching_department_id' value='" . $matching_department_id . "' />");
echo("<input type='hidden' name='matching_employer_id' value='" . $matching_employer_id . "' />");
echo("<input type='hidden' name='matching_contact_id' value='" . $matching_contact_id . "' />");
echo("<input type='hidden' name='pending_employer_id' value='" . $pendingEmployer->employer_id . "' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo("<input type='hidden' name='continue' value='method_for_adding' />");
echo("<input type='hidden' name='choice' value='save_updated_department' />");

$sql = ("
	SELECT DISTINCT company_name
	FROM employer_company
	WHERE employer_id='" . addslashes($matching_employer_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$company_name = $row["company_name"];

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

echo("<tr>");
	echo("<td width='50%'>&nbsp;</td>");
	echo("<td width='50%'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
        echo("<td align='center' colspan='2'><b class='black'>Parent Company:</b>&nbsp;");
	echo($company_name); 
        echo("</td>");
echo("</tr>");

echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td width='50%' align='right'>Division Name:</td>");
	echo("<td width='50%' align='left'>" . $oldValues->department_name . "</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Division Phone:</td>");
	echo("<td align='left'>");
	
	$sql = ("
		SELECT DISTINCT phone
		FROM employer_company
		WHERE employer_id='" . addslashes($oldValues->department_company_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
                
	if (trim($oldValues->department_phone) == USE_COMPANY)
	{
		echo($row["phone"]);
		echo("&nbsp;&nbsp;(Company phone)&nbsp;&nbsp;");
		echo("<input type='submit' name='choice' value='Specify Division Phone' />");
		echo("<input type='hidden' name='department_phone' value='" . USE_COMPANY . "' />");
		echo(($pendingEmployer->company_phone) ? "<br /><b class='black'>" . (($pendingEmployer->company_phone == USE_COMPANY) ? "Using Company's" : $pendingEmployer->company_phone) . "</b>" : "");
	}
	else
	{
		echo("<input type='text' name='department_phone' size='22' maxlength='22' value='" . $oldValues->department_phone . "' />");
		if ($row["phone"])
		{
			echo("&nbsp;&nbsp;Or&nbsp;&nbsp;");
			echo("<input type='submit' name='choice' value='Use Company Phone' />");
		}
		echo(($pendingEmployer->company_phone) ? "<br /><b class='black'>" . (($pendingEmployer->company_phone == USE_COMPANY) ? "Using Company's" : $pendingEmployer->company_phone) . "</b>" : "");
	}
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Division Fax:</td>");
	echo("<td align='left'>");
		
        $sql = ("
                SELECT DISTINCT fax 
                FROM employer_company
                WHERE employer_id='" . addslashes($oldValues->department_company_id) . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

	if (trim($oldValues->department_fax) == USE_COMPANY)
        {
                echo($row["fax"]);
                echo("&nbsp;&nbsp;(Company fax)&nbsp;&nbsp;");
                echo("<input type='submit' name='choice' value='Specify Division Fax' />");
                echo("<input type='hidden' name='department_fax' value='" . USE_COMPANY . "' />");
		echo(($pendingEmployer->company_fax) ? "<br /><b class='black'>" . (($pendingEmployer->company_fax == USE_COMPANY) ? "Using Company's" : $pendingEmployer->company_fax) . "</b>" : "");
        }
	else
	{
		echo("<input type='text' name='department_fax' size='22' maxlength='22' value='" . $oldValues->department_fax . "' />");
		if ($row["fax"])
		{
			echo("&nbsp;&nbsp;Or&nbsp;&nbsp;");
			echo("<input type='submit' name='choice' value='Use Company Fax' />");
		}
		echo(($pendingEmployer->company_fax) ? "<br /><b class='black'>" . (($pendingEmployer->company_fax == USE_COMPANY) ? "Using Company's" : $pendingEmployer->company_fax) . "</b>" : "");
	}
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Division E-mail:</td>");
	echo("<td align='left'>");
	echo("<input type='text' name='department_email' size='40' value='" . $oldValues->department_email . "'' />");
	echo(($pendingEmployer->company_email) ? "<br /><b class='black'>" . $pendingEmployer->company_email . "</b>" : "");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Division Website:</td>");
	echo("<td align='left'>");
	echo("<input type='text' name='department_website' size='45' maxlength='60' value='" . $oldValues->department_website . "' />");
	echo(($pendingEmployer->company_website) ? "<br /><b class='black'>" . $pendingEmployer->company_email . "</b>" : "");
	echo("</td>");
echo("</tr>");

$sql = ("
	SELECT DISTINCT size_id, size_range
	FROM employer_sizes
	ORDER BY size_id
	");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<tr>");
	echo("<td align='right'>Division Size:</td>");
	echo("<td align='left'>");
	echo("<select name='department_size_id'>");

	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["size_id"] . "' ");
		if ($oldValues->department_size_id == $row["size_id"])
		{
			echo("selected='selected'");
		}
		echo(">" . $row["size_range"] . "</option>");
	}
	echo("</select>");
	echo(($pendingEmployer->company_size_id) ? "<br /><b class='black'>" . getSizeRange($pendingEmployer->company_size_id) . "</b>" : "");
	echo("</td>");
echo("</tr>");

$sql = ("
	SELECT DISTINCT industry_id, industry_name
	FROM industries
	ORDER BY industry_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<tr>");
	echo("<td align='right'>Division Industry:</td>");
	echo("<td align='left'>");
	echo("<select name='department_industry_id'>");

	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["industry_id"] . "' ");
		if ($oldValues->department_industry_id == $row["industry_id"])
		{
			echo("selected='selected'");
		}
		echo(">" . $row["industry_name"] . "</option>");
	}
	
	echo("</select>");
	echo(($pendingEmployer->company_industry_id) ? "<br /><b class='black'>" . getIndustryName($pendingEmployer->company_industry_id) . "</b>" : "");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'>Division Location:");
	
        $sql = ("
                SELECT DISTINCT street_address_1, street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code
                FROM employer_company 
                WHERE employer_id='" . addslashes($matching_employer_id) . "'
                ");
        $company_result = $GLOBALS['dbh']->Execute($sql);
        $company_row = $company_result->FetchRow();
                        
        /*      
         If there is at least one of the location fields filled out, then set the variable
         to let us know that this is the case.
        */              
                        
        $company_info_valid = ($company_row["street_address_1"] || $company_row["city"] || $company_row["region_id"]
        || $company_row["provstate_id"] || $company_row["country_id"] || $company_row["postal_code"]);
                
        if ($department_location_info)
        {
                echo("&nbsp;<input type='submit' name='choice' value='Specify Division Location' />");
        }       
        if ($department_location_info != USE_COMPANY && $company_info_valid)
        {
                echo("&nbsp;<input type='submit' name='choice' value='Use Company Location' />");
        }
 
        echo("<input type='hidden' name='department_location_info' value='" . $department_location_info . "' />");

	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<table cellpadding='5' cellspacing='0' border='0' class='row0'>");

        if ($department_location_info == USE_COMPANY)
        {
                echo("<tr>");
                        echo("<td colspan='2' align='center'>");
                        echo("<b class='black'>Using Company's Location</b>");
                        echo("</td>");
                echo("</tr>");
        }

        if (($department_location_info == USE_COMPANY && $company_row["street_address_1"])
        || !$department_location_info)
        {
		echo("<tr>");
			echo("<td align='right'>Street Address 1:</td>");
			echo("<td align='left'>");

			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["street_address_1"]) ? $company_row["street_address_1"] : "&nbsp;");
				echo(($pendingEmployer->company_street_address_1) ? "<br /><b class='black'>" . $pendingEmployer->company_street_address_1 . "</b>" : "");
			}
			else
			{
				echo("<input type='text' name='department_street_address_1' size='45' maxlength='75' value='" . $oldValues->department_street_address_1 . "' />");
				echo(($pendingEmployer->company_street_address_1) ? "<br /><b class='black'>" . $pendingEmployer->company_street_address_1 . "</b>" : "");
			}

			echo("</td>");
		echo("</tr>");
        }

	if (($department_location_info == USE_COMPANY && $company_row["street_address_2"]) || !$department_location_info)
	{
		echo("<tr>");
			echo("<td align='right'>Street Address 2 (optional):</td>");
			echo("<td align='left'>");
			
			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["street_address_2"]) ? $company_row["street_address_2"] : "&nbsp;");
				echo(($pendingEmployer->company_street_address_2) ? "<br /><b class='black'>" . $pendingEmployer->company_street_address_2 . "</b>" : "");
			}
			else
			{
				echo("<input type='text' name='department_street_address_2' size='45' maxlength='75' value='" . $oldValues->department_street_address_2 . "' />");
				echo(($pendingEmployer->company_street_address_2) ? "<br /><b class='black'>" . $pendingEmployer->company_street_address_2 . "</b>" : "");
			}
			
			echo("</td>");
		echo("</tr>");
	}

	if (($department_location_info == USE_COMPANY && $company_row["street_address_3"]) || !$department_location_info)
	{
		echo("<tr>");
			echo("<td align='right'>Street Address 3 (optional):</td>");
			echo("<td align='left'>");
		
			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["street_address_3"]) ? $company_row["street_address_3"] : "&nbsp;");
				echo(($pendingEmployer->company_street_address_3) ? "<br /><b class='black'>" . $pendingEmployer->company_street_address_3 . "</b>" : "");
			}
			else
			{
				echo("<input type='text' name='department_street_address_3' size='45' maxlength='75' value='" . $oldValues->department_street_address_3 . "' />");
				echo(($pendingEmployer->company_street_address_3) ? "<br /><b class='black'>" . $pendingEmployer->company_street_address_3 . "</b>" : "");
			}

			echo("</td>");
		echo("</tr>");
	}

	if (($department_location_info == USE_COMPANY && $company_row["country_id"]) || !$department_location_info)
	{
		echo("<tr>");
			echo("<td align='right'>Country:</td>");
			echo("<td>");

			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["country_id"]) ? getCountryName($company_row["country_id"]) : "&nbsp;");
				echo(($pendingEmployer->company_country_id) ? "<br /><b class='black'>" . getCountryName($pendingEmployer->company_country_id) . "</b>" : "");
			}
			else
			{
				echo("<select name='department_country_id' onchange='update_prov()'>");
		
				echo("<option value='' ");
				if (!$oldValues->department_country_id || $oldValues->department_country_id == '')
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
					if (($oldValues) && ($oldValues->department_country_id == $row["country_id"]))
					{
						echo(" selected='selected'");
					}
					echo(">" . $row["country_name"] . "</option>");
				}

				echo("</select>");

				echo(($pendingEmployer->company_country_id) ? "<br /><b class='black'>" . getCountryName($pendingEmployer->company_country_id) . "</b>" : "");
			}
			
			echo("</td>");
		echo("</tr>");
	}

	if (($department_location_info == USE_COMPANY && $company_row["provstate_id"])
	|| !$department_location_info)
	{
		echo("<tr>");
			echo("<td align='right'>Province/State:</td>");
			echo("<td>");
			
			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["provstate_id"]) ? getProvstateName($company_row["provstate_id"]) : "&nbsp;");
				echo(($pendingEmployer->company_provstate_id) ? "<br /><b class='black'>" . getProvstateName($pendingEmployer->company_provstate_id) . "</b>" : "");
			}
			else
			{
				echo("<select name='department_provstate_id' onchange='update_region()'>");
		
				if ($oldValues->department_country_id == '' || !$oldValues->department_country_id)
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
						WHERE country_id='" . addslashes($oldValues->department_country_id) . "'
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
						if (!$oldValues->department_country_id || $oldValues->department_country_id == '')
						{
							echo("selected='selected'");
						}
						echo(">&nbsp;</option>");
						while ($row = $result->FetchRow())
						{
							echo("<option value='" . $row["provstate_id"] . "'");
							if ((!($oldValues->department_provstate_id) && ($row["provstate_id"] == 1)) || (($oldValues) && ($oldValues->department_provstate_id == $row["provstate_id"])))
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

				echo(($pendingEmployer->company_provstate_id) ? "<br /><b class='black'>" . getProvstateName($pendingEmployer->company_provstate_id) . "</b>" : "");
			}
		
			echo("</td>");
		echo("</tr>");
	}

	if (($department_location_info == USE_COMPANY && $company_row["region_id"])
	|| !$department_location_info)
	{
		echo("<tr>");
			echo("<td align='right'>Region<br />(If in Canada):</td>");
			echo("<td>");
			
			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["region_id"]) ? getRegionName($company_row["region_id"]) : "&nbsp;");
				echo(($pendingEmployer->company_region_id) ? "<br /><b class='black'>" . getRegionName($pendingEmployer->company_region_id) . "</b>" : "");
			}
			else
			{
				echo("<select name='department_region_id' onchange='update_city()'>");
		
				if ($oldValues->department_country_id == '' || !$oldValues->department_country_id)
				{
					echo("<option value=''>Choose a Country</option>");
				}
				elseif (!isset($oldValues->department_provstate_id))
				{
					echo("<option value=''>Choose a Province/State</option>");
				}
				else
				{
					$sql = ("
						SELECT DISTINCT region_id, region_name
						FROM region_list
						WHERE provstate_id='" . addslashes($oldValues->department_provstate_id) . "'
						ORDER BY region_id
						");
					$result = $GLOBALS['dbh']->Execute($sql);
					if ($result->RecordCount())
					{
						echo("<option value='' ");
						if (!$oldValues->department_region_id || $oldValues->department_region_id == '')
						{
							echo("selected='selected'");
						}
						echo(">&nbsp;</option>");
						while ($row = $result->FetchRow())
						{
							echo("<option value='" . $row["region_id"] . "' ");
							if ($oldValues->department_region_id == $row["region_id"])
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

				echo(($pendingEmployer->company_region_id) ? "<br /><b class='black'>" . getRegionName($pendingEmployer->company_region_id) . "</b>" : "");
			}
			
			echo("</td>");
		echo("</tr>");
	}

	if (($department_location_info == USE_COMPANY && $company_row["city"])
	|| !$department_location_info)
	{
		echo("<tr>");
			echo("<td align='right'>City:</td>");
			echo("<td align='left'>");
			
			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["city"]) ? $company_row["city"] : "&nbsp;");
				echo(($pendingEmployer->company_city) ? "<br /><b class='black'>" . $pendingEmployer->company_city . "</b>" : "");
			}
			else
			{
				echo("<input type='text' name='department_city' size='40' maxlength='40' value='" . $oldValues->department_city . "' />");
				echo(($pendingEmployer->company_city) ? "<br /><b class='black'>" . $pendingEmployer->company_city . "</b>" : "");
			}
			
			echo("</td>");
		echo("</tr>");
	}

	if (($department_location_info == USE_COMPANY && $company_row["postal_code"])
	|| !$department_location_info)
	{
		echo("<tr>");
			echo("<td align='right'>Postal/Zip Code:</td>");
			echo("<td align='left'>");
			
			if ($department_location_info == USE_COMPANY)
			{
				echo(($company_row["postal_code"]) ? $company_row["postal_code"] : "&nbsp;");
				echo(($pendingEmployer->company_postal_code) ? "<br /><b class='black'>" . $pendingEmployer->company_postal_code . "</b>" : "");
			}
			else
			{
				echo("<input type='text' name='department_postal_code' size='10' maxlength='10' value='" . $oldValues->department_postal_code . "' />");
				echo(($pendingEmployer->company_postal_code) ? "<br /><b class='black'>" . $pendingEmployer->company_postal_code . "</b>" : "");
			}

			echo("</td>");
		echo("</tr>");
	}

	echo("</table>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Division Description:</td>");
	echo("<td align='left'>");
	echo("<textarea name='department_description' rows='10' cols='40'>" . $oldValues->department_description . "</textarea>");
	echo(($pendingEmployer->company_description) ? "<br /><b class='black'>" . $pendingEmployer->company_description . "</b>" : "");
	echo("</td>");
echo("</tr>");

echo("<tr>");
        echo("<td colspan='2'><hr /></td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'>");
	echo("<input type='submit' value='Save Changes' />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

	
echo("</table>");
echo("</center>");

echo("</form>");

?>
