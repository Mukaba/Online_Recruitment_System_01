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
 | Filename: add_company_header.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This is just the header file for adding a company.  It opens    |
 | and closes the table that all the forms are displayed in, and it displays    |
 | "Add a new company" on the screen.                                           |
 +------------------------------------------------------------------------------+

*/


if ($continue == 'Add Company Now')
{
	include('contact/add/company/add_company.inc');
}
else
{
	// They've finished adding a department, so we need to display the header with the quickmenu in it.
	echo("<table border='0' cellspacing='0' cellpadding='2' width='97%'>");
	echo("<tr>");
		echo("<td>");
		echo("<h3 align='left'>Employer Information - Add a New Company</h3>");
		
		include('contact/add/company/add_company.inc');
	
		echo("</td>");
	echo("</tr>");
	echo("</table>");
}

?>
