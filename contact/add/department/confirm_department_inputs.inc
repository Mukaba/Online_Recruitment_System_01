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
 | Filename: confirm_department_inputs.inc                                      |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the attributes for a        |
 | division.  The form can be displayed without buttons                         |
 +------------------------------------------------------------------------------+

*/


$sql = ("
	SELECT DISTINCT company_name, city
	FROM employer_company
	WHERE employer_id='" . $contact->department_company_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$parent_company = $row["company_name"];
$parent_company_city = $row["city"];

/*
 Next is the last part of the screen that will be displayed.  We declare it now to get it out of the way.
 If we are showing form buttons, then this needs to be added after those buttons have been displayed.
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

if (!$no_buttons)
{
        instruct("Below are the inputs that you have specified.  Please check to make sure they are correct, and then click
                'Add Division Now' at the bottom of the page to finish adding this division.");
}


$departmentOutput .= ("<center>");
$departmentOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='100%'>");
$departmentOutput .= ("<tr>");
$departmentOutput .= ("<td>");
$departmentOutput .= ("<table border='0' align='center' width='100%'>");
$departmentOutput .= ("<tr>");
	$departmentOutput .= ("<td align='center' colspan='2'>");
	$departmentOutput .= ("<h3>");
	$departmentOutput .= ($contact->department_name);
	$departmentOutput .= ("<br /></h3>");
	$departmentOutput .= ("</td>");
$departmentOutput .= ("</tr>");

$employer_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_company&amp;show_quick=true&amp;no_buttons=true&amp;employer_id=");

$departmentOutput .= ("<tr>");
	$departmentOutput .= ("<td align='center' colspan='2'>");
	$departmentOutput .= ("<b class='black'>Parent Company:&nbsp;</b>");
	$departmentOutput .= ("<a class='blue' href='" . $employer_href . $contact->department_company_id . "'>" . $parent_company . "</a>");
	$departmentOutput .= (($parent_company_city) ? "&nbsp;(" . $parent_company_city . ")" : "");
	$departmentOutput .= ("<br /><br />");
	$departmentOutput .= ("<hr />");
	$departmentOutput .= ("</td>");
$departmentOutput .= ("</tr>");

$departmentOutput .= ("<tr align='center'>");
	$departmentOutput .= ("<td colspan='2'>&nbsp;</td>");
$departmentOutput .= ("</tr>");

if ($contact->department_status)
{
        $departmentOutput .= ("<tr align='center'>");
                $departmentOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Division Status:</b></td>");
                $departmentOutput .= ("<td align='left' width='50%'>");
                $departmentOutput .= (getEmployerStatusName($contact->department_status));
                $departmentOutput .= ("</td>");
        $departmentOutput .= ("</tr>");
}

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
		$departmentOutput .= ("<td width='50%' align='left'>" . $contact->department_email . "</td>");
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

if ($contact->department_new_note)
{
	$departmentOutput .= ("<tr>");
		$departmentOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Notes viewed<br />by all co-op staff:</b></td>");
		$departmentOutput .= ("<td><br />");
	
		$temp = split("\n", $contact->department_new_note);

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


/*
 And that's the end of the departmentOutput variable.  Now we must form the formEnd variable, which will be displayed
 on this screen.
*/

if (!$no_buttons)
{
	$formEnd .= ("
		<tr>
		<td colspan='2'>
        <form method='post' action='" . $PHP_SELF . "'>
        <input type='hidden' name='select' value='add_contact' />
        <input type='hidden' name='contact' value='" . packObject($contact) . "' />
        <input type='hidden' name='level1' value='department' />

		<table border='0' width='100%'>
		<tr align='center'>
		<td width='50%'>
		<input type='submit' name='continue' value='Add Division Now' />
		</td>
		<td>
		<input type='submit' name='continue' value='Edit Division' />
		<input type='hidden' name='employer_id' value='" . $contact->department_company_id . "' />
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
                echo("<h3 align='left'>Employer Information - Add a New Division");
                echo("</h3>");
            echo("</td>");
            // Include the quickmenu
            echo("<td align='right'>");
                include('contact/division_quickmenu.inc');
            echo("</td>");
        echo("</tr>");
        echo("<tr>");
            echo("<td colspan='2'>");

		// Get the company name that this division was just added to.

            $sql = ("
                SELECT DISTINCT company_name
                FROM employer_company
                WHERE employer_id='" . $contact->department_company_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

		notify("Division: " . $contact->department_name . " has been successfully added to " . $row["company_name"] . ".");
                echo("</td>");
        echo("</tr>");

        echo("</table>");
}

echo($departmentOutput . $formEnd . $departmentEnd);
