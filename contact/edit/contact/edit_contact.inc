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
 | Filename: edit_contact.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file determines if the user needs to be shown the edit     |
 | contact form, or if they have submitted the form and are ready to save       |
 | their changes.                                                               |
 +------------------------------------------------------------------------------+

*/


$error = 0;

if (!$continue)
{
    // No form has been submitted yet, so display it for the user.
    $contact = new EditContact('');
    $contact->loadContactValues($contact_id);

    // Create an entitiy with the current status of the division, so that we may later check to ensure no concurrent editing problems.
    $check_changes = new EditContact('');
    $check_changes->loadContactValues($contact_id);

    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Save Changes')
{
    unset($changes_array);

    // Restore the check_changes contact class
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Fix address to be on subsequent lines.
    if (trim($contact_street_address_2) == '' && trim($contact_street_address_3) != '')
    {
        $contact_street_address_2 = trim($contact_street_address_3);
        $contact_street_address_3 = '';
    }
    if (trim($contact_street_address_1) == '' && trim($contact_street_address_2) != '')
    {
        $contact_street_address_1 = trim($contact_street_address_2);
        $contact_street_address_2 = '';
    }
    if (trim($contact_street_address_2) == '' && trim($contact_street_address_3) != '')
    {
        $contact_street_address_2 = trim($contact_street_address_3);
        $contact_street_address_3 = '';
    }

    // Replace "s with 's
    $contact_first_name = preg_replace("/\"/", "'", $contact_first_name);
    $contact_last_name = preg_replace("/\"/", "'", $contact_last_name);

    // Trim all of the empty elements out of $contact_flags, and $contact_delete_note_ids.  Also fix $contact_note_ids.
    $contact_flags = trim_array($contact_flags);
    $contact_delete_note_ids = trim_array($contact_delete_note_ids);

    $contact_flags = trim_array($contact_flags);

    // Save the inputs into the contact object.
    $contact = new EditContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name, 
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone, 
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1, 
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, 
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    // Begin validating inputs here.
    if (trim($contact->contact_first_name) == '')
    {
        $error = 1;
        error("You must specify a first name for this contact.");
        include('contact/edit/contact/edit_contact_form.inc');
    }
    elseif (trim($contact->contact_last_name) == '')
    {
        $error = 1;
        error("You must specify a last name for this contact.");
        include('contact/edit/contact/edit_contact_form.inc');
    }
    elseif ((trim($contact->contact_email) != '') && !(isValidEmail($contact->contact_email)))
    {
        $error = 1;
        error("You must include a valid set of e-mail addresses, or none at all.");
        include('contact/edit/contact/edit_contact_form.inc');
    }
    elseif ((trim($contact->contact_phone) != '') && (trim($contact->contact_phone) != USE_COMPANY) && (trim($contact->contact_phone) != USE_DEPARTMENT) && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_phone)))))
    {
        $error = 1;
        error("You must include a valid phone number with an area code, or none at all.");
        include('contact/edit/contact/edit_contact_form.inc');
    }
    elseif ((trim($contact->contact_fax) != '') && (trim($contact->contact_fax) != USE_COMPANY) && (trim($contact->contact_fax) != USE_DEPARTMENT) && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_fax)))))
    {
        $error = 1;
        error("You must include a valid fax number with an area code, or none at all.");
        include('contact/edit/contact/edit_contact_form.inc');
    }
    elseif ((trim($contact->contact_pager) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_pager)))))
    {
        $error = 1;
        error("You must include a valid pager number with an area code, or none at all.");
        include('contact/edit/contact/edit_contact_form.inc');
    }
    elseif ((trim($contact->contact_cellphone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_cellphone)))))
    {
        $error = 1;
        error("You must include a valid cellphone number with an area code, or none at all.");
        include('contact/edit/contact/edit_contact_form.inc');
    }

    if ($error == 0)
    {
        $changes_array = checkContactChanges($check_changes, $contact);
    }

    if ($changes_array["contact_changes_made"])
    {
        $error = 1;

        if ($changes_array["contact_change_by"])
        {
            $change_contact = getContactName($changes_array["contact_change_by"]);
            $change_name = "<b>" . $change_contact["first_name"] . " " . $change_contact["last_name"] . "</b>";
        }
        else
        {
            $change_name = "Someone";
        }

        $msg = ($change_name . " has modified this contact since you last loaded the page.");
        $msg .= ("<br /><br />The changes you have made have NOT been saved, but are shown in <b class='black'>bold</b> below the data that can be modified.");
        $msg .= ("  Once you have modified the contact as desired, press the 'Save Changes' button below to update the data.");

        error($msg);

        //If they entered new notes, save them

        $new_notes = $contact->contact_new_note;

        $contact = new EditContact('');
        $contact->loadContactValues($contact_id);

        $contact->contact_new_note = $new_notes;

        // Create an entity with the current status of the contact, so that we may later check to ensure no concurrent editing problems.
        $check_changes = new EditContact('');
        $check_changes->loadContactValues($contact_id);

        include('contact/edit/contact/edit_contact_form.inc');
    }

    if ($error == 0)
    {
        if (((sizeof($contact->contact_status_flag_array) && in_array(CONTACT_DO_NOT_CONTACT_FLAG,$contact->contact_employer_type)) 
            || $contact->contact_deleted_flag) && ($auth->department == DIRECTORS_OFFICE_DEPARTMENT || $auth->userlevel == TRAFFICDIRECTOR))
        {
            // If this is the directors office setting a contact to deleted, it will affect ALL depts status.  Get confirmation.
            $msg = ("Setting this contact's status to <b>" . getEmployerStatusName($contact->contact_status_id) . "</b> will set this status for ALL");
            $msg .= (" departments at " . ORGANIZATION_CODE . ".<br /><br />Are you sure you want to do this?");

            $msg .= ("<table cellspacing='2' cellpadding='2'>");
            $msg .= ("<tr>");
                $msg .= ("<td>");
                $msg .= ("<form method='post' action='$PHP_SELF'>");
                $msg .= ("<input type='hidden' name='select' value='edit_contact' />");
                $msg .= ("<input type='hidden' name='level1' value='contact' />");
                $msg .= ("<input type='hidden' name='continue' value='add_confirmed' />");
                $msg .= ("<input type='hidden' name='global_status_change' value='true' />");
                $msg .= ("<input type='hidden' name='contact' value='" . packObject($contact) . "' />");
                $msg .= ("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
                $msg .= ("<input type='hidden' name='department_id' value='" . $department_id . "' />");
                $msg .= ("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
                $msg .= ("<input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />");
                $msg .= ("</form> ");
                $msg .= ("</td>");
                $msg .= ("<td>");
                $msg .= ("<form method='post' action='$PHP_SELF'>");
                $msg .= ("<input type='hidden' name='select' value='edit_contact' />");
                $msg .= ("<input type='hidden' name='level1' value='contact' />");
                $msg .= ("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
                $msg .= ("<input type='hidden' name='department_id' value='" . $department_id . "' />");
                $msg .= ("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
                $msg .= ("<input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />");
                $msg .= ("</form>");
                $msg .= ("</td>");
            $msg .= ("</tr>");
            $msg .= ("</table>");
            error($msg);
        }
        else
        {
            include('contact/edit/contact/save_contact.inc');
        }
    }
}
elseif ($continue == 'add_confirmed')
{
    if ($contact && is_string($contact))
    {
        $contact = unpackObject($contact);
    }

    include('contact/edit/contact/save_contact.inc');

    unset($add_confirmed);
    unset($contact);
}
elseif ($continue == 'global_delete_confirmed')
{
    include('contact/edit/contact/save_contact.inc');

    unset($add_confirmed);
    unset($contact);
}
elseif ($continue == 'Use Company Fax')
{
    // They want to use the company's fax number for this contact, so simply reload the page.

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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    $contact->contact_fax = USE_COMPANY;
    $contact->contact_phone = $contact_phone;
    $contact->contact_location_info = $contact_location_info;
    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Use Division Fax')
{
    // They want to use the division's fax number for this contact, so simply reload the page.

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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    $contact->contact_fax = USE_DEPARTMENT;
    $contact->contact_phone = $contact_phone;
    $contact->contact_location_info = $contact_location_info;
    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Specify contact fax')
{
    /*
     They want to stop using the company fax number and specify
     one for the contact.
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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    if ($contact->contact_fax == USE_DEPARTMENT || $contact->contact_fax == USE_COMPANY)
    {
        $contact->contact_fax = '';
    }

    $contact->contact_phone = $contact_phone;
    $contact->contact_location_info = $contact_location_info;

    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Use Company Location')
{
    // They want to use the company's location for this contact, so simply reload the page.

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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    $contact->contact_location_info = USE_COMPANY;
    $contact->contact_phone = $contact_phone;
    $contact->contact_fax = $contact_fax;
    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Use Division Location')
{
    // They want to use the division's location for this contact, so simply reload the page.

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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    $contact->contact_location_info = USE_DEPARTMENT;
    $contact->contact_phone = $contact_phone;
    $contact->contact_fax = $contact_fax;
    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Specify contact location')
{
    /*
     They want to stop using the division or company information
     and specify the contact's own.
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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    $contact->contact_location_info = '';
    $contact->contact_phone = $contact_phone;
    $contact->contact_fax = $contact_fax;

    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Use Company Phone')
{
    // They want to use the company's phone number for this contact, so simply reload the page.

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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    $contact->contact_phone = USE_COMPANY;
    $contact->contact_fax = $contact_fax;
    $contact->contact_location_info = $contact_location_info;
    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Use Division Phone')
{
    // They want to use the division's phone number for this contact, so simply reload the page.

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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    $contact->contact_phone = USE_DEPARTMENT;
    $contact->contact_fax = $contact_fax;
    $contact->contact_location_info = $contact_location_info;
    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Specify contact phone')
{
    /*
     They want to stop using the company phone number and specify
     one for the contact.
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
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name,
            $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, $contact_department_comments, $contact_flags, $contact_new_note, $contact_note_ids,
            $contact_delete_note_ids);

    $contact->contact_id = $contact_id;

    if ($contact->contact_phone == USE_DEPARTMENT || $contact->contact_phone == USE_COMPANY)
    {
        $contact->contact_phone = '';
    }
    $contact->contact_fax = $contact_fax;
    $contact->contact_location_info = $contact_location_info;

    include('contact/edit/contact/edit_contact_form.inc');
}
elseif ($continue == 'Change Parent Company/Division')
{
    /*
     Pull out the current parent company's name, and set the $order variable to
     the first letter in this name, so that the user starts by having the current
     parent company already selected.
    */

    $sql = ("
        SELECT company_name
        FROM employer_company
        WHERE employer_id='" . $employer_id . "'
        AND company_display = '1'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $company_name = $row["company_name"];

    if (!$order)
    {
        $order = $company_name[0];
    }

    // Set these variables so we can check if jobs will need to be reassigned a new contact.
    if (!$old_employer_id)
    {
        $old_employer_id = $employer_id;
    }

    include('contact/edit/contact/change_company_department.inc');
}
elseif ($continue == 'Merge Contact with Another' || $continue == 'merge_contact_with_another')
{
    // The want to merge this contact with another.  Load up the list of contacts under this company

    include('contact/edit/contact/merge_contact_list.inc');
}
elseif ($continue == 'merge_contacts_chosen')
{
    // They've chosen the two contacts to merge.  Now we need to determine which contact is primary

    $contact_name = getContactName($first_contact_id);

    if (!$second_contact_id)
    {
        error("You must choose a second contact to merge <b>" . $contact_name["first_name"] . " " . $contact_name["last_name"] . "</b> with.");
        $contact_id = $first_contact_id;
        include('contact/edit/contact/merge_contact_list.inc');
    }
    else
    { 
        include('contact/edit/contact/choose_primary_contact.inc');
    }
}
elseif ($continue == 'merge_information_chosen')
{
    // They've chosen the information that is ready to be merged.  Now we just need to check for validity, then go ahead and merge the information.

    // Test to make sure that they can validly set location info here.
    $sql = ("
        SELECT ec.street_address_1, ec.street_address_2, ec.street_address_3, ec.country_id, ec.provstate_id, ec.region_id, ec.city, ec.postal_code,
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


    // Test to make that they can validly set phone info here.
    $sql = ("
        SELECT ed.street_address_1, ed.street_address_2, ed.street_address_3, ed.country_id, ed.provstate_id, ed.region_id, ed.city, ed.postal_code,
            ed.phone, ed.fax
        FROM employer_department AS ed
        INNER JOIN contact_employer AS ce
            ON ce.department_id=ed.department_id
        WHERE ce.contact_id='" . $primary_contact_id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $division_info_valid = ($row["street_address_1"] || $row["street_address_2"] || $row["street_address_3"] || $row["city"] || $row["region_id"]
            || $row["provstate_id"] || $row["country_id"] || $row["postal_code"]);
    $division_phone_valid = $row["phone"];
    $division_fax_valid = $row["fax"];

    $error = 0;

    if (!$primary_contact_id)
    {
        $error = 1;
        $msg .= ("Please choose one of the two contacts to be used as the primary contact for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($title_diff && !isset($merge_title))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available titles to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($greeting_diff && !isset($merge_greeting))
    {
        $error = 1;
        $msg .= ("Please choose one of the two greetings (Dear...) to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($phone_diff && !isset($merge_phone))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available phone numbers to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($fax_diff && !isset($merge_fax))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available fax numbers to use for the merge.");
        error($smg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($pager_diff && !isset($merge_pager))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available fax numbers to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($cellphone_diff && !isset($merge_cellphone))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available cellphone numbers to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($email_diff && !isset($merge_email))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available e-mail addresses to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($position_title_diff && !isset($merge_position_title))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available position titles to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($department_name_diff && !isset($merge_department_name))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available department names to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($employer_type_diff && !isset($merge_employer_type))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available employer types to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($location_diff && !isset($merge_location))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available location information options to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($address_diff && !isset($merge_address))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available addresses to use for the merge.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($merge_phone == USE_COMPANY && !trim($company_phone_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the company's phone number for the merged contact.  However, the primary contact you have chosen works for");
        $msg .= (" a company that does not have a phone number filled out.  As such, you may not choose to use the company's phone number if");
        $msg .= (" you wish to use this contact as the primary contact.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($merge_phone == USE_DEPARTMENT && !trim($division_phone_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the division's phone number for the merged contact.  However, the primary contact you have chosen works for");
        $msg .= (" a division that does not have a phone number filled out.  As such, you may not choose to use the division's phone number if");
        $msg .= (" you wish to use this contact as the primary contact.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($merge_fax == USE_COMPANY && !trim($company_fax_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the company's fax number for the merged contact.  However, the primary contact you have chosen works for");
        $msg .= (" a company that does not have a fax number filled out.  As such, you may not choose to use the company's fax number if");
        $msg .= (" you wish to use this contact as the primary contact.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($merge_fax == USE_DEPARTMENT && !trim($division_fax_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the division's fax number for the merged contact.  However, the primary contact you have chosen works for");
        $msg .= (" a division that does not have a fax number filled out.  As such, you may not choose to use the division's fax number if");
        $msg .= (" you wish to use this contact as the primary contact.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($merge_location == USE_COMPANY && !trim($company_info_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the company's location info for the merged contact.  However, the primary contact you have chosen works for");
        $msg .= (" a company that does not have any location information filled out.  As such, you may not choose to use the company's location info if");
        $msg .= (" you wish to use this contact as the primary contact.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }
    elseif ($merge_location == USE_DEPARTMENT && !trim($division_info_valid))
    {
        $error = 1;
        $msg .= ("You have attempted to use the division's location info for the merged contact.  However, the primary contact you have chosen works for");
        $msg .= (" a division that does not have any location information filled out.  As such, you may not choose to use the division's location info if");
        $msg .= (" you wish to use this contact as the primary contact.");
        error($msg);
        include('contact/edit/contact/choose_primary_contact.inc');
    }

    if ($error == 0)
    {
        if ($second_contact_id == $primary_contact_id)
        {
            $secondary_contact_id = $first_contact_id;
        }
        else
        {
            $secondary_contact_id = $second_contact_id;
        }

        $current_term = getCurrentTermID();
        $current_year = getCurrentYear();

        // :TODO: Use of obsolete employer_type field. Hard coded value = obsolete contact
        $sql = ("
                SELECT DISTINCT ce2.contact_id
                FROM contact_employer AS ce1, contact_employer AS ce2
                WHERE ce1.contact_id='" . $secondary_contact_id . "'
                AND ce1.employer_id=ce2.employer_id
                AND ce2.contact_id!='" . $secondary_contact_id . "'
                AND ce2.employer_type!='4'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        if (!$result->RecordCount())
        {
            // There's only one contact at this company, so check and make sure there are no current jobs for them.

            $sql = ("
                    SELECT ji.job_id
                    FROM job_info AS ji, term AS t
                    WHERE 
                    (
                     ji.status IN ('".NOT_CHECKED."','".CHECKED."')
                     OR
                     (
                      (ji.year>'" . $current_year . "'
                       OR (ji.year='" . $current_year . "'
                           AND t.year_order>'" . $current_term . "'))
                      AND t.term_id=ji.term_id
                      AND ji.status IN ('".POSTED."','".CLOSED."')
                     )
                    )
                    AND ji.contact_id='" . $secondary_contact_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                // There's a current job associated with this contact, so the contact can't be merged.

                $error = 1;
                $second_contact_name = getContactName($secondary_contact_id);
                $sql = ("
                    SELECT employer_id
                    FROM contact_employer
                    WHERE contact_id='" . $secondary_contact_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $secondary_employer_id = $row["employer_id"];

                $msg .= ("<b>" . $second_contact_name["first_name"] . " " . $second_contact_name["last_name"] . "</b>");
                $msg .= (" is the only contact at <b>" . getCompanyName($secondary_employer_id) . "</b>, and");
                $msg .= (" they have a job currently associated with them.  Due to this, you may not merge this contact with another");
                $msg .= (" unless this contact is used as the primary contact.");
                error($msg);
                include('contact/edit/contact/choose_primary_contact.inc');
            }
        }

        if ($error == 0)
        {
            // If this contact had current jobs associated with it, load up the screen to allow them
            // to change those jobs over.  Otherwise just load up the success screen.

            $sql = ("
                SELECT DISTINCT ji.job_id
                FROM job_info AS ji, term AS t
                WHERE 
                (
                ji.status IN ('" . NOT_CHECKED . "','" . CHECKED . "')
                OR (
                    (ji.year>'" . $current_year . "' OR (ji.year='" . $current_year . "' AND t.year_order>'" . $current_term . "'))
                    AND t.term_id=ji.term_id
                    AND ji.status IN ('" . POSTED . "','" . CLOSED . "')
                )
                )
                AND ji.contact_id='" . $secondary_contact_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                // They have jobs associated with them, so let the user choose a new contact to associate the jobs with.

                $second_contact_name = getContactName($secondary_contact_id);
                $secondContact = new EditContact('');
                $secondContact->loadContactValues($secondary_contact_id);

                $msg = ("<b>" . $second_contact_name["first_name"] . " " . $second_contact_name["last_name"] . "</b>");
                $msg .= (" had current jobs associated with them.  Please choose a new contact at ");
                $msg .= ("<b>" . getCompanyName($secondContact->contact_employer_id) . "</b> to associate with these jobs.");
                notify($msg);

                $employer_id = $secondContact->contact_employer_id;

                include('contact/edit/contact/choose_job_contact.inc');
            }
            else
            {
                include('contact/edit/contact/merge_contacts.inc');

                $no_buttons = true;
                $contact = new EditContact('');
                $contact->loadContactValues($primary_contact_id);

                include('contact/view/contact/view_specific_contact.inc');
            }
        }
    }
}
elseif ($continue == 'chosen_job_contact')
{
    // This is passed as a hidden input from the previous form.
    $merge_flags = unpackObject(($merge_flags));

    $error = 0;

    $sql = ("
            SELECT ce.employer_id, ce.department_id
            FROM contact_employer AS ce
            WHERE ce.contact_id='" . $job_contact_id . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $sql = ("
        SELECT status_flag_id
        FROM employer_info_status_flags_join
        WHERE contact_id = '".$job_contact_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $status_flag_array = array();
    if ($result->RecordCount() > 0)
    {
        while ($status_row = $result->FetchRow())
        {
            $status_flag_array[] = $status_row['status_flag_id'];
        }
    }

    $sql = ("
            SELECT employer_id
            FROM contact_employer
            WHERE contact_id='" . $secondary_contact_id . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row2 = $result->FetchRow();

    if (!$job_contact_id)
    {
        $error = 1;
        $msg .= ("You must choose a contact to be used as the new contact for all jobs that were previously associated with the merged contact.");
        error($msg);
        include('contact/edit/contact/choose_job_contact.inc');
    }
    elseif ($row["employer_id"] != $employer_id)
    {
        $error = 1;
        $msg .= ("You must choose a contact that still works for <b>" . getCompanyName($employer_id) . "</b> to associate with the merged contact's");
        $msg .= (" previous jobs.");
        error($msg);
        include('contact/edit/contact/choose_job_contact.inc');
    }
    elseif (sizeof($status_flag_array) > 0 && (in_array(CONTACT_DELETED_FLAG,$status_flag_array) || in_array(CONTACT_DO_NOT_CONTACT_FLAG,$status_flag_array)))
    {
        $error = 1;
        $msg .= ("You may not choose a contact with status <b>" . getEmployerStatusName($status_flag_array[0]) . "</b> to associate jobs with.  Please choose a contact from the list below.");
        error($msg);
        include('contact/edit/contact/choose_job_contact.inc');
    }
    else
    {
        $sql = ("
            UPDATE job_info
            SET contact_id='" . $job_contact_id . "',
            employer_department_id='" . $row["department_id"] . "'
            WHERE contact_id='" . $secondary_contact_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        include('contact/edit/contact/merge_contacts.inc');

        $job_contact_name = getContactName($job_contact_id);
        $msg = ("The existing jobs in the system have been successfully associated with <b>");
        $msg .= ($job_contact_name["first_name"] . " " . $job_contact_name["last_name"] . "</b>.");
        $msg .= ("<br /><br />Below are the results of your merge.");
        notify($msg);

        $no_buttons = true;

        $contact = new EditContact('');
        $contact->loadContactValues($primary_contact_id);

        include('contact/view/contact/view_specific_contact.inc');
    }
}
elseif ($continue == 'choose_contact_after_move')
{
    $error = 0;

    $sql = ("
            SELECT employer_id
            FROM contact_employer
            WHERE contact_id='" . $job_contact_id . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $sql = ("
        SELECT status_flag_id
        FROM employer_info_status_flags_join
        WHERE contact_id = '".$job_contact_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $status_flag_array = array();
    if ($result->RecordCount() > 0)
    {
        while ($status_row = $result->FetchRow())
        {
            $status_flag_array[] = $status_row['status_flag_id'];
        }
    }

    $sql = ("
            SELECT employer_id, department_id
            FROM contact_employer
            WHERE contact_id='" . $contact_id . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row2 = $result->FetchRow();

    if (!$job_contact_id)
    {
        $error = 1;
        $msg .= ("You must choose a contact to be used as the new contact for all jobs that were previously associated with the moved contact.");
        error($msg);
        include('contact/edit/contact/choose_contact_after_move.inc');
    }
    elseif ($row["employer_id"] != $old_employer_id)
    {
        $error = 1;
        $msg .= ("You must choose a contact that still works for <b>" . getCompanyName($old_employer_id) . "</b> to associate with the moved contact's");
        $msg .= (" previous jobs.");
        error($msg);
        include('contact/edit/contact/choose_contact_after_move.inc');
    }
    elseif (sizeof($status_flag_array) > 0 && (in_array(CONTACT_DELETED_FLAG,$status_flag_array) || in_array(CONTACT_DO_NOT_CONTACT_FLAG,$status_flag_array)))
    {
        $error = 1;
        $msg .= ("You may not choose an <b>" . getEmployerStatusName($status_flag_array[0]) . "</b> contact to associate jobs with.");
        $msg .= ("  Please choose a contact from the list below.");
        error($msg);
        include('contact/edit/contact/choose_contact_after_move.inc');
    }
    else
    {
        // They've successfully chosen a new contact for the jobs.  We can now move the other contact, and reassign the jobs.

        // Check and see if the division and/or company are changed, and if so, record these changes.
        $sql = ("
                SELECT employer_id, department_id
                FROM contact_employer
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        if ($department_id == 'none')
        {
            $tempdep = '';
        }
        else
        {
            $tempdep = $department_id;
        }

        $changeDepartment = (trim($tempdep) != trim($row["department_id"]));
        $changeCompany = recordChange($employer_id, $row["employer_id"]);

        if ($changeCompany)
        {
            /*
             If they've changed company, then they must have changed
             division, so record both of these as changed.
            */

            $recordedChange = "Parent Company, Parent Division";
            if (getJobsWithContact($contact_id))
            {
                $reassignContactJobs = true;
            }
        }
        elseif ($changeDepartment)
        {
            $recordedChange = "Parent Division";
        }

        if ($department_id == 'none')
        {
            $sql = ("
                    UPDATE contact_employer
                    SET employer_id='" . $employer_id . "',
                    department_id=''
                    WHERE contact_id='" . $contact_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }
        else
        {
            $sql = ("
                    UPDATE contact_employer
                    SET employer_id='" . $employer_id . "',
                    department_id='" . $department_id . "'
                    WHERE contact_id='" . $contact_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        // Update the contact's login
        $sql = ("
                UPDATE employer_login
                SET employer_id='" . $employer_id . "'
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($recordedChange)
        {
            $sql = ("
                    UPDATE contact_employer
                    SET changes_recorded_3=changes_recorded_2,
                    changes_recorded_2=changes_recorded_1,
                    changes_recorded_1='" . $recordedChange . "',
                    change_date_3=change_date_2,
                    change_date_2=change_date_1,
                    change_date_1=CURRENT_DATE,
                    change_by_3=change_by_2,
                    change_by_2=change_by_1,
                    change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "'
                    WHERE contact_id='" . $contact_id . "'
                    ");
            $GLOBALS['dbh']->Execute($sql);
        }

        // Reassign the jobs to the new contact.
        $sql = ("
                UPDATE job_info
                SET contact_id='" . $job_contact_id . "',
                employer_department_id='" . getContactsDepartmentID($job_contact_id) . "'
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        // Reevaluate the contact's status, as well as that of both the new and old companies and divisions.
        $sql = ("
            SELECT department_id
            FROM department
            WHERE job_list = '1'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row2 = $result->FetchRow())
        {
            // get current status of primary contact 
            $current_status_sql = ("
                SELECT activity_date, status_id
                FROM department_contact_status
                WHERE contact_id = '".$job_contact_id."' AND department_id = '".$row2["department_id"]."' AND current_status = 1
                ");
            $current_status_result = $GLOBALS['dbh']->Execute($current_status_sql);
            $current_status_row = $current_status_result->FetchRow();
            
            $status_array = reevaluateContactStatus($job_contact_id, $row2["department_id"]);
            if ($current_status_row["status_id"] != $status_array["status"])
            {
                $sql = ("
                    INSERT INTO department_contact_status
                    (contact_id, status_id, department_id, entered_on, activity_date, activity_type_id, term_id, year)
                    VALUES (
                        '" . $job_contact_id . "'
                        , '" . $status_array["status"] . "'
                        , '" . $row2["department_id"] . "'
                        , now()
                        , '" . $current_status_row["activity_date"] . "'
                        , '" . AUTOMATED . "'
                        , '0'
                        , '0000'
                        )
                    ");
                $GLOBALS['dbh']->Execute($sql);

                updateContactCurrentStatus($job_contact_id, $row2["department_id"]);
                // Set the division status
                updateDivisionStatus(getContactsDepartmentID($job_contact_id), $row2["department_id"]);
                // Set the company status
                updateCompanyStatus(getContactsEmployerID($job_contact_id), $row2["department_id"]);
            }

            updateContactCurrentStatus($contact_id, $row2["department_id"]);

            updateDivisionStatus(getContactsDepartmentID($contact_id), $row2["department_id"]);
            updateCompanyStatus(getContactsEmployerID($contact_id), $row2["department_id"]);
        }

        $msg = ("Changes to contact's parent company/division have been successfully changed and saved");
        $job_contact_name = getContactName($job_contact_id);
        $msg .= (" and the existing jobs in the system have been successfully associated with <b>");
        $msg .= ($job_contact_name["first_name"] . " " . $job_contact_name["last_name"] . "</b>.");
        notify($msg);

        $contact = new EditContact('');
        $contact->loadContactValues($contact_id);

        // Create an entity with the current status of the contact, so that we may later check to ensure no concurrent editing problems.
        $check_changes = new EditContact('');
        $check_changes->loadContactValues($contact_id);

        include('contact/edit/contact/edit_contact_form.inc');
    }
}
elseif ($continue == 'Save Parent Company/Division')
{
    /*
     Save the new parent company's employer_id into the contact's database,
     and then reload the contact edit form.
     */

    // Check if they've actually chosen a division.
    if (!trim($department_id))
    {
        $msg = ("You must choose a company as well as a division if you wish to change this contact's company (and division).");
        error($msg);
        include('contact/edit/contact/change_company_department.inc');
    }
    else
    {
        // Check and see if the division and/or company are changed, and if so, record these changes.
        $sql = ("
                SELECT employer_id, department_id
                FROM contact_employer
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        if ($department_id == 'none')
        {
            $tempdep = '';
        }
        else
        {
            $tempdep = $department_id;
        }

        $changeDepartment = (trim($tempdep) != trim($row["department_id"]));
        $changeCompany = recordChange($employer_id, $row["employer_id"]);

        if ($changeCompany)
        {
            if (getJobsWithContact($contact_id, true))
            {
                // We need to load up the page allowing them to reassign this contact's jobs.
                $msg .= ("  The contact you are moving has jobs associated with him/her, and these jobs must ");
                $msg .= ("  stay with the division where they were originally posted. Please choose a new contact ");
                $msg .= ("  within this division to transfer these jobs to. This step must be completed before ");
                $msg .= ("  the contact can be successfully moved. ");
                notify ($msg);

                include('contact/edit/contact/choose_contact_after_move.inc');
            }
            else
            {
                // If they've changed company, then they must have changed
                // division, so record both of these as changed.

                $recordedChange = "Parent Company, Parent Division";

                if ($department_id == 'none')
                {
                    $sql = ("
                            UPDATE contact_employer
                            SET employer_id='" . $employer_id . "',
                            department_id=''
                            WHERE contact_id='" . $contact_id . "'
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                }
                else
                {
                    $sql = ("
                            UPDATE contact_employer
                            SET employer_id='" . $employer_id . "',
                            department_id='" . $department_id . "'
                            WHERE contact_id='" . $contact_id . "'
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                }

                // Update the contact's login.
                $sql = ("
                        UPDATE employer_login
                        SET employer_id='" . $employer_id . "'
                        WHERE contact_id='" . $contact_id . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);

                // Record the changes made.
                if ($recordedChange)
                {
                    $sql = ("
                            UPDATE contact_employer
                            SET changes_recorded_3=changes_recorded_2,
                            changes_recorded_2=changes_recorded_1,
                            changes_recorded_1='" . $recordedChange . "',
                            change_date_3=change_date_2,
                            change_date_2=change_date_1,
                            change_date_1=CURRENT_DATE,
                            change_by_3=change_by_2,
                            change_by_2=change_by_1,
                            change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "'
                            WHERE contact_id='" . $contact_id . "'
                            ");
                    $GLOBALS['dbh']->Execute($sql);
                }

                // Reevaluate the contact's status, as well as that of both the new and old companies and divisions.
                $sql = ("
                    SELECT department_id
                    FROM department
                    WHERE job_list = '1'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row2 = $result->FetchRow())
                {
                    // New division/company
                    updateDivisionStatus($department_id, $row2["department_id"]);
                    updateCompanyStatus($employer_id, $row2["department_id"]);

                    // Old division/company
                    updateDivisionStatus($row["department_id"], $row2["department_id"]);
                    updateCompanyStatus($row["employer_id"], $row2["department_id"]);
                }

                $msg = ("Changes to contact's parent company/division have been successfully changed and saved.");
                notify($msg);

                $contact = new EditContact('');
                $contact->loadContactValues($contact_id);

                // Create an entity with the current status of the contact, so that we may later check to ensure no concurrent editing problems.
                $check_changes = new EditContact('');
                $check_changes->loadContactValues($contact_id);

                include('contact/edit/contact/edit_contact_form.inc');
            }
        }
        elseif ($changeDepartment)
        {
            // No need to reassign this contact's jobs, complete the procedure here.
            $recordedChange = "Parent Division";

            if ($department_id == 'none')
            {
                $sql = ("
                    UPDATE contact_employer
                    SET employer_id='" . $employer_id . "',
                    department_id=''
                    WHERE contact_id='" . $contact_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
            else
            {
                $sql = ("
                    UPDATE contact_employer
                    SET employer_id='" . $employer_id . "',
                    department_id='" . $department_id . "'
                    WHERE contact_id='" . $contact_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }

            // Update the contact's login.
            $sql = ("
                UPDATE employer_login
                SET employer_id='" . $employer_id . "'
                WHERE contact_id='" . $contact_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);

            // Update the contact's changes_recorded field.
            $sql = ("
                UPDATE contact_employer
                SET changes_recorded_3=changes_recorded_2,
                changes_recorded_2=changes_recorded_1,
                changes_recorded_1='" . $recordedChange . "',
                change_date_3=change_date_2,
                change_date_2=change_date_1,
                change_date_1=CURRENT_DATE,
                change_by_3=change_by_2,
                change_by_2=change_by_1,
                change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "'
                WHERE contact_id='" . $contact_id . "'
                ");
            $GLOBALS['dbh']->Execute($sql);

            // The contact has moved within the company, and to a new department, so update the job's attached to this contact.
            if ($department_id == 'none')
            {
                $sql = ("
                    UPDATE job_info
                    SET employer_department_id=''
                    WHERE contact_id='" . $contact_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
            else
            {
                $sql = ("
                    UPDATE job_info
                    SET employer_department_id='" . $department_id . "'
                    WHERE contact_id='" . $contact_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
                
            // Reevaluate the contact's status, as well as that of both the new and old companies and divisions.
            $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row2 = $result->FetchRow())
            {
                // New division
                updateDivisionStatus($department_id, $row2["department_id"]);
                // Old division
                updateDivisionStatus($row["department_id"], $row2["department_id"]);
            }

            $contact = new EditContact('');
            $contact->loadContactValues($contact_id);

            notify("Changes to contact's parent division have been successfully changed and saved.");

            // Create an entity with the current status of the division, so that we may later check to ensure no concurrent editing problems.
            $check_changes = new EditContact('');
            $check_changes->loadContactValues($contact_id);

            include('contact/edit/contact/edit_contact_form.inc');
        } else {
            $contact = new EditContact('');
            $contact->loadContactValues($contact_id);

            notify("Contact's parent company/division are unchanged.");

            // Create an entity with the current status of the division, so that we may later check to ensure no concurrent editing problems.
            $check_changes = new EditContact('');
            $check_changes->loadContactValues($contact_id);

            include('contact/edit/contact/edit_contact_form.inc');
        }
    }
}

?>
