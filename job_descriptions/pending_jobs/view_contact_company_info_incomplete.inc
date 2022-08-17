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
 | Filename: view_contact_company_info_incomplete.inc                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the inputs for a contact    |
 | and company that are pending approval.                                       |
 +------------------------------------------------------------------------------+

*/

$msg = ("This contact (and company if applicable) did not finish entering all of the information required to post a job.");
$msg .= ("  If you like, you may print out the information below and contact this person to gather the remaining information.");
$msg .= ("  When you are done, please hit the 'Delete Incomplete Information' button below.");
notify($msg);

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='contact_id' value='" . $pendingEmployer->contact_id . "' />");

echo("<center>");
echo("<table border='1' cellpadding='4' cellspacing='0' width='100%'>");
echo("<tr>");
echo("<td>");
echo("<table border='0' align='center' width='100%'>");
echo("<br />");
echo("<tr>");
	echo("<td align='center' colspan='2'>");
	echo("<h3>");
	if ($pendingEmployer->contact_title)
	{
		echo(getTitleName($pendingEmployer->contact_title) . " ");
	}
	echo($pendingEmployer->contact_first_name . " ");
	if ($pendingEmployer->contact_middle_name)
	{
		echo($pendingEmployer->contact_middle_name . " ");
	}
	echo($pendingEmployer->contact_last_name);
	echo("<br /></h3>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2' align='center'>");
	echo("<hr />");
	echo("</td>");
echo("</tr>");

echo("<tr align='center'>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($pendingEmployer->contact_title)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Title:</b></td>");
		echo("<td width='50%' align='left'>");
		echo(getTitleName($pendingEmployer->contact_title));
		echo("</td>");
	echo("</tr>");
}

echo("<tr align='center'>");
	echo("<td align='right' width='50%' nowrap='nowrap'><b>Name:</b></td>");
	echo("<td width='50%' align='left'>");
	echo($pendingEmployer->contact_first_name . " ");
	if ($pendingEmployer->contact_middle_name)
	{
		echo($pendingEmployer->contact_middle_name . " ");
	}
	echo($pendingEmployer->contact_last_name);
	echo("</td>");
echo("</tr>");

if ($pendingEmployer->contact_called_name)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Preferred Name:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($pendingEmployer->contact_called_name);
		echo("</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_phone)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Phone:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($pendingEmployer->contact_phone);
		echo("</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_fax)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Fax:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($pendingEmployer->contact_fax);
		echo("</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_pager)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Pager:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($pendingEmployer->contact_pager);
		echo("</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_cellphone)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Cellphone:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($pendingEmployer->contact_cellphone);
		echo("</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_email)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>E-mail:</b></td>");
		echo("<td width='50%' align='left'>" . $pendingEmployer->contact_email . "</td>");
	echo("</tr>");
}

echo("<tr align='center'>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($pendingEmployer->contact_street_address_1) 
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Street Address:</b></td>");
		echo("<td width='50%' align='left'>" . $pendingEmployer->contact_street_address_1);
		if ($pendingEmployer->contact_street_address_2)
		{
			echo("<br />" . $pendingEmployer->contact_street_address_2);
		}
		if ($pendingEmployer->contact_street_address_3)
		{
			echo("<br />" . $pendingEmployer->contact_street_address_3);
		}
		echo("</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_country_id)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Country:</b></td>");
		echo("<td width='50%' align='left'>" . getCountryName($pendingEmployer->contact_country_id) . "</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_provstate_id)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Province/State:</b></td>");
		echo("<td width='50%' align='left'>" . getProvstateName($pendingEmployer->contact_provstate_id) . "</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_region_id)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Region:</b></td>");
		echo("<td width='50%' align='left'>" . getRegionName($pendingEmployer->contact_region_id) . "</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_city)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>City:</b></td>");
		echo("<td width='50%' align='left'>" . $pendingEmployer->contact_city . "</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_postal_code)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		echo("<td width='50%' align='left'>" . $pendingEmployer->contact_postal_code . "</td>");
	echo("</tr>");
}

if ($pendingEmployer->contact_department_name)
{
	echo("<tr>");
		echo("<td width='50%' align='center'>&nbsp;</td>");
		echo("<td width='50%' align='center'>&nbsp;</td>");
	echo("</tr>");
	
        echo("<tr align='center'>");
                echo("<td width='50%' align='right' nowrap='nowrap'><b>Contact's Department:</b></td>");
                echo("<td width='50%' align='left'>" . $pendingEmployer->contact_department_name . "</td>");
        echo("</tr>");
}

echo("<tr align='center'>");
	echo("<td colspan='2' align='center'>");
	echo("<hr />");
	echo("</td>");
echo("</tr>");

if ($pendingEmployer->employer_id)
{
	echo("<tr>");
		echo("<td align='center' colspan='2'>");
			echo("<br /><h3>");
			echo($pendingEmployer->company_name);
			echo("<br /></h3>");
			echo("<hr />");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	if ($pendingEmployer->company_phone)
	{
		echo("<tr align='center'>");
			echo("<td align='right' width='50%' nowrap='nowrap'><b>Company Phone:</b></td>");
			echo("<td width='50%' align='left'>");
			echo($pendingEmployer->company_phone);
			echo("</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_fax)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Fax:</b></td>");
			echo("<td width='50%' align='left'>");
			echo($pendingEmployer->company_fax);
			echo("</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_email)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Company E-mail:</b></td>");
			echo("<td width='50%' align='left'>" . $pendingEmployer->company_email . "</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_website)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Website:</b></td>");
			echo("<td width='50%' align='left'>");
			echo("<a class='blue' target='company_website' href='http://" . stripSlashes($pendingEmployer->company_website) . "'>" . stripSlashes($pendingEmployer->company_website) . "</a>");
			echo("</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_size_id)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Size:</b></td>");
			echo("<td width='50%' align='left'>" . getSizeRange($pendingEmployer->company_size_id) . "</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_type_id)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Type:</b></td>");
			echo("<td width='50%' align='left'>" . getCompanyTypeName($pendingEmployer->company_type_id) . "</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_industry_id)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Industry:</b></td>");
			echo("<td width='50%' align='left'>" . getIndustryName($pendingEmployer->company_industry_id) . "</td>");
		echo("</tr>");
	}

	echo("<tr align='center'>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	if ($pendingEmployer->company_street_address_1)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Street Address:</b></td>");
			echo("<td width='50%' align='left'>" . $pendingEmployer->company_street_address_1);
			if ($pendingEmployer->company_street_address_2)
			{
				echo("<br />" . $pendingEmployer->company_street_address_2);
			}
			echo("</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_country_id)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Country:</b></td>");
			echo("<td width='50%' align='left'>" . getCountryName($pendingEmployer->company_country_id) . "</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_provstate_id)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Province/State:</b></td>");
			echo("<td width='50%' align='left'>" . getProvstateName($pendingEmployer->company_provstate_id) . "</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_region_id)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Region:</b></td>");
			echo("<td width='50%' align='left'>" . getRegionName($pendingEmployer->company_region_id) . "</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_city)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>City:</b></td>");
			echo("<td width='50%' align='left'>" . $pendingEmployer->company_city . "</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->company_postal_code)
	{
		echo("<tr align='center'>");
			echo("<td width='50%' align='right' nowrap='nowrap'><b>Postal Code:</b></td>");
			echo("<td width='50%' align='left'>" . $pendingEmployer->company_postal_code . "</td>");
		echo("</tr>");
	}


	echo("<tr align='center'>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	if ($pendingEmployer->company_description)
	{
		echo("<tr>");
			echo("<td valign='top' align='right' nowrap='nowrap'><b>Company Description:</b></td>");
			echo("<td>");

			echo(nl2br($pendingEmployer->company_description));

			echo("</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>");

	}
}

echo("<tr>");
        echo("<td align='center' colspan='2'>");
        echo("<input type='submit' name='continue' value='Delete Incomplete Information' />");
        echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");
echo("</center>");
echo("</table>");

?>
