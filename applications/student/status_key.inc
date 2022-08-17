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
 | Filename: status_key.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of all the statuses and their keys.             |
 +------------------------------------------------------------------------------+

*/

?>
<br />
<h4>Status Icon Key:</h4>

<table cellspacing="0" cellpadding="0" border="1">
<tr>
<td>
<table border="0" cellpadding="2">
<tr>
        <td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>
        <td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>
</tr>
<tr>
        <td class='row0d' align='center'>&nbsp;<img src='misc/images/doc10.gif' alt='' />&nbsp;</td>
        <td class='row0d' align='center'>&nbsp;<?php echo (getStudentStatusName("1")); ?>&nbsp;</td>
</tr>
<tr>
	<td class='row1d' align='center'>&nbsp;<img src='misc/images/cancel.gif' alt='' />&nbsp;</td>
        <td class='row1d' align='center'>&nbsp;<?php echo (getStudentStatusName("2")); ?>&nbsp;</td>
</tr>
<tr>
        <td class='row0d' align='center'>&nbsp;<img src='misc/images/Warning.gif' alt='' />&nbsp;</td>
        <td class='row0d' align='center'>&nbsp;Applications pulled by admin&nbsp;</td>
</tr>
<tr>
        <td class='row1d' align='center'>&nbsp;<img src='misc/images/job_shortlist.gif' alt='' />&nbsp;</td>
        <td class='row1d' align='center'>&nbsp;<?php echo (getStudentStatusName("3")); ?>&nbsp;</td>
</tr>
<tr>
        <td class='row0d' align='center'>&nbsp;<img src='misc/images/clock.gif' alt='' />&nbsp;</td>
        <td class='row0d' align='center'>&nbsp;<?php echo (getStudentStatusName("4")); ?>&nbsp;</td>
</tr>
<tr>
        <td class='row1d' align='center'>&nbsp;<img src='misc/images/smchk_gr.gif' alt='' />&nbsp;</td>
        <td class='row1d' align='center'>&nbsp;<?php echo (getStudentStatusName("5")); ?>&nbsp;</td>
</tr>
<tr>
        <td class='row0d' align='center'>&nbsp;<img src='misc/images/smchk_yl.gif' alt='' />&nbsp;</td>
        <td class='row0d' align='center'>&nbsp;<?php echo (getStudentStatusName("6")); ?>&nbsp;</td>
</tr>
<tr>
	<td class='row1d' align='center'>&nbsp;<img src='misc/images/X.gif' alt='' />&nbsp;</td>
        <td class='row1d' align='center'>&nbsp;<?php echo (getStudentStatusName("7")); ?>&nbsp;</td>
</tr>
<tr>
        <td class='row0d' align='center'>&nbsp;<img src='misc/images/you_applied.gif' alt='' />&nbsp;</td>
        <td class='row0d' align='center'>&nbsp;You have applied to this job&nbsp;</td>
</tr>
<tr>
	<td class='row1d' align='center'>&nbsp;<img src='misc/images/your_app_has_been_pulled.gif' alt='' />&nbsp;</td>
	<td class='row1d' align='center'>&nbsp;Your application was added by admin&nbsp;</td>
</tr>
<tr>
        <td class='row0d' align='center'>&nbsp;<img src='misc/images/you_have_been_shortlisted.gif' alt='' />&nbsp;</td>
        <td class='row0d' align='center'>&nbsp;You have been shortlisted&nbsp;</td>
</tr>
<tr>
        <td class='row1d' align='center'>&nbsp;<img src='misc/images/you_have_an_interview.gif' alt='' />&nbsp;</td>
        <td class='row1d' align='center'>&nbsp;You have an interview&nbsp;</td>
</tr>
<tr>
        <td class='row0d' align='center'>&nbsp;<img src='misc/images/you_have_been_placed.gif' alt='' />&nbsp;</td>
        <td class='row0d' align='center'>&nbsp;You have been placed&nbsp;</td>
</tr>
<tr>
        <td class='row1d' align='center'>&nbsp;<img src='misc/images/no_more_progress.gif' alt='' />&nbsp;</td>
        <td class='row1d' align='center'>&nbsp;You did not get this job&nbsp;</td>
</tr>

</table>
</td>
</tr>
</table>

