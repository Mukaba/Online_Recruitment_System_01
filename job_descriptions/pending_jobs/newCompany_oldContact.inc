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
 | Filename: newCompany_oldContact.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is used to give the user the choice of how he would   |
 | like to use the given company information to update our existing records.    |
 +------------------------------------------------------------------------------+

*/

?>

<script type='text/javascript' language='javascript'>
<!--javascript

function popUpContact(slotName, slotName2, formName)
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

	window.open("mamook.php?select=contact_choose&no_headers=1&contact_name_slot="+slot+"&id_slot="+slot2+"&parentFormName="+formName, "ContactChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes");
}

//-->
</script>

<?php
// Pull out pertinent information from database.

$pendingEmployer = new PendingEmployer('');
$pendingEmployer->load_pending_values($job_id);

$checkContactLastName = $pendingEmployer->contact_last_name[0] . $pendingEmployer->contact_last_name[1] . $pendingEmployer->contact_last_name[2];

notify("Please select the contact that matches the contact you are approving (<b>" . $pendingEmployer->contact_first_name
	. " " . $pendingEmployer->contact_last_name . "</b>).  Then, choose one of the options below and click Continue.  
	Should you choose to do so, the contact that you have selected can be updated with the new information.<br /><br />
	You may click the names of the companies and contacts to have a new window open with that company or contact's details displayed.");

$sql = ("
        SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS contact_name, c.contact_id, ec.employer_id, ec.company_name
        FROM contact AS c, employer_company AS ec, contact_employer AS ce
        WHERE c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
        AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
        AND c.type='employer'
        AND ce.contact_id=c.contact_id
        AND ec.employer_id=ce.employer_id
        ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("<h3>Similar Contacts:</h3>");

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='continue' value='method_for_adding' />");
echo("<input type='hidden' name='situation' value='newCompany_oldContact' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo("<input type='hidden' name='department' value='" . $department . "' />");

echo("<table cellspacing='0' cellpadding='0' border='1'>");
echo("<tr>");
echo("<td>");

echo("<table border='0' cellpadding='2'>");

echo("<tr>");
	echo("<td class='rowgrey' align='center'>&nbsp;</td>");
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Contact Name&nbsp;</b></td>");
	echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name(s)&nbsp;</b></td>");
echo("</tr>");

$rowclass = 0;

$company_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_details&amp;no_buttons=true&amp;employer_id=";
$contact_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;no_buttons=true&amp;contact_id=";

while ($row = $result->FetchRow())
{
	echo("<tr>");
		echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		echo("<input type='radio' name='matching_contact_id' value='" . $row["contact_id"] . "' />");
		echo("</td>");

		echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		echo("<a class='blue' target='contact_popup' href='" . $contact_href . $row["contact_id"] . "'>" . $row["contact_name"] . "</a>");
		echo("</td>");

		echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
		echo("<a class='blue' target='company_popup' href='" . $company_href . $row["employer_id"] . "'>" . $row["company_name"] . "</a>");
		echo("</td>");
	
	echo("</tr>");

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
    echo("<td align='center' colspan='2'><b class='black'>Choose from a list</b></td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("Contact:&nbsp;");
    echo("<input type='text' size='45' name='contact_name' value='" . $contact_name . "' READONLY />");
    echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    echo("&nbsp;<input type='button' name='contact_list' value='Choose from a list' onclick='popUpContact(\"contact_name\", \"contact_id\", \"form1\");' />");
    echo("</td>");
echo("</tr>");
            
echo("</table>");
echo("</center>");

echo("<br />");


echo("<table border='0'>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='updateContact_addCompany' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Add the new company, move the chosen contact to this company, and update the contact with the new information.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='choice' value='dontUpdate_addCompany' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("Add the new company, move the chosen contact to this company, but do NOT update the contact with the new information.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td>");
    echo("<input type='radio' name='choice' value='updateContactFromBox_addCompany' />");
    echo("</td>");
    echo("<td align='left'>");
    echo("Add the new company, move the contact chosen from the <b>pop-up box</b>, and update the contact with the new information.");
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
