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
 | Filename: forgotten_password.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This file is responsible for collecting an e-mail address       |
 | from the user and then mailing them out their new password.                  |
 +------------------------------------------------------------------------------+

*/


if (!$continue)
{
	// They've just gotten here, get their e-mail address
	include('contact/login_info/employer/get_email_address.inc');
}

elseif ($continue == 'confirm_email')
{
    if (!trim($employer_email))
    {
        error("In order to send out a new password, an e-mail address needs to be entered.  Please enter an e-mail address and resubmit the form.");
        include('contact/login_info/employer/get_email_address.inc');
    }
    else
    {
        // They've given us an e-mail, check if it matches the database, and if so, create and send a new password
        
        $sql = ("
            SELECT DISTINCT ec.employer_id, ec.company_name, ce.contact_id, CONCAT(c.first_name, ' ', c.last_name) AS contact_name
            FROM employer_company AS ec, contact AS c, contact_employer AS ce, employer_login AS el
            WHERE ec.employer_id=ce.employer_id
            AND c.contact_id=ce.contact_id
            AND c.email = ?
            AND c.type='employer'
            AND c.contact_id=el.contact_id
            ");
        $stmt = $GLOBALS['dbh']->Prepare($sql);
        $result = $GLOBALS['dbh']->Execute($stmt, array(trim($employer_email)));

        if ($result->RecordCount())
        {
            $row = $result->FetchRow();
            $contact_id = $row["contact_id"];
            $contact_name = $row["contact_name"];
            $employer_id = $row["employer_id"];
            $company_name = $row["company_name"];

            include('contact/login_info/employer/confirm_account_for_password.inc');
        }
        else
        {
            (!$number_of_tries ? $number_of_tries = 1 : $number_of_tries++);

            if ($emails_attempted)
            {
                $emails_attempted = unpackObject(($emails_attempted));
            }

            $emails_attempted .= ($employer_email . ", ");

            // get traffic director name and email
            $sql = ("
                    SELECT CONCAT(first_name, ' ', last_name) AS name, email, phone
                    FROM contact
                    WHERE contact_id = ? 
                    ");
            $contact_results = $GLOBALS['dbh']->Execute($sql, array(TRAFFIC_DIRECTOR_ID));
            $contact_info = $contact_results->FetchRow();

            if ($number_of_tries > 3)
            {
                // They've tried to enter an e-mail address 5 or more times.  Blacklist their IP.

                $emails_attempted = substr($emails_attempted, 0, -2);

                $sql = ("
                    INSERT INTO forgot_password_blacklist
                    (blacklisted_IP, datetime_blacklisted, emails_attempted)
                    VALUES (?, ?, ?)
                    ");
                $stmt = $GLOBALS['dbh']->Prepare($sql);
                $GLOBALS['dbh']->Execute($stmt,
                        array($REMOTE_ADDR, date("Y-m-d H:i:s"),
                            trim($emails_attempted)));

                $msg = ("You have tried unsuccessfully " . $number_of_tries . " times to enter the e-mail address that matches your account.");
                $msg .= ("  For security purposes, you will now need to contact a ".BRAND_NAME." administrator before being able to log in to the system.");
                $msg .= ("  Please contact ".$contact_info['name']." (<a href='mailto:".$contact_info['email']."'>".$contact_info['email']."</a>) to have your password reset.");
                error($msg);
                echo("<br />");
            }
            else
            {
                $msg = ("The e-mail address you have entered does not match any that we have in the database.  Please try again,");
                $msg .= (" or contact ".$contact_info['name']." (<a href='mailto:".$contact_info['email']."'>".$contact_info['email']."</a>".
                ($contact_info['phone'] ? (", ".$contact_info['phone']) : "").") ");
                $msg .= ("for assistance.");
                error($msg);
                echo("<br />");

                include('contact/login_info/employer/get_email_address.inc');
            }
        }
    }
}

elseif ($continue == 'Confirm')
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

	$sql = ("
        UPDATE employer_login
		SET password = ?
		WHERE contact_id = ?
		AND employer_id = ?
        ");
	$stmt = $GLOBALS['dbh']->Prepare($sql);
    $GLOBALS['dbh']->Execute($stmt,
            array(crypt($password, "EM"), $contact_id, $employer_id));
	
	include('contact/login_info/employer/send_password_email.inc');
}

elseif ($continue == 'Cancel')
{
	// They got the wrong account by accident

	include('contact/login_info/employer/get_email_address.inc');
}

?>
