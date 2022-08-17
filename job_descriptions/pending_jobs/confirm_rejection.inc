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
 | Filename: confirm_rejection.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This page confirms that the user would like to reject the       |
 | company and contact information (along with the job description).            |
 +------------------------------------------------------------------------------+

*/

echo("<form method='post' action='" . $PHP_SELF . "' name='form'>");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='job_id' VALUe='" . $job_id . "' />");

echo("<table border='0' cellspacing='0' cellpadding='0' border='0'>");
echo("<tr class='row1'>");
	echo("<td>");
	echo("<table width='100%' cellpadding='5' cellspacing='0'>");
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("You are about to reject the company and contact information for <b class='black'>" . $pendingEmployer->contact_first_name . " " 
		. $pendingEmployer->contact_last_name . "</b> at <b class='black'>" . $pendingEmployer->company_name . "</b>.  This information, along with the
		job description they have entered in the database will be deleted.  Please confirm that this is what you would like to do
		below.");
		echo("</td>");
	echo("</tr>");

	echo("<tr align='center'>");
		echo("<td colspan='2' align='center'>");
		echo("<input type='hidden' name='continue' value='' />");
		echo("<input type='submit' value='Cancel Rejection' />");
		echo("&nbsp;&nbsp;");
		echo("<input type='submit' name='continue' value='Confirm Rejection' />");
                echo("</form>");
                echo("</td>");
	echo("</tr>");

	echo("</table>");
	echo("</td>");
echo("</tr>");
echo("</table>");

?>
