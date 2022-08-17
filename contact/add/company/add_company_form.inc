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
 | Filename: add_company_form.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This file displays the form to collect inputs from the user     |
 | when they are adding a new company to the system.  These inputs are then     |
 | returned to add_company.inc to be verified and then saved.                   |
 +------------------------------------------------------------------------------+

*/


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

$result = $GLOBALS['dbh']->Execute("
    SELECT c.country_id, provstate_id, provstate_name
    FROM country_list as c 
    LEFT JOIN provstate_list as p ON c.country_id = p.country_id
    ORDER BY c.order_by
    ");
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
            //$str = "Array(),";
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
        //$str .= "Array(),";
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

$result = $GLOBALS['dbh']->Execute("
    SELECT p.provstate_id, r.region_id, r.region_name
    FROM provstate_list as p 
    LEFT JOIN region_list as r ON p.provstate_id = r.provstate_id
    ORDER BY p.provstate_id
    ");

while ($row = $result->FetchRow())
{
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
            //$str = "Array(),";
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

echo("<form method='post' action='" . $URL . "' name='form1'>");
echo("<input type='hidden' name='level1' value='company' />");

?>


<center>
<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
	<td align='right'>Company Name: <b class='red'>*</b></td>
	<td align='left'><input type='text' name='company_name' size='50' maxlength='75' value="<?php echo($contact->company_name); ?>" /></td>
</tr>

<?php
/*
 :EXPIRY: This code should no longer be used, as company statuses are set immediately as CONTACT_POTENTIAL_LEAD upon entry
 Expires: Dec 07, 2003
 Expired by: Adam
<tr>
	<td align='right'>Company Status: <b class='red'>*</b></td>
	<td align='left'>
	<select name='company_status'>
	<?php
	
	$sql = ("
		SELECT DISTINCT status_id, status_name
		FROM employer_status
		ORDER BY status_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["status_id"] . "' ");
		echo((!$company_status && $row["status_id"] == '1') || $company_status == $row["status_id"] ? "checked='checked'" : "");
		echo(">" . $row["status_name"] . "</option>");
	}

	?>
	</select>
	</td>
</tr>
*/
?>

<tr>
	<td align='right'>Company Phone:</td>
	<td align='left'><input type='text' name='company_phone' size='22' maxlength='22' value='<?php echo($contact->company_phone); ?>' /></td>
</tr>

<tr>
	<td align='right'>Company Fax:</td>
	<td align='left'><input type='text' name='company_fax' size='22' maxlength='22' value='<?php echo($contact->company_fax); ?>' /></td>
</tr>

<tr>
	<td align='right'>Company E-mail:</td>
	<td align='left'><input type='text' name='company_email' size='40' value='<?php echo($contact->company_email); ?>' /></td>
</tr>

<tr>
	<td align='right'>Company Website:</td>
	<td align='left'><input type='text' name='company_website' size='45' maxlength='60' value='<?php echo($contact->company_website); ?>' /></td>
</tr>
<?php

$sql = ("
	SELECT size_id, size_range
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
		if (($contact->company_size_id == $row["size_id"])
		|| (!$contact->company_size_id && $row["size_range"] == 'Unknown'))
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

echo("<tr>");
	echo("<td align='right'>Company Type: <b class='red'>*</b></td>");
	echo("<td align='left'>");
	
	$sql = ("
		SELECT type_id, type_name
		FROM company_type
		ORDER BY type_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	echo("<select name='company_type_id'>");
	echo("<option value='' ");
	echo((!$contact->company_type_id) ? "selected='selected'" : "");
	echo(">Please choose a type</option>");
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["type_id"] . "' ");
		echo(($contact->company_type_id == $row["type_id"]) ? "selected='selected'" : "");
		echo(">" . $row["type_name"] . "</option>");
	}
	echo("</select>");
	echo("</td>");
echo("</tr>");

$sql = ("
	SELECT industry_id, industry_name
	FROM industries
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
	if (!$contact->company_industry_id)
	{
		echo(" selected='selected'");
	}
	echo(">Please choose an industry</option>");
	while ($row = $result->FetchRow())
	{
		if ($row["industry_name"] != '?')
		{
			echo("<option value='" . $row["industry_id"] . "' ");
			if ($contact->company_industry_id == $row["industry_id"])
			{
				echo("selected='selected'");
			}
			echo(">" . htmlentities($row["industry_name"]) . "</option>");
		}
	}
	?>
	</select>
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
		<td align='left'><input type='text' name='company_street_address_1' size='45' maxlength='75' value="<?php echo($contact->company_street_address_1); ?>" /></td>
	</tr>
	<tr>
		<td align='right'>Street Address 2 (optional):</td>
		<td align='left'><input type='text' name='company_street_address_2' size='45' maxlength='75' value="<?php echo($contact->company_street_address_2); ?>" /></td>
	</tr>
	<tr>
		<td align='right'>Street Address 3 (optional):</td>
		<td align='left'><input type='text' name='company_street_address_3' size='45' maxlength='75' value="<?php echo($contact->company_street_address_3); ?>" /></td>
	</tr>
	<tr>
		<td align='right'>Country:</td>
		<td>
		<select name="company_country_id" onchange='update_prov()'>
		<?php
		echo("<option value='' ");
                if (!$contact->company_country_id || $contact->company_country_id == '')
                {       
                        echo("selected='selected'");
                }       
                echo(">&nbsp;</option>");
                $sql = ("
                        SELECT country_id, country_name
                        FROM country_list
                        ORDER BY order_by 
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row = $result->FetchRow())
                {
                        echo("<option value='" . $row["country_id"] . "'");
                        if (($contact) && ($contact->company_country_id == $row["country_id"]))
                        {
                                echo(" selected='selected'");
                        }
                        echo(">" . $row["country_name"] . "</option>");
                }
                ?>
                </select>
                </td>
	</tr>
	<tr>
		<td align='right'>Province/State:</td>
		<td>
		<select name="company_provstate_id" onchange="update_region()">
                <?php

		if ($contact->company_country_id == '')
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
				SELECT provstate_id, provstate_name
				FROM provstate_list
				WHERE country_id='" . addslashes($contact->company_country_id) . "'
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
				if (!$contact->company_country_id || $contact->company_country_id == '')
				{
					echo("selected='selected'");
				}
				echo(">&nbsp;</option>");
				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["provstate_id"] . "'");
					if ((!($contact->company_provstate_id) && ($row["provstate_id"] == 1)) || (($contact) && ($contact->company_provstate_id == $row["provstate_id"])))
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
        </td>
	</tr>
	<tr>
		<td align='right'>Region<br />(If in Canada):</td>
		<td>
		<select name="company_region_id" onchange='update_city()'>
        <?php

		if ($contact->company_country_id == '')
                {
                        echo("<option value=''>Choose a Country</option>");
                }
                elseif (!isset($contact->company_provstate_id))
                {
                        echo("<option value=''>Choose a Province/State</option>");
                }
                else
                {
			$sql = ("
                                SELECT region_id, region_name
                                FROM region_list
                                WHERE provstate_id='" . $contact->company_provstate_id . "'
                                ORDER BY region_id
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        if ($result->RecordCount())
                        {
                                echo("<option value='' ");
                                if (!$contact->company_region_id || $contact->company_region_id == '')
                                {
                                        echo("selected='selected'");
                                }
                                echo(">&nbsp;</option>");
                                while ($row = $result->FetchRow())
                                {
                                        echo("<option value='" . $row["region_id"] . "' ");
                                        if ($contact->company_region_id == $row["region_id"])
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
                ?>
                </select>
		</td>
	</tr>
	<tr>
		<td align='right'>City:</td>
		<td align='left'><input type='text' name='company_city' size='40' maxlength='40' value="<?php echo($contact->company_city); ?>" /></td>
	</tr>
	<tr>
		<td align='right'>Postal/Zip Code:</td>
		<td align='left'><input type='text' name='company_postal_code' size='10' maxlength='10' value="<?php echo($contact->company_postal_code); ?>" /></td>
	</tr>
	</table>
	</td>
</tr>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>


<?php
	
$sql = ("
	SELECT DISTINCT a.flag_name, a.flag_id
	FROM company_flags AS a, department_company_flags AS b
	WHERE b.department_id='" . $auth->department . "'
	AND a.flag_id=b.flag_id
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{	

	// If there are company flags for this co-op department, then display them and let them check them.

	echo("<tr>");
		echo("<td colspan='2' align='center'>Company Flags</td>");
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
			echo("<td nowrap='nowrap'><input type='checkbox' class='row2' name='company_flags[]' value='" . $row["flag_id"] . "' ");
			if (is_array($contact->company_flags) && in_array($row["flag_id"], $contact->company_flags))
			{
				echo("checked='checked'");
			}
			echo(" />" . $row["flag_name"] . "</td>");
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
		echo("</tr>");
		
		echo("</table>");
		echo("</td>");
	echo("</tr>");
}

?>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
	<td align='right'>Company Description:</td>
	<td align='left'><textarea name='company_description' rows='10' cols='40'><?php echo($contact->company_description); ?></textarea></td>
</tr>

<tr>
	<td align='right'>Notes:</td>
	<td align='left'><textarea name='company_new_note' rows='5' cols='40'><?php echo($contact->company_new_note); ?></textarea></td>
</tr>



<?php
/*

This part will not be released at UVic.

<tr>
	<td align='right'>Comments specific<br />to your department:</td>
	<td align='left'><textarea name='company_department_comments' rows='10' cols='40'><?php echo($contact->company_department_comments); ?></textarea></td>
</tr>

*/
?>


<tr>
        <td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class='red'>*</b>) are required</td>
</tr>

<tr>
	<td align='center' colspan='2'><hr />
	<input type='submit' name='continue' value='Continue' />
	</td>
</tr>

	
</table>
</center>

</form>
