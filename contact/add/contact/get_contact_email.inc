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
 | Filename: get_contact_email.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This page simply gets a contact e-mail address from the user.   |
 +------------------------------------------------------------------------------+

*/


echo("<form method='post' action='" . $PHP_SELF . "' name='form'>");
echo("<input type='hidden' name='select' value='add_contact'>");
echo("<input type='hidden' name='contact_id' value='" . $contact_id . "'>");
echo("<input type='hidden' name='employer_id' value='" . $employer_id . "'>");
echo("<input type='hidden' name='department_id' value='" . $department_id . "'>");
echo("<input type='hidden' name='level1' value='contact'>");
echo("<input type='hidden' name='continue' value='validate_contact_email'>");

echo("<table border='0' cellspacing='0' cellpadding='0' border='0'>");
echo("<tr class='row1'>");
	echo("<td>");
	echo("<table width='100%' cellpadding='5' cellspacing='0'>");
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("You have not yet entered an e-mail address for this contact.  In order to e-mail their login information, an e-mail address will
			be needed.  Please enter one below and click 'Continue'.");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("Contact's E-mail Address:&nbsp;&nbsp;");
		echo("<input type='text' name='contact_email' size='40' maxlength='100' value='" . $contact_email . "'>");
		echo("</td>");
	echo("</tr>");

	echo("<tr align='center'>");
		echo("<td colspan='2' align='center'>");
		echo("<input type='submit' value='Continue'>");
                echo("</form>");
                echo("</td>");
	echo("</tr>");

	echo("</table>");
	echo("</td>");
echo("</tr>");
echo("</table>");

?>
