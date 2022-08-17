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
 | Filename: delete_main.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Seems to be the header file for deleting resumes (?)            |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Resume - Delete</h3>
<form method="post" action="<?php echo ($PHP_SELF);?>">
<input type="hidden" name="select" value="resume" />
<input type="hidden" name="page" value="view" />
<table border="0" width="100%" cellpadding="5" cellspacing="0">
  <tr>
    <td align="center">
      <table width="100%" class="row1" cellspacing="0" cellpadding="5" border="0">
        <tr>
	  <td colspan="2" align="left" class="tableheaders"> Enter a Student Number</td>
	</tr>
	<tr>
          <td align="right">Student Number:</td>
          <td align="left"><input type="text" size="10" name="student_num" /></td>
        </tr>
	<tr>
          <td colspan="2" align="center" width="25%"><input type="submit" name="page" value="View & Delete" /></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
</form>
