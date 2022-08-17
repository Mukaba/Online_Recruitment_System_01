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
 | Filename: confirm_company_inputs.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the attributes for a        |
 | company.  The form can be displayed without buttons, in which case the user  |
 | is given the option of viewing the information in PDF (for easy printing).   |
 +------------------------------------------------------------------------------+

*/

/*
 Next is the last part of the screen that will be displayed.  We declare it now to get it out of the way.
 If we are showing form buttons, then this needs to be added after those buttons have been displayed, but
 if we are viewing in PDF, it needs to be added before the buttons (actually, the buttons aren't even
 displayed when viewing in PDF).
*/

$companyEnd .= ("</table>");
$companyEnd .= ("</td>");
$companyEnd .= ("</tr>");
$companyEnd .= ("</table>");
$companyEnd .= ("</center>");

/*
 And now for the rest of the company's information.  This is saved to a variable called companyOutput, which
 is then printed to the screen.
*/

if (!$no_buttons)
{
	instruct("Below are the inputs that you have specified.  Please check to make sure they are correct, and then click
		'Add Company Now' at the bottom of the page to finish adding this company.");
}

$companyOutput .= ("<center>");
$companyOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='100%'>");
$companyOutput .= ("<tr>");
$companyOutput .= ("<td>");
$companyOutput .= ("<table border='0' align='center' width='100%'>");
$companyOutput .= ("<tr>");
	$companyOutput .= ("<td align='center' colspan='2'>");
		$companyOutput .= ("<h3>");
		$companyOutput .= ($contact->company_name);
		$companyOutput .= ("<br /></h3>");
		$companyOutput .= ("<hr />");
	$companyOutput .= ("</td>");
$companyOutput .= ("</tr>");

$companyOutput .= ("<tr>");
	$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
$companyOutput .= ("</tr>");

if ($contact->company_status)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Company Status:</b></td>");
		$companyOutput .= ("<td align='left' width='50%'>");
		$companyOutput .= (getEmployerStatusName($contact->company_status));
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

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
		$companyOutput .= ("<td width='50%' align='left'>" . $contact->company_email . "</td>");
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

if ($contact->company_type_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Type:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getCompanyTypeName($contact->company_type_id) . "</td>");
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


if ($contact->company_description)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Company Description:</b></td>");
		$companyOutput .= ("<td>");
		
		$temp = split("\n", $contact->company_description);

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

if ($contact->company_new_note)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Notes viewed<br />by all co-op staff:</b></td>");
		$companyOutput .= ("<td><br />");
	
		$temp = split("\n", $contact->company_new_note);

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

if ($contact->company_department_comments)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Comments viewed by co-op<br />staff in your department:</b></td>");
		$companyOutput .= ("<td><br />");

		$temp = split("\n", $contact->company_department_comments);

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

/*
 And that's the end of the companyOutput variable.  Now we must form the formEnd variable, which will be displayed
 on this screen, but not when viewing the information in PDF.
*/

if (!$no_buttons)
{
	$formEnd .= ("
            <tr>
            <td colspan='2'>
            <form method='post' action='" . $PHP_SELF . "'>
            <input type='hidden' name='select' value='add_contact' />
            <input type='hidden' name='contact' value='" . packObject($contact) . "' />
            <input type='hidden' name='level1' value='company' />
            <table border='0' width='100%'>
            <tr align='center'>
            <td width='50%'>
            <input type='submit' name='continue' value='Add Company Now' />
            </td>
            <td>
            <input type='submit' name='continue' value='Edit Company' />
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
    // Display a quickmenu, since they aren't showing form buttons.
    echo("<table border='0' width='100%'>");
    echo("<tr>");
    echo("<td align='left'>");
    echo("<h3 align='left'>Employer Information - Add a New Company");
    echo("</h3>");
    echo("</td>");
    // Include the quickmenu
    echo("<td align='right'>");
    include('contact/company_quickmenu.inc');
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2'>");
    $msg = ("Company: " . $contact->company_name . " has been successfully saved into the database.  In addition, a division of the same name has been saved ");
    $msg .= ("under this company using the same inputs.");
    notify($msg);
		echo("</td>");
	echo("</tr>");

	echo("</table>");
}


echo($companyOutput . $formEnd . $companyEnd);
