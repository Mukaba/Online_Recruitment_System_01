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
 | Filename: mail_contact_lp.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This page does two things: It checks for confirmation that      |
 | the user would like to send the new login and passowrd, and it mails the     |
 | login and password.                                                          |
 +------------------------------------------------------------------------------+

*/


if (!$add_confirmed)
{
	echo("<form method='post' action='" . $PHP_SELF . "' name='form0'>");
	echo("<input type='hidden' name='select' value='make_login_info' />");
	echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
	echo("<input type='hidden' name='contact_email' value='" . $contact_email . "' />");
	echo("<input type='hidden' name='add_confirmed' value='true' />");
	echo("<input type='hidden' name='continue' value='mail_contact_lp' />");

	// this was added in, because logins can now be created through applications
	if ($come_from_apps) {
		echo ("<input type='hidden' name='option' value='$option' />");
		echo ("<input type='hidden' name='job_id' value='$job_id' />");
		echo ("<input type='hidden' name='come_from_apps' value='1' />");
	}
    //echo("</form>");			
	$sql = ("
		SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS name, ec.company_name
		FROM contact AS c, employer_company AS ec, contact_employer AS ce
		WHERE c.contact_id='" . $contact_id . "'
		AND ce.contact_id=c.contact_id
		AND ec.employer_id=ce.employer_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	$sql = ("
		SELECT DISTINCT *
		FROM employer_login
		WHERE contact_id='" . $contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($result->RecordCount())
	{
		// There is already a login ID and password for this contact.  Set the message appropriately.

		$msg = ("You are about to <b class='black'>DELETE</b> the old password for <b class='black'>" . $row["name"] . "</b> at");
		$msg .= (" <b class='black'>" . $row["company_name"] . "</b>, create a new password, and e-mail that password to the e-mail ");
		$msg .= ("address <b class='black'>" . $contact_email . "</b>.  Please make sure that this is the correct e-mail address, and");
		$msg .= (" then click 'Continue'.");
	}
	else
	{
		$msg = ("You are about to e-mail the login ID and password for <b class='black'>" . $row["name"] . "</b> at <b class='black'>");
		$msg .= ($row["company_name"] . "</b> to the e-mail address <b class='black'>" . $contact_email . "</b>.  Please make sure ");
		$msg .= ("that this is the correct e-mail address, and then click 'Continue'.");
	}
	
	echo("<table width='96%' cellspacing='0' cellpadding='0' border='0'>");
	echo("<tr class='row1'>");
		echo("<td>");
		echo("<table width='100%' cellpadding='5' cellspacing='0'>");
		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo($msg);
			echo("</td>");
		echo("</tr>");
	
		echo("<tr align='center'>");
			echo("<td width='50%' align='right'>");
			echo("<input type='submit' value='Continue' />");
            echo("</td>");
	      	echo("<td width='50%' align='left'>");
            echo("<input type='button' value='Edit Contact Info' onclick='javascript:document.editform.submit();' />");
            echo("</td>");
        echo("</tr>");
	
		echo("</table>");
		echo("</td>");
	echo("</tr>");
	echo("</table>");
    echo("</form>");
    echo("<form method='post' action='" . $PHP_SELF . "' name='editform'>");
    echo("<input type='hidden' name='select' value='edit_contact' />");
    echo("<input type='hidden' name='level1' value='contact' />");
    echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    echo("</form>");
	
}
else
{
	include('contact/login_info/admin/send_lp_mail.inc');
	unset($add_confirmed);
}
?>
