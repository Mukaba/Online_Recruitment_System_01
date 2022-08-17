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
 | Filename: view_specific_contact.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the inputs for a contact.   |
 |  The form can be displayed without buttons, in which case the user is given  |
 | the option of viewing the information in PDF (for easy printing).            |
 +------------------------------------------------------------------------------+

*/

//:TODO: This file no longer does PDF output. Adjust comments as required. 

if (!$no_buttons)
{
	/*
	 Figure out where this contact's contact_id lies in the contact_id_list string (that has been passed to this script)
	 and then find the left and right contact_id_list for the links at the bottom and top of the page.
	*/
	
	$contact_id_array = explode(",", trim($contact_id_list));
	
	for ($i = 0; $i < sizeof($contact_id_array); $i++)
	{
		if ($contact->contact_id == $contact_id_array[$i])
		{
			$position = $i;
			$left_pos = $i - 1;
			$right_pos = $i + 1;
			break;
		}
	}
	
	if ($left_pos < 0)
	{
		$left_pos = (sizeof($contact_id_array) - 1);
	}
	if ($right_pos > (sizeof($contact_id_array) - 1))
	{
		$right_pos = 0;
	}
	
	$left_id = trim($contact_id_array[$left_pos]);
	$right_id = trim($contact_id_array[$right_pos]);
	
	
	/*
	 formStart is placed at the start of the rest of the output when just displaying the contact's information.
	 When sent to be seen in PDF, formStart is ignored, because we don't want any form information showing up
	 in the PDF view.
	*/
	$formStart .= ("
		<center>
		<table border='0' width='96%'>
		<tr>
		<td align='left'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='contact' />
		<input type='hidden' name='continue' value='view_specific_contact' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
        <input type='hidden' name='search_company_criteria' value='".$search_company_criteria."' />
        <input type='hidden' name='search_department_criteria' value='".$search_department_criteria."' />
		<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />
		<input type='hidden' name='contact_id' value='" . $left_id . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='image' src='misc/images/previous.gif' />
		</form>
		</td>
		<td align='center'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='contact' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
        <input type='hidden' name='search_company_criteria' value='".$search_company_criteria."' />
        <input type='hidden' name='search_department_criteria' value='".$search_department_criteria."' />
        <input type='hidden' name='start_row' value='" . $start_row . "' />
        <input type='hidden' name='per_page_max' value='" . $per_page_max . "' />
		<input type='submit' name='continue' value='Back to List' />
		</form>
		</td>
		<td align='right'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='contact' />
		<input type='hidden' name='continue' value='view_specific_contact' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
        <input type='hidden' name='search_company_criteria' value='".$search_company_criteria."' />
        <input type='hidden' name='search_department_criteria' value='".$search_department_criteria."' />
		<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />
		<input type='hidden' name='contact_id' value='" . $right_id . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='image' src='misc/images/next.gif' />
		</form>
		</td>
		</tr>
		</table>
		</center>
		");
}

$sql = ("
	SELECT DISTINCT company_name, city
	FROM employer_company
	WHERE employer_id='" . $contact->contact_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$parent_company = $row["company_name"];
$parent_company_city = $row["city"];

if ($contact->contact_department_id && $contact->contact_department_id != 'none')
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

$sql = ("
        SELECT CURRENT_DATE as today
        ");
$date_result = $GLOBALS['dbh']->Execute($sql);
$date_row = $date_result->FetchRow();
$todays_date = $date_row["today"];

/*
 Next is the last part of the screen that will be displayed.  We declare it now to get it out of the way.
 If we are showing form buttons, then this needs to be added after those buttons have been displayed, but
 if we are viewing in PDF, it needs to be added before the buttons (actually, the buttons aren't even
 displayed when viewing in PDF).
*/

$contactEnd .= ("</table>");
$contactEnd .= ("</td>");
$contactEnd .= ("</tr>");
$contactEnd .= ("</table>");
$contactEnd .= ("</center>");

/*
 And now for the rest of the contact's information.  This is saved to a variable called contactOutput, which
 is then printed to the screen.
*/

$contactOutput .= ("<center>");

// Open up the table that everything is displayed in.

$contactOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='96%'>");
$contactOutput .= ("<tr>");
	$contactOutput .= ("<td align='center' valign='middle' colspan='2'>");

	// Secondary table to avoid ugly lines all over the place.

	$contactOutput .= ("<table border='0' align='center' width='100%'>");
	$contactOutput .= ("<tr>");
	$contactOutput .= ("<td colspan='2' align='center'>");
	$contactOutput .= ("<h3>");
	$contactOutput .= ("<br />");
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
	$division_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_department&amp;show_quick=true&amp;no_buttons=true&amp;department_id=");

	$contactOutput .= ("<tr>");
		$contactOutput .= ("<td align='center' colspan='2'>");
		$contactOutput .= ("<b class='black'>Parent Company:&nbsp;</b>");
		$contactOutput .= ("<a class='blue' href='" . $employer_href . $contact->contact_employer_id . "'>" . $parent_company . "</a>");
		$contactOutput .= (($parent_company_city) ? "&nbsp;(" . $parent_company_city . ")" : "");
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");

	if ($parent_department)
	{
		$contactOutput .= ("<tr>");
			$contactOutput .= ("<td align='center' colspan='2'>");
			$contactOutput .= ("<b class='black'>Parent Division:&nbsp;</b>");
			$contactOutput .= ("<a class='blue' href='" . $division_href . $contact->contact_department_id . "'>" . $parent_department . "</a>");
			$contactOutput .= (($parent_department_city) ? "&nbsp;(" . $parent_department_city . ")" : "");
			$contactOutput .= ("</td>");
		$contactOutput .= ("</tr>");
	}

$contactOutput .= ("<tr>");
	$contactOutput .= ("<td colspan='2'><br /><hr /></td>");
$contactOutput .= ("</tr>");

$contactOutput .= ("<tr align='center'>");
	$contactOutput .= ("<td width='50%'>&nbsp;</td>");
	$contactOutput .= ("<td width='50%'>&nbsp;</td>");
$contactOutput .= ("</tr>");

if ($contact->contact_title)
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact'sTitle:</b></td>");
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
		$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Dear... :</b></td>");
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
        $contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact's E-mail:</b>");
        $contactOutput .= ("<form name='emailform1' method='post' action='".$PHP_SELF."'>"); 
        $contactOutput .= ("<input type='hidden' name='level1' value='contact' />");
        $contactOutput .= ("<input type='hidden' name='select' value='view_contact' />");
        $contactOutput .= ("<input type='hidden' name='continue' value='set_contact_mail' />");
        $contactOutput .= ("<input type='hidden' name='to' value='".$contact->contact_email."' />");
        $contactOutput .= ("<input type='hidden' name='contact_id' value='".$contact->contact_id."' />");
        if(isset($searchContact)){
            $contactOutput .= ("<input type='hidden' name='contact_id_list' value='".$contact_id_list."' />");
            $contactOutput .= ("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />");
        }
        
        $contactOutput .= ("<input type='hidden' name='no_buttons' value='".$no_buttons."' />");
        //$contactOutput .= ("<input type='hidden' name='select' value='set_contact_mail' />"); 
        $contactOutput .= ("</form>");
        $contactOutput .= ("</td>");
        $contactOutput .= ("<td width='50%' align='left'>");
        $contactOutput .= ("<a href='javascript:document.emailform1.submit()'>". $contact->contact_email . "</a>");
        $contactOutput .= ("</td>");
        //$contactOutput .= ("<td width='50%' align='left'><a href='mailto:" . $contact->contact_email . "'>" . $contact->contact_email . "</a></td>");
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
		$contactOutput .= ("<td width='50%' align='left'>" . $contact->contact_department_name . "</td>");
	$contactOutput .= ("</tr>");
}


$sql = ("
        SELECT *
        FROM employer_login
        WHERE contact_id='" . $contact->contact_id . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);

$contactOutput .= ("<tr align='center'>");
$contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Contact Has Login ID?:</b></td>");
if($result->RecordCount() != 0)
{
    $contactOutput .= ("<td width='50%' align='left'>Yes</td>");
}else{
    $contactOutput .= ("<td width='50%' align='left'>No</td>");
}
$contactOutput .= ("</tr>");


if($result->RecordCount() != 0){
    $sql = ("
            SELECT last_datetime_in
            FROM login_info
            WHERE contact_id = '".$contact->contact_id."'");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        $contactOutput .= ("<tr align='center'>");
        $contactOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Last Login:</b></td>");
        $contactOutput .= ("<td width='50%' align='left'>" . date("F d, Y",strtotime($row["last_datetime_in"])) . "</td>");
        $contactOutput .= ("</tr>");
    }
}


$contactOutput .= ("<tr align='center'>");
$contactOutput .= ("<td colspan='2'>&nbsp;</td>");
$contactOutput .= ("</tr>");

// Pull out company and division location information in case we have a link.
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

$contact_info_valid = (trim($contact->contact_street_address_1) || trim($contact->contact_city) || trim($contact->contact_region_id)
|| trim($contact->contact_provstate_id) || trim($contact->contact_country_id) || trim($contact->contact_postal_code));


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

// Bring me a status for each department that posts jobs (job_list = 1).
$sql = ("
    SELECT eis.status_name, d.department_name
    FROM department_contact_status dcs
    INNER JOIN employer_info_statuses eis
    ON dcs.status_id = eis.status_id
    INNER JOIN department d
    ON dcs.department_id = d.department_id
    WHERE dcs.contact_id = '".$contact->contact_id."' AND d.job_list = '1' AND dcs.status_id != '".CONTACT_POTENTIAL_LEAD."' AND dcs.current_status = '1'
    ORDER BY d.department_name
    ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	$contactOutput .= ("<tr align='center'>");
		$contactOutput .= ("<td align='center' colspan='2'>");
		$contactOutput .= ("<b>This contact has the following statuses with the departments listed below:</b>");
		$contactOutput .= ("</td>");
	$contactOutput .= ("</tr>");

    while ($row = $result->FetchRow())
    {
		$contactOutput .= ("<tr align='center'>");
			$contactOutput .= ("<td align='right' width='50%'>");
			$contactOutput .= ("<b>" . $row["department_name"] . ":</b>");
			$contactOutput .= ("</td>");
			$contactOutput .= ("<td align='left' width='50%'>");
			$contactOutput .= ("<b class='black'>" . $row["status_name"] . "</b>");
			$contactOutput .= ("</td>");
		$contactOutput .= ("</tr>");
    }

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

$sql_actions = ("
        SELECT DISTINCT cas.action_id, cas.contact_id, cas.action_by, cas.action_on, cas.action_method_id, cas.action_note, cas.bring_forward_date
        FROM contact_actions_set AS cas
        WHERE cas.contact_id = '" .$contact->contact_id. "'
        ORDER BY cas.action_on DESC, cas.contact_actions_id DESC
        LIMIT 10
        ");
$result_actions = $GLOBALS['dbh']->Execute($sql_actions);

if ($result_actions->RecordCount())
{
    $contactOutput .= ("</table>\n");
    $contactOutput .= ("<hr />");
    $contactOutput .= ("<table cellpadding='5' cellspacing='0' border='0' align='center'>\n");
    $contactOutput .= ("<tr>");
    $contactOutput .= ("<td colspan='6'>&nbsp;</td>");
    $contactOutput .= ("</tr>\n");
    $contactOutput .= ("<tr>");
    $contactOutput .= ("<td align='center' colspan='6'>");
    $contactOutput .= ("Ten Most Recent Actions/Notes for ".$contact->contact_first_name." ".$contact->contact_last_name.": ");
    $contactOutput .= ("</td>");
    $contactOutput .= ("</tr>\n");
    $contactOutput .= ("<tr>");
    $contactOutput .= ("<td align='center' colspan='6'>&nbsp;</td>");
    $contactOutput .= ("</tr>\n");

    $count = 0; // keep a counter to identify the actions displayed
    while ($row_actions = $result_actions->FetchRow())
    {
        // Get action name
        $mini_sql = ("
                SELECT brief_action_name
                FROM action_types
                WHERE action_id = '" . $row_actions['action_id']. "'
                ");
        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
        $mini_row = $mini_result->FetchRow();
        $action_name = $mini_row["brief_action_name"];

        // Get action method name
        // Note: action methods introduced at later development date, thus some actions methods were "Never Set"
        $mini_sql =("
                SELECT action_method_name
                FROM action_methods
                WHERE action_method_id = '" .$row_actions["action_method_id"]. "'
                ");
        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
        $mini_row = $mini_result->FetchRow();
        $action_method_name = ($mini_row["action_method_name"]!="" ? $mini_row["action_method_name"] : "Never Set");

        // get name of action_by
        $mini_sql = ("
                SELECT CONCAT(first_name,' ', last_name) AS name
                FROM contact
                WHERE contact_id='".$row_actions['action_by']."'
                ");
        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
        $mini_row = $mini_result->FetchRow();
        $display_name = $mini_row['name'];

        // Display our query results
        $contactOutput .= ("<tr>");
        
        // Display bring_forward_date in pop-up form if this action had one
        if(($row_actions["bring_forward_date"]!='0000-00-00') && ($row_actions["bring_forward_date"] != NULL) && ($row_actions["bring_forward_date"] > $todays_date))
        {
            $contactOutput .= ("<td nowrap='nowrap'>");

            // pop-up function call
            $contactOutput .= ("<a href='javascript:void(0);' onMouseOver=\"return overlib('".$row_actions['bring_forward_date']."', CAPTION, 'Bring Forward Date', LEFT, VAUTO, STICKY, ol_closeclick);\" onMouseOut=\"nd();\">");       
            $contactOutput .= ("<img border='0' src='misc/images/clock.gif' alt='' />");
            $contactOutput .= ("</a>");
            $contactOutput .= ("</td>\n");
        }
        else
        {
            $contactOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
        }

        // Display note in pop-up form if this action had one
        if($row_actions["action_note"])
        {
            $contactOutput .= ("<td nowrap='nowrap'>");
            $contact_action_note = trim($row_actions['action_note']);
            $contact_action_note = addslashes($contact_action_note);

                    //  newlines, carriage returns, and double quotes passed into overlib() function must be changed to work properly
                    $contact_action_note = str_replace("\"", "'", $contact_action_note);
                    $contact_action_note = str_replace("\r", "", $contact_action_note);
                    $contact_action_note = str_replace("\n", "<br />", $contact_action_note);

                    // pop-up function call
                    $contactOutput .= ("<a href='javascript:void(0);' onMouseOver=\"return overlib('".$contact_action_note."', CAPTION, 'Action Note', LEFT, VAUTO, STICKY, ol_closeclick);\" onMouseOut=\"nd();\">"); 
                    $contactOutput .= ("<img border='0' src='misc/images/sticky_sm.gif' alt='' />");
                    $contactOutput .= ("</a>");
                    $contactOutput .= ("</td>\n");
        }
        else
        {
            $contactOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
        }

        //date of action
        $contactOutput .= ("<td nowrap='nowrap'>");
        $contactOutput .= ("<b class='black'>On:</b> " . formatShortDate($row_actions["action_on"]));
        $contactOutput .= ("</td>\n");
        $date[$count] = formatShortDate($row_actions["action_on"]);

        //action chosen
        $contactOutput .= ("<td nowrap='nowrap'>");
        if ($row_actions['action_id'] == GENERIC_NOTE)
        {
            $contactOutput .= ("<b class='black'>Action:</b> <b>" . $action_name . "</b>");
        } else {
            $contactOutput .= ("<b class='black'>Action:</b> " . $action_name);
        }
        $contactOutput .= ("</td>\n");

        //action performed by
        if($row_actions['action_id'] == INT_CONDUCTED)
        {
            $mini_sql = ("
                    SELECT interviewer_name
                    FROM interview_interviewer
                    WHERE interviewer_id='".$row_actions['action_by']."'
                    ");
            $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
            $mini_row  = $mini_result->FetchRow();
            $display_name = $mini_row[0];
        }
        // 'By' field will contain interviewer names, not the staff memeber who entered the interview into system
        $contactOutput .= ("<td nowrap='nowrap'>");
        $contactOutput .= ("<b class='black'>By: </b>".$display_name);
        $contactOutput .= ("</td>\n");

        // method of action (do not display method column if action is "GENERIC NOTE" or if it's JOB_DESC_RECIEVED)
        $contactOutput .= ("<td nowrap='nowrap'>");
        $contactOutput .= (($row_actions["action_id"] == GENERIC_NOTE || $row_actions["action_id"] == JOB_DESC_RECEIVED)? "&nbsp;" : "<b class='black'>Method:</b> " . $action_method_name); 
        $contactOutput .= ("</td>\n");
        $contactOutput .= ("</tr>\n");
    }
    $contactOutput .= ("</table>");
    $contactOutput .= ("<table width='100%' border='0'>");
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

if ($contact->contact_changes_recorded_1)
{
        $contactOutput .= ("<tr>");
                $contactOutput .= ("<td colspan='2'><hr /></td>");
        $contactOutput .= ("</tr>");

                $contactOutput .= ("<tr>");
                        $contactOutput .= ("<td align='center' colspan='2'>");
                        $contactOutput .= ("<b class='black'>Recent history of changes for this contact:</b>");
                        $contactOutput .= ("</td>");
                $contactOutput .= ("</tr>");

                $contactOutput .= ("<tr>");
                        $contactOutput .= ("<td width='50%'>&nbsp;</td>");
                        $contactOutput .= ("<td width='50%'>&nbsp;</td>");
                $contactOutput .= ("</tr>");

                $contactOutput .= ("<tr>");
                        $contactOutput .= ("<td align='right'>");
                        $contactOutput .= ("<b>Fields changed:</b>");
                        $contactOutput .= ("</td>");
                        $contactOutput .= ("<td align='left'>");
                        $contactOutput .= ($contact->contact_changes_recorded_1);
                        $contactOutput .= ("</td>");
                $contactOutput .= ("</tr>");

                $contactOutput .= ("<tr>");
                        $contactOutput .= ("<td align='right'>");
                        $contactOutput .= ("<b>Changes above made on:</b>");
                        $contactOutput .= ("</td>");
                        $contactOutput .= ("<td align='left'>");
                        $contactOutput .= (formatLongDate($contact->contact_change_date_1));
                        $contactOutput .= ("</td>");
                $contactOutput .= ("</tr>");

                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                        FROM contact AS a, department AS b, contact_internal AS c
                        WHERE a.contact_id='" . $contact->contact_change_by_1 . "'
                        AND c.contact_id=a.contact_id
                        AND b.department_id=c.department_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                $contactOutput .= ("<tr>");
                        $contactOutput .= ("<td align='right'>");
                        $contactOutput .= ("<b>Changes above made by:</b>");
                        $contactOutput .= ("</td>");
                        $contactOutput .= ("<td align='left'>");
                        $contactOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                        $contactOutput .= ("</td>");
                $contactOutput .= ("</tr>");

                if ($contact->contact_changes_recorded_2)
                {
                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $contactOutput .= ("</tr>");

                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td align='right'>");
				$contactOutput .= ("<b>Fields changed:</b>");
                                $contactOutput .= ("</td>");
                                $contactOutput .= ("<td align='left'>");
                                $contactOutput .= ($contact->contact_changes_recorded_2);
                                $contactOutput .= ("</td>");
                        $contactOutput .= ("</tr>");

                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td align='right'>");
                                $contactOutput .= ("<b>Changes above made on:</b>");
                                $contactOutput .= ("</td>");
                                $contactOutput .= ("<td align='left'>");
                                $contactOutput .= (formatLongDate($contact->contact_change_date_2));
                                $contactOutput .= ("</td>");
                        $contactOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
                                WHERE a .contact_id='" . $contact->contact_change_by_2 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td align='right'>");
                                $contactOutput .= ("<b>Changes above made by:</b>");
                                $contactOutput .= ("</td>");
                                $contactOutput .= ("<td align='left'>");
                                $contactOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $contactOutput .= ("</td>");
                        $contactOutput .= ("</tr>");
                }
                if ($contact->contact_changes_recorded_3)
                {
                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $contactOutput .= ("</tr>");

                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td align='right'>");
                                $contactOutput .= ("<b>Fields changed:</b>");
                                $contactOutput .= ("</td>");
                                $contactOutput .= ("<td align='left'>");
                                $contactOutput .= ($contact->contact_changes_recorded_3);
                                $contactOutput .= ("</td>");
                        $contactOutput .= ("</tr>");

                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td align='right'>");
                                $contactOutput .= ("<b>Changes above made on:</b>");
                                $contactOutput .= ("</td>");
                                $contactOutput .= ("<td align='left'>");
                                $contactOutput .= (formatLongDate($contact->contact_change_date_3));
                                $contactOutput .= ("</td>");
                        $contactOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
				WHERE a .contact_id='" . $contact->contact_change_by_3 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $contactOutput .= ("<tr>");
                                $contactOutput .= ("<td align='right'>");
                                $contactOutput .= ("<b>Changes above made by:</b>");
                                $contactOutput .= ("</td>");
                                $contactOutput .= ("<td align='left'>");
                                $contactOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $contactOutput .= ("</td>");
                        $contactOutput .= ("</tr>");
                }
}

/*
 And that's the end of the contactOutput variable.  Now we must form the formEnd variable, which will be displayed
 on this screen, but not when viewing the information in PDF.
*/

if (!$no_buttons)
{
	/*
	 Even though the variables below say HTMLCompany and companyOutput, this is simply to allow for reuse of code.
	 We're actually transmitting the contact information to be viewed in PDF.
	*/
	
	$formEnd .= ("
		<center>
		<table border='0' width='96%'>
		<tr>
		<td align='left'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='contact' />
		<input type='hidden' name='continue' value='view_specific_contact' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
        <input type='hidden' name='search_company_criteria' value='".$search_company_criteria."' />
        <input type='hidden' name='search_department_criteria' value='".$search_department_criteria."' />
		<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />
		<input type='hidden' name='contact_id' value='" . $left_id . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='image' src='misc/images/previous.gif' />
		</form>
		</td>
		<td align='center'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='contact' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
        <input type='hidden' name='search_company_criteria' value='".$search_company_criteria."' />
        <input type='hidden' name='search_department_criteria' value='".$search_department_criteria."' />
        <input type='hidden' name='start_row' value='" . $start_row . "' />
        <input type='hidden' name='per_page_max' value='" . $per_page_max . "' />
		<input type='submit' name='continue' value='Back to List' />
		</form>
		</td>
		<td align='right'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='contact' />
		<input type='hidden' name='continue' value='view_specific_contact' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
        <input type='hidden' name='search_company_criteria' value='".$search_company_criteria."' />
        <input type='hidden' name='search_department_criteria' value='".$search_department_criteria."' />
		<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />
		<input type='hidden' name='contact_id' value='" . $right_id . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='image' src='misc/images/next.gif' />
		</form>
		</td>
		</tr>
		</table>
		</center>
		");
}

echo($formStart . $contactOutput . $contactEnd . $formEnd);

?>
