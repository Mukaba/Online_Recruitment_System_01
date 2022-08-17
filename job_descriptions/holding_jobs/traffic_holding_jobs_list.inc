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
 | Filename: traffic_holding_jobs_list.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file displays all of the jobs that are currently in the    |
 | 24 hour hold status, and allows the user to either view a job's specifics,   |
 | or apply to multiple jobs.                                                   |
 +------------------------------------------------------------------------------+

*/

?>

<script type='text/javascript' language='javascript'>
<!--javascript

function checkAll()
{
	for (var i = 0; i < document.form2.elements.length; i++)
	{
		var e = document.form2.elements[i];
		if ((e.name != 'allbox') && (e.type == 'checkbox'))
		{
			e.checked = document.form2.allbox.checked;
		}
	}
}

function CheckCheckAll()
{
	var TotalBoxes = 0;
	var TotalOn = 0;

	for (var i=0; i < document.form2.elements.length; i++)
	{
		var e = document.form2.elements[i];
		if ((e.name != 'allbox') && (e.type=='checkbox'))
		{
			TotalBoxes++;
			if (e.checked)
			{
				TotalOn++;
			}
		}
	}

	if (TotalBoxes == TotalOn)
	{
		document.form2.allbox.checked = true;
	}
	else
	{
		document.form2.allbox.checked = false;
	}
}

function updateJobs()
{
	var j = 0;

	for (var i = 0; i < document.form2.elements.length; i++)
	{
		var e = document.form2.elements[i];
		if ((e.name != 'allbox') && (e.type == 'checkbox'))
		{
			var index = document.form2.apply_this_department[document.form2.apply_this_department.selectedIndex].value;

			var job = dept_values[index];

			e.checked = job[j];

			j++;
		}
	}
}

//-->

</script>

<?php

$sql = ("
	SELECT DISTINCT ji.job_id, ji.job_code, ji.position_title, ji.hold_begin_datetime, ji.department_id, ec.employer_id, ec.company_name
	FROM job_info AS ji, employer_company AS ec
	WHERE ji.status='" . HOLDING . "'
	AND ji.employer_id=ec.employer_id
	AND ji.hold_end_datetime>NOW()
	");

$sql .= ("
        ORDER BY " . (($order == "company_name") ?  "ec.company_name, ji.hold_begin_datetime, ji.position_title"
	: (($order == "position_title") ? "ji.position_title, ji.hold_begin_datetime, ec.company_name"
	: (($order == "status") ? "ji.admin_status, ji.job_code, ec.company_name"
	: (($order == "job_code") ? "ji.job_code, ji.position_title, ji.hold_begin_datetime"
	: "ji.hold_begin_datetime, ji.position_title, ec.company_name"))))
        );
$sql_copy = $sql;

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0)
{
	// if there are no job in the database, indicate this 

	notify("There are currently no jobs on 24 hour hold.");
}
else      
{
	// There are jobs in the 24 hour holding pattern

	// Build the flags array that the javascript is going to reference.
	
	$max_job_id = 0;
	$result_copy = $GLOBALS['dbh']->Execute($sql_copy);

	while ($row_copy = $result_copy->FetchRow())
	{
		$job_ids_included[] = $row_copy["job_id"];
	}

	/*
	 We should now have an array that holds all of the job_ids that will departments will be applying
	 to, and in the order that they are going to be displayed.  Now we need to find out which departments
	 have applied to which jobs.
	*/

	$dept_sql = ("
		SELECT DISTINCT department_id
		FROM department
		WHERE job_list
		");

	$dept_result = $GLOBALS['dbh']->Execute($dept_sql);
	
	while ($dept_row = $dept_result->FetchRow())
	{
		$dept_settings[] = $dept_row["department_id"];
	}
	
	/*
	 We now have an array containing all of the contact_ids that we need to check,
	 and an array containing all of the flags that can be set.  We need to form an
	 array that holds the value of each flag for each contact id.
	*/

	$str = ("var dept_values = new Array();");

	for ($i = 0; $i < sizeof($dept_settings); $i++)
	{
		$str2 .= ("dept_values[" . $dept_settings[$i] . "] = Array(");

		for ($j = 0; $j < sizeof($job_ids_included); $j++)
		{
			$job_sql = ("
				SELECT DISTINCT job_id
				FROM department_job_join
				WHERE job_id='" . $job_ids_included[$j] . "'
				AND department_id='" . $dept_settings[$i] . "'
				");

			$job_result = $GLOBALS['dbh']->Execute($job_sql);
			
			if (!$job_result->RecordCount())
			{
				$str2 .= ("'',");
			}
			else
			{
				$str2 .= ("'checked='checked'',");
			}
		}

		// Clear out the trailing comma, and close the array call.

		$str2 = substr($str2, 0, (strlen($str2) - 1));
		$str2 .= (");");
	}

	?>
	<script type='text/javascript' language='javascript'>
	<!--javascript

	/* 
	 We now have the two strings, str and str2 that will be used to create our 
	 array holding all of the flag values for each contact id.  Let's parseroo them.
	*/

	eval("<?php echo($str); ?>");
	eval("<?php echo($str2); ?>");

	//-->
	</script>
	<?php

	echo("<h4>Jobs in 24 Hour Hold:</h4>");

	$link_href = $PHP_SELF . "&amp;select=holding_jobs&amp;order=";

	echo("<form method='post' name='form2' action='" . $PHP_SELF . "'>");
	echo("<input type='hidden' name='select' value='holding_jobs' />");

	echo("<table border='0' cellpadding='2' cellspacing='0'>");
	echo("<tr align='center'>");
		echo("<td class='row1'>");
		echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>"); 

		echo("<tr align='center'>");
		echo("<td>");

		echo("Setting Department Applicability for:&nbsp;");

		echo("<select name='apply_this_department' onchange='updateJobs()'>");

		$sql = ("
			SELECT DISTINCT department_id, department_name
			FROM department
			WHERE job_list
			ORDER BY department_name
			");
		$result2 = $GLOBALS['dbh']->Execute($sql);

		while ($row2 = $result2->FetchRow())
		{
			// If no apply_this_department yet, set one.

			if (!$apply_this_department)
			{
				$apply_this_department = $row2["department_id"];
			}
			echo("<option value='" . $row2["department_id"] . "' ");
			echo($apply_this_department == $row2["department_id"] ? "selected='selected'" : "");
			echo(">" . $row2["department_name"] . "</option>");
		}

		echo("</select>");

		echo("</td>");
	echo("</tr>");

	echo("</table>");
	echo("</td>");
	echo("</tr>");
	echo("</table>");

	echo("<br />");

        echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
		echo("<td>");
		echo("<table border='0' cellpadding='2'>");

		echo("<tr>");
			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "job_code'><b class='white'>Job Code</b></a>&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "position_title'><b class='white'>Position Title</b></a>&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "company_name'><b class='white'>Company Name</b></a>&nbsp;");
			echo("</td>");
	
			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<a class='orderable' href='" . $link_href . "time_remaining'><b class='white'>Time Remaining<br />on Hold</b></a>&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<b class='white'>Applied<br />Departments</b>&nbsp;");
			echo("</td>");

			echo("<td class='rowgrey' align='center'>");
			echo("&nbsp;<b class='white'>Host<br />Department</b>&nbsp;");
			echo("</td>");
		echo("</tr>");
        
	$rowclass = 0;
	while ($row = $result->FetchRow())
	{
		$total_job_ids[] = $row["job_id"];
        	$position_title = $row["position_title"];
		$job_id = $row["job_id"];
		$company_name = $row["company_name"];
		$employer_id = $row["employer_id"];
		$job_code = $row["job_code"];

		$host_department = $row["department_id"];

		$sql = ("
			SELECT DISTINCT job_id
			FROM department_job_join
			WHERE job_id='" . $row["job_id"] . "'
			AND department_id='" . $apply_this_department . "'
			");
		$result2 = $GLOBALS['dbh']->Execute($sql);
		$already_applied = $result2->RecordCount();

		// Determine the time remaining

		$time_remaining = getHoldTimeRemaining($row["hold_begin_datetime"]);
		$split_time = split(":", $time_remaining);
		$long_time_remaining = $split_time[0] . " hours, " . $split_time[1] . " minutes";

	        $href = $PHP_SELF . "&amp;select=holding_jobs&amp;job_id=" . urlencode($job_id) . "&amp;continue=view_specifics_of_job";
		$company_href = $PHP_SELF . "&amp;select=holding_jobs&amp;employer_id=" . urlencode($employer_id) . "&amp;continue=view_company_specifics";

		$sql = ("
			SELECT DISTINCT department_id
			FROM department_job_join
			WHERE job_id='" . $job_id . "'
			");
		$result2 = $GLOBALS['dbh']->Execute($sql);
	
		$apply_departments = '';
		while ($row2 = $result2->FetchRow())
		{
			$sql = ("
				SELECT DISTINCT department_code
				FROM department
				WHERE department_id='" . $row2["department_id"] . "'
				");
			$result3 = $GLOBALS['dbh']->Execute($sql);
			$row3 = $result3->FetchRow();
			$apply_departments .= ($row3["department_code"] . ", ");
		}

		if ($apply_departments)
		{
			$apply_departments = substr($apply_departments, 0, -2);
		}
		else
		{
			$apply_departments = "None";
		}

        	echo("<tr>");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<input type='checkbox' name='apply_to_job[]' value='" . $row["job_id"] . "' ");
			echo($already_applied ? "checked='checked'" : "");
			echo(" /></td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "'>$job_code</a>&nbsp;</td>\n");
			echo("</td>");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' href='" . $href . "'>$position_title</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d"). "'>");
			echo("&nbsp;<a class='blue' href='" . $company_href . "'>$company_name</a>&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d"). "'>");
			echo("&nbsp;$long_time_remaining&nbsp;</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;" . $apply_departments . "&nbsp;");
			echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;" . getDepartmentCode($host_department) . "&nbsp;");
			echo("</td>\n");

		echo("</tr>\n");

	        $rowclass++;
	}
	echo("</table>");
	echo("</td>");
	echo("</tr>");
	echo("</table>");

	echo("<input type='hidden' name='total_job_ids' value='" . packObject(($total_job_ids)) . "' />");

	echo("<br />");

	echo("<table border='0' cellpadding='2' cellspacing='0'>");
	echo("<tr align='center'>");
		echo("<td class='row1'>");
		echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");
		echo("<tr>");
			echo("<td>");
			echo("<input type='submit' value='Apply Selected Department to Selected Jobs' />");
			echo("<input type='hidden' name='continue' value='apply_traffic_multiple' />");
			echo("</td>\n");
		echo("</tr>\n");
		echo("</table>\n");
		echo("</td>\n");
	echo("</tr>\n");
	echo("</table>\n");

	echo("</form>\n");
}

?>
