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
 | Filename: view.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This is the main file that directs the appropriate view page    |
 | to the user.                                                                 |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
?>

	       View By : </td>
               <td width="10">&nbsp;</td>
               <td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=view&amp;level1=date"); ?>"><b class="blue">Date</b></a>&nbsp;</td>
               <td width="10">&nbsp;</td>
               <td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=view&amp;level1=week"); ?>"><b class="blue">Week</b></a>&nbsp;</td>
               <td width="10">&nbsp;</td>
               <td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=view&amp;level1=term"); ?>"><b class="blue">Term</b></a>&nbsp;</td>
               <td width="10">&nbsp;</td>
               <td nowrap='nowrap' valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=view&amp;level1=job_code"); ?>"><b class="blue">Job Code(s)</b></a>&nbsp;</td>
               <td width="10">&nbsp;</td>
               <td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=view&amp;level1=company"); ?>"><b class="blue">Company</b></a>&nbsp;</td>
               <td width="10">&nbsp;</td>
               <td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=view&amp;level1=student"); ?>"><b class="blue">Student</b></a>&nbsp;</td>
               <td width="10">&nbsp;</td>

		<td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>

	<table border="0" cellspacing="0" cellpadding="2" width="99%"><tr><td>

<?php
switch ($level1)
{

   case "job_code":
      include("interview/view/list/view_by_job_code.inc");
      break;

   case "term":
      include("interview/view/list/view_by_term.inc");
      break;

   case "student":
      include("interview/view/list/view_by_student.inc");
      break;

   case "company":
      include("interview/view/list/view_by_company.inc");
      break;
   case "week":
      include("interview/view/list/view_by_week.inc");
      break;
   case "email_students":
      include("interview/edit/mail/send_shortlisted_students_email2.inc");
      break;
   case "date":
   default:
      include("interview/view/list/view_by_date.inc");
      break;
}

?>
</td></tr></table>

