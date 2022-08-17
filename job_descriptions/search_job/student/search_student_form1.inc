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
 | Filename: search_student_form1.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called by search_student.inc and is used to        |
 | gather search inputs from the user.                                          |
 +------------------------------------------------------------------------------+

*/

/*
 The variables below are used to determine what options the student has to search on for the term and year.
*/
include_once('job_descriptions/folders/functions.inc');

$nextTermYear = getNextTermYear();
$dates = explode("-", $current_date);
$cur_year = $dates[0];

$max_area_id=0;
$area_array = array();

$sql = ("
	SELECT a.area_id, country_id, country_name
	FROM area_list AS a LEFT JOIN country_list AS c ON a.area_id=c.area_id
	ORDER BY area_id,c.order_by
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

$result = $GLOBALS['dbh']->Execute("SELECT c.country_id, provstate_id, provstate_name
                        FROM country_list as c LEFT JOIN provstate_list as p ON c.country_id = p.country_id
                        ORDER BY c.order_by");
while ($row = $result->FetchRow()){
        if ($row["provstate_id"] == ""){
		
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

$result = $GLOBALS['dbh']->Execute("SELECT p.provstate_id, r.region_id, r.region_name
                        FROM provstate_list as p LEFT JOIN region_list as r ON p.provstate_id = r.provstate_id
                        ORDER BY p.provstate_id");
while ($row = $result->FetchRow()){
        if ($row["region_id"] == ""){
                
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
<!--javascript

        var region_array = new <?php echo $region_array?>;
        var prov_array = new <?php echo $prov_state_array?>;
	var cntry_array = new <?php echo $cntry_array?>;

	function update_country()
	{
		index3 = document.form1.search_area[document.form1.search_area.selectedIndex].value;
		country = cntry_array[index3];
		if (document.form1.search_area[document.form1.search_area.selectedIndex].value == "")
		{
			document.form1.search_country.options.length = 1;
			document.form1.search_country.options[0] = new Option("Choose an Area", "");
			document.form1.search_country.selectedIndex = 0;
			document.form1.search_provstate.options.length = 1;
			document.form1.search_provstate.options[0] = new Option("Choose an Area", "");
			document.form1.search_provstate.selectedIndex = 0;
			document.form1.search_region.options.length = 1;
			document.form1.search_region.options[0] = new Option("Choose an Area", "");
			document.form1.search_region.selectedIndex = 0;
		}
		else
		{
			if (country.length == 1 && country[0] == "NULL")
			{
				document.form1.search_country.options.length = 1;
				document.form1.search_country[0] = new Option("N/A", "");
				document.form1.search_country.selectedIndex = 0;
				document.form1.search_provstate.options.length = 1;
				document.form1.search_provstate.options[0] = new Option("N/A", "");
				document.form1.search_provstate.selectedIndex = 0;
				document.form1.search_region.options.length = 1;
				document.form1.search_region.options[0] = new Option("N/A", "");
				document.form1.search_region.selectedIndex = 0;
			}
			else
			{
				document.form1.search_country.options.length = 0;
				document.form1.search_country.options.length = country.length;
				for ($i = 0; $i < country.length; $i++)
				{
					if (country[$i] == "NULL")
					{
						document.form1.search_country.options[$i] = new Option(" ", "");
					}
					else
					{
						document.form1.search_country.options[$i] = new Option(country[$i][1], country[$i][0]);
					}
				}
		
				if (document.form1.search_area[document.form1.search_area.selectedIndex].value == 1)
                                {
                                        document.form1.search_country.selectedIndex = 1;
                                }
                                else
                                {
                                        if (document.form1.search_area[document.form1.search_area.selectedIndex].value == 2)
                                        {
                                                document.form1.search_country.selectedIndex = 1;
                                        }
                                        else
                                        {
                                                document.form1.search_country.selectedIndex = 0;
                                        }
                                }
	
				index4 = document.form1.search_country[document.form1.search_country.selectedIndex].value;
				prov = prov_array[index4];
				if (document.form1.search_country[document.form1.search_country.selectedIndex].value == "")
                		{
                		        document.form1.search_provstate.options.length = 1;
                		        document.form1.search_provstate.options[0] = new Option("Choose a Country", "");
                		        document.form1.search_provstate.selectedIndex = 0;
                		        document.form1.search_region.options.length = 1;
                		        document.form1.search_region.options[0] = new Option("Choose a Country", "");
                		        document.form1.search_region.selectedIndex = 0;
                		}
                		else
                		{
					if (prov.length == 1 && prov[0] == "NULL")
					{
						document.form1.search_provstate.options.length = 1;
						document.form1.search_provstate.options[0] = new Option("N/A", "");
						document.form1.search_provstate.selectedIndex = 0;
						document.form1.search_region.options.length = 1;
						document.form1.search_region.options[0] = new Option("N/A", "");
						document.form1.search_region.selectedIndex = 0;
					}
					else
					{
						document.form1.search_provstate.options.length = 0;
						document.form1.search_provstate.length = prov.length;
						for ($i = 0; $i < prov.length; $i++)
						{
							if (prov[$i] == "NULL")
							{
								document.form1.search_provstate.options[$i] = new Option(" ", "");
							}
							else
							{
								document.form1.search_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
							}
						}
						document.form1.search_provstate.selectedIndex = 0;
		
						index5 = document.form1.search_provstate[document.form1.search_provstate.selectedIndex].value;
						region = region_array[index5];
						if (document.form1.search_provstate[document.form1.search_provstate.selectedIndex].value == "")
						{
							document.form1.search_region.options.length = 1;
							document.form1.search_region.options[0] = new Option("Choose a Province/State", "");
							document.form1.search_region.selectedIndex = 0;
						}
						else
						{
							if (region.length == 1 && region[0] == "NULL")
							{
								document.form1.search_region.options.length = 1;
								document.form1.search_region.options[0] = new Option("N/A", "");
								document.form1.search_region.selectedIndex = 0;
							}
							else
							{
								document.form1.search_region.options.length = 0;
								document.form1.search_region.options.length = region.length;
								for ($i = 0; $i < region.length; $i++)
								{
									if (region[$i] == "NULL")
									{
										document.form1.search_region.options[$i] = new Option(" ", "");
									}
									else
									{
										document.form1.search_region.options[$i] = new Option(region[$i][1], region[$i][0]);
									}
								}
								document.form1.search_region.options.selectedIndex = 0;
							}
						}	
					}	
				}	
			}	
		}
	}	
	
        function update_prov(){
                index = document.form1.search_country[document.form1.search_country.selectedIndex].value;
                prov = prov_array[index];
		if (document.form1.search_country[document.form1.search_country.selectedIndex].value == "")
		{
			if (document.form1.search_area[document.form1.search_area.selectedIndex].value == "")
			{
				message = "Choose an Area";
			}
			else
			{
				message = "Choose a Country";
			}
			document.form1.search_provstate.options.length = 1;
			document.form1.search_provstate.options[0] = new Option(message, "");
			document.form1.search_provstate.selectedIndex = 0;
			document.form1.search_region.options.length = 1;
			document.form1.search_region.options[0] = new Option(message, "");
			document.form1.search_region.selectedIndex = 0;
		}
		else
		{
	                if (prov.length==1 && prov[0] == "NULL"){
	                        document.form1.search_provstate.options.length = 1;
	                        document.form1.search_provstate.options[0] = new Option("N/A", "");
	                        document.form1.search_provstate.selectedIndex = 0;
	                        document.form1.search_region.options.length=1;
	                        document.form1.search_region.options[0] = new Option("N/A", "");
	                        document.form1.search_region.selectedIndex = 0;
	
	                }else{
	
	                        document.form1.search_provstate.options.length =0;
	                        document.form1.search_provstate.options.length = prov.length;
	                        for($i=0;$i<prov.length;$i++){
					if (prov[$i] == "NULL")
					{
						document.form1.search_provstate.options[$i] = new Option(" ", "");
					}
					else
					{
						document.form1.search_provstate.options[$i] = new Option(prov[$i][1], prov[$i][0]);
					}
	                        }
	                        document.form1.search_provstate.selectedIndex = 0;
	
	                        index2=document.form1.search_provstate[document.form1.search_provstate.selectedIndex].value;
	                        region = region_array[index2];
				if (document.form1.search_provstate[document.form1.search_provstate.selectedIndex].value == "")
				{
					if (document.form1.search_country[document.form1.search_country.selectedIndex].value == "")
					{
						if (document.form1.search_area[document.form1.search_area.selectedIndex].value == "")
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
					document.form1.search_region.options.length = 1;
					document.form1.search_region.options[0] = new Option(message, "");
					document.form1.search_region.selectedIndex = 0;
				}
				else
				{
	                        	if (region.length==1 && region[0] == "NULL"){
	                               		document.form1.search_region.options.length=1;
	                               		document.form1.search_region.options[0] = new Option("N/A", "");
	                        	}else{
	                        	        document.form1.search_region.options.length=0;
	                        	        document.form1.search_region.options.length=region.length;
	                        	        for($i=0;$i<region.length;$i++){
	                        	                document.form1.search_region.options[$i] = new Option(region[$i][1], region[$i][0]);
	                        	        }
	                        	}
				}
	        	}
		}
	}

        function update_region(){

                document.form1.search_region.options.length=0;
                index2=document.form1.search_provstate[document.form1.search_provstate.selectedIndex].value;
                region = region_array[index2];
		if (document.form1.search_provstate[document.form1.search_provstate.selectedIndex].value == "")
		{
			if (document.form1.search_country[document.form1.search_country.selectedIndex].value == "")
			{
				if (document.form1.search_area[document.form1.search_area.selectedIndex].value == "")
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
		
			document.form1.search_region.options.length = 1;
			document.form1.search_region.options[0] = new Option(message, "");
			document.form1.search_region.selectedIndex = 0;
		}
		else
		{       
                	if (region.length==1 && region[0] == "NULL"){
                	        document.form1.search_region.options.length=1;
                	        document.form1.search_region.options[0] = new Option("N/A", "");
                	}else{
                	        document.form1.search_region.options.length = region.length;
                	        for($i=0;$i<region.length;$i++){
					if (region[$i] == "NULL")
					{
						document.form1.search_region.options[$i] = new Option(" ", "");
					}
					else
					{
                	                	document.form1.search_region.options[$i] = new Option(region[$i][1], region[$i][0]);
					}
                	        }
                	}
                	document.form1.search_region.selectedIndex = 0;
        	}
	}
//-->
</script>

<form method="post" name='form1' action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="search_job" />
   
<center>
<table border="0" cellpadding="5" cellspacing="0" width="100%" class="row1">

<tr>
	<td align="center" colspan='2'><b class='black'><br />
	The jobs will be searched based on the criteria entered below.  Leave<br />any fields blank that you do not wish to be used in the search.</b>
	</td>
</tr>

<tr>
        <td colspan='2'><hr width='50%' size='1' /></td>
</tr>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<tr>
        <td align='center' colspan='2'>
        <table class='row2'>
	<tr>
		<td colspan='2' align='center'>
		Perform this search on jobs that:&nbsp;&nbsp;
		<select name='search_on_jobs'>
		<?php
		/*
		gone for now
		echo("<option value='preferences' ");
		if ($search_on_jobs == 'preferences')
		{
			echo("selected='selected'");
		}
		echo(">match your preferences</option>");
		*/

		echo("<option value='flagged' ");
		if ($search_on_jobs == 'flagged')
		{
			echo("selected='selected'");
		}
		echo(">you have flagged</option>");

		echo("<option value='applied' ");
		if ($search_on_jobs == 'applied')
		{
			echo("selected='selected'");
		}
		echo(">you have applied for</option>");
		
        $sql = ("
            SELECT DISTINCT d.department_id, d.department_name
            FROM student_department AS sd
            INNER JOIN department AS d
                ON d.department_id=sd.department_id
            WHERE sd.student_number='" . $student_number . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["department_id"] . "' ");
            echo((!$search_on_jobs || $search_on_jobs == $row["department_id"]) ? "selected='selected'" : "");
            echo(">are in " . $row["department_name"] . "</option>");
        }

		echo("<option value='uvic' ");
		if ($search_on_jobs == 'uvic')
		{
			echo("selected='selected'");
		}
		echo(">are registered at UVic</option>");
		?>
		</select>
		</td>
	   </tr>
<?php
    echo("<tr>");
    echo("<td colspan='2' align='center'>Show Jobs that are in &nbsp;");
    echo("<select name='folder_id'>");
    echo("<option value='0'>Any</option>\n");
    $sql = ("
            SELECT folder_id
            FROM student_job_folder
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        echo("<option value='".$row["folder_id"]."'>".getFolderName($row["folder_id"],$student_number)."</option>\n");

    }
    echo("</select>&nbsp;Folder");
    echo("</td>");
    echo("</tr>");
?>	

	   <tr>
		<td colspan='2' align='center'>
		Show jobs for:&nbsp;&nbsp;
		<select name='search_show_term'>
		<?php

		echo("<option value='" . getCurrentTermID() . "' ");
		if ($search_show_term == getCurrentTermID())
		{
			echo("selected='selected'");
		}
		echo(">" . getTermName(getCurrentTermID()) . "</option>");

		echo("<option value='" . $nextTermYear["term"] . "' ");
		if (!$search_show_term || $search_show_term == $nextTermYear["term"])
		{
			echo("selected='selected'");
		}
		echo(">" . getTermName($nextTermYear["term"]) . "</option>");

		?>
		</select>
	   	&nbsp;&nbsp;&nbsp;
		<select name='search_show_year'>
		<?php
		if ($cur_year != $nextTermYear["year"])
		{
			echo("<option value='" . $cur_year . "' ");
			if ($search_show_year == $cur_year)
			{
				echo("selected='selected'");
			}
            echo(">" . $nextTermYear["year"] . "</option>");
			echo("<option value='" . $nextTermYear["year"] . "' ");
			if (!$search_show_year || $search_show_year == $nextTermYear["year"])
			{
				echo("selected='selected'");
			}
            echo(">" . $cur_year . "</option>");
        }
		else
		{
			echo("<option value='" . $cur_year. "' selected='selected'>" . $cur_year . "</option>");
		}

		?>
		</select>
        </td>
	   </tr>
	   <tr>
		<td colspan='2' align='center'>
		Show jobs that are for future terms?
		<select name='search_future_jobs'>
		<?php
		echo("<option value='no' ");
		if ($search_future_jobs == 'no' || !$search_future_jobs)
		{
			echo("selected='selected'");
		}
		echo(">No</option>");
		echo("<option value='yes' ");
		if ($search_future_jobs == 'yes')
		{
			echo("selected='selected'");
		}
		echo(">Yes</option>");
		?>
		</select>
        </td>
        </tr>
        </table>
        </td>
</tr>
<tr>
	<td colspan='2'>&nbsp;</td>
</tr>
<tr>
	<td colspan='2' align='center'>Show jobs with status:</td>
</tr>
<tr>
        <td align='center' colspan='2'>
        <table class='row2'>
	<?php

		$sql = ("
			SELECT DISTINCT *
			FROM student_status
			ORDER BY status_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$newrow = 0;
		while ($row = $result->FetchRow())
		{
			if (!($newrow % 2))
			{
				echo("<tr>");
			}
			echo("<td><input type='checkbox' name='search_status[]' value='" . $row["status_id"] . "' ");
			if ($search_status && in_array($row["status_id"], $search_status))
			{
				echo("checked='checked'");
			}
			echo(" />" . $row["description"] . "</td>");
			if ($newrow % 2)
			{
				echo("</tr>");
			}
			$newrow++;
		}
        /*
		if ($newrow % 2)
		{
			echo("</tr>");
		}
        */
		?>
	</tr>
	</table>
	</td>
</tr>

<tr><td colspan='2'><br /></td></tr>

<!-- IGNORE THIS CODE FOR NOW (TIMESTAMP SEARCHING).

<tr>
        <td colspan='2' align='center'>Show jobs that:</td>
</tr>
<tr>
        <td align='center' colspan='2'>
        <table cellpadding='3' cellspacing='3' class='row2'>
        <tr>
        <td class='row2'>
        <table width='100%' class='row2'>
        <?php
/*

	echo("<tr>");
		echo("<td>");
		echo("Have been: ");
		echo("<select name='search_add_updated'>");
		echo("<option value='added' ");
		if ($search_add_updated == 'added')
		{
			echo("selected='selected'");
		}
		echo(">Added</option>");
		
		echo("<option value='updated' ");
		if ($search_add_updated == 'updated')
		{
			echo("selected='selected'");
		}
		echo(">Updated</option>");
	
		echo("<option value='both' ");
		if (!$search_add_updated || $search_add_updated == 'both')
		{
			echo("selected='selected'");
		}
		echo(">Added or Updated</option>");

		echo("</select>");

		echo("&nbsp;&nbsp;&nbsp;&nbsp;");
		
		echo("<input type='radio' name='search_add_update_how' value='before' ");
		if ($search_add_updated_how == 'before')
		{
			echo("checked='checked'");
		}
		echo(" />Before&nbsp;&nbsp;&nbsp;");
		
		echo("<input type='radio' name='search_add_update_how' value='after' ");
		if (!$search_add_updated_how || $search_add_updated_how == 'after')
                {
			echo("checked='checked'");
		}
		echo(" />After");
		echo("</td>");
	echo("</tr>");

	$sql = ("
		SELECT DISTINCT view_job_last, view_job_last_time
		FROM student
		WHERE student_number='" . $student_number . "'
		");
	echo("<tr>");
		echo("<td>");
		echo("Date (YYYY-MM-DD): ");
		echo("<input type='text' name='search_add_update_date' value='" . $search_add_update_date . "' size='12' maxlength='12' />");
		echo("&nbsp;" . popup('search_add_update_date', 'form1'));
		echo("&nbsp;");
		echo("Time:&nbsp;");
		
		echo("<select name='search_add_update_time'>");

		echo("<option value='' ");
		if (!$search_add_update_time)
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</option>");
	
		for ($i = 7; $i < 12; $i++)
		{
			echo("<option value='" . $i . ":00' ");
			if ($search_add_update_time == ($i . ":00"))
			{
				echo("selected='selected'");
			}
			echo(">" . $i . ":00AM</option>");
		}

		echo("<option value='12:00' ");
		if ($

		for ($i = 
		
		for ($i = 
		echo("</td>");
	echo("</tr>");

        echo("</table>");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</td>");
echo("</tr>");
*/

?>

-->

<tr>
	<td align='center' colspan='2'>Jobs located in:
	</td>
</tr>
<tr>
        <td align='center' colspan='2'>
        <table cellpadding='3' cellspacing='3'>
        <tr>
        <td class='row2'>
        <table width='100%' class='row2'>
	   <tr>
		<td align='left'>Area:
		</td>
		<td>
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
		<td align='left'>Country:
		</td>
		<td>
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
		<td align='left'>Province/State:
		</td>
		<td>
		<select name='search_provstate' onchange='update_region()'>
		<?php
	
		/*
	         More tricky nonsense here.  The same situation as above, except that now we have to take into account
	         the situations of an Area being selected but no Country, or no Area AND no Country.  Ugh.
	        */
	
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
		<td align='left'>Region:
		</td>
		<td>
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
	                echo("<option value=''>Choose a Province/State</option>");
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
	<td align='right' width='50%'>Job codes starting with:
	</td>
	<td align='left'>
	<select name='search_job_codes'>
		<?php
		echo("<option value='' ");
		if (!$search_job_codes || $search_job_codes == '')
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</option>");
		for ($i = A; $i < Z; $i++)
		{
			echo("<option value='$i' ");
			if ($search_job_codes && $search_job_codes == $i)
			{
				echo("selected='selected'");
			}
			echo(">".$i."</option>");
		}
		echo("<option value='$i' ");
		if ($search_job_codes && $search_job_codes == $i)
		{
			echo("selected='selected'");
		}
		echo(">".$i."</option>");
		?>
	</select>
	&nbsp;or&nbsp;
	<input type="text" name="search_job_codes_m" size="12" maxlength="12"
	<?php
	if ($search_job_codes_m && $search_job_codes_m != '')
	{
		echo(" value='" . addslashes(trim($search_job_codes_m)) . "'");
	}
	echo(" />");
	?>
	</td>
</tr>

<tr>
	<td align='right' width='50%'>Company names starting with:
	</td>
	<td align='left'>
	<select name='search_company_name'>
		<?php
		echo("<option value='' ");
		if (!$search_company_name || $search_company_name == '')
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</option>");
		for ($i = A; $i < Z; $i++)
		{
			echo("<option value='$i' ");
			if ($search_company_name && $search_company_name == $i)
			{
				echo("selected='selected'");
			}
			echo(">".$i."</option>");
		}
		echo("<option value='$i' ");
		if ($search_company_name && $search_company_name == $i)
		{
			echo("selected='selected'");
		}
		echo(">".$i."</option>");
		?>
	</select>
	&nbsp;or&nbsp;
	<input type='text' name='search_company_name_m' size='10' maxlength='10'
	<?php
	if ($search_company_name_m && $search_company_name_m != '')
	{
		echo(" value='" . addslashes(trim($search_company_name_m)) . "'");
	}
	echo(" />");
	?>
	</td>
</tr>


<tr>
    <td align='right' width='50%' nowrap='nowrap'>Applying Method:</td>
    <td align='left' nowrap='nowrap'>
        <input type='checkbox' name='apply_method[]' value='<?php echo(JOB_APP_METHOD_ONLINE); ?>' />Electronic
        &nbsp;&nbsp;<input type='checkbox' name='apply_method[]' value='<?php echo(JOB_APP_METHOD_PAPER); ?>' />Paper
        &nbsp;&nbsp;<input type='checkbox' name='apply_method[]' value='<?php echo(JOB_APP_METHOD_SPECIAL); ?>' />Special
    </td>
</tr>

<tr>
	<td align='right' width='50%'>Closing date (YYYY-MM-DD):</td>
	<td nowrap='nowrap'>
	On
	<input type='radio' class='row2' name='search_close_date_as' value='on'<?php
	if ($search_close_date_as == 'on')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;Before
	<input type='radio' class='row2' name='search_close_date_as' value='before'<?php
	if ($search_close_date_as == 'before')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;After
	<input type='radio' class='row2' name='search_close_date_as' value='after'
	<?php
	if ($search_close_date_as == 'after')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
 	&nbsp;Date&nbsp;
	<input type='text' name='search_close_date' size='12' maxlength='12'
	<?php
	if ($search_close_date && $search_close_date != '')
	{
		echo(" value='" . addslashes(trim($search_close_date)) . "'");
	}
	echo(" />");
	echo("&nbsp;");
	echo(popup('search_close_date', 'form1'));
	?>
	</td>
</tr>

<?php

/*
 For now, don't let students search on the workterm for the job.  If they want, they can see future jobs in the
 search results, but we'll leave it at that.


<tr>
	<td colspan='2'>
	<table border='0' cellpadding='0' cellspacing='0' width='100%'>
	<tr>
	<td>Workterm starts:</td>
	<td align='right' nowrap='nowrap'>
	On
	<input type='radio' class='row2' name='search_start_as' value='on'<?php
	if ($search_start_as == 'on')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;Before
	<input type='radio' class='row2' name='search_start_as' value='before'<?php
	if ($search_start_as == 'before')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;After
	<input type='radio' class='row2' name='search_start_as' value='after'<?php
	if ($search_start_as == 'after')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;
	<select name='search_start_term'>
		<option value='' 
		<?php
		if (!$search_start_term || $search_start_term == '')
		{
			echo(" selected='selected'");
		}
		echo(">&nbsp;</option>");
		$sql = ("
			SELECT DISTINCT term_name, term_id
			FROM term
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while($row = $result->FetchRow())
		{
			echo("<option value='" . $row["term_id"] . "'");
			if ($search_start_term == $row["term_id"])
			{
				echo(" selected='selected'");
			}
			echo(">" . $row["term_name"] . "</option>");
		}
		?>
	</select>
	&nbsp;
	<select name='search_start_year'>
		<option value=''
		<?php
		if (!$search_start_year || $search_start_year == '')
		{
			echo(" selected='selected'");
		}
		echo(">&nbsp;</option>");
		$tempdate = explode("-", $current_date);
		$limit = $tempdate[0] + 2;
		for ($i = $tempdate[0]; $i < $limit; $i++)
		{
			echo("<option value='" . $i . "'");
			if ($search_start_year == $i)
			{
				echo(" selected='selected'");
			}
			echo(">".$i."</option>");
		}
		?>
	</select>
	</td>
	</tr>
	</table>
	</td>
</tr>

*/

?>


<tr>
	<td align='right' width='50%' nowrap='nowrap'>Minimum academic years completed:</td>
	<td align='left' nowrap='nowrap'>
	Equal to
	<input type='radio' class='row2' name='search_academic_as' value='equal'
	<?php
	if ($search_academic_as == 'equal')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;Less than
	<input type='radio' class='row2' name='search_academic_as' value='lesser'
	<?php
	if ($search_academic_as == 'lesser')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;Greater than
	<input type='radio' class='row2' name='search_academic_as' value='greater'
	<?php
	if ($search_academic_as == 'greater')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
	&nbsp;
	<select name='search_min_academic'>
	<?php
	echo("<option value=''");
	if (!$search_min_academic || $search_min_academic == '')
	{
		echo(" selected='selected'");
	}
	echo(">&nbsp;</option>");
	for ($i = 1; $i < 5; $i++)
	{
		echo("<option value='" . $i . "'");
		if ($search_min_academic == $i)
		{
			echo(" selected='selected'");
		}
		echo(">" . $i);
		echo(($i == 4) ? "+" : "");
		echo("</option>");
	}
	?>
	</select>
	</td>
	</tr>

    
	<tr>
	<td align='right' width='50%'>Minimum number of workterms completed:</td>
	<td align='left'>
	Equal to
        <input type='radio' class='row2' name='search_workterms_as' value='equal'
	<?php
	if ($search_workterms_as == 'equal')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
        &nbsp;Less than
        <input type='radio' class='row2' name='search_workterms_as' value='lesser'
	<?php
	if ($search_workterms_as == 'lesser')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
        &nbsp;Greater than
        <input type='radio' class='row2' name='search_workterms_as' value='greater'
	<?php
	if ($search_workterms_as == 'greater')
	{
		echo(" checked='checked'");
	}
	echo(" />");
	?>
        &nbsp;
	<select name='search_min_workterms'>
	<?php
	echo("<option value=''");
	if (!$search_min_workterms || $search_min_workterms == '')
	{
		echo(" selected='selected'");
	}
	echo(">&nbsp;</option>");
	for ($i = 0; $i < 6; $i++)
	{
		echo("<option value='" . $i . "'");
		if ($search_min_workterms == $i && $search_min_workterms != '')
		{
			echo(" selected='selected'");
		}
		echo(">" . $i);
		echo(($i == 5) ? "+" : "");
		echo("</option>");
	}
	?>
	</select>
	</td>
</tr>

<tr>
	<td colspan='2'>&nbsp;</td>
</tr>

<?php

$student_departments = getStudentDepartments($student_number, true);
$student_disciplines = getDisciplinesForDepts($student_departments);

if ($student_disciplines)
{
    echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("Disciplines:");
        echo("</td>");
	echo("</tr>");

    echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table align='center' cellpadding='3' cellspacing='3'>");
        echo("<tr align='center'>");
            echo("<td align='center'>");
            echo("<table width='100%' class='row2' cellpadding='3' >");
            
            foreach ($student_departments AS $sd)
            {
                if (isset($student_disciplines[$sd]))
                {
                    echo("<tr>");
                        echo("<td colspan='2' align='center'>");
                        echo("Disciplines belonging to <b class='black'>" . getDepartmentName($sd) . "</b>");
                        echo("</td>");
                    echo("</tr>");
        
                    $newrow = 0;
                
                    foreach ($student_disciplines[$sd] AS $sdisc)
                    {
                        if (!($newrow % 2))
                        {
                            echo("<tr>");
                        }

                        echo("<td><input type='checkbox' name='search_disciplines[]' value='" . $sdisc["id"] . "' />");
                        echo($sdisc["name"]);
                        echo("</td>");

                        if ($newrow % 2)
                        {
                            echo("</tr>");
                        }
                        $newrow++;
                    }

                    if ($newrow % 2)
                    {
                        echo("<td>&nbsp;</td>");
                        echo("</tr>");
                    }

                    echo("<tr>");
                        echo("<td colspan='2' align='center'>&nbsp;</td>");
                    echo("</tr>");
                }
            }

            echo("</table>");
            echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</td>");
	echo("</tr>");
}

$sql = ("
    SELECT DISTINCT industry_name, industry_id
    FROM industries 
    WHERE industry_name!='?'
    ORDER BY industry_name
    ");             
$result = $GLOBALS['dbh']->Execute($sql);
                        
?>                           
                        
<tr>                 
        <td colspan='2' align='center'>Industries:</td>
</tr>           
<tr>         
        <td colspan='2'>
        <table align='center' cellpadding='3' cellspacing='3'>
        <tr align='center'>
        <td align='center' class='row2'>
        <table width='100%' class='row2'>
        <tr>    
                <td colspan='6' align='center'>
                Click the <b class='black'>Yes</b> box for any industries that you would like jobs returned to be in.<br />
                Click the <b class='black'>No</b> box for any industries that you would NOT like jobs returned to be in.<br />
                Leave both boxes blank if you don't care about an industry.
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
/*
 Temporarily disabled - poor implementation was causing huge server problems.
<tr>
	<td colspan='2'>
	<table width='100%' border='0'>
	<tr>
	<td colspan='2' align='center'>Search for the following keywords (separate multiple keywords with a comma):</td>
	</tr>
	<tr>
	<td colspan='2' align='center'><input type='text' name='search_keywords' size='70' maxlength='100' value='<?php 
	echo(($search_keywords) ? $search_keywords : ""); ?>' />
	</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td colspan='2'>
	<table width='100%' border='0'>
	<tr>
	<td colspan='2' align='center'>Search for jobs without the following keywords (separate words with a comma):</td>
	</tr>
	<tr>
	<td colspan='2' align='center'><input type='text' name='search_not_keywords' size='70' maxlength='100' value='<?php
	echo(($search_not_keywords) ? $search_not_keywords: ""); ?>' />
	</td>
	</tr>
	</table>
	</td>
</tr>
*/
?>

<!--

 For now, leave this part out.  It's better just to show students the jobs ordered on job code, and then they can go from there.
	
<tr>
	<td colspan='2'>
		<table width='96%' cellpadding='2' cellspacing='0' border='0'>
		<tr>
		<td colspan='3'>How would you like your search sorted?
		</td>
		</tr>
		<tr>
		<td nowrap='nowrap'>
		1st:
		<select name='search_order[]'>
			<option value=''
			<?php
			if (!$search_order || $search_order[0] == '')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			&nbsp;</option>
			<option value='a.closing_date'
			<?php
			if ($search_order[0] == 'a.closing_date')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Closing Date</option>
			<option value='b.company_name'
			<?php
			if ($search_order && $search_order[0] == 'b.company_name')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Company</option>
			<option value='a.job_code'
			<?php
			if ($search_order && $search_order[0] == 'a.job_code')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Job Codes</option>
			<option value='a.student_status'
			<?php
			if ($search_order && $search_order[0] == 'a.student_status')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Status</option>
			<option value='a.min_academic'
			<?php
			if ($search_order && $search_order[0] == 'a.min_academic')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Academic Yr</option>
			<option value='a.min_workterms'
			<?php
			if ($search_order && $search_order[0] == 'a.min_workterms')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Workterms</option>
		</select>
		</td>
		<td nowrap='nowrap'>
		2nd:
		<select name='search_order[]'>
			<option value=''
			<?php
			if (!$search_order || $search_order[1] == '')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			&nbsp;</option>
			<option value='a.closing_date'
			<?php
			if ($search_order[1] == 'b.closing_date')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Closing Date</option>
			<option value='b.company_name'
			<?php
			if ($search_order[1] == 'b.company_name')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Company</option>
			<option value='a.job_code'
			<?php
			if ($search_order[1] == 'a.job_code')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Job Codes</option>
			<option value='a.student_status'
			<?php
			if ($search_order[1] == 'a.student_status')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Status</option>
			<option value='a.min_academic'
                        <?php
                        if ($search_order[1] == 'a.min_academic')
                        {
                                echo(" selected='selected'");
                        }
                        echo(">");
                        ?>
                        Academic Yr</option>
                        <option value='a.min_workterms'
                        <?php
                        if ($search_order[1] == 'a.min_workterms')
                        {
                                echo(" selected='selected'");
                        }
                        echo(">");
                        ?>
                        Workterms</option>
		</select>
		</td>
		<td nowrap='nowrap'>
		3rd:
		<select name='search_order[]'>
			<option value=''
			<?php
			if (!$search_order || $search_order[2] == '')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			&nbsp;</option>
			<option value='a.closing_date'
			<?php
			if ($search_order[2] == 'a.closing_date')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Closing Date</option>
			<option value='b.company_name'
			<?php
			if ($search_order[2] == 'b.company_name')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Company</option>
			<option value='a.job_code'
			<?php
			if ($search_order[2] == 'a.job_code')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Job Codes</option>
			<option value='a.student_status'
			<?php
			if ($search_order[2] == 'a.student_status')
			{
				echo(" selected='selected'");
			}
			echo(">");
			?>
			Status</option>
			<option value='a.min_academic'
                        <?php
                        if ($search_order[2] == 'a.min_academic')
                        {
                                echo(" selected='selected'");
                        }
                        echo(">");
                        ?>
                        Academic Yr</option>
                        <option value='a.min_workterms'
                        <?php
                        if ($search_order[2] == 'a.min_workterms')
                        {
                                echo(" selected='selected'");
                        }
                        echo(">");
                        ?>
                        Workterms</option>
		</select>
		</td>
		</tr>
		</table>
	</td>
	</tr>
-->

  <tr>
	<td align='center' colspan='2'><hr />
	<input type='submit' name='continue' value='View search results' />&nbsp;&nbsp;
	<input type='reset' value='Reset Form' />
	</td>
  </tr>
		

</table>
</center>
</form>

 
