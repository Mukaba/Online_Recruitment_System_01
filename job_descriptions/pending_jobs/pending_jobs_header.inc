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
 | Filename: pending_jobs_header.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This file displays the appropriate header across the top of     |
 | the screen.                                                                  |
 +------------------------------------------------------------------------------+

*/

$URL = "$PHP_SELF&amp;select=pending_jobs";

if ($continue != 'view_contact_company_info' && $continue != 'view_job_info')
{
	echo("</td>");
	echo("<td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' />");
	echo("</td>");
	echo("</tr>");
	echo("</table>");
}
else
{
	echo("View :</td>");

	echo("<td width='5'>&nbsp;</td>");
	echo("<td valign='middle'>&nbsp;<a href='" . $URL . "&amp;continue=view_contact_company_info&amp;job_id=" . $job_id . "'>");
	echo("<b class='blue'>Contact/Company Info</b></a>&nbsp;</td>\n");
	
	echo("<td width='5'>&nbsp;</td>");
	echo("<td valign='middle'>&nbsp;<a href='" . $URL . "&amp;continue=view_job_info&amp;job_id=" . $job_id . "'>");
	echo("<b class='blue'>Job Info</b></a>&nbsp;</td>\n");
	
	echo("<td width='5'>&nbsp;</td>");
	echo("<td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td>");
	echo("</tr>");
	echo("</table>");
}

echo("<h3>Job Descriptions - Incoming Jobs</h3>");

echo("<table border='0' width='96%'>");
echo("<tr>");
	echo("<td>");

	include('job_descriptions/pending_jobs/pending_jobs.inc');
	
	echo("</td>");
echo("</tr>");
echo("</table>");

?>
