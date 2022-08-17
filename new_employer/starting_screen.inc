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
 | Filename: starting_screen.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This is the starting point for a new employer.  All this does   |
 | is display a message welcoming them, and then a button for them to click on  |
 | to get started.                                                              |
 +------------------------------------------------------------------------------+

*/

echo("<table border='0' cellspacing='0' cellpadding='20' width='100%'>");
echo("<tr>");
	echo("<td>");
	echo("<br /><br />");

	echo("<h2 align='center'>Co-op at ".ORGANIZATION_FULL."</h2>");
	echo("<br />");

	echo("<br />");

	echo("<p align='center'>Welcome to ".BRAND_NAME.", the information system for the Co-op programs at ".ORGANIZATION_FULL.".</p>");

	echo("<br />");

	echo("<p align='center'> Since you are a first-time MyCo-op user who would like to post a job with UVic Co-op, you will need to set 
        up an employer account by providing some company and contact information. Once you enter your contact information and a job posting, 
        UVic will provide you with a ".BRAND_NAME." UserID and Password.<br /><br />
        The Company and Contact information forms -- on the next page -- will only take a few minutes to fill in; if you are short on time, 
        you can fill in only the fields marked with an asterisk (*). Program staff will contact you at a later date for the rest of the 
        information.
        </p>");

	echo("<br />");
	echo("<p align='center'>Click on the 'Begin' button to start setting up a new employer account and post a job.</p>");
	echo("</td>\n");
echo("</tr>\n");
echo("<tr>");
	echo("<td align='center'>");

	echo("<table>");
	echo("<tr>");
		echo("<td align='right'>");

		echo("<form method='post' action='" . $PHP_SELF . "'>");
		echo("<input type='hidden' name='select' value='new_employer' />");
		echo("<input type='hidden' name='continue' value='collect_contact_information' />");
		
		echo("<input type='submit' value='Begin' />");

		echo("</form>");
		
		echo("</td>\n");
		echo("<td align='left'>");
		
		echo("<form method='post' action='" . $PHP_SELF . "'>");
		echo("<input type='hidden' name='select' value='logout' />");
		
		echo("<input type='submit' value='Return to Login' />");
		
		echo("</form>");

		echo("</td>\n");
	echo("</tr>\n");
	echo("</table>\n");

	echo("</td>\n");
echo("</tr>\n");
echo("</table>\n");

?>
