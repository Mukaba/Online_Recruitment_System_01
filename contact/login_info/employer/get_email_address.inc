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
 | Filename: get_email_address.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This page simply gets the e-mail addresses from the contact.    |
 +------------------------------------------------------------------------------+

*/


echo("<table border='0' cellspacing='0' cellpadding='0'>");
echo("<tr class='row1'>");
	echo("<td>");
    echo("<form method='post' action='" . $PHP_SELF . "' name='form'>");

    echo("<input type='hidden' name='select' value='forgotten_password' />");
    echo("<input type='hidden' name='continue' value='confirm_email' />");
    echo("<input type='hidden' name='number_of_tries' value='" . $number_of_tries . "' />");
    echo("<input type='hidden' name='emails_attempted' value='" . packObject(($emails_attempted)) . "' />");

	echo("<table width='100%' cellpadding='5' cellspacing='0'>");

	if ($incorrect_pass_four_times && !$continue_trying)
	{
		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo("You have been brought here because you have incorrectly entered your password 4 or more times.  If you would");
			echo(" like to continue attempting to login, please click BACK on your browser and continue trying.  Otherwise, read below.");
			echo("</td>\n");
		echo("</tr>\n");
	}

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("To retrieve your password, please follow the appropriate instructions below.");
		echo("</td>\n");
	echo("</tr>\n");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("<hr width='50%' />");
		echo("</td>\n");
	echo("</tr>\n");
		
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("If you are an <b class='instruct'>employer</b>, please type your e-mail address below and click the 
            continue button.");
		echo("</td>\n");
	echo("</tr>\n");

	echo("<tr>");
		echo("<td align='right'>");
		echo("E-mail address:&nbsp;&nbsp;");
		echo("<input type='text' name='employer_email' size='40' maxlength='100' />");
		echo("</td>\n");
		echo("<td align='left'>");
        echo("<input type='submit' value='Continue' />&nbsp;&nbsp;");
        echo("</td>\n");
	echo("</tr>\n");

    echo("</table>");
echo("</form>");
echo("<table width='100%' cellpadding='5' cellspacing='0'>");

	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>\n");
	echo("</tr>\n");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("If you are a <b class='instruct'>student</b>, please contact ");
		echo("Netlink to retrieve your password.  The Netlink website is located at <a href='http://netlink.uvic.ca' target='_blank'>netlink.uvic.ca</a>");
		echo("</td>\n");
	echo("</tr>\n");

	echo("<tr>");
		echo("<td colspan='2' align='center'><hr width='50%' /></td>\n");
	echo("</tr>");
	
	echo("<tr align='center'>");
		echo("<td colspan='2' align='center'>");
            echo("<form method='post' action='" . $PHP_SELF . "'>");
                echo("<input type='hidden' name='select' value='logout' />");
                echo("<input type='submit' value='Return to Login' />");
            echo("</form>\n");
        echo("</td>\n");
	echo("</tr>\n");

	echo("</table>\n");
	echo("</td>\n");
echo("</tr>\n");
echo("</table>\n");

?>
