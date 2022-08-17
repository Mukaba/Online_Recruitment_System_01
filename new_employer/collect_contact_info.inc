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
 | Filename: collect_contact_info.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file displays the form to collect contact inputs from      |
 | the user.                                                                    |
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
for($i=0;$i<$max_country_id;$i++){
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
                $str = "Array(\"" . $row["region_id"] . "\", \"" . $row["region_name"] . "\"),";
        }
        $region_array[$row["provstate_id"]] .= $str;

        if ($row["provstate_id"] > $max_prov_id){
                $max_prov_id = $row["provstate_id"];
        }
}
$str = "Array(";
for($i=0;$i<$max_prov_id;$i++){
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
<script type="text/javascript" language="javascript">
<!--javascript

	var city_array = new <?php echo($city_array); ?>;
        var region_array = new <?php echo $region_array?>;
        var prov_array = new <?php echo $prov_state_array?>;
        var cntry_array = new <?php echo $country_array?>;

        function update_prov(){
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
                        if (prov.length==1 && prov[0] == "NULL"){
                                document.form1.contact_provstate_id.options.length = 1;
                                document.form1.contact_provstate_id.options[0] = new Option("N/A", "");
                                document.form1.contact_provstate_id.selectedIndex = 0;
                                document.form1.contact_region_id.options.length=1;
                                document.form1.contact_region_id.options[0] = new Option("N/A", "");
                                document.form1.contact_region_id.selectedIndex = 0;

                        }else{

                                document.form1.contact_provstate_id.options.length =0;
                                document.form1.contact_provstate_id.options.length = prov.length;
                                for($i=0;$i<prov.length;$i++){
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

        function update_region(){

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
                        if (region.length==1 && region[0] == "NULL"){
                                document.form1.contact_region_id.options.length=1;
                                document.form1.contact_region_id.options[0] = new Option("N/A", "");
                        }else{
                                document.form1.contact_region_id.options.length = region.length;
                                for($i=0;$i<region.length;$i++){
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
		}
		else
		{
			city = "";
		}

                document.form1.contact_city.value = city;
        }

//-->
</script>

<form method='post' action='<?php echo($PHP_SELF); ?>' name='form1'>
<input type='hidden' name='select' value='new_employer' />

<h3>Contact Information Form</h3>

<center>
<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>

<tr>
	<td align='center' colspan='2'>&nbsp;</td>
</tr>


<tr>
	<td align='right'>Title:</td>
	<td align='left'>
	<?php

        $sql = ("
                SELECT DISTINCT title_id, title_name
                FROM title
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        echo("<select name='contact_title'>");
	echo("<option value='' ");
	echo(!$newEmployer->contact_title ? "selected='selected'" : "");
	echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
                echo("<option value='" . $row["title_id"] . "' ");
                if ($newEmployer->contact_title == $row["title_id"])
                {
                        echo("selected='selected'");
                }
                echo(">" . $row["title_name"] . "</option>");
        }
        echo("</select>");

        ?>
	</td>
</tr>

<tr>
	<td align='right'>First name: <B class='red'>*</b></td>
	<td align='left'><input type='text' name='contact_first_name' maxlength='40' size='22' value="<?php echo($newEmployer->contact_first_name); ?>" /></td>
</tr>

<tr>
	<td align='right'>Last name: <B class='red'>*</b></td>
	<td align='left'><input type='text' name='contact_last_name' maxlength='40' size='22' value="<?php echo($newEmployer->contact_last_name); ?>" /></td>
</tr>

<tr>
	<td align='right'>Middle name:</td>
	<td align='left'><input type='text' name='contact_middle_name' maxlength='40' size='22' value="<?php echo($newEmployer->contact_middle_name); ?>" /></td>
</tr>

<tr>
	<td align='right'>E-mail address: <B class='red'>*</b></td>
	<td align='left'><input type='text' name='contact_email' maxlength='100' size='40' value='<?php echo($newEmployer->contact_email); ?>' /></td>
</tr>

<tr>
	<td align='right'>Phone number:</td>
	<td align='left' nowrap='nowrap'>
	<input type='text' name='contact_phone' size='22' maxlength='22' value='<?php echo($newEmployer->contact_phone); ?>' />
	</td>
</tr>

<tr>
	<td align='right'>Fax number:</td>
	<td align='left'>
	<input type='text' name='contact_fax' size='22' maxlength='22' value='<?php echo($newEmployer->contact_fax); ?>' />
	</td>
</tr>

<tr>
	<td align='right'>Pager number:</td>
	<td align='left'><input type='text' name='contact_pager' size='22' maxlength='25' value='<?php echo($newEmployer->contact_pager); ?>' /></td>
</tr>

<tr>
	<td align='right'>Cellphone number:</td>
	<td align='left'><input type='text' name='contact_cellphone' size='22' maxlength='25' value='<?php echo($newEmployer->contact_cellphone); ?>' /></td>
</tr>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
	<td align='center' colspan='2'>Location:</td>
</tr>

<tr>
	<td colspan='2' align='center'>
	<table cellpadding='5' cellspacing='0' border='0' class='row0'>
	<?php

	echo("<tr>");
		echo("<td align='right'>Street Address 1:</td>");
		echo("<td align='left'>");
		echo("<input type='text' name='contact_street_address_1' size='45' maxlength='75' value=\"" . $newEmployer->contact_street_address_1 . "\" />");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Street Address 2 (optional):</td>");
		echo("<td align='left'>");
		echo("<input type='text' name='contact_street_address_2' size='45' maxlength='75' value=\"" . $newEmployer->contact_street_address_2 . "\" />");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Street Address 3 (optional):</td>");
		echo("<td align='left'>");
		echo("<input type='text' name='contact_street_address_3' size='45' maxlength='75' value=\"" . $newEmployer->contact_street_address_3 . "\" />");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Country:</td>");
		echo("<td>");
		echo("<select name='contact_country_id' onchange='update_prov()'>");

		echo("<option value='' ");
		if (isset($newEmployer->contact_country_id))
		{
			if (!$newEmployer->contact_country_id)
			{
				echo("selected='selected'");
			}
		}
		elseif (!isset($newEmployer->contact_country_id) && !$newEmployer->company_country_id)
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
			if (isset($newEmployer->contact_country_id))
			{
				if ($newEmployer->contact_country_id == $row["country_id"])
				{
					echo(" selected='selected'");
				}
			}
			elseif ($newEmployer->company_country_id == $row["country_id"])
			{
					echo(" selected='selected'");
			}
			echo(">" . $row["country_name"] . "</option>");
		}

		echo("</select>");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Province/State:</td>");
		echo("<td align='left'>");

		echo("<select name='contact_provstate_id' onchange='update_region()'>");

		if ($newEmployer->contact_country_id == '')
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
                                WHERE country_id = ?
                                ORDER BY provstate_id
                                ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $result = $GLOBALS['dbh']->Execute($stmt,
                                array($newEmployer->contact_country_id));
                        if ($result->RecordCount())
                        {

                                /*
                                 If the chosen country has provinces or states, display them,
                                 otherwise show N/A.
                                */

                                echo("<option value='' ");
                                if (!$newEmployer->contact_country_id || $newEmployer->contact_country_id == '')
                                {
                                        echo("selected='selected'");
                                }
                                echo(">&nbsp;</option>");
                                while ($row = $result->FetchRow())
                                {
                                        echo("<option value='" . $row["provstate_id"] . "'");
                                        if ((!($newEmployer->contact_provstate_id) && ($row["provstate_id"] == 1)) || (($newEmployer) && ($newEmployer->contact_provstate_id == $row["provstate_id"])))
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

		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Region<br />(if in Canada):</td>");
		echo("<td align'left'>");

		echo("<select name='contact_region_id' onchange='update_city()'>");

		if ($newEmployer->contact_country_id == '')
                {
                        echo("<option value=''>Choose a Country</option>");
                }
                elseif (!($newEmployer->contact_provstate_id))
                {
                        echo("<option value=''>Choose a Province/State</option>");
                }
                else
                {
                        $sql = ("
                                SELECT DISTINCT region_id, region_name
                                FROM region_list
                                WHERE provstate_id = ?
                                ORDER BY region_id
                                ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $result = $GLOBALS['dbh']->Execute($stmt,
                                array($newEmployer->contact_provstate_id));
                        if ($result->RecordCount())
                        {
                                echo("<option value='' ");
                                if (!$newEmployer->contact_region_id || $newEmployer->contact_region_id == '')
                                {
                                        echo("selected='selected'");
                                }
                                echo(">&nbsp;</option>");
                                while ($row = $result->FetchRow())
                                {
                                        echo("<option value='" . $row["region_id"] . "' ");
                                        if ($newEmployer->contact_region_id == $row["region_id"])
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

		echo("</td>");
	echo("</tr>");
	
	echo("<tr>");
		echo("<td align='right'>City:</td>");
		echo("<td align='left'>");

		echo("<input type='text' name='contact_city' size='40' maxlength='40' value=\"");
		echo(($newEmployer->contact_city) ? $newEmployer->contact_city : ($newEmployer->company_city ? $newEmployer->company_city : ""));
		echo("\" />");
		
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Postal/Zip Code:</td>");
		echo("<td align='left'>");
		
		echo("<input type='text' name='contact_postal_code' size='10' maxlength='10' value=\"");
		echo($newEmployer->contact_postal_code);
		echo("\" />");
		
		echo("</td>");
	echo("</tr>");

	echo("</table>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<hr />");
	echo("Fields marked with an asterisk (<B class='red'>*</b>) are required.");
	echo("</td>");
echo("</tr>");

?>

<tr>
	<td align='center' colspan='2'>
	<hr />
	<input type='submit' value='Continue' />
	<input type='hidden' name='continue' value='confirm_contact_information' />
	</td>
</tr>

</table>
</center>

</form>
