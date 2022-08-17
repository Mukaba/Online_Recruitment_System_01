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
 | Filename: search_application_main.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: Displays a form allowing the user to search the applications    |
 | of the selected job.                                                         |
 +------------------------------------------------------------------------------+

*/
?>

<form method="post" action="<?php echo $PHP_SELF?>">
<h3>Search Applicants</h3>
<input type="hidden" name="select" value="coverletter" />
<input type="hidden" name="job_id" value="<?php echo $job_id?>" />
<input type="hidden" name="page" value="Search" />
<table border="1">
  <tr>
    <td>
      <table border="0" width="100%" cellspacing="0" cellpadding="5">
        <tr>
	  <td>Search: </td>
	  <td valign="top"><input type="radio" name="search_select[]" value="coverletter">/Cover Letters<br />
	      <input type="radio" name="search_select[]" value="resume" />Resumes<br />
	  </td>
	  <td valign="top">
	      <input type="radio" name="search_select[]" value="transcript" />Transcripts</td>
	  </td>
	  <td rowspan="3">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
	</tr>
	<tr>
	  <td>For: </td>
	  <td colspan="2"><input type="text" name="search_item" size="30" /></td>
	</tr>
	<tr>
	  <td colspan="3">Note: Enter commas between the words you wish to search for.  The search will return true if anyone of the key words you enter is found.</td>
	</tr>
	<tr>
	  <td colspan="3"><input type="submit" name="page" value="Search"></td>
	</tr>
      </table>
    </td>
  </tr>
</table>
</form>

