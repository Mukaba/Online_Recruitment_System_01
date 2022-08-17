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
 | Filename: add_job_form_b.inc                                                 |
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

 
   <form method="post" name='form1' action="<?php echo($PHP_SELF); ?>">
   
   <center>
   <table border="0" cellpadding="5" cellspacing="0" width="100%" class="row1">
   <tr>
   <td colspan='2'>&nbsp;
   <input type='hidden' name='job' value='<?php echo(packObject($job)); ?>' />
   <input type="hidden" name="select" value="add_job" />
   </td>
   </tr>
   <?php
   if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
   {
   ?>
   <tr>
        <td align="right">Closing Date:<br />(YYYY-MM-DD)</td>
        <td>
        <input type="text" name="closing_date" size="15" maxlength="10" value="<?php 
        if (($job) && (check_date($job->closing_date) != 0)) { echo($job->closing_date); } ?>" />
        &nbsp;<?php echo popup('closing_date','form1')?>
	</td>
   </tr>
   <tr>
        <td align="right">Closing Time:<br />(24 hour clock)</td>
        <td>
        <input type="text" name="closing_time" size="15" maxlength="10" value="<?php 
        echo( (($job) && (validTime($job->closing_time) != -1)) ? ($job->closing_time) : ("10:00")); ?>" />
        </td>
   </tr>
   <?php
   }
   if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
   {    
	   echo("<tr>");
	        echo("<td align='right'>Status of job once entered?</td>");
	        echo("<td><select name='admin_status'>");
		
		if ($userlevel == OFFICE)
		{
			$sql = ("
				SELECT DISTINCT *
				FROM admin_status
				WHERE job_status_id IN (" . NOT_CHECKED . ", " . CHECKED . ", " . HOLDING . ")
				AND status_id NOT IN (" . NOT_CHECKED_EMPLOYER . ")
				ORDER BY job_status_id
				");
		}
		elseif ($userlevel == TRAFFICDIRECTOR)
		{
			$sql = ("
				SELECT DISTINCT *
				FROM admin_status
				WHERE job_status_id IN (" . NOT_CHECKED . ", " . CHECKED . ", " . HOLDING . ", " . POSTED . ")
				AND status_id NOT IN (" . NOT_CHECKED_EMPLOYER . ")
				ORDER BY job_status_id
				");
		}

		$result = $GLOBALS['dbh']->Execute($sql);

		while ($row = $result->FetchRow())
		{
			echo("<option value='" . $row["status_id"] . "' ");
			if ($job->admin_status == $row["status_id"])
			{
				echo("selected='selected'");
			}
			echo(">" . $row["status_name"] . "</option>");
		}
   	     echo("</select>");
   	     echo("</td>");
   	echo("</tr>");
   } 
   ?>  
   <tr>
	<td align='right' nowrap='nowrap'>Title of position: <b class='red'>*</b></td>
	<td><input type="text" name="position_title" size="40" maxlength="50" value="<?php echo( ($job) ? $job->position_title : ""); ?>" />
	</td>
   </tr>
   <tr>
	<td align='right' nowrap='nowrap'>Employer's internal job code (if applicable):</td>
	<td><input type='text' name='employer_code' size='15' maxlength='30' value='<?php echo( ($job) ? $job->employer_code : ""); ?>' />
	</td>
   </tr>
   <tr>
	<td align="right" nowrap='nowrap'>Number of positions available: <b class='red'>*</b></td>
	<td><input type="text" name="num_positions" size='5' maxlength='4' value='<?php echo( ($job) ? $job->num_positions : ""); ?>' />
	</td>
   </tr>
   <tr>
   	<td align="right">How would you like to receive applications?</td>
	<td>
	<input class="row2" type="radio" name="apply_method" value="<?php echo(JOB_APP_METHOD_ONLINE); ?>"
	<?php 
	if ( !($job->apply_method) || (($job) && ($job->apply_method == JOB_APP_METHOD_ONLINE))) 
	{ 
		echo("checked='checked'"); 
	} 
	?>
	 />Online&nbsp;
	<input class="row2" type="radio" name="apply_method" value="<?php echo(JOB_APP_METHOD_PAPER); ?>"
	<?php 
	if (($job) && ($job->apply_method == JOB_APP_METHOD_PAPER)) 
	{ 
		echo("checked='checked'"); 
	} 
	?>
	 />Paper&nbsp;
	<?php
	if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) {
	?>
	<input class="row2" type="radio" name="apply_method" value="<?php echo(JOB_APP_METHOD_SPECIAL); ?>"
	<?php
	if (($job) && ($job->apply_method == JOB_APP_METHOD_SPECIAL))
	{ 
		echo("checked='checked'"); 
	} 
	?>
	 />Special&nbsp;
	<?php
	}
	?>
	</td>
   </tr>
   <tr>
	<td align="right">E-mail address for online applications: <b class='red'>*</b><br />(leave blank if unapplicable)</td>
	<td><input type="text" name="receive_address" size="40" value="<?php 
	echo( ($job->receive_address) ? $job->receive_address : $job->contact_email); ?>" />
	</td>
   </tr>
   <tr>
	<td colspan='2' align="center">Location of job offered:</td>
   </tr>
   <tr>
	<td align='center' colspan='2'>
	<table cellpadding='3' cellspacing='3' class='row2'>
	<tr>
	<td class='row2'>
	<table width='100%' class='row2'>
	<tr>
        	<td align="right">Country:</td>
        	<td>
        	<select name="country" onchange='update_prov()'>
        	<?php
		
		/*
		 The below if and elseif statements are used to find out if we have a country for the
		 employer that this job is being added for.  If we do, then we want to use this country
		 as the default setting.
		*/

		if ($job->department_id && $job->department_id != 'none')
		{
			$sql = ("
				SELECT DISTINCT country_id
				FROM employer_department
				WHERE department_id='" . $job->department_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$countryrow = $result->FetchRow();
		}
		if ($job->employer_id && !$countryrow["country_id"])
		{
			$using_employer_location = TRUE;
			$sql = ("
				SELECT DISTINCT country_id
				FROM employer_company
				WHERE employer_id='" . $job->employer_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$countryrow = $result->FetchRow();
		}

        	$sql = ("
        	        SELECT DISTINCT country_id, country_name
        	        FROM country_list
        	        ORDER BY order_by
        	        ");
        	$result = $GLOBALS['dbh']->Execute($sql);
        	while ($row = $result->FetchRow())
        	{
        	        echo("<option value='" . $row["country_id"] . "'");
       		        if (!($job->country))
			{
				if ($countryrow["country_id"]) 
				{
					if ($row["country_id"] == $countryrow["country_id"])
					{
						echo(" selected='selected'");
					}
				}	
				elseif ($row["country_id"] == 1)
				{
					echo(" selected='selected'");
				}
			}
			elseif (($job) && ($job->country == $row["country_id"]))
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
		<td align="right">Province/State:<br />(If in Canada or US)</td>
		<td>
		<select name="provstate" onchange="update_region()">
		<?php
	
		/*
                 The below if and elseif statements are used to find out if we have a provicne/state for the
                 employer that this job is being added for.  If we do, then we want to use this provicne/state
                 as the default setting.
                */	

		if (!$using_employer_location && $job->department_id && $job->department_id != 'none')
                {
                        $sql = ("
                                SELECT DISTINCT provstate_id
                                FROM employer_department
                                WHERE department_id='" . $job->department_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $provstaterow = $result->FetchRow();
                }
                elseif ($job->employer_id)
                {
                        $sql = ("
                                SELECT DISTINCT provstate_id
                                FROM employer_company
                                WHERE employer_id='" . $job->employer_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $provstaterow = $result->FetchRow();
                }

		/*
		 We were running into problems here when the page would be reloaded by add_job_form.inc after receiving 
		 incorrect input.  We would end up with the original country selected, but instead of having N/A for
		 the provstate and region, we had the original Canadian and British Columbian choices, respectively.
		 To fix this problem, the code below has been put in here, and in the region section.
		*/
	
		$get_country = ($job->country ? $job->country : ($countryrow["country_id"] ? $countryrow["country_id"] : 1));
	
		/*
		 If the country we have is either Canada, US, or hasn't been chosen yet, then either load the provinces
		 for the US, or Canada (if it's Canada or unchosen).  Otherwise, just display a N/A option.
		*/
	
		if ($get_country == 1 || $get_country == 2)
		{
			$sql = ("
				SELECT DISTINCT provstate_id, provstate_name
				FROM provstate_list
				WHERE country_id='" . addslashes($get_country) . "'
				ORDER BY provstate_id
				");
	        	$result = $GLOBALS['dbh']->Execute($sql);

	        	while ($row = $result->FetchRow())
	        	{
        		        echo("<option value='" . $row["provstate_id"] . "'");
	        	        if (!($job->provstate))
				{
					if ($provstaterow)
					{
						if ($row["provstate_id"] == $provstaterow["provstate_id"])
						{
							echo(" selected='selected'");
						}
					}
					elseif ($row["provstate_id"] == 1) 
	        	        	{
	        	        	        echo(" selected='selected'");
	        	        	}
				}
				elseif (($job) && ($job->provstate == $row["provstate_id"]))
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
		?>
		</select>
        	</td>
   	</tr>	
   	<tr>
		<td align="right">Region:<br />(If in Canada)</td>
		<td>
		<select name="region" onchange='update_city()'>
		<?php
		$get_prov = ($job->provstate ? $job->provstate : ($provstaterow["provstate_id"] ? $provstaterow["provstate_id"] : $job->provstate));

		/*
                 The below if and elseif statements are used to find out if we have a region for the
                 employer that this job is being added for.  If we do, then we want to use this region
                 as the default setting.
                */

		if (!$using_employer_location && $job->department_id && $job->department_id != 'none')
                {
                        $sql = ("
                                SELECT DISTINCT region_id
                                FROM employer_department
                                WHERE department_id='" . $job->department_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $regionrow = $result->FetchRow();
                }
                if ($job->employer_id && !$regionrow["region_id"])
                {
                        $sql = ("
                                SELECT DISTINCT region_id
                                FROM employer_company
                                WHERE employer_id='" . $job->employer_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $regionrow = $result->FetchRow();
                }
		
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
				SELECT DISTINCT region_id, region_name
				FROM region_list
				WHERE provstate_id='" . addslashes($get_prov) . "'
				ORDER BY region_id
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			while ($row = $result->FetchRow())
			{
				echo("<option value='" . $row["region_id"] . "'");
				if (!($job->region))
				{
					if ($regionrow)
					{
						if ($row["region_id"] == $regionrow["region_id"])
						{
							echo(" selected='selected'");
						}
					}
					elseif ($row["region_id"] == 1)
					{
						echo(" selected='selected'");
					}
				}
				elseif (($job) && ($job->region == $row["region_id"]))
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
		?>
		</select>
		</td>
	</tr>

	<?php

	/*
	 If the employer's division or company has a city associated with it, then use this as the
	 default value.
	*/

	if (!$using_employer_location && $job->department_id && $job->department_id != 'none')
	{
		$sql = ("
			SELECT DISTINCT city
			FROM employer_department
			WHERE department_id='" . $job->department_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$cityrow = $result->FetchRow();
	}
	if ($job->employer_id && !$cityrow["city"])
	{
		$sql = ("
			SELECT DISTINCT city
			FROM employer_company
			WHERE employer_id='" . $job->employer_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$cityrow = $result->FetchRow();
	}
	?>

   	<tr>
		<td align='right'>City: <b class='red'>*</b></td>
		<td>
		<input type='text' size='30' maxlength='50' name='city' value="<?php 
		echo(($job->city) ? ($job->city) : ($cityrow["city"] ? $cityrow["city"] : "")); 
		?>" />
		</td>
	</tr>
	</table>
	</td>
	</tr>
	</table>
	</td>
   </tr>
   <tr>
	<td colspan='2'>&nbsp;</td>
   </tr>
   <tr>
	<td align="right">Approximate Salary Range:<br />(leave blank if this will <br /> be discussed later)</td> 
	<td nowrap='nowrap'>$&nbsp;<input type="text" name="salary_amount1" size="8" maxlength="8" value="<?php 
	echo(($job) ? stripSlashes($job->salary_amount1) : ""); ?>" /> &nbsp;-&nbsp;$
	<input type='text' name='salary_amount2' size='8' maxlength='8' value="<?php
	echo( ($job) ? stripSlashes($job->salary_amount2) : ""); ?>" /> &nbsp;per&nbsp;
	<select name="salary_period">
	<option value="hour" <?php if (($job) && ($job->salary_period == hour)) { echo ("selected='selected'"); } ?>>&nbsp;Hour&nbsp;</option>
	<option value="week" <?php if (($job) && ($job->salary_period == week)) { echo ("selected='selected'"); } ?>>&nbsp;Week&nbsp;</option>
	<option value="2 weeks" <?php if (($job) && ($job->salary_period == '2 weeks')) { echo ("selected='selected'"); } ?>>&nbsp;2 Weeks&nbsp;</option>
	<option value="month" <?php if (($job) && ($job->salary_period == month)) { echo ("selected='selected'"); } ?>>&nbsp;Month&nbsp;</option>
	<option value="year" <?php if (($job) && ($job->salary_period == year)) { echo ("selected='selected'"); } ?>>&nbsp;Year&nbsp;</option>
	</select>
	</td>
   </tr>
   <tr>
	<td align='right'>Hours of work per week:</td>
	<td>
	<input type='text' name='workterm_hours' size='10' maxlength='10' value="<?php echo(($job) ? $job->workterm_hours : ""); ?>" />
	</td>
   </tr>

   <tr align="center">
   <td colspan="2">
   <table cellpadding='3' cellspacing='3'>
   <tr>
   <td class='row2'>
   <table width='100%' class='row2'>
   <tr>
	<td align="right">Term:</td>
	<td>
	<select name="start_term">
	<?php
	
	/*
	 We need to figure out what term we are currently in.
	*/

	$nextTermYear = getNextTermYear();
	$nextterm = $nextTermYear["term"];

	$sql = ("
		SELECT DISTINCT term_id, term_name, start_date, end_date
		FROM term
		ORDER BY start_date
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["term_id"] . "'");
		if (($job && $job->start_term == $row["term_id"]) || (!($job->start_term) && ($row["term_id"] == $nextterm)))
		{
			echo(" selected='selected'");
		}
		echo(">" . getMonth($row["start_date"])."</option>");
	}
	?>
	</select>
	&nbsp;
	<select name="start_year">
	<?php
	$date = explode("-", date("Y-m-d"));
	$limit = $date[0] + 1;

	for ($i = $limit; $i >= $date[0]; $i--)
	{
		echo("<option value='$i'");
		if ((($job) && ($job->start_year == $i)) || (!($job->start_year) && ($i == $nextTermYear["year"])))
		{ 
			echo("selected='selected'"); 
		} 
		echo(">&nbsp;" . $i . "&nbsp;</option>");
	}
	?>

	</select>
	</td>
   </tr>
   <tr>
	<td align='right'>Workterm Length(s):</td>
	<td>
	<?php
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
		if (($job && is_array($job->workterm_lengths) && in_array($i, $job->workterm_lengths)) 
		|| (!$job->workterm_lengths && $i == $multiple))
		{
			echo(" checked='checked'");
		}
		echo(" />" . $i . " or ");
	}
	echo("<input type='checkbox' class='row1' name='workterm_lengths[]' value='" . $i . "'");
	if (($job && is_array($job->workterm_lengths) && in_array($i, $job->workterm_lengths))
	|| (!$job->workterm_lengths && $i == $multiple))
	{
		echo(" checked='checked'");
	}
	echo(" />" . $i . " months");
	?>
	</td>
   </tr>
   <tr>
   <td colspan="2">&nbsp;</td>
   </tr>
   <tr>
     <td align="center" colspan="2">
       -or-
     </td>
   </tr>
   <tr>
   <td colspan="2">&nbsp;</td>
   </tr>
   <tr>
     <td align="center" colspan="2">
       Normal start months are January, May, and September.<br /> A limited number of students may be available starting at other times. 
     </td>
   </tr>
   <tr>
   <td colspan="2">&nbsp;</td>
   </tr>
   <tr>
     <td align="right">
       Start Date:
     </td>
     <td>
       <input type='text' size="2" maxlength="2" name="start_date_date" value="<?php echo($start_date_date); ?>" />&nbsp; 
       <select name="start_date_month">
       <option value="">&nbsp;</option>
       <?php
        for($i = 1; $i <= 12; $i++)
        {
            echo("<option value='".$i."' ");
            if ($i == $start_date_month)
            {
                echo("selected='selected'");
            }
            echo(">");
            echo(getMonth($i));
            echo("</option>");
        }
       ?>
       </select>
       <select name="start_date_year">
       <option value="">&nbsp;</option>
       <?php
        for($i = date("Y") + 1; $i >= date("Y"); $i--)
        {
            echo("<option value='".$i."' ");
            if ($i== $start_date_year)
            {
                echo("selected='selected'");
            }
            echo(">");
            echo($i);
            echo("</option>");
        }
       ?>
       </select>
       (DD-Month-YYYY)
     </td>
   </tr>
   <tr>
     <td align="right">
       End Date:
     </td>
     <td>
       <input type='text' size="2" maxlength="2" name="end_date_date" value="<?php echo($end_date_date); ?>" />&nbsp; 
       <select name="end_date_month">
       <option value="">&nbsp;</option>
       <?php
        for($i = 1; $i <= 12; $i++)
        {
            echo("<option value='".$i."' ");
            if ($i == $end_date_month)
            {
                echo("selected='selected'");
            }
            echo(">");
            echo(getMonth($i));
            echo("</option>");
        }
       ?>
       </select>
       <select name="end_date_year">
       <option value="">&nbsp;</option>
       <?php
        for($i = date("Y") + 1; $i >= date("Y"); $i--)
        {
            echo("<option value='".$i."' ");
            if ($i== $end_date_year)
            {
                echo("selected='selected'");
            }
            echo(">");
            echo($i);
            echo("</option>");
        }
       ?>
       </select>
       (DD-Month-YYYY)
     </td>
   </tr>
   </table>
   </td>
   </tr>
   </table>
   </td>
   </tr>

   <tr>
	<td align='right'>This job is also posted at:</td>
	<td><input type='text' name='other_postings' size='50' maxlength='100' value="<?php 
	echo(($job) ? ($job->other_postings) : ""); ?>" />
	</td>
   </tr>

   <tr>
        <td align='center' colspan='2'>Profile of applicant:
   </td>
   </tr>
   <tr>
        <td align='center' colspan='2'>
        <table cellpadding='3' cellspacing='3'>
        <tr>
        <td class='row2'>
        <table width='100%' class='row2'>
        <tr>
                <td align='center'>Minimum academic year completed:
                <select name='min_academic'>
		<?php
		
		for ($i = 1; $i < 5; $i++)
		{
			echo("<option value='" . $i . "' ");
			if ((!$job && $i == '1') || $job->min_academic == $i)
			{
				echo("selected='selected'");
			}
			echo(">" . $i . "</option>");
		}
		
		?>
                </select>
                </td>
        </tr>
        <tr>
                <td align='center'>Minimum workterms completed:
                <select name='min_workterms'>
		<?php
		
		for ($i = 0; $i < 6; $i++)
		{
			echo("<option value='" . $i . "' ");
			if ((!$job && $i == '1') || $job->min_workterms == $i)
			{
				echo("selected='selected'");
			}
			echo(">" . (($i == '5') ? ($i . "+") : $i) . "</option>");
		}

		?>
                </select>
                </td>
        </tr>
	<tr>
		<td colspan='2'>&nbsp;</td>
	</tr>
	<tr>
		<td colspan='2' align='center'>Receive applications from students in these departments:</td>
	</tr>
	<tr>
		<td colspan='2'>
		<table border='0'>
		<?php
	
		$sql = ("
			SELECT DISTINCT department_id, department_name
			FROM department
			WHERE job_list
			ORDER BY department_name
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		// Put this line in the above query when we want to limit the departments shown
		//WHERE department_id IN ('1','2')
	
		while ($row = $result->FetchRow())
		{
            echo("<tr>");
			echo("<td>");
			echo("<input type='checkbox' class='row2' name='apply_departments[]' value='" . $row["department_id"] . "' ");
			if (is_array($job->apply_departments) && in_array($row["department_id"], $job->apply_departments))
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
	
		?>
		</table>
		</td>
	</tr>
        </table>
        </td>
        </tr>
        </table>
        </td>
   </tr>
   <tr>
	<td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class='red'>*</b>) are required for the default form settings</td>
   </tr>
   <tr>
	<td colspan='2' align='center'><hr />
	<input type='hidden' name='continue' value='level2' />
	<input type="submit" value="Continue" />
	</td>
   </tr>
   </table>
   </center>
   </form>

 
