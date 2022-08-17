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
 | Filename: preferences_faculty.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This page redirects faculty users to the applicable             |
 | preference page.                                                             |
 +------------------------------------------------------------------------------+

*/

?>
Preferences:
</td>
<td width="10">&nbsp;</td>
<td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=prefer&amp;level1=contact"); ?>"><b class="blue">Contact Info</b></a>&nbsp;</td>
<td width="10">&nbsp;</td>
<td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=prefer&amp;level1=scheme"); ?>"><b class="blue">Colour Scheme</b></a>&nbsp;</td>
<td width="10">&nbsp;</td>

<td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>
<table border="0" cellspacing="0" cellpadding="10" width="100%"><tr><td>

<?php
switch ($level1)
{
   case "scheme":
      include("preferences/personal/scheme.inc");
      break;
      
   case "contact":
   default:
      include("preferences/personal/contact.inc");
      break;
}

?>
</td></tr></table>

