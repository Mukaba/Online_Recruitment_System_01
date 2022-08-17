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
 | Description: This file is called by view_contact.inc and is used to gather   |
 | search inputs from the user.                                                 |
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
	SELECT DISTINCT area_id
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
<!-- javascript

    var region_array = new <?php echo $region_array?>;
    var prov_array = new <?php echo $prov_state_array?>;
	var cntry_array = new <?php echo $cntry_array?>;

	function update_company_country()
	{
		index3 = document.myform.search_company_area[document.myform.search_company_area.selectedIndex].value;
		country = cntry_array[index3];
		if (document.myform.search_company_area[document.myform.search_company_area.selectedIndex].value == "")
		{
			document.myform.search_company_country.options.length = 1;
			document.myform.search_company_country.options[0] = new Option("Choose an Area", "");
			document.myform.search_company_country.selectedIndex = 0;
			document.myform.search_company_provstate.options.length = 1;
			document.myform.search_company_provstate.options[0] = new Option("Choose an Area", "");
			document.myform.search_company_provstate.selectedIndex = 0;
			document.myform.search_company_region.options.length = 1;
			document.myform.search_company_region.options[0] = new Option("Choose an Area", "");
			document.myform.search_company_region.selectedIndex = 0;
		}
		else
		{
			if (country.length == 1 && country[0] == "NULL")
			{
				document.myform.search_company_country.options.length = 1;
				document.myform.search_company_country[0] = new Option("N/A", "");
				document.myform.search_company_country.selectedIndex = 0;
				document.myform.search_company_provstate.options.length = 1;
				document.myform.search_company_provstate.options[0] = new Option("N/A", "");
				document.myform.search_company_provstate.selectedIndex = 0;
				document.myform.search_company_region.options.length = 1;
				document.myform.search_company_region.options[0] = new Option("N/A", "");
				document.myform.search_company_region.selectedIndex = 0;
			}
			else
			{
				document.myform.search_company_country.options.length = 0;
				document.myform.search_company_country.options.length = country.length;
				for ($i = 0; $i < country.length; $i++)
				{
					if (country[$i] == "NULL")
					{
						document.myform.search_company_country.options[$i] = new Option(" ", "");
					}
					else
					{
						document.myform.search_company_country.options[$i] = new Option(country[$i][1], country[$i][0]);
					}
				}
				document.myform.search_company_country.selectedIndex = 0;
	
				index4 = document.myform.search_company_country[document.myform.search_company_country.selectedIndex].value;
				prov = prov_array[index4];
				if (document.myform.search_company_country[document.myform.search_company_country.selectedIndex].value == "")
                {
                    document.myform.search_company_provstate.options.length = 1;
                    document.myform.search_company_provstate.options[0] = new Option("Choose a Country", "");
                    document.myform.search_company_provstate.selectedIndex = 0;
                    document.myform.search_company_region.options.length = 1;
                    document.myform.search_company_region.options[0] = new Option("Choose a Country", "");
                    document.myform.search_company_region.selectedIndex = 0;
                }
                else
                {
                    if (prov.length == 1 && prov[0] == "NULL")
                    {
                        document.myform.search_company_provstate.options.length = 1;
                        document.myform.search_company_provstate.options[0] = new Option("N/A", "");
                        document.myform.search_company_provstate.selectedIndex = 0;
                        document.myform.search_company_region.options.length = 1;
                        document.myform.search_company_region.options[0] = new Option("N/A", "");
                        document.myform.search_company_region.selectedIndex = 0;
                    }
                    else
                    {
                        document.myform.search_company_provstate.options.length = 0;
                        document.myform.search_company_provstate.length = prov.length;
                        for ($i = 0; $i < prov.length; $i++)
                        {
                            if (prov[$i] == "NULL")
                            {
                                document.myform.search_company_provstate.options[$i] = new Option(" ", "");
                            }
                            else
                            {
                                document.myform.search_company_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                            }
                        }
                        document.myform.search_company_provstate.selectedIndex = 0;

                        index5 = document.myform.search_company_provstate[document.myform.search_company_provstate.selectedIndex].value;
                        region = region_array[index5];
                        if (document.myform.search_company_provstate[document.myform.search_company_provstate.selectedIndex].value == "")
                        {
                            document.myform.search_company_region.options.length = 1;
                            document.myform.search_company_region.options[0] = new Option("Choose a Province/State", "");
                            document.myform.search_company_region.selectedIndex = 0;
                        }
                        else
                        {
                            if (region.length == 1 && region[0] == "NULL")
                            {
                                document.myform.search_company_region.options.length = 1;
                                document.myform.search_company_region.options[0] = new Option("N/A", "");
                                document.myform.search_company_region.selectedIndex = 0;
                            }
							else
							{
								document.myform.search_company_region.options.length = 0;
								document.myform.search_company_region.options.length = region.length;
								for ($i = 0; $i < region.length; $i++)
								{
									if (region[$i] == "NULL")
									{
										document.myform.search_company_region.options[$i] = new Option(" ", "");
									}
									else
									{
										document.myform.search_company_region.options[$i] = new Option(region[$i][1], region[$i][0]);
									}
								}
								document.myform.search_company_region.options.selectedIndex = 0;
                            }
                        }	
                    }	
                }	
            }	
        }
    }	

    function update_company_prov()
    {
        index = document.myform.search_company_country[document.myform.search_company_country.selectedIndex].value;
        prov = prov_array[index];

		if (document.myform.search_company_country[document.myform.search_company_country.selectedIndex].value == "")
		{
			if (document.myform.search_company_area[document.myform.search_company_area.selectedIndex].value == "")
			{
				message = "Choose an Area";
			}
			else
			{
				message = "Choose a Country";
			}
			document.myform.search_company_provstate.options.length = 1;
			document.myform.search_company_provstate.options[0] = new Option(message, "");
			document.myform.search_company_provstate.selectedIndex = 0;
			document.myform.search_company_region.options.length = 1;
			document.myform.search_company_region.options[0] = new Option(message, "");
			document.myform.search_company_region.selectedIndex = 0;
		}
		else
		{
            if (prov.length==1 && prov[0] == "NULL")
            {
                document.myform.search_company_provstate.options.length = 1;
                document.myform.search_company_provstate.options[0] = new Option("N/A", "");
                document.myform.search_company_provstate.selectedIndex = 0;
                document.myform.search_company_region.options.length=1;
                document.myform.search_company_region.options[0] = new Option("N/A", "");
                document.myform.search_company_region.selectedIndex = 0;
            }
            else
            {
                document.myform.search_company_provstate.options.length =0;
                document.myform.search_company_provstate.options.length = prov.length;
                for($i=0;$i<prov.length;$i++)
                {
                    if (prov[$i] == "NULL")
                    {
                        document.myform.search_company_provstate.options[$i] = new Option(" ", "");
                    }
                    else
                    {
                        document.myform.search_company_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                    }
                }
                document.myform.search_company_provstate.selectedIndex = 0;

                index2=document.myform.search_company_provstate[document.myform.search_company_provstate.selectedIndex].value;
                region = region_array[index2];
                if (document.myform.search_company_provstate[document.myform.search_company_provstate.selectedIndex].value == "")
                {
                    if (document.myform.search_company_country[document.myform.search_company_country.selectedIndex].value == "")
					{
						if (document.myform.search_company_area[document.myform.search_company_area.selectedIndex].value == "")
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
					document.myform.search_company_region.options.length = 1;
					document.myform.search_company_region.options[0] = new Option(message, "");
					document.myform.search_company_region.selectedIndex = 0;
				}
				else
				{
                    if (region.length==1 && region[0] == "NULL")
                    {
                        document.myform.search_company_region.options.length=1;
                        document.myform.search_company_region.options[0] = new Option("N/A", "");
                    }
                    else
                    {
                        document.myform.search_company_region.options.length=0;
                        document.myform.search_company_region.options.length=region.length;
                        for($i=0;$i<region.length;$i++)
                        {
                            document.myform.search_company_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                        }
                    }
                }
            }
        }
    }

    function update_company_region()
    {
        document.myform.search_company_region.options.length=0;
        index2=document.myform.search_company_provstate[document.myform.search_company_provstate.selectedIndex].value;
        region = region_array[index2];

		if (document.myform.search_company_provstate[document.myform.search_company_provstate.selectedIndex].value == "")
		{
			if (document.myform.search_company_country[document.myform.search_company_country.selectedIndex].value == "")
			{
				if (document.myform.search_company_area[document.myform.search_company_area.selectedIndex].value == "")
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
		
			document.myform.search_company_region.options.length = 1;
			document.myform.search_company_region.options[0] = new Option(message, "");
			document.myform.search_company_region.selectedIndex = 0;
		}
		else
		{    
            if (region.length==1 && region[0] == "NULL")
            {
                document.myform.search_company_region.options.length=1;
                document.myform.search_company_region.options[0] = new Option("N/A", "");
            }
            else
            {
                document.myform.search_company_region.options.length = region.length;
                for($i=0;$i<region.length;$i++)
                {
                    if (region[$i] == "NULL")
                    {
                        document.myform.search_company_region.options[$i] = new Option(" ", "");
					}
					else
					{
						document.myform.search_company_region.options[$i] = new Option(region[$i][1], region[$i][0]);
					}
                }
            }
            document.myform.search_company_region.selectedIndex = 0;
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

		window.open("mamook.php?select=company_choose&no_headers=1&company_slot="+slot+"&parentFormName="+formName, "CompanyChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes");
	}

	function update_department_country()
	{
		index3 = document.myform.search_department_area[document.myform.search_department_area.selectedIndex].value;
		country = cntry_array[index3];
		if (document.myform.search_department_area[document.myform.search_department_area.selectedIndex].value == "")
		{
			document.myform.search_department_country.options.length = 1;
			document.myform.search_department_country.options[0] = new Option("Choose an Area", "");
			document.myform.search_department_country.selectedIndex = 0;
			document.myform.search_department_provstate.options.length = 1;
			document.myform.search_department_provstate.options[0] = new Option("Choose an Area", "");
			document.myform.search_department_provstate.selectedIndex = 0;
			document.myform.search_department_region.options.length = 1;
			document.myform.search_department_region.options[0] = new Option("Choose an Area", "");
			document.myform.search_department_region.selectedIndex = 0;
		}
		else
		{
			if (country.length == 1 && country[0] == "NULL")
			{
				document.myform.search_department_country.options.length = 1;
				document.myform.search_department_country[0] = new Option("N/A", "");
				document.myform.search_department_country.selectedIndex = 0;
				document.myform.search_department_provstate.options.length = 1;
				document.myform.search_department_provstate.options[0] = new Option("N/A", "");
				document.myform.search_department_provstate.selectedIndex = 0;
				document.myform.search_department_region.options.length = 1;
				document.myform.search_department_region.options[0] = new Option("N/A", "");
				document.myform.search_department_region.selectedIndex = 0;
			}
			else
			{
				document.myform.search_department_country.options.length = 0;
				document.myform.search_department_country.options.length = country.length;
				for ($i = 0; $i < country.length; $i++)
				{
					if (country[$i] == "NULL")
					{
						document.myform.search_department_country.options[$i] = new Option(" ", "");
					}
					else
					{
						document.myform.search_department_country.options[$i] = new Option(country[$i][1], country[$i][0]);
					}
				}
				document.myform.search_department_country.selectedIndex = 0;
	
				index4 = document.myform.search_department_country[document.myform.search_department_country.selectedIndex].value;
				prov = prov_array[index4];
				if (document.myform.search_department_country[document.myform.search_department_country.selectedIndex].value == "")
                {
                    document.myform.search_department_provstate.options.length = 1;
                    document.myform.search_department_provstate.options[0] = new Option("Choose a Country", "");
                    document.myform.search_department_provstate.selectedIndex = 0;
                    document.myform.search_department_region.options.length = 1;
                    document.myform.search_department_region.options[0] = new Option("Choose a Country", "");
                    document.myform.search_department_region.selectedIndex = 0;
                }
                else
                {
                    if (prov.length == 1 && prov[0] == "NULL")
                    {
                        document.myform.search_department_provstate.options.length = 1;
                        document.myform.search_department_provstate.options[0] = new Option("N/A", "");
                        document.myform.search_department_provstate.selectedIndex = 0;
                        document.myform.search_department_region.options.length = 1;
						document.myform.search_department_region.options[0] = new Option("N/A", "");
						document.myform.search_department_region.selectedIndex = 0;
					}
					else
					{
						document.myform.search_department_provstate.options.length = 0;
						document.myform.search_department_provstate.length = prov.length;
						for ($i = 0; $i < prov.length; $i++)
						{
							if (prov[$i] == "NULL")
							{
								document.myform.search_department_provstate.options[$i] = new Option(" ", "");
							}
							else
							{
								document.myform.search_department_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
							}
						}
						document.myform.search_department_provstate.selectedIndex = 0;
		
						index5 = document.myform.search_department_provstate[document.myform.search_department_provstate.selectedIndex].value;
						region = region_array[index5];
						if (document.myform.search_department_provstate[document.myform.search_department_provstate.selectedIndex].value == "")
						{
							document.myform.search_department_region.options.length = 1;
							document.myform.search_department_region.options[0] = new Option("Choose a Province/State", "");
							document.myform.search_department_region.selectedIndex = 0;
						}
						else
						{
							if (region.length == 1 && region[0] == "NULL")
							{
								document.myform.search_department_region.options.length = 1;
								document.myform.search_department_region.options[0] = new Option("N/A", "");
								document.myform.search_department_region.selectedIndex = 0;
							}
							else
							{
								document.myform.search_department_region.options.length = 0;
								document.myform.search_department_region.options.length = region.length;
								for ($i = 0; $i < region.length; $i++)
								{
									if (region[$i] == "NULL")
									{
										document.myform.search_department_region.options[$i] = new Option(" ", "");
									}
									else
									{
										document.myform.search_department_region.options[$i] = new Option(region[$i][1], region[$i][0]);
									}
								}
								document.myform.search_department_region.options.selectedIndex = 0;
							}
						}	
					}	
				}	
			}	
		}
	}	
	
    function update_department_prov()
    {
        index = document.myform.search_department_country[document.myform.search_department_country.selectedIndex].value;
        prov = prov_array[index];
		if (document.myform.search_department_country[document.myform.search_department_country.selectedIndex].value == "")
		{
			if (document.myform.search_department_area[document.myform.search_department_area.selectedIndex].value == "")
			{
				message = "Choose an Area";
			}
			else
			{
				message = "Choose a Country";
			}
			document.myform.search_department_provstate.options.length = 1;
			document.myform.search_department_provstate.options[0] = new Option(message, "");
			document.myform.search_department_provstate.selectedIndex = 0;
			document.myform.search_department_region.options.length = 1;
			document.myform.search_department_region.options[0] = new Option(message, "");
			document.myform.search_department_region.selectedIndex = 0;
		}
		else
		{
            if (prov.length==1 && prov[0] == "NULL")
            {
                document.myform.search_department_provstate.options.length = 1;
                document.myform.search_department_provstate.options[0] = new Option("N/A", "");
                document.myform.search_department_provstate.selectedIndex = 0;
                document.myform.search_department_region.options.length=1;
                document.myform.search_department_region.options[0] = new Option("N/A", "");
                document.myform.search_department_region.selectedIndex = 0;
            }
            else
            {
                document.myform.search_department_provstate.options.length =0;
                document.myform.search_department_provstate.options.length = prov.length;
                for($i=0;$i<prov.length;$i++)
                {
                    if (prov[$i] == "NULL")
                    {
                        document.myform.search_department_provstate.options[$i] = new Option(" ", "");
                    }
                    else
                    {
                        document.myform.search_department_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                    }
                }
                document.myform.search_department_provstate.selectedIndex = 0;
	
                index2=document.myform.search_department_provstate[document.myform.search_department_provstate.selectedIndex].value;
                region = region_array[index2];
				if (document.myform.search_department_provstate[document.myform.search_department_provstate.selectedIndex].value == "")
				{
					if (document.myform.search_department_country[document.myform.search_department_country.selectedIndex].value == "")
					{
						if (document.myform.search_department_area[document.myform.search_department_area.selectedIndex].value == "")
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
					document.myform.search_department_region.options.length = 1;
					document.myform.search_department_region.options[0] = new Option(message, "");
					document.myform.search_department_region.selectedIndex = 0;
				}
				else
				{
                    if (region.length==1 && region[0] == "NULL")
                    {
                        document.myform.search_department_region.options.length=1;
                        document.myform.search_department_region.options[0] = new Option("N/A", "");
                    }
                    else
                    {
                        document.myform.search_department_region.options.length=0;
                        document.myform.search_department_region.options.length=region.length;
                        for($i=0;$i<region.length;$i++)
                        {
                            document.myform.search_department_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                        }
                    }
                }
            }
        }
    }

    function update_department_region()
    {
        document.myform.search_department_region.options.length=0;
        index2=document.myform.search_department_provstate[document.myform.search_department_provstate.selectedIndex].value;
        region = region_array[index2];
        if (document.myform.search_department_provstate[document.myform.search_department_provstate.selectedIndex].value == "")
        {
            if (document.myform.search_department_country[document.myform.search_department_country.selectedIndex].value == "")
            {
                if (document.myform.search_department_area[document.myform.search_department_area.selectedIndex].value == "")
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
		
			document.myform.search_department_region.options.length = 1;
			document.myform.search_department_region.options[0] = new Option(message, "");
			document.myform.search_department_region.selectedIndex = 0;
		}
		else
		{    
            if (region.length==1 && region[0] == "NULL")
            {
                document.myform.search_department_region.options.length=1;
                document.myform.search_department_region.options[0] = new Option("N/A", "");
            }
            else
            {
                document.myform.search_department_region.options.length = region.length;
                for($i=0;$i<region.length;$i++)
                {
                    if (region[$i] == "NULL")
                    {
                        document.myform.search_department_region.options[$i] = new Option(" ", "");
					}
					else
					{
						document.myform.search_department_region.options[$i] = new Option(region[$i][1], region[$i][0]);
					}
                }
            }
            document.myform.search_department_region.selectedIndex = 0;
        }
    }

//-->

</script>

<?php
echo("<form method='post' name='myform' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='select' value='view_contact'>");
echo("<input type='hidden' name='level1' value='contact'>");
echo("<input type='hidden' name='search_company_criteria' value='".$search_company_criteria."'>");
echo("<input type='hidden' name='search_department_criteria' value='".$search_department_criteria."'>");
echo("<input type='hidden' name='searchContact' value='".packObject($searchContact)."'>");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='96%' class='row1'>");
if (sizeof($error_array) > 0 )
{
    echo("<tr>");
    echo("<td colspan='2'>");
    foreach ($error_array as $error_msg)
    {
        error($error_msg);
    }
    echo("</td>");
    echo("</tr>");
}

echo("<tr>");
	echo("<td align='center' colspan='2'><b class='black'><br />");
	echo("The criteria below will allow you to further narrow down your search results based on the <br />");
	echo("company and/or division that contacts matching the previous criteria work for.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($search_company_criteria == 'company')
{
	echo("<tr>");
        echo("<td align='right'>Company name starts with:</td>");
        echo("<td align='left'>");
        echo("<input type='text' name='search_company_name' size='30'");
        if ($searchContact->search_company_name)
        {
            echo(" value='" . trim($searchContact->search_company_name) . "'");
        }       
        echo(">");
        echo("</td>");
	echo("</tr>");
	
	$sql = ("
        SELECT DISTINCT employer_id
        FROM employer_company
        WHERE company_display
        "); 
	$result = $GLOBALS['dbh']->Execute($sql); 
	        
	if ($result->RecordCount())
	{       
        echo("<tr>");
            echo("<td colspan='2' align='center'>-Or-</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td colspan='2' align='center'>");
            echo("<input type='button' name='company_list' value='Choose from a list' onclick='popUpCompany(\"search_company_name\", \"myform\");'>");
            echo("</td>");
        echo("</tr>");
	}                       

	echo("<tr>");
		echo("<td colspan='2' align='center'>&nbsp;</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Company website:&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_website_as' value='start'");
		if ($searchContact->search_company_website_as == 'start' || !$searchContact->search_company_website_as)
		{
			echo(" CHECKED");
		}

		echo(">Starting with&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_website_as' value='containing'");
		if ($searchContact->search_company_website_as == 'containing')
		{
			echo(" CHECKED");
		}
		echo(">Containing&nbsp;");
		echo("</td>");
		echo("<td align='left'>");
		echo("<input type='text' name='search_company_website' size='30' ");
		if ($searchContact->search_company_website)
		{
			echo(" value='" . addslashes(trim($searchContact->search_company_website)) . "'");
		}
		echo(">");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>E-mail address:&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_email_as' value='start'");
		if ($searchContact->search_company_email_as == 'start' || !$searchContact->search_company_email_as)
		{
			echo(" CHECKED");
		}
		echo(">Starting with&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_email_as' value='containing'");
		if ($searchContact->search_company_email_as == 'containing')
		{
			echo(" CHECKED");
		}
		echo(">Containing&nbsp;");
		echo("</td>");

		echo("<td align='left'>");
		echo("<input type='text' name='search_company_email' size='30'");
		if ($searchContact->search_company_email)
		{
			echo(" value='" . addslashes(trim($searchContact->search_company_email)) . "'");
		}
		echo(">");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Phone number:&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_phone_as' value='start'");
		if ($searchContact->search_company_phone_as == 'start' || !$searchContact->search_company_phone_as)
		{
			echo(" CHECKED");
		}
		echo(">Starting with&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_phone_as' value='containing'");
		if ($searchContact->search_company_phone_as == 'containing')
		{
			echo(" CHECKED");
		}
		echo(">Containing&nbsp;");
		echo("</td>");
		echo("<td align='left'>");
		echo("<input type='text' name='search_company_phone' size='20'");
		if ($searchContact->search_company_phone)
		{
			echo(" value='" . addslashes(trim($searchContact->search_company_phone)) . "'");
		}
		echo(">");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>Fax number:&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_fax_as' value='start'");
		if ($searchContact->search_company_fax_as == 'start' || !$searchContact->search_company_fax_as)
		{
			echo(" CHECKED");
		}
		echo(">Starting with&nbsp;");
		echo("<input type='radio' class='row1' name='search_company_fax_as' value='containing'");
		if ($searchContact->search_company_fax_as == 'containing')
		{
			echo(" CHECKED");
		}
		echo(">Containing&nbsp;");
		echo("</td>");
		echo("<td align='left'>");
		echo("<input type='text' name='search_company_fax' size='20'");
		if ($searchContact->search_company_fax)
		{
			echo(" value='" . addslashes(trim($searchContact->search_company_fax)) . "'");
		}
		echo(">");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>&nbsp;</td>");
	echo("</tr>");
    
	echo("<tr>");
		echo("<td colspan='2' align='center'>Company's Industry:&nbsp;</td>");
	echo("</tr>");
    
	echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table align='center' cellpadding='3' cellspacing='3'>");
        echo("<tr align='center'>");
        echo("<td align='center' class='row2'>");
        echo("<table width='100%' class='row2'>");
        echo("<tr>");
            echo("<td colspan='6' align='center'>");
            echo("Click the <b class='black'>Yes</b> box for any industries that you would like companies returned to be in.<br />");
            echo("Click the <b class='black'>No</b> box for any industries that you would NOT like companies returned to be in.<br />");
            echo("Leave both boxes blank if you don't want an industry included in your search.");
            echo("</td>");
        echo("</tr>");
        
        echo("<tr>");
            echo("<td colspan='6'>&nbsp;</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td>Yes</td>");
            echo("<td>No</td>");
            echo("<td>&nbsp;</td>");
            echo("<td>Yes</td>");
            echo("<td>No</td>");
            echo("<td>&nbsp;</td>");
        echo("</tr>");
	
        $newrow = 0;

        $sql = ("
            SELECT DISTINCT industry_id, industry_name
            FROM industries
            ORDER BY industry_name
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow())
        {
            if (!newrow)
            {
                echo("<tr>");
            }

            echo("<td>");
            echo("<input type='checkbox' class='row2' name='search_company_yes_industry[]' value='" . $row["industry_id"] . "' ");
            if (is_array($searchContact->search_company_yes_industry) && 
                in_array($row["industry_id"], $searchContact->search_company_yes_industry))
            {
                echo(" CHECKED");
            }
            echo(">");
            echo("</td>");

            echo("<td>");
            echo("<input type='checkbox' class='row2' name='search_company_no_industry[]' value='" . $row["industry_id"] . "' ");
            if (is_array($searchContact->search_company_no_industry) && 
                in_array($row["industry_id"], $searchContact->search_company_no_industry))
            {
                echo(" CHECKED");
            }
            echo(">");
            echo("</td>");

            echo("<td>");
            echo($row["industry_name"]);
            echo("</td>");

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
	
		echo("</table>");
		echo("</td>");
		echo("</tr>");
    echo("</table>");
    echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>&nbsp;</td>");
	echo("</tr>");

	echo("<tr>");
        echo("<td colspan='2' align='center'>Company's Size:&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table align='center' cellpadding='3' cellspacing='3'>");
        echo("<tr align='center'>");
        echo("<td align='center' class='row2'>");
        echo("<table width='100%' class='row2'>");

        $rowcount = 0; 
                    
        $sql = ("
            SELECT DISTINCT size_id, size_range
            FROM employer_sizes
            ORDER BY size_id
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow())
        {
            if (!($rowcount % 2))
            {
                echo("<tr>");
            }

            echo("<td>");
            echo("<input type='checkbox' class='row2' name='search_company_size[]' value='" . $row["size_id"] . "' ");
            if ($searchContact->search_company_size && in_array($row["size_id"], $searchContact->search_company_size))
            {
                echo("CHECKED");
            }
            echo(">");
            echo($row["size_range"]);
            echo("</td>");

            if ($rowcount % 2)
            {
                echo("</tr>");
            }

            $rowcount++;
        }

        if ($rowcount % 2)
        {
            echo("<td>");
            echo("&nbsp;");
            echo("</td>");
            echo("</tr>");
        }
            
        echo("</table>");
        echo("</td>");
        echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    
    echo("<tr>");
        echo("<td colspan='2' align='center'>Company Type:&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table align='center' cellpadding='3' cellspacing='3'>");
        echo("<tr align='center'>");
        echo("<td align='center' class='row2'>");
        echo("<table width='100%' class='row2'>");

        $rowcount = 0;

        $sql = ("
            SELECT type_id, type_name
            FROM company_type
            ORDER BY type_name
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow())
        {
            if (!($rowcount % 2))
            {
                echo("<tr>");
            }

            echo("<td>");
            echo("<input type='checkbox' class='row2' name='search_company_type[]' value='" . $row["type_id"] . "' ");
            echo(($searchContact->search_company_type && in_array($row["type_id"], $searchContact->search_company_type)) ? "CHECKED" : "");
            echo(">");
            echo($row["type_name"]);
            echo("</td>");
            
            if ($rowcount % 2)
            {
                echo("</tr>");
            }
            $rowcount++;
        }

        echo("</table>");
        echo("</td>");
        echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>&nbsp;</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='center' colspan='2'>Company located in:</td>");
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
                    echo("<select name='search_company_area' onchange='update_company_country()'>");
	                echo("<option value='' ");
	                if (!$searchContact->search_company_area || $searchContact->search_company_area == '')
	                {
	                        echo("selected='selected'");
	                }
	                echo(">&nbsp;");
                    echo("</option>");
	                $sql = ("
                        SELECT area_id, area_name
                        FROM area_list
                        ORDER BY area_id
                        ");
	                $result = $GLOBALS['dbh']->Execute($sql);
	                while ($row = $result->FetchRow())
	                {
                        echo("<option value='" . $row["area_id"] . "' ");
                        if ($searchContact->search_company_area && $searchContact->search_company_area == $row["area_id"])
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
                    echo("<select name='search_company_country' onchange='update_company_prov()'>");
                    
                    // Tricky bit of code here.  If for some reason the page has been refreshed, we need to be careful about
                    // what we put in these location select bars.  If the area has been chosen, then we need to display the
                    // appropriate countries.  If not, then we need to display a blank and a 'Choose an Area'.

                    if ($searchContact->search_company_area == '')
                    {
                        echo("<option value=''>Choose an Area</option>");
                    }
                    else
                    {
                        // An area has been selected.

                        $sql = ("
                            SELECT DISTINCT country_id, country_name
                            FROM country_list
                            WHERE area_id='" . $searchContact->search_company_area . "'
                            ORDER BY order_by
                            ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        echo("<option value='' ");

                        if (!$searchContact->search_company_country || $searchContact->search_company_country == '')
                        {
                            echo("selected='selected'");
                        }
                        echo(">&nbsp;</option>");
                        while ($row = $result->FetchRow())
                        {
                            echo("<option value='" . $row["country_id"] . "' ");
                            if ($searchContact->search_company_country == $row["country_id"])
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
                    echo("<select name='search_company_provstate' onchange='update_company_region()'>");
        
                    // More tricky nonsense here.  The same situation as above, except that now we have to take into account
                    // the situations of an Area being selected but no Country, or no Area AND no Country.  Ugh.
        
                    if ($searchContact->search_company_area == '')
                    {
                        echo("<option value=''>Choose an Area</option>");
                    }
                    elseif ($searchContact->search_company_country == '')
                    {
                        echo("<option value=''>Choose a Country</option>");
                    }
                    else
                    {
                        // A country and area have been selected.

                        $sql = ("
                            SELECT provstate_id, provstate_name
                            FROM provstate_list
                            WHERE country_id='" . $searchContact->search_company_country . "'
                            ORDER BY provstate_id
                            ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        if ($result->RecordCount())
                        {
                            // If the chosen country has provinces/states associated with it, displays those and a blank
                            // as choices.  Otherwise display N/A.

                            echo("<option value='' ");
                            if (!$searchContact->search_company_provstate || $searchContact->search_company_provstate == '')
                            {
                                echo("selected='selected'");
                            }
                            echo(">&nbsp;</option>");
                            while ($row = $result->FetchRow())
                            {
                                echo("<option value='" . $row["provstate_id"] . "' ");
                                if ($searchContact->search_company_provstate == $row["provstate_id"])
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
                    echo("<select name='search_company_region'>");
	
	                // Here's the last task.  This is the trickiest.  We have to check if we're dealing with a blank Area, Country
	                // or Province/State, and display the appropriate choices (this is only really critical when someone updates the
	                // page somehow).
	
	                if ($searchContact->search_company_area == '')
	                {
                        echo("<option value=''>Choose an Area</option>");
                    }
                    elseif ($searchContact->search_company_country == '')
                    {
                        echo("<option value=''>Choose a Country</option>");
                    }
	                elseif (!isset($searchContact->search_company_provstate))
                    {
                        echo("<option value='' selected='selected'>&nbsp;</option>");
                    }
	                else
	                {
                        $sql = ("
                            SELECT region_id, region_name
                            FROM region_list
                            WHERE provstate_id='" . $searchContact->search_company_provstate . "'
                            ORDER BY region_id
                            ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        if ($result->RecordCount())
                        {
                            echo("<option value='' ");
                            if (!$searchContact->search_company_region || $searchContact->search_company_region == '')
                            {
                                echo("selected='selected'");
                            }
                            echo(">&nbsp;</option>");
                            while ($row = $result->FetchRow())
                            {
                                echo("<option value='" . $row["region_id"] . "' ");
                                if ($searchContact->search_company_region == $row["region_id"])
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
                echo("<td align='left'><input type='text' size='20' name='search_company_city'");
                if ($searchContact->search_company_city)
                {
                    echo("VALUE='" . $searchContact->search_company_city . "'");
                }
                echo(">");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Postal/Zip Code starting with:</td>");
                echo("<td align='left'><input type='text' size='10' name='search_company_postal_code' ");
                if ($searchContact->search_company_postal_code)
                {
                    echo("VALUE ='" . $searchContact->search_company_postal_code . "'");
                }
                echo(">");
                echo("</td>");
            echo("</tr>");
            echo("</table>");
	        echo("</td>");
		echo("</tr>");
        echo("</table>");
        echo("</td>");
	echo("</tr>");

	echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

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
		echo("<tr>");
			echo("<td colspan='2' align='center'>Company Flags:</td>");
		echo("</tr>");
        
		echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table align='center' cellpadding='3' cellspacing='3'>");
			echo("<tr align='center'>");
			echo("<td align='center' class='row2'>");
			echo("<table width='100%' class='row2'>");
                echo("<tr>");
                    echo("<td colspan='2' align='center'>Companies with&nbsp;");
                    echo("<input type='radio' class='row2' name='search_company_flags_boolean' value='and' ");
                    if ($searchContact->search_company_flags_boolean == 'and' || !$searchContact->search_company_flags_boolean)
                    {
                        echo("CHECKED");
                    }
                    echo(">All&nbsp;");
                    echo("<input type='radio' class='row2' name='search_company_flags_boolean' value='or' ");
                    if ($searchContact->search_company_flags_boolean == 'or')
                    {
                        echo("CHECKED");
                    }
                    echo(">Any&nbsp;");
                    echo("of the flags below checked&nbsp;");
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2'>&nbsp;</td>");
                echo("</tr>");

                $newrow = 0;

                while ($row = $result->FetchRow())
                {
                    if (!$newrow)
                    {
                        echo("<tr>");
                    }

                    echo("<td>");
                    echo("<input type='checkbox' class='row2' name='search_company_flags[]' value='" . $row["flag_id"] . "' ");
                    if (is_array($searchContact->search_company_flags) && in_array($row["flag_id"], $searchContact->search_company_flags))
                    {
                        echo("CHECKED");
                    }
                    echo(">");
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
	}//end if
}//end if
if ($search_department_criteria == 'department')
{
    echo("<tr>");
        echo("<td align='right'>Division name starts with:</td>");
        echo("<td align='left'>");
        echo("<input type='text' name='search_department_name' size='30' ");
        if ($searchContact->search_department_name)
        {
                echo(" value='" . addslashes(trim($searchContact->search_department_name)) . "'");
        }
        echo(">");
        echo("</td>");
    echo("</tr>");

    echo("<tr>");
        echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
        echo("<td align='right'>Division website:&nbsp;");
        echo("<input type='radio' class='row1' name='search_department_website_as' value='start' ");
        if ($searchContact->search_department_website_as == 'start' || !$searchContact->search_department_website_as)
        {
                echo("CHECKED");
        }
        echo(">Starting with&nbsp;");
        echo("<input type='radio' class='row1' name='search_department_website_as' value='containing' ");
        if ($searchContact->search_department_website_as == 'containing')
        {
                echo("CHECKED");
        }
        echo(">Containing&nbsp;");
        echo("</td>");
        echo("<td align='left'>");
        echo("<input type='text' name='search_department_website' size='30' ");
        if ($searchContact->search_department_website)
        {
                echo(" value='" . $searchContact->search_department_website . "'");
        }
        echo(">");
        echo("</td>");
    echo("</tr>");

    echo("<tr>");
        echo("<td align='right'>E-mail address:&nbsp;");
        echo("<input type='radio' class='row1' name='search_department_email_as' value='start' ");
        if ($searchContact->search_department_email_as == 'start' || !$searchContact->search_department_email_as)
        {
            echo("CHECKED");
        }
        echo(">Starting with&nbsp;");
        echo("<input type='radio' class='row1' name='search_department_email_as' value='containing' ");
        if ($searchContact->search_department_email_as == 'containing')
        {
            echo("CHECKED");
        }
        echo(">Containing&nbsp;");
        echo("</td>");

		echo("<td align='left'>");
		echo("<input type='text' name='search_department_email' size='30' ");
		if ($searchContact->search_department_email)
		{
			echo(" value='" . $searchContact->search_department_email . "'");
		}
		echo(">");
		echo("</td>");
	echo("</tr>");

    echo("<tr>");
        echo("<td align='right'>Phone number:&nbsp;");
        echo("<input type='radio' class='row1' name='search_department_phone_as' value='start'");
        if ($searchContact->search_department_phone_as == 'start' || !$searchContact->search_department_phone_as)
        {       
            echo("CHECKED");
        }
        echo(">Starting with&nbsp;  ");
        echo("<input type='radio' class='row1' name='search_department_phone_as' value='containing' ");
        if ($searchContact->search_department_phone_as == 'containing')
        {                       
            echo("CHECKED");
        }
        echo(">Containing&nbsp;");
        echo("</td>");
        echo("<td align='left'>");
        echo("<input type='text' name='search_department_phone' size='20' ");
        if ($searchContact->search_department_phone)
        {
                echo(" value='" . addslashes(trim($searchContact->search_department_phone)) . "'");
        }
        echo(">");
        echo("</td>");
    echo("</tr>");
    
    echo("<tr>");
        echo("<td align='right'>Fax number:&nbsp;");
        echo("<input type='radio' class='row1' name='search_department_fax_as' value='start'");
        if ($searchContact->search_department_fax_as == 'start' || !$searchContact->search_department_fax_as)
        {       
            echo("CHECKED");
        }
        echo(">Starting with&nbsp;  ");
        echo("<input type='radio' class='row1' name='search_department_fax_as' value='containing' ");
        if ($searchContact->search_department_fax_as == 'containing')
        {                       
            echo("CHECKED");
        }
        echo(">Containing&nbsp;");
        echo("</td>");
        echo("<td align='left'>");
        echo("<input type='text' name='search_department_fax' size='20' ");
        if ($searchContact->search_department_fax)
        {
                echo(" value='" . addslashes(trim($searchContact->search_department_fax)) . "'");
        }
        echo(">");
        echo("</td>");
    echo("</tr>");
    
    echo("<tr>");
        echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");
    
    echo("<tr>");
        echo("<td colspan='2' align='center'>Division's Industry:&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table align='center' cellpadding='3' cellspacing='3'>");
        echo("<tr align='center'>");
        echo("<td align='center' class='row2'>");
        echo("<table width='100%' class='row2'>");
            echo("<tr>");
                echo("<td colspan='6' align='center'>");
                echo("Click the <b class='black'>Yes</b> box for any industries that you would like divisions returned to be in.<br />");
                echo("Click the <b class='black'>No</b> box for any industries that you would NOT like divisions returned to be in.<br />");
                echo("Leave both boxes blank if you don't want an industry included in your search.");
                echo("</td>");
	        echo("</tr>");

	        echo("<tr>");
                echo("<td colspan='6'>&nbsp;</td>");
	        echo("</tr>");

	        echo("<tr>");
                echo("<td>Yes</td>");
                echo("<td>No</td>");
                echo("<td>&nbsp;</td>");
                echo("<td>Yes</td>");
                echo("<td>No</td>");
                echo("<td>&nbsp;</td>");
	        echo("</tr>");
	                                
	        $newrow = 0;            
	                                
	        $sql = ("       
                SELECT DISTINCT industry_id, industry_name
                FROM industries
                ORDER BY industry_name
                ");
	        $result = $GLOBALS['dbh']->Execute($sql);
	        while ($row = $result->FetchRow())
	        {
                if (!newrow)
                {
                    echo("<tr>");
                }
	
                echo("<td>");
                echo("<input type='checkbox' class='row2' name='search_department_yes_industry[]' value='" . $row["industry_id"] . "' ");
                if (is_array($searchContact->search_department_yes_industry) && 
                    in_array($row["industry_id"], $searchContact->search_department_yes_industry))
                {
                    echo("CHECKED");
                }
                echo(">");
                echo("</td>");

                echo("<td>");
                echo("<input type='checkbox' class='row2' name='search_department_no_industry[]' value='" . $row["industry_id"] . "' ");
                if (is_array($searchContact->search_department_no_industry) && 
                    in_array($row["industry_id"], $searchContact->search_department_no_industry))
                {
                    echo("CHECKED");
                }
                echo(">");
                echo("</td>");
                echo("<td>");
                echo($row["industry_name"]);
                echo("</td>");
                //echo("</td>");

                if ($newrow)
                {
                    echo("</tr>");
                }

                $newrow = !($newrow);
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

    echo("<tr>");
        echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
        echo("<td colspan='2' align='center'>Division's Size:&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table align='center' cellpadding='3' cellspacing='3'>");
        echo("<tr align='center'>");
        echo("<td align='center' class='row2'>");
        echo("<table width='100%' class='row2'>");

            $rowcount = 0;

            $sql = ("
                SELECT DISTINCT size_id, size_range
                FROM employer_sizes
                ORDER BY size_id
                ");
            $result = $GLOBALS['dbh']->Execute($sql);

            while ($row = $result->FetchRow())
            {
                if (!($rowcount % 2))
                {
                    echo("<tr>");
                }

                echo("<td>");
                echo("<input type='checkbox' class='row2' name='search_department_size[]' value='" . $row["size_id"] . "' ");
                if ($searchContact->search_department_size && in_array($row["size_id"], $searchContact->search_department_size))
                {
                    echo("CHECKED");
                }
                echo(">");
                echo($row["size_range"]);
                echo("</td>");

                if ($rowcount % 2)
                {
                    echo("</tr>");
                }

                $rowcount++;
            }

            if ($rowcount % 2)
            {
                echo("<td>");
                echo("&nbsp;");
                echo("</td>");
                echo("</tr>");
            }

        echo("</table>");
        echo("</td>");
        echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
        echo("<td colspan='2' align='center'>&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
        echo("<td align='center' colspan='2'>Division located in:</td>");
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
                echo("<select name='search_department_area' onchange='update_department_country()'>");
                echo("<option value='' ");
                if (!$searchContact->search_department_area || $searchContact->search_department_area == '')
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
                    if ($searchContact->search_department_area && $searchContact->search_department_area == $row["area_id"])
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
                echo("<select name='search_department_country' onchange='update_department_prov()'>");

                // Tricky bit of code here.  If for some reason the page has been refreshed, we need to be careful about
                // what we put in these location select bars.  If the area has been chosen, then we need to display the
                // appropriate countries.  If not, then we need to display a blank and a 'Choose an Area'.

                if ($searchContact->search_department_area == '')
                {
                    echo("<option value=''>Choose an Area</option>");
                }
                else
                {
                    // An area has been selected.

                    $sql = ("
                        SELECT DISTINCT country_id, country_name
                        FROM country_list
                        WHERE area_id='" . $searchContact->search_department_area . "'
                        ORDER BY order_by
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    echo("<option value='' ");
                    if (!$searchContact->search_department_country || $searchContact->search_department_country == '')
                    {
                        echo("selected='selected'");
                    }
                    echo(">&nbsp;</option>");
                    while ($row = $result->FetchRow())
                    {
                        echo("<option value='" . $row["country_id"] . "' ");
                        if ($searchContact->search_department_country == $row["country_id"])
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
                echo("<select name='search_department_provstate' onchange='update_department_region()'>");

                // More tricky nonsense here.  The same situation as above, except that now we have to take into account
                // the situations of an Area being selected but no Country, or no Area AND no Country.  Ugh.

                if ($searchContact->search_department_area == '')
                {
                    echo("<option value=''>Choose an Area</option>");
                }
                elseif ($searchContact->search_department_country == '')
                {
                    echo("<option value=''>Choose a Country</option>");
                }
                else
                {
                    // A country and area have been selected.

                    $sql = ("
                        SELECT DISTINCT provstate_id, provstate_name
                        FROM provstate_list
                        WHERE country_id='" . $searchContact->search_department_country . "'
                        ORDER BY provstate_id
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    if ($result->RecordCount())
                    {
                        // If the chosen country has provinces/states associated with it, displays those and a blank
                        // as choices.  Otherwise display N/A.

                        echo("<option value='' ");
                        if (!$searchContact->search_department_provstate || $searchContact->search_department_provstate == '')
                        {
                            echo("selected='selected'");
                        }
                        echo(">&nbsp;</option>");
                        while ($row = $result->FetchRow())
                        {
                            echo("<option value='" . $row["provstate_id"] . "' ");
                            if ($searchContact->search_department_provstate == $row["provstate_id"])
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
                echo("<select name='search_department_region'>");

                // Here's the last task.  This is the trickiest.  We have to check if we're dealing with a blank Area, Country
                // or Province/State, and display the appropriate choices (this is only really critical when someone updates the
                // page somehow).

                if ($searchContact->search_department_area == '')
                {
                    echo("<option value=''>Choose an Area</option>");
                }
                elseif ($searchContact->search_department_country == '')
                {
                    echo("<option value=''>Choose a Country</option>");
                }
                elseif (!isset($searchContact->search_department_provstate))
                {
                    echo("<option value='' selected='selected'>&nbsp;</option>");
                }
                else
                {
                    $sql = ("
                        SELECT DISTINCT region_id, region_name
                        FROM region_list
                        WHERE provstate_id='" . $searchContact->search_department_provstate . "'
                        ORDER BY region_id
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    if ($result->RecordCount())
                    {
                        echo("<option value='' ");
                        if (!$searchContact->search_department_region || $searchContact->search_department_region == '')
                        {
                            echo("selected='selected'");
                        }
                        echo(">&nbsp;</option>");
                        while ($row = $result->FetchRow())
                        {
                            echo("<option value='" . $row["region_id"] . "' ");
                            if ($searchContact->search_department_region == $row["region_id"])
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
                echo("<td align='left'><input type='text' size='20' name='search_department_city' ");
                if ($searchContact->search_department_city)
                {
                    echo("VALUE='" . $searchContact->search_department_city . "'");
                }
                echo(">");

                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Postal/Zip Code starting with:</td>");
                echo("<td align='left'><input type='text' size='10' name='search_department_postal_code' ");
                if ($searchContact->search_department_postal_code)
                {
                    echo("VALUE ='" . $searchContact->search_department_postal_code . "'");
                }
                echo(">");
                echo("</td>");
            echo("</tr>");
        echo("</table>");
        echo("</td>");
        echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    
    echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");

    $sql = ("
        SELECT DISTINCT a.flag_id, a.flag_name
        FROM division_flags AS a, department_division_flags AS b
        WHERE a.flag_id=b.flag_id
        AND b.department_id='" . $auth->department . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {
        echo("<tr>");
            echo("<td colspan='2' align='center'>Division Flags:</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table align='center' cellpadding='3' cellspacing='3'>");
			echo("<tr align='center'>");
			echo("<td align='center' class='row2'>");
			echo("<table width='100%' class='row2'>");
                echo("<tr>");
                    echo("<td colspan='2' align='center'>Return divisions with&nbsp;");
                    echo("<input type='radio' class='row2' name='search_department_flags_boolean' value='and' ");
                    if ($searchContact->search_department_flags_boolean == 'and' || !$searchContact->search_department_flags_boolean)
                    {
                        echo("CHECKED");
                    }
                    echo(">All&nbsp;");

                    echo("<input type='radio' class='row2' name='search_department_flags_boolean' value='or' ");
                    if ($searchContact->search_department_flags_boolean == 'or')
                    {
                        echo("CHECKED");
                    }
                    echo(">Any&nbsp;");
                    echo("of the flags below checked&nbsp;");
                    echo("</td>");
                echo("</tr>");

                echo("<tr>");
                    echo("<td colspan='2'>&nbsp;</td>");
                echo("</tr>");

                $newrow = 0;

                while ($row = $result->FetchRow())
                {
                    if (!$newrow)
                    {
                        echo("<tr>");
                    }

                    echo("<td>");
                    echo("<input type='checkbox' class='row2' name='search_department_flags[]' value='" . $row["flag_id"] . "' ");
                    if (is_array($searchContact->search_department_flags) && in_array($row["flag_id"], $searchContact->search_department_flags))
                    {
                        echo("CHECKED");
                    }
                    echo(">");
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
}

echo("<tr>");
echo("<td colspan='2' align='center'>");
echo("<input type='hidden' name='continue' value='second_contact_form_done'>");
echo("<input type='submit' value='Continue'>");
echo("&nbsp;&nbsp;&nbsp;");
// Because HTML does not allow nested forms, this reset button calls reset_form down below. 
echo("<input type='button' onclick='javascript:document.reset_form.submit()' value='Reset'>");
/* TODO: Temporarily disabled saved searches for company/division criteria. 
echo("&nbsp;&nbsp;&nbsp;");
if($edit_page == 'yes')
{
    //These variables are only set if you are editing a saved search
    echo("<input type='hidden' name='search_id' value='$search_id'>");
    echo("<input type='hidden' name='edit_page' value='yes'>");
}
echo("<input type='submit' name='btnSearch' value='Save Search'>");
*/
echo("</td>");
echo("</tr>");
echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("</table>");
echo("</form> ");

// This sends the user back to the main contact advanced search page. 
echo("<form name='reset_form' method='post' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='continue' value='Go to Advanced Search'>");
echo("<input type='hidden' name='level1' value='contact'>");
if (strlen($search_id))
{
    echo("<input type='hidden' name='search_id' value='".$search_id."'>");
    echo("<input type='hidden' name='edit_page' value='yes'>");
    echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria'>");
}
echo("</form>");
