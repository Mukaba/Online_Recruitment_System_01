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
 | Filename: division_quickmenu.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display the division quickmenu             |
 | consistently.  Each of the available options is displayed in a pull down     |
 | menu, and should be accessible from any screen that has a particular         |
 | division record open (eg, after adding a division, editing a division,       |
 | viewing a division...).  This may result in redundant options, such as an    |
 | option to edit a division when you're already doing so, but this is          |
 | acceptable to allow for greater consistency.                                 |
 |                                                                              |
 | This file needs the following variables set before being called:             |
 | $department_id                                                               |
 +------------------------------------------------------------------------------+

*/


$sql = ("
    SELECT employer_id
    FROM employer_department
    WHERE department_id='" . $department_id . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$quick_employer_id = $row["employer_id"];

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='PDF' value='1' />");
echo("<input type='hidden' name='department_id' value='" . $department_id . "' />");
echo("<input type='hidden' name='employer_id' value='" . $quick_employer_id . "' />");

echo("<select name='select'>");
echo("<option value='view_this_division'>View this Division</option>");
echo("<option value='view_this_company'>View this Division's Company</option>");
echo("<option value='view_department_contacts'>View this Division's Contacts</option>");
echo("<option value='view_division_job'>View this Division's Job Descriptions</option>");

if ($auth->userlevel == OFFICE)
{
    echo("<option value='view_division_history'>View this Division's Hiring History</option>");
}

echo("<option value='view_company_job'>View Parent Company's Job Descriptions</option>");
echo("<option value='add_contact_to_division'>Add a Contact to this Division</option>");
echo("<option value='add_job_under_division'>Add a Job for this Division</option>");
echo("<option value='edit_department'>Edit this Division</option>");
echo("<option value='edit_company'>Edit this Division's Company</option>");
echo("</select>");

echo("&nbsp;&nbsp;");
echo("<input type='image' src='misc/images/arrowg.gif' alt='go' />");
echo("</form>");

