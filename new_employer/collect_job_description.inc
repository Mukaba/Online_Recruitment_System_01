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
 | Filename: collect_job_description.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: This is the second form called by add_job_form.inc.  After      |
 | being filled out, the inputs are returned to add_job_form.inc for            |
 | validation.                                                                  |
 +------------------------------------------------------------------------------+

*/



$max_country_id=0;
$country_array = array();

$result = $GLOBALS['dbh']->Execute("
	SELECT c.country_id, provstate_id, provstate_name 
	FROM country_list as c LEFT JOIN provstate_list as p ON c.country_id = p.country_id 
	ORDER BY c.order_by
	");

while ($row = $result->FetchRow())
{
	if ($row["provstate_id"] == "")
	{
		$str = "Array(\"NULL\"),";
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
for($i=0;$i<$max_country_id;$i++)
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
$result = $GLOBALS['dbh']->Execute("
	SELECT p.provstate_id, r.region_id, r.region_name 
	FROM provstate_list as p LEFT JOIN region_list as r ON p.provstate_id = r.provstate_id 
	ORDER BY p.provstate_id
	");

while ($row = $result->FetchRow())
{
	if ($row["region_id"] == "")
	{
		$str = "Array(\"NULL\"),";
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
for($i=0;$i<$max_prov_id;$i++)
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

	function update_prov(){
		var index = document.form1.country[document.form1.country.selectedIndex].value;
		prov = prov_array[index];
		if (prov.length==1 && prov[0] == "NULL"){
			document.form1.provstate.options.length = 1;
			document.form1.provstate.options[0] = new Option("N/A", "");
			document.form1.provstate.selectedIndex = 0;
			document.form1.region.options.length=1;
			document.form1.region.options[0] = new Option("N/A", "");
			document.form1.region.selectedIndex = 0;
			document.form1.city.value = "";

		}
		else
		{
			
			document.form1.provstate.options.length = 0;
			document.form1.provstate.options.length = prov.length;
			for($i=0;$i<prov.length;$i++)
			{
				document.form1.provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
			}
			document.form1.provstate.selectedIndex = 0;

			index2=document.form1.provstate[document.form1.provstate.selectedIndex].value;
			region = region_array[index2];
			if (region.length==1 && region[0] == "NULL")
			{
				document.form1.region.options.length=1;
				document.form1.region.options[0] = new Option("N/A", "");
				document.form1.city.value = "";
			}
			else
			{
				document.form1.region.options.length=0;
				document.form1.region.options.length=region.length;
				for($i=0;$i<region.length;$i++)
				{
					document.form1.region.options[$i] = new Option(region[$i][1], region[$i][0]);
				}
				document.form1.region.selectedIndex = 0;
	
				index3 = document.form1.region[document.form1.region.selectedIndex].value;
				city = city_array[index3];
				document.form1.city.value = city;
			}
			
		}
	}

	function update_region()
	{
		
		document.form1.region.options.length=0;
		index2=document.form1.provstate[document.form1.provstate.selectedIndex].value;
		region = region_array[index2];
		if (region.length==1 && region[0] == "NULL"){
			document.form1.region.options.length=1;
			document.form1.region.options[0] = new Option("N/A", "");
			document.form1.city.value = '';
		}
		else
		{
			document.form1.region.options.length = region.length;
			for($i=0;$i<region.length;$i++)
			{
				document.form1.region.options[$i] = new Option(region[$i][1], region[$i][0]);
			}
		
			document.form1.region.selectedIndex = 0;

			index3 = document.form1.region[document.form1.region.selectedIndex].value;
			city = city_array[index3];
			document.form1.city.value = city;
		}
	}

	function update_city()
	{
		index3 = document.form1.region[document.form1.region.selectedIndex].value;
		city = city_array[index3];
			
		document.form1.city.value = city;
	}

//-->

</script>

 
<h3>Job Description Form</h3>
<?php

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='new_employer' />");
echo("<input type='hidden' name='newEmployer' value='" . packObject($newEmployer) . "' />");
   
echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right' nowrap='nowrap'>Employer's internal job code (if applicable):</td>");
	echo("<td><input type='text' name='employer_code' size='15' maxlength='30' value='" . $newEmployer->job_employer_code . "' />");
echo("</tr>");

echo("<tr>");
	echo("<td align='right' nowrap='nowrap'>Title of position: <b class='red'>*</b></td>");
	echo("<td><input type='text' name='position_title' size='40' maxlength='50' value='" . $newEmployer->job_position_title . "' />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right' nowrap='nowrap'>Number of positions available:</td>");
	echo("<td><select name='num_positions'>");

	// Change $i = startnumber and $i <= endnumber below to change
	// the range of number of positions the employer will have to
	// choose from.

	for($i = 1; $i <= 10; $i++)
	{
		echo("<option value='$i' ");
		if (($newEmployer->job_num_positions == $i) || (!($newEmployer->job_num_positions) && ($i == 1)))
		{
			echo("selected='selected'");
		}
		echo(">" . $i . "</option>");
	}
	
	echo("</select>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>How would you like to receive applications?</td>");
	echo("<td>");
	echo("<input class='row1' type='radio' name='apply_method' value='".JOB_APP_METHOD_ONLINE."'");

	if (!($newEmployer->job_apply_method) || ($newEmployer->job_apply_method == JOB_APP_METHOD_ONLINE))
	{ 
		echo("checked='checked'"); 
	} 

	echo(" />Online&nbsp;");
	echo("<input class='row1' type='radio' name='apply_method' value='".JOB_APP_METHOD_PAPER."'"); 

	if ($newEmployer->job_apply_method == JOB_APP_METHOD_PAPER)
	{ 
		echo("checked='checked'"); 
	} 
	echo(" />Paper&nbsp;");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>E-mail address for online applications: <b class='red'>*</b><br />(leave blank if unapplicable)</td>");
	echo("<td><input type='text' name='receive_address' size='40' value='");
	echo((trim($newEmployer->job_receive_address)) ? $newEmployer->job_receive_address : $newEmployer->contact_email);
	echo("' /></td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>Location of job offered:</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'>");
	echo("<table cellpadding='3' cellspacing='3' class='row2'>");

	echo("<tr>");
	echo("<td class='row2'>");
	echo("<table width='100%' class='row2'>");
	
	echo("<tr>");
	echo("<td align='right'>Country:</td>");
	echo("<td>");

	echo("<select name='country' onchange='update_prov()'>");
	
	/*
	 The below if and elseif statements are used to find out if we have a country for the
	 employer that this job is being added for.  If we do, then we want to use this country
	 as the default setting.
	*/

	$sql = ("
		SELECT country_id, country_name
		FROM country_list
		ORDER BY order_by
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["country_id"] . "'");
		if (!($newEmployer->job_country_id))
		{
			if ($newEmployer->company_country_id) 
			{
				if ($row["country_id"] == $newEmployer->company_country_id)
				{
					echo(" selected='selected'");
				}
			}	
			elseif ($row["country_id"] == 1)
			{
				echo(" selected='selected'");
			}
		}
		elseif ($newEmployer->job_country_id == $row["country_id"])
		{
			echo(" selected='selected'");
		}
		echo(">" . $row["country_name"] . "</option>");
	}

	echo("</select>");
	echo("</td>");
	echo("</tr>");

	echo("<tr>");
	echo("<td align='right'>Province/State:<br />(If in Canada or US)</td>");
	echo("<td>");
	echo("<select name='provstate' onchange='update_region()'>");

	/*
	 We were running into problems here when the page would be reloaded by add_job_form.inc after receiving 
	 incorrect input.  We would end up with the original country selected, but instead of having N/A for
	 the provstate and region, we had the original Canadian and British Columbian choices, respectively.
	 To fix this problem, the code below has been put in here, and in the region section.
	*/

	$get_country = ($newEmployer->job_country_id ? $newEmployer->job_country_id : ($newEmployer->company_country_id ? $newEmployer->company_country_id : 1));

	/*
	 If the country we have is either Canada, US, or hasn't been chosen yet, then either load the provinces
	 for the US, or Canada (if it's Canada or unchosen).  Otherwise, just display a N/A option.
	*/

	if ($get_country == 1 || $get_country == 2)
	{
		$sql = ("
			SELECT provstate_id, provstate_name
			FROM provstate_list
			WHERE country_id = ?
			ORDER BY provstate_id
			");
        $stmt = $GLOBALS['dbh']->Prepare($sql);
		$result = $GLOBALS['dbh']->Execute($stmt, array($get_country));

		while ($row = $result->FetchRow())
		{
			echo("<option value='" . $row["provstate_id"] . "'");
			if (!($newEmployer->job_provstate_id))
			{
				if ($newEmployer->company_provstate_id)
				{
					if ($row["provstate_id"] == $newEmployer->company_provstate_id)
					{
						echo(" selected='selected'");
					}
				}
				elseif ($row["provstate_id"] == 1) 
				{
					echo(" selected='selected'");
				}
			}
			elseif ($newEmployer->job_provstate_id == $row["provstate_id"])
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

	echo("</select>");
	echo("</td>");
	echo("</tr>");

	echo("<tr>");
	echo("<td align='right'>Region:<br />(If in Canada)</td>");
	echo("<td>");

	echo("<select name='region' onchange='update_city()'>");

	$get_prov = ($newEmployer->job_provstate_id ? $newEmployer->job_provstate_id : ($newEmployer->company_provstate_id ? $newEmployer->company_provstate_id : $newEmployer->job_provstate_id));

	/*
	 If the province we have chosen is one with regions, OR it is blank AND the chosen country is Canada or unchosen
	 then load either the respective regions (if we have a province) or the regions for BC (if province is unchosen).
	*/

	if ($get_country == 1 && $get_prov == '')
	{
		$get_prov = 1;
	}
	if ($get_prov == 1 || $get_prov == 2 || $get_prov == 3 || $get_prov == 9 || $get_prov == 11 || $get_prov == 12) 
	{
		$sql = ("
			SELECT region_id, region_name
			FROM region_list
			WHERE provstate_id = ?
			ORDER BY region_id
			");
        $stmt = $GLOBALS['dbh']->Prepare($sql);
		$result = $GLOBALS['dbh']->Execute($stmt, array($get_prov));
		while ($row = $result->FetchRow())
		{
			echo("<option value='" . $row["region_id"] . "'");
			if (!($newEmployer->job_region_id))
			{
				if ($newEmployer->company_region_id)
				{
					if ($row["region_id"] == $newEmployer->company_region_id)
					{
						echo(" selected='selected'");
					}
				}
				elseif ($row["region_id"] == 1)
				{
					echo(" selected='selected'");
				}
			}
			elseif ($newEmployer->job_region_id == $row["region_id"])
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

	echo("</select>");
	echo("</td>");
	echo("</tr>");

	echo("<tr>");
	echo("<td align='right'>City: <b class='red'>*</b></td>");
	echo("<td>");

	echo("<input type='text' size='30' maxlength='50' name='city' value='");
	echo((isset($newEmployer->job_city) ? $newEmployer->job_city : $newEmployer->company_city)); 
	echo("' />");
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

echo("<tr>");
	echo("<td align='right'>Approximate Salary Range:<br />(leave blank if this will <br /> be discussed later)</td>");
	echo("<td nowrap='nowrap'>$&nbsp;");
	echo("<input type='text' name='salary_amount1' size='8' maxlength='8' value='");
	echo(stripSlashes($newEmployer->job_salary_amount1));
	echo("' /> &nbsp;-&nbsp;$");
	echo("<input type='text' name='salary_amount2' size='8' maxlength='8' value='");
	echo(stripSlashes($newEmployer->job_salary_amount2));
	echo("' /> &nbsp;per&nbsp;");

	echo("<select name='salary_period'>");

	echo("<option value='hour' ");
	echo(($newEmployer->job_salary_period == hour) ? "selected='selected'" : "");
	echo(">&nbsp;Hour&nbsp;</option>");
	echo("<option value='week' ");
	echo(($newEmployer->job_salary_period == week) ? "selected='selected'" : "");
	echo(">&nbsp;Week&nbsp;</option>");
	echo("<option value='2 weeks' ");
	echo(($newEmployer->job_salary_period == '2 weeks') ? "selected='selected'" : "");
	echo(">&nbsp;2 Weeks&nbsp;</option>");
	echo("<option value='month' ");
	echo(($newEmployer->job_salary_period == month) ? "selected='selected'" : "");
	echo(">&nbsp;Month&nbsp;</option>");
	echo("<option value='year' ");
	echo(($newEmployer->job_salary_period == year) ? "selected='selected'" : "");
	echo(">&nbsp;Year&nbsp;</option>");

	echo("</select>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Hours of work per week:</td>");
	echo("<td>");
	echo("<input type='text' name='workterm_hours' size='10' maxlength='10' value='" . $newEmployer->job_workterm_hours . "' />");
	echo("</td>");
echo("</tr>");

// beginning a block of nested tables
echo("<tr align='center'>");
echo("<td colspan='2'>");
echo("<table cellpadding='3' cellspacing='3'>");
echo("<tr>");
echo("<td class='row2'>");

echo("<table width='100%' class='row2'>");
echo("<tr>");
	echo("<td align='right'>Start of workterm:</td>");
	echo("<td>");

	echo("<select name='start_term'>");

	/*
	 We need to figure out what term we are currently in.
	*/

	$nextTermYear = getNextTermYear();
	$nextterm = $nextTermYear["term"];
	
	$sql = ("
		SELECT term_id, term_name, start_date, end_date
		FROM term
		ORDER BY start_date
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["term_id"] . "'");
		if (($newEmployer && $newEmployer->job_start_term == $row["term_id"]) || (!($newEmployer->job_start_term) && ($row["term_id"] == $nextterm)))
		{
			echo(" selected='selected'");
		}
		echo(">" .getMonth($row["start_date"]). "</option>");
	}
	
	echo("</select>");
	echo("&nbsp;");
	echo("<select name='start_year'>");

	$date = explode("-", date("Y-m-d"));
	$limit = $date[0] + 1;
	
	for ($i = $limit; $i >= $date[0]; $i--)
	{
		echo("<option value='$i'");
		if ((($newEmployer) && ($newEmployer->job_start_year == $i)) || (!($newEmployer->job_start_year) && ($i == $nextTermYear["year"])))
		{ 
			echo("selected='selected'"); 
		} 
		echo(">&nbsp;" . $i . "&nbsp;</option>");
	}
	
	echo("</select>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Workterm Length(s): <b class='red'>*</b></td>");
	echo("<td>");

	$currentTerm = getCurrentTermID();
	$sql = ("
		SELECT DISTINCT start_date, end_date
		FROM term
		WHERE term_id='" . $currentTerm . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$termLength = $row["end_date"] - $row["start_date"] + 1;
	
	$limit = $termLength * 5;
	$multiple = $termLength;
	for ($i = $termLength; $i < ($limit - $multiple); $i += $multiple)
	{
		echo("<input type='checkbox' class='row1' name='workterm_lengths[]' value='" . $i . "'");
		if (($newEmployer && is_array($newEmployer->job_workterm_lengths) && in_array($i, $newEmployer->job_workterm_lengths)) 
		|| (!$newEmployer->job_workterm_lengths && $i == $multiple))
		{
			echo(" checked='checked'");
		}
		echo(" />" . $i . " or ");
	}
	echo("<input type='checkbox' class='row1' name='workterm_lengths[]' value='" . $i . "'");
	if (($newEmployer && is_array($newEmployer->job_workterm_lengths) && in_array($i, $newEmployer->job_workterm_lengths))
	|| (!$newEmployer->job_workterm_lengths && $i == $multiple))
	{
		echo(" checked='checked'");
	}
	echo(" />" . $i . " months");

	echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("-or-");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");
echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("Normal start months are January, May, and September.<br /> A limited number of students may be available starting at other times.");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='right'>");
    echo("Start Date:");
    echo("</td>");
    echo("<td>");
    echo("<input type='text' size='2' maxlength='2' name='start_date_date' value='".$newEmployer->job_start_date_date."' />&nbsp;");
    echo("<select name='start_date_month'>");
    echo("<option value=''>&nbsp;</option>");

    for($i = 1; $i <= 12; $i++)
    {
        echo("<option value='".$i."' ");
        if ($i == $newEmployer->job_start_date_month)
        {
            echo("selected='selected'");
        }
        echo(">");
        echo(getMonth($i));
        echo("</option>");
    }

    echo("</select>");
    echo("<select name='start_date_year'>");
    echo("<option value=''>&nbsp;</option>");
    
    for($i = date("Y") + 1; $i >= date("Y"); $i--)
    {
        echo("<option value='".$i."' ");
        if ($i== $newEmployer->job_start_date_year)
        {
            echo("selected='selected'");
        }
        echo(">");
        echo($i);
        echo("</option>");
    }

    echo("</select>");
    echo("(DD-Month-YYYY)");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='right'>");
        echo("End Date:");
    echo("</td>");
    echo("<td>");
    echo("<input type='text' size='2' maxlength='2' name='end_date_date' value='".$newEmployer->job_end_date_date."' />&nbsp;");
    echo("<select name='end_date_month'>");
    echo("<option value=''>&nbsp;</option>");
    for($i = 1; $i <= 12; $i++)
    {
        echo("<option value='".$i."' ");
        if ($i == $newEmployer->job_end_date_month)
        {
            echo("selected='selected'");
        }
        echo(">");
        echo(getMonth($i));
        echo("</option>");
    }
    echo("</select>");
    echo("<select name='end_date_year'>");
    echo("<option value=''>&nbsp;</option>");

    for($i = date("Y") + 1; $i >= date("Y"); $i--)
    {
        echo("<option value='".$i."' ");
        if ($i== $newEmployer->job_end_date_year)
        {
            echo("selected='selected'");
        }
        echo(">");
        echo($i);
        echo("</option>");
    }

    echo("</select>");
    echo("(DD-Month-YYYY)");
    echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");
// end of nested table block

echo("<tr>");
	echo("<td align='right'>This job is also posted at:</td>");
	echo("<td><input type='text' name='other_postings' size='50' maxlength='100' value='" . $newEmployer->job_other_postings . "' />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'>Profile of applicant:");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'>");
	echo("<table cellpadding='3' cellspacing='3'>");
	echo("<tr>");
	echo("<td class='row2'>");
	echo("<table width='100%' class='row2'>");
	echo("<tr>");
		//echo("<td align='right'>Minimum academic year completed:</td>");
		//echo("<td align='left'>");
		echo("<td align='center'>Minimum academic year completed:");
		echo("<select name='min_academic'>");
		
        // :TODO: Minimum academic year should not be hardcoded like this here
		for ($i = 1; $i < 5; $i++)
		{
			echo("<option value='" . $i . "' ");
			if ((!$newEmployer && $i == '1') || $newEmployer->job_min_academic == $i)
			{
				echo("selected='selected'");
			}
			echo(">" . $i . "</option>");
		}
		
		echo("</select>");
		echo("</td>");
	echo("</tr>");
	echo("<tr>");
		echo("<td align='center'>Minimum workterms completed:");
		//echo("<td align='right'>Minimum workterms completed:</td>");
		//echo("<td align='left'>");
		echo("<select name='min_workterms'>");
		
        // :TODO: The min number of workterms completed shouldn't be hardcoded either. 
		for ($i = 0; $i < 6; $i++)
		{
			echo("<option value='" . $i . "' ");
			if ((!$newEmployer && $i == '1') || $newEmployer->job_min_workterms == $i)
			{
				echo("selected='selected'");
			}
			echo(">" . (($i == '5') ? ($i . "+") : $i) . "</option>");
		}
	
		echo("</select>");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>Receive applications from students in these departments:</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2'>");
		echo("<table border='0'>");
	
		$sql = ("
			SELECT department_id, department_name
			FROM department
			WHERE job_list = '1'
			ORDER BY department_name
			");
		$result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow())
        {
            echo("<tr>");
            echo("<td>");
            echo("<input type='checkbox' class='row2' name='apply_departments[]' value='" . $row["department_id"] . "' ");
            if (is_array($apply_departments) && in_array($row["department_id"], $apply_departments))
            {
                echo("checked='checked'");
            }
            echo(" />" . $row["department_name"] . "</td>");
            echo("</tr>");

            $arrDiscipline = getDisciplinesForDepts($row['department_id']);

            echo("<tr>");
            echo("<td>");
            if (sizeof($arrDiscipline[$row['department_id']]) > 0) {

                $discipline_list = array();

                for ($x = 0; $x < sizeof($arrDiscipline[$row['department_id']]); $x++) {
                    $discipline_list[] = $arrDiscipline[$row['department_id']][$x]['name'];
                }
                $discipline_string = "(".implode(", ",$discipline_list).")";
                echo("<blockquote>");
                echo("<font size='2'>");
                echo(htmlentities($discipline_string));
                echo("</font>");
                echo("</blockquote>");
            }
            echo("</td>");
            echo("</tr>");
        }
	
    /*
		$rownew = 1;
	
		while ($row = $result->FetchRow())
		{
			if ($rownew % 2)
			{
				echo("<tr>");
			}
			echo("<td>");
			echo("<input type='checkbox' class='row2' name='apply_departments[]' value='" . $row["department_id"] . "' ");
			if (is_array($newEmployer->job_apply_departments) && in_array($row["department_id"], $newEmployer->job_apply_departments))
			{
				echo("checked='checked'");
			}
			echo(" />" . $row["department_name"] . "</td>");
			if (!($rownew % 2))
			{
				echo("</tr>");
			}
			$rownew++;
		}
	
		if ($rownew % 2)
		{
			echo("<td>&nbsp;</td>");
			echo("</tr>");
		}
        */
	
		echo("</table>");
		echo("</td>");
	echo("</tr>");
	echo("</table>");
	echo("</td>");
	echo("</tr>");
	echo("</table>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<hr />Fields marked with an asterisk (<b class='red'>*</b>) are required.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<hr />");
	echo("<input type='hidden' name='continue' value='collect_more_job_info' />");
	echo("<input type='submit' value='Continue' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");
echo("</form>");

?> 
