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
 | Filename: verify_delete_contact.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This is the confirmation screen that the user has to go through |
 |              before deleting the contact.                                    |
 +------------------------------------------------------------------------------+

*/

// Input: $contact_id - The contact id of the contact we want to delete.
assert(isInteger($contact_id));

$sql = ("
    SELECT c.first_name, c.last_name
    FROM contact c
    INNER JOIN contact_employer ce
    ON c.contact_id = ce.contact_id
    WHERE c.contact_id = '".addslashes($contact_id)."'
    ");
$results = $GLOBALS['dbh']->Execute($sql);

if ($results->RecordCount() == 1)
{
    $row = $results->FetchRow();
    $message = "Are you sure you want to delete ".$row['first_name']." ".$row['last_name']." from the system?</b>";
    $message .= "<table border='0' cellpadding='0' cellspacing='0'>";
    $message .= "<tr><td><br />";
    $message .= "<form method='post' name='delete_contact' action='".$PHP_SELF."'>";
    $message .= "<table border='0' cellpadding='0' cellspacing='0'>";
    $message .= "<tr><td>";
    $message .= "<input type='hidden' name='contact_id' value='".$contact_id."' />";
    $message .= "<input type='hidden' name='select' value='delete_contact' />";
    $message .= "<input type='submit' value='Delete Contact' />";
    $message .= "</td><td>";
    $message .= "&nbsp;&nbsp;";
    $message .= "</td><td>";
    $message .= "<input type='button' name='cancel' value='Cancel' onclick='javascript:document.cancel_form.submit()' />";
    $message .= "</td></tr>";
    $message .= "</table>";
    $message .= "</form>";
    $message .= "</td></tr>";
    $message .= "</table><b>&nbsp;";
}
else
{
    $message = "An error has occurred while retrieving the contact's information. Please try again.";
}

// Start our presentation layer

echo("</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");
echo("<h3 align='left'>Employer Information - Delete Contact</h3>");
if ($results->RecordCount() == 1)
{
    instruct($message);
}
else 
{
    error($message);
}
    
echo("<form method='post' name='cancel_form' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='contact_id' value='".$contact_id."' />");
echo("<input type='hidden' name='select' value='edit_contact' />");
echo("<input type='hidden' name='level1' value='contact' />");
echo("</form>");

?>
