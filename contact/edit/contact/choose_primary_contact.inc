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
 | Filename: choose_primary_contact.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file allows the user to choose which contact is the        |
 | primary contact, and point all of the contacts under the old non-primary     |
 | contact towards the new one.                                                 |
 +------------------------------------------------------------------------------+

*/


$first_contact_name = getContactName($first_contact_id);
$second_contact_name = getContactName($second_contact_id);

echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_contact' />");
echo("<input type='hidden' name='level1' value='contact' />");
echo("<input type='hidden' name='first_contact_id' value='" . $first_contact_id . "' />");
echo("<input type='hidden' name='second_contact_id' value='" . $second_contact_id . "' />");
echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
echo("<input type='hidden' name='continue' value='merge_information_chosen' />");

echo("<table border='0' cellpadding='0' cellspacing='0' width='97%' class='row1'>");

echo("<tr>");
        echo("<td colspan='2'>");
        echo("<br />");
        echo("</td>\n");
echo("</tr>");

echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
        echo("<tr>");
                echo("<td colspan='2' class='row0'>");
                echo("<table width='97%' cellpadding='2' cellspacing='0' border='0'>");
		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo("You have chosen to merge <b>" . $first_contact_name["first_name"] . " " . $first_contact_name["last_name"]);
			echo("</b> and <b>" . $second_contact_name["first_name"] . " " . $second_contact_name["last_name"] . "</b> together.");
			echo("  Please choose which of these two contacts will be the primary contact.</td>");
		echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>\n");
		echo("</tr>\n");

        echo("<tr>");
            echo("<td colspan='2'  align='center'>");
			echo("Primary Contact:");
            echo("</td>\n");
        echo("</tr>\n");

		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>\n");
		echo("</tr>\n");

		echo("<tr>");
            echo("<td colspan='2' align='center'>");
            echo("<table border='0'>");
            echo("<tr>");

            $sql = ("
                SELECT DISTINCT ce.department_id, ce.employer_id, el.login_id
                FROM contact_employer AS ce
                LEFT JOIN employer_login AS el ON ce.contact_id=el.contact_id
                WHERE ce.contact_id='" . $first_contact_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

                echo("<td align='right'>");
                echo("<input type='radio' name='primary_contact_id' value='" . $first_contact_id . "' ");
                echo($primary_contact_id == $first_contact_id ? "checked='checked'" : "");
                echo(" />");
                echo("</td>\n");

                echo("<td align='left'>");
                echo($first_contact_name["first_name"] . " " . $first_contact_name["last_name"]);
                echo($row["department_id"] ? "<br />" . getDivisionName($row["department_id"]) : "");
                echo("<br />" . getCompanyName($row["employer_id"]));
                echo(($row["login_id"]) ? "<br />(Has a login ID)" : "<br />(Does not have a login ID)");
                echo("</td>\n");

            $sql = ("
                SELECT DISTINCT ce.department_id, ce.employer_id, el.login_id
                FROM contact_employer AS ce
                LEFT JOIN employer_login AS el ON ce.contact_id=el.contact_id
                WHERE ce.contact_id='" . $second_contact_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

                echo("<td align='right'>");
                echo("<input type='radio' name='primary_contact_id' value='" . $second_contact_id . "' ");
                echo($primary_contact_id == $second_contact_id ? "checked='checked'" : "");
                echo(" />");
                echo("</td>\n");

                echo("<td align='left'>");
                echo($second_contact_name["first_name"] . " " . $second_contact_name["last_name"]);
                echo($row["department_id"] ? "<br />" . getDivisionName($row["department_id"]) : "");
                echo("<br />" . getCompanyName($row["employer_id"]));
                echo(($row["login_id"]) ? "<br />(Has a login ID)" : "<br />(Does not have a login ID)");
                echo("</td>\n");

            echo("</tr>");
            echo("</table>");
            echo("</td>\n");
		echo("</tr>\n");
		
		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>\n");
		echo("</tr>\n");

		// Display the differences between the information here, and let the user choose which to use.

		$firstContact = new EditContact('');
		$firstContact->loadContactValues($first_contact_id);

		$secondContact = new EditContact('');
		$secondContact->loadContactValues($second_contact_id);

		// Set booleans so we know whether or not to show them differences, and which ones to show.

		if ($firstContact->contact_title != $secondContact->contact_title)
			$title_diff = true;
		if ($firstContact->contact_email != $secondContact->contact_email)
			$email_diff = true;
		if ($firstContact->contact_greeting != $secondContact->contact_greeting)
			$greeting_diff = true;
		if ($firstContact->contact_phone != $secondContact->contact_phone)
			$phone_diff = true;
		if ($firstContact->contact_pager != $secondContact->contact_pager)
			$pager_diff = true;
		if ($firstContact->contact_cellphone != $secondContact->contact_cellphone)
			$cellphone_diff = true;
		if ($firstContact->contact_fax != $secondContact->contact_fax)
			$fax_diff = true;
		if ($firstContact->contact_position_title != $secondContact->contact_position_title)
			$position_title_diff = true;
		if ($firstContact->contact_department_name != $secondContact->contact_department_name)
			$department_name_diff = true;
		if ($firstContact->contact_location_info != $secondContact->contact_location_info) 
			$location_diff = true;

        if ($firstContact->contact_street_address_1 != $secondContact->contact_street_address_1)
            $address_diff = true;
        if ($firstContact->contact_street_address_2 != $secondContact->contact_street_address_2)
            $address_diff = true;
        if ($firstContact->contact_street_address_3 != $secondContact->contact_street_address_3)
            $address_diff = true;
        if ($firstContact->contact_country_id != $secondContact->contact_country_id)
            $address_diff = true;
        if ($firstContact->contact_provstate_id != $secondContact->contact_provstate_id)
            $address_diff = true;
        if ($firstContact->contact_region_id != $secondContact->contact_region_id)
            $address_diff = true;
        if ($firstContact->contact_city != $secondContact->contact_city)
            $address_diff = true;
        if ($firstContact->contact_postal_code != $secondContact->contact_postal_code)
            $address_diff = true;

		// Contact flags

        if (is_array($firstContact->contact_flags) XOR is_array($secondContact->contact_flags))
        {
            $flags_diff = true;
        }
		elseif (is_array($firstContact->contact_flags) && is_array($secondContact->contact_flags))
		{
			// Check to make sure each element in BOTH arrays is in the other one.

			foreach ($firstContact->contact_flags AS $fcf)
			{
				if (!in_array($fcf, $secondContact->contact_flags))
				{
					$flags_diff = true;
				}
			}
			foreach ($secondContact->contact_flags AS $scf)
			{
				if (!in_array($scf, $firstContact->contact_flags))
				{
					$flags_diff = true;
				}
			}
		}

        // Contact status
        $status_diff = true;

		if ($title_diff || $email_diff || $greeting_diff || $phone_diff || $pager_diff || $cellphone_diff 
		|| $fax_diff || $position_title_diff || $department_name_diff || $location_diff || $address_diff || $flags_diff || $status_diff)
		{
			echo("<input type='hidden' name='title_diff' value='" . $title_diff . "' />");
			echo("<input type='hidden' name='greeting_diff' value='" . $greeting_diff . "' />");
			echo("<input type='hidden' name='email_diff' value='" . $email_diff . "' />");
			echo("<input type='hidden' name='phone_diff' value='" . $phone_diff . "' />");
			echo("<input type='hidden' name='fax_diff' value='" . $fax_diff . "' />");
			echo("<input type='hidden' name='pager_diff' value='" . $pager_diff . "' />");
			echo("<input type='hidden' name='cellphone_diff' value='" . $cellphone_diff . "' />");
			echo("<input type='hidden' name='position_title_diff' value='" . $position_title_diff . "' />");
			echo("<input type='hidden' name='department_name_diff' value='" . $department_name_diff . "' />");
			echo("<input type='hidden' name='flags_diff' value='" . $flags_diff . "' />");
			echo("<input type='hidden' name='location_diff' value='" . $location_diff . "' />");
			echo("<input type='hidden' name='address_diff' value='" . $address_diff . "' />");
			echo("<input type='hidden' name='status_diff' value='" . $status_diff . "' />");

			echo("<tr>");
				echo("<td colspan='2' align='center'>");
				echo("Listed below are the differences between the data for these two contacts.  Please choose the data");
				echo(" that you would like to keep for the merged contact.  Keep in mind that you will be able to change");
				echo(" these values after the merge is complete.");
				echo("</td>\n");
			echo("</tr>\n");

			echo("<tr>");
				echo("<td colspan='2'>&nbsp;</td>\n");
			echo("</tr>\n");

			echo("<tr>");
				echo("<td colspan='2' align='center'>");
				echo("<table border='1' align='center' class='row0' width='100%'>");

				echo("<tr align='center'>");
					echo("<td>");
					echo("Contact Name");
					echo("</td>\n");
					echo("<td>");
					echo($firstContact->contact_first_name . " ");
					echo($firstContact->contact_middle_name ? $firstContact->contact_middle_name . " " : "");
					echo($firstContact->contact_last_name);
					echo("</td>\n");
					echo("<td>");
					echo($secondContact->contact_first_name . " ");
					echo($secondContact->contact_middle_name ? $secondContact->contact_middle_name . " " : "");
					echo($secondContact->contact_last_name);
					echo("</td>\n");
				echo("</tr>\n");

				if ($title_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Title");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_title' value='" . $firstContact->contact_title . "' ");
						echo(isset($merge_title) && $merge_title == $firstContact->contact_title ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_title ? getTitleName($firstContact->contact_title) : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_title' value='" . $secondContact->contact_title . "' ");
						echo(isset($merge_title) && $merge_title == $secondContact->contact_title ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_title ? getTitleName($secondContact->contact_title) : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($greeting_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Dear...");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_greeting' value='" . $firstContact->contact_greeting . "' ");
						echo(isset($merge_greeting) && $merge_greeting == $firstContact->contact_greeting ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_greeting ? $firstContact->contact_greeting : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_greeting' value='" . $secondContact->contact_greeting . "' ");
						echo(isset($merge_greeting) && $merge_greeting == $secondContact->contact_greeting ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_greeting ? $secondContact->contact_greeting : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($phone_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Phone");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_phone' value='" . $firstContact->contact_phone . "' ");
						echo(isset($merge_phone) && $merge_phone == $firstContact->contact_phone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_phone ? $firstContact->contact_phone : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_phone' value='" . $secondContact->contact_phone . "' ");
						echo(isset($merge_phone) && $merge_phone == $secondContact->contact_phone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_phone ? $secondContact->contact_phone : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($fax_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Fax");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_fax' value='" . $firstContact->contact_fax . "' ");
						echo(isset($merge_fax) && $merge_fax == $firstContact->contact_fax ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_fax ? $firstContact->contact_fax : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_fax' value='" . $secondContact->contact_fax . "' ");
						echo(isset($merge_fax) && $merge_fax == $secondContact->contact_fax ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_fax ? $secondContact->contact_fax : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($pager_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Pager");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_pager' value='" . $firstContact->contact_pager . "' ");
						echo(isset($merge_pager) && $merge_pager == $firstContact->contact_pager ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_pager ? $firstContact->contact_pager : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_pager' value='" . $secondContact->contact_pager . "' ");
						echo(isset($merge_pager) && $merge_pager == $secondContact->contact_pager ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_pager ? $secondContact->contact_pager : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($cellphone_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Cellphone");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_cellphone' value='" . $firstContact->contact_cellphone . "' ");
						echo(isset($merge_cellphone) && $merge_cellphone == $firstContact->contact_cellphone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_cellphone ? $firstContact->contact_cellphone : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_cellphone' value='" . $secondContact->contact_cellphone . "' ");
						echo(isset($merge_cellphone) && $merge_cellphone == $secondContact->contact_cellphone ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_cellphone ? $secondContact->contact_cellphone : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($email_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("E-mail");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_email' value='" . $firstContact->contact_email . "' ");
						echo(isset($merge_email) && $merge_email == $firstContact->contact_email ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_email ? $firstContact->contact_email : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_email' value='" . $secondContact->contact_email . "' ");
						echo(isset($merge_email) && $merge_email == $secondContact->contact_email ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_email ? $secondContact->contact_email : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($position_title_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Position Title");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_position_title' value='" . $firstContact->contact_position_title . "' ");
						echo(isset($merge_position_title) && $merge_position_title == $firstContact->contact_position_title ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_position_title ? $firstContact->contact_position_title : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_position_title' value='" . $secondContact->contact_position_title . "' ");
						echo(isset($merge_position_title) && $merge_position_title == $secondContact->contact_position_title ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_position_title ? $secondContact->contact_position_title : "Not Set");
						echo("</td>\n");
					echo("</tr>\n");
				}

				if ($department_name_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Department Name");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_department_name' value='" . $firstContact->contact_department_name . "' ");
						echo(isset($merge_department_name) && $merge_department_name == $firstContact->contact_department_name ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_department_name ? $firstContact->contact_department_name : "Not Set");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_department_name' value='" . $secondContact->contact_department_name . "' ");
						echo(isset($merge_department_name) && $merge_department_name == $secondContact->contact_department_name ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_department_name ? $secondContact->contact_department_name : "Not Set");
						echo("</td>\n");
					echo("</tr>");
				}

				if ($location_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Location information being used");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_location' value='" . $firstContact->contact_location_info . "' ");
						echo(isset($merge_location) && $merge_location == $firstContact->contact_location_info ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($firstContact->contact_location_info ? $firstContact->contact_location_info : "Using own information");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_location' value='" . $secondContact->contact_location_info . "' ");
						echo(isset($merge_location) && $merge_location == $secondContact->contact_location_info ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
						echo($secondContact->contact_location_info ? $secondContact->contact_location_info : "Using own information");
						echo("</td>\n");
					echo("</tr>\n");
				}
						
				if ($address_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Address");
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_address' value='" . $first_contact_id . "' ");
						echo(isset($merge_address) && $merge_address == $first_contact_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
                        if (!$firstContact->contact_street_address_1 && !$firstContact->contact_street_address_2 && !$firstContact->contact_street_address_3
                            && !$firstContact->contact_country_id && !$firstContact->contact_provstate_id && !$firstContact->contact_region_id 
                            && !$firstContact->contact_city && !$firstContact->contact_postal_code)
                        {
                            echo("None Set");
                        }
                        else
                        {
                            echo($firstContact->contact_street_address_1 ? $firstContact->contact_street_address_1 . "<br />" : "");
                            echo($firstContact->contact_street_address_2 ? $firstContact->contact_street_address_2 . "<br />" : "");
                            echo($firstContact->contact_street_address_3 ? $firstContact->contact_street_address_3 . "<br />" : "");
                            echo($firstContact->contact_country_id ? getCountryName($firstContact->contact_country_id) . "<br />" : "");
                            echo($firstContact->contact_provstate_id ? getProvstateName($firstContact->contact_provstate_id) . "<br />" : "");
                            echo($firstContact->contact_region_id ? getRegionName($firstContact->contact_region_id) . "<br />" : "");
                            echo($firstContact->contact_city ? $firstContact->contact_city . "<br />" : "");
                            echo($firstContact->contact_postal_code ? $firstContact->contact_postal_code . "<br />" : "");
                        }
						echo("</td>\n");
						echo("<td>");
						echo("<input type='radio' name='merge_address' value='" . $second_contact_id . "' ");
						echo(isset($merge_address) && $merge_address == $second_contact_id ? "checked='checked'" : "");
						echo(" />");
						echo("&nbsp;");
                        if (!$secondContact->contact_street_address_1 && !$secondContact->contact_street_address_2 && !$secondContact->contact_street_address_3
                            && !$secondContact->contact_country_id && !$secondContact->contact_provstate_id && !$secondContact->contact_region_id 
                            && !$secondContact->contact_city && !$secondContact->contact_postal_code)
                        {
                            echo("None Set");
                        }
                        else
                        {
                            echo($secondContact->contact_street_address_1 ? $secondContact->contact_street_address_1 . "<br />" : "");
                            echo($secondContact->contact_street_address_2 ? $secondContact->contact_street_address_2 . "<br />" : "");
                            echo($secondContact->contact_street_address_3 ? $secondContact->contact_street_address_3 . "<br />" : "");
                            echo($secondContact->contact_country_id ? getCountryName($secondContact->contact_country_id) . "<br />" : "");
                            echo($secondContact->contact_provstate_id ? getProvstateName($secondContact->contact_provstate_id) . "<br />" : "");
                            echo($secondContact->contact_region_id ? getRegionName($secondContact->contact_region_id) . "<br />" : "");
                            echo($secondContact->contact_city ? $secondContact->contact_city . "<br />" : "");
                            echo($secondContact->contact_postal_code ? $secondContact->contact_postal_code . "<br />" : "");
                        }
						echo("</td>\n");
					echo("</tr>\n");
				}

                if ($status_diff)
                {
                    $first_contact_status_sql = ("
                        SELECT COUNT(*) AS status_count, min(activity_date) AS activity_date
                        FROM department_contact_status
                        WHERE contact_id = '".addslashes($first_contact_id)."'
                        ");
                    $first_contact_status_result = $GLOBALS['dbh']->Execute($first_contact_status_sql);
                    $first_contact_status_row = $first_contact_status_result->FetchRow();

                    $second_contact_status_sql = ("
                        SELECT COUNT(*) AS status_count, min(activity_date) AS activity_date
                        FROM department_contact_status
                        WHERE contact_id = '".addslashes($second_contact_id)."'
                        ");
                    $second_contact_status_result = $GLOBALS['dbh']->Execute($second_contact_status_sql);
                    $second_contact_status_row = $second_contact_status_result->FetchRow();

                    echo("<tr>");
                    echo("<td align='center'>Contact Status</td>");
                    echo("<td align='center'>");
						echo("<input type='radio' name='merge_status_contact_id' value='" . $first_contact_id . "' /> ");
                        echo($first_contact_status_row['status_count']." since ".$first_contact_status_row['activity_date']);
                    echo("</td>");
                    echo("<td align='center'>");
						echo("<input type='radio' name='merge_status_contact_id' value='" . $second_contact_id . "' /> ");
                        echo($second_contact_status_row['status_count']." since ".$second_contact_status_row['activity_date']);
                    echo("</td>");
                    echo("</tr>");
                }

				if ($flags_diff)
				{
					echo("<tr align='center'>");
						echo("<td>");
						echo("Flags Currently Set");
						echo("</td>\n");
						echo("<td>");

                        if (!is_array($firstContact->contact_flags))
                        {
                            $first_flags = ("<b class='black'>None Set</b>");
                        }
                        else
                        {
                            foreach ($firstContact->contact_flags AS $fcf)
                            {
                                $first_flags .= ("<b class='black'>" . getFlagName($fcf, 'contact') . "</b>, ");
                            }
                            $first_flags = substr($first_flags, 0, -2);
                        }
						echo($first_flags);
						echo("</td>\n");
						echo("<td>");
                        if (!is_array($secondContact->contact_flags))
                        {
                            $second_flags = ("<b class='black'>None Set</b>");
                        }
                        else
                        {
                            foreach ($secondContact->contact_flags AS $scf)
                            {
                                $second_flags .= ("<b class='black'>" . getFlagName($scf, 'contact') . "</b>, ");
                            }
                            $second_flags = substr($second_flags, 0, -2);
                        }
						echo($second_flags);
						echo("</td>\n");
					echo("</tr>\n");

					echo("<tr align='center'>");
						echo("<td>");
						echo("Flags To Be Set After Merge");
						echo("</td>\n");
						echo("<td colspan='2'>");
						echo("<table border='0' width='100%'>");

						$sql = ("
							SELECT DISTINCT a.flag_name, a.flag_id
							FROM contact_flags AS a, department_contact_flags AS b
							WHERE b.department_id='" . $auth->department . "'
							AND a.flag_id=b.flag_id
                            ORDER BY a.flag_name
							");
						$result = $GLOBALS['dbh']->Execute($sql);

						$newrow = 1;
						
						while ($row = $result->FetchRow())
						{
							echo($newrow % 2 ? "<tr align='center'>" : "");

							echo("<td width='5%' align='right'>");
							echo("<input type='checkbox' name='merge_flags[]' value='" . $row["flag_id"] . "' ");
							if (is_array($merge_flags) && in_array($row["flag_id"], $merge_flags) 
                                || (!isset($merge_flags) && is_array($firstContact->contact_flags) && in_array($row["flag_id"], $firstContact->contact_flags) 
                                && is_array($secondContact->contact_flags) && in_array($row["flag_id"], $secondContact->contact_flags)))
							{
								echo("checked='checked'");
							}
							echo(" />");
							echo("</td>\n");
							echo("<td width='45%' align='left'>");
							echo($row["flag_name"]);
							echo("</td>\n");
							
							echo(!($newrow % 2) ? "</tr>" : "");
							$newrow++;
						}
						
						if (!($newrow % 2))
						{
                            // Fill out the remaining column
                            echo("<td width='5%'>&nbsp;</td>");
                            echo("<td width='45%'>&nbsp;</td>");
							echo("</tr>");
						}
						echo("</table>");
						echo("</td>\n");
					echo("</tr>");
				}


				echo("</table>");
				echo("</td>\n");
			echo("</tr>\n");
		}
					
                echo("</table>");
                echo("</td>\n");
        echo("</tr>");
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("<input type='submit' value='Continue' />");
	echo("</tr>");
	echo("</form>");
        echo("</table>");
        echo("</td>\n");
echo("</tr>");
echo("</table>");

?>
