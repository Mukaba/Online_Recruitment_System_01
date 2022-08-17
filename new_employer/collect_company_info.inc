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
 | Filename: collect_company_info.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file displays the form to collect inputs from the user     |
 | if it is their first time logging into the system.                           |
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
                index = document.form1.company_country_id[document.form1.company_country_id.selectedIndex].value;
                prov = prov_array[index];
                if (document.form1.company_country_id[document.form1.company_country_id.selectedIndex].value == "")
                {
			message = "Choose a Country";
                        document.form1.company_provstate_id.options.length = 1;
                        document.form1.company_provstate_id.options[0] = new Option(message, "");
                        document.form1.company_provstate_id.selectedIndex = 0;
                        document.form1.company_region_id.options.length = 1;
			            document.form1.company_region_id.options[0] = new Option(message, "");
                        document.form1.company_region_id.selectedIndex = 0;
			            document.form1.company_city.value = '';
                }
                else
                {
                        if (prov.length==1 && prov[0] == "NULL"){
                                document.form1.company_provstate_id.options.length = 1;
                                document.form1.company_provstate_id.options[0] = new Option("N/A", "");
                                document.form1.company_provstate_id.selectedIndex = 0;
                                document.form1.company_region_id.options.length=1;
                                document.form1.company_region_id.options[0] = new Option("N/A", "");
                                document.form1.company_region_id.selectedIndex = 0;
				document.form1.company_city = '';

                        }else{

                                document.form1.company_provstate_id.options.length =0;
                                document.form1.company_provstate_id.options.length = prov.length;
                                for($i=0;$i<prov.length;$i++){
                                        if (prov[$i] == "NULL")
                                        {
                                                document.form1.company_provstate_id.options[$i] = new Option(" ", "");
                                        }
                                        else
                                        {
                                                document.form1.company_provstate_id.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                                        }
                                }
                                document.form1.company_provstate_id.selectedIndex = 0;

                                index2=document.form1.company_provstate_id[document.form1.company_provstate_id.selectedIndex].value;
                                region = region_array[index2];
                                if (document.form1.company_provstate_id[document.form1.company_provstate_id.selectedIndex].value == "")
                                {
                                        if (document.form1.company_country_id[document.form1.company_country_id.selectedIndex].value == "")
                                        {
						message = "Choose a Country";
                                        }
                                        else
                                        {
						message = "Choose a Province/State";
                                        }
                                        document.form1.company_region_id.options.length = 1;
                                        document.form1.company_region_id.options[0] = new Option(message, "");
                                        document.form1.company_region_id.selectedIndex = 0;
                                }
                                else
                                {
                                        if (region.length==1 && region[0] == "NULL"){
                                                document.form1.company_region_id.options.length=1;
                                                document.form1.company_region_id.options[0] = new Option("N/A", "");
                                        }else{
                                                document.form1.company_region_id.options.length=0;
                                                document.form1.company_region_id.options.length=region.length;
                                                for($i=0;$i<region.length;$i++){
                                                        document.form1.company_region_id.options[$i] = new Option(region[$i][1], region[$i][0]);
                                                }
                                        }
                                }
                        }
                }
        }

        function update_region(){

                document.form1.company_region_id.options.length=0;
                index2=document.form1.company_provstate_id[document.form1.company_provstate_id.selectedIndex].value;
                region = region_array[index2];
                if (document.form1.company_provstate_id[document.form1.company_provstate_id.selectedIndex].value == "")
                {
                        if (document.form1.company_country_id[document.form1.company_country_id.selectedIndex].value == "")
                        {
				message = "Choose a Country";
                        }
                        else
                        {
                                message = "Choose a Province/State";
                        }
			
                        document.form1.company_region_id.options.length = 0;
                        document.form1.company_region_id.options[0] = new Option(message, "");
                        document.form1.company_region_id.selectedIndex = 0;
                }
                else
                {
                        if (region.length==1 && region[0] == "NULL"){
                                document.form1.company_region_id.options.length=1;
                                document.form1.company_region_id.options[0] = new Option("N/A", "");
                        }else{
                                document.form1.company_region_id.options.length = region.length;
                                for($i=0;$i<region.length;$i++){
                                        if (region[$i] == "NULL")
                                        {
                                                document.form1.company_region_id.options[$i] = new Option(" ", "");
                                        }
                                        else
                                        {
                                                document.form1.company_region_id.options[$i] = new Option(region[$i][1], region[$i][0]);
                                        }
                                }
                        }
                }
        }

	function update_city()
        {
                index3 = document.form1.company_region_id[document.form1.company_region_id.selectedIndex].value;

		if (index3)
		{
                	city = city_array[index3];
                	document.form1.company_city.value = city;
		}
        }

//-->
</script>

<?php

echo("<form method='post' action='" . $PHP_SELF . "' name='form1'>");
echo("<input type='hidden' name='select' value='new_employer' />");
echo("<input type='hidden' name='newEmployer' value='" . packObject($newEmployer) . "' />");

echo("<h3>Company Information Form</h3>");

?>


<center>
<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
	<td align='right'>Company Name: <b class='red'>*</b></td>
	<td align='left'><input type='text' name='company_name' size='50' maxlength='75' value="<?php echo($newEmployer->company_name); ?>" /></td>
</tr>

<tr>
	<td align='right'>Company Phone:</td>
	<td align='left'><input type='text' name='company_phone' size='22' maxlength='22' value='<?php echo($newEmployer->company_phone); ?>' /></td>
</tr>

<tr>
	<td align='right'>Company Fax:</td>
	<td align='left'><input type='text' name='company_fax' size='22' maxlength='22' value='<?php echo($newEmployer->company_fax); ?>' /></td>
</tr>

<tr>
	<td align='right'>Company E-mail:</td>
	<td align='left'><input type='text' name='company_email' size='40' value='<?php echo($newEmployer->company_email); ?>' /></td>
</tr>

<tr>
	<td align='right'>Company Website:</td>
	<td align='left'><input type='text' name='company_website' size='45' maxlength='60' value='<?php echo($newEmployer->company_website); ?>' /></td>
</tr>
<?php

$sql = ("
	SELECT DISTINCT size_id, size_range
	FROM employer_sizes
	ORDER BY size_id
	");
$result = $GLOBALS['dbh']->Execute($sql);

?>

<tr>
	<td align='right'>Company Size:</td>
	<td align='left'>
	<select name='company_size_id'>
	<?php
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["size_id"] . "' ");
		if (($newEmployer->company_size_id == $row["size_id"])
		|| (!$newEmployer->company_size_id && $row["size_range"] == 'Unknown'))
		{
			echo("selected='selected'");
		}
		echo(">" . $row["size_range"] . "</option>");
	}
	?>
	</select>
	</td>
</tr>

<?php

$sql = ("
	SELECT DISTINCT *
	FROM company_type
	ORDER BY type_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<tr>");
	echo("<td align='right'>Company's Type: <b class='red'>*</b></td>");
	echo("<td align='left'>");
	echo("<select name='company_type_id'>");

	echo("<option value='' ");
	echo((!$newEmployer->company_type_id) ? "selected='selected'" : "");
	echo(">Please choose a company type</option>");
	
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["type_id"] . "' ");
		echo(($newEmployer->company_type_id == $row["type_id"]) ? "selected='selected'" : "");
		echo(">" . $row["type_name"] . "</option>");
	}
	
	echo("</select>");
	echo("</td>");
echo("</tr>");

$sql = ("
	SELECT DISTINCT industry_id, industry_name
	FROM industries
	WHERE industry_name!='?'
	ORDER BY industry_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

?>

<tr>
	<td align='right'>Company's Industry: <b class='red'>*</b></td>
	<td align='left'>
	<select name='company_industry_id'>
	<?php

	echo("<option value='' ");
        if (!$newEmployer || !$newEmployer->company_industry_id)
        {
                echo(" selected='selected'");
        }
        echo(">Please choose an industry</option>");
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["industry_id"] . "' ");
		if ($newEmployer->company_industry_id == $row["industry_id"])
		{
			echo("selected='selected'");
		}
		echo(">" . htmlentities($row["industry_name"]) . "</option>");
	}
	?>
	</select>
	</td>
</tr>

<tr>
	<td align='right'>Department Name (if applicable):</td>
	<td align='left' nowrap='nowrap'>
	<input type='text' name='company_department_name' value='<?php echo($newEmployer->company_department_name); ?>' size='50' maxlength='75' />
	</td>
</tr>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
	<td align='center' colspan='2'>Company Location:</td>
</tr>

<tr>
	<td colspan='2' align='center'>
	<table cellpadding='5' cellspacing='0' border='0' class='row0'>
	<tr>
		<td align='right'>Street Address 1:</td>
		<td align='left'><input type='text' name='company_street_address_1' size='45' maxlength='75' value="<?php echo($newEmployer->company_street_address_1); ?>" /></td>
	</tr>
	<tr>
		<td align='right'>Street Address 2 (optional):</td>
		<td align='left'><input type='text' name='company_street_address_2' size='45' maxlength='75' value="<?php echo($newEmployer->company_street_address_2); ?>" /></td>
	</tr>
	<tr>
		<td align='right'>Street Address 3 (optional):</td>
		<td align='left'><input type='text' name='company_street_address_3' size='45' maxlength='75' value="<?php echo($newEmployer->company_street_address_3); ?>" /></td>
	</tr>
	<tr>
		<td align='right'>Country:</td>
		<td>
		<select name="company_country_id" onchange='update_prov()'>
		<?php
		echo("<option value='' ");
		if (isset($newEmployer->company_country_id))
                {
                        if (!$newEmployer->company_country_id)
                        {
                                echo("selected='selected'");
                        }
                }
                elseif (!isset($newEmployer->company_country_id) && !$newEmployer->contact_country_id)
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
			if (isset($newEmployer->company_country_id))
                        {
                                if ($newEmployer->company_country_id == $row["country_id"])
                                {
                                        echo(" selected='selected'");
                                }
                        }
                        elseif ($newEmployer->contact_country_id == $row["country_id"])
                        {
                                        echo(" selected='selected'");
                        }
                        echo(">" . $row["country_name"] . "</option>");
                }
                ?>
                </select>
	</tr>
	<tr>
		<td align='right'>Province/State:</td>
		<td align='left'>
		<select name="company_provstate_id" onchange="update_region()">
                <?php

		if (!isset($newEmployer->company_country_id) && $newEmployer->contact_country_id == '')
                {
                        echo("<option value=''>Choose a Country</option>");
                }
                elseif (isset($newEmployer->company_country_id) && !$newEmployer->company_country_id)
                {
                        echo("<option value=''>Choose a Country</option>");
                }
                else
                {
	                /*
	                 A country has been selected if we're at this point, so pull that countries provinces
			 or states out of the database and display them.
	                */
	
			if (isset($newEmployer->company_country_id)) {
                $country_id = $newEmployer->company_country_id;
			} else {
                $country_id = $newEmployer->contact_country_id;
			}
            $sql = ("
                SELECT DISTINCT provstate_id, provstate_name
                FROM provstate_list
                WHERE country_id = ?
                ORDER BY provstate_id
                ");
            $stmt = $GLOBALS['dbh']->Prepare($sql);
			$result = $GLOBALS['dbh']->Execute($stmt, array($country_id));
			if ($result->RecordCount())
			{
			
				/*
				 If the chosen country has provinces or states, display them,
				 otherwise show N/A.
				*/
			
				echo("<option value='' ");
				if (isset($newEmployer->company_provstate_id))
                                {
                                        if (!$newEmployer->company_provstate_id)
                                        {
                                                echo("selected='selected'");
                                        }
                                }
                                elseif (!$newEmployer->contact_provstate_id)
                                {
                                        echo("selected='selected'");
                                }
				echo(">&nbsp;</option>");
				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["provstate_id"] . "'");
					if (isset($newEmployer->company_provstate_id))
                                        {
                                                if ($newEmployer->company_provstate_id == $row["provstate_id"])
                                                {
                                                        echo(" selected='selected'");
                                                }
                                        }
                                        elseif ($newEmployer->contact_provstate_id == $row["provstate_id"])
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
                ?>
                </select>
	</tr>
	<tr>
		<td align='right'>Region<br />(If in Canada):</td>
		<td align='left'>
		<select name="company_region_id" onchange='update_city()'>
                <?php

		if (isset($newEmployer->company_country_id) && $newEmployer->company_country_id == '')
                {
                        echo("<option value=''>Choose a Country</option>");
                }
                elseif (isset($newEmployer->company_country_id) && $newEmployer->company_provstate_id == '')
                {
                        if ($newEmployer->company_country_id == '1' || $newEmployer->company_country_id == '2')
                        {
                                echo("<option value=''>Choose a Province/State</option>");
                        }
                        else
                        {
                                echo("<option value='' selected='selected'>N/A</option>");
                        }
                }
                elseif (!isset($newEmployer->company_country_id) && $newEmployer->contact_country_id == '')
                {
                        echo("<option value=''>Choose a Country</option>");
                }
                elseif (!isset($newEmployer->company_country_id) && !$newEmployer->contact_provstate_id)
                {
                        if ($newEmployer->contact_country_id == '1' || $newEmployer->contact_country_id == '2')
                        {
                                echo("<option value=''>Choose a Province/State</option>");
                        }
                        else
                        {
                                echo("<option value='' selected='selected'>N/A</option>");
                        }
                }
                else
                {
			if (isset($newEmployer->company_provstate_id)) {
                $provstate_id = $newEmployer->company_provstate_id;
			} else {
                $provstate_id = $newEmployer->contact_provstate_id;
			}
            $sql = ("
                SELECT DISTINCT region_id, region_name
                FROM region_list
                WHERE provstate_id = ?
                ORDER BY region_id
                ");
            $stmt = $GLOBALS['dbh']->Prepare($sql);
			$result = $GLOBALS['dbh']->Execute($stmt, array($provstate_id));
			if ($result->RecordCount())
			{
				echo("<option value='' ");
				if (isset($newEmployer->company_region_id))
				{
					if (!$newEmployer->company_region_id)
					{
						echo("selected='selected'");
					}
				}
				elseif (!$newEmployer->contact_region_id)
				{
					echo("selected='selected'");
				}
				echo(">&nbsp;</option>");
				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["region_id"] . "' ");
					if (isset($newEmployer->company_region_id))
					{
						if ($newEmployer->company_region_id == $row["region_id"])
						{
							echo("selected='selected'");
						}
					}
					elseif ($newEmployer->contact_region_id == $row["region_id"])
					{
						echo(" selected='selected'");
					}
					echo(">" . $row["region_name"] . "</option>");
				}
			}
			else
			{
				echo("<option value='' selected='selected'>N/A</option>");
			}
                }
                ?>
                </select>
		</td>
	</tr>

	<?php
	echo("<tr>");
                echo("<td align='right'>City:</td>");
                echo("<td align='left'>");

                echo("<input type='text' name='company_city' size='40' maxlength='40' value=\"");
                echo(($newEmployer->company_city) ? $newEmployer->company_city : ($newEmployer->contact_city ? $newEmployer->contact_city : ""));
                echo("\" />");

                echo("</td>");
        echo("</tr>");
	?>

	<tr>
		<td align='right'>Postal/Zip Code:</td>
		<td align='left'><input type='text' name='company_postal_code' size='10' maxlength='10' value="<?php echo($newEmployer->company_postal_code); ?>" /></td>
	</tr>
	</table>
	</td>
</tr>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
	<td align='right'>Company Description:</td>
	<td align='left'><textarea name='company_description' rows='10' cols='40'><?php echo($newEmployer->company_description); ?></textarea></td>
</tr>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
	<td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class=red>*</b>) are required.</td>
</tr>
<tr>
	<td align='center' colspan='2'>
	<hr />
	<input type='submit' value='Continue' />
	<input type='hidden' name='continue' value='confirm_company_information' />
	&nbsp;&nbsp;&nbsp;&nbsp;
	<input type='reset' value='Reset Form' />
	</td>
</tr>

	
</table>
</center>

</form>
