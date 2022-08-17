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
 | Filename: traffic_flag_header.inc                                            |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/

$URL = ($PHP_SELF . "&amp;select=flag_editor");
?>
Modify Flags:</td>
<td width="5">&nbsp;</td>
<td valign='middle'>&nbsp;<a href='<?php echo($URL . "&amp;level1=add"); ?>'><b class='blue'>Add</b></a>&nbsp;</td>
<td width='5'>&nbsp;</td>
<td valign='middle'>&nbsp;<a href='<?php echo($URL . "&amp;level1=edit_delete"); ?>'><b class='blue'>Edit/Delete</b></a>&nbsp;</td>
<td width='5'>&nbsp;</td>
<td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" /></td>
</tr>
</table>
<br />

<?php 

switch($level1)
{
    case "edit_delete":
        echo("<h3>Edit/Delete Flags</h3>");
    break;

    case "add":
        echo("<h3>Add a New Flag</h3>");
    break;
}
?>
          
