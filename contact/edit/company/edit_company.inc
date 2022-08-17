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
 | Filename: edit_company.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file determines if the user needs to be shown the edit     |
 | company form, or if they have submitted the form and are ready to save       |
 | their changes.                                                               |
 +------------------------------------------------------------------------------+

*/


$error = 0;

if (!$continue)
{
    // No form has been submitted yet, so display it for the user.
	$contact = new EditContact('');
	$contact->loadCompanyValues($employer_id);

	// Create an entitiy with the current status of the company, so that we may later check to ensure no concurrent editing problems.
	$check_changes = new EditContact('');
	$check_changes->loadCompanyValues($employer_id);

    include('contact/edit/company/edit_company_form.inc');
}

elseif ($continue == 'Save Changes')
{
	// Restore the check_changes contact class
	
    if ($check_changes && is_string($check_changes))
    {
        $check_changes = unpackObject($check_changes);
    }

    // Fix the address to be on subsequent lines.

    if (trim($company_street_address_2) == '' && trim($company_street_address_3) != '')
    {
        $company_street_address_2 = trim($company_street_address_3);
        $company_street_address_3 = '';
    }
    if (trim($company_street_address_1) == '' && trim($company_street_address_2) != '')
    {
        $company_street_address_1 = trim($company_street_address_2);
        $company_street_address_2 = '';
    }
    if (trim($company_street_address_2) == '' && trim($company_street_address_3) != '')
    {
        $company_street_address_2 = trim($company_street_address_3);
        $company_street_address_3 = '';
    }

    // Replace "s with 's
    $company_name = preg_replace("/\"/", "'", $company_name);

    // Trim all of the empty elements out of $company_flags, and $company_delete_note_ids.  Also fix $company_note_ids.

    $company_flags = trim_array($company_flags);
    $company_delete_note_ids = trim_array($company_delete_note_ids);

    // Save the inputs in $contact.

    $contact = new EditContact('');
    $contact->set_company_values($company_name, $company_website, $company_description, $company_street_address_1,
        $company_street_address_2, $company_street_address_3, $company_city, $company_region_id, $company_provstate_id, 
        $company_country_id, $company_postal_code, $company_phone, $company_fax, $company_email, $company_flags, $company_industry_id,
        $company_size_id, $company_type_id, $company_department_comments, $company_name_legitimate, $company_status, $company_new_note, 
        $company_note_ids, $company_delete_note_ids);

    // Check to make sure there isn't already a company with this name in the database.

    $sql = ("
        SELECT DISTINCT company_name
        FROM employer_company
        WHERE company_name='" . addslashes(trim($contact->company_name)) . "'
        AND employer_id!='" . $employer_id . "'
        AND company_display
        ");

    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {
        $error = 1;
        $row = $result->FetchRow();
        $msg = ("There is already a company with the name: <b>" . $contact->company_name . "</b> in the database.");
        error($msg);
        include('contact/edit/company/edit_company_form.inc');
    }
    // Check that all inputs are valid.
    elseif (trim($contact->company_name) == '')
    {
        $error = 1;
        error("You must include a company name.");
        include('contact/edit/company/edit_company_form.inc');
    }
    elseif ((trim($contact->company_email) != '') && !(isValidEmail($contact->company_email)))
    {
        $error = 1;
        error("You must include a valid set of e-mail address, or none at all.");
        include('contact/edit/company/edit_company_form.inc');
    }
    elseif ((trim($contact->company_phone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->company_phone)))))
    {
        $error = 1;
        error("You must include a valid phone number with an area code, or none at all.");
        include('contact/edit/company/edit_company_form.inc');
    }
    elseif ((trim($contact->company_fax) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->company_fax)))))
    {
        $error = 1;
        error("You must include a valid fax number with an area code, or none at all.");
        include('contact/edit/company/edit_company_form.inc');
    }
    elseif (!trim($contact->company_type_id))
    {
        $error = 1;
        error("You must choose a company type for this company.");
        include('contact/edit/company/edit_company_form.inc');
    }
    elseif (trim($contact->company_industry_id) == '45')
    {
        $error = 1;
        error("You must choose a valid industry for this company.");
        include('contact/edit/company/edit_company_form.inc');
    }

    if ($error == 0)
    {
        $changes_array = checkCompanyChanges($check_changes, $contact);

        if ($changes_array["company_changes_made"])
        {
            $error = 1;

            if ($changes_array["company_change_by"])
            {
                $change_contact = getContactName($changes_array["company_change_by"]);
                $change_name = "<b>" . $change_contact["first_name"] . " " . $change_contact["last_name"] . "</b>";
            }
            else
            {
                $change_name = "Someone";
            }

            $msg = ($change_name . " has modified this company since you last loaded the page.");
            $msg .= ("<br /><br />The changes you have made have NOT been saved, but are shown in <b class='black'>bold</b> below the data that can be modified.");
            $msg .= ("  Once you have modified the company as desired, press the 'Save Changes' button below to update the data.");

            error($msg);

            //If they entered new notes, save them

            $new_notes = $contact->company_new_note;

            $contact = new EditContact('');
            $contact->loadCompanyValues($employer_id);

            $contact->company_new_note = $new_notes;

            // Create an entity with the current status of the company, so that we may later check to ensure no concurrent editing problems.
            $check_changes = new EditContact('');
            $check_changes->loadCompanyValues($employer_id);

            include('contact/edit/company/edit_company_form.inc');
        }
    }

    if( $error == 0 )
    {
        include('contact/edit/company/save_company.inc');
    }

}

elseif ($continue == 'add_confirmed')
{
    if ($contact && is_string($contact))
    {
        $contact = unpackObject($contact);
    }

    include('contact/edit/company/save_company.inc');
}

elseif ($continue == 'Merge Company with Another' || $continue == 'merge_company_with_another')
{
    // They want to merge this company with another.  Give them the list of companies to merge.

    include('contact/edit/company/merge_company_list.inc');
}

elseif ($continue == 'merge_companies_chosen')
{
    // They've chosen the two companies to merge.  Now we need to determine which company is primary

    if (!$second_employer_id)
    {
        error("You must choose a second company to merge with <b>" . getCompanyName($first_employer_id) . "</b>.");
        $employer_id = $first_employer_id;
        include('contact/edit/company/merge_company_list.inc');
    }
    else
    {
        include('contact/edit/company/choose_primary_company.inc');
    }
}

elseif ($continue == 'merge_information_chosen')
{
    // They've chosen the information that is ready to be merged.  Now we just need to go ahead and merge the information.

    $error = 0;

    if (!$primary_employer_id)
    {
        $error = 1;
        $msg .= ("Please choose one of the two companies to be used as the primary company for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($phone_diff && !isset($merge_phone))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available phone numbers to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($fax_diff && !isset($merge_fax))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available fax numbers to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($email_diff && !isset($merge_email))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available e-mail addresses to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($website_diff && !isset($merge_website))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available websites to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($size_diff && !isset($merge_size))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available size ranges to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($industry_diff && !isset($merge_industry))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available industries to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($type_diff && !isset($merge_type))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available company types to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($location_diff && !isset($merge_location))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available location information options to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($address_diff && !isset($merge_address))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available addresses to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif ($description_diff && !isset($merge_description))
    {
        $error = 1;
        $msg .= ("You must choose one of the two available descriptions to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }
    elseif (!isset($merge_status_employer_id))
    {
        $error = 1;
        $msg .= ("You must choose one of the two statuses to use for the merge.");
        error($msg);
        include('contact/edit/company/choose_primary_company.inc');
    }

    if ($error == 0)
    {
        if ($second_employer_id == $primary_employer_id)
        {
            $secondary_employer_id = $first_employer_id;
        }
        else
        {
            $secondary_employer_id = $second_employer_id;
        }

        include('contact/edit/company/merge_companies.inc');

        $no_buttons = true;

        $contact = new EditContact('');
        $contact->loadCompanyValues($primary_employer_id);

        include('contact/view/company/view_specific_company.inc');
    }
}

?>
