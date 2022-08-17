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
 | Filename: add_contact.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This is the file that does most of the decision making          |
 | involved with adding a contact.  It decides whether or not the user has      |
 | filled out the contact form, and if they haven't, it displays the form.  If  |
 | they have filled out the form, then it checks all of the inputs to make      |
 | sure they are valid, checks and makes sure they aren't adding a contact for  |
 | this company/division that isn't already in the database, and prompts them   |
 | for confirmation to add the contact.                                         |
 +------------------------------------------------------------------------------+

*/


$error = 0;

if (!$employer_id || $java_reload || !$department_id)
{
	/*
	 The user has not chosen a company/division to add a contact to yet, so show them the list
	 to choose from.
	*/
	
	include('contact/add/contact/company_department_list.inc');
}

if ($employer_id && !$java_reload && $department_id && !$continue)
{
	/*
	 The user has chosen a company to add the contact to, but hasn't filled out
	 the form, so show them the form.
	*/

    if (!is_numeric(trim($employer_id)) || !is_numeric(trim($department_id)))
    {
        if (!is_numeric($employer_id))
        {
            $employer_id = '';
            $department_id = '';
        }
        elseif (!is_numeric($department_id))
        {
            $department_id = '';
        }

        error("You have attempted to choose a company and a division not included in the list below.  Please try again.");
        include('contact/add/contact/company_department_list.inc');
    }
    else
    { 
        include('contact/add/contact/add_contact_form.inc');
    }
}

if ($employer_id && $continue == 'Continue')
{
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

    //Change "s to 's in the name
    $contact_first_name = preg_replace("/\"/", "'", $contact_first_name);
    $contact_last_name = preg_replace("/\"/", "'", $contact_last_name);

	// Trim all the empty elements out of the contact_flags array.
	$contact_flags = trim_array($contact_flags);

	// Save the inputs into the contact object.
	$contact = new AddContact('');
	$contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
    $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, 
	$contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
	$contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
	$contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
	$contact_department_comments, $contact_flags);

	// Begin validating inputs here.
	if ((trim($contact->contact_first_name) == '') || (trim($contact->contact_last_name) == ''))
	{
		$error = 1;
		error("To add a new contact, you must specify both a first and last name.");
		include('contact/add/contact/add_contact_form.inc');
	}
	elseif ((trim($contact->contact_email) != '') && !(isValidEmail($contact->contact_email)))
	{
		$error = 1;
		error("You must include a valid set of e-mail addresses, or none at all.");
		include('contact/add/contact/add_contact_form.inc');
	}
	elseif ((trim($contact->contact_phone) != '') && (trim($contact->contact_phone) != USE_COMPANY) 
	&& (trim($contact->contact_phone) != USE_DEPARTMENT) && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_phone)))))
	{
		$error = 1;
		error("You must include a valid phone number with an area code, or none at all.");
		include('contact/add/contact/add_contact_form.inc');
	}
	elseif ((trim($contact->contact_fax) != '') && (trim($contact->contact_fax) != USE_COMPANY)
        && (trim($contact->contact_fax) != USE_DEPARTMENT) && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_fax)))))
        {
		$error = 1;
		error("You must include a valid fax number with an area code, or none at all.");
		include('contact/add/contact/add_contact_form.inc');
	}
	elseif ((trim($contact->contact_pager) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_pager)))))
	{
		$error = 1;
		error("You must include a valid pager number with an area code, or none at all.");
		include('contact/add/contact/add_contact_form.inc');
	}
	elseif ((trim($contact->contact_cellphone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->contact_cellphone)))))
	{
		$error = 1;
		error("You must include a valid cellphone number with an area code, or none at all.");
		include('contact/add/contact/add_contact_form.inc');
	}

        if ($error == 0)
        {
                include('contact/add/contact/confirm_contact_inputs.inc');
        }

}

if ($continue == 'add_confirmed')
{
        if ($add_confirmed && is_string($contact))
        {
                $contact = unpackObject($contact);
        }
        include('contact/add/contact/confirm_contact_inputs.inc');
}

elseif ($continue == 'email_contact_lp')
{
	/*
	 The user would like to automatically generate a login and password for this contact, and then e-mail it
	 to them.  Check first and see if we have an e-mail address for this contact.  If not, prompt the user
	 for one.
	*/

	$sql = ("
		SELECT DISTINCT email
		FROM contact
		WHERE contact_id='" . $contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	if (trim($row["email"]))
	{
		// We have an e-mail address.  Prompt for confirmation.
		$contact_email = $row["email"];

		include('contact/add/contact/email_lp_confirm.inc');
	}
	else
	{
		// We don't have an e-mail address.  Get one, and then validate it.
		include('contact/add/contact/get_contact_email.inc');
	}
}

elseif ($continue == 'validate_contact_email')
{
	// We've got an e-mail address.  Validate it, and then load up the confirmation screen.
	if ((trim($contact_email) == '') || !(isValidEmail($contact_email)))
    {
        $error = 1;
        error("You must include a valid e-mail address.");
        include('contact/add/contact/get_contact_email.inc');
    }
    else
    {
        // Save this e-mail address for the contact in the database.
        $sql = ("
                UPDATE contact
                SET email='" . $contact_email . "'
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        include('contact/add/contact/email_lp_confirm.inc');
    }
}

elseif ($continue == 'mail_contact_lp')
{
    /*
     We're all set.  Check to make sure that we don't already have a login ID and password for this 
     contact in the database, and if we don't, proceed.  Create the login ID and password, insert them 
     into the database, mail them out, and let the user know everything was done successfully.  I love it 
     when a plan comes together.
     */

    $sql = ("
            SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS name
            FROM contact AS c, employer_login AS el
            WHERE c.contact_id='" . $contact_id . "'
            AND el.contact_id=c.contact_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {
        $row = $result->FetchRow();
        error("A login ID and password have already been created and e-mailed for <b>" . $row["name"] . "</b>.  You may
                only do this once per contact.");
    }
    else
    {
        /*
         The call below is dangerous.  Srand is already called once in our script (potentially) by the colorscheme
         random scheme generator.  However, this can't be relied upon, and shouldn't make a huge difference. 
         As long as the seed below is called only once for the entire password generation process, we shouldn't
         run into problems.
         */

        srand((double)microtime()*10000000);

        $password_unique = '0';

        while (!$password_unique)
        {
            $password = generatePassword();

            $sql = ("
                    SELECT *
                    FROM employer_login
                    WHERE password='" . crypt($password, "EM") . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if (!$result->RecordCount())
            {
                $password_unique = true;
            }
        }

        // We've got a unique password, now generate a login ID.
        $sql = ("
                SELECT DISTINCT first_name, last_name
                FROM contact
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $loginID = strtolower($row["first_name"][0] . $row["last_name"]);

        if (strlen($loginID) > 18)
        {
            $loginID = substr($loginID, 0, 18);
        }

        $loginID_unique = '0';
        $count = 0;

        $id_to_check = $loginID;

        while (!$loginID_unique)
        {
            $sql = ("
                    SELECT DISTINCT login_id
                    FROM employer_login
                    WHERE login_id='" . $id_to_check . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $id_to_check = $loginID . $count++;
            }
            else
            {
                $loginID_unique = true;
            }
        }

        $loginID = $id_to_check;

        // We now have a unique password and loginID.  Save them in the database and include the script to mail them off.

        $sql = ("
                SELECT DISTINCT employer_id
                FROM contact_employer
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $sql = ("
                INSERT INTO employer_login
                (employer_id, contact_id, login_id, password)
                VALUES
                (
                 '" . $row["employer_id"] . "',
                 '" . $contact_id . "',
                 '" . $loginID . "',
                 '" . crypt($password, "EM") . "'
                )
                ");	
            $result = $GLOBALS['dbh']->Execute($sql);

        include('contact/add/contact/send_lp_mail.inc');
    }
}

elseif ($continue == 'Use Company Fax')
{
    // They want to use the company's fax number for this contact, so simply reload the page.
    $contact_fax = USE_COMPANY;

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Use Division Fax')
{
    // They want to use the division's fax number for this contact, so simply reload the page.
    $contact_fax = USE_DEPARTMENT;

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Specify contact fax')
{
    /*
     They want to stop using the company fax number and specify
     one for the contact.
     */

    $contact_fax = '';

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Use Company Location')
{
    // They want to use the company's location for this contact, so simply reload the page.
    $contact_location_info = USE_COMPANY;

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Use Division Location')
{
    // They want to use the division's location for this contact, so simply reload the page.
    $contact_location_info = USE_DEPARTMENT;

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Specify contact location')
{
    /*
     They to stop using the division or company information
     and specify the contact's own.
     */

    $contact_location_info = '';
    $contact_street_address_1 = '';
    $contact_street_address_2 = '';
    $contact_street_address_3 = '';
    $contact_country_id = '';
    $contact_provstate_id = '';
    $contact_region_id = '';
    $contact_city = '';
    $contact_postal_code = '';

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Use Company Phone')
{
    // They want to use the company's phone number for this contact, so simply reload the page.
    $contact_phone = USE_COMPANY;

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Use Division Phone')
{
    // They want to use the division's phone number for this contact, so simply reload the page.
    $contact_phone = USE_DEPARTMENT;

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

elseif ($continue == 'Specify contact phone')
{
    /*
     They want to stop using the company phone number and specify
     one for the division.
     */

    $contact_phone = '';

    $contact = new AddContact('');
    $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name,
            $contact_middle_name, $contact_last_name, $contact_called_name, $contact_greeting, $contact_email,
            $contact_phone, $contact_pager, $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
            $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
            $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_new_note, 
            $contact_department_comments, $contact_flags);

    include('contact/add/contact/add_contact_form.inc');
}

if ($continue == 'Add Contact Now')
{
    $contact = unpackObject($contact);
    include('contact/add/contact/save_contact.inc');
}

if ($continue == 'Edit Contact')
{
    $contact = unpackObject($contact);
    include('contact/add/contact/add_contact_form.inc');
}

?>
