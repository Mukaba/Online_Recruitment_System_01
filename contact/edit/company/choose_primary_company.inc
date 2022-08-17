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
 | Filename: choose_primary_company.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file allows the user to choose which company is the        |
 | primary company, and point all of the contacts under the old non-primary     |
 | company towards the new one.                                                 |
 +------------------------------------------------------------------------------+

*/


$sql = ("
	SELECT DISTINCT company_name
	FROM employer_company
	WHERE employer_id='" . $first_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$first_company_name = $row["company_name"];

$sql = ("
	SELECT DISTINCT company_name
	FROM employer_company
	WHERE employer_id='" . $second_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$second_company_name = $row["company_name"];

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_contact' />");
echo("<input type='hidden' name='level1' value='company' />");
echo("<input type='hidden' name='first_employer_id' value='" . $first_employer_id . "' />");
echo("<input type='hidden' name='second_employer_id' value='" . $second_employer_id . "' />");
echo("<input type='hidden' name='continue' value='merge_information_chosen' />");

echo("<table border='0' cellpadding='0' cellspacing='0' width='97%' class='row1'>");

echo("<tr>");
        echo("<td colspan='2'>");
        echo("<br />");
        echo("</td>");
echo("</tr>");

echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
        echo("<tr>");
                echo("<td colspan='2' class='row0'>");
                echo("<table width='97%' cellpadding='2' cellspacing='0' border='0'>");
		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo("You have chosen to merge <b>" . $first_company_name . "</b> and <b>" . $second_company_name . "</b> together.");
			echo("  Please choose which of these two companies will be the primary company.  All contacts associated with the secondary ");
			echo("company will now be associated with the primary company after the merge.</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

                echo("<tr>");
                        echo("<td colspan='2'  align='center'>");
			echo("Primary Company:");
                        echo("</td>");
                echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo("<input type='radio' name='primary_employer_id' value='" . $first_employer_id . "' ");
			echo($primary_employer_id == $first_employer_id ? "checked='checked'" : "");
			echo(" />");
			echo("&nbsp;" . getCompanyName($first_employer_id));
			echo("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
			echo("<input type='radio' name='primary_employer_id' value='" . $second_employer_id . "' ");
			echo($primary_employer_id == $second_employer_id ? "checked='checked'" : "");
			echo(" />");
			echo("&nbsp;" . getCompanyName($second_employer_id));
			echo("</td>");
		echo("</tr>\n");
		
		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

		// Display the differences between the information here, and let the user choose which to use.
		$firstComp = new EditContact('');
		$firstComp->loadCompanyValues($first_employer_id);

		$secondComp = new EditContact('');
		$secondComp->loadCompanyValues($second_employer_id);
		// Set booleans so we know whether or not to show them differences, and which ones to show.

		if (trim($firstComp->company_phone) != trim($secondComp->company_phone))
        {
			$phone_diff = true;
        }
		if (trim($firstComp->company_fax) != trim($secondComp->company_fax))
        {
			$fax_diff = true;
        }
		if (trim($firstComp->company_email) != trim($secondComp->company_email))
        {
			$email_diff = true;
        }
		if (trim($firstComp->company_website) != trim($secondComp->company_website))
        {
			$website_diff = true;
        }
		if (trim($firstComp->company_size_id) != trim($secondComp->company_size_id))
        {
			$size_diff = true;
        }
		if (trim($firstComp->company_industry_id) != trim($secondComp->company_industry_id))
        {
			$industry_diff = true;
        }

        if ($firstComp->company_street_address_1 != $secondComp->company_street_address_1)
        {
            $address_diff = true;
        }
        if ($firstComp->company_street_address_2 != $secondComp->company_street_address_2)
        {
            $address_diff = true;
        }
        if ($firstComp->company_street_address_3 != $secondComp->company_street_address_3)
        {
            $address_diff = true;
        }
        if ($firstComp->company_country_id != $secondComp->company_country_id)
        {
            $address_diff = true;
        }
        if ($firstComp->company_provstate_id != $secondComp->company_provstate_id)
        {
            $address_diff = true;
        }
        if ($firstComp->company_region_id != $secondComp->company_region_id)
        {
            $address_diff = true;
        }
        if ($firstComp->company_city != $secondComp->company_city)
        {
            $address_diff = true;
        }
        if ($firstComp->company_postal_code != $secondComp->company_postal_code)
        {
            $address_diff = true;
        }

		if ($firstComp->company_description != $secondComp->company_description)
			$description_diff = true;

		if ($firstComp->company_type_id != $secondComp->company_type_id)
			$type_diff = true;

        $status_diff = true;

		if ($phone_diff || $fax_diff || $email_diff || $website_diff || $size_diff || $industry_diff 
		|| $location_diff || $address_diff || $description_diff || $type_diff || $status_diff)
		{
			echo("<input type='hidden' name='phone_diff' value='" . $phone_diff . "' />");
			echo("<input type='hidden' name='fax_diff' value='" . $fax_diff . "' />");
			echo("<input type='hidden' name='email_diff' value='" . $email_diff . "' />");
			echo("<input type='hidden' name='website_diff' value='" . $website_diff . "' />");
			echo("<input type='hidden' name='size_diff' value='" . $size_diff . "' />");
			echo("<input type='hidden' name='industry_diff' value='" . $industry_diff . "' />");
			echo("<input type='hidden' name='location_diff' value='" . $location_diff . "' />");
			echo("<input type='hidden' name='address_diff' value='" . $address_diff . "' />");
			echo("<input type='hidden' name='description_diff' value='" . $description_diff . "' />");
			echo("<input type='hidden' name='type_diff' value='" . $type_diff . "' />");
			echo("<input type='hidden' name='status_diff' value='" . $status_diff . "' />");

			echo("<tr>");
				echo("<td colspan='2' align='center'>");
				echo("Listed below are the differences between the data for these two companies.  Please choose the data");
				echo(" that you would like to keep for the merged company.  Keep in mind that you will be able to change");
				echo(" these values after the merge is complete.");
				echo("</td>");
			echo("</tr>\n");

			echo("<tr>");
				echo("<td colspan='2'>&nbsp;</td>");
			echo("</tr>\n");

			echo("<tr>");
				echo("<td colspan='2' align='center'>");
				echo("<table border='1' align='center' class='row0' width='100%'>");

				echo("<tr align='center'>");
					echo("<td>");
					echo("Company Name");
					echo("</td>");
					echo("<td>");
					echo($firstComp->company_name);
					echo("</td>");
					echo("<td>");
					echo($secondComp->company_name);
					echo("</td>");
				echo("</tr>\n");

				if ($phone_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Phone");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_phone' value='" . $firstComp->company_phone . "' ");
						echo(isset($merge_phone) && $merge_phone == $firstComp->company_phone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_phone ? $firstComp->company_phone : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_phone' value='" . $secondComp->company_phone . "' ");
						echo(isset($merge_phone) && $merge_phone == $secondComp->company_phone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_phone ? $secondComp->company_phone : "Not Set");
						echo("</td>");
					echo("</tr>\n");
				}

				if ($fax_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Fax");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_fax' value='" . $firstComp->company_fax . "' ");
						echo(isset($merge_fax) && $merge_fax == $firstComp->company_fax ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_fax ? $firstComp->company_fax : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_fax' value='" . $secondComp->company_fax . "' ");
						echo(isset($merge_fax) && $merge_fax == $secondComp->company_fax ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_fax ? $secondComp->company_fax : "Not Set");
						echo("</td>");
					echo("</tr>\n");
				}

				if ($email_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("E-mail");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_email' value='" . $firstComp->company_email . "' ");
						echo(isset($merge_email) && $merge_email == $firstComp->company_email ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_email ? $firstComp->company_email : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_email' value='" . $secondComp->company_email . "' ");
						echo(isset($merge_email) && $merge_email == $secondComp->company_email ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_email ? $secondComp->company_email : "Not Set");
						echo("</td>");
					echo("</tr>\n");
				}

				if ($website_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Website");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_website' value='" . $firstComp->company_website . "' ");
						echo(isset($merge_website) && $merge_website == $firstComp->company_website ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_website ? $firstComp->company_website : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_website' value='" . $secondComp->company_website . "' ");
						echo(isset($merge_website) && $merge_website == $secondComp->company_website ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_website ? $secondComp->company_website : "Not Set");
						echo("</td>");
					echo("</tr>\n");
				}

				if ($size_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Size Range");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_size' value='" . $firstComp->company_size_id . "' ");
						echo(isset($merge_size) && $merge_size == $firstComp->company_size_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_size_id ? getSizeRange($firstComp->company_size_id) : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_size' value='" . $secondComp->company_size_id . "' ");
						echo(isset($merge_size) && $merge_size == $secondComp->company_size_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_size_id ? getSizeRange($secondComp->company_size_id) : "Not Set");
						echo("</td>");
					echo("</tr>\n");
				}

				if ($industry_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Industry");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_industry' value='" . $firstComp->company_industry_id . "' ");
						echo(isset($merge_industry) && $merge_industry == $firstComp->company_industry_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_industry_id ? getIndustryName($firstComp->company_industry_id) : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_industry' value='" . $secondComp->company_industry_id . "' ");
						echo(isset($merge_industry) && $merge_industry == $secondComp->company_industry_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_industry_id ? getIndustryName($secondComp->company_industry_id) : "Not Set");
						echo("</td>");
					echo("</tr>\n");
				}
			
				if ($address_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Address");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_address' value='" . $first_employer_id . "' ");
						echo(isset($merge_address) && $merge_address == $first_employer_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
                        if (!$firstComp->company_street_address_1 && !$firstComp->company_street_address_2 && !$firstComp->company_street_address_3
                            && !$firstComp->company_country_id && !$firstComp->company_provstate_id && !$firstComp->company_region_id 
                            && !$firstComp->company_city && !$firstComp->company_postal_code)
                        {
                            echo("Not Set");
                        }
                        else
                        {
                            echo($firstComp->company_street_address_1 ? $firstComp->company_street_address_1 . "<br />" : "");
                            echo($firstComp->company_street_address_2 ? $firstComp->company_street_address_2 . "<br />" : "");
                            echo($firstComp->company_street_address_3 ? $firstComp->company_street_address_3 . "<br />" : "");
                            echo($firstComp->company_country_id ? getCountryName($firstComp->company_country_id) . "<br />" : "");
                            echo($firstComp->company_provstate_id ? getProvstateName($firstComp->company_provstate_id) . "<br />" : "");
                            echo($firstComp->company_region_id ? getRegionName($firstComp->company_region_id) . "<br />" : "");
                            echo($firstComp->company_city ? $firstComp->company_city . "<br />" : "");
                            echo($firstComp->company_postal_code ? $firstComp->company_postal_code . "<br />" : "");
                        }
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_address' value='" . $second_employer_id . "' ");
						echo(isset($merge_address) && $merge_address == $second_employer_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
                        if (!$secondComp->company_street_address_1 && !$secondComp->company_street_address_2 && !$secondComp->company_street_address_3
                            && !$secondComp->company_country_id && !$secondComp->company_provstate_id && !$secondComp->company_region_id 
                            && !$secondComp->company_city && !$secondComp->company_postal_code)
                        {
                            echo("Not Set");
                        }
                        else
                        {
                            echo($secondComp->company_street_address_1 ? $secondComp->company_street_address_1 . "<br />" : "");
                            echo($secondComp->company_street_address_2 ? $secondComp->company_street_address_2 . "<br />" : "");
                            echo($secondComp->company_street_address_3 ? $secondComp->company_street_address_3 . "<br />" : "");
                            echo($secondComp->company_country_id ? getCountryName($secondComp->company_country_id) . "<br />" : "");
                            echo($secondComp->company_provstate_id ? getProvstateName($secondComp->company_provstate_id) . "<br />" : "");
                            echo($secondComp->company_region_id ? getRegionName($secondComp->company_region_id) . "<br />" : "");
                            echo($secondComp->company_city ? $secondComp->company_city . "<br />" : "");
                            echo($secondComp->company_postal_code ? $secondComp->company_postal_code . "<br />" : "");
                        }
						echo("</td>");
					echo("</tr>\n");
				}

				if ($description_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Description");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_description' value='" . $first_employer_id . "' ");
						echo(isset($merge_description) && $merge_description == $first_employer_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_description ? $firstComp->company_description : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_description' value='" . $second_employer_id . "' ");
						echo(isset($merge_description) && $merge_description == $second_employer_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_description ? $secondComp->company_description : "Not Set");
						echo("</td>");
					echo("</tr>");
				}

				if ($type_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Company Type");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_type' value='" . $firstComp->company_type_id . "' ");
						echo(isset($merge_type) && $merge_type == $firstComp->company_type_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstComp->company_type_id ? getCompanyTypeName($firstComp->company_type_id) : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_type' value='" . $secondComp->company_type_id . "' ");
						echo(isset($merge_type) && $merge_type == $secondComp->company_type_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondComp->company_type_id ? getCompanyTypeName($secondComp->company_type_id) : "Not Set");
						echo("</td>");
					echo("</tr>");
				}

				if ($status_diff)
				{
                    $first_company_status_sql = ("
                        SELECT COUNT(*) AS status_count, MIN(entered_on) AS entered_on
                        FROM department_company_status
                        WHERE employer_id = '".addslashes($first_employer_id)."'
                        ");
                    $first_company_status_result = $GLOBALS['dbh']->Execute($first_company_status_sql);
                    $first_company_status_row = $first_company_status_result->FetchRow();

                    $second_company_status_sql = ("
                        SELECT COUNT(*) AS status_count, MIN(entered_on) AS entered_on
                        FROM department_company_status
                        WHERE employer_id = '".addslashes($second_employer_id)."'
                        ");
                    $second_company_status_result = $GLOBALS['dbh']->Execute($second_company_status_sql);
                    $second_company_status_row = $second_company_status_result->FetchRow();

					echo("<tr align='center'>");
						echo("<td>");
						echo("Company Status");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_status_employer_id' value='" . $first_employer_id . "' ");
                        echo(isset($merge_status_employer_id) && $merge_status_employer_id == $first_employer_id ? "checked='checked'" : "");
                        echo(" />");
                        echo($first_company_status_row['status_count']." since ".$first_company_status_row['entered_on']);
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_status_employer_id' value='" . $second_employer_id . "' ");
                        echo(isset($merge_status_employer_id) && $merge_status_employer_id == $second_employer_id ? "checked='checked'" : "");
                        echo(" />");
                        echo($second_company_status_row['status_count']." since ".$second_company_status_row['entered_on']);
						echo("</td>");
					echo("</tr>");
				}
				echo("</table>");
				echo("</td>");
			echo("</tr>\n");
		}
					
                echo("</table>");
                echo("</td>");
        echo("</tr>");
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("<input type='submit' value='Continue' />");
	echo("</tr>");
	echo("</form>");
        echo("</table>");
        echo("</td>");
echo("</tr>");
echo("</table>");

?>
