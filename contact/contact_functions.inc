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
 | Filename: contact_functions.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file contains any functions that are specific to the       |
 | contact info module. Most of these functions operate with the Contact.class  |
 | file.                                                                        |
 |                                                                              |
 | Function List:                                                               |
 |                                                                              |
 | void promptAddCompany($msg, $contact) void promptAddDepartment($msg,         |
 | $contact) void promptAddContact($msg, $contact) void                         |
 | promptEditCompany($msg, $employer_id) void promptEditDepartment($msg,        |
 | $department_id) void promptEditContact($msg, $contact_id)                    |
 | array checkCompanyChanges(class $initial_contact,                            |
 | class $attempted_save_contact) array                                         |
 | checkDivisionchanges(class $initial_contact, class $attempted_save_contact)  |
 | array checkContactChanges(class $initial_contact, class                      |
 | $attempted_save_contact)                                                     |
 +------------------------------------------------------------------------------+

*/

/*
 Function is called when we have found a name similar to the company we are trying to add.
 Checks to make sure the user really wants to add this company.  If so, then add
 the company to the database.  Otherwise, return the user to the front menu.
*/

function promptAddCompany($msg, $contact)
{
        global $PHP_SELF;
        instruct($msg .  "
                </b><table cellspacing='2' cellpadding='2'>
                <tr>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_contact' />
                        <input type='hidden' name='level1' value='company' />
                        <input type='hidden' name='add_confirmed' value='true' />
                        <input type='hidden' name='continue' value='Add Company Now' />
                        <input type='hidden' name='contact' value='" . packObject($contact) . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='' />
                        <input type='hidden' name='menu_select' value='' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form>
                </td>
                </tr>
                </table><b>&nbsp;");
}

/*
 Function is called when we have found a name similar to the division we are trying to add.
 Checks to make sure the user really wants to add this division.  If so, then add
 the division to the database.  Otherwise, return the user to the front menu.
*/

function promptAddDepartment($msg, $contact)
{
        global $PHP_SELF;
        instruct($msg .  "
                </b><table cellspacing='2' cellpadding='2'>
                <tr>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_contact' />
                        <input type='hidden' name='level1' value='department' />
                        <input type='hidden' name='add_confirmed' value='true' />
                        <input type='hidden' name='employer_id' value='$contact->department_company_id' />
                        <input type='hidden' name='continue' value='Add Division Now' />
                        <input type='hidden' name='contact' value='" . packObject($contact) . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='' />
                        <input type='hidden' name='menu_select' value='' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form>
                </td>
                </tr>
                </table><b>&nbsp;");
}

/*
 Function is called when we have found a name similar to the contact we are trying to add.
 Checks to make sure the user really wants to add this contact.  If so, then add
 the contact to the database.  Otherwise, return the user to the front menu.
*/

function promptAddContact($msg, $contact)
{
        global $PHP_SELF;
        instruct($msg .  "
                </b><table cellspacing='2' cellpadding='2'>
                <tr>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_contact' />
                        <input type='hidden' name='level1' value='contact' />
                        <input type='hidden' name='employer_id' value='" . $contact->contact_employer_id . "' />
                        <input type='hidden' name='department_id' value='" . $contact->contact_department_id . "' />
                        <input type='hidden' name='add_confirmed' value='true' />
                        <input type='hidden' name='continue' value='Add Contact Now' />
                        <input type='hidden' name='contact' value='" . packObject($contact) . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='' />
                        <input type='hidden' name='menu_select' value='' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form>
                </td>
                </tr>
                </table><b>&nbsp;");
}

/*
 Function below is called when we have found an exact match of name for a company we
 are trying to add.  Since we can't have two companies in the database with the same
 name, the user is given the choice of either editing the existing company, or returning
 to the main menu.
*/

function promptEditCompany($msg, $employer_id)
{
	global $PHP_SELF;
	instruct($msg . "
		</b><table cellspacing='2' cellpadding='2'>
		<tr>
		<td>
		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit_contact' />
			<input type='hidden' name='level1' value='company' />
			<input type='hidden' name='employer_id' value='" . $employer_id . "' />
			<input style='width:5em;' type='submit' value='&nbsp;Yes&nbsp;' />
		</form>
		</td>
		<td>
		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='' />
			<input type='hidden' name='menu_select' value='' />
			<input style='width:5em;' type='submit' value='&nbsp;No&nbsp;' />
		</form>
		</td>
		</tr>
		</table><b>&nbsp;");
}

/*
 Function below is called when we have found an exact match of name for a division
 we are trying to add, in the company we are trying to add it to.  Since we can't have
 two divisions with the same name for the same company, the user is given the choice
 of either editing the existing division, or returning to the main menu.
*/

function promptEditDepartment($msg, $department_id)
{
	global $PHP_SELF;
	instruct($msg . "
		</b><table cellspacing='2' cellpadding='2'>
		<tr>
		<td>
		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit_contact' />
			<input type='hidden' name='level1' value='department' />
			<input type='hidden' name='department_id' value='" . $department_id . "' />
			<input style='width:5em;' type='submit' value='&nbsp;Yes&nbsp;' />
		</form>
		</td>
		<td>
		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='' />
			<input type='hidden' name='menu_select' value='' />
			<input style='width:5em;' type='submit' value='&nbsp;No&nbsp;' />
		</form>
		</td>
		</tr>
		</table><b>&nbsp;");
}

/*
 Function below is called when we have found an exact match of name for a contact,
 within a company or division, that we are trying to add.  Since we can't have 
 two contacts with the same name for the same company/division, the user is given
 the choice of either editing the existing division, or returning to the main menu.
*/

function promptEditContact($msg, $contact_id)
{
	global $PHP_SELF;
	instruct($msg . "
		</b><table cellspacing='2' cellpadding='2'>
		<tr>
		<td>
		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='edit_contact' />
			<input type='hidden' name='level1' value='contact' />
			<input type='hidden' name='contact_id' value='" . $contact_id . "' />
			<input style='width:5em;' type='submit' value='&nbsp;&Yes&nbsp;' />
		</form>
		</td>
		<td>
		<form method='post' action='$PHP_SELF'>
			<input type='hidden' name='select' value='' />
			<input type='hidden' name='menu_select' value='' />
			<input style='width:5em;' type='submit' value='&nbsp;No&nbsp;' />
		</form>
		</td>
		</tr>
		</table><b>&nbsp;");
}

function checkCompanyChanges($initial_contact, $attempted_save_contact)
{
	$current_contact = new EditContact('');
	$current_contact->loadCompanyValues($initial_contact->employer_id);

	$changes_made = FALSE;

	// First create the changes_array variable.
	$changes_array["company_name"] = $attempted_save_contact->company_name;
	$changes_array["company_website"] = $attempted_save_contact->company_website;
	$changes_array["company_description"] = $attempted_save_contact->company_description;
	$changes_array["company_street_address_1"] = $attempted_save_contact->company_street_address_1;
	$changes_array["company_street_address_2"] = $attempted_save_contact->company_street_address_2;
	$changes_array["company_street_address_3"] = $attempted_save_contact->company_street_address_3;
	$changes_array["company_city"] = $attempted_save_contact->company_city;
	$changes_array["company_region_id"] = $attempted_save_contact->company_region_id;
	$changes_array["company_provstate_id"] = $attempted_save_contact->company_provstate_id;
	$changes_array["company_country_id"] = $attempted_save_contact->company_country_id;
	$changes_array["company_postal_code"] = $attempted_save_contact->company_postal_code;
	$changes_array["company_phone"] = $attempted_save_contact->company_phone;
	$changes_array["company_fax"] = $attempted_save_contact->company_fax;
	$changes_array["company_email"] = $attempted_save_contact->company_email;

	/* 
	 If one of the flags array's is set, but not the other one, something has changed, so record all of
	 the user's current flag settings.  Otherwise, they are both arrays.  If there is a flag in one array
	 but not the other, something has changed, and we must record.
	*/

	if (is_array($attempted_save_contact->company_flags))
	{
		foreach($attempted_save_contact->company_flags AS $cf)
		{
			$changes_array["company_flags"][] = $cf;
		}
	}
	else
	{
		$changes_array["company_flags"] = array();
	}

	$changes_array["company_industry_id"] = $attempted_save_contact->company_industry_id;
	$changes_array["company_size_id"] = $attempted_save_contact->company_size_id;
	$changes_array["company_name_legitimate"] = $attempted_save_contact->company_name_legitimate;
	$changes_array["company_type_id"] = $attempted_save_contact->company_type_id;
	$changes_array["company_status"] = $attempted_save_contact->company_status;

	if (is_array($attempted_save_contact->company_note_ids))
	{
		foreach($attempted_save_contact->company_note_ids AS $cni)
		{
			$changes_array["company_note_ids"][] = $cni;
		}
	}
	else
	{
		$changes_array["company_note_ids"] = array();
	}

	if (is_array($attempted_save_contact->company_delete_note_ids))
	{
		foreach($attempted_save_contact->company_delete_note_ids AS $cdni)
		{
			$changes_array["company_delete_note_ids"][] = $cdni;
		}
	}
	else
	{
		$changes_array["company_delete_note_ids"] = array();
	}

	$changes_array["company_change_by"] = $current_contact->company_change_by_1;


	// Set the changes_made variable here, if it needs to be set.
	if ($current_contact->company_name != $initial_contact->company_name)
		$changes_made = TRUE;
	
	if ($current_contact->company_website != $initial_contact->company_website)
		$changes_made = TRUE;

	if ($current_contact->company_description != $initial_contact->company_description)
		$changes_made = TRUE;

	if ($current_contact->company_street_address_1 != $initial_contact->company_street_address_1)
		$changes_made = TRUE;

	if ($current_contact->company_street_address_2 != $initial_contact->company_street_address_2)
		$changes_made = TRUE;

	if ($current_contact->company_street_address_3 != $initial_contact->company_street_address_3)
		$changes_made = TRUE;

	if ($current_contact->company_city != $initial_contact->company_city)
		$changes_made = TRUE;

	if ($current_contact->company_region_id != $initial_contact->company_region_id)
		$changes_made = TRUE;

	if ($current_contact->company_provstate_id != $initial_contact->company_provstate_id)
		$changes_made = TRUE;
	
	if ($current_contact->company_country_id != $initial_contact->company_country_id)
		$changes_made = TRUE;

	if ($current_contact->company_postal_code != $initial_contact->company_postal_code)
		$changes_made = TRUE;

	if ($current_contact->company_phone != $initial_contact->company_phone)
		$changes_made = TRUE;

	if ($current_contact->company_fax != $initial_contact->company_fax)
		$changes_made = TRUE;

	if ($current_contact->company_email != $initial_contact->company_email)
		$changes_made = TRUE;


	if (is_array($current_contact->company_flags) XOR is_array($initial_contact->company_flags))
	{
		$changes_made = TRUE;
	}
	elseif (is_array($current_contact->company_flags) && is_array($initial_contact->company_flags))
	{
		foreach ($current_contact->company_flags AS $cf)
		{
			if (!in_array($cf, $initial_contact->company_flags))
			{
				$changes_made = TRUE;
				break;
			}
		}
		foreach ($initial_contact->company_flags AS $cf)
		{
			if (!in_array($cf, $current_contact->company_flags))
			{
				$changes_made = TRUE;
				break;
			}
		}
	}
		
	if ($current_contact->company_industry_id != $initial_contact->company_industry_id)
		$changes_made = TRUE;

	if ($current_contact->company_size_id != $initial_contact->company_size_id)
		$changes_made = TRUE;

	if ($current_contact->company_name_legitimate != $initial_contact->company_name_legitimate)
		$changes_made = TRUE;

	if ($current_contact->company_type_id != $initial_contact->company_type_id)
		$changes_made = TRUE;

	if ($current_contact->company_status != $initial_contact->company_status)
		$changes_made = TRUE;

	if (is_array($current_contact->company_note_ids) XOR is_array($initial_contact->company_note_ids))
	{
		$changes_made = TRUE;
	}
	elseif (is_array($current_contact->company_note_ids) && is_array($initial_contact->company_note_ids))
	{
		foreach ($current_contact->company_note_ids AS $cni)
		{
			if (!in_array($cni, $initial_contact->company_note_ids))
			{
				$changes_made = TRUE;
				break;
			}
		}
		
		foreach ($initial_contact->company_note_ids AS $cni)
		{
			if (!in_array($cni, $current_contact->company_note_ids))
			{
				$changes_made = TRUE;
				break;
			}
		}
	}

	$changes_array["company_changes_made"] = $changes_made;

	return ($changes_array);
}

function checkDivisionchanges($initial_contact, $attempted_save_contact)
{
	$current_contact = new EditContact('');
	$current_contact->loadDepartmentValues($initial_contact->department_id);

	// First create the changes_array variable.
	$changes_array["department_company_id"] = $attempted_save_contact->department_company_id;
	$changes_array["department_name"] = $attempted_save_contact->department_name;
	$changes_array["department_website"] = $attempted_save_contact->department_website;
	$changes_array["department_description"] = $attempted_save_contact->department_description;
	$changes_array["department_location_info"] = $attempted_save_contact->department_location_info;
	$changes_array["department_street_address_1"] = $attempted_save_contact->department_street_address_1;
	$changes_array["department_street_address_2"] = $attempted_save_contact->department_street_address_2;
	$changes_array["department_street_address_3"] = $attempted_save_contact->department_street_address_3;
	$changes_array["department_city"] = $attempted_save_contact->department_city;
	$changes_array["department_region_id"] = $attempted_save_contact->department_region_id;
	$changes_array["department_provstate_id"] = $attempted_save_contact->department_provstate_id;
	$changes_array["department_country_id"] = $attempted_save_contact->department_country_id;
	$changes_array["department_postal_code"] = $attempted_save_contact->department_postal_code;
	$changes_array["department_phone"] = $attempted_save_contact->department_phone;
	$changes_array["department_fax"] = $attempted_save_contact->department_fax;
	$changes_array["department_email"] = $attempted_save_contact->department_email;

	// Have we got any location information?
	$changes_array["department_location_changed"] = (trim($attempted_save_contact->department_street_address_1) 
	|| trim($attempted_save_contact->department_street_address_2) || trim($attempted_save_contact->department_street_address_3) 
	|| trim($attempted_save_contact->department_city) || trim($attempted_save_contact->department_region_id)
	|| trim($attempted_save_contact->department_provstate_id) || trim($attempted_save_contact->department_country_id) 
	|| trim($attempted_save_contact->department_postal_code));

	/* 
	 If one of the flags array's is set, but not the other one, something has changed, so record all of
	 the user's current flag settings.  Otherwise, they are both arrays.  If there is a flag in one array
	 but not the other, something has changed, and we must record.
	*/

	if (is_array($attempted_save_contact->department_flags))
	{
		foreach($attempted_save_contact->department_flags AS $cf)
		{
			$changes_array["department_flags"][] = $cf;
		}
	}
	else
	{
		$changes_array["department_flags"] = array();
	}

	$changes_array["department_industry_id"] = $attempted_save_contact->department_industry_id;
	$changes_array["department_size_id"] = $attempted_save_contact->department_size_id;
	$changes_array["department_name_legitimate"] = $attempted_save_contact->department_name_legitimate;
	$changes_array["department_status"] = $attempted_save_contact->department_status;

	if (is_array($attempted_save_contact->department_note_ids))
	{
		foreach($attempted_save_contact->department_note_ids AS $cni)
		{
			$changes_array["department_note_ids"][] = $cni;
		}
	}
	else
	{
		$changes_array["department_note_ids"] = array();
	}

	if (is_array($attempted_save_contact->department_delete_note_ids))
	{
		foreach($attempted_save_contact->department_delete_note_ids AS $dni)
		{
			$changes_array["department_delete_note_ids"][] = $dni;
		}
	}
	else
	{
		$changes_array["department_delete_note_ids"] = array();
	}

	$changes_array["department_change_by"] = $current_contact->department_change_by_1;

	
	// Now set the changes_made variable if needed.

	if ($current_contact->department_company_id != $initial_contact->department_company_id)
		$changes_made = TRUE;

	if ($current_contact->department_name != $initial_contact->department_name)
		$changes_made = TRUE;
	
	if ($current_contact->department_website != $initial_contact->department_website)
		$changes_made = TRUE;

	if ($current_contact->department_description != $initial_contact->department_description)
		$changes_made = TRUE;

	if ($current_contact->department_location_info != $initial_contact->department_location_info)
		$changes_made = TRUE;

	if ($current_contact->department_street_address_1 != $initial_contact->department_street_address_1)
		$changes_made = TRUE;

	if ($current_contact->department_street_address_2 != $initial_contact->department_street_address_2)
		$changes_made = TRUE;

	if ($current_contact->department_street_address_3 != $initial_contact->department_street_address_3)
		$changes_made = TRUE;

	if ($current_contact->department_city != $initial_contact->department_city)
		$changes_made = TRUE;

	if ($current_contact->department_region_id != $initial_contact->department_region_id)
		$changes_made = TRUE;

	if ($current_contact->department_provstate_id != $initial_contact->department_provstate_id)
		$changes_made = TRUE;
	
	if ($current_contact->department_country_id != $initial_contact->department_country_id)
		$changes_made = TRUE;

	if ($current_contact->department_postal_code != $initial_contact->department_postal_code)
		$changes_made = TRUE;

	if ($current_contact->department_phone != $initial_contact->department_phone)
		$changes_made = TRUE;

	if ($current_contact->department_fax != $initial_contact->department_fax)
		$changes_made = TRUE;

	if ($current_contact->department_email != $initial_contact->department_email)
		$changes_made = TRUE;

	if (is_array($current_contact->department_flags) XOR is_array($initial_contact->department_flags))
	{
		$changes_made = TRUE;
	}
	elseif (is_array($current_contact->department_flags) && is_array($current_contact->department_flags))
	{
		foreach ($current_contact->department_flags AS $cf)
		{
			if (!in_array($cf, $initial_contact->department_flags))
			{
				$changes_made = TRUE;
				break;
			}
		}
		foreach ($initial_contact->department_flags AS $cf)
		{
			if (!in_array($cf, $current_contact->department_flags))
			{
				$changes_made = TRUE;
				break;
			}
		}
	}
		
	if ($current_contact->department_industry_id != $initial_contact->department_industry_id)
		$changes_made = TRUE;

	if ($current_contact->department_size_id != $initial_contact->department_size_id)
		$changes_made = TRUE;

	if ($current_contact->department_name_legitimate != $initial_contact->department_name_legitimate)
		$changes_made = TRUE;

	if ($current_contact->department_status != $initial_contact->department_status)
		$changes_made = TRUE;

	if (is_array($current_contact->department_note_ids) XOR is_array($initial_contact->department_note_ids))
	{
		$changes_made = TRUE;
	}
	elseif (is_array($current_contact->department_note_ids) && is_array($initial_contact->department_note_ids))
	{
		foreach ($current_contact->department_note_ids AS $cni)
		{
			if (!in_array($cni, $initial_contact->department_note_ids))
			{
				$changes_made = TRUE;
				break;
			}
		}
		
		foreach ($initial_contact->department_note_ids AS $cni)
		{
			if (!in_array($cni, $current_contact->department_note_ids))
			{
				$changes_made = TRUE;
				break;
			}
		}
	}
	
	$changes_array["department_changes_made"] = $changes_made;
	return ($changes_array);
}
		
function checkContactChanges($initial_contact, $attempted_save_contact)
{
	$current_contact = new EditContact('');
        $current_contact->loadContactValues($initial_contact->contact_id);

	// First create the changes_array variable.
	$changes_array["contact_employer_id"] = $attempted_save_contact->contact_employer_id;
	$changes_array["contact_department_id"] = $attempted_save_contact->contact_department_id;
	$changes_array["contact_title"] = $attempted_save_contact->contact_title;
	$changes_array["contact_first_name"] = $attempted_save_contact->contact_first_name;
	$changes_array["contact_middle_name"] = $attempted_save_contact->contact_middle_name;
	$changes_array["contact_last_name"] = $attempted_save_contact->contact_last_name;
	$changes_array["contact_called_name"] = $attempted_save_contact->contact_called_name;
	$changes_array["contact_greeting"] = $attempted_save_contact->contact_greeting;
	$changes_array["contact_email"] = $attempted_save_contact->contact_email;
	$changes_array["contact_phone"] = $attempted_save_contact->contact_phone;
	$changes_array["contact_pager"] = $attempted_save_contact->contact_pager;
	$changes_array["contact_cellphone"] = $attempted_save_contact->contact_cellphone;
	$changes_array["contact_fax"] = $attempted_save_contact->contact_fax;
	$changes_array["contact_position_title"] = $attempted_save_contact->contact_position_title;
	$changes_array["contact_department_name"] = $attempted_save_contact->contact_department_name;
	$changes_array["contact_location_info"] = $attempted_save_contact->contact_location_info;
	$changes_array["contact_street_address_1"] = $attempted_save_contact->contact_street_address_1;
	$changes_array["contact_street_address_2"] = $attempted_save_contact->contact_street_address_2;
	$changes_array["contact_street_address_3"] = $attempted_save_contact->contact_street_address_3;
	$changes_array["contact_city"] = $attempted_save_contact->contact_city;
	$changes_array["contact_region_id"] = $attempted_save_contact->contact_region_id;
	$changes_array["contact_provstate_id"] = $attempted_save_contact->contact_provstate_id;
	$changes_array["contact_country_id"] = $attempted_save_contact->contact_country_id;
	$changes_array["contact_postal_code"] = $attempted_save_contact->contact_postal_code;

	// Have we gotten any location information?

	$changes_array["contact_location_changed"] = (trim($attempted_save_contact->contact_street_address_1) 
	|| trim($attempted_save_contact->contact_street_address_2) || trim($attempted_save_contact->contact_street_address_3) 
	|| trim($attempted_save_contact->contact_city) || trim($attempted_save_contact->contact_region_id)
	|| trim($attempted_save_contact->contact_provstate_id) || trim($attempted_save_contact->contact_country_id) 
	|| trim($attempted_save_contact->contact_postal_code));

	/* 
	 If one of the flags array's is set, but not the other one, something has changed, so record all of
	 the user's current flag settings.  Otherwise, they are both arrays.  If there is a flag in one array
	 but not the other, something has changed, and we must record.
	*/

	if (is_array($attempted_save_contact->contact_flags))
	{
		foreach($attempted_save_contact->contact_flags AS $cf)
		{
			$changes_array["contact_flags"][] = $cf;
		}
	}
	else
	{
		$changes_array["contact_flags"] = array();
	}

	if (is_array($attempted_save_contact->contact_note_ids))
	{
		foreach($attempted_save_contact->contact_note_ids AS $cni)
		{
			$changes_array["contact_note_ids"][] = $cni;
		}
	}
	else
	{
		$changes_array["contact_note_ids"] = array();
	}

	if (is_array($attempted_save_contact->contact_delete_note_ids))
	{
		foreach($attempted_save_contact->contact_delete_note_ids AS $cni)
		{
			$changes_array["contact_delete_note_ids"][] = $cni;
		}
	}
	else
	{
		$changes_array["contact_delete_note_ids"] = array();
	}

	$changes_array["contact_change_by_1"] = $current_contact->contact_change_by_1;



	// Set the changes_made variable if needed.
        if ($current_contact->contact_employer_id != $initial_contact->contact_employer_id)
		$changes_made = TRUE;

	if ($current_contact->contact_department_id != $initial_contact->contact_department_id)
		$changes_made = TRUE;

	if ($current_contact->contact_title != $initial_contact->contact_title)
		$changes_made = TRUE;

	if ($current_contact->contact_first_name != $initial_contact->contact_first_name)
		$changes_made = TRUE;
	
	if ($current_contact->contact_middle_name != $initial_contact->contact_middle_name)
		$changes_made = TRUE;

	if ($current_contact->contact_last_name != $initial_contact->contact_last_name)
		$changes_made = TRUE;

	if ($current_contact->contact_called_name != $initial_contact->contact_called_name)
		$changes_made = TRUE;

	if ($current_contact->contact_greeting != $initial_contact->contact_greeting)
		$changes_made = TRUE;

	if ($current_contact->contact_email != $initial_contact->contact_email)
		$changes_made = TRUE;

	if ($current_contact->contact_phone != $initial_contact->contact_phone)
		$changes_made = TRUE;

	if ($current_contact->contact_pager != $initial_contact->contact_pager)
		$changes_made = TRUE;

	if ($current_contact->contact_cellphone != $initial_contact->contact_cellphone)
		$changes_made = TRUE;

	if ($current_contact->contact_fax != $initial_contact->contact_fax)
		$changes_made = TRUE;

	if ($current_contact->contact_position_title != $initial_contact->contact_position_title)
		$changes_made = TRUE;

	if ($current_contact->contact_department_name != $initial_contact->contact_department_name)
		$changes_made = TRUE;

	if ($current_contact->contact_location_info != $initial_contact->contact_location_info)
		$changes_made = TRUE;

	if ($current_contact->contact_street_address_1 != $initial_contact->contact_street_address_1)
		$changes_made = TRUE;

	if ($current_contact->contact_street_address_2 != $initial_contact->contact_street_address_2)
		$changes_made = TRUE;

	if ($current_contact->contact_street_address_3 != $initial_contact->contact_street_address_3)
		$changes_made = TRUE;

	if ($current_contact->contact_city != $initial_contact->contact_city)
		$changes_made = TRUE;

	if ($current_contact->contact_region_id != $initial_contact->contact_region_id)
		$changes_made = TRUE;

	if ($current_contact->contact_provstate_id != $initial_contact->contact_provstate_id)
		$changes_made = TRUE;

	if ($current_contact->contact_country_id != $initial_contact->contact_country_id)
		$changes_made = TRUE;

	if ($current_contact->contact_postal_code != $initial_contact->contact_postal_code)
		$changes_made = TRUE;

	if (is_array($current_contact->contact_flags) XOR is_array($initial_contact->contact_flags))
	{
		$changes_made = TRUE;
	}
	elseif (is_array($current_contact->contact_flags) && is_array($initial_contact->contact_flags))
	{
		foreach ($current_contact->contact_flags AS $cf)
		{
			if (!in_array($cf, $initial_contact->contact_flags))
			{
				$changes_made = TRUE;
				break;
			}
		}
		foreach ($initial_contact->contact_flags AS $cf)
		{
			if (!in_array($cf, $current_contact->contact_flags))
			{
				$changes_made = TRUE;
				break;
			}
		}
	}

	if (is_array($current_contact->contact_note_ids) XOR is_array($initial_contact->contact_note_ids))
	{
		$changes_made = TRUE;
	}
	elseif (is_array($current_contact->contact_note_ids) && is_array($initial_contact->contact_note_ids))
	{
		foreach ($current_contact->contact_note_ids AS $cni)
		{
			if (!in_array($cni, $initial_contact->contact_note_ids))
			{
				$changes_made = TRUE;
				break;
			}
		}
		
		foreach ($initial_contact->contact_note_ids AS $cni)
		{
			if (!in_array($cni, $current_contact->contact_note_ids))
			{
				$changes_made = TRUE;
				break;
			}
		}
	}
	
	$changes_array["contact_changes_made"] = $changes_made;

	return($changes_array);
}

?>
