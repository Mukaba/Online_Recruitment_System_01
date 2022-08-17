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
 | Filename: oldCompany_newContact.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is used to give the user the choice of how he would   |
 | like to use the given company information to update our existing records.    |
 +------------------------------------------------------------------------------+

*/

$max_dept_id = 0;
$sql = ("
        SELECT DISTINCT department_id, employer_id
	FROM employer_department
        ORDER BY department_id
        ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
	$str = $row["employer_id"];

        $parent_company_array[$row["department_id"]] = $str;

        if ($row["department_id"] > $max_dept_id)
        {
                $max_dept_id = $row["department_id"];
        }
}

$str = "Array(";
for ($i = 0; $i <= $max_dept_id; $i++)
{
	$str .= "\"" . $parent_company_array[$i] . "\",";
}

$str = substr($str, 0, (strlen($str) - 1)) . ")";
$parent_company_array = $str;

?>

<script type='text/javascript' language='javascript'>
<!--javascript

var parent_company_array = new <?php echo($parent_company_array); ?>;

function uncheck_depts(employer_id)
{
        for (var i = 0; i < document.form1.elements.length; i++)
        {
                var e = document.form1.elements[i];
		if (e.name == 'matching_department_id')
		{
			e.checked = '';
		}
        }
	for (var i = 0; i < document.form1.elements.length; i++)
	{
		var e = document.form1.elements[i];
		if ((e.name == 'matching_employer_id') && (e.value == employer_id))
		{
			document.form1.elements[i + 1].checked = 'checked';
		}
	}
}

function check_parent_company(employer_id)
{
	for (var i = 0; i < document.form1.elements.length; i++)
	{
		var e = document.form1.elements[i];
		if (e.name == 'matching_employer_id' && e.value == employer_id)
		{
			e.checked = 'checked';
		}
		else
		{
			if (e.name != 'matching_department_id')
			{
				e.checked = '';
			}
		}
	}
}
		
function popUpCompany(slotName, slotName2, formName)
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

	var j = eval(str);
	for (var i = 0; i < j.length; i++)
	{
		var e = j[i];
		if (e.name == slotName2)
		{
			var slot2 = i;
			break;
		}
	}

	window.open("mamook.php?select=company_department_choose_3&no_headers=1&company_slot="+slot+"&id_slot="+slot2+"&parentFormName="+formName, "CompanyChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes,width=285,height=175");
}

//-->
</script>

<?php

// Pull out pertinent information from database.

$pendingEmployer = new PendingEmployer('');
$pendingEmployer->load_pending_values($job_id);

$checkCompanyName = $pendingEmployer->company_name[0] . $pendingEmployer->company_name[1] . $pendingEmployer->company_name[2];

notify("Please select the company (and division if applicable) that matches the company you are entering
	(<b>" . $pendingEmployer->company_name . "</b>).  Divisions will appear indented underneath their parent
	companies.  Then choose one of the options below and click Continue.  Should you choose to do so, the 
	division or company that you have selected will be updated with the new information.<br /><br />You may click 
	the names of the companies and divisions to have a new window open with that company or division's details displayed.");

$sql = ("
        SELECT DISTINCT employer_id, company_name
        FROM employer_company
        WHERE company_name LIKE '" . addslashes($checkCompanyName) . "%'
        AND company_display
        ORDER BY company_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<h3>Similar Companies:</h3>");

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='continue' value='method_for_adding' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo("<input type='hidden' name='department' value='" . $department . "' />");

echo("<table cellspacing='0' cellpadding='0' border='1'>");
echo("<tr>");
echo("<td>");

echo("<table border='0' cellpadding='2'>");

echo("<tr>");
	echo("<td class='rowgrey' align='center'>&nbsp;</td>");
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name&nbsp;</b></td>");
	echo("<td class='rowgrey' align='center'>&nbsp;</td>");
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Division Name&nbsp;</b></td>");
echo("</tr>");

$rowclass = 0;

$company_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_details&amp;no_buttons=true&amp;employer_id=";
$department_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_department&amp;no_buttons=true&amp;department_id=";

while ($row = $result->FetchRow())
{
	echo("<tr>");
		echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		echo("<input type='radio' name='matching_employer_id' onclick='uncheck_depts(" . $row["employer_id"] . ")' value='" . $row["employer_id"] . "' />");
		echo("</td>");

		echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		echo("<a class='blue' target='company_popup' href='" . $company_href . $row["employer_id"] . "'>" . $row["company_name"] . "</a>");
		echo("</td>");

	$sql = ("
		SELECT DISTINCT department_id, department_name
		FROM employer_department
		WHERE employer_id='" . $row["employer_id"] . "'
        ORDER BY department_name
		");
	$dept_result = $GLOBALS['dbh']->Execute($sql);

	if ($dept_result->RecordCount())
	{
		echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
                echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . ")' value='' />");
                echo("</td>");
	
		echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		echo("None");
		echo("</td>");
	
        	echo("</tr>");

		while ($dept_row = $dept_result->FetchRow())
		{
			echo("<tr>");
			
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;");
			echo("</td>");
			
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;");
			echo("</td>");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . ")' value='" . $dept_row["department_id"] . "' />");
			echo("</td>");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<a class='blue' target='department_popup' href='" . $department_href . $dept_row["department_id"] . "'>" . $dept_row["department_name"] . "</a>");
			echo("</td>");

			echo("</tr>");
		}
	}
	else
	{
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . ")' value='' />");
			echo("</td>");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("N/A");
			echo("</td>");
		echo("</tr>");
	}

	$rowclass++;
}

echo("</table>");

echo("</td>");
echo("</tr>");
echo("</table>");

echo("<br />");
echo("<h3 align='center'>-Or-</h3>");
echo("<br />");

echo("<center>");
echo("<table border='0' cellpadding='9' cellspacing='0' class='row1'>");

echo("<tr>");
    echo("<td align='center' colspan='2'><b class='black'>Choose from a list of all companies and divisions</b></td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("Company/Division:&nbsp;");
    echo("<input type='text' size='45' name='company_div_name' READONLY />");
    echo("<input type='hidden' name='comp_div_id' />");
    echo("&nbsp;<input type='button' name='company_list' value='Choose from a list' onclick='popUpCompany(\"company_div_name\", \"comp_div_id\", \"form1\");' />");
    echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");

echo("<br />");

echo("<table border='0'>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='updateCompany_addContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Update the chosen company with the new information and add the contact to the chosen company/division.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='updateDepartment_addContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Update the chosen division with the new information and add the contact to the chosen company/division.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td>");
    echo("<input type='radio' name='choice' value='updateDepComp_addContact' />");
    echo("</td>");
    echo("<td align='left'>");
    echo("Update the chosen company/division (<b>from the pop-up box</b>) with the new information and add the contact to the chosen company/division.");
    echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='dontUpdate_addContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Don't update anything with the new information and add the contact to the chosen company/division.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
        echo("<td>");
        echo("<input type='radio' name='choice' value='Cancel' />");
        echo("</td>");
        echo("<td align='left'>");
        echo("Cancel and return to the list of incoming jobs.");
        echo("</td>");
echo("</tr>");

echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
        echo("<td align='center' colspan='2'>");
	echo("<input type='submit' value='Continue' />");
        echo("</td>");
echo("</tr>");

echo("</table>");

echo("</form>");
