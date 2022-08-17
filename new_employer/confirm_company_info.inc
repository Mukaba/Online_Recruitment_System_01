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
 | Filename: confirm_company_info.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the attributes for a        |
 | company.  The form can be displayed without buttons, in which case the user  |
 | is given the option of viewing the information in PDF (for easy printing).   |
 +------------------------------------------------------------------------------+

*/

instruct("Below is a preview of the company information that you have entered.  Please ensure that all information is correct 
	before continuing to the Job Description Form.<br /><br />If you would like to make a correction, click on the 'Edit Inputs' 
	button and your Company Information Form will be reloaded with your inputs displayed for you to edit.");

echo("<center>");
echo("<table border='1' cellpadding='4' cellspacing='0' width='100%'>");
echo("<tr>");
echo("<td>");
echo("<table border='0' align='center' width='100%'>");
echo("<tr>");
	echo("<td align='center' colspan='2'>");
        echo("<br />");
		echo("<h3>");
		echo($newEmployer->company_name);
		echo("</h3>");
		echo("<hr />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($newEmployer->company_phone)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Company Phone:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($newEmployer->company_phone);
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->company_fax)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Fax:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($newEmployer->company_fax);
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->company_email)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Company E-mail:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->company_email . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_website)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Website:</b></td>");
		echo("<td width='50%' align='left'>");
		echo("<a class='blue' target='company_website' href='http://" . stripSlashes($newEmployer->company_website) . "'>" . stripSlashes($newEmployer->company_website) . "</a>");
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->company_size_id)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Size:</b></td>");
		echo("<td width='50%' align='left'>" . getSizeRange($newEmployer->company_size_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_type_id)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Compay Type:</b></td>");
		echo("<td width='50%' align='left'>" . getCompanyTypeName($newEmployer->company_type_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_industry_id)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Company Industry:</b></td>");		
		echo("<td width='50%' align='left'>" . getIndustryName($newEmployer->company_industry_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_department_name)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Department Name:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->company_department_name . "</td>");
	echo("</tr>");
}

echo("<tr align='center'>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($newEmployer->company_street_address_1)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Street Address:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->company_street_address_1);
		if ($newEmployer->company_street_address_2)
		{
			echo("<br />" . $newEmployer->company_street_address_2);
		}
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->company_country_id)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Country:</b></td>");
		echo("<td width='50%' align='left'>" . getCountryName($newEmployer->company_country_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_provstate_id)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Province/State:</b></td>");
		echo("<td width='50%' align='left'>" . getProvstateName($newEmployer->company_provstate_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_region_id)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Region:</b></td>");
		echo("<td width='50%' align='left'>" . getRegionName($newEmployer->company_region_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_city)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>City:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->company_city . "</td>");
	echo("</tr>");
}

if ($newEmployer->company_postal_code)
{
	echo("<tr align='center'>");
		echo("<td width='50%' align='right' nowrap='nowrap'><b>Postal Code:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->company_postal_code . "</td>");
	echo("</tr>");
}


echo("<tr align='center'>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($newEmployer->company_description)
{
	echo("<tr>");
		echo("<td valign='top' align='right' nowrap='nowrap'><b>Company Description:</b></td>");
		echo("<td>");
		
		$temp = split("\n", $newEmployer->company_description);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			echo($temp[$i]);
			echo("<br />");
		}

		echo("</td>");
	echo("</tr>");
	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

}

echo("<tr>");
	echo("<td align='center' colspan='2'>");
    echo("<form method='post' action='" . $PHP_SELF . "'>");
    echo("<input type='hidden' name='select' value='new_employer' />");
    echo("<input type='hidden' name='newEmployer' value='" . packObject($newEmployer) . "' />");
	echo("<input type='hidden' name='continue' value='save_company_information' />");
	echo("<input type='submit' value='Continue' />");
	echo("&nbsp;&nbsp;");
	echo("<input type='submit' name='continue' value='Edit Inputs' />");
    echo("</form>");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</center>");

?>
