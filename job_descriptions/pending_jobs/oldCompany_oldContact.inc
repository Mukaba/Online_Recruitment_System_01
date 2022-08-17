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
 | Filename: oldCompany_oldContact.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is used to give the user the choice of how he would   |
 | like to use the given company information to update our existing records,    |
 | and do the same for the contact.                                             |
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

function uncheck_depts()
{
        for (var i = 0; i < document.form1.elements.length; i++)
        {
                var e = document.form1.elements[i];
		if (e.name == 'matching_department_id' || e.name == 'matching_contact_id')
		{
			e.checked = '';
		}
        }
}

function uncheck_contacts()
{
	for (var i = 0; i < document.form1.elements.length; i++)
	{
		var e = document.form1.elements[i];
		if (e.name == 'matching_contact_id')
		{
			e.checked = '';
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
			if (e.name != 'matching_department_id' && e.name == 'matching_contact_id')
			{
				e.checked = '';
			}
		}
	}
}

function check_parent_department(department_id, employer_id)
{
	for (var i = 0; i < document.form1.elements.length; i++)
	{
		var e = document.form1.elements[i];
		if (e.name == 'matching_department_id' && e.value == department_id)
		{
			e.checked = 'checked';
		}
		else
		{
			if (e.name == 'matching_employer_id' && e.value == employer_id)
			{
				e.checked = 'checked';
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

	window.open("mamook.php?select=company_department_choose_3&no_headers=1&company_slot="+slot+"&id_slot="+slot2+"&withContacts=true&parentFormName="+formName, "CompanyChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes");
}

function popUpContact(slotName, slotName2, companyDivSlotName, formName)
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

    var str2 = "document." + formName + "." + companyDivSlotName + ".value";

    var compDivId = eval(str2);
    // Trim off extra white space
    compDivId = compDivId.replace(/^\s*/, "");
    compDivId = compDivId.replace(/\s*$/, "");

    if (compDivId != "")
    {
        var compOrDiv = compDivId.substr(0, 4);
        var idNumber = compDivId.substr(4);
    }
    else
    {
        var compOrDiv = "-1";
        var idNumber = "-1";
    }

	window.open("mamook.php?select=contact_choose&no_headers=1&contact_name_slot="+slot+"&id_slot="+slot2+"&compOrDiv="+compOrDiv+"&compDivId="+idNumber+"&parentFormName="+formName, "ContactChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes,width=285,height=175");
}

function clearContacts()
{
    document.form1.contact_id.value = "";
    document.form1.contact_name.value = "";
}

//-->
</script>

<?php

// Pull out pertinent information from database.

$pendingEmployer = new PendingEmployer('');
$pendingEmployer->load_pending_values($job_id);

$checkCompanyName = $pendingEmployer->company_name[0] . $pendingEmployer->company_name[1] . $pendingEmployer->company_name[2];
$checkContactLastName = $pendingEmployer->contact_last_name[0] . $pendingEmployer->contact_last_name[1] . $pendingEmployer->contact_last_name[2];

notify("Please select the company and contact (and division if applicable) that match the approved company 
	and contact pair.  (<b>" .  $pendingEmployer->contact_first_name . " " . $pendingEmployer->contact_last_name 
	. "</b> at <b>" .  $pendingEmployer->company_name . "</b>).  Divisions will appear indented underneath 
	their parent companies, and contact's underneath divisions.  Please note, only contacts and companies that are
	similar to the ones you have approved will be shown.  Then choose one of the options below and click 
	Continue.  Should you choose to do so, the division or company that you have selected will be 
	updated with the new information.<br /><br />You may click the names of the companies, divisions and contacts 
	to have a new window open with that company, division or contact's details displayed.");

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='situation' value='oldCompany_oldContact' />");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='continue' value='method_for_adding' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo("<input type='hidden' name='department' value='" . $department . "' />");

$sql = ("
        SELECT DISTINCT ec.employer_id, ec.company_name
        FROM employer_company AS ec, contact_employer AS ce, contact AS c
        WHERE ec.employer_id=ce.employer_id
        AND c.contact_id=ce.contact_id
        AND ec.company_name LIKE '" . addslashes($checkCompanyName) . "%'
        AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
        AND c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
        AND ec.company_display
        ORDER BY ec.company_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	echo("<h3>Matching Companies and Contacts:</h3>");

	echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
	echo("<td>");

	echo("<table border='0' cellpadding='2'>");

	echo("<tr>");
		echo("<td class='rowgrey' align='center'>&nbsp;</td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name&nbsp;</b></td>");
		echo("<td class='rowgrey' align='center'>&nbsp;</td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Division Name&nbsp;</b></td>");
		echo("<td class='rowgrey' align='center'>&nbsp;</td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Contact Name&nbsp;</b></td>");
	echo("</tr>");

	$rowclass = 0;

	/* 
	 We need some way to keep matching_department_ids that are marked as "none" distinct.  We do this with a count variable
	 which is decremented and holds a unique negative value for each department_id marked as "none".
	*/

	$count = -1;

	$company_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_details&amp;no_buttons=true&amp;employer_id=";
	$department_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=department&amp;continue=view_specific_department&amp;no_buttons=true&amp;department_id=";
	$contact_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;no_buttons=true&amp;contact_id=";

	while ($row = $result->FetchRow())
	{
		echo("<tr>");
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_employer_id' onclick='uncheck_depts()' value='" . $row["employer_id"] . "' />");
			echo("</td>");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<a class='blue' target='company_popup' href='" . $company_href . $row["employer_id"] . "'>" . $row["company_name"] . "</a>");
			echo("</td>");

		// If we have matches for contacts without a parent division, display them.

		$sql = ("
			SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS contact_name, c.contact_id
			FROM contact AS c, contact_employer AS ce
			WHERE c.contact_id=ce.contact_id
			AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
			AND c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
			AND ce.employer_id='" . $row["employer_id"] . "'
			AND ce.department_id='0'
            ORDER BY contact_name
			");
		$contact_result = $GLOBALS['dbh']->Execute($sql);

		if ($contact_result->RecordCount())
		{
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . "); uncheck_contacts();' value='" . $count-- . "' />");
			echo("</td>");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("None");
			echo("</td>");
			
			$contact_row = $contact_result->FetchRow();
			
			// Collect the contact_ids in a variable.
			$contact_id_list .= ("'" . $contact_row["contact_id"] . "',");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(" . ($count + 1) . ", " . $row["employer_id"] . ")' value='" . $contact_row["contact_id"] . "' />");
			echo("</td>");
			
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<a class='blue' target='contact_popup' href='" . $contact_href . $contact_row["contact_id"] . "'>" . $contact_row["contact_name"] . "</a>");
			echo("</td>");

			echo("</tr>");

			$rowclass++;
				
			while ($contact_row = $contact_result->FetchRow())
			{
				// Collect the contact_ids in a variable.
				$contact_id_list .= ("'" . $contact_row["contact_id"] . "',");

				echo("<tr>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(\"none\", " . $row["employer_id"] . ")' value='" . $contact_row["contact_id"] . "' />");
					echo("</td>");
				
					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("<a class='blue' target='contact_popup' href='" . $contact_href . $contact_row["contact_id"] . "'>" . $contact_row["contact_name"] . "</a>");
					echo("</td>");
				
				echo("</tr>");

				$rowclass++;
			}

			//Start up the next column for the contacts and divisions below this one.
			echo("<tr>");
				
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;");
			echo("</td>");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;");
			echo("</td>");
		}

		// Pull out all divisions under this company that have a matching contact under them.

		$sql = ("
			SELECT DISTINCT ed.department_id, ed.department_name
			FROM employer_department AS ed, contact_employer AS ce, contact AS c
			WHERE ed.employer_id='" . $row["employer_id"] . "'
			AND ce.department_id=ed.department_id
			AND ce.employer_id='" . $row["employer_id"] . "'
			AND ce.contact_id=c.contact_id
			AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
			AND c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
			ORDER BY ed.department_name
			");
		$department_result = $GLOBALS['dbh']->Execute($sql);

		if ($department_result->RecordCount())
		{
			// Set it up so that the next loop will start from the third column
			$department_row = $department_result->FetchRow();
			
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . "); uncheck_contacts();' value='" . $department_row["department_id"] . "' />");
			echo("</td>");
			
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<a class='blue' target='department_popup' href='" . $department_href . $department_row["department_id"] . "'>" . $department_row["department_name"] . "</a>");
			echo("</td>");

			$sql = ("
				SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS contact_name, c.contact_id
				FROM contact AS c, contact_employer AS ce
				WHERE c.contact_id=ce.contact_id
				AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
				AND c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
				AND ce.employer_id='" . $row["employer_id"] . "'
				AND ce.department_id='" . $department_row["department_id"] . "'
                ORDER BY contact_name
				");
			$contact_result = $GLOBALS['dbh']->Execute($sql);

			$contact_row = $contact_result->FetchRow();
				
			// Collect the contact_ids in a variable.
			$contact_id_list .= ("'" . $contact_row["contact_id"] . "',");

			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(" . $department_row["department_id"] . ", " . $row["employer_id"] . ")' value='" . $contact_row["contact_id"] . "' />");
			echo("</td>");
		
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<a class='blue' target='contact_popup' href='" . $contact_href . $contact_row["contact_id"] . "'>" . $contact_row["contact_name"] . "</a>");
			echo("</td>");

			echo("</tr>");
			
			$rowclass++;
			
			while ($contact_row = $contact_result->FetchRow())
			{
				// Collect the contact_ids in a variable.
				$contact_id_list .= ("'" . $contact_row["contact_id"] . "',");

				echo("<tr>");
				
					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");
				
					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("&nbsp;");
					echo("</td>");

					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(" . $department_row["department_id"]  . ", " . $row["employer_id"] . ")' value='" . $contact_row["contact_id"] . "' />");
					echo("</td>");
			
					echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
					echo("<a class='blue' target='contact_popup' href='" . $contact_href . $contact_row["contact_id"] . "'>" . $contact_row["contact_name"] . "</a>");
					echo("</td>");
		
				echo("</tr>");
		
				$rowclass++;
			}

			// Do the same thing for the remaining divisions.
			while ($department_row = $department_result->FetchRow())
			{
				echo("<tr>");
					
				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("&nbsp;");
				echo("</td>");

				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("&nbsp;");
				echo("</td>");

				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . ")' value='" . $department_row["department_id"] . "' />");
				echo("</td>");
		
				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<a class='blue' target='department_popup' href='" . $department_href . $department_row["department_id"] . "'>" . $department_row["department_name"] . "</a>");
				echo("</td>");

				$sql = ("
					SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS contact_name, c.contact_id
					FROM contact AS c, contact_employer AS ce
					WHERE c.contact_id=ce.contact_id
					AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
					AND c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
					AND ce.employer_id='" . $row["employer_id"] . "'
					AND ce.department_id='" . $department_row["department_id"] . "'
                    ORDER BY contact_name
					");
				$contact_result = $GLOBALS['dbh']->Execute($sql);
		
				$contact_row = $contact_result->FetchRow();
		
				// Collect the contact_ids in a variable.
				$contact_id_list .= ("'" . $contact_row["contact_id"] . "',");

				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(" . $department_row["department_id"]  . ", " . $row["employer_id"] . ")' value='" . $contact_row["contact_id"] . "' />");
				echo("</td>");

				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<a class='blue' target='contact_popup' href='" . $contact_href . $contact_row["contact_id"] . "'>" . $contact_row["contact_name"] . "</a>");
				echo("</td>");
		
				echo("</tr>");
		
				$rowclass++;
			
				while ($contact_row = $contact_result->FetchRow())
				{
					// Collect the contact_ids in a variable.
					$contact_id_list .= ("'" . $contact_row["contact_id"] . "',");

					echo("<tr>");
		
						echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
						echo("&nbsp;");
						echo("</td>");
		
						echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
						echo("&nbsp;");
						echo("</td>");
		
						echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
						echo("&nbsp;");
						echo("</td>");
				
						echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
						echo("&nbsp;");
						echo("</td>");
			
						echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
						echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(" . $department_row["department_id"]  . ", " . $row["employer_id"] . ")' value='" . $contact_row["contact_id"] . "' />");
						echo("</td>");
		
						echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
						echo("<a class='blue' target='contact_popup' href='" . $contact_href . $contact_row["contact_id"] . "'>" . $contact_row["contact_name"] . "</a>");
						echo("</td>");
		
					echo("</tr>");
		
					$rowclass++;
				}
			}	
		}
	}

	echo("</table>");

	echo("</td>");
	echo("</tr>");
	echo("</table>");
}

if ($contact_id_list)
{
	$contact_id_list = substr($contact_id_list, 0, -1);
}
else
{
	$contact_id_list = ("''");
}

$sql = ("
        SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS contact_name, c.contact_id, 
	ec.employer_id, ec.company_name, ed.department_id, ed.department_name
        FROM contact AS c, employer_company AS ec, contact_employer AS ce
	LEFT JOIN employer_department AS ed ON ce.department_id=ed.department_id
        WHERE c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
        AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
        AND c.type='employer'
        AND ce.contact_id=c.contact_id
        AND ec.employer_id=ce.employer_id
	AND c.contact_id NOT IN (" . $contact_id_list . ")
        ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	echo("<h3>Matching Contacts:</h3>");

	echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
	echo("<td>");

	echo("<table border='0' cellpadding='2'>");

	echo("<tr>");
		echo("<td class='rowgrey' align='center'>&nbsp;</td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name(s)&nbsp;</b></td>");
		echo("<td class='rowgrey' align='center'>&nbsp;</td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Division Name&nbsp;</b></td>");
		echo("<td class='rowgrey' align='center'>&nbsp;</td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Contact Name&nbsp;</b></td>");
	echo("</tr>");

	$rowclass = 0;

	$company_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_details&amp;no_buttons=true&amp;employer_id=";
	$division_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_department&amp;no_buttons=true&amp;department_id=";
	$contact_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;no_buttons=true&amp;contact_id=";

	while ($row = $result->FetchRow())
	{
		echo("<tr>");
			echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='radio' name='matching_employer_id' onclick='uncheck_depts()' value='" . $row["employer_id"] . "' />");
			echo("</td>");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<a class='blue' target='company_popup' href='" . $company_href . $row["employer_id"] . "'>&nbsp;" . $row["company_name"] . "&nbsp;</a>");
			echo("</td>");
		
			if ($row["department_id"])
			{
				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . ")' value='" . $row["department_id"] . "' />");
				echo("</td>");
			
				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<a class='blue' target='division_popup' href='" . $division_href . $row["department_id"] . "'>&nbsp;" . $row["department_name"] . "&nbsp;</a>");
				echo("</td>");

				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(" . $row["department_id"]  . ", " . $row["employer_id"] . ")' value='" . $row["contact_id"] . "' />");
				echo("</td>");

				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<a class='blue' target='contact_popup' href='" . $contact_href . $row["contact_id"] . "'>&nbsp;" . $row["contact_name"] . "&nbsp;</a>");
				echo("</td>");

			}
			else
			{
				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<input type='radio' name='matching_department_id' onclick='check_parent_company(" . $row["employer_id"] . "); uncheck_contacts();' value='" . $count-- . "' />");
				echo("</td>");

				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("None");
				echo("</td>");
				
				echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<input type='radio' name='matching_contact_id' onclick='check_parent_department(" . ($count + 1) . ", " . $row["employer_id"] . ")' value='" . $row["contact_id"] . "' />");
				echo("</td>");
				
				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("<a class='blue' target='contact_popup' href='" . $contact_href . $row["contact_id"] . "'>&nbsp;" . $row["contact_name"] . "&nbsp;</a>");
				echo("</td>");
			}
		echo("</tr>");

		$rowclass++;
	}

	echo("</table>");

	echo("</td>");
	echo("</tr>");
	echo("</table>");
}

echo("<br />");
echo("<h3 align='center'>-Or-</h3>");
echo("<br />");

echo("<center>");
echo("<table border='0' cellpadding='9' cellspacing='0' class='row1'>");

echo("<tr>");
    echo("<td align='center' colspan='2'><b class='black'>Choose from a list</b></td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("Company/Division:&nbsp;");
    echo("<input type='text' size='45' name='company_div_name' value='" . $company_div_name . "' READONLY />");
    echo("<input type='hidden' name='comp_div_id' value='" . $comp_div_id . "' />");
    echo("&nbsp;<input type='button' name='company_list' value='Choose from a list' onclick='clearContacts();popUpCompany(\"company_div_name\", \"comp_div_id\", \"form1\");' />");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("Contact:&nbsp;");
    echo("<input type='text' size='45' name='contact_name' value='" . $contact_name . "' READONLY />");
    echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    echo("&nbsp;<input type='button' name='contact_list' value='Choose from a list' onclick='popUpContact(\"contact_name\", \"contact_id\", \"comp_div_id\", \"form1\");' />");
    echo("</td>");
echo("</tr>");
            
echo("</table>");
echo("</center>");

echo("<br />");

echo("<table border='0'>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='updateCompany_ignoreContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Update the chosen company with the new information and discard the contact information.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='updateDepartment_ignoreContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Update the chosen division with the new information and discard the contact information.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='ignoreCompany_updateContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Update the chosen contact with the new information and discard the company information.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='updateCompany_updateContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Update the chosen company and contact with the new information.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td>");
    echo("<input type='radio' name='choice' value='updateDepComp_updateContact' />");
    echo("</td>");
    echo("<td align='left'>");
    echo("Update the chosen company/division and contact (<b>from the pop-up box</b>) with the new information.");
    echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='updateDepartment_updateContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Update the chosen division and contact with the new information.");
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
