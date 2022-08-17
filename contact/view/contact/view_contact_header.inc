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
 | Filename: view_contact_header.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This is the header file for viewing a contact.  It opens and    |
 | closes the table that the rest of the information is displayed in, and it    |
 | determines whether or not we need to give users a choice of                  |
 | companies/divisions/contacts to view and choose from, or to display the      |
 | contact's information.                                                       |
 +------------------------------------------------------------------------------+

*/

echo("<table border='0' width='99%' cellpadding='2'>");
echo("<tr>");
echo("<td align='left'>");
if($btnSearch == "Save Search")
{
    echo("<h3 align='left'>Employer Information - Contacts Save Search</h3>");
}
else
{
    echo("<h3 align='left'>Employer Information - View Contacts</h3>");
}
echo("</td>");

if ($show_quick && $contact_id)
{
	$contact = new EditContact('');
	$contact->loadContactValues($contact_id);

    // Include the quickmenu
    echo("<td align='right'>");
        include('contact/contact_quickmenu.inc');
    echo("</td>");
}

echo("</tr>");
echo("</table>");
	
include('contact/view/contact/view_contact.inc');


?>
