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
 | Filename: choose_primary_division.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: This file allows the user to choose which division is the       |
 | primary division, and point all of the contacts under the old non-primary    |
 | division towards the new one.                                                |
 +------------------------------------------------------------------------------+

*/


//:TODO Finish testing this part of the code, and then release this functionality for depts, companies and contacts.
$sql = ("
	SELECT DISTINCT department_name
	FROM employer_department
	WHERE department_id='" . $first_department_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$first_department_name = $row["department_name"];

$sql = ("
	SELECT DISTINCT department_name
	FROM employer_department
	WHERE department_id='" . $second_department_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$second_department_name = $row["department_name"];

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_contact' />");
echo("<input type='hidden' name='level1' value='department' />");
echo("<input type='hidden' name='first_department_id' value='" . $first_department_id . "' />");
echo("<input type='hidden' name='second_department_id' value='" . $second_department_id . "' />");
echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
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
			echo("You have chosen to merge <b>" . $first_department_name . "</b> and <b>" . $second_department_name . "</b> together.");
			echo("  Please choose which of these two divisions will be the primary division.  All contacts associated with the secondary ");
			echo("division will now be associated with the primary division after the merge.</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

                echo("<tr>");
                        echo("<td colspan='2'  align='center'>");
			echo("Primary Division:");
                        echo("</td>");
                echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo("<input type='radio' name='primary_department_id' value='" . $first_department_id . "' ");
			echo($primary_department_id == $first_department_id ? "checked='checked'" : "");
			echo(" />");
			echo("&nbsp;" . getDivisionName($first_department_id));
			echo("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
			echo("<input type='radio' name='primary_department_id' value='" . $second_department_id . "' ");
			echo($primary_department_id == $second_department_id ? "checked='checked'" : "");
			echo(" />");
			echo("&nbsp;" . getDivisionName($second_department_id));
			echo("</td>");
		echo("</tr>\n");
		
		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>\n");

		// Display the differences between the information here, and let the user choose which to use.

		$firstDept = new EditContact('');
		$firstDept->loadDepartmentValues($first_department_id);

		$secondDept = new EditContact('');
		$secondDept->loadDepartmentValues($second_department_id);

		// Set booleans so we know whether or not to show them differences, and which ones to show.

		if (trim($firstDept->department_phone) != trim($secondDept->department_phone))
        {
			$phone_diff = true;
        }
		if (trim($firstDept->department_fax) != trim($secondDept->department_fax))
        {
			$fax_diff = true;
        }
		if (trim($firstDept->department_email) != trim($secondDept->department_email))
        {
			$email_diff = true;
        }
		if (trim($firstDept->department_website) != trim($secondDept->department_website))
        {
			$website_diff = true;
        }
		if (trim($firstDept->department_size_id) != trim($secondDept->department_size_id))
        {
			$size_diff = true;
        }
		if (trim($firstDept->department_industry_id) != trim($secondDept->department_industry_id))
        {
			$industry_diff = true;
        }

		if (trim($firstDept->department_location_info) != trim($secondDept->department_location_info)) 
        {
			$location_diff = true;
        }

        if ($firstDept->department_street_address_1 != $secondDept->department_street_address_1)
        {
            $address_diff = true;
        }
        if ($firstDept->department_street_address_2 != $secondDept->department_street_address_2)
        {
            $address_diff = true;
        }
        if ($firstDept->department_street_address_3 != $secondDept->department_street_address_3)
        {
            $address_diff = true;
        }
        if ($firstDept->department_country_id != $secondDept->department_country_id)
        {
            $address_diff = true;
        }
        if ($firstDept->department_provstate_id != $secondDept->department_provstate_id)
        {
            $address_diff = true;
        }
        if ($firstDept->department_region_id != $secondDept->department_region_id)
        {
            $address_diff = true;
        }
        if ($firstDept->department_city != $secondDept->department_city)
        {
            $address_diff = true;
        }
        if ($firstDept->department_postal_code != $secondDept->department_postal_code)
        {
            $address_diff = true;
        }

		if ($firstDept->department_description != $secondDept->department_description)
        {
			$description_diff = true;
        }

        $status_diff = true;

		if ($phone_diff || $fax_diff || $email_diff || $website_diff || $size_diff || $industry_diff || $location_diff || $address_diff || $status_diff)
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
			echo("<input type='hidden' name='status_diff' value='" . $status_diff . "' />");

			echo("<tr>");
				echo("<td colspan='2' align='center'>");
				echo("Listed below are the differences between the data for these two divisions.  Please choose the data");
				echo(" that you would like to keep for the merged division.  Keep in mind that you will be able to change");
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
					echo("Division Name");
					echo("</td>");
					echo("<td>");
					echo($firstDept->department_name);
					echo("</td>");
					echo("<td>");
					echo($secondDept->department_name);
					echo("</td>");
				echo("</tr>\n");

				if ($phone_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Phone");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_phone' value='" . $firstDept->department_phone . "' ");
						echo(isset($merge_phone) && $merge_phone == $firstDept->department_phone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstDept->department_phone ? $firstDept->department_phone : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_phone' value='" . $secondDept->department_phone . "' ");
						echo(isset($merge_phone) && $merge_phone == $secondDept->department_phone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondDept->department_phone ? $secondDept->department_phone : "Not Set");
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
						echo("<input type='radio' name='merge_fax' value='" . $firstDept->department_fax . "' ");
						echo(isset($merge_fax) && $merge_fax == $firstDept->department_fax ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstDept->department_fax ? $firstDept->department_fax : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_fax' value='" . $secondDept->department_fax . "' ");
						echo(isset($merge_fax) && $merge_fax == $secondDept->department_fax ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondDept->department_fax ? $secondDept->department_fax : "Not Set");
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
						echo("<input type='radio' name='merge_email' value='" . $firstDept->department_email . "' ");
						echo(isset($merge_email) && $merge_email == $firstDept->department_email ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstDept->department_email ? $firstDept->department_email : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_email' value='" . $secondDept->department_email . "' ");
						echo(isset($merge_email) && $merge_email == $secondDept->department_email ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondDept->department_email ? $secondDept->department_email : "Not Set");
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
						echo("<input type='radio' name='merge_website' value='" . $firstDept->department_website . "' ");
						echo(isset($merge_website) && $merge_website == $firstDept->department_website ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstDept->department_website ? $firstDept->department_website : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_website' value='" . $secondDept->department_website . "' ");
						echo(isset($merge_website) && $merge_website == $secondDept->department_website ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondDept->department_website ? $secondDept->department_website : "Not Set");
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
						echo("<input type='radio' name='merge_size' value='" . $firstDept->department_size_id . "' ");
						echo(isset($merge_size) && $merge_size == $firstDept->department_size_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo(getSizeRange($firstDept->department_size_id));
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_size' value='" . $secondDept->department_size_id . "' ");
						echo(isset($merge_size) && $merge_size == $secondDept->department_size_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo(getSizeRange($secondDept->department_size_id));
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
						echo("<input type='radio' name='merge_industry' value='" . $firstDept->department_industry_id . "' ");
						echo(isset($merge_industry) && $merge_industry == $firstDept->department_industry_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstDept->department_industry_id ? getIndustryName($firstDept->department_industry_id) : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_industry' value='" . $secondDept->department_industry_id . "' ");
						echo(isset($merge_industry) && $merge_industry == $secondDept->department_industry_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondDept->department_industry_id ? getIndustryName($secondDept->department_industry_id) : "Not Set");
						echo("</td>");
					echo("</tr>\n");
				}
			
				if ($location_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Location information being used");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_location' value='" . $firstDept->department_location_info . "' ");
						echo(isset($merge_location) && $merge_location == $firstDept->department_location_info ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstDept->department_location_info ? $firstDept->department_location_info : "Using own information");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_location' value='" . $secondDept->department_location_info . "' ");
						echo(isset($merge_location) && $merge_location == $secondDept->department_location_info ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondDept->department_location_info ? $secondDept->department_location_info : "Using own information");
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
						echo("<input type='radio' name='merge_address' value='" . $first_department_id . "' ");
						echo(isset($merge_address) && $merge_address == $first_department_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
                        if (!$firstDept->department_street_address_1 && !$firstDept->department_street_address_2 && !$firstDept->department_street_address_3
                            && !$firstDept->department_country_id && !$firstDept->department_provstate_id && !$firstDept->department_region_id 
                            && !$firstDept->department_city && !$firstDept->department_postal_code)
                        {
                            echo("Not Set");
                        }
                        else
                        {
                            echo($firstDept->department_street_address_1 ? $firstDept->department_street_address_1 . "<br />" : "");
                            echo($firstDept->department_street_address_2 ? $firstDept->department_street_address_2 . "<br />" : "");
                            echo($firstDept->department_street_address_3 ? $firstDept->department_street_address_3 . "<br />" : "");
                            echo($firstDept->department_country_id ? getCountryName($firstDept->department_country_id) . "<br />" : "");
                            echo($firstDept->department_provstate_id ? getProvstateName($firstDept->department_provstate_id) . "<br />" : "");
                            echo($firstDept->department_region_id ? getRegionName($firstDept->department_region_id) . "<br />" : "");
                            echo($firstDept->department_city ? $firstDept->department_city . "<br />" : "");
                            echo($firstDept->department_postal_code ? $firstDept->department_postal_code . "<br />" : "");
                        }
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_address' value='" . $second_department_id . "' ");
						echo(isset($merge_address) && $merge_address == $second_department_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
                        if (!$secondDept->department_street_address_1 && !$secondDept->department_street_address_2 && !$secondDept->department_street_address_3
                            && !$secondDept->department_country_id && !$secondDept->department_provstate_id && !$secondDept->department_region_id 
                            && !$secondDept->department_city && !$secondDept->department_postal_code)
                        {
                            echo("Not Set");
                        }
                        else
                        {
                            echo($secondDept->department_street_address_1 ? $secondDept->department_street_address_1 . "<br />" : "");
                            echo($secondDept->department_street_address_2 ? $secondDept->department_street_address_2 . "<br />" : "");
                            echo($secondDept->department_street_address_3 ? $secondDept->department_street_address_3 . "<br />" : "");
                            echo($secondDept->department_country_id ? getCountryName($secondDept->department_country_id) . "<br />" : "");
                            echo($secondDept->department_provstate_id ? getProvstateName($secondDept->department_provstate_id) . "<br />" : "");
                            echo($secondDept->department_region_id ? getRegionName($secondDept->department_region_id) . "<br />" : "");
                            echo($secondDept->department_city ? $secondDept->department_city . "<br />" : "");
                            echo($secondDept->department_postal_code ? $secondDept->department_postal_code . "<br />" : "");
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
						echo("<input type='radio' name='merge_description' value='" . $first_department_id . "' ");
						echo(isset($merge_description) && $merge_description == $first_department_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstDept->department_description ? $firstDept->department_description : "Not Set");
						echo("</td>");
						echo("<td>");
						echo("<input type='radio' name='merge_description' value='" . $second_department_id . "' ");
						echo(isset($merge_description) && $merge_description == $second_department_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondDept->department_description ? $secondDept->department_description : "Not Set");
						echo("</td>");
					echo("</tr>");
				}

				if ($status_diff)
				{
                    $first_division_status_sql = ("
                        SELECT COUNT(*) AS status_count, MIN(entered_on) AS entered_on
                        FROM department_division_status
                        WHERE division_id = '".addslashes($first_department_id)."'
                        ");
                    $first_division_status_result = $GLOBALS['dbh']->Execute($first_division_status_sql);
                    $first_division_status_row = $first_division_status_result->FetchRow();

                    $second_division_status_sql = ("
                        SELECT COUNT(*) AS status_count, MIN(entered_on) AS entered_on
                        FROM department_division_status
                        WHERE division_id = '".addslashes($second_department_id)."'
                        ");
                    $second_division_status_result = $GLOBALS['dbh']->Execute($second_division_status_sql);
                    $second_division_status_row = $second_division_status_result->FetchRow();

					echo("<tr align='center'>");
						echo("<td>");
						echo("Division Status");
						echo("</td>");
						echo("<td align='center'>");
						echo("<input type='radio' name='merge_status_department_id' value='" . $first_department_id . "' ");
                        echo(isset($merge_status_department_id) && $merge_status_department_id == $first_department_id ? "checked='checked'" : "");
                        echo(" />");
						echo("&nbsp;");
                        echo($first_division_status_row['status_count']." since ".$first_division_status_row['entered_on']);
						echo("</td>");
						echo("<td align='center'>");
						echo("<input type='radio' name='merge_status_department_id' value='" . $second_department_id . "' ");
                        echo(isset($merge_status_department_id) && $merge_status_department_id == $second_department_id ? "checked='checked'" : "");
                        echo(" />");
						echo("&nbsp;");
                        echo($second_division_status_row['status_count']." since ".$second_division_status_row['entered_on']);
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

