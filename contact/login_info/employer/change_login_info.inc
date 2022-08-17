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
 | Filename: change_login_info.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file is called when an employer would like to change       |
 | their password.  It loads the appropriate files.                             |
 +------------------------------------------------------------------------------+

*/


$sql = ("
	SELECT DISTINCT contact_id
	FROM employer_login
	WHERE login_id = ? 
	");
$result = $GLOBALS['dbh']->Execute($sql, array($auth->login));
$row = $result->FetchRow();

$contact_id = $row["contact_id"];

if ($continue == '')
{
	// They haven't filled any form out yet, so load up the password change form.

	include('contact/login_info/employer/password_change_form.inc');
}

elseif ($continue == 'new_password')
{
	/*
	 We've been given a new password.  It has already been either determine invalid (and thus error will
	 be set, or it has been saved into the database and we just need to notify the user of success.
	*/

	if ($error == 1)
	{
		// We've been passed an error_msg.  Print it with the error function, and reload the form.

		error($error_msg);
		include('contact/login_info/employer/password_change_form.inc');
	}
	else
	{
		// The password has been saved successfully.  Notify user of success.

		notify("Your password has been successfully updated.  From now on, you will need to use this password when logging in.");
	}
}

?>
