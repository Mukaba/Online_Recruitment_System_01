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
 | Filename: set_header.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description: This file lets the user choose to set either actions or flags   |
 | for given contacts.                                                          |
 +------------------------------------------------------------------------------+

*/


$URL = "$PHP_SELF&amp;select=set_contact_flag";
?>

        	Set : </td>
            <td width='5'>&nbsp;</td>
            <td valign="middle">&nbsp;<a href="<?php echo($URL . "&amp;level1=actions"); ?>"><b class="blue">Actions</b></a>&nbsp;</td>
            <td width='5'>&nbsp;</td>
<?php

	$sql = ("
		SELECT DISTINCT *
		FROM department_contact_flags
		WHERE department_id='" . $auth->department . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{
		echo("<td valign='middle'>&nbsp;<a href='" . $URL . "&amp;level1=flags'><b class='blue'>Flags</b></a>&nbsp;</td>");
            	echo("<td width='5'>&nbsp;</td>");
	}

// TASK: Add a quickmenu in this area, so that users can return from setting flags and actions.

?>
	    <td align='left'><img src='<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>' alt='' /></td> 
         </tr>
        </table>
