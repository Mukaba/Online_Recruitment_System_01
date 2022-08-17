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
 | Filename: password_change_form.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This page simply gets the new password from the employer.       |
 +------------------------------------------------------------------------------+

*/


echo("<form method='post' action='" . $PHP_SELF . "' name='form'>");
echo("<input type='hidden' name='select' value='change_login_password' />");
echo("<input type='hidden' name='continue' value='new_password' />");
echo("<input type='hidden' name='password_changed' value='true' />");

echo("<table border='0' cellspacing='0' cellpadding='0'>");
echo("<tr class='row1'>");
	echo("<td>");
	echo("<table width='100%' cellpadding='5' cellspacing='0'>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("You are about to change the password for the login ID <b class='black'>" . $auth->login . "</b> 
			(you are currently logged in under this ID).  Please type your new password in the two boxes below.  
			Note: passwords must be at least four characters in length, and ten characters maximum.");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("Your new password:&nbsp;&nbsp;");
		echo("<input type='password' name='password1' size='10' maxlength='10' />");
		echo("</td>");
	echo("</tr>");
	
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("New password again:&nbsp;&nbsp;");
		echo("<input type='password' name='password2' size='10' maxlength='10' />");
		echo("</td>");
	echo("</tr>");

	echo("<tr align='center'>");
		echo("<td colspan='2' align='center'>");
		echo("<input type='submit' value='Save New Password' />");
                echo("</td>");
	echo("</tr>");

	echo("</table>");
	echo("</td>");
echo("</tr>");
echo("</table>");
echo("</form>");

?>
