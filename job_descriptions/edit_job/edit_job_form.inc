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
 | Filename: edit_job_form.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This is the first of the edit job forms.  This lets people      |
 | edit various attributes of the jobs in the system provided that they have    |
 | passed the security clearance.                                               |
 +------------------------------------------------------------------------------+

*/

/*
 Arrays created below are used by java script
*/

$max_country_id=0;
$country_array = array();
$result = $GLOBALS['dbh']->Execute("SELECT c.country_id, provstate_id, provstate_name
                        FROM country_list as c LEFT JOIN provstate_list as p ON c.country_id = p.country_id
                        ORDER BY c.order_by");
while ($row = $result->FetchRow()){
        if ($row["provstate_id"] == ""){
                $str = "Array(\"NULL\"),";
                //$str = "Array(),";
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
$result = $GLOBALS['dbh']->Execute("SELECT p.provstate_id, r.region_id, r.region_name
                        FROM provstate_list as p LEFT JOIN region_list as r ON p.provstate_id = r.provstate_id
                        ORDER BY p.provstate_id");
while ($row = $result->FetchRow()){
        if ($row["region_id"] == ""){
                $str = "Array(\"NULL\"),";
                //$str = "Array(),";
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
            index = document.form1.country[document.form1.country.selectedIndex].value;
            prov = prov_array[index];
            if (prov.length==1 && prov[0] == "NULL"){
                document.form1.provstate.options.length = 1;
                document.form1.provstate.options[0] = new Option("N/A", "");
                document.form1.provstate.selectedIndex = 0;
                document.form1.region.options.length=1;
                document.form1.region.options[0] = new Option("N/A", "");
                document.form1.region.selectedIndex = 0;
                document.form1.city.value = "";

            }else{

                document.form1.provstate.options.length =0;
                document.form1.provstate.options.length = prov.length;
                for($i=0;$i<prov.length;$i++)
                {
                    document.form1.provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                }
                document.form1.provstate.selectedIndex = 0;

                index2=document.form1.provstate[document.form1.provstate.selectedIndex].value;
                region = region_array[index2];
                if (region.length==1 && region[0] == "NULL"){
                    document.form1.region.options.length=1;
                    document.form1.region.options[0] = new Option("N/A", "");
                    document.form1.city.value = "";
                }else{
                    document.form1.region.options.length=0;
                    document.form1.region.options.length=region.length;
                    for($i=0;$i<region.length;$i++){
                        document.form1.region.options[$i] = new Option(region[$i][1], region[$i][0]);
                    }
                    document.form1.region.selectedIndex = 0;

                    index3 = document.form1.region[document.form1.region.selectedIndex].value;
                    city = city_array[index3];
                    document.form1.city.value = city;
                }

            }
        }

function update_region(){

    document.form1.region.options.length=0;
    index2=document.form1.provstate[document.form1.provstate.selectedIndex].value;
    region = region_array[index2];
    if (region.length==1 && region[0] == "NULL"){
        document.form1.region.options.length=1;
        document.form1.region.options[0] = new Option("N/A", "");
        document.form1.city.value = '';
    }else{
        document.form1.region.options.length = region.length;
        for($i=0;$i<region.length;$i++){
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

<form method="post" name='form1' action="<?php echo($URL); ?>">

<table border="0" cellpadding="8" cellspacing="0" width="100%" class="row1">
<tr>
	<td colspan='2'>&nbsp;
    <input type="hidden" name="select" value="edit_job" />
    <input type='hidden' name='level1' value='job_info' />
    <input type='hidden' name='job_id' value='<?php echo($job_id); ?>' />
    </td>
</tr>
<?php
if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{
echo("<tr>");
	echo("<td align='right' nowrap='nowrap'>Job Code:</td>");
	echo("<td>");
	echo("<b class='black'>");
	echo(($job->job_code) ? $job->job_code : "Not Yet Set");
	echo("</b>");
	echo("</td>");
echo("</tr>");
}
else
{
echo("<tr>");
	echo("<td colspan='2'>");
	echo("<input type='hidden' name='job_code' value='" . $job->job_code . "' />");
	echo("</td>");
echo("</tr>");
}

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{
echo("<tr>");
	echo("<td align='right'>Closing date:<br />(YYYY-MM-DD)</td>");
	echo("<td>");
	echo("<input type='text' name='closing_date' size='15' maxlength='10' value='");
	if (($job) && ($job->closing_date != -1)) 
	{ 
		echo($job->closing_date); 
	}
	echo("' />");
	echo("&nbsp;" . popup('closing_date','form1'));
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>Closing time:<br />(24 hour clock)</td>");
	echo("<td>");
	echo("<input type='text' name='closing_time' size='15' maxlength='10' value='");
	if (($job) && (validTime($job->closing_time) != -1)) 
	{ 
		echo($job->closing_time); 
	} 
	else 
	{ 
		echo("10:00"); 
	}
	echo("' />");
	echo("</td>");
echo("</tr>");
}
else
{
	/*
	 If ever there is time in the future, it would be a much better idea to break this file up into two, one for the employer, one for
	 an administrator.  That way, the employer couldn't override the hidden fields manually.
	*/

echo("<tr>");
	echo("<td colspan='2'>");
	echo("<input type='hidden' name='closing_date' value='" . $job->closing_date . "' />");
	echo("<input type='hidden' name='closing_time' value='" . $job->closing_time . "' />");
	echo("</td>");
echo("</tr>");
}

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{
	echo("<tr>");
		echo("<td align='right'>Status of Job:</td>");
		echo("<td>");
		
		if ($job->status != HOLDING || $userlevel == TRAFFICDIRECTOR)
		{
			echo("<select name='admin_status'>");
			
			$status_string = adminStatusString($job->job_id);

			$sql = ("
				SELECT DISTINCT status_id, status_name
				FROM admin_status
				WHERE job_status_id != " . INT_ONLY . "
				" . ((!$job->is_rejectable) ? ("AND job_status_id != " . REJECTED) : "") . "
				AND status_id IN (" . $status_string . ")
				ORDER BY order_by
				");
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
		}
		else
		{
			echo("<b class='black'>" . getAdminStatusName($job->admin_status) . "</b>");
		}

		echo("</td>");
	echo("</tr>");
}

echo("<tr>");
	echo("<td align='right' nowrap='nowrap'>Title of position: <b class='red'>*</b></td>");
	echo("<td>");
	echo("<input type='text' name='position_title' size='30' maxlength='50' ");
	echo(($job) ? "value='" . $job->position_title . "'" : "");
	echo(" />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right' nowrap='nowrap'>Employer job code:</td>");
	echo("<td>");
	echo("<input type='text' name='employer_code' size='15' maxlength='30' ");
	echo( ($job) ? "value='" . $job->employer_code . "'" : "");
	echo(" />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right' nowrap='nowrap'>Number of positions available:</td>");
	echo("<td><input name='num_positions' type='text' size='5' maxlength='4'");
	echo( ($job) ? "value='" . $job->num_positions . "'" : "");
	echo(" />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='right'>How would you like to receive applications?</td>");

	echo("<td><input class='row1' type='radio' name='apply_method' value='".JOB_APP_METHOD_ONLINE."' ");
	if (!($job) || (($job) && ($job->apply_method == JOB_APP_METHOD_ONLINE))) { echo("checked='checked'"); } 
	echo(" />Online&nbsp;");

	echo("<input class='row1' type='radio' name='apply_method' value='".JOB_APP_METHOD_PAPER."' ");
	if (($job) && ($job->apply_method == JOB_APP_METHOD_PAPER)) { echo("checked='checked'"); } 
	echo(" />Paper&nbsp;");

	if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) 
	{
		echo("<input class='row1' type='radio' name='apply_method' value='".JOB_APP_METHOD_SPECIAL."' ");
		if (($job) && ($job->apply_method == JOB_APP_METHOD_SPECIAL)) { echo("checked='checked'"); }
		echo(" />Special&nbsp;");
	}
	echo("</td>");
echo("</tr>");

?>
<tr>
<td align="right">E-mail address for online applications: <b class='red'>*</b><br />(leave blank if unapplicable)</td>
<td><input type="text" name="receive_address" size="30" value="<?php echo( ($job) ? ($job->receive_address) : ""); ?>" />
</td>
</tr>
<tr>
<td colspan='2'>&nbsp;</td>
</tr>
<tr> 
<td colspan='2' align="center">Location of job offered:</td>
</tr>
   <tr> 
        <td align='center' colspan='2'>
        <table class='row2' cellpadding='3' cellspacing='3'>
        <tr>
        <td class='row2'>
        <table width='100%' class='row2'>
	<tr>
	        <td align="right">Country:</td>
	        <td>
	        <select name="country" onchange='update_prov()'>
	        <?php
	        $sql = ("
	                SELECT country_id, country_name
	                FROM country_list
	                ORDER BY order_by
	                ");
	        $result = $GLOBALS['dbh']->Execute($sql);
	        while ($row = $result->FetchRow())
	        {
	                echo("<option value='" . $row["country_id"] . "'");
	                if ((!($job->country) && ($row["country_id"] == 1)) || (($job) && ($job->country == $row["country_id"])))
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
	        <select name="provstate" onchange='update_region()'>
	        <?php
	        $sql = ("
	                SELECT provstate_id, provstate_name
	                FROM provstate_list
                    WHERE country_id='" . addslashes($job->country) . "'
	                ORDER BY provstate_name
	                ");
	        $result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount())
		{
	        	while ($row = $result->FetchRow())
	        	{
	        	        echo("<option value='" . $row["provstate_id"] . "'");
	        	        if ((!($job->provstate) && ($row["provstate_id"] == 1)) || (($job) && ($job->provstate == $row["provstate_id"])))
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
	        $sql = ("
	                SELECT region_id, region_name
	                FROM region_list
                    WHERE provstate_id='" . $job->provstate . "'
	                ORDER BY region_name
	                ");
	        $result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount())
		{
	        	while ($row = $result->FetchRow())
	        	{
	        	        echo("<option value='" . $row["region_id"] . "'");
	        	        if ((!($job->region) && ($row["region_id"] == 1)) || (($job) && ($job->region == $row["region_id"])))
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
	<tr>
		<td align="right">City: <b class='red'>*</b></td>
		<td><input type="text" name="city" size="20" maxlength="30" value="<?php echo( ($job) ? ($job->city) : ""); ?>" />
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
        <td nowrap='nowrap'>$<input type="text" name="salary_amount1" size="8" maxlength="8" value="<?php
        echo( ($job) ? ($job->salary_amount1) : ""); ?>" /> &nbsp;to&nbsp;$
        <input type='text' name='salary_amount2' size='8' maxlength='8' value="<?php
        echo( ($job) ? ($job->salary_amount2) : ""); ?>" /> &nbsp;per&nbsp;
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
	<td><input type='text' name='workterm_hours' size='6' maxlength='10' value='<?php 
	echo(($job->workterm_hours) ? ($job->workterm_hours) : ""); ?>' />
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

        $current_term = getCurrentTermID();
        $current_term++;
        if ($current_term > 3)
        {
                $current_term -= 3;
        }
        $nextterm = $current_term;

        $sql = ("
                SELECT term_id, term_name, start_date, end_date
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
                echo(">" .getMonth($row["start_date"])."</option>");
        }
        ?>
        </select>
        &nbsp;
	<select name="start_year">
        <?php
        $date = explode("-",date("Y-m-d")); 
        $limit = $date[0] + 1;

        for ($i = $limit; $i >= $date[0]; $i--)
        {
                echo("<option value='$i'");
                if (($job) && ($job->start_year == $i))
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
        <td align='right'>Workterm Length(s): <b class='red'>*</b></td>
        <td>
        <?php
        $currentTerm = getCurrentTermID();
        $sql = ("
                SELECT start_date, end_date
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
       <input type='text' size="2" maxlength="2" name="start_date_date" value="<?php echo($job->start_date_date); ?>" />&nbsp; 
       <select name="start_date_month">
       <option value="">&nbsp;</option>
       <?php
        for($i = 1; $i <= 12; $i++)
        {
            echo("<option value='".$i."' ");
            if ($i == $job->start_date_month)
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
            if ($i== $job->start_date_year)
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
       <input type='text' size="2" maxlength="2" name="end_date_date" value="<?php echo($job->end_date_date); ?>" />&nbsp; 
       <select name="end_date_month">
       <option value="">&nbsp;</option>
       <?php
        for($i = 1; $i <= 12; $i++)
        {
            echo("<option value='".$i."' ");
            if ($i == $job->end_date_month)
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
            if ($i== $job->end_date_year)
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
	<td>
	<input type='text' name='other_postings' size='50' maxlength='60' value="<?php 
	echo( ($job) ? ($job->other_postings) : ""); ?>" />
	</td>
   </tr>

<!-- We are no longer assigning industries to jobs

   <tr>
	<td align='right'>Job's Industry:</td>
	<td>
	<select name='industry'>
	<?php
	/*

	$sql = ("
		SELECT DISTINCT industry_id, industry_name
		FROM industries
		WHERE industry_name != '?'
		ORDER BY order_by
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["industry_id"] . "' ");
		if ($job->industry == $row["industry_id"])
		{
			echo("selected");
		}
		echo(">" . $row["industry_name"] . "</option>");
	}

	*/
	?>
	</select>
	</td>
   </tr>
-->
   <tr>
	<td colspan='2'>&nbsp;</td>
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
		<td align="right">Minimum academic year completed:</td>
		<td>
		<select name='min_academic'>
		<option value='1' <?php echo((!$job || $job->min_academic == '1') ? " selected='selected'" : ""); ?>>1 year</option>
		<option value='2' <?php echo(($job->min_academic == '2') ? " selected='selected'" : ""); ?>>2 years</option>
	    	<option value='3' <?php echo(($job->min_academic == '3') ? " selected='selected'" : ""); ?>>3 years</option>
	        <option value='4' <?php echo(($job->min_academic == '4') ? " selected='selected'" : ""); ?>>4+ years</option>
	        </select>
	        </td>
     	</tr>
   	<tr>
	        <td align='right'>Minimum workterms completed:</td>
	        <td>
	        <select name='min_workterms'>
	        <option value='0' <?php echo((!$job || $job->min_workterms == '0') ? " selected='selected'" : ""); ?>>0</option>
	        <option value='1' <?php echo(($job->min_workterms == '1') ? " selected='selected'" : ""); ?>>1</option>
	        <option value='2' <?php echo(($job->min_workterms == '2') ? " selected='selected'" : ""); ?>>2</option>
	        <option value='3' <?php echo(($job->min_workterms == '3') ? " selected='selected'" : ""); ?>>3</option>
	        <option value='4' <?php echo(($job->min_workterms == '4') ? " selected='selected'" : ""); ?>>4</option>
	        <option value='5' <?php echo(($job->min_workterms == '5') ? " selected='selected'" : ""); ?>>5+</option>
	        </select>
	        </td>
     	</tr>	
	<?php

	if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
	{
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
	                        WHERE job_list = 1
	                        ORDER BY department_name
	                        ");
	                $result = $GLOBALS['dbh']->Execute($sql);
	
	                $rownew = 1;
	
	                while ($row = $result->FetchRow())
	                {
	                        if ($rownew % 2)
	                        {
	                                echo("<tr>");
	                        }
	                        echo("<td>");
	                        echo("<input type='checkbox' class='row2' name='apply_departments[]' value='" . $row["department_id"] . "' ");
	                        if (is_array($job->apply_departments) && in_array($row["department_id"], $job->apply_departments))
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
	                        echo("<tr><td>&nbsp;</td>");
	                        echo("</tr>");
	                }
	
	                echo("</table>");
	                echo("</td>");
	        echo("</tr>");
	}
	elseif ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
	{
		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>");

		echo("<tr>");
			echo("<td colspan='2' align='center'>You will receive applications from students in these departments:</td>");
		echo("</tr>");

		echo("<tr>");
			echo("<td colspan='2' align='center'>");

			if (is_array($job->apply_departments))
			{
				foreach ($job->apply_departments AS $ad)
				{
					$app_depts .= ("<b class='black'>" . getDepartmentName($ad) . "</b>, ");
				}
				$app_depts = substr($app_depts, 0, -2);
				echo($app_depts);
			}
			else
			{
				echo("No departments have been set yet.");
			}

			echo("</td>");
		echo("</tr>");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>");
	}

	echo("<tr>");
		echo("<td colspan='2' align='center'>Applicable disciplines for this job");
		echo("</td>");
	echo("</tr>");

	if ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
	{
		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			
			if (sizeof($job->apply_departments) > 3)
			{
				echo("Disciplines are only relevant when there are less than four applicable departments for a job.");
			}
			elseif ($job->job_disciplines)
			{
				foreach($job->job_disciplines AS $jd)
				{
					$job_disc .= ("<b class='black'>" . getDisciplineCode($jd) . "</b>, ");
				}
				$job_disc = substr($job_disc, 0, -2);
				echo($job_disc);
			}
			
			echo("</td>");
		echo("</tr>");
	}
	elseif ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
	{
		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo("<input type='submit' name='submit' value='Refresh Disciplines' />");
			echo("</td>");
		echo("</tr>");

		if (sizeof($job->apply_departments) > 3)
		{
			echo("<tr>");
				echo("<td colspan='2' align='center'>");
				echo("You may only choose disciplines for a job if<br />the number of applicable departments is less than four.");
				echo("</td>");
			echo("</tr>");
		}
		elseif ($job->apply_departments)
		{
			$newrow = 0;
			foreach ($job->apply_departments AS $ad)
			{
				$apply_depts .= ("'" . $ad . "',");
			}
			$apply_depts = substr($apply_depts, 0, -1);

			$sql = ("
				SELECT DISTINCT dd.discipline_id, dd.discipline_name
				FROM department_disciplines AS dd, department_discipline_join AS ddj
				WHERE ddj.department_id IN (" . $apply_depts . ")
				AND dd.discipline_id=ddj.discipline_id
                ORDER BY dd.discipline_name
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			while ($row = $result->FetchRow())
			{
				if (!($newrow % 2))
				{
					echo("<tr>");
				}

				echo("<td>");
				echo("<input type='checkbox' name='job_disciplines[]' value='" . $row["discipline_id"] . "' ");
				if (is_array($job->job_disciplines) && in_array($row["discipline_id"], $job->job_disciplines))
				{
					echo("checked='checked'");
				}
				echo(" />" . htmlentities($row["discipline_name"]));
				echo("</td>");

				if ($newrow % 2)
				{
					echo("</tr>");
				}
				$newrow++;
			}
			if ($newrow % 2)
			{
				//Finish off the last column
				echo("<td>&nbsp;</td>");
				echo("</tr>");
			}

			echo("<tr><td colspan='2'>&nbsp;</td></tr>");
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
	<td colspan='2'>&nbsp;</td>
   </tr>
   <tr>
	<td align="right">Job description: <b class='red'>*</b></td>
	<td><textarea name="job_description" rows="10" cols="40"><?php echo(($job) ? ($job->job_description) : ""); ?></textarea>
	</td>
   </tr>
   <tr>
	<td align="right">Skills required: <b class='red'>*</b></td>
	<td><textarea name="skills_required" rows="10" cols="40"><?php echo(($job) ? ($job->skills_required) : ""); ?></textarea>
	</td>
   </tr>
   <tr>
	<td align="right">Special job requirements:<br />(Drivers License required,<br />must be returning to an<br />academic term, etc.)</td>

	<td><textarea name="special_requirements" rows="8" cols="40"><?php echo(($job) ? ($job->special_requirements) : ""); ?></textarea>
	</td>
   </tr>
   <tr>
	<td align="right">Employer comments for Co-op staff:<br />(Only viewable by Co-op staff and employer, not students)</td>
	<td><textarea name="comments" rows="8" cols="40"><?php echo(($job) ? ($job->comments) : ""); ?></textarea>
	</td>
   </tr>
   <?php
   if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
   {
   ?>
   <tr>
	<td align='right'>Co-op staff comments:<br />(Only viewable by Co-op staff, not employer or students):</td>
	<td><textarea name='admin_comments' rows='8' cols='40'><?php
	echo(($job) ? ($job->admin_comments) : ""); ?></textarea>
	</td>
   </tr>
   <tr>
	<td align='right'>Job status comments for students:<br />(Only viewable by Co-op staff and students, not employer).</td>
	<td><textarea name='for_student_comments' rows='8' cols='40'><?php
	echo(($job) ? ($job->for_student_comments) : ""); ?></textarea>
	</td>
   </tr>
   <tr>
	<td align="right">Special instructions for applying: <br />(apply directly to employers site, etc.)</td>

	<td><textarea name="special_instructions" rows="8" cols="40"><?php echo (($job) ? ($job->special_instructions) : ""); ?></textarea>
	</td>
   </tr>
   <tr>
   	<td align="right">Special instructions button:<br /> When student clicks, their name will be <br />listed with this job in the Applications module. <br />
	(Only displayed if checked and <br />receiving applications is "Special", <br />eg: "Sign up for Presentation", "Applied to Employer's Site")</td>
	<td><input type='checkbox' name='show_special_instr_button' class='row1' value='1'<?php echo (($job->apply_method == JOB_APP_METHOD_SPECIAL) ? " checked='checked'" : ""); ?> /> Display Special Instruction Button <br />
	<input type='text' size="40" name='special_instr_button' value="<?php echo (($job) ? ($job->special_instr_button) : ""); ?>" />
	</td>
   </tr>
   <?php
   }
$checked = '';
if($job->displayname_on_coverltr == '1')
{
        $checked = "checked='checked'";
}
?>

<tr>
<td colspan='2'>
    <table cellpadding='5' cellspacing='5'>
    <tr>
    <td valign='top' align='right'>
       <input type='checkbox' name='displayname_on_coverltr' value='1' <?php echo($checked) ?> />
    </td>
    <td>
        Check this box if you wish for your name to be visible to students.  
        Knowing your name enables students to address their cover letters to you directly instead of 'Dear Sir/Madam'.
        For your privacy, you may remain anonymous to students if you leave the box unchecked.
    </td>
    </tr>
    </table>
</td>
</tr>

   <tr>
	<td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class='red'>*</b>) are required for the default form settings</td>
   </tr>
   <tr>
	<td align='center' colspan='2'><hr />
	<input type="submit" name="submit" value="Save Changes" />
	&nbsp;&nbsp;
	<input type="reset" name="Reset Form" value="Reset Changes" />
	</td>
   </tr>
   </table>
   </form>
