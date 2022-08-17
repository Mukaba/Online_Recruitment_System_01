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
 | Filename: interview_notify.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: Set up to email interview information to the contact for the    |
 | job once the employer has selected a shortlist.                              |
 +------------------------------------------------------------------------------+

*/

include ("misc/email.inc");
$sql = "SELECT c.email, c.first_name, c.last_name
	FROM contact AS c, employer_login AS el
	WHERE c.contact_id = '$contact_id' 
	AND c.contact_id = el.contact_id";
$result = $GLOBALS['dbh']->Execute($sql);
if ($row = $result->FetchRow()) {
	$contact_email = $row["email"];
	$contact_name = $row["first_name"] . " " . $row["last_name"];
}


echo ("<h3>Send Interview Preferences</h3>");

instruct ("Please enter any interview preferences (i.e. date, time, location, length, etc.) in the e-mail message text 
	box below. To receive a reference copy of this e-mail, click on the 'Please send me a copy' button above the message text box.");

echo ("<form method='post' action='$PHP_SELF'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='shortlist' value='1' />");
echo ("<input type='hidden' name='send_interview_prefs' value='1' />");
echo ("<input type='hidden' name='job_id' value='$job_id' />");
echo ("<input type='hidden' name='shortlist_numbers' value='$shortlist_numbers' />");

echo ("<table class='row1' border='0' cellspacing='5' cellpadding='5'>");
echo ("<tr>");
	echo ("<td>");
	echo ("<table class='row1' border='0' cellspacing='0' cellpadding='0'>");
		echo ("<tr>");
			echo ("<td align='right'><b class='black'>From:</b> &nbsp;</td>");
			echo ("<td>". implode (getContactName ($contact_id), " ") ." &lt;". getCiEmailUsingID($contact_id) ."&gt;</td>");
		echo ("</tr>");

		echo ("<tr>");
			echo ("<td>&nbsp;</td>");
			echo ("<td><input type='checkbox' name='send_self' value='1' />Please send me a copy.</td>");
		echo ("</tr>");

		echo ("<tr>");
			echo ("<td colspan='2' align='center'>");
			echo ("<br /><textarea name='mail_body' rows='22' cols='80' wrap></textarea>");
			echo ("</td>");
		echo ("</tr>");
		
		echo ("<tr>");
			echo ("<td align='center' colspan='2'><hr />");
			echo ("<input type='submit' name='send_with_prefs' value=' Send Mail ' /> &nbsp; ");
			//echo ("<input type='submit' name='send_no_prefs' value=' Cancel ' />");
			echo ("</td>");
		echo ("</tr>");
	echo ("</table>");
	echo ("</td>");
echo ("</tr>");
echo ("</table>");

echo ("</form>");


?>
