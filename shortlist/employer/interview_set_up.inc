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
 | Filename: interview_set_up.inc                                               |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/

/*
 Filename: interview_set_up.inc
 Description: Once the employer has selected which students to shortlist, the shortlist is saved to the database and a form requesting
 interview set up information is provied to the employer.  The employer can elect to enter information in this form
 and press the submit button.  Once that is completed the form is e-mailed to the contact of the job.
 :NOTE: This file is now deprecated and should be removed from the next release.
 */

include("shortlist/employer/shortlist.class");
// $sql = "SELECT job_id FROM contact_job WHERE job_id = '" . $job_id . "' AND contact_id = '" . $auth->contact_id . "'";
//echo $sql;

$sql = "select job_id from job_info where job_id=\"$job_id\" and contact_id=\"".$auth->contact_id."\"";
//echo $sql;

$result = $GLOBALS['dbh']->Execute($sql);
if ($job_id == "" || $result->RecordCount()<=0 || !is_array($students) || (is_array($students) && sizeof($students) < 0)){
	error ("You must select at least one student");
	include ("shortlist/employer/shortlist.inc");
}else{
	$short_list = new shortlist($students, $job_id);
	$short_list->save_students();

	$result = $GLOBALS['dbh']->Execute("SELECT job_code FROM job_info WHERE job_id = '" . $job_id . "'");
	$row = $result->FetchRow();
	$job_code = $row["job_code"];
	?>
	<h3>Interview Set Up</h3>
	<h4>Job Code: <?php echo $job_code?></h4>
	<form method="post" action="<?php echo $PHP_SELF?>">
	<input type="hidden" name="SELECT" value="shortlist" />
	<input type="hidden" name="short_list" value="<?php echo packObject(($short_list))?>" />
	<input type="hidden" name="level" value="3" />
	<table width="100%" border="0" class='backgrey' cellpadding="5">
	  <tr>
	    <td><h4>Shortlist Submitted</h4>
	    	The following students have been shortlisted for this job:<br />
		<?
		for($i=0;$i<count($short_list->student_names);$i++){
			echo $short_list->student_names[$i] . "<br />";
		}
	   	?>
	    </td>
	  </tr>
	</table>
	<br />
	<table border="0" width="100%" class='backgrey'  cellpadding="5">
	  <tr>
	    <td>
	    	<h4>Interview Set Up Information</h4>
	    	In the following text box please enter all interview specific information needed to set up an interview for you.  Please include interview dates,
		start times, interview length, lunch time, break times and any other important information needed to set up an interview.<br />Once the interview has been
		set up you will be e-mailed a copy of the schedule or you can view the schedule by selected the View Interview Schedule menu option.
	    </td>
	  </tr>
	</table>
	<br />
	<table border="0">
	  <tr>
	    <td>Interview Information:</td>
	  </tr>
	  <tr>
	    <td><textarea name="interview_info" cols="70" rows="6"></textarea></td>
	  </tr>
	</table>
	<input type="submit" value="Submit Information" />
	</form>

<?
}
?>
