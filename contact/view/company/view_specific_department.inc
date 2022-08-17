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
 | Filename: view_specific_department.inc                                       |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the attributes for a        |
 | division.  The form can be displayed without buttons, in which case the      |
 | user is given the option of viewing the information in PDF (for easy         |
 | printing).                                                                   |
 +------------------------------------------------------------------------------+

*/

//:TODO: This file no longer does PDF export. Adjust comments as required. 

if(strcmp($message_sent,"yes") == 0)
{
    echo "<br />".notify("The e-mail has been sent successfully.")."<br />";
    $message_sent == "";
}
$sql = ("
        SELECT CURRENT_DATE as today
        ");
$date_result = $GLOBALS['dbh']->Execute($sql);
$date_row = $date_result->FetchRow();
$todays_date = $date_row["today"];

if ($companies_match)
{
	$msg .= ("There were companies in the database that matched the criteria you searched on.  Click the button below to view these companies.");
	$msg .= ("<br /><br />");
	$msg .= ("<form method='post' action='" . $PHP_SELF . "'>");
	$msg .= ("<input type='hidden' name='select' value='view_contact' />");
	$msg .= ("<input type='hidden' name='level1' value='company' />");
	$msg .= ("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
	$msg .= ("<input type='hidden' name='continue' value='view_company_search_results' />");
	$msg .= ("<center><input type='submit' value='View Companies Returned' /></center>");
	$msg .= ("</form>");
	
	instruct($msg);

	echo("<br />");
}

if (!$no_buttons)
{
	/*
	 Figure out where this division's department_id lies in the department_id_list string (that has been passed to this script)
	 and then find the left and right department_id_list for the links at the bottom and top of the page.
	*/
	 
	$department_id_array = explode(",", trim($department_id_list));
	 
	for ($i = 0; $i < sizeof($department_id_array); $i++)
	{
	        if ($contact->department_id == $department_id_array[$i])
	        {
	                $position = $i;
	                $left_pos = $i - 1;
	                $right_pos = $i + 1;
	                break;
	        }
	}
	
	if ($left_pos < 0)
	{
	        $left_pos = (sizeof($department_id_array) - 1);
	}
	if ($right_pos > (sizeof($department_id_array) - 1))
	{
	        $right_pos = 0;
	}
	
	$left_id = trim($department_id_array[$left_pos]);
	$right_id = trim($department_id_array[$right_pos]);
	
	/*
	 formStart is placed at the start of the rest of the output when just displaying the division's information.
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
	        <input type='hidden' name='level1' value='company' />
	        <input type='hidden' name='continue' value='view_specific_department' />
	        <input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
	        <input type='hidden' name='department_id_list' value='" . $department_id_list . "' />
	        <input type='hidden' name='department_id' value='" . $left_id . "' />
            <input type='hidden' name='show_quick' value='true' />
            <input type='hidden' name='companies_match' value='" . $companies_match . "' />
	        <input type='image' src='misc/images/previous.gif' />
	        </form>
	        </td>
	        <td align='center'>
	        <form method='post' action='" . $PHP_SELF . "'>
	        <input type='hidden' name='select' value='view_contact' />
	        <input type='hidden' name='level1' value='company' />
	        <input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
            <input type='hidden' name='continue' value='back_to_department_list' />
	        <input type='submit' value='Back to List' />
	        </form>
	        </td>
	        <td align='right'>
	        <form method='post' action='" . $PHP_SELF . "'>
	        <input type='hidden' name='select' value='view_contact' />
	        <input type='hidden' name='level1' value='company' />
	        <input type='hidden' name='continue' value='view_specific_department' />
	        <input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
	        <input type='hidden' name='department_id_list' value='" . $department_id_list . "' />
	        <input type='hidden' name='department_id' value='" . $right_id . "' />
            <input type='hidden' name='companies_match' value='" . $company_match . "' />
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
	SELECT DISTINCT company_name
	FROM employer_company
	WHERE employer_id='" . $contact->department_company_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$parent_company = $row["company_name"];


/*
 Next is the last part of the screen that will be displayed.  We declare it now to get it out of the way.
 If we are showing form buttons, then this needs to be added after those buttons have been displayed, but
 if we are viewing in PDF, it needs to be added before the buttons (actually, the buttons aren't even
 displayed when viewing in PDF).
*/

$departmentEnd .= ("</table>");
$departmentEnd .= ("</td>");
$departmentEnd .= ("</tr>");
$departmentEnd .= ("</table>");
$departmentEnd .= ("</center>");

/*
 And now for the rest of the division's information.  This is saved to a variable called departmentOutput, which
 is then printed to the screen.
*/

$departmentOutput .= ("<center>");
$departmentOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='96%'>");
$departmentOutput .= ("<tr>");
	$departmentOutput .= ("<td align='center' colspan='2'>");
	$departmentOutput .= ("<table border='0' align='center' width='100%'>");
	$departmentOutput .= ("<tr>");
	$departmentOutput .= ("<td colspan='2' align='center'>");
	$departmentOutput .= ("<h3>");
	$departmentOutput .= ("<br />");
	$departmentOutput .= ($contact->department_name);
	$departmentOutput .= ("<br /></h3>");
	$departmentOutput .= ("</td>");
$departmentOutput .= ("</tr>");

$employer_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_company&amp;show_quick=true&amp;no_buttons=true&amp;employer_id=");

$departmentOutput .= ("<tr>");
	$departmentOutput .= ("<td align='center' colspan='2'>");
	$departmentOutput .= ("<b class='black'>Parent Company:&nbsp;</b>");
	$departmentOutput .= ("<a class='blue' href='" . $employer_href . $contact->department_company_id . "'>" . $parent_company . "</a>");
	$departmentOutput .= ("<br />");
	$departmentOutput .= ("</td>");
$departmentOutput .= ("</tr>");

$departmentOutput .= ("<tr align='center'>");
	$departmentOutput .= ("<td colspan='2'><br /><hr /></td>");
$departmentOutput .= ("</tr>");

$departmentOutput .= ("<tr align='center'>");
	$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
$departmentOutput .= ("</tr>");

if ($contact->department_phone)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division Phone:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>");
		if (trim($contact->department_phone == USE_COMPANY))
                { 
                        $sql = ("
                                SELECT DISTINCT phone
                                FROM employer_company
                                WHERE employer_id='" . $contact->department_company_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $departmentOutput .= ($row["phone"]);
                        $departmentOutput .= ("&nbsp;&nbsp;<b class='black'>(Company phone)</b>&nbsp;&nbsp;");
                }
                else
                {
                        $departmentOutput .= ($contact->department_phone);
                }
		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");
}

if ($contact->department_fax)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division Fax:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>");
		if (trim($contact->department_fax == USE_COMPANY))
                {
                        $sql = ("
                                SELECT DISTINCT fax
                                FROM employer_company
                                WHERE employer_id='" . $contact->department_company_id . "'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $departmentOutput .= ($row["fax"]);
                        $departmentOutput .= ("&nbsp;&nbsp;<b class='black'>(Company fax)</b>&nbsp;&nbsp;");
                }
                else
                {
                        $departmentOutput .= ($contact->department_fax);
                }
		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");
}

if ($contact->department_email)
{
    $departmentOutput .= ("<tr align='center'>");
    $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division E-mail:</b></td>");
    $departmentOutput .= ("<td width='50%' align='left' valign='middle'>");
    if(is_string($searchContact)){
        $searchContact = unpackObject($searchContact);
    }

    $departmentOutput .= ("<form name='emailform1' method='post' action='".$PHP_SELF."'>");
    $departmentOutput .= ("<input type='hidden' name='after' value='contact/view/company/view_company.inc' />");
    $departmentOutput .= ("<input type='hidden' name='to' value='".$contact->department_email."' />");
    $departmentOutput .= ("<input type='hidden' name='employer_id' value='".$contact->employer_id."' />");
    if(isset($searchContact)){
        $departmentOutput .= ("<input type='hidden' name='employer_id_list' value='".$department_id_list."' />");
        $departmentOutput .= ("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />");
    }
    $departmentOutput .= ("<input type='hidden' name='no_buttons' value='".$no_buttons."' />");
    $departmentOutput .= ("<input type='hidden' name='select' value='set_department_mail' />");
    $departmentOutput .= ("<a href='javascript:document.emailform1.submit()'>". $contact->department_email . "</a>");
    $departmentOutput .= ("</form>");
    $departmentOutput .= ("</td>");
    $departmentOutput .= ("</tr>");
}

if ($contact->department_website)
{
    $departmentOutput .= ("<tr align='center'>");
    $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division Website:</b></td>");
    $departmentOutput .= ("<td width='50%' align='left'>");
    $departmentOutput .= ("<a class='blue' target='department_website' href='http://" . stripSlashes($contact->department_website) . "'>" . stripSlashes($contact->department_website) . "</a>");
    $departmentOutput .= ("</td>");
    $departmentOutput .= ("</tr>");
}

if ($contact->department_size_id)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division Size:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>" . getSizeRange($contact->department_size_id) . "</td>");
	$departmentOutput .= ("</tr>");
}

if ($contact->department_industry_id)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division Industry:</b></td>");		
		$departmentOutput .= ("<td width='50%' align='left'>" . getIndustryName($contact->department_industry_id) . "</td>");
	$departmentOutput .= ("</tr>");
}

$departmentOutput .= ("<tr align='center'>");
	$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
$departmentOutput .= ("</tr>");

/*
 Pull out company and division location information in case we have a link.
*/
 
$sql = ("
        SELECT DISTINCT street_address_1, street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code
        FROM employer_company 
        WHERE employer_id='" . $contact->department_company_id . "'
        ");
$company_result = $GLOBALS['dbh']->Execute($sql);
$company_row = $company_result->FetchRow();

$department_info_valid = ($contact->department_street_address_1 || $contact->department_city || $contact->department_region_id
|| $contact->department_provstate_id || $contact->department_country_id || $contact->department_postal_code);

$company_info_valid = ($company_row["street_address_1"] || $company_row["city"] || $company_row["region_id"]
|| $company_row["provstate_id"] || $company_row["country_id"] || $company_row["postal_code"]);

if ($department_info_valid || ($company_info_valid && $contact->department_location_info == USE_COMPANY))
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td colspan='2' align='center'>");
                $departmentOutput .= ("<b>Division's Location");

        if ($contact->department_location_info == USE_COMPANY)
        {
                $departmentOutput .= (" (Using Company's Information)");
        }
                $departmentOutput .= (":</b></td>");
        $departmentOutput .= ("</tr>");
        $departmentOutput .= ("<tr>");
                $departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
        $departmentOutput .= ("</tr>");
}

if ($contact->department_street_address_1)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Street Address:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>" . $contact->department_street_address_1);
		if ($contact->department_street_address_2)
		{
			$departmentOutput .= ("<br />" . $contact->department_street_address_2);
		}
		if ($contact->department_street_address_3)
		{
			$departmentOutput .= ("<br />" . $contact->department_street_address_3);
		}
		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");
}
elseif ($contact->department_location_info == USE_COMPANY && $company_row["street_address_1"])
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division's Street Address:</b></td>");
                $departmentOutput .= ("<td width='50%' align='left'>" . $company_row["street_address_1"]);
                if ($company_row["street_address_2"])
                {
                        $departmentOutput .= ("<br />" . $company_row["street_address_2"]);
                }
		if ($company_row["street_address_3"])
		{
			$departmentOutput .= ("<br />" . $company_row["street_address_3"]);
		}
                $departmentOutput .= ("</td>");
        $departmentOutput .= ("</tr>");
}

if ($contact->department_country_id)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Country:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>" . getCountryName($contact->department_country_id) . "</td>");
	$departmentOutput .= ("</tr>");
}
elseif ($contact->department_location_info == USE_COMPANY && $company_row["country_id"])
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Country:</b></td>");
                $departmentOutput .= ("<td width='50%' align='left'>" . getCountryName($company_row["country_id"]) . "</td>");
        $departmentOutput .= ("</tr>");
}

if ($contact->department_provstate_id)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Province/State:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>" . getProvstateName($contact->department_provstate_id) . "</td>");
	$departmentOutput .= ("</tr>");
}
elseif ($contact->department_location_info == USE_COMPANY && $company_row["provstate_id"])
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Province/State:</b></td>");
                $departmentOutput .= ("<td width='50%' align='left'>" . getProvstateName($company_row["provstate_id"]) . "</td>");
        $departmentOutput .= ("</tr>");
}

if ($contact->department_region_id)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Region:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>" . getRegionName($contact->department_region_id) . "</td>");
	$departmentOutput .= ("</tr>");
}
elseif ($contact->department_location_info == USE_COMPANY && $company_row["region_id"])
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Region:</b></td>");
                $departmentOutput .= ("<td width='50%' align='left'>" . getRegionName($company_row["region_id"]) . "</td>");
        $departmentOutput .= ("</tr>");
}

if ($contact->department_city)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>City:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>" . $contact->department_city . "</td>");
	$departmentOutput .= ("</tr>");
}
elseif ($contact->department_location_info == USE_COMPANY && $company_row["city"])
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>City:</b></td>");
                $departmentOutput .= ("<td width='50%' align='left'>" . $company_row["city"] . "</td>");
        $departmentOutput .= ("</tr>");
}

if ($contact->department_postal_code)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>" . $contact->department_postal_code . "</td>");
	$departmentOutput .= ("</tr>");
}
elseif ($contact->department_location_info == USE_COMPANY && $company_row["postal_code"])
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
                $departmentOutput .= ("<td width='50%' align='left'>" . $company_row["postal_code"] . "</td>");
        $departmentOutput .= ("</tr>");
}


if ($department_info_valid || ($company_info_valid && $contact->department_location_info == USE_COMPANY))
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
        $departmentOutput .= ("</tr>");
}

// Bring me a status for each department that posts jobs (job_list = 1).
$sql = ("
    SELECT d.department_name, eis.status_name
    FROM department_division_status dds
    INNER JOIN employer_info_statuses eis
    ON dds.status_id = eis.status_id
    INNER JOIN department d
    ON dds.department_id = d.department_id
    WHERE dds.division_id = '".$contact->department_id."' AND dds.status_id != '".CONTACT_POTENTIAL_LEAD."' AND d.job_list = '1' AND dds.current_status = '1'
    ORDER BY d.department_name
    ");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='center' colspan='2'>");
		$departmentOutput .= ("<b>This division has the following statuses with the departments listed below:</b>");
		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");

    while ($row = $result->FetchRow())
    {
		$departmentOutput .= ("<tr align='center'>");
			$departmentOutput .= ("<td align='right' width='50%'>");
			$departmentOutput .= ("<b>" . $row["department_name"] . ":</b>");
			$departmentOutput .= ("</td>");
			$departmentOutput .= ("<td align='left' width='50%'>");
			$departmentOutput .= ("<b class='black'>" . $row["status_name"] . "</b>");
			$departmentOutput .= ("</td>");
		$departmentOutput .= ("</tr>");
    }

	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
	$departmentOutput .= ("</tr>");
}

if ($contact->department_flags)
{
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division Flags that<br />have been Checked:</b></td>");
		$departmentOutput .= ("<td width='50%' align='left'>");
		if (sizeof($contact->department_flags) == 1)
		{
			$departmentOutput .= (getFlagName($contact->department_flags[0], "department"));
		}
		else
		{
			for ($i = 0; $i < (sizeof($contact->department_flags) - 1); $i++)
			{
				$departmentOutput .= (getFlagName($contact->department_flags[$i], "department") . ", ");
			}
			$departmentOutput .= (getFlagName($contact->department_flags[$i], "department"));
		}
		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");
	$departmentOutput .= ("<tr align='center'>");
		$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
	$departmentOutput .= ("</tr>");

}


if ($contact->department_description)
{
	$departmentOutput .= ("<tr>");
		$departmentOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Division Description:</b></td>");
		$departmentOutput .= ("<td>");
		
		$temp = split("\n", $contact->department_description);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$departmentOutput .= ($temp[$i]);
			$departmentOutput .= ("<br />");
		}

		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");
	$departmentOutput .= ("<tr>");
		$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
	$departmentOutput .= ("</tr>");

}

$sql = ("
        SELECT DISTINCT ce.contact_id
        FROM contact AS c
        INNER JOIN contact_employer ce
        ON c.contact_id = ce.contact_id
        WHERE ce.department_id='" . $contact->department_id . "'
        AND ce.deleted_flag = '0'
        ORDER BY c.last_name, c.first_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

$contact_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;show_quick=true&amp;no_buttons=true&amp;contact_id=");

if ($result->RecordCount())
{
 
    if($auth->contact_id) 
    {
        $sql1 = ("
                SELECT department_id
                FROM contact_internal
                WHERE contact_id = ".$auth->contact_id);

        $result1 = $GLOBALS['dbh']->Execute($sql1);

        $row1 = $result1->FetchRow();
        $user_department = $row1["department_id"];
    }

    $departmentOutput .= ("<tr>");
    $departmentOutput .= ("<td colspan='2'><hr /></td>");
    $departmentOutput .= ("</tr>");


    $departmentOutput .= ("<tr>");
    $departmentOutput .= ("<td align='center' colspan='2'><b>Contacts under this division:</b></td>");
    $departmentOutput .= ("</tr>");

    $departmentOutput .= ("<tr>");
    $departmentOutput .= ("<td align='center' colspan='2'>");
    $departmentOutput .= ("<table border='0'><tr><td>");

    while ($row = $result->FetchRow())
    {
        $sql2 = ("
                SELECT status_id
                FROM department_contact_status
                WHERE contact_id='" . $row["contact_id"] . "' 
                AND department_id = '".$user_department."'
                AND current_status = '1'
                ");
        $result2 = $GLOBALS['dbh']->Execute($sql2);
        $row2 = $result2->FetchRow();

        $contact_name = getContactName($row["contact_id"]);
        if($row2["status_id"] <> CONTACT_POTENTIAL_LEAD)
        {            
            $contact_status = "(".getEmployerStatusName($row2["status_id"]).")";
            $departmentOutput .= ("<a class='blue' href='" . $contact_href . $row["contact_id"] . "'><b>" . $contact_name["last_name"] . ", " . $contact_name["first_name"] . "</b></a>"." ".$contact_status);
        } else {
            $departmentOutput .= ("<a class='blue' href='" . $contact_href . $row["contact_id"] . "'>" . $contact_name["last_name"] . ", " . $contact_name["first_name"] . "</a>");
        }
        $departmentOutput .= ("\n<br />");
        $contact_status = "";
    }

    $departmentOutput .= ("</td></tr></table>");
    $departmentOutput .= ("</td>");
    $departmentOutput .= ("</tr>");
}

$sql_actions = ("
        SELECT cas.action_id, cas.contact_id, cas.action_by, cas.action_on 
            , cas.action_method_id, cas.action_note, cas.bring_forward_date
        FROM contact_employer AS ce
        INNER JOIN contact_actions_set AS cas ON cas.contact_id=ce.contact_id
        WHERE ce.department_id = '" .$contact->department_id. "'
        AND cas.action_id != '". GENERIC_NOTE ."'
        ORDER BY cas.action_on DESC, cas.contact_actions_id DESC
        LIMIT 10
        ");
$result_actions = $GLOBALS['dbh']->Execute($sql_actions);

if ($result_actions->RecordCount())
{
    $departmentOutput .= ("</table>\n");
    $departmentOutput .= ("<hr />");
    $departmentOutput .= ("<table cellpadding='5' cellspacing='0' border='0' align='center'>\n");
    $departmentOutput .= ("<tr>");
    $departmentOutput .= ("<td colspan='6'>&nbsp;</td>");
    $departmentOutput .= ("</tr>\n");
    $departmentOutput .= ("<tr>");
    $departmentOutput .= ("<td align='center' colspan='6'>");
    $departmentOutput .= ("Ten Most Recent Actions for ".$contact->department_name.": ");
    $departmentOutput .= ("</td>");
    $departmentOutput .= ("</tr>\n");
    $departmentOutput .= ("<tr>");
    $departmentOutput .= ("<td align='center' colspan='6'>&nbsp;</td>");
    $departmentOutput .= ("</tr>\n");

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

        // get name of action_for
        $mini_sql = ("
                SELECT CONCAT(first_name,' ', last_name) AS name
                FROM contact
                WHERE contact_id='".$row_actions['contact_id']."'
                ");
        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
        $mini_row = $mini_result->FetchRow();
        $display_name_for = $mini_row['name'];

        // Display our query results
        $departmentOutput .= ("<tr>");
        // Display bring_forward_date in pop-up form if this action had one
        if(($row_actions["bring_forward_date"]!='0000-00-00') && ($row_actions["bring_forward_date"] != NULL) && ($row_actions["bring_forward_date"] > $todays_date))
        {
            $departmentOutput .= ("<td nowrap='nowrap'>");

            // pop-up function call
            $departmentOutput .= ("<a href='javascript:void(0);' onMouseOver=\"return overlib('".$row_actions['bring_forward_date']."', CAPTION, 'Bring Forward Date', LEFT, VAUTO, STICKY, ol_closeclick);\" onMouseOut=\"nd();\">");
            $departmentOutput .= ("<img border='0' src='misc/images/clock.gif' alt='' />");
            $departmentOutput .= ("</a>");
            $departmentOutput .= ("</td>\n");
        }
        else
        {
            $departmentOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
        }

        // Display note in pop-up form if this action had one
        if($row_actions["action_note"])
        {
            $departmentOutput .= ("<td nowrap='nowrap'>");
            $contact_action_note = trim($row_actions['action_note']);
            $contact_action_note = addslashes($contact_action_note);

            //  newlines, carriage returns, and double quotes passed into overlib() function must be changed to work properly
            $contact_action_note = str_replace("\"", "'", $contact_action_note);
            $contact_action_note = str_replace("\r", "", $contact_action_note);
            $contact_action_note = str_replace("\n", "<br />", $contact_action_note);

            // pop-up function call
            $departmentOutput .= ("<a href='javascript:void(0);' onMouseOver=\"return overlib('".$contact_action_note."', CAPTION, 'Action Note', LEFT, VAUTO, STICKY, ol_closeclick);\" onMouseOut=\"nd();\">");
            $departmentOutput .= ("<img border='0' src='misc/images/sticky_sm.gif' alt='' />");
            $departmentOutput .= ("</a>");
            $departmentOutput .= ("</td>\n");
        }
        else
        {
            $departmentOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
        }

        //date of action
        $departmentOutput .= ("<td nowrap='nowrap'>");
        if ($row_actions['action_on'] == '0000-00-00' || $row_actions['action_on'] == NULL)
        {
        $departmentOutput .= ("<b class='black'>On:</b> " . formatShortDate($row_actions["entered_on"]));
        } else {
        $departmentOutput .= ("<b class='black'>On:</b> " . formatShortDate($row_actions["action_on"]));
        }
        $departmentOutput .= ("</td>\n");
        $date[$count] = formatShortDate($row_actions["action_on"]);

        //action chosen
        $departmentOutput .= ("<td nowrap='nowrap'>");
        $departmentOutput .= ("<b class='black'>Action:</b> " . $action_name);
        $departmentOutput .= ("</TD\n>");

        //action performed by/for
        $departmentOutput .= ("<td nowrap='nowrap'>");
        $departmentOutput .= ("<b class='black'>For: </b>".$display_name_for);
        $departmentOutput .= ("</td>\n");

        // method of action (do not display method column if action is JOB_DESC_RECIEVED)
        $departmentOutput .= ("<td nowrap='nowrap'>");
        $departmentOutput .= (($row_actions["action_id"] == JOB_DESC_RECEIVED)? "&nbsp;" : "<b class='black'>Method:</b> " . $action_method_name);
        $departmentOutput .= ("</td>\n");
        $departmentOutput .= ("</tr>\n");
    }
    $departmentOutput .= ("</table>");
    $departmentOutput .= ("<table width='100%' border='0'>");
}

if ($contact->department_note_ids)
{
        $departmentOutput .= ("<tr>");
                $departmentOutput .= ("<td colspan='2'><hr /></td>");
        $departmentOutput .= ("</tr>");

	$departmentOutput .= ("<tr>");
	$departmentOutput .= ("<td colspan='2'>");
	$departmentOutput .= ("<table width='100%' border='0'>");

        foreach ($contact->department_note_ids AS $cni)
        {
                $sql = ("
                        SELECT DISTINCT *
                        FROM division_notes
                        WHERE note_id='" . $cni . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $name_array = getContactName($row["entered_by"]);
		$contact_name = ($row["entered_by"] ? $contact_name = $name_array["first_name"] . " " . $name_array["last_name"] : "&nbsp;");

		$departmentOutput .= ("<tr>");
			$departmentOutput .= ("<td align='left'>");
                        $departmentOutput .= ("<b>Note entered by: </b>");
			$departmentOutput .= ("</td>");
			$departmentOutput .= ("<td align='left'>");
			$departmentOutput .= ($contact_name);
			$departmentOutput .= ("</td>");
		$departmentOutput .= ("</tr>");
		$departmentOutput .= ("<tr>");
			$departmentOutput .= ("<td align='left'>");
                        $departmentOutput .= ("<b>Note entered on: </b>");
			$departmentOutput .= ("</td>");
			$departmentOutput .= ("<td align='left'>");
			$departmentOutput .= ($row["date_entered"]);
			$departmentOutput .= ("</td>");
		$departmentOutput .= ("</tr>");
		$departmentOutput .= ("<tr>");
			$departmentOutput .= ("<td align='left'>");
                        $departmentOutput .= ("<b>Note: </b>");
                        $departmentOutput .= ("</td>");
                        $departmentOutput .= ("<td align='left'>");

			$temp = split("\n", $row["notes"]);

			for ($i = 0; $i < sizeof($temp); $i++)
			{
				$departmentOutput .= ($temp[$i]);
				$departmentOutput .= ("<br />");
			}

                        $departmentOutput .= ("</td>");
		$departmentOutput .= ("</tr>");

		$departmentOutput .= ("<tr>");
			$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
		$departmentOutput .= ("</tr>");
        }

	$departmentOutput .= ("</table>");
	$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");

        $departmentOutput .= ("<tr>");
                $departmentOutput .= ("<td colspan='2'><hr /></td>");
        $departmentOutput .= ("</tr>");
}

/*
if ($contact->department_general_comments)
{
	$departmentOutput .= ("<tr>");
		$departmentOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Comments viewed<br />by all co-op staff:</b></td>");
		$departmentOutput .= ("<td><br />");
	
		$temp = split("\n", $contact->department_general_comments);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$departmentOutput .= ($temp[$i]);
			$departmentOutput .= ("<br />");
		}

		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");
	$departmentOutput .= ("<tr>");
		$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
	$departmentOutput .= ("</tr>");

}
*/

if ($contact->department_department_comments)
{
	$departmentOutput .= ("<tr>");
		$departmentOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Comments viewed by co-op<br />staff in your department:</b></td>");
		$departmentOutput .= ("<td><br />");

		$temp = split("\n", $contact->department_department_comments);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$departmentOutput .= ($temp[$i]);
			$departmentOutput .= ("<br />");
		}

		$departmentOutput .= ("</td>");
	$departmentOutput .= ("</tr>");
	$departmentOutput .= ("<tr>");
		$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
	$departmentOutput .= ("</tr>");

}

if ($contact->department_changes_recorded_1)
{

    $departmentOutput .= ("<tr>");
    $departmentOutput .= ("<td align='center' colspan='2'>");
                        $departmentOutput .= ("<b class='black'>Recent history of changes for this division:</b>");
                        $departmentOutput .= ("</td>");
                $departmentOutput .= ("</tr>");

                $departmentOutput .= ("<tr>");
                        $departmentOutput .= ("<td width='50%'>&nbsp;</td>");
                        $departmentOutput .= ("<td width='50%'>&nbsp;</td>");
                $departmentOutput .= ("</tr>");

                $departmentOutput .= ("<tr>");
                        $departmentOutput .= ("<td align='right'>");
                        $departmentOutput .= ("<b>Fields changed:</b>");
                        $departmentOutput .= ("</td>");
                        $departmentOutput .= ("<td align='left'>");
                        $departmentOutput .= ($contact->department_changes_recorded_1);
                        $departmentOutput .= ("</td>");
                $departmentOutput .= ("</tr>");

                $departmentOutput .= ("<tr>");
                        $departmentOutput .= ("<td align='right'>");
                        $departmentOutput .= ("<b>Changes above made on:</b>");
                        $departmentOutput .= ("</td>");
                        $departmentOutput .= ("<td align='left'>");
                        $departmentOutput .= (formatLongDate($contact->department_change_date_1));
                        $departmentOutput .= ("</td>");
                $departmentOutput .= ("</tr>");

                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                        FROM contact AS a, department AS b, contact_internal AS c
                        WHERE a.contact_id='" . $contact->department_change_by_1 . "'
                        AND c.contact_id=a.contact_id
                        AND b.department_id=c.department_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                $departmentOutput .= ("<tr>");
                        $departmentOutput .= ("<td align='right'>");
                        $departmentOutput .= ("<b>Changes above made by:</b>");
                        $departmentOutput .= ("</td>");
                        $departmentOutput .= ("<td align='left'>");
                        $departmentOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                        $departmentOutput .= ("</td>");
                $departmentOutput .= ("</tr>");

                if ($contact->department_changes_recorded_2)
                {
                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $departmentOutput .= ("</tr>");

                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td align='right'>");
				$departmentOutput .= ("<b>Fields changed:</b>");
                                $departmentOutput .= ("</td>");
                                $departmentOutput .= ("<td align='left'>");
                                $departmentOutput .= ($contact->department_changes_recorded_2);
                                $departmentOutput .= ("</td>");
                        $departmentOutput .= ("</tr>");

                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td align='right'>");
                                $departmentOutput .= ("<b>Changes above made on:</b>");
                                $departmentOutput .= ("</td>");
                                $departmentOutput .= ("<td align='left'>");
                                $departmentOutput .= (formatLongDate($contact->department_change_date_2));
                                $departmentOutput .= ("</td>");
                        $departmentOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
                                WHERE a .contact_id='" . $contact->department_change_by_2 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td align='right'>");
                                $departmentOutput .= ("<b>Changes above made by:</b>");
                                $departmentOutput .= ("</td>");
                                $departmentOutput .= ("<td align='left'>");
                                $departmentOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $departmentOutput .= ("</td>");
                        $departmentOutput .= ("</tr>");
                }
                if ($contact->department_changes_recorded_3)
                {
                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $departmentOutput .= ("</tr>");

                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td align='right'>");
                                $departmentOutput .= ("<b>Fields changed:</b>");
                                $departmentOutput .= ("</td>");
                                $departmentOutput .= ("<td align='left'>");
                                $departmentOutput .= ($contact->department_changes_recorded_3);
                                $departmentOutput .= ("</td>");
                        $departmentOutput .= ("</tr>");

                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td align='right'>");
                                $departmentOutput .= ("<b>Changes above made on:</b>");
                                $departmentOutput .= ("</td>");
                                $departmentOutput .= ("<td align='left'>");
                                $departmentOutput .= (formatLongDate($contact->department_change_date_3));
                                $departmentOutput .= ("</td>");
                        $departmentOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
				WHERE a .contact_id='" . $contact->department_change_by_3 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $departmentOutput .= ("<tr>");
                                $departmentOutput .= ("<td align='right'>");
                                $departmentOutput .= ("<b>Changes above made by:</b>");
                                $departmentOutput .= ("</td>");
                                $departmentOutput .= ("<td align='left'>");
                                $departmentOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $departmentOutput .= ("</td>");
                        $departmentOutput .= ("</tr>");
                }
}

/*
 And that's the end of the departmentOutput variable.  Now we must form the formEnd variable, which will be displayed
 on this screen, but not when viewing the information in PDF.
*/

if (!$no_buttons)
{
	/*
	 Even though the variables below say HTMLCompany and companyOutput, this is simply to allow for reuse of code.
	 We're actually transmitting the division information to be viewed in PDF.
	*/
	
	$formEnd .= ("
		<center>
		<table border='0' width='96%'>
	        <tr>
	        <td align='left'>
	        <form method='post' action='" . $PHP_SELF . "'>
	        <input type='hidden' name='select' value='view_contact' />
	        <input type='hidden' name='level1' value='company' />
	        <input type='hidden' name='continue' value='view_specific_department' />
	        <input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
	        <input type='hidden' name='department_id_list' value='" . $department_id_list . "' />
	        <input type='hidden' name='department_id' value='" . $left_id . "' />
            <input type='hidden' name='companies_match' value='" . $companies_match . "' />
            <input type='hidden' name='show_quick' value='true' />
	        <input type='image' src='misc/images/previous.gif' />
	        </form>
	        </td>
	        <td align='center'>
	        <form method='post' action='" . $PHP_SELF . "'>
	        <input type='hidden' name='select' value='view_contact' />
	        <input type='hidden' name='level1' value='company' />
	        <input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
            <input type='hidden' name='continue' value='back_to_department_list' />
	        <input type='submit' value='Back to List' />
	        </form>
	        </td>
	        <td align='right'>
	        <form method='post' action='" . $PHP_SELF . "'>
	        <input type='hidden' name='select' value='view_contact' />
	        <input type='hidden' name='level1' value='company' />
	        <input type='hidden' name='continue' value='view_specific_department' />
	        <input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
	        <input type='hidden' name='department_id_list' value='" . $department_id_list . "' />
	        <input type='hidden' name='department_id' value='" . $right_id . "' />
            <input type='hidden' name='companies_match' value='" . $companies_match . "' />
            <input type='hidden' name='show_quick' value='true' />
	        <input type='image' src='misc/images/next.gif' />
	        </form>
	        </td>
	        </tr>
	        </table>
		</center>
	        ");
}

echo($formStart . $departmentOutput . $departmentEnd . $formEnd);

?>
