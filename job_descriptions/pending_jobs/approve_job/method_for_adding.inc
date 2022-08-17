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
 | Filename: method_for_adding.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file is called once the user has decided how the           |
 | provided information will be saved in the database (new company, old         |
 | contact, update company, etc.).  It takes their choice and loads up the      |
 | appropriate file.                                                            |
 +------------------------------------------------------------------------------+

*/

include('contact/edit/EditContact.class');
include('contact/contact_functions.inc');

switch($choice)
{
	case 'updateCompany_addContact':
		if (!$matching_employer_id)
		{
			error("You must choose the company that you wish to update with the new information and add the contact under.");
			include('job_descriptions/pending_jobs/oldCompany_newContact.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/approve_job/updateCompany_addContact.inc');
		}
		break;

    case 'updateDepComp_updateContact':
        if (!trim($contact_id))
        {
            error("You must choose both the company/division and the contact that you wish to update with the new information.");
            include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
        }
        elseif (!trim($comp_div_id))
        {
            error("You must choose both the company/division and the contact that you wish to update with the new information.");
            include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
        }
        else
        {
            // Determine if we are to update a company or division.
            $comp_or_div = substr($comp_div_id, 0, 4);
            $id_number = substr($comp_div_id, 4);

            if ($comp_or_div == 'comp')
            {
                $matching_employer_id = $id_number;
                $matching_contact_id = $contact_id;
                include('job_descriptions/pending_jobs/approve_job/updateCompany_updateContact.inc');
            }
            else
            {
                $matching_department_id = $id_number;
                $matching_contact_id = $contact_id;
                $sql = ("
                    SELECT DISTINCT employer_id
                    FROM employer_department
                    WHERE department_id='" . addslashes($matching_department_id) . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $matching_employer_id = $row["employer_id"];
                include('job_descriptions/pending_jobs/approve_job/updateDepartment_updateContact.inc');
            }
        }
        break;

    case 'updateDepComp_addContact':
        if (!$comp_div_id)
        {
            error("You must choose the company or division that you wish to update with the new information and add the contact under.");
            include('job_descriptions/pending_jobs/oldCompany_newContact.inc');
        }
        else
        {
            // Determine if this is a company or division.
            $comp_or_div = substr($comp_div_id, 0, 4);
            $id_number = substr($comp_div_id, 4);

            if ($comp_or_div == 'comp')
            {
                $matching_employer_id = $id_number;
                include('job_descriptions/pending_jobs/approve_job/updateCompany_addContact.inc');
            }
            else
            {
                $matching_department_id = $id_number;
                include('job_descriptions/pending_jobs/approve_job/updateDepartment_addContact.inc');
            }
        }
        break;

	case 'updateDepartment_addContact':
		if (!$matching_department_id)
        {
            error("You must choose the division that you wish to update with the new information and add the contact under.");
            include('job_descriptions/pending_jobs/oldCompany_newContact.inc');
        }
        else
        {
            include('job_descriptions/pending_jobs/approve_job/updateDepartment_addContact.inc');
		}
		break;

	case 'dontUpdate_addContact':
		if (!$matching_employer_id)
        {
            error("You must choose the company/division that you wish to add the contact under");
            include('job_descriptions/pending_jobs/oldCompany_newContact.inc');
        }
        else
        {
            include('job_descriptions/pending_jobs/approve_job/dontUpdate_addContact.inc');
		}
		break;

	case 'dontUpdate_addCompany':
		if (!$matching_contact_id)
		{
			error("You must choose the contact that the job description is being added under.");
			include('job_descriptions/pending_jobs/newCompany_oldContact.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/approve_job/dontUpdate_addCompany.inc');
		}
		break;

	case 'updateContact_addCompany':
		if (!$matching_contact_id)
		{
			error("You must choose the contact that the job description is being added under.");
			include('job_descriptions/pending_jobs/newCompany_oldContact.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/approve_job/updateContact_addCompany.inc');
		}
		break;
	
    case 'updateContactFromBox_addCompany':
        if (!$contact_id)
        {
            error("You must choose a contact that you wish to move and update with the new information from the pop-up box.");
            include('job_descriptions/pending_jobs/newCompany_oldContact.inc');
        }
        else
        {
            $matching_contact_id = $contact_id;
            include('job_descriptions/pending_jobs/approve_job/updateContact_addCompany.inc');
        }
        break;

	case 'updateCompany_ignoreContact':
		if (!$matching_employer_id)
		{
			error("You must choose the company that you wish to update with the new information to proceed.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		elseif (!matching_contact_id)
		{
			error("You must choose the contact for the job description that is being saved.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		else
		{
			if ($matching_department_id < 0)
			{
				$matching_department_id = 0;
			}
			include('job_descriptions/pending_jobs/approve_job/updateCompany_ignoreContact.inc');
		}
		break;

	case 'updateDepartment_ignoreContact':
		if (!$matching_department_id || $matching_department_id < 0)
		{
			error("You must choose the division that you wish to update with the new information to proceed.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		elseif (!matching_contact_id)
		{
			error("You must choose the contact for the job description that is being saved.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/approve_job/updateDepartment_ignoreContact.inc');
		}
		break;

	case 'ignoreCompany_updateContact':
		if (!$matching_contact_id)
		{
			error("You must choose the contact that you wish to update with the new information to proceed.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		elseif (!matching_employer_id)
		{
			error("You must choose a company as well as a contact to proceed.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		else
		{
			if ($matching_department_id < 0)
			{
				$matching_department_id = 0;
			}
			include('job_descriptions/pending_jobs/approve_job/ignoreCompany_updateContact.inc');
		}
		break;

	case 'updateCompany_updateContact':
		if (!$matching_employer_id)
		{
			error("You must choose the company that you wish to update with the new information.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		elseif (!$matching_contact_id)
		{
			error("You must choose the contact that you wish to update with the new information.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		else
		{
			if ($matching_department_id < 0)
			{
				$matching_department_id = 0;
			}
			include('job_descriptions/pending_jobs/approve_job/updateCompany_updateContact.inc');
		}
		break;

	case 'updateDepartment_updateContact':
		if (!$matching_employer_id)
        {
            error("You must choose the company that you wish to update with the new information.");
            include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
        }
        elseif (!$matching_contact_id)
        {
            error("You must choose the contact that you wish to update with the new information.");
            include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
        }
        elseif ($matching_department_id < 0)
		{
			error("You must choose the division that you wish to update with the new information.");
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/approve_job/updateDepartment_updateContact.inc');
		}
        break;

	case 'newCompany_newContact':
		include('job_descriptions/pending_jobs/approve_job/newCompany_newContact.inc');
		break;

	case 'save_updated_company':

		$error = 0;

		$oldValues = new EditContact('');
        $oldValues->set_company_values($updatedCompany->company_name, $company_website, $company_description, $company_street_address_1,
            $company_street_address_2, $company_street_address_3, $company_city, $company_region_id, $company_provstate_id, $company_country_id,
            $company_postal_code, $company_phone, $company_fax, $company_email, $updatedCompany->company_flags, $company_industry_id, $company_size_id,
            $company_type_id, $updatedCompany->company_department_comments, $company_name_legitimate, $company_status, $company_new_note, $company_note_ids,
            $company_delete_note_ids);

		// Validate updated inputs.

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

		$updatedCompany = new EditContact('');
		$updatedCompany->loadCompanyValues($matching_employer_id);

		$updatedCompany->set_company_values($updatedCompany->company_name, $company_website, $company_description, $company_street_address_1,
            $company_street_address_2, $company_street_address_3, $company_city, $company_region_id, $company_provstate_id, $company_country_id,
            $company_postal_code, $company_phone, $company_fax, $company_email, $updatedCompany->company_flags, $company_industry_id, $company_size_id,
            $company_type_id, $updatedCompany->company_department_comments, $company_name_legitimate, $company_status, $company_new_note, $company_note_ids,
            $company_delete_note_ids);

		if ((trim($updatedCompany->company_email) != '') && !(isValidEmail($updatedCompany->company_email)))
        {
            $error = 1;
            error("You must include a valid set of e-mail addresses, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_company_inputs.inc');
        }
        elseif ((trim($updatedCompany->company_phone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($updatedCompany->company_phone)))))
        {
            $error = 1;
            error("You must include a valid phone number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_company_inputs.inc');
        }
        elseif ((trim($updatedCompany->company_fax) != '') && !(ereg('(.*[0-9].*){8,}',(trim($updatedCompany->company_fax)))))
        {
            $error = 1;
            error("You must include a valid fax number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_company_inputs.inc');
        }

		if ($error == 0)
		{
			if ($action == 'do_contact_next')
			{
				include('job_descriptions/pending_jobs/approve_job/save_updated_company_then_contact.inc');
			}
			else
			{
				include('job_descriptions/pending_jobs/approve_job/save_updated_company.inc');
			}
		}

		break;

	case "save_updated_department":
		
		$error = 0;

		$oldValues = new EditContact('');
		$oldValues->set_department_values($updatedDepartment->department_company_id, $updatedDepartment->department_name,
            $department_website, $department_description, $department_location_info, $department_street_address_1, $department_street_address_2,
            $department_street_address_3, $department_city, $department_region_id, $department_provstate_id, $department_country_id,
            $department_postal_code, $department_phone, $department_fax, $department_email, $updatedDepartment->department_flags, $department_industry_id,
            $department_size_id, $updatedDepartment->department_department_comments, $updatedDepartment->department_name_legitimate,
            $updatedDepartment->department_status, $updatedDepartment->department_new_note, $updateDepartment->department_note_ids,
            $updatedDepartment->department_delete_note_ids);

        // Validate updated inputs.

        //Fix the address to be on subsequent lines.

        if (trim($department_street_address_2) == '' && trim($department_street_address_3) != '')
        {
            $department_street_address_2 = trim($department_street_address_3);
            $department_street_address_3 = '';
        }
        if (trim($department_street_address_1) == '' && trim($department_street_address_2) != '')
        {
            $department_street_address_1 = trim($department_street_address_2);
            $department_street_address_2 = '';
        }
        if (trim($department_street_address_2) == '' && trim($department_street_address_3) != '')
        {
            $department_street_address_2 = trim($department_street_address_3);
            $department_street_address_3 = '';
        }

        $updatedDepartment = new EditContact('');
		$updatedDepartment->loadDepartmentValues($matching_department_id);

        $updatedDepartment->set_department_values($updatedDepartment->department_company_id, $updatedDepartment->department_name, 
            $department_website, $department_description, $department_location_info, $department_street_address_1, $department_street_address_2, 
            $department_street_address_3, $department_city, $department_region_id, $department_provstate_id, $department_country_id, 
            $department_postal_code, $department_phone, $department_fax, $department_email, $updatedDepartment->department_flags, $department_industry_id, 
            $department_size_id, $updatedDepartment->department_department_comments, $updatedDepartment->department_name_legitimate, 
            $updatedDepartment->department_status, $updatedDepartment->department_new_note, $updateDepartment->department_note_ids,
            $updatedDepartment->department_delete_note_ids);

        if ((trim($updatedDepartment->department_email) != '') && !(isValidEmail($updatedDepartment->department_email)))
        {
            $error = 1;
            error("You must include a valid set of e-mail addresses, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
        }
        elseif ((trim($updatedDepartment->department_phone) != '') 
                && !(ereg('(.*[0-9].*){8,}',(trim($updatedDepartment->department_phone)))) && trim($updatedDepartment->department_phone != USE_COMPANY))
        {
            $error = 1;
            error("You must include a valid phone number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
        }
        elseif ((trim($updatedDepartment->department_fax) != '') 
                && !(ereg('(.*[0-9].*){8,}',(trim($updatedDepartment->department_fax)))) && trim($updatedDepartment->department_fax != USE_COMPANY))
        {
            $error = 1;
            error("You must include a valid fax number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
        }

        if ($error == 0)
        {
            if ($action == 'do_contact_next')
            {
                include('job_descriptions/pending_jobs/approve_job/save_updated_department_then_contact.inc');
            }
            else
            {
                include('job_descriptions/pending_jobs/approve_job/save_updated_department.inc');
            }
        }
        break;

	case "save_updated_contact":

		$error = 0;

		$oldValues = new EditContact('');
            $oldValues->set_contact_values($updatedContact->contact_employer_id, $updatedContact->contact_department_id, $contact_title,
            $updatedContact->contact_first_name, $updatedContact->contact_middle_name, $updatedContact->contact_last_name,
            $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone,
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1,
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id,
            $contact_country_id, $contact_postal_code, $updatedContact->contact_department_comments,
            $updatedContact->contact_flags, $contact_new_note, $contact_note_ids, $contact_delete_note_ids);

        // Validate updated inputs.

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

		$updatedContact = new EditContact('');
		$updatedContact->loadContactValues($matching_contact_id);

		$updatedContact->set_contact_values($updatedContact->contact_employer_id, $updatedContact->contact_department_id, $contact_title, 
            $updatedContact->contact_first_name, $updatedContact->contact_middle_name, $updatedContact->contact_last_name, 
            $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, $contact_cellphone, 
            $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, $contact_street_address_1, 
            $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, 
            $contact_country_id, $contact_postal_code, $updatedContact->contact_department_comments, 
            $updatedContact->contact_flags, $contact_new_note, $contact_note_ids, $contact_delete_note_ids);
		
		if ((trim($updatedContact->contact_email) != '') && !(isValidEmail($updatedContact->contact_email)))
        {
            $error = 1;
            error("You must include a valid set of e-mail addresses, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
        }
        elseif ((trim($updatedContact->contact_phone) != '') && (trim($updatedContact->contact_phone) != USE_COMPANY) && (trim($updatedContact->contact_phone) != USE_DEPARTMENT) && !(ereg('(.*[0-9].*){8,}',(trim($updatedContact->contact_phone)))))
        {
            $error = 1;
            error("You must include a valid phone number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
        }
        elseif ((trim($updatedContact->contact_fax) != '') && (trim($updatedContact->contact_fax) != USE_COMPANY) && (trim($updatedContact->contact_fax) != USE_DEPARTMENT) && !(ereg('(.*[0-9].*){8,}',(trim($updatedContact->contact_fax)))))
        {
            $error = 1;
            error("You must include a valid fax number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
        }
        elseif ((trim($updatedContact->contact_pager) != '') && !(ereg('(.*[0-9].*){8,}',(trim($updatedContact->contact_pager)))))
        {
            $error = 1;
            error("You must include a valid pager number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
        }
        elseif ((trim($updatedContact->contact_cellphone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($updatedContact->contact_cellphone)))))
        {
            $error = 1;
            error("You must include a valid cellphone number with an area code, or none at all.");
            include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
        }

		if ($error == 0)
        {
            include('job_descriptions/pending_jobs/approve_job/save_updated_contact.inc');
        }
        break;
		
	case "Specify Contact Phone":

		$sql = ("
			SELECT DISTINCT phone
			FROM contact
			WHERE contact_id='" . addslashes($matching_contact_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$contact_phone = $row["phone"];

		$oldValues = new EditContact('');
		$oldValues->loadContactValues($matching_contact_id);
		
		$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title, 
		$contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email, $contact_phone, 
		$contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2, 
		$contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, $contact_postal_code, 
		'', '', '', '', '', '');
	
		$pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);
		
		include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
		break;

	case "Use Company Phone":
		
		if ($situation == 'updateContact')
		{
			$contact_phone = USE_COMPANY;
	
			$oldValues = new EditContact('');
            $oldValues->loadContactValues($matching_contact_id);
			
			$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title, 
                $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email, 
                $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2, 
                $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, $contact_postal_code, 
                '', '', '', '', '', '');
	
            $pendingEmployer = new PendingEmployer('');
			$pendingEmployer->load_pending_values($job_id);
	
            include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
		}
		else
		{
			$department_phone = USE_COMPANY;

            $oldValues = new EditContact('');
            $oldValues->loadDepartmentValues($matching_department_id);
	
			$oldValues->set_department_values($oldValues->department_company_id, $department_name, $department_website, $department_description, '', 
                $department_street_address_1, $department_street_address_2, $department_street_address_3, $department_city, 
                $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, $department_phone, 
                $department_fax, $department_email, '', $department_industry_id, $department_size_id, '', '', '', '', '', '');
	
            $pendingEmployer = new PendingEmployer('');
			$pendingEmployer->load_pending_values($job_id);
	
			if ($action == 'do_contact_next')
			{
				include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs_then_contact.inc');
			}
			else
			{
                include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
			}
		}

        break;

	case "Use Division Phone":

		$contact_phone = USE_DEPARTMENT;

		$oldValues = new EditContact('');
		$oldValues->loadContactValues($matching_contact_id);

		$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title,
            $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2,
            $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, $contact_postal_code,
            '', '', '', '', '', '');

		$pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);

		include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');

		break;

	case "Use Division Fax":

		$contact_fax = USE_DEPARTMENT;

		$oldValues = new EditContact('');
		$oldValues->loadContactValues($matching_contact_id);

		$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title,
            $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2,
            $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, $contact_postal_code,
            '', '', '', '', '', '');

		$pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);

		include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');

		break;

	case "Specify Contact Fax":
		
		$sql = ("
			SELECT DISTINCT fax
			FROM contact
			WHERE contact_id='" . addslashes($matching_contact_id) . "'
			");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $contact_fax = $row["fax"];

        $oldValues = new EditContact('');
        $oldValues->loadContactValues($matching_contact_id);

		$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title, 
            $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email, $contact_phone, 
            $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2, 
            $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, 
            $contact_postal_code, '', '', '', '', '', '');

        $pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);

        include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
        break;

	case "Specify Division Phone":
		
		$sql = ("
			SELECT DISTINCT phone
			FROM employer_department
			WHERE department_id='" . addslashes($matching_department_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$department_phone = ""; $row["phone"];
		
		$oldValues = new EditContact('');
		$oldValues->loadDepartmentValues($matching_department_id);

		$oldValues->set_department_values($oldValues->department_company_id, $department_name, $department_website, $department_description, '', 
            $department_street_address_1, $department_street_address_2, $department_street_address_3, $department_city, 
            $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, $department_phone, 
            $department_fax, $department_email, '', $department_industry_id, $department_size_id, '', '', '', '', '', '');

		$pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);

		if ($action == 'do_contact_next')
		{
			include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs_then_contact.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
		}
		break;

	case "Specify Division Fax":
		/*
        $sql = ("
			SELECT DISTINCT fax
			FROM employer_department
			WHERE department_id='" . addslashes($matching_department_id) . "'
			");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        */
        $department_fax = ""; $row["fax"];
		
		$oldValues = new EditContact('');
		$oldValues->loadDepartmentValues($matching_department_id);
		
		$oldValues->set_department_values($oldValues->department_company_id, $department_name, $department_website, $department_description, '', 
            $department_street_address_1, $department_street_address_2, $department_street_address_3, $department_city, 
            $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, $department_phone, 
            $department_fax, $department_email, '', $department_industry_id, $department_size_id, '', '', '', '', '', '');

		$pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);

		if ($action == 'do_contact_next')
		{
			include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs_then_contact.inc');
		}
		else
 		{
	 		include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
		}
		break;

	case "Use Company Fax":
		
		if ($situation == 'updateContact')
		{
			$contact_fax = USE_COMPANY;
			
			$oldValues = new EditContact('');
			$oldValues->loadContactValues($matching_contact_id);
		
			$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title, 
                $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email, $contact_phone, 
                $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2, 
                $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, $contact_postal_code, 
                '', '', '', '', '', '');
	
			$pendingEmployer = new PendingEmployer('');
			$pendingEmployer->load_pending_values($job_id);
			
            include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
		}
		else
		{
			$department_fax = USE_COMPANY;
			
			$oldValues = new EditContact('');
			$oldValues->loadDepartmentValues($matching_department_id);
			
			$oldValues->set_department_values($oldValues->department_company_id, $department_name, $department_website, $department_description, 
                '', $department_street_address_1, $department_street_address_2, $department_street_address_3, $department_city, 
                $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, $department_phone, 
                $department_fax, $department_email, '', $department_industry_id, $department_size_id, '', '', '', '', '', '');

			$pendingEmployer = new PendingEmployer('');
			$pendingEmployer->load_pending_values($job_id);
			
			if ($action == 'do_contact_next')
            {
                include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs_then_contact.inc');
            }
            else
            {
                include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
            }
        }

        break;

	case "Specify Division Location":

		$sql = ("
			SELECT DISTINCT country_id, provstate_id, region_id, city, postal_code, street_address_1, street_address_2, street_address_3
			FROM employer_department
			WHERE department_id='" . addslashes($matching_department_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();

		$department_street_address_1 = $row["street_address_1"];
		$department_street_address_2 = $row["street_address_2"];
		$department_street_address_3 = $row["street_address_3"];
		$department_country_id = $row["country_id"];
		$department_provstate_id = $row["provstate_id"];
		$department_region_id = $row["region_id"];
		$department_city = $row["city"];
		$department_postal_code = $row["postal_code"];

		$department_location_info = '';

		$oldValues = new EditContact('');
		$oldValues->loadDepartmentValues($matching_department_id);
		
		$oldValues->set_department_values($oldValues->department_company_id, $department_name, $department_website, $department_description, '', 
            $department_street_address_1, $department_street_address_2, $department_street_address_3, $department_city, 
            $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, $department_phone, 
            $department_fax, $department_email, '', $department_industry_id, $department_size_id, '', '', '', '', '', '');

		$pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);
		
		if ($action == 'do_contact_next')
		{
			include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs_then_contact.inc');
		}
		else
 		{
	 		include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
		}
		break;
	
	case "Specify Contact Location":
		
		$sql = ("
			SELECT DISTINCT country_id, provstate_id, region_id, city, postal_code, street_address_1, street_address_2,
			street_address_3
			FROM contact
			WHERE contact_id='" . addslashes($matching_contact_id) . "'
			");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $contact_street_address_1 = $row["street_address_1"];
        $contact_street_address_2 = $row["street_address_2"];
        $contact_street_address_3 = $row["street_address_3"];
        $contact_country_id = $row["country_id"];
        $contact_provstate_id = $row["provstate_id"];
        $contact_region_id = $row["region_id"];
        $contact_city = $row["city"];
        $contact_postal_code = $row["postal_code"];

        $contact_location_info = '';

        $oldValues = new EditContact('');
        $oldValues->loadContactValues($matching_contact_id);

		$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title, 
            $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email, $contact_phone, 
            $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2, 
            $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, 
            $contact_postal_code, '', '', '', '', '', '');
	
        $pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);

        include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
		break;

	case "Use Company Location":

		if ($situation == 'updateContact')
		{
			$contact_location_info = USE_COMPANY;
			
			$oldValues = new EditContact('');
			$oldValues->loadContactValues($matching_contact_id);
			
			$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title, 
                $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email, $contact_phone, 
                $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2, 
                $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id, 
                $contact_postal_code, '', '', '', '', '', '');
	
			$pendingEmployer = new PendingEmployer('');
			$pendingEmployer->load_pending_values($job_id);
			
			include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
		}
		else
		{
			$department_location_info = USE_COMPANY;
			
			$oldValues = new EditContact('');
			$oldValues->loadDepartmentValues($matching_department_id);
	
			$oldValues->set_department_values($oldValues->department_company_id, $department_name, $department_website, 
                $department_description, '', $department_street_address_1, $department_street_address_2, $department_street_address_3, 
                $department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
                $department_phone, $department_fax, $department_email, '', $department_industry_id, $department_size_id, '', '', 
                '', '', '', '');

			$pendingEmployer = new PendingEmployer('');
			$pendingEmployer->load_pending_values($job_id);
			
			if ($action == 'do_contact_next')
            {
                include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs_then_contact.inc');
            }
            else
            {
                include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');
            }
        }

		break;

	case "Use Division Location":

		$contact_location_info = USE_DEPARTMENT;

		$oldValues = new EditContact('');
		$oldValues->loadContactValues($matching_contact_id);

		$oldValues->set_contact_values($oldValues->contact_employer_id, $oldValues->contact_department_id, $contact_title,
            $contact_first_name, $contact_middle_name, $contact_last_name, $contact_called_name, '', $contact_email, $contact_phone,
            $contact_pager, $contact_cellphone, $contact_fax, '', '', '', $contact_street_address_1, $contact_street_address_2,
            $contact_street_address_3, $contact_city, $contact_region_id, $contact_provstate_id, $contact_country_id,
            $contact_postal_code, '', '', '', '', '', '');

		$pendingEmployer = new PendingEmployer('');
		$pendingEmployer->load_pending_values($job_id);

		include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');
		break;

} //switch

?>
