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
 | Filename: contact_quickmenu.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display the contact quickmenu              |
 | consistently.  Each of the available options is displayed in a pull down     |
 | menu, and should be accessible from any screen that has a particular         |
 | contact record open (eg, after adding a contact, editing a contact, viewing  |
 | a contact...).  This may result in redundant options, such as an option to   |
 | edit a contact when you're already doing so, but this is acceptable to       |
 | allow for greater consistency.                                               |
 |                                                                              |
 | This file needs the following variables set before being called:             |
 | $contact_id                                                                  |
 +------------------------------------------------------------------------------+

*/


$sql = ("
    SELECT employer_id, department_id
    FROM contact_employer
    WHERE contact_id='" . $contact_id . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$quick_employer_id = $row["employer_id"];
$quick_department_id = $row["department_id"];

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='PDF' value='1' />");
echo("<input type='hidden' name='contact_id' value='".$contact_id."' />");
// This contact_id_list is for the set actions page. 
echo("<input type='hidden' name='contact_id_list' value='".$contact_id."' />");
echo("<input type='hidden' name='employer_id' value='".$quick_employer_id."' />");
echo("<input type='hidden' name='department_id' value='".$quick_department_id."' />");

echo("<select name='select'>");
echo("<option value='view_this_contact'>View this Contact</option>");

if ($quick_department_id && $quick_department_id != 'none')
{
    echo("<option value='view_this_division'>View this Contact's Division</option>");
}

echo("<option value='view_this_company'>View this Contact's Company</option>");
echo("<option value='view_contact_job'>View this Contact's Job Descriptions</option>");

if ($userlevel != TRAFFICDIRECTOR && $quick_department_id && $quick_department_id != 'none')
{
    echo("<option value='view_division_history'>View Parent Division's Hiring History</option>");
}
echo("<option value='view_division_job'>View Parent Division's Job Descriptions</option>");
if ($userlevel != TRAFFICDIRECTOR){
    echo("<option value='view_company_history'>View Parent Company's Hiring History</option>");
}
echo("<option value='view_company_job'>View Parent Company's Job Descriptions</option>");
echo("<option value='add_job_under_contact'>Add a Job for this Contact</option>");
echo("<option value='edit_contact'>Edit this Contact</option>");
if ($quick_department_id && $quick_department_id != 'none')
{
    echo("<option value='edit_department'>Edit this Contact's Division</option>");
}
echo("<option value='edit_company'>Edit this Contact's Company</option>");

echo("<option value='set_actions'>View/Set Actions for this Contact</option>");

$sql = ("
    SELECT DISTINCT *
    FROM department_contact_flags
    WHERE department_id='" . $auth->department . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
if ($result->RecordCount())
{
    echo("<option value='set_flags'>Set Flags for this Contact</option>");
}

echo("<option value='create_lp'>Create Login Info for Contact</option>");
echo("</select>");

echo("&nbsp;&nbsp;");
echo("<input type='image' src='misc/images/arrowg.gif' alt='go' />");
echo("</form>");
?>
