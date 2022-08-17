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
 | Filename: view_contact_criteria.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is called by view_contact_header.inc and is used to   |
 | gather search inputs from the user.                                          |
 +------------------------------------------------------------------------------+

*/

$max_area_id=0;
$area_array = array();

$sql = ("
	SELECT a.area_id, country_id, country_name
	FROM area_list AS a 
    LEFT JOIN country_list AS c 
    ON a.area_id=c.area_id
	ORDER BY area_id, c.order_by
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


// We need to put blank values at the start of every pull down array


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
    LEFT JOIN provstate_list as p 
    ON c.country_id = p.country_id
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

$result = $GLOBALS['dbh']->Execute("
    SELECT p.provstate_id, r.region_id, r.region_name
    FROM provstate_list as p 
    LEFT JOIN region_list as r 
    ON p.provstate_id = r.provstate_id
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
?>

<script type="text/javascript" language="javascript">
<!--javascript

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
	
    function update_prov()
    {
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

            }
            
            else
            {

                document.myform.search_provstate.options.length =0;
                document.myform.search_provstate.options.length = prov.length;
                for($i=0;$i<prov.length;$i++)
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
                    }
                    
                    else
                    {
                        document.myform.search_region.options.length=0;
                        document.myform.search_region.options.length=region.length;
                        
                        for($i=0;$i<region.length;$i++)
                        {
                            document.myform.search_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                        }
                    }
                }
            }
        }
    }

    function update_region()
    {
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
            if (region.length==1 && region[0] == "NULL")
            {
                document.myform.search_region.options.length=1;
                document.myform.search_region.options[0] = new Option("N/A", "");
            }
            
            else
            {
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

	//-->

</script>
<script type="text/javascript" language="javascript">
<!--javascript

    function clearOptions(select_name,skip_column,max_columns)
    {
        var current_column = 1 + skip_column;
        for (var i=0; i < document.myform.elements.length; i++)
        {
            var e = document.myform.elements[i];
            var string = select_name + "[" + current_column + "]";
            if (e.name == string)
            {
                e.selectedIndex = 0;
                current_column++;
            }
            if (current_column > max_columns)
            {
                break;
            }
        }
    }

//-->
</script>


<?php

echo("<table border='0' cellpadding='8' cellspacing='0' width='97%' class='row1'>");

if (sizeof($error_array) && is_array($error_array))
{
    echo("<tr>");
        echo("<td align='center' colspan='2'>");
        foreach($error_array AS $error_msg)
        {
            error($error_msg);
        }
        echo("</td>");
    echo("</tr>");
}

echo("<tr>");
	echo("<td align='center' colspan='2'>");
    echo("<form method='post' name='myform2' action='".$PHP_SELF."'>");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='contact' />");
    echo("<input type='submit' name='switch_search' value='Go to Simple Search' />");
	echo("</form>");
    echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'>");
        echo("<b class='black'>The list of contacts returned will match the criteria entered below.  ");
        echo("<br />Leave any fields blank that you do not wish to be used in the search.</b>");
	echo("</td>");
echo("</tr>");
echo("<tr>");
    echo("<td colspan='2'><hr width='50%' size='1' /></td>");
echo("</tr>");

//if there are any saved searches for student advanced search they are now shown here
//in a drop down box
$s_sql = ("  
    SELECT search_name, search_id
    FROM search
    WHERE contact_id = '".$auth->contact_id."'
    AND search_module_id = '".CONTACT_ADV_SEARCH."'
    ORDER BY search_name
    ");

$s_results = $GLOBALS['dbh']->Execute($s_sql);
if($s_results->RecordCount() > 0)
{
    echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("<form method='post' name='myform3' action='".$PHP_SELF."'>");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='contact' />");
    echo("<input type='hidden' name='continue' value='View' />");
    echo("<input type='hidden' name='search_comp_or_div' value='contact' />");
    echo("<input type='hidden' name='btnSearch' value='Search' />");
    echo("Search: ");
    echo("<select name='search_id'>");

    while ($s_row = $s_results->FetchRow())
    {
        echo("<option value='".$s_row["search_id"]."'>".$s_row["search_name"]."</option>");
    }
    echo("</select>");
    echo("&nbsp;&nbsp;<input type='submit' value='Perform Search' />&nbsp;");
    echo("<input type='submit' name='continue' value='Manage My Saved Searches' />");
    echo("</form>");

    echo("</td>");
    echo("</tr>");
}

//:TODO: Rename search_first_name and search_first_name_m to something meaningful. Currently there is a variable name conflict. 
echo("<tr>");
echo("<td>");

echo("<form method='post' name='myform' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='select' value='view_contact' />");
echo("<input type='hidden' name='level1' value='contact' />");

echo("<table width='100%'>");
echo("<tr>");
	echo("<td align='right' width='50%'>First names starting with:</td>");
	echo("<td align='left' width='50%'>");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='contact' />");

   
    if(strlen($search_first_name) > 1)
    {
        $search_first_name_m = $search_first_name;
        unset($search_first_name);
    }
    
	echo("<select name='search_first_name'>");
		echo("<option value='' ");
		if (!$search_first_name || $search_first_name == '')
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</option>");
		for ($i = A; $i < Z; $i++)
		{
			echo("<option value='" . $i . "' ");
			if ($search_first_name && $search_first_name == $i)
			{
				echo("selected='selected'");
			}
			echo(">".$i."</option>");
		}
		echo("<option value='$i' ");
		if ($search_first_name && $search_first_name == $i)
		{
			echo("selected='selected'");
		}
		echo(">".$i."</option>");
	echo("</select>");
	echo("&nbsp;or&nbsp;");
	echo("<input type='text' name='search_first_name_m' size='10' maxlength='10'");
    
    if(trim($search_first_name_m))
	{
		echo(" value=\"" . removeSlashes(trim($search_first_name_m)) . "\"");
	}
	echo(" />");
    
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Last names starting with:</td>");
	echo("<td align='left'>");
    if(strlen($search_last_name) > 1)
    {
        $search_last_name_m = $search_last_name;
        unset($search_last_name);
    }
	echo("<select name='search_last_name'>");
		echo("<option value='' ");
		if (!$search_last_name || $search_last_name == '')
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</option>");
		for ($i = A; $i < Z; $i++)
		{
			echo("<option value='" . $i . "' ");
			if ($search_last_name && $search_last_name == $i)
			{
				echo("selected='selected'");
			}
			echo(">" . $i . "</option>");
		}
		echo("<option value='" . $i . "' ");
		if ($search_last_name && $search_last_name == $i)
		{
			echo("selected='selected'");
		}
		echo(">" . $i . "</option>");
	echo("</select>");
	echo("&nbsp;or&nbsp;");
	echo("<input type='text' name='search_last_name_m' size='10' maxlength='10' ");

	if ($search_last_name_m && $search_last_name_m != '')
	{
		echo(" value=\"" . removeSlashes(trim($search_last_name_m)) . "\"");
	}
	echo(" />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' nowrap='nowrap' align='center'>");
        echo("<table border='0'>");
            echo("<tr>");
                echo("<td>Return contacts with a status of:&nbsp;&nbsp;&nbsp;</td>");

                $sql = ("
                    SELECT status_id, status_name
                    FROM employer_info_statuses
                    ORDER BY level_of_activity
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);

                echo("<td>");

                while ($row = $result->FetchRow())
                {
                    echo("<input type='checkbox' class='row1' name='search_contact_status_status[]' value='" . $row["status_id"] . "' ");
                    if ($search_contact_status_status && in_array($row["status_id"], $search_contact_status_status))
                    {
                        echo("checked='checked'");
                    }
                    echo(" />" . $row["status_name"]);
                    echo("<br />");
                }

                echo("</td>");
                
                $sql = ("
                    SELECT department_id, department_name
                    FROM department
                    WHERE using_full_system = '1'
                    ORDER BY department_name
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                
                echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;in&nbsp;&nbsp;</td>");
                echo("<td>");

                while ($row = $result->FetchRow())
                {
                    echo("<input type='checkbox' class='row1' name='search_contact_status_department[]' value='" . $row["department_id"] . "' ");
                    if (is_array($search_contact_status_department) && sizeof($search_contact_status_department) > 0 
                        && in_array($row["department_id"], $search_contact_status_department))
                    {
                        echo("checked='checked'");
                    }
                    // If the user loads a saved search, and no departments have been selected, then don't check off default departments. 
                    elseif((!is_array($search_contact_status_department) || sizeof($search_contact_status_department) == 0) && strlen($search_id))
                    {
                    }
                    // Otherwise, check off the default department. 
                    elseif(sizeof($search_contact_status_department) == 0 && $row["department_id"] == $auth->department)
                    {
                        echo("checked='checked'");
                    }
                    echo(" />" . $row["department_name"]);
                    echo("<br />");
                }

                echo("</td>");
            echo("</tr>");
        echo("</table>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>E-mail address:&nbsp;");
	echo("<input type='radio' class='row1' name='search_email_as' value='start'");
	if ($search_email_as == 'start' || !$search_email_as)
	{
		echo("checked='checked'");
	}
	echo(" />Starting with&nbsp;");
	echo("<input type='radio' class='row1' name='search_email_as' value='containing'");
	if ($search_email_as == 'containing')
	{
		echo("checked='checked'");
	}
	echo(" />Containing&nbsp;");
    echo("</td>");
	/*  Commented out by: Jon
        Date: Oct 8, 2004

        Reason: This code has been commented out because it gets in the way of 
        of the containing option and the text box. Replace if problems occur. 
       
    echo("<input type='radio' class='row1' name='search_email_as' value='isblank'");
	if ($search_email_as == 'isblank')
	{
		echo("checked='checked'");
	}
	echo(" />Is Blank&nbsp;");
	echo("</td>");*/
	echo("<td align='left'>");
	echo("<input type='text' name='search_email' size='30'");
	if ($search_email)
	{
		echo(" value=\"" . removeSlashes(trim($search_email)) . "\"");
	}
	echo(" />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Phone number:&nbsp;");
	echo("<input type='radio' class='row1' name='search_phone_as' value='start'");
	if ($search_phone_as == 'start' || !$search_phone_as)
	{
		echo("checked='checked'");
	}
	echo(" />Starting with&nbsp;");
	echo("<input type='radio' class='row1' name='search_phone_as' value='containing'");
	if ($search_phone_as == 'containing')
	{
		echo("checked='checked'");
	}
	echo(" />Containing&nbsp;");
	echo("</td>");
	echo("<td align='left'>");
	echo("<input type='text' name='search_phone' size='20'");
	if ($search_phone)
	{
		echo(" value=\"" . removeSlashes(trim($search_phone)) . "\"");
	}
	echo(" />");
	echo("</td>");
echo("</tr>");
echo("<tr>");
echo("<td align='right'>Cellphone number:&nbsp;");
echo("<input type='radio' class='row1' name='search_cellphone_as' value='start'");
if ($search_cellphone_as == 'start' || !$search_cellphone_as)
{
    echo("checked='checked'");
}   
echo(" />Starting with&nbsp;");
echo("<input type='radio' class='row1' name='search_cellphone_as' value='containing'");
if ($search_cellphone_as == 'containing')
{
    echo("checked='checked'");
}
echo(" />Containing&nbsp;");
echo("</td>");
echo("<td align='left'>");
echo("<input type='text' name='search_cellphone' size='20'");
if ($search_cellphone)
{
    echo(" value=\"" . removeSlashes(trim($search_cellphone)) . "\"");
}
echo(" />");
echo("</td>");
echo("</tr>");
                                                                                                                            

echo("<tr>");
	echo("<td align='right'>Fax number:&nbsp;");
	echo("<input type='radio' class='row1' name='search_fax_as' value='start'");
	if ($search_fax_as == 'start' || !$search_fax_as)
	{
		echo("checked='checked'");
	}
	echo(" />Starting with&nbsp;");
	echo("<input type='radio' class='row1' name='search_fax_as' value='containing'");
	if ($search_fax_as == 'containing')
	{
		echo("checked='checked'");
	}
	echo(" />Containing&nbsp;");
	echo("</td>");
	echo("<td align='left'>");
	echo("<input type='text' name='search_fax' size='20'");
	if ($search_fax)
	{
		echo(" value=\"" . removeSlashes(trim($search_fax)) . "\"");
	}
	echo(" />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Works for a company starting with:</td>");
	echo("<td align='left'>");
	echo("<input type='text' id='search_company_name' name='search_company_name' size='30'");
	if ($search_company_name)
	{
		echo(" value=\"" . removeSlashes(trim($search_company_name)) . "\"");
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
    echo("<tr>");
        echo("<td colspan='2' align='center'>-Or-</td>");
    echo("</tr>");

    echo("<tr>");
        echo("<td colspan='2' align='center'>");
            echo("<input type='button' name='company_list' value='Choose from a list' onclick='popUpChooser(\"search_company_name\");' />");
        echo("</td>");
    echo("</tr>");
}                       

echo("<tr>");
	echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Works for a division starting with:</td>");
	echo("<td align='left'>");
	echo("<input type='text' name='search_division_name' size='30'");
	if ($search_division_name)
	{
		echo(" value=\"" . removeSlashes(trim($search_division_name)) . "\"");
	}
	echo(" />");
	
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Works in a department starting with:</td>");
	echo("<td align='left'>");
	echo("<input type='text' name='search_department_name' size='30'");
	if ($search_department_name)
	{
		echo(" value=\"" . removeSlashes(trim($search_department_name)) . "\"");
	}
	echo(" />");
	
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'>Contacts located in:</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2'>");
        echo("<table align='center' cellpadding='3' cellspacing='3'>");
            echo("<tr align='center'>");
                echo("<td align='center' class='row2'>");
                    echo("<table width='100%' class='row2'>");
                        echo("<tr align='center'>");
                            echo("<td align='right'>Area:</td>");
                            echo("<td align='left'>");
                                echo("<select name='search_area' onchange='update_country()'>");
                                
                                echo("<option value='' ");
                                if (!$search_area || $search_area == '')
                                {
                                        echo("selected='selected'");
                                }
                                echo(">&nbsp;</option>");
                                $sql = ("
                                        SELECT area_id, area_name
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
                            echo("</td>");
                        echo("</tr>");

                        echo("<tr>");
                            echo("<td align='right'>Country:</td>");
                            echo("<td align='left'>");
                            echo("<select name='search_country' onchange='update_prov()'>");

                            
                            // Tricky bit of code here.  If for some reason the page has been refreshed, we need to be careful about
                            // what we put in these location select bars.  If the area has been chosen, then we need to display the
                            // appropriate countries.  If not, then we need to display a blank and a 'Choose an Area'.
                            

                            if ($search_area == '')
                            {
                                    echo("<option value=''>Choose an Area</option>");
                            }
                            else
                            {
                                // An area has been selected.

                                $sql = ("
                                        SELECT country_id, country_name
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
                            echo("</td>");
                        echo("</tr>");

                        echo("<tr>");
                            echo("<td align='right'>Province/State:</td>");
                            echo("<td align='left'>");
                            echo("<select name='search_provstate' onchange='update_region()'>");

                            
                            // More tricky nonsense here.  The same situation as above, except that now we have to take into account
                            // the situations of an Area being selected but no Country, or no Area AND no Country.  Ugh.
                            

                            if ($search_area == '')
                            {
                                echo("<option value=''>Choose an Area</option>");
                            }
                            elseif ($search_country == '')
                            {
                                echo("<option value=''>Choose a Country</option>");
                            }
                            else
                            {
                                
                                // A country and area have been selected.
                                
                                $sql = ("
                                    SELECT provstate_id, provstate_name
                                    FROM provstate_list
                                    WHERE country_id='" . $search_country . "'
                                    ORDER BY provstate_id
                                    ");
                                $result = $GLOBALS['dbh']->Execute($sql);
                                if ($result->RecordCount())
                                {
                                    
                                    //  If the chosen country has provinces/states associated with it, displays those and a blank
                                    //  as choices.  Otherwise display N/A.
                                     

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
                            echo("</td>");
                        echo("</tr>");

                        echo("<tr>");
                            echo("<td align='right'>Region:</td>");
                            echo("<td align='left'>");
                            echo("<select name='search_region'>");

                            
                            // Here's the last task.  This is the trickiest.  We have to check if we're dealing with a blank Area, Country
                            // or Province/State, and display the appropriate choices (this is only really critical when someone updates the
                            // page somehow).
                            
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
                                    SELECT region_id, region_name
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
                            echo("</td>");
                        echo("</tr>");

                        echo("<tr>");
                            echo("<td align='right'>City starting with:</td>");
                            echo("<td align='left'><input type='text' size='20' name='search_city'");

                            if ($search_city)
                            {
                                echo("VALUE=\"" . removeSlashes(trim($search_city)) . "\"");
                            }
                            echo(" />");
                            echo("</td>");
                        echo("</tr>");

                        echo("<tr>");
                            echo("<td align='right'>Postal/Zip Code starting with:</td>");
                            echo("<td align='left'><input type='text' size='10' name='search_postal_code'");

                            if ($search_postal_code)
                            {
                                echo("VALUE =\"" . removeSlashes(trim($search_postal_code)) . "\"");
                            }
                            echo(" />");
                            echo("</td>");
                        echo("</tr>");
                    echo("</table>");
                echo("</td>");
            echo("</tr>");
        echo("</table>");
    echo("</td>");
echo("</tr>");
echo("<tr>");
	echo("<td colspan='2' align='center'><br />Contact Action:</td>");
echo("</tr>");

    //WHERE action_id NOT IN ('".GENERIC_NOTE."','".CONDUCTED_WORKSITE_VISIT."','".INT_CONDUCTED."','".FINAL_PLACEMENT."','".JOB_DESC_RECEIVED."')
$sql = ("
	SELECT action_id, action_name
	FROM action_types
	ORDER BY action_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($department_id == '0' || $department_id == '') 
{
    $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
    $department_in_str = "(" . $department_in_str . ")";
} 

else 
{
    $department_in_str = "('$department_id')";
}

echo("<tr>");
    echo("<td colspan='2'>");
        echo("<table align='center' cellpadding='3' cellspacing='3'>");
            echo("<tr align='center'>");
                echo("<td align='center' class='row2'>");
                    echo("<table width='100%' class='row2'>");
                        echo("<tr align='center'>");
                            echo("<td align='center' colspan='2'>Action:&nbsp;");
                            echo("<select name='search_action'>");
                            echo("<option value='' ");
                            if (!$search_action)
                            {
                                echo("selected='selected'");
                            }
                            echo(">&nbsp;</option>");
                            while ($row = $result->FetchRow())
                            {
                                echo("<option value='" . $row["action_id"] . "' ");
                                if ($search_action == $row["action_id"])
                                {
                                    echo("selected='selected'");
                                }
                                echo(">" . $row["action_name"] . "</option>");
                            }
                            echo("</select>");
                            echo("</td>");
                        echo("</tr>");

                        // Pull out a list of internal staff who are in this department and has
                        // performed an action at some point. 

                        $sql_action_by = ("
                            SELECT ci.contact_id, c.first_name, c.last_name, ci.netlink_id 
                            FROM contact AS c 
                            INNER JOIN contact_internal AS ci 
                            ON c.contact_id = ci.contact_id 
                            INNER JOIN contact_actions_set AS cas 
                            ON cas.action_by = ci.contact_id 
                            WHERE cas.action_id NOT IN ('".GENERIC_NOTE."','".CONDUCTED_WORKSITE_VISIT."','".INT_CONDUCTED."',
                                '".JOB_DESC_RECEIVED."','".FINAL_PLACEMENT."')
                            ");
                        if ($auth->department == DIRECTORS_OFFICE_DEPARTMENT)
                        {
                            $sql_dept =  ("
                                SELECT DISTINCT department_id
                                FROM department
                                ");

                            $result_dept = $GLOBALS['dbh']->Execute($sql_dept);
                            while ($row_dept = $result_dept->FetchRow())
                            {
                                $departments_in_group2[] = $row_dept["department_id"];
                            }
                            $department_in_str2 = "'" . implode("', '", $departments_in_group2) . "'";
                            $department_in_str2 = "(" . $department_in_str2 . ")";

                            $sql_action_by .= ("
                                AND ci.department_id IN ".$department_in_str2."  
                            ");
                        } 
                        else 
                        {
                            $sql_action_by .= ("
                                AND ci.department_id IN ".$department_in_str."  
                            ");
                        }

                        $sql_action_by .= ("
                            GROUP BY c.first_name, c.last_name, ci.netlink_id
                            ORDER BY c.last_name, c.first_name
                            ");
                        $result_action_by = $GLOBALS['dbh']->Execute($sql_action_by);
                        
                        echo("<tr align='center'>");
                            echo("<td align='center' colspan='2'>Action Performed By:&nbsp;");
                            echo("<select name='search_action_by'>");
                            echo("<option value='' ");
                            if (!$search_action_by)
                            {
                                echo("selected='selected'");
                            }
                            echo(">&nbsp;</option>");
                            while ($row_action_by = $result_action_by->FetchRow())
                            {
                                echo("<option value='" . $row_action_by["contact_id"] . "' ");
                                if ($search_action_by == $row_action_by["contact_id"])
                                {
                                    echo("selected='selected'");
                                }
                                if($row_action_by["first_name"]|| $row_action_by["last_name"]){
                                    echo(">" . $row_action_by["first_name"]." " . $row_action_by["last_name"] ." </option>");
                                }
                                else{
                                    echo(">&nbsp;</option>");
                                }
                            }    
                            echo("</select>");
                            echo("</td>");
                        echo("</tr>");

                        echo("<tr>");
                            echo("<td align='center' colspan='2' nowrap='nowrap'><br />Action Last Performed:&nbsp;<br />");
                            echo("<input type='radio' class='row2' name='search_action_as' value='on' ");
                            if ($search_action_as == 'on' || !$search_action_as)
                            {
                                echo("checked='checked'");
                            }
                            echo(" />On&nbsp;");
                            echo("<input type='radio' class='row2' name='search_action_as' value='before' ");
                            if ($search_action_as == 'before')
                            {
                                echo("checked='checked'");
                            }
                            echo(" />Before&nbsp;");
                            echo("<input type='radio' class='row2' name='search_action_as' value='after' ");
                            if ($search_action_as == 'after')
                            {
                                echo("checked='checked'");
                            }
                            echo(" />After&nbsp;&nbsp;");
                            echo("(YYYY-MM-DD) ");
                            echo("<input type='text' name='search_action_on' value='".$search_action_on."' />&nbsp;");
                            echo(popup('search_action_on','myform'));
                            echo("<br />");

                            echo("<br />-or-<br />");

                            echo("<br />Action Last Performed Between:<br />");

                            echo("(YYYY-MM-DD) ");
                            echo("<input type='text' name='search_action_between1' value='".$search_action_between1."' />&nbsp;");
                            echo(popup('search_action_between1','myform'));
                            echo(" and ");
                            echo("(YYYY-MM-DD) ");
                            echo("<input type='text' name='search_action_between2' value='".$search_action_between2."' />&nbsp;");
                            echo(popup('search_action_between2','myform'));
                            echo("</td>");
                        echo("</tr>");
                    echo("</table>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td>");
                    echo("&nbsp;");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='center' class='row2'>");
                    echo("<table width='100%' class='row2'>");
                        echo("<tr align='center'>");
                            echo("<td align='center' colspan='2' nowrap='nowrap'>Bring Forward Date:&nbsp;<br />");
                            echo("<input type='radio' class='row2' name='search_action_bf_as' value='on' ");
                            if ($search_action_bf_as == 'on' || !$search_action_bf_as)
                            {
                                echo("checked='checked'");
                            }
                            echo(" />On&nbsp;");
                            echo("<input type='radio' class='row2' name='search_action_bf_as' value='before' ");
                            if ($search_action_bf_as == 'before')
                            {
                                echo("checked='checked'");
                            }
                            echo(" />Before&nbsp;");
                            echo("<input type='radio' class='row2' name='search_action_bf_as' value='after' />After ");
                            if ($search_action_bf_as == 'after')
                            {
                                echo("checked='checked'");
                            }
                            echo("&nbsp;&nbsp;");
                            echo("(YYYY-MM-DD) ");
                            echo("<input type='text' name='search_action_bf_on' value='".$search_action_bf_on."' />&nbsp;");
                            echo(popup('search_action_bf_on','myform'));
                            echo("<br />");

                            echo("<br />-or-<br />");

                            echo("<br />Bring Forward Date Between:<br />");

                            echo("(YYYY-MM-DD) ");
                            echo("<input type='text' name='search_action_bf_between1' value='".$search_action_bf_between1."' />&nbsp;");
                            echo(popup('search_action_bf_between1','myform'));
                            echo(" and ");
                            echo("(YYYY-MM-DD) ");
                            echo("<input type='text' name='search_action_bf_between2' value='".$search_action_bf_between2."' />&nbsp;");
                            echo(popup('search_action_bf_between2','myform'));
                            echo("</td>");
                        echo("</tr>");
                    echo("</table>");
                echo("</td>");
            echo("</tr>");
        echo("</table>");
	echo("</td>");
echo("</tr>");

$sql = ("
	SELECT DISTINCT cf.flag_id, cf.flag_name
	FROM contact_flags AS cf
    INNER JOIN department_contact_flags AS dcf
    ON cf.flag_id = dcf.flag_id
	WHERE dcf.department_id='" . $auth->department . "'
    ORDER BY cf.flag_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	echo("<tr>");
		echo("<td colspan='2' align='center'><br />Contact Flags:</td>");
	echo("</tr>");
	echo("<tr>");
		echo("<td colspan='2'>");
            echo("<table align='center' cellpadding='3' cellspacing='3'>");
                echo("<tr align='center'>");
                    echo("<td align='center' class='row2'>");
                        echo("<table width='100%' class='row2'>");
                            echo("<tr>");
                                echo("<td colspan='2' align='center'>Return contacts with&nbsp;");
                                echo("<input type='radio' class='row2' name='search_flags_boolean' value='and' ");
                                if ($search_flags_boolean == 'and' || !$search_flags_boolean)
                                {
                                    echo("checked='checked'");
                                }
                                echo(" />All&nbsp;");
                                echo("<input type='radio' class='row2' name='search_flags_boolean' value='or' ");
                                if ($search_flags_boolean == 'or')
                                {
                                    echo("checked='checked'");
                                }
                                echo(" />Any&nbsp;");
                                echo("of the flags below checked&nbsp;");
                                echo("</td>");
                            echo("</tr>");
                            
                            echo("<tr>");
                                echo("<td colspan='2'>&nbsp;</td>");
                            echo("</tr>");

                            echo("<tr>");
                                echo("<td>Yes/No</td>");
                                echo("<td>Yes/No</td>");
                            echo("</tr>");
                            
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
                                echo("<input type='checkbox' class='row2' name='checked_nflags[]' value='" . $row["flag_id"] . "' ");
                                if (is_array($checked_nflags) && in_array($row["flag_id"], $checked_nflags))
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
                        echo("</table>");
                    echo("</td>");
                echo("</tr>");
            echo("</table>");
		echo("</td>");
	echo("</tr>");
}

include("contact/view/contact/display_array.inc");

// function clearOptions is used to remove all pre-selected ordering/display columns and set them to blank. Notice
// that below, I the drop down menus that controls displaying of columns are named column[1],column[2],etc, while 
// ordering is named order[1],order[2], etc. So, the first parameter in clearOptions is the array name. ie: column
// or order. Next parameter is skip_column. Some ordering/display columns may have a pre-selected field that 
// cannot be changed. For instance, for displaying columns, you will ALWAYS have last name and first name. These
// occupy column[1] and column[2]. So by setting skip_column = 2, we skip over these fields that we cannot reset. 
// Last is max_columns, and that's the number of drop down menus in display/order. Once we hit this upper limit, we
// stop. 
?>

<?php
echo("<tr>");
    echo("<td class='row1' colspan='2' align='center'><br />Display the following fields in the search results:<br /><br />");
        echo("<table cellspacing='0' cellpadding='4' border='0' class='row0'>");

            // Everytime we hit more columns, we will get x more columns. 
            $column_iteration = 8;
            
            // This figures out the maximum number of columns an individual can view. We take the number of viewable columns (from $display_array)
            // and round up so it can be divided by $column_iteration. IE If column_iteration = 8 and length of $display_array = 15, we would make
            // the maximum number of viewable columns to be 16. 
            $remainder = sizeof($display_array) % $column_iteration;
            $difference = $column_iteration - $remainder;
            $max_columns = sizeof($display_array) + $difference;
            // The minimum number of columns viewable.  
            $min_columns = $column_iteration;
            
            // We just got to this page, so display only $column_iteration number of columns.  
            if (!$num_column)
            {
                if(sizeof($column) > $column_iteration)
                {
                    $remainder = sizeof($column) % $column_iteration;
                    $difference = $column_iteration - $remainder;
                    $num_column = sizeof($column) + $difference;
                }
                else
                {
                    $num_column = $column_iteration;
                }
            }

            // Show more columns
            elseif($more_columns && $num_column <= $max_columns)
            {
                $num_column = $num_column + $column_iteration;
            }

            // Show less columns
            elseif($less_columns && $num_column >= $min_columns)
            {
                $num_column = $num_column - $column_iteration;
            }

            // Get default values
            $contact_first_name_index = 0;
            $contact_last_name_index = 0;
            $contact_phone_index = 0;
            $contact_cellphone_index = 0;
            $contact_email_index = 0;
            $division_name_index = 0;
            $company_name_index = 0;

            for($i=0;$i<sizeof($display_array);$i++)
            {
                switch($display_array[$i]["name"])
                {
                    case "Contact First Name":
                        $contact_first_name_index = $i;
                        break;
                    case "Contact Last Name":
                        $contact_last_name_index = $i;
                        break;
                    case "Contact Phone Number":
                        $contact_phone_index = $i;
                        break;
                    case "Contact Cell Number":
                        $contact_cellphone_index = $i;
                        break;
                    case "Contact E-mail":
                        $contact_email_index = $i;
                        break;
                    case "Company Name":
                        $company_name_index = $i;
                        break;
                    case "Division Name":
                        $division_name_index = $i;
                        break;
                        
                }
            }

            for ($column_number=1;$column_number<=$num_column;$column_number++)
            {
                // Start new table row. 
                if ($column_number%2 == 1)
                {
                    echo("<tr>");
                }
                echo("<td>Column ".$column_number.":</td>");
                echo("<td>");
                if ($column_number == 1)
                {
                    echo("<input type='hidden' name='column[".$column_number."]' value='".$contact_first_name_index."' />Contact First Name");
                }
                elseif ($column_number == 2)
                {
                    echo("<input type='hidden' name='column[".$column_number."]' value='".$contact_last_name_index."' />Contact Last Name");
                }

                else
                {
                    echo("<select name='column[".$column_number."]'>");
                    $selected = "";
                    for ($i=0;$i<sizeof($display_array);$i++)
                    {
                        // If user reloads this page and has selected an option here, preserve that option.
                        if (is_array($column))
                        {
                            if ($column[$column_number] == $i)
                            {
                                $selected = "selected='selected'";
                            }
                        }
                        // Otherwise, here are the default settings for the drop down boxes. 
                        elseif ($column_number >= 3 && $column_number <= 7 && !$more_columns && !$less_columns)
                        {
                            switch ($column_number)
                            {
                                case 3:
                                    $selected_value = $contact_phone_index;
                                    break;
                                case 4:
                                    $selected_value = $contact_cellphone_index;
                                    break;
                                case 5:
                                    $selected_value = $contact_email_index;
                                    break;
                                case 6:
                                    $selected_value = $division_name_index;
                                    break;
                                case 7:
                                    $selected_value = $company_name_index;
                                    break;
                                default:
                                    $selected_value = 0;
                            }

                            if ($i == $selected_value && $selected_value != "")
                            {
                                $selected = "selected='selected'";
                            }
                        }
                        echo("<option value='".$i."' ".$selected.">".$display_array[$i]["name"]."</option>");
                        $selected = "";
                    }
                    echo("</select>");
                }
                echo("</td>");

                if ($column_number%2 == 0)
                {
                    echo("</tr>");
                }
            }
            echo("<tr>");
                    echo("<td colspan='4' align='right' class='row1'>");
                    // Buttons that control how many columns will be displayed. 
                     echo("<input type='hidden' name='num_column' value='".$num_column."' />");
                    if($btnSearch == "Edit Search Criteria")
                    {
                        echo("<input type='hidden' name='search_id' value='$search_id' />");
                        echo("<input type='hidden' name='edit_page' value='yes' />");
                        echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
                    }

                    echo("<input type='button' name='order' value='Clear Columns' onclick='javascript:clearOptions(\"column\",2,".$num_column.")' />");
                    if ($num_column > $min_columns)
                    {
                        echo("<input type='submit' name='less_columns' value='Less Columns' />");
                    }

                    if ($num_column < $max_columns)
                    {
                        echo("<input type='submit' name='more_columns' value='More Columns' />");
                    }
                echo("</td>");
            echo("</tr>");
        echo("</table>");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2' align='center'>Order search results using:</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2'>");
        echo("<table align='center' class='row0' cellpadding='4' cellspacing='0' border='0'>");

            // Same as columns up above. Check up a few blocks up for more details.  
            $order_iteration = 4;
            
            $max_orders = $order_iteration * 2;
            $min_orders = $order_iteration;
            
            if (!$num_order)
            {
                if(sizeof($order) > $order_iteration)
                {
                    $remainder = sizeof($order) % $order_iteration;
                    $difference = $order_iteration - $remainder;
                    $num_order = sizeof($order) + $difference;
                }
                else
                {
                    $num_order = $order_iteration;
                }
            }

            elseif($more_orders && $num_order <= $max_orders)
            {
                $num_order = $num_order + $order_iteration;
            }

            elseif($less_orders && $num_order >= $min_orders)
            {
                $num_order = $num_order - $order_iteration;
            }

            for ($order_number=1;$order_number<=$num_order;$order_number++)
            {
                if ($order_number%2 == 1)
                {
                    echo("<tr>");
                }
                echo("<td>Order ".$order_number.":</td>");
                echo("<td>");
                    echo("<select name='order[".$order_number."]'>");
                    $selected = "";
                    for ($i=0;$i<sizeof($display_array);$i++)
                    {
                        if (is_array($order))
                        {
                            if ($order[$order_number] == $i)
                            {
                                $selected = "selected='selected'";
                            }
                        }
                        // Otherwise, here are the default settings for the drop down boxes. 
                        elseif ($order_number >= 1 && $order_number <= 2 && !$more_orders && !$less_orders)
                        {
                            switch ($order_number)
                            {
                                case 1:
                                    $selected_value = $contact_last_name_index;
                                    break;
                                case 2:
                                    $selected_value = $contact_first_name_index;
                                    break;
                                default:
                                    $selected_value = 0;
                            }

                            if ($i == $selected_value && $selected_value != "")
                            {
                                $selected = "selected='selected'";
                            }
                        }

                        // we can't order by contact flags, so we're excluding it here. Contact flags come back as a list of contact flags which are comma 
                        // separated. Sorting this doesn't work well. 
                        if ($display_array[$i]["name"] != "Contact Flags")
                        {
                            echo("<option value='".$i."' ".$selected.">".$display_array[$i]["name"]."</option>");
                        }
                        $selected = "";
                    }
                    echo("</select>");
                echo("</td>");

                if ($order_number%2 == 0)
                {
                    echo("</tr>");
                }
            }
            echo("<tr>");
                echo("<td colspan='4' align='right' class='row1'>");
                echo("<input type='hidden' name='num_order' value='".$num_order."' />");
                    if($btnSearch == "Edit Search Criteria")
                    {
                        echo("<input type='hidden' name='search_id' value='$search_id' />");
                        echo("<input type='hidden' name='edit_page' value='yes' />");
                        echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
                    }

               echo("<input type='button' name='order' value='Clear Columns' onclick='javascript:clearOptions(\"order\",0,".$num_order.")' />");
                    if ($num_order > $min_orders)
                    {
                        echo("<input type='submit' name='less_orders' value='Less Columns' />");
                    }
                    if ($num_order < $max_orders)
                    {
                        echo("<input type='submit' name='more_orders' value='More Columns' />");
                    }
                echo("</td>");
            echo("</tr>");

        echo("</table>");
    echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>Would you like to include company and division criteria in your search?&nbsp;");
	echo("<input type='checkbox' class='row1' name='search_company_criteria' value='company' ");
	if ($search_company_criteria == 'company')
	{
		echo("checked='checked'");
	}
	echo(" />Company&nbsp;");
	echo("<input type='checkbox' class='row1' name='search_department_criteria' value='department' ");
	if ($search_department_criteria == 'department')
	{
		echo("checked='checked'");
	}
	echo(" />Division&nbsp;");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'><hr />");
	echo("<input type='hidden' name='continue' value='first_contact_form_done' />");
	echo("<input type='submit' value='Continue' />");
	echo("&nbsp;&nbsp;&nbsp;");
	echo("<input type='button' onclick='javascript:document.reset_form.submit()' value='Reset' />");
	echo("&nbsp;&nbsp;&nbsp;");
    if($btnSearch == "Edit Search Criteria")
    {
        //These variables are only set if you are editing a saved search
        echo("<input type='hidden' name='search_id' value='".$search_id."' />");
        echo("<input type='hidden' name='edit_page' value='yes' />");
        echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
    }
	echo("<input type='submit' name='btnSearch' value='Save Search' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</form>");
echo("</td>");
echo("</tr>");

echo("</table>");
echo("<form name='reset_form' method='post' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='continue' value='Go to Advanced Search' />");
echo("<input type='hidden' name='level1' value='contact' />");
if (strlen($search_id))
{
    echo("<input type='hidden' name='search_id' value='".$search_id."' />");
    echo("<input type='hidden' name='edit_page' value='yes' />");
    echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
}
echo("</form>");

?>
