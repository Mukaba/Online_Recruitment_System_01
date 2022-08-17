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
 | Filename: make_login_info.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file determines which phase of creating login info we      |
 | are in, and loads the appropriate file for the user.                         |
 +------------------------------------------------------------------------------+

*/


include('contact/contact_functions.inc');

if ($continue == '')
{
	// They have not yet chosen a contact, so give them the list to choose from.

	include('contact/login_info/admin/contact_list.inc');
}

elseif ($continue == 'contact_chosen')
{
	// They've chosen a contact.  Check if we have an e-mail for this contact.  If not, collect one, and if so, load up the confirmation page.

    if ($contact_id)
    {
    // Check that a contact was really chosen (in case they chose a blank entry)
        $sql = ("
                SELECT DISTINCT email
                FROM contact
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        if (trim($row["email"]))
        {
            $contact_email = $row["email"];
            include('contact/login_info/admin/mail_contact_lp.inc');
        }
        else
        {
            include('contact/login_info/admin/get_contact_email.inc');
        }
    }
    else
    {
        notify("The contact you have chosen is not valid.  Please try again.\n");
        include('contact/login_info/admin/contact_list.inc');
    }
}

elseif ($continue == 'validate_contact_email')
{
	// We've got an e-mail address.  Validate it, and then load up the confirmation screen.

	$contact_email = removeSlashes($contact_email);

        if ((trim($contact_email) == '') || !(isValidEmail($contact_email)))
        {
                $error = 1;
                error("You must include a valid e-mail address.");
                include('contact/login_info/admin/get_contact_email.inc');
        }
        else
        {
                // Save this e-mail address for the contact in the database.

                $sql = ("
                        UPDATE contact
                        SET email='" . addslashes($contact_email) . "'
                        WHERE contact_id='" . $contact_id . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);

                include('contact/login_info/admin/mail_contact_lp.inc');
        }
}

elseif ($continue == 'mail_contact_lp')
{
	// They've confirmed that they would like to e-mail the contact a new login and password, so create it, and send it off.

	$sql = ("
		SELECT DISTINCT *
		FROM employer_login
		WHERE contact_id='" . $contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount())
	{
		// Set the variable below so that the e-mail being sent is worded correctly.

		$old_info_existent = true;
	}

	// Delete the old login info (if it existed).

	$sql = ("
		DELETE FROM employer_login
		WHERE contact_id='" . $contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

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
	$loginID = preg_replace("/[^\w]/", "", $loginID);

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
			WHERE login_id='" . addslashes($id_to_check) . "'
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
		'" . addslashes($loginID) . "',
		'" . addslashes(crypt($password, "EM")) . "'
		)
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	include('contact/login_info/admin/send_lp_mail.inc');
}

?>
