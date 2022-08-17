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
 | Filename: view_company_specifics.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the attributes for a        |
 | company.  The form can be displayed without buttons, in which case the user  |
 | is given the option of viewing the information in PDF (for easy printing).   |
 +------------------------------------------------------------------------------+

*/

$contact = new EditContact('');
$contact->loadCompanyValues($employer_id);

$companyOutput .= ("<center>");

// Open up the table that everything will be displayed in.  This is the main table that holds everything.

$companyOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='96%'>");
$companyOutput .= ("<tr>");
	$companyOutput .= ("<td align='center' valign='middle' colspan='2'>");

	// Secondary table below is used to avoid ugly lines all over the place.

	$companyOutput .= ("<table border='0' align='center' width='100%'>");
	$companyOutput .= ("<tr>");
	$companyOutput .= ("<td colspan='2' align='center'>");
		$companyOutput .= ("<h3>");
		$companyOutput .= ("<br />");
		$companyOutput .= ($contact->company_name);
		$companyOutput .= ("<br />");
		$companyOutput .= ("</h3>");
	$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
	
	$companyOutput .= ("<tr>");
	        $companyOutput .= ("<td colspan='2'><hr /></td>");
	$companyOutput .= ("</tr>");
	
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");
	
if ($contact->company_phone)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Company Phone:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>");
		$companyOutput .= ($contact->company_phone);
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_fax)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Fax:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>");
		$companyOutput .= ($contact->company_fax);
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_email)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company E-mail:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'><a href='mailto:" . $contact->company_email . "'>" . $contact->company_email . "</a></td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_website)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Website:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>");
		$companyOutput .= ("<a class='blue' target='company_website' href='http://" . stripSlashes($contact->company_website) . "'>" . stripSlashes($contact->company_website) . "</a>");
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_size_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Size:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getSizeRange($contact->company_size_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_industry_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Industry:</b></td>");		
		$companyOutput .= ("<td width='50%' align='left'>" . getIndustryName($contact->company_industry_id) . "</td>");
	$companyOutput .= ("</tr>");
}

$companyOutput .= ("<tr align='center'>");
	$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
$companyOutput .= ("</tr>");

if ($contact->company_street_address_1)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Street Address:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . $contact->company_street_address_1);
		if ($contact->company_street_address_2)
		{
			$companyOutput .= ("<br />" . $contact->company_street_address_2);
		}
		if ($contact->company_street_address_3)
		{
			$companyOutput .= ("<br />" . $contact->company_street_address_3);
		}
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_country_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Country:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getCountryName($contact->company_country_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_provstate_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Province/State:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getProvstateName($contact->company_provstate_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_region_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Region:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getRegionName($contact->company_region_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_city)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>City:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . $contact->company_city . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_postal_code)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . $contact->company_postal_code . "</td>");
	$companyOutput .= ("</tr>");
}


$companyOutput .= ("<tr align='center'>");
	$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
$companyOutput .= ("</tr>");

if ($contact->company_flags)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Flags that<br />have been Checked:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>");
		if (sizeof($contact->company_flags) == 1)
		{
			$companyOutput .= (getFlagName($contact->company_flags[0], "company"));
		}
		else
		{
			for ($i = 0; $i < (sizeof($contact->company_flags) - 1); $i++)
			{
				$companyOutput .= (getFlagName($contact->company_flags[$i], "company") . ", ");
			}
			$companyOutput .= (getFlagName($contact->company_flags[$i], "company"));
		}
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");

}

$companyOutput .= ("<tr>");
	$companyOutput .= ("<td width='50%'>&nbsp;</td>");
	$companyOutput .= ("<td width='50%'>&nbsp;</td>");
$companyOutput .= ("</tr>");

if ($contact->company_description)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td width='50%' align='right'><b>Company Description:</b></td>");
		$companyOutput .= ("<td>");
		
		$companyOutput .= nl2br($contact->company_description);

		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");

	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");

}

if ($contact->company_note_ids)
{
        $companyOutput .= ("<tr>");
                $companyOutput .= ("<td colspan='2'><hr /></td>");
        $companyOutput .= ("</tr>");

        foreach ($contact->company_note_ids AS $cni)
        {
                $sql = ("
                        SELECT DISTINCT *
                        FROM company_notes
                        WHERE note_id='" . $cni . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $name_array = getContactName($row["entered_by"]);
                $contact_name = $name_array["first_name"] . " " . $name_array["last_name"];

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='right'>");
                        $companyOutput .= ("<b>Note entered by: </b><br />");
                        $companyOutput .= ("<b>Note entered on: </b><br />");
                        $companyOutput .= ("<b>Note: </b>");
                        $companyOutput .= ("</td>");
                        $companyOutput .= ("<td align='left'>");
                        $companyOutput .= ($contact_name . "<br />");
                        $companyOutput .= ($row["date_entered"] . "<br />");
                        $companyOutput .= ($row["notes"]);
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");
        }

        $companyOutput .= ("<tr>");
                $companyOutput .= ("<td colspan='2'><hr /></td>");
        $companyOutput .= ("</tr>");
}

/*
if ($contact->company_general_comments)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='right'><b>Comments viewed<br />by all co-op staff:</b></td>");
		$companyOutput .= ("<td><br />");
	
		$temp = split("\n", $contact->company_general_comments);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$companyOutput .= ($temp[$i]);
			$companyOutput .= ("<br />");
		}

		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");

	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");

}
*/

if ($contact->company_department_comments)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='right'><b>Comments viewed by co-op<br />staff in your department:</b></td>");
		$companyOutput .= ("<td>");
		$companyOutput .= ("<br />");

		$companyOutput .= nl2br($contact->company_department_comments);

		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");

}

$sql = ("
	SELECT DISTINCT department_id
	FROM employer_department
	WHERE employer_id='" . $contact->employer_id . "'
	ORDER BY department_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

$division_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=department&amp;continue=view_specific_department&amp;show_quick=true&amp;no_buttons=true&amp;department_id=");

if ($result->RecordCount())
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='center' colspan='2'><b>Divisions at this company:</b></td>");
	$companyOutput .= ("</tr>");

	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='center' colspan='2'>");
		$companyOutput .= ("<table border='0'><tr><td>");
		
		while ($row = $result->FetchRow())
		{
			$sql = ("
				SELECT DISTINCT city
				FROM employer_department
				WHERE department_id='" . $row["department_id"] . "'
				");
			$result2 = $GLOBALS['dbh']->Execute($sql);
			$row2 = $result2->FetchRow();

			$companyOutput .= ("<a class='blue' href='" . $division_href . $row["department_id"] . "'>" . getDivisionName($row["department_id"]) . "</a>");
			$companyOutput .= ($row2["city"] ? " (" . $row2["city"] . ")" : "");
			$companyOutput .= ("\n<br />");
		}
		
		$companyOutput .= ("</td></tr></table>");
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>\n");
}

$sql = ("
	SELECT DISTINCT ce.contact_id, c.first_name, c.last_name
	FROM contact_employer AS ce, contact AS c
	WHERE ce.employer_id='" . $contact->employer_id . "'
	AND (ce.department_id='' OR ce.department_id IS NULL)
	AND ce.contact_id=c.contact_id
	ORDER BY c.last_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

$contact_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;show_quick=true&amp;no_buttons=true&amp;contact_id=");

if ($result->RecordCount())
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='center' colspan='2'><b>Contacts directly under this company:</b></td>");
	$companyOutput .= ("</tr>");

	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='center' colspan='2'>");
		$companyOutput .= ("<table border='0'><tr><td>");
		
		while ($row = $result->FetchRow())
		{
			$companyOutput .= ("<a class='blue' href='" . $contact_href . $row["contact_id"] . "'>" . $row["first_name"] . " " . $row["last_name"] . "</a>");
			$companyOutput .= ("\n<br />");
		}

		$companyOutput .= ("</td></tr></table>\n");
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>\n");
}

if ($contact->company_changes_recorded_1)
{
        $companyOutput .= ("<tr>");
                $companyOutput .= ("<td colspan='2'><hr /></td>");
        $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='center' colspan='2'>");
                        $companyOutput .= ("<b class='black'>Recent history of changes for this company:</b>");
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td width='50%'>&nbsp;</td>");
                        $companyOutput .= ("<td width='50%'>&nbsp;</td>");
                $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='right'>");
                        $companyOutput .= ("<b>Fields changed:</b>");
                        $companyOutput .= ("</td>");
                        $companyOutput .= ("<td align='left'>");
                        $companyOutput .= ($contact->company_changes_recorded_1);
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='right'>");
                        $companyOutput .= ("<b>Changes above made on:</b>");
                        $companyOutput .= ("</td>");
                        $companyOutput .= ("<td align='left'>");
                        $companyOutput .= (formatLongDate($contact->company_change_date_1));
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                        FROM contact AS a, department AS b, contact_internal AS c
                        WHERE a.contact_id='" . $contact->company_change_by_1 . "'
                        AND c.contact_id=a.contact_id
                        AND b.department_id=c.department_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='right'>");
                        $companyOutput .= ("<b>Changes above made by:</b>");
                        $companyOutput .= ("</td>");
                        $companyOutput .= ("<td align='left'>");
                        $companyOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                if ($contact->company_changes_recorded_2)
                {
                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
				$companyOutput .= ("<b>Fields changed:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($contact->company_changes_recorded_2);
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made on:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= (formatLongDate($contact->company_change_date_2));
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
                                WHERE a .contact_id='" . $contact->company_change_by_2 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made by:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");
                }
                if ($contact->company_changes_recorded_3)
                {
                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Fields changed:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($contact->company_changes_recorded_3);
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made on:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= (formatLongDate($contact->company_change_date_3));
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
				WHERE a .contact_id='" . $contact->company_change_by_3 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made by:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");
                }

        $companyOutput .= ("<tr>");
                $companyOutput .= ("<td colspan='2'><hr /></td>");
        $companyOutput .= ("</tr>");
}

$companyOutput .= ("<tr>");
	$companyOutput .= ("<form method='post' action='" . $PHP_SELF . "'>");
	$companyOutput .= ("<input type='hidden' name='select' value='holding_jobs' />");
	$companyOutput .= ("<td colspan='2' align='center'>");
	$companyOutput .= ("<input type='submit' value='Return to Holding Jobs' />");
	$companyOutput .= ("</td>");
	$companyOutput .= ("</form>");
$companyOutput .= ("</tr>");

/*
 Next is the last part of the screen that will be displayed.  We declare it now to get it out of the way.
 If we are showing form buttons, then this needs to be added after those buttons have been displayed, but
 if we are viewing in PDF, it needs to be added before the buttons (actually, the buttons aren't even
 displayed when viewing in PDF).
*/

$companyOutput.= ("</table>");
$companyOutput .= ("</td>");
$companyOutput .= ("</tr>");
$companyOutput .= ("</table>");
$companyOutput .= ("</center>");

/*
 And that's the end of the companyOutput variable.  Now we must form the formEnd variable, which will be displayed
 on this screen, but not when viewing the information in PDF.
*/

echo($companyOutput . $companyEnd);
