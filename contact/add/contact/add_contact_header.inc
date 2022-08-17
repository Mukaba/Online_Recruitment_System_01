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
 | Filename: add_contact_header.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This is just the header file for adding a contact.  It opens    |
 | and closes the table that all the forms are displayed in, and it displays    |
 | "Add a new contact" on the screen.                                           |
 +------------------------------------------------------------------------------+

*/


if ($continue == 'Add Contact Now')
{
	include('contact/add/contact/add_contact.inc');
}
else
{
	echo("<table border='0' cellspacing='0' cellpadding='2' width='97%'>");
	echo("<tr>");
		echo("<td>");
		echo("<h3 align='left'>Employer Information - Add a New Contact");
		echo("</h3>");
	
		if ($employer_id)
		{
			$sql = ("
				SELECT DISTINCT company_name
				FROM employer_company
				WHERE employer_id='" . $employer_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			$company_name = $row["company_name"];
		}
		elseif ($contact->contact_employer_id)
		{
			$sql = ("
				SELECT DISTINCT company_name
				FROM employer_company
				WHERE employer_id='" . $contact->contact_employer_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			$company_name = $row["company_name"];
		}
	
		if ($department_id)
		{
			$sql = ("
				SELECT DISTINCT department_name
				FROM employer_department
				WHERE department_id='" . $department_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			$department_name = $row["department_name"];
		}
		elseif ($contact->contact_department_id)
		{
			$sql = ("
				SELECT DISTINCT department_name
				FROM employer_department
				WHERE department_id='" . $contact->contact_department_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			$department_name = $row["department_name"];
		}
		
		
		include('contact/add/contact/add_contact.inc');
	
		echo("</td>");
	echo("</tr>");
	echo("</table>");
}

?>
