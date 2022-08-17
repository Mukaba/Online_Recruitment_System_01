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
 | Filename: edit_department.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file determines if the user needs to be shown the edit     |
 | division form, or if they have submitted the form and are ready to save      |
 | their changes.                                                               |
 +------------------------------------------------------------------------------+

*/


$error = 0;

if (!$continue)
{
    // No form has been submitted yet, so display it for the user.

    $contact = new EditContact('');
    $contact->loadDepartmentValues($department_id);

    // Create an entitiy with the current status of the division, so that we may later check to ensure no concurrent editing problems.
    $check_changes = new EditContact('');
    $check_changes->loadDepartmentValues($department_id);

    include('contact/edit/department/edit_department_form.inc');
}

elseif ($continue == 'Save Changes')
{
    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Fix the address to be on subsequent lines.
    if (trim($department_street_address_2) == '' && trim($department_street_address_3) != '')
    {
        $department_street_address_2 = trim($department_street_address_3);
        $department_street_address_2 = '';
    }
    if (trim($department_street_address_1) == '' && trim($department_street_address_2) != '')
    {
        $department_street_address_1 = trim($department_street_address_2);
        $department_street_address_2 = '';
    }
    if (trim($department_street_address_2) == '' && trim($department_street_address_3) != '')
    {
        $department_street_address_2 = trim($department_street_address_3);
        $department_street_address_2 = '';
    }

    // Replace "s with 's
    $department_name = preg_replace("/\"/", "'", $department_name);

    // Trim all of the empty elements out of $company_flags, and $company_delete_note_ids.  Also fix $company_note_ids.
    $department_flags = trim_array($department_flags);
    $department_delete_note_ids = trim_array($department_delete_note_ids);

    // Save the inputs into the contact object.
    $contact = new EditContact('');
    $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
            $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3, 
            $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
            $department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
            $department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
            $department_delete_note_ids);

    // Begin validating inputs here.
    if (trim($contact->department_name) == '')
    {
        $error = 1;
        error("You must specify a division name.");
        include('contact/edit/department/edit_department_form.inc');
    }
    elseif ((trim($contact->department_email) != '') && !(isValidEmail($contact->department_email)))
    {
        $error = 1;
        error("You must include a valid set of e-mail addresses, or none at all.");
        include('contact/edit/department/edit_department_form.inc');
    }
    elseif ((trim($contact->department_phone) != '') && (trim($contact->department_phone) != USE_COMPANY) && !(ereg('(.*[0-9].*){8,}',(trim($contact->department_phone)))))
    {
        $error = 1;
        error("You must include a valid phone number with an area code, or none at all.");
        include('contact/edit/department/edit_department_form.inc');
    }
    elseif ((trim($contact->department_fax) != '') && (trim($contact->department_fax) != USE_COMPANY) && !(ereg('(.*[0-9].*){8,}',(trim($contact->department_fax)))))
    {
        $error = 1;
        error("You must include a valid fax number with an area code, or none at all.");
        include('contact/edit/department/edit_department_form.inc');
    }
    elseif (trim($contact->department_industry_id) == '45')
    {
        $error = 1;
        error("You must choose a valid industry for this division.");
        include('contact/edit/department/edit_department_form.inc');
    }

    if ($error == 0)
    {
        $changes_array = checkDivisionchanges($check_changes, $contact);

        if ($changes_array["department_changes_made"])
        {
            $error = 1;

            if ($changes_array["department_change_by"])
            {
                $change_contact = getContactName($changes_array["department_change_by"]);
                $change_name = "<b>" . $change_contact["first_name"] . " " . $change_contact["last_name"] . "</b>";
            }
            else
            {
                $change_name = "Someone";
            }

            $msg = ($change_name . " has modified this division since you last loaded the page.");
            $msg .= ("<br /><br />The changes you have made have NOT been saved, but are shown in <b class='black'>bold</b> below the data that can be modified.");
            $msg .= ("  Once you have modified the division as desired, press the 'Save Changes' button below to update the data.");

            error($msg);

            //If they entered new notes, save them
            $new_notes = $contact->department_new_note;

            $contact = new EditContact('');
            $contact->loadDepartmentValues($department_id);

            $contact->department_new_note = $new_notes;

            // Create an entity with the current status of the division, so that we may later check to ensure no concurrent editing problems.
            $check_changes = new EditContact('');
            $check_changes->loadDepartmentValues($department_id);

            include('contact/edit/department/edit_department_form.inc');
        }
    }

    if ($error == 0)
    {
        include('contact/edit/department/save_department.inc');
    }
}

elseif ($continue == 'add_confirmed')
{
    if ($contact && is_string($contact))
    {
        $contact = unpackObject($contact);
    }

    include('contact/edit/department/save_department.inc');
}

elseif ($continue == 'Use Company Fax')
{               
    /*      
     They want to use the company's fax number for this division.
     So simply reload the page.
    */

    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Restore the changes_array if it's set.
    if ($changes_array && is_string($changes_array))
    {
        $changes_array = unpackObject(($changes_array));
    }

    $contact = new EditContact('');
    $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
            $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
            $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code,
            $department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id,
            $department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
            $department_delete_note_ids);

    $contact->department_id = $department_id;

    $contact->department_fax = USE_COMPANY;
    $contact->department_phone = $department_phone;
    $contact->department_location_info = $department_location_info;
    include('contact/edit/department/edit_department_form.inc');
}

elseif ($continue == 'Specify division fax')
{
    /*
     They want to stop using the company fax number and specify
     one for the division.
     */

    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Restore the changes_array if it's set.
    if ($changes_array && is_string($changes_array))
    {
        $changes_array = unpackObject(($changes_array));
    }

    $contact = new EditContact('');
    $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
            $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
            $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code,
            $department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id,
            $department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
            $department_delete_note_ids);

    $contact->department_id = $department_id;

    $contact->department_fax = '';
    $contact->department_phone = $department_phone;
    $contact->department_location_info = $department_location_info;
    include('contact/edit/department/edit_department_form.inc');
}

elseif ($continue == 'Use Company Phone')
{
    /*
     They want to use the company's phone number for this division.
     So simply reload the page.
    */

    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Restore the changes_array if it's set.
    if ($changes_array && is_string($changes_array))
    {
        $changes_array = unpackObject(($changes_array));
    }

    $contact = new EditContact('');
    $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
            $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
            $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code,
            $department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id,
            $department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
            $department_delete_note_ids);

    $contact->department_id = $department_id;

    $contact->department_phone = USE_COMPANY;
    $contact->department_fax = $department_fax;
    $contact->department_location_info = $department_location_info;
    include('contact/edit/department/edit_department_form.inc');
}

elseif ($continue == 'Specify division phone')
{
    /*
     They want to stop using the company phone number and specify
     one for the division.
     */

    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Restore the changes_array if it's set.
    if ($changes_array && is_string($changes_array))
    {
        $changes_array = unpackObject(($changes_array));
    }

    $contact = new EditContact('');
    $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
            $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
            $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code,
            $department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id,
            $department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
            $department_delete_note_ids);

    $contact->department_id = $department_id;

    $contact->department_phone = '';
    $contact->department_fax = $department_fax;
    $contact->department_location_info = $department_location_info;
    include('contact/edit/department/edit_department_form.inc');
}

elseif ($continue == 'Use Company Location')
{
    // They want to use the company's location for this division, so simply reload the page.
    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Restore the changes_array if it's set.
    if ($changes_array && is_string($changes_array))
    {
        $changes_array = unpackObject(($changes_array));
    }

    $contact = new EditContact('');
    $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
            $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
            $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code,
            $department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id,
            $department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
            $department_delete_note_ids);

    $contact->department_id = $department_id;

    $contact->department_location_info = USE_COMPANY;
    $contact->department_phone = $department_phone;
    $contact->department_fax = $department_fax;
    include('contact/edit/department/edit_department_form.inc');
}

elseif ($continue == 'Specify division location')
{
    // They want to stop using the company information and specify the division's own.
    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Restore the changes_array if it's set.
    if ($changes_array && is_string($changes_array))
    {
        $changes_array = unpackObject(($changes_array));
    }

    $contact = new EditContact('');
    $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
            $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
            $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code,
            $department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id,
            $department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
            $department_delete_note_ids);

    $contact->department_id = $department_id;

    $contact->department_location_info = '';
    $contact->department_phone = $department_phone;
    $contact->department_fax = $department_fax;

    include('contact/edit/department/edit_department_form.inc');
}

elseif ($continue == 'Merge Division with Another')
{
    // The want to merge this division with another.  Load up the list of divisions under this company
    include('contact/edit/department/merge_division_list.inc');
}

elseif ($continue == 'merge_divisions_chosen')
{
    if ($java_reload)
    {
        // They're still choosing merge divisions (probably just clicked on a new company) so reload the page.
        include('contact/edit/department/merge_division_list.inc');
    }
    else
    {
        // They've chosen the two divisions to merge.  Now we need to determine which division is primary
        if (!$second_department_id)
        {
            error("You must choose a second division to merge <b>" . getDivisionName($first_department_id) . "</b> with.");
            $department_id = $first_department_id;
            include('contact/edit/department/merge_division_list.inc');
        }
        else
        { 
            include('contact/edit/department/choose_primary_division.inc');
        }
    }
}

elseif ($continue == 'merge_information_chosen')
{
    // They've chosen the information that is ready to be merged.  Now we just need to go ahead and merge the information.

    // Test to make sure that they can validly set location info here.
    $sql = ("
        SELECT DISTINCT ec.street_address_1, ec.street_address_2, ec.street_address_3, ec.country_id, ec.provstate_id, ec.region_id, ec.city, ec.postal_code,
            ec.phone, ec.fax
        FROM employer_company AS ec
        INNER JOIN contact_employer AS ce
            ON ce.employer_id=ec.employer_id
        WHERE ce.contact_id='" . $primary_contact_id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $company_info_valid = ($row["street_address_1"] || $row["street_address_2"] || $row["street_address_3"] || $row["city"] || $row["region_id"]
            || $row["provstate_id"] || $row["country_id"] || $row["postal_code"]);
    $company_phone_valid = $row["phone"];
    $company_fax_valid = $row["fax"];

    $error = 0;

    if (!$primary_department_id)
    {
        $error = 1;
        $msg .= ("Please choose one of the two divisions to be used as the primary division for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($phone_diff && !isset($merge_phone))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available phone numbers to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($fax_diff && !isset($merge_fax))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available fax numbers to use for the merge.");
        error($smg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($email_diff && !isset($merge_email))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available e-mail addresses to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($website_diff && !isset($merge_website))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available websites to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($size_diff && !isset($merge_size))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available size ranges to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($industry_diff && !isset($merge_industry))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available industries to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($location_diff && !isset($merge_location))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available location information options to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($address_diff && !isset($merge_address))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available addresses to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($description_diff && !isset($merge_description))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available descriptions to use for the merge.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($merge_phone == USE_COMPANY && !trim($company_phone_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the company's phone number for the merged division.  However, the primary division you have chosen operates under");
        $msg .= (" a company that does not have a phone number filled out.  As such, you may not choose to use the company's phone number if");
        $msg .= (" you wish to use this division as the primary division.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($merge_fax == USE_COMPANY && !trim($company_fax_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the company's fax number for the merged division.  However, the primary division you have chosen operates under");
        $msg .= (" a company that does not have a fax number filled out.  As such, you may not choose to use the company's fax number if");
        $msg .= (" you wish to use this division as the primary division.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif ($merge_location == USE_COMPANY && !trim($company_info_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the company's location info for the merged division.  However, the primary division you have chosen operates under");
        $msg .= (" a company that does not have any location information filled out.  As such, you may not choose to use the company's location info if");
        $msg .= (" you wish to use this division as the primary division.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }
    elseif (!isset($merge_status_department_id))
    {
        $error = 1;
        $msg .= ("You must choose one of the two division's statuses for merging.");
        error($msg);
        include('contact/edit/department/choose_primary_division.inc');
    }

    if ($error == 0)
    {
        if ($second_department_id == $primary_department_id)
        {
            $secondary_department_id = $first_department_id;
        }
        else
        {
            $secondary_department_id = $second_department_id;
        }

        include('contact/edit/department/merge_departments.inc');

        $no_buttons = true;

        $contact = new EditContact('');
        $contact->loadDepartmentValues($primary_department_id);

        include('contact/view/company/view_specific_department.inc');
    }
}

elseif ($continue == 'Change Parent Company')
{
    /*
     Pull out the current parent company's name, and set the $order variable to
     the first letter in this name, so that the user starts by having the current
     parent company already selected.
     */

    $sql = ("
            SELECT DISTINCT company_name
            FROM employer_company
            WHERE employer_id='" . $employer_id . "'
            AND company_display
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $company_name = $row["company_name"];

    if (!$order)
    {
        $order = $company_name[0];
    }

    include('contact/edit/department/change_parent_company.inc');
}

elseif ($continue == 'Save Parent Company')
{
    //:NOTE Remove this line if we want to allow the user the choice of what to do with contacts.
    $move_contacts = 'department';

    /*
     Save the new parent company's employer_id into the division's database,
     and then reload the division edit form.
    */

    if (!$employer_id)
    {
        $msg = ("You must choose a new company to move this department to.");
        error($msg);
        include('contact/edit/department/change_parent_company.inc');
    }
    else
    {
        $sql = ("
                SELECT DISTINCT employer_id
                FROM employer_department
                WHERE department_id='" . $department_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $old_employer_id = $row["employer_id"];

        $changeCompany = recordChange($employer_id, $old_employer_id);

        if ($changeCompany)
        {
            // We've changed the division's parent company, so record this change.
            $recordedChange = "Parent Company";

            $sql = ("
                    UPDATE employer_department
                    SET changes_recorded_3=changes_recorded_2,
                    changes_recorded_2=changes_recorded_1,
                    changes_recorded_1='" . $recordedChange . "',
                    change_date_3=change_date_2,
                    change_date_2=change_date_1,
                    change_date_1=CURRENT_DATE,
                    change_by_3=change_by_2,
                    change_by_2=change_by_1,
                    change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "'
                    WHERE department_id='" . $department_id . "'
                    ");
            $GLOBALS['dbh']->Execute($sql);
        }

        if ($move_contacts == 'department')
        {
            // Contact's are moving with the department.
            $sql = ("
                    UPDATE contact_employer
                    SET employer_id='" . $employer_id . "',
                    changes_recorded_3=changes_recorded_2,
                    changes_recorded_2=changes_recorded_1,
                    changes_recorded_1='Parent Company',
                    change_date_3=change_date_2,
                    change_date_2=change_date_1,
                    change_date_1=CURRENT_DATE,
                    change_by_3=change_by_2,
                    change_by_2=change_by_1,
                    change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "'
                    WHERE department_id='" . $department_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                UPDATE job_info
                SET employer_id='" . $employer_id . "'
                WHERE employer_department_id='" . $department_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }
        else
        {
            $sql = ("
                    UPDATE contact_employer
                    SET department_id='',
                    changes_recorded_3=changes_recorded_2,
                    changes_recorded_2=changes_recorded_1,
                    changes_recorded_1='Parent Division',
                    change_date_3=change_date_2,
                    change_date_2=change_date_1,
                    change_date_1=CURRENT_DATE,
                    change_by_3=change_by_2,
                    change_by_2=change_by_1,
                    change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "'
                    WHERE department_id='" . $department_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                UPDATE job_info
                SET employer_department_id=''
                WHERE employer_department_id='" . $department_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        $sql = ("
                UPDATE employer_department
                SET employer_id='" . $employer_id . "'
                WHERE department_id='" . $department_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        // Update all of the history records that pointed to this division.
        $sql = ("
                UPDATE history
                SET employer_id='" . $employer_id . "'
                WHERE company_department_id='" . $department_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $contact = new EditContact('');
        $contact->loadDepartmentValues($department_id);

        notify("Parent Company has been successfully changed and saved.");

        // Create an entity with the current status of the division, so that we may later check to ensure no concurrent editing problems.
        // Restore the check_changes contact class
        if ($check_changes && is_string($check_changes))
        {
            $check_changes = unpackObject($check_changes);
        }
        else
        {
            $check_changes = new EditContact('');
            $check_changes->loadDepartmentValues($department_id);
        }

        // Update this division's old and new company, as well as its own status, for each department at our organization.
        $sql = ("
            SELECT DISTINCT department_id
            FROM department
            WHERE job_list
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            // Although the division's status shouldn't change with the current code, it will change if user's are once again
            // given the option to leave division contact's with their old company.
            updateDivisionStatus($department_id, $row["department_id"]);
            updateCompanyStatus($old_employer_id, $row["department_id"]);
            updateCompanyStatus($employer_id, $row["department_id"]);
        }
        include('contact/edit/department/edit_department_form.inc');
    }
}

?>
