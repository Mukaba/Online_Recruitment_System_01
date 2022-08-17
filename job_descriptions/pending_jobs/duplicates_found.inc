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
 | Filename: duplicates_found.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This file is called when we have found similar companies        |
 | and/or contacts in the database upon a user approving a recently entered     |
 | contact.  One of $similar_company_contacts, $similar_companies, and          |
 | $similar_contacts must have been set by the file that calls this one.        |
 +------------------------------------------------------------------------------+

*/

assert($similar_company_contacts || $similar_companies || $similar_contacts);

$sql = ("
	SELECT DISTINCT pec.company_name, CONCAT(pecn.first_name, ' ', pecn.last_name) AS contact_name
	FROM pending_employer_company AS pec, pending_employer_contact AS pecn, pending_job_description AS pjd
	WHERE pjd.job_id='" . addslashes($job_id) . "'
	AND pec.employer_id=pjd.employer_id
	AND pecn.contact_id=pjd.contact_id
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

notify("Below is a list of companies and contacts already existant in the database that are similar to the ones that
	you have just approved (<b>" . $row["contact_name"] . "</b> at <b>" . $row["company_name"] . "</b>).  Please check 
	to make sure that you are not entering a duplicate company and/or contact, choose one of the five options below, 
	and then click Continue.<br /><br />You may click the names of the companies and contacts to have a new window open
	with that company or contact's details displayed.");

if ($similar_company_contacts)
{
	echo("<br />");

	echo("<h3>Similar company and contact pairs:</h3>");
	
	echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
	echo("<td>");
	
	echo("<table border='0' cellpadding='2'>");
	
	echo("<tr>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name&nbsp;</b></td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Contact Name&nbsp;</b></td>");
	echo("</tr>");

	$company_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_details&amp;no_buttons=true&amp;employer_id=";
	$contact_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;no_buttons=true&amp;contact_id=";
	
	$rowclass = 0;
	
	foreach ($similar_company_contacts AS $scc)
	{
		echo("<tr>");
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' target='company_popup' href='" . $company_href . $scc["employer_id"] . "'>" . $scc["company_name"] . "</a>&nbsp;");
			echo("</td>\n");
		
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' target='contact_popup' href='" . $contact_href . $scc["contact_id"] . "'>" . $scc["contact_name"] . "</a>&nbsp;");
			echo("</td>\n");
		echo("</tr>");

		$rowclass++;
	}
	
	echo("</table>");
	
	echo("</td>");
	echo("</tr>");
	echo("</table>");
}

if ($similar_companies)
{
	echo("<br />");
	
	echo("<h3>Similar companies:</h3>");

	echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
	echo("<td>");

	echo("<table border='0' cellpadding='2'>");

	echo("<tr>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name&nbsp;</b></td>");
	echo("</tr>");

	$rowclass = 0;

	$company_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_details&amp;no_buttons=true&amp;employer_id=";

	foreach ($similar_companies AS $sc)
	{
		echo("<tr>");
			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' target='company_popup' href='" . $company_href . $sc["employer_id"] . "'>" . $sc["company_name"] . "</a>&nbsp;");
			echo("</td>\n");
		echo("</tr>");

		$rowclass++;
	}

	echo("</table>");

	echo("</td>");
	echo("</tr>");
	echo("</table>");
}

if ($similar_contacts)
{
	echo("<br />");

        echo("<h3>Similar contacts:</h3>");

        echo("<table cellspacing='0' cellpadding='0' border='1'>");
        echo("<tr>");
        echo("<td>");

        echo("<table border='0' cellpadding='2'>");

        echo("<tr>");
                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name&nbsp;</b></td>");
		echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Contact Name&nbsp;</b></td>");
        echo("</tr>");

        $rowclass = 0;

	$company_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_details&amp;no_buttons=true&amp;employer_id=";
	$contact_href = $PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;no_buttons=true&amp;contact_id=";

        foreach ($similar_contacts AS $sc)
        {
                echo("<tr>");
                        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' target='company_popup' href='" . $company_href . $sc["employer_id"] . "'>" . $sc["company_name"] . "</a>&nbsp;");
                        echo("</td>\n");

			echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("&nbsp;<a class='blue' target='contact_popup' href='" . $contact_href . $sc["contact_id"] . "'>" . $sc["contact_name"] . "</a>&nbsp;");
			echo("</td>\n");
                echo("</tr>");

                $rowclass++;
        }

        echo("</table>");

        echo("</td>");
        echo("</tr>");
        echo("</table>");
}

echo("<br />");

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");

echo("<table border='0'>");

echo("<tr>");
    echo("<td>");
    echo("<input type='radio' name='continue' value='oldCompany_newContact' />");
    echo("</td>");
    echo("<td align='left'>");
    echo("The approved company is already in the database, but the contact is new and should be added to this company.");
    echo("</td>");
echo("</tr>");

/* 

Commented out by: Chan
Date: 2004-11-29
Reason: This path allows a new company to be created (without a division), and a contact is put directly under the company.
        It's a problem because we assume every contact has a division. 

echo("<tr>");
    echo("<td>");
    echo("<input type='radio' name='continue' value='newCompany_oldContact' />");
    echo("</td>");
    echo("<td align='left'>");
    echo("The approved company is new, but the contact needs to be moved from an old company to the new one.");
    echo("</td>");
echo("</tr>");
*/

echo("<tr>");
    echo("<td>");
    echo("<input type='radio' name='continue' value='oldCompany_oldContact' />");
    echo("</td>");
    echo("<td align='left'>");
    echo("The approved company and contact are both already in the database but I would like to update their info.");
    echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='continue' value='newCompany_newContact' />");
	echo("</td>");
	echo("<td align='left'>");
	echo("The approved company and contact are both new.  Continue adding them as before.");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td>");
	echo("<input type='radio' name='continue' value='Cancel' />");
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
?>
