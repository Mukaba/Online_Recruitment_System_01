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
 | Filename: view_applications.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This page displays all the applications for a given job code.   |
 |  The employer can view all the applicants or just selected applicants.       |
 | Currently they can only view the PDF version of the applications.            |
 +------------------------------------------------------------------------------+

*/
if ($page == "View All Applications"){
	error ("Unknown Error");
}elseif($page == "View Selected Applications"){
	error ("Please select which applications you wish to view");
}

$sql = "SELECT job_id
	FROM contact_job
	WHERE job_id = '" . addslashes($job_id) . "' AND contact_id = '" . addslashes($auth->contact_id). "'";
$result = $GLOBALS['dbh']->Execute($sql);
	
if ( $job_id == "" || !$result || $result->RecordCount() != 1){
	error( "Invalid Job. " );
	include ("coverletters/employer/view_main.inc");

}else{
	$sql = "SELECT job_code 
		FROM job_info 
		WHERE job_id = " . addslashes(stripslashes($job_id));
	$result = $GLOBALS['dbh']->Execute( $sql );
	$row =  $result->FetchRow();
	$job_code = $row["job_code"];
		
		
	$sql = "SELECT CONCAT(s.first_name, ' ', s.last_name) as name, c.student_number as student_number
		FROM coverletter as c, student as s
		WHERE c.student_number = s.student_number AND c.job_id = " . addslashes(stripslashes($job_id)) . " 
			AND c.status_id= " . ACTIVE;

	$result = $GLOBALS['dbh']->Execute( $sql );
	$numrows =  $result->RecordCount();

	if ($numrows == 0){
		?>
		<h3>View Applications</h3>
		<b class="black">No applications exist for this job.</b>
		<?php
	}else{
		?>
		<h3><?php echo $job_code?> Applicants</h3>
		<table class='backgrey' border="0" cellpadding="5" cellspacing="0">
		  <tr>
		    <td>
		      <h4>Application Information:</h4>
		      Below is a list of students who have applied to this job.
		      <ul>
		      <li>To view all students' applications click on "View All Applications"
		      button.</li>
		      <li>To view a single applicant simple click on their name.</li>
		      <li>To view a group of applicants, click on the check box next to
		      their name, and when you have selected all the applicants you wish to view click on the "View Selected Applications" button located at the end of the list of students.</li>
		      </ul>
		    </td>
		  </tr>
		</table>
		<form method="post" activate="<?php echo $PHP_SELF?>">
		<input type="hidden" name="select" value="coverletter" />
		<input type="hidden" name="job_id" value="<?php echo $job_id?>" /> 
		<input type="hidden" name="PDF" value="1" />
		<input type="submit" name="page" value="View All Applications" />
		<br /><br />
		<table border="1">
		  <tr>
		    <td class="rowgrey"><b class="white">&nbsp;</td>
		    <td class="rowgrey"><b class="white">Student Name</td>
		  </tr>
		  <?php
		  $i = 0;
		  while($row =  $result->FetchRow()){
			  ($i % 2 == 0) ? $class = "row0d" : $class = "row1d";
			  $i ++;
			  $href = $PHP_SELF .  "&amp;select=coverletter&amp;page=viewapplication&amp;PDF=1&amp;job_id=" . urlencode($job_id) . "&amp;student=" . urlencode($row["student_number"]);
		  ?>
			  <tr>
			    <td class="<?php echo $class?>"><input type="checkbox" class="<?php echo $class?>" name="student[]" value="<?php echo $row["student_number"]?>" /></td>
		<input type="submit" name="page" value="View Selected Applications" />
		</form>
			    <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["name"] == "") ? "&nbsp;" : $row["name"])?></a></td>
			  </td>
		<?php
		  }
		  ?>
		</table>
		<br />
		<input type="submit" name="page" value="View Selected Applications" />
		</form>
		<?php
	}
}
?>
