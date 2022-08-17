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
 | Filename: confirm_contact_info.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the inputs for a contact.   |
 |  The form can be displayed without buttons, in which case the user is given  |
 | the option of viewing the information in PDF (for easy printing).            |
 +------------------------------------------------------------------------------+

*/

/*
 Include the file that sets up the variables for PDF conversion.
*/

instruct("Below is a preview of the contact information that you have entered.  Please ensure that all information is correct before continuing to the Company Information 
	Form.<br /><br />If you would like to make a correction, click on the 'Edit Inputs' button and your Contact Information Form will be reloaded with your inputs 
	displayed for you to edit.");

echo("<center>");
echo("<table border='1' cellpadding='4' cellspacing='0' width='100%'>");
echo("<tr>");
echo("<td>");
echo("<table border='0' align='center' width='100%'>");
echo("<tr>");
	echo("<td align='center' colspan='2'>");
    echo("<br />");
	echo("<h3>");
	if ($newEmployer->contact_title)
	{
		echo(getTitleName($newEmployer->contact_title) . " ");
	}
	echo($newEmployer->contact_first_name . " ");
	if ($newEmployer->contact_middle_name)
	{
		echo($newEmployer->contact_middle_name . " ");
	}
	echo($newEmployer->contact_last_name);
	echo("</h3>");
	echo("</td>");
echo("</tr>");

echo("<tr align='center'>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($newEmployer->contact_title)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Title:</b></td>");
		echo("<td width='50%' align='left'>");
		echo(getTitleName($newEmployer->contact_title));
		echo("</td>");
	echo("</tr>");
}

echo("<tr align='center'>");
	echo("<td align='right' width='50%' nowrap='nowrap'><b>Name:</b></td>");
	echo("<td width='50%' align='left'>");
	echo($newEmployer->contact_first_name . " ");
	if ($newEmployer->contact_middle_name)
	{
		echo($newEmployer->contact_middle_name . " ");
	}
	echo($newEmployer->contact_last_name);
	echo("</td>");
echo("</tr>");

if ($newEmployer->contact_called_name)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Preferred Name:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($newEmployer->contact_called_name);
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->contact_phone)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Phone:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($newEmployer->contact_phone);
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->contact_fax)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Fax:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($newEmployer->contact_fax);
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->contact_pager)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Pager:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($newEmployer->contact_pager);
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->contact_cellphone)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Cellphone:</b></td>");
		echo("<td width='50%' align='left'>");
		echo($newEmployer->contact_cellphone);
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->contact_email)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>E-mail:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->contact_email . "</td>");
	echo("</tr>");
}

echo("<tr align='center'>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if ($newEmployer->contact_street_address_1) 
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Contact's Street Address:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->contact_street_address_1);
		if ($newEmployer->contact_street_address_2)
		{
			echo("<br />" . $newEmployer->contact_street_address_2);
		}
		if ($newEmployer->contact_street_address_3)
		{
			echo("<br />" . $newEmployer->contact_street_address_3);
		}
		echo("</td>");
	echo("</tr>");
}

if ($newEmployer->contact_country_id)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Country:</b></td>");
		echo("<td width='50%' align='left'>" . getCountryName($newEmployer->contact_country_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->contact_provstate_id)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Province/State:</b></td>");
		echo("<td width='50%' align='left'>" . getProvstateName($newEmployer->contact_provstate_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->contact_region_id)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Region:</b></td>");
		echo("<td width='50%' align='left'>" . getRegionName($newEmployer->contact_region_id) . "</td>");
	echo("</tr>");
}

if ($newEmployer->contact_city)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>City:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->contact_city . "</td>");
	echo("</tr>");
}

if ($newEmployer->contact_postal_code)
{
	echo("<tr align='center'>");
		echo("<td align='right' width='50%' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		echo("<td width='50%' align='left'>" . $newEmployer->contact_postal_code . "</td>");
	echo("</tr>");
}

echo("<tr align='center'>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
        echo("<td align='center' colspan='2'>");
        echo("<form method='post' action='" . $PHP_SELF . "'>");
        echo("<input type='hidden' name='select' value='new_employer' />");
        echo("<input type='hidden' name='newEmployer' value='" . packObject($newEmployer) . "' />");
        echo("<input type='hidden' name='inputsFor' value='contact' />");

        echo("<input type='hidden' name='continue' value='save_contact_information' />");
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
