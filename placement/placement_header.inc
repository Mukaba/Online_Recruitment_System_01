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
 | Filename: placement_header.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This is the header for all placement pages. It includes two     |
 |              links in the headers.                                           |
 +------------------------------------------------------------------------------+

*/
?>
&nbsp;
</td>
<td width='5'>&nbsp;</td>
<td valign="middle">
    <b class='black'>Place by:</b>&nbsp;
    <a href="javascript:document.place_by_term.submit()"><b class="blue">Term&nbsp;</b></a>
</td>
<td valign='middle'>&nbsp;
    <a href='javascript:document.place_by_student.submit()'><b class='blue'>Student</b></a>
</td>
<td>
<form name="place_by_term" action="<?php echo($PHP_SELF); ?>" method="post">
    <input type="hidden" name="select" value="placement_by_term" />
    <input type='hidden' name='menu_select' value='<?php echo $menu_select?>' />
</form>
<form name='place_by_student' action='<?php echo($PHP_SELF); ?>' method='post'>
    <input type='hidden' name='select' value='placement_by_student' />
    <input type='hidden' name='menu_select' value='<?php echo $menu_select?>' />
</form>
</td>

<td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>
