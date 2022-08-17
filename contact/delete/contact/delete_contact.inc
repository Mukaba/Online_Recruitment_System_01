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
 | Filename: delete_contact.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This page sets the delete flag for the contact.                 |
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
    $delete_sql = ("
        UPDATE contact_employer
        SET deleted_flag = '1'
        WHERE contact_id = '".addslashes($contact_id)."'
        ");
    $GLOBALS['dbh']->Execute($delete_sql);

    $row = $results->FetchRow();
    $message = $row['first_name']." ".$row['last_name']." has been deleted from the system.";
}
else
{
    $message = "An error has occurred while deleting the contact. Please try again.";
}

// Start our presentation layer

echo("</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");
echo("<h3 align='left'>Employer Information - Delete Contact</h3>");
if ($results->RecordCount() == 1)
{
    notify($message);
}
else 
{
    error($message);
}

echo("<br />");
echo("<div align='center'>");
echo("<a href='".$PHP_SELF."&amp;select=view_contact&amp;level1=contact'>Back to View Contact</a>");
echo("</div>");
    
?>
