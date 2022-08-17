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
 | Filename: confirm_account_for_password.inc                                   |
 +------------------------------------------------------------------------------+
 | Description: This page simply confirms the account that the employer will    |
 | be getting a password for.                                                   |
 +------------------------------------------------------------------------------+

*/


echo("<table border='0' cellspacing='0' cellpadding='0'>");
echo("<tr class='row1'>");
	echo("<td>");
	echo("<table width='100%' cellpadding='5' cellspacing='0'>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("The account that matches the e-mail you entered (<b class='black'>" . $employer_email . "</b>) is for <b class='black'>");
		echo($contact_name . "</b> working at <b class='black'>" . $company_name . "</b>.  If this is the correct account, please hit");
		echo(" <b class='black'>Confirm</b> to have a new password e-mailed to you.  If you mistyped the e-mail address, please hit ");
		echo("<b class='black'>Cancel</b> to re-type the e-mail.");
		echo("</td>\n");
	echo("</tr>\n");

	echo("<tr>");
		echo("<td align='right'>");
            echo("<form method='post' action='" . $PHP_SELF . "' name='form'>");
            echo("<input type='hidden' name='PDF' value='1' />");
            echo("<input type='hidden' name='select' value='log_me_out' />");
            echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
            echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");

            echo("<input type='submit' name='continue' value='Confirm' />");
            echo("</form>\n");
		echo("</td>\n");
		echo("<td align='left'>");
            echo("<form method='post' action='" . $PHP_SELF . "'>");
            echo("<input type='hidden' name='select' value='forgotten_password' />");
            echo("<input type='submit' name='continue' value='Cancel' />");
            echo("</form>\n");
        echo("</td>\n");
	echo("</tr>\n");

	echo("</table>\n");
	echo("</td>\n");
echo("</tr>\n");
echo("</table>\n");

?>
