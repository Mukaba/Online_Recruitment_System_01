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
 | Filename: confirm_force.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This just prompts the user to make sure they want to force      |
 | the html out of a builder                                                    |
 +------------------------------------------------------------------------------+

*/

echo ("<form action='$PHP_SELF' method='post' name='form_continue'>");
echo ("<input type='hidden' name='select' value='resume' />");
echo ("<input type='hidden' name='page' value='edit' />");
echo ("<input type='hidden' name='resume_id' value='$resume_id' />");


echo ("<table border='1' cellspacing='0' cellpadding='0' class='error'>");
echo ("<tr>");

	echo ("<td align='center'>");

	echo ("<table width='100%' cellpadding='5' cellspacing='0'>");
	echo ("<tr>");
		echo ("<td align='center'>");
		echo ("By forcing this Resume from the Builder into HTML, <br />");
		echo ("you will not be able to return to using the Builder for this Resume.<br /><br />");
		echo ("Would you like to continue?");
		echo ("</td>");
	echo ("</tr>");

	echo ("<tr>");
		echo ("<td align='center'>");
		echo ("<input type='submit' name='force_html_confirmed' value='Continue' />");
		echo ("&nbsp; &nbsp;");
		echo ("<input type='submit' name='force_html_cancelled' value='Cancel' />");
		echo ("</td>");
	echo ("</tr>");
	echo ("</table>");

	echo ("</td>");
echo ("</tr>");
echo ("</table>");

echo ("</form>");
?>
