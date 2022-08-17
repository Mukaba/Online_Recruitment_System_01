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
 | Filename: add_main.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: Displays a form where the user can enter a job code and         |
 | students names (or a single student), select to user their generic cover     |
 | letter and resume, and add the coverletter to the system and apply the       |
 | student to that job.                                                         |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Add Cover Letter</h3>

<form method="post" action="<?php echo ($PHP_SELF)?>">
<input type="hidden" name="select" value="coverletter" />
<input type="hidden" name="department_id" value="0" />

<table width="100%" border="0" cellpadding="3" cellspacing="0" class="row1">
  <tr>
    <td>Job Code:</td>
    <td><input type="text" name="job_code" size="20" /></td>
  </tr>
  <tr>
    <td>Student Names:</td>
    <td><textarea name="student" cols="40" rows="5"></textarea></td>
  </tr>
  <tr>
    <td colspan="2">
      <input type="checkbox" name="use_generic" checked='checked' />Use Generic Cover Letter & Resume<br />
    </td>
  <tr>
    <td colspan="2"><input type="submit" name="page" value="Add Cover Letter" /></td>
  </tr>
</table>
</form>
