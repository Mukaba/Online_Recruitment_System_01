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
 | Filename: confirm_contact_inputs.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the inputs for a contact.   |
 +------------------------------------------------------------------------------+

*/


$sql = ("
	SELECT DISTINCT company_name, city
	FROM employer_company
	WHERE employer_id='" . $contact->contact_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$parent_company = $row["company_name"];
$parent_company_city = $row["city"];

if ($department_id && $department_id != 'none')
{
	$sql = ("
		SELECT DISTINCT department_name, city
		FROM employer_department
		WHERE department_id='" . $contact->contact_department_id . "'
		");
	
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$parent_department = $row["department_name"];
	$parent_department_city = $row["city"];
}

$contactEnd .= ("</table>");
$contactEnd .= ("</td>");
$contactEnd .= ("</tr>");
$contactEnd .= ("</table>");
$contactEnd .= ("</center>");

/*
 And now for the rest of the contact's information.  This is saved to a variable called contactOutput, which
 is then printed to the screen.
*/

if (!$no_buttons)
{
    instruct("Below are the inputs that you have specified.  Please check to make sure they are correct, and then click
    'Add Contact Now' at the bottom of the page to finish adding this contact.");
}


$contactOutput .= ("<center>");
$contactOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='100%'>");
$contactOutput .= ("<tr>");
$contactOutput .= ("<td>");
$contactOutput .= ("<table border='0' align='center' width='100%'>");
$contactOutput .= ("<tr>");
	$contactOutput .= ("<td align='center' colspan='2'>");
	$contactOutput .= ("<h3>");
	if ($contact->contact_title)
	{
		$contactOutput .= (getTitleName($contact->contact_title) . " ");
	}
	$contactOutput .= ($contact->contact_first_name . " ");
	if ($contact->contact_middle_name)
	{
		$contactOutput .= ($contact->contact_middle_name . " ");
	}
	$contactOutput .= ($contact->contact_last_name);
	$contactOutput .= ("<br /></h3>");
	$contactOutput .= ("</td>");
$contactOutput .= ("</tr>");

$employer_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_company&amp;show_quick=true&amp;no_buttons=true&amp;employer_id=");
$division_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=department&amp;continue=view_specific_department&amp;show_quick=true&amp;no_buttons=true&amp;department_id=");

$contactOutput .= ("<tr>");
	$contactOutput .= ("<td align='center' colspan='2'>");
	$contactOutput .= ("<b class='black'>Parent Company:&nbsp;</b>");
	$contactOutput .= ("<a class='blue' href='" . $employer_href . $contact->contact_employer_id . "'>" . $parent_company . "</a>");
	$contactOutput .= (($parent_company_city) ? "&nbsp;(" . $parent_company_city . ")" : "");
	$contactOutput .= ("<br />");
	$contactOutput .= ("</td>");
$contactOutput .= ("</tr>");
$contactOutput .= ("<tr>");
	$contactOutput .= ("<td align='center' colspan='2'>");

if ($parent_department)
{
	$contactOutput .= ("<b class='black'>Parent Division:&nbsp;</b>");
	$contactOutput .= ("<a class='blue' href='" . $division_href . $contact->contact_department_id . "'>" . $parent_department . "</a>");
	$contactOutput .= (($parent_department_city) ? "&nbsp;(" . $parent_department_city . ")" : "");
	$contactOutput .= ("<br /><br />");
}

	$contactOutput .= ("<hr />");
	$contactOutput .= ("</td>");
$contactOutput .= ("</tr>");

$contactOutput .= ("<tr align='center'>");
	$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
$contactOutput .= ("</tr>");

if ($contact->contact_status_id)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact Status:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'><b class='black'>" . getEmployerStatusName($contact->contact_status_id) . "</b></td>");
	$contactOutput .= ("</tr>");
}

$contactOutput .= ("<tr align='center'>");
	$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
$contactOutput .= ("</tr>");

if ($contact->contact_title)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Title:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		$contactOutput .= (getTitleName($contact->contact_title));
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

$contactOutput .= ("<tr align='center'>");
	$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Name:</b></td>");
	$contactOutput .= ("<td width='50%' align='left'>");
	$contactOutput .= ($contact->contact_first_name . " ");
	if ($contact->contact_middle_name)
	{
		$contactOutput .= ($contact->contact_middle_name . " ");
	}
	$contactOutput .= ($contact->contact_last_name);
	$contactOutput .= ("</td>");
$contactOutput .= ("</tr>");

if ($contact->contact_called_name)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Preferred Name:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		$contactOutput .= ($contact->contact_called_name);
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_greeting)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Greeting:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		$contactOutput .= ($contact->contact_greeting);
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_phone)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Phone:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		if (trim($contact->contact_phone) == USE_COMPANY)
                {
                        $sql = ("
                                SELECT DISTINCT phone
                                FROM employer_company
                                WHERE employer_id='" . $contact->contact_employer_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $contactOutput .= ($row["phone"]);
                        $contactOutput .= ("&nbsp;&nbsp;<b class='black'>(Company phone)</b>&nbsp;&nbsp;");
                }
		elseif (trim($contact->contact_phone) == USE_DEPARTMENT)
		{
			$sql = ("
				SELECT DISTINCT phone
				FROM employer_department
				WHERE department_id='" . $contact->contact_department_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			
			if (trim($row["phone"]) == USE_COMPANY)
			{
				$sql = ("
					SELECT DISTINCT phone
					FROM employer_company
					WHERE employer_id='" . $contact->contact_employer_id . "'
					");
				$result = $GLOBALS['dbh']->Execute($sql);
				$row = $result->FetchRow();
			}
			
			$contactOutput .= ($row["phone"]);
			$contactOutput .= ("&nbsp;&nbsp;<b class='black'>(Division phone)</b>&nbsp;&nbsp;");
		}
                else
                {
                        $contactOutput .= ($contact->contact_phone);
                }
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_fax)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Fax:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		if (trim($contact->contact_fax) == USE_COMPANY)
                {
                        $sql = ("
                                SELECT DISTINCT fax
                                FROM employer_company
                                WHERE employer_id='" . $contact->contact_employer_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $contactOutput .= ($row["fax"]);
                        $contactOutput .= ("&nbsp;&nbsp;<b class='black'>(Company fax)</b>&nbsp;&nbsp;");
                }
                elseif (trim($contact->contact_fax) == USE_DEPARTMENT)
                {
                        $sql = ("
                                SELECT DISTINCT fax
                                FROM employer_department
                                WHERE department_id='" . $contact->contact_department_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();
                        
                        if (trim($row["fax"]) == USE_COMPANY)
                        {
                                $sql = ("
                                        SELECT DISTINCT fax
                                        FROM employer_company
                                        WHERE employer_id='" . $contact->contact_employer_id . "'
                                        ");
                                $result = $GLOBALS['dbh']->Execute($sql);
                                $row = $result->FetchRow();
                        }

                        $contactOutput .= ($row["fax"]);
                        $contactOutput .= ("&nbsp;&nbsp;<b class='black'>(Division fax)</b>&nbsp;&nbsp;");
                }
                else
                {
                        $contactOutput .= ($contact->contact_fax);
                }
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_pager)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Pager:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		$contactOutput .= ($contact->contact_pager);
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_cellphone)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Cellphone:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		$contactOutput .= ($contact->contact_cellphone);
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_email)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's E-mail:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $contact->contact_email . "</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_position_title)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Position Title:</b></td>");
		$contactOutput .= ("<td align='left' width='50%'>" . $contact->contact_position_title . "</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_department_name)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Department:</b></td>");
		$contactOutput .= ("<td align='left' width='50%'>" . $contact->contact_department_name . "</td>");
	$contactOutput .= ("</tr>");
}

$contactOutput .= ("<tr align='center'>");
	$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
$contactOutput .= ("</tr>");

/*
 Pull out company and division location information in case we have a link.
*/

$sql = ("
	SELECT DISTINCT street_address_1, street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code
	FROM employer_company
	WHERE employer_id='" . $contact->contact_employer_id . "'
	");
$company_result = $GLOBALS['dbh']->Execute($sql);
$company_row = $company_result->FetchRow();

$sql = ("
	SELECT DISTINCT street_address_1, street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code, location_info
	FROM employer_department
	WHERE department_id='" . $contact->contact_department_id . "'
	");
$department_result = $GLOBALS['dbh']->Execute($sql);
$department_row = $department_result->FetchRow();

if ($department_row["location_info"] == USE_COMPANY)
{
	$department_row = $company_result->FetchRow();
}

$department_info_valid = ($department_row["street_address_1"] || $department_row["city"] || $department_row["region_id"]
|| $department_row["provstate_id"] || $department_row["country_id"] || $department_row["postal_code"]);

$company_info_valid = ($company_row["street_address_1"] || $company_row["city"] || $company_row["region_id"]
|| $company_row["provstate_id"] || $company_row["country_id"] || $company_row["postal_code"]);

$contact_info_valid = ($contact->contact_street_address_1 || $contact->contact_city || $contact->contact_region_id
|| $contact->contact_provstate_id || $contact->contact_country_id || $contact->contact_postal_code);


if ($contact_info_valid 
|| ($department_info_valid && $contact->contact_location_info == USE_DEPARTMENT) 
|| ($company_info_valid && $contact->contact_location_info == USE_COMPANY))
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td colspan='2' align='center'>");
		$contactOutput .= ("<b>Contact's Location");
	
	if ($contact->contact_location_info == USE_COMPANY)
	{
		$contactOutput .= (" (Using Company's Information)");
	}
	elseif ($contact->contact_location_info == USE_DEPARTMENT)
	{
		$contactOutput .= (" (Using Division's Information)");
	}

		$contactOutput .= (":</b></td>");
	$contactOutput .= ("</tr>");
	$contactOutput .= ("<tr>");
		$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_street_address_1) 
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Street Address:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $contact->contact_street_address_1);
		if ($contact->contact_street_address_2)
		{
			$contactOutput .= ("<br />" . $contact->contact_street_address_2);
		}
		if ($contact->contact_street_address_3)
		{
			$contactOutput .= ("<br />" . $contact->contact_street_address_3);
		}
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_COMPANY && $company_row["street_address_1"]) 
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Street Address:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $company_row["street_address_1"]);
		if ($company_row["street_address_2"])
		{
			$contactOutput .= ("<br />" . $company_row["street_address_2"]);
		}
		if ($company_row["street_address_3"])
		{
			$contactOutput .= ("<br />" . $company_row["street_address_3"]);
		}
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_DEPARTMENT && $department_row["street_address_1"])
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Street Address:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $department_row["street_address_1"]);
		if ($department_row["street_address_2"])
		{
			$contactOutput .= ("<br />" . $department_row["street_address_2"]);
		}
		if ($department_row["street_address_3"])
		{
			$contactOutput .= ("<br />" . $department_row["street_address_3"]);
		}
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_country_id)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Country:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getCountryName($contact->contact_country_id) . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_COMPANY && $company_row["country_id"]) 
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Country:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getCountryName($company_row["country_id"]) . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_DEPARTMENT && $department_row["country_id"])
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Country:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getCountryName($department_row["country_id"]) . "</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_provstate_id)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Province/State:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getProvstateName($contact->contact_provstate_id) . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_COMPANY && $company_row["provstate_id"]) 
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Province/State:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getProvstateName($company_row["provstate_id"]) . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_DEPARTMENT && $department_row["provstate_id"])
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Province/State:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getProvstateName($department_row["provstate_id"]) . "</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_region_id)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Region:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getRegionName($contact->contact_region_id) . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_COMPANY && $company_row["region_id"]) 
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Region:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getRegionName($company_row["region_id"]) . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_DEPARTMENT && $department_row["region_id"])
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Region:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . getRegionName($department_row["region_id"]) . "</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_city)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>City:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $contact->contact_city . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_COMPANY && $company_row["city"]) 
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>City:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $company_row["city"] . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_DEPARTMENT && $department_row["city"])
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>City:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $department_row["city"] . "</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_postal_code)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $contact->contact_postal_code . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_COMPANY && $company_row["postal_code"]) 
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $company_row["postal_code"] . "</td>");
	$contactOutput .= ("</tr>");
}
elseif ($contact->contact_location_info == USE_DEPARTMENT && $department_row["postal_code"])
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>" . $department_row["postal_code"] . "</td>");
	$contactOutput .= ("</tr>");
}



if ($contact_info_valid 
|| ($department_info_valid && $contact->contact_location_info == USE_DEPARTMENT) 
|| ($company_info_valid && $contact->contact_location_info == USE_COMPANY))
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
	$contactOutput .= ("</tr>");
}

if ($contact->contact_flags)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact Flags that<br />have been Checked:</b></td>");
		$contactOutput .= ("<td width='50%' align='left'>");
		if (sizeof($contact->contact_flags) == 1)
		{
			$contactOutput .= (getFlagName($contact->contact_flags[0], "contact"));
		}
		else
		{
			for ($i = 0; $i < (sizeof($contact->contact_flags) - 1); $i++)
			{
				$contactOutput .= (getFlagName($contact->contact_flags[$i], "contact") . ", ");
			}
			$contactOutput .= (getFlagName($contact->contact_flags[$i], "contact"));
		}
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
	$contactOutput .= ("</tr>");

}


if ($contact->contact_new_note)
{
	$contactOutput .= ("<tr>");
		$contactOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Notes viewed<br />by all co-op staff:</b></td>");
		$contactOutput .= ("<td><br />");
	
		$temp = split("\n", $contact->contact_new_note);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$contactOutput .= ($temp[$i]);
			$contactOutput .= ("<br />");
		}

		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
	$contactOutput .= ("<tr>");
		$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
	$contactOutput .= ("</tr>");

}

if ($contact->contact_department_comments)
{
	$contactOutput .= ("<tr>");
		$contactOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Comments viewed by co-op<br />staff in your department:</b></td>");
		$contactOutput .= ("<td><br />");

		$temp = split("\n", $contact->contact_department_comments);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$contactOutput .= ($temp[$i]);
			$contactOutput .= ("<br />");
		}

		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");
	$contactOutput .= ("<tr>");
		$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
	$contactOutput .= ("</tr>");

}

/*
 And that's the end of the contactOutput variable.  Now we must form the formEnd variable, which will be displayed
 on this screen
*/

if (!$no_buttons)
{
	$formEnd .= ("
		<tr>
		<td colspan='2'>
        <form method='post' action='" . $PHP_SELF . "'>
        <input type='hidden' name='select' value='add_contact' />
        <input type='hidden' name='contact' value='" . packObject($contact) . "' />
        <input type='hidden' name='level1' value='contact' />

		<table border='0' width='100%'>
		<tr align='center'>
		<td align='right' width='50%'>
		<input type='hidden' name='no_buttons' value='true' />
		<input type='submit' name='continue' value='Add Contact Now' />
		</td>
		<td align='left'>
		<input type='submit' name='continue' value='Edit Contact' />
		<input type='hidden' name='employer_id' value='" . $contact->contact_employer_id . "' />
		<input type='hidden' name='department_id' value='" . $contact->contact_department_id . "' />
		</td>
		</tr>
		</table>
        </form>
		</td>
		</tr>
		");
}
elseif ($no_buttons)
{
    echo("<table border='0' width='100%'>");
    echo("<tr>");
        echo("<td align='left'>");
            echo("<h3 align='left'>Employer Information - Add a New Contact");
            echo("</h3>");
        echo("</td>");
        //Include the quickmenu
        echo("<td align='right'>");
            include('contact/contact_quickmenu.inc');
        echo("</td>");
    echo("</tr>");
	echo("</table>");

	// Get the company name that this contact was just added to.

	$sql = ("
		SELECT DISTINCT company_name
		FROM employer_company
		WHERE employer_id='" . $contact->contact_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$company_name = $row["company_name"];

	// Get the division name this contact was just added to, if there is one.

	if ($contact->contact_department_id != 'none')
	{
		$sql = ("
			SELECT DISTINCT department_name
			FROM employer_department
			WHERE department_id='" . $contact->contact_department_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$department_name = $row["department_name"];
	}

	$msg = ("Contact: " . $contact->contact_first_name . " " . $contact->contact_last_name);
	$msg .= (" has been successfully added to " . $company_name . (($department_name) ? (", " . $department_name) : "") . ".");
	$msg2 .= ("If you would like to automatically generate a login ID and password for this contact, and have it e-mailed to them, ");
	$msg2 .= ("please choose the 'Create Login Info for Contact' option from the quickmenu above.");

	notify($msg);
	instruct($msg2);
}


echo($contactOutput . $formEnd . $contactEnd);

?>
